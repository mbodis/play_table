#ifndef SENSORMULTI_H
#define SENSORMULTI_H

#include <Arduino.h>

/*
 * multi
 * plays one tone based on distance from sensor
 */
class SensorMulti : public Sensor {
	private:
		// bool arr to know what note is on
		bool noteIsOn[18];
		// current selected note setup
		byte notesValue[18];

		// total notes count
		byte notesCount;

		//
		byte thresholdCount;

		//
		byte thresholdValues[18];

	public:

		/*
		 * id <byte> - id of a sensor
		 * notesSize <byte> - number of notes
		 * notesArr <byte[]> - sensor multi - note values
		 * threshSize <byte> - size of threshold array
		 * thresholdArr <byte[]> - threshold values
		 *
		 * NOTE: threshold example: {20, 60, 120} ->
		 *  thhreshold level 1: <20-60>
		 *  thhreshold level 2: <60-120>
		 *  thhreshold level 3: <120,999>
		 */
		SensorMulti(byte id, byte notesCount, byte notesArr[], byte threshSize, byte thresholdArr[]): Sensor(id, TONE_MODE_MULTI, id) {

			this->notesCount = notesCount;

			for (byte i = 0; i < notesCount; i++) {
				notesValue[i] = notesArr[i];
				noteIsOn[i] = false;
			}

			thresholdCount = threshSize;
			for (byte i = 0; i < thresholdCount; i++) {
				thresholdValues[i] = thresholdArr[i];
			}

		}

		virtual void sensorOff(MyMidi &mMyMidi) {
			for (byte note = 0; note < notesCount; note++) {
				noteIsOn[note] = false;
				mMyMidi.noteOff(getChannel(), notesValue[note], mMyMidi.velocity);
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


			for (byte note = 0; note < notesCount; note++) {
				bool inRange = false;

				// loop through thresholh values
				for (byte th = 0; th < thresholdCount; th++) {

					if (note == th) {
						// threhold in middle
						if (th + 1 < notesCount) {
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
				if (noteIsOn[note] == false && inRange) {
					noteIsOn[note] = true;
					mMyMidi.noteOn(getChannel(), notesValue[note], mMyMidi.velocity);

					// note OFF
				}
				else if (noteIsOn[note] == true && !inRange) {
					noteIsOn[note] = false;
					mMyMidi.noteOff(getChannel(), notesValue[note], mMyMidi.velocity);
				}

				// note pressure - disabled, because tones are changing by distance
			}
		}
};


#endif