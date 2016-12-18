#ifndef SENSORMULTICHORD_H
#define SENSORMULTICHORD_H

#include <Arduino.h>

/*
 * multi chorda
 * plays multiple tones based on distance from sensor
 */
class SensorMultiChord : public Sensor {
	private:

		// number of chords
		byte chordCount;

		// to know if note is on
		bool noteIsOn[3];

		// arr of note values
		byte noteValue[3][6];

		// total notes count
		byte notesCount;

		// size of threshold array
		byte thresholdCount;

		// threshold sensor array
		byte thresholdValues[18];

	public:

		/*
		 * id <byte> - id of a sensor
		 * notesSize <byte> - number of notes that are played at same time
		 * notesArr <byte[][]> - array of note values
		 * threshSize <byte> - size of threshold array
		 * thresholdArr <byte[]> - threshold values
		 *
		 * NOTE: if you want to play less notes just duplicate the value
		 * NOTE: threshold example: {20, 60, 120} ->
		 *  thhreshold level 1: <20-60>
		 *  thhreshold level 2: <60-120>
		 *  thhreshold level 3: <120,999>
		 */
		template< size_t N >
		SensorMultiChord(byte id, byte chordCount, byte notesCount, byte notesArr[N][N],
		                 byte threshSize, byte thresholdArr[])
			: Sensor(id, TONE_MODE_MULTI_CHORD, id) {

			this->chordCount = chordCount;
			this->notesCount = notesCount;

			for (byte i = 0; i < chordCount; i++) {

				noteIsOn[i] = false; // chord is default off

				for (byte j = 0; j < notesCount; j++) {
					noteValue[i][j] = notesArr[i][j];
				}
			}

			thresholdCount = threshSize;
			for (byte i = 0; i < thresholdCount; i++) {
				thresholdValues[i] = thresholdArr[i];
			}

		}

		virtual void sensorOff(MyMidi &mMyMidi) {
			for (byte chord = 0; chord < notesCount; chord++) {
				noteIsOn[chord] = false;

				for (byte note = 0; note < notesCount; note++) {
					mMyMidi.noteOff(getChannel(), noteValue[chord][note], mMyMidi.velocity);
				}
			}
		}

		/*
		 *
		 * byte thresholdRaw <0 - 50>
		 * byte thresholdFiltered <2-255>
		 */
		virtual void playNote(MyMidi &mMyMidi, byte thresholdRaw, byte thresholdFiltered) {
			byte pressure = thresholdFiltered / 1.5;
			if (pressure > 127) {
				pressure = 127;
			}


			for (byte chord = 0; chord < chordCount; chord++) {
				bool inRange = false;

				// loop through thresholh values
				for (byte th = 0; th < thresholdCount; th++) {
					if (chord == th) {
						// threhold in middle
						if (th + 1 < thresholdCount) {
							if (thresholdFiltered >= thresholdValues[th] && thresholdFiltered < thresholdValues[th + 1]) {
								inRange = true;
							}
							// last threshold
						}
						else {
							if (thresholdFiltered >= thresholdValues[th]) {
								inRange = true;
							}
						}
					}
				}

				// note ON
				if (noteIsOn[chord] == false && inRange) {
					noteIsOn[chord] = true;
					for (byte note = 0; note < notesCount; note++) {
						mMyMidi.noteOn(getChannel(), noteValue[chord][note], mMyMidi.velocity);
					}

					// note OFF
				}
				else if (noteIsOn[chord] == true && !inRange) {
					noteIsOn[chord] = false;
					for (byte note = 0; note < notesCount; note++) {
						mMyMidi.noteOff(getChannel(), noteValue[chord][note], mMyMidi.velocity);
					}
				}

				// note pressure - disabled, because tones are changing by distance
			}
		}
};

#endif