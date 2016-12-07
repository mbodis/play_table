#ifndef SENSORARPEGGIO_H
#define SENSORARPEGGIO_H

#include <Arduino.h>


/*
 * arpeggio
 * toggle between tones, increase tempo by distance
 */
class SensorArpeggio : public Sensor {
	private:
		// arpeggio counter
		byte arpCount = 0;

		// bool arr to know what note is on
		bool noteIsOnMulti[18];

		// current selected note setup
		byte notesValueMulti[18];

		// total number of notes
		byte notesCount;

		// looping part
		byte tempo = DEFAULT_TEMPO;
		byte lastDistanceSensorValues[HAND_DISATNCE_SAVED_VALUES];
		byte lastDistanceSensorValuesIdx = 0;
		byte userTouchSensorLocked = 0;

	public:

		/*
		 * id <byte> - id of a sensor
		 * notesSize <byte> - number of notes that are played at same time
		 * notesArr <byte[]> - array of note values
		 *
		 */
		SensorArpeggio(byte id, byte notesCount, byte notesArr[]): Sensor(id, TONE_MODE_ARPEGGIO, id) {

			this->notesCount = notesCount;

			for (byte i = 0; i < notesCount; i++) {
				notesValueMulti[i] = notesArr[i];
				noteIsOnMulti[i] = false;
			}
		}

		virtual void sensorOff(MyMidi &mMyMidi) {
			for (byte note = 0; note < notesCount; note++) {
				noteIsOnMulti[note] = false;
				mMyMidi.noteOff(getChannel(), notesValueMulti[note], mMyMidi.velocity);
			}
		}

		/*
		 *
		 * byte thresholdRaw <0 - 50>
		 * byte thresholdFiltered <2-255>
		 */
		virtual void playNote(MyMidi &mMyMidi, byte thresholdRaw, byte thresholdFiltered) {

			if (userTouchSensorLocked > 0) {
				userTouchSensorLocked--;
				return;
			}

			for (byte note = 0; note < notesCount; note++) {
				bool inRange = false;

				// if too close just play acord
				// if (thresholdFiltered > 235) inRange = true;

				if (thresholdRaw > 2) {

					// play only one note at time
					if ((arpCount > (note * (255 / notesCount)))
					        && (arpCount < ((note + 1) * (255 / notesCount))) ) {
						inRange = true;

					}
				}

				// note ON
				if (noteIsOnMulti[note] == false && inRange) {
					noteIsOnMulti[note] = true;
					mMyMidi.noteOn(0, notesValueMulti[note], mMyMidi.velocity);

					// note OFF
				}
				else if (noteIsOnMulti[note] == true && !inRange) {
					noteIsOnMulti[note] = false;
					mMyMidi.noteOff(0, notesValueMulti[note], mMyMidi.velocity);
				}
			}

			if (thresholdRaw <= 2 ) {
				arpCount = 0;
			}

			// "thresholdFiltered/10" is increasing faster than "thresholdFiltered/20"
			// thresholdFiltered/40 - best
			thresholdFiltered = (thresholdFiltered < 40) ? 40 : thresholdFiltered;
			arpCount += thresholdFiltered / 40;

			if (arpCount == 255) {
				arpCount = 0;
			}
		}

		byte getArpCount() {
			return arpCount;
		}

};


#endif