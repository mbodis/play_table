#ifndef SENSORARPEGGIOCHORD_H
#define SENSORARPEGGIOCHORD_H

#include <Arduino.h>

/*
 * arpeggio
 * toggle between tones, increase tempo by distance
 * type chord, keep playing previous tones
 */
class SensorArpeggioChord : public Sensor {
	private:
		// arpeggio counter
		byte arpCount = 0;

		// bool arr to know what note is on
		bool noteIsOn[42];

		// current selected note setup
		byte notesValue[42];

		// total notes count
		byte notesCount;

		byte tempoLimit;

	public:

		/*
		 * id <byte> - id of a sensor
		 * notesSize <byte> - number of notes that are played at same time
		 * notesArr <byte[]> - array of note values
		 *
		 */
		SensorArpeggioChord(byte id, byte notesCount, byte notesArr[]): Sensor(id, TONE_MODE_ARPEGGIO, id) {

			this->notesCount = notesCount;

			for (byte i = 0; i < notesCount; i++) {
				notesValue[i] = notesArr[i];
				noteIsOn[i] = false;
			}

			tempoLimit = notesCount * (255 / notesCount);
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

				if (thresholdFiltered > 240) inRange = true;

				if (thresholdRaw > 2) {
					if (arpCount > note * 255 / notesCount && arpCount < 255) {
						inRange = true;
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

			if (thresholdRaw <= 2 ) {
				arpCount = 0;
			}

			// "thresholdFiltered/10" is increasing faster than "thresholdFiltered/20"
			thresholdFiltered = (thresholdFiltered < 40) ? 40 : thresholdFiltered;
			arpCount += thresholdFiltered / 40;

			if (arpCount >= tempoLimit) {
				arpCount = arpCount%tempoLimit;
			}
		}

		byte getArpCount() {
			return arpCount;
		}
};


#endif