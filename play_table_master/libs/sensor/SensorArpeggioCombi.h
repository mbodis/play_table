#ifndef SENSORARPEGGIOCOMBI_H
#define SENSORARPEGGIOCOMBI_H

#include <Arduino.h>

/*
 * arpeggio
 * toggle between tones, increase tempo by distance
 */
class SensorArpeggioCombi : public Sensor {
	private:
		// arpeggio counter
		byte arpCount = 0;

		// bool arr to know what note is on
		bool noteIsOnMulti[42];

		// current selected note setup
		byte notesValueMulti[42];

		// total number of notes
		byte notesCount;

		// looping part
		byte tempo = DEFAULT_TEMPO;
		byte lastDistanceSensorValues[HAND_DISATNCE_SAVED_VALUES];
		byte lastDistanceSensorValuesIdx = 0;
		byte userTouchSensorLocked = 0;

		byte tempoLimit;

	public:

		/*
		 * id <byte> - id of a sensor
		 * notesSize <byte> - number of notes that are played at same time
		 * notesArr <byte[]> - array of note values
		 *
		 */
		SensorArpeggioCombi(byte id, byte notesCount, byte notesArr[]): Sensor(id, TONE_MODE_ARPEGGIO, id) {

			this->notesCount = notesCount;

			for (byte i = 0; i < notesCount; i++) {
				notesValueMulti[i] = notesArr[i];
				noteIsOnMulti[i] = false;
			}
			setAutoMode(true);
			setLooping(false);

			resetSensorDistanceValues();

			tempoLimit = notesCount * (255 / notesCount);
		}

		virtual void sensorQuiet(MyMidi &mMyMidi) {
			for (byte note = 0; note < notesCount; note++) {
				noteIsOnMulti[note] = false;
				mMyMidi.noteOff(getChannel(), notesValueMulti[note], mMyMidi.velocity);
			}
		}

		virtual void sensorOff(MyMidi &mMyMidi) {
			sensorQuiet(mMyMidi);
			setLooping(false);
			resetSensorDistanceValues();
			tempo = DEFAULT_TEMPO;
		}

		/*
		 *
		 * byte thresholdRaw <0 - 50>
		 * byte thresholdFiltered <2-255>
		 */
		virtual void playNote(MyMidi &mMyMidi, byte thresholdRaw, byte thresholdFiltered) {

			// hand is placed on the sensor - this is wrong behavior
			if (userTouchSensorLocked > 0
			        && thresholdFiltered >= 253) {
				sensorQuiet(mMyMidi);
				return;
			}

			// sensor has just changed autoMode - wait
			if (userTouchSensorLocked > 0) {
				userTouchSensorLocked--;
				return;
			}

			if (thresholdRaw > 2 ) {
				saveSensorNewDistanceValue(thresholdFiltered);
			}


			for (byte note = 0; note < notesCount; note++) {
				bool inRange = false;

				// if too close just play chord
				// if (thresholdFiltered > 235) inRange = true;

				if (thresholdRaw > 2 || isLoopingEnabled()) {

					// play only one note at time
					if ((arpCount > (note * (255 / notesCount)))
					        && (arpCount < ((note + 1) * (255 / notesCount))) ) {
						inRange = true;

					}
				}

				// note ON
				if (noteIsOnMulti[note] == false && inRange) {
					noteIsOnMulti[note] = true;
					mMyMidi.noteOn(getChannel(), notesValueMulti[note], mMyMidi.velocity);

					// note OFF
				}
				else if (noteIsOnMulti[note] == true && !inRange) {
					noteIsOnMulti[note] = false;
					mMyMidi.noteOff(getChannel(), notesValueMulti[note], mMyMidi.velocity);
				}
			}



			if (thresholdFiltered >= 253 && userTouchSensorLocked == 0) {
				setLooping(!isLoopingEnabled());
				tempo = getMedianFromLastValues();
				tempo = (tempo < 40) ? 40 : tempo;
				userTouchSensorLocked = USER_TOUCH_LOCK_LIMIT;

				if (!isLoopingEnabled()) {
					sensorOff(mMyMidi);
				}
			}

			// if loop is ON
			if (isLoopingEnabled()) {
				arpCount += tempo / 40;

				// if loop is OFF
			}
			else {
				if (thresholdRaw <= 2 ) {
					arpCount = 0;
				}

				// "thresholdFiltered/10" is increasing faster than "thresholdFiltered/20"
				// thresholdFiltered/40 - best
				thresholdFiltered = (thresholdFiltered < 40) ? 40 : thresholdFiltered;
				arpCount += thresholdFiltered / 40;

			}

			if (arpCount >= tempoLimit) {
				arpCount = arpCount%tempoLimit;
			}
		}


		void saveSensorNewDistanceValue(byte touchSensorValue) {
			lastDistanceSensorValues[lastDistanceSensorValuesIdx] = touchSensorValue;
			lastDistanceSensorValuesIdx ++;
			if (lastDistanceSensorValuesIdx == HAND_DISATNCE_SAVED_VALUES)
				lastDistanceSensorValuesIdx = 0;
		}


		void resetSensorDistanceValues() {
			for (byte i = 0; i < HAND_DISATNCE_SAVED_VALUES; i++) {
				lastDistanceSensorValues[i] = 0;
			}
			lastDistanceSensorValuesIdx = 0;
		}

		/*
		 * getMedian: sort array, return middle value
		 */
		byte getMedianFromLastValues() {
			for (byte i = 0; i < HAND_DISATNCE_SAVED_VALUES; i++) {
				if (lastDistanceSensorValues[i] == 0) {
					return DEFAULT_TEMPO;
				}
			}

			isort(lastDistanceSensorValues, HAND_DISATNCE_SAVED_VALUES);
			return lastDistanceSensorValues[HAND_DISATNCE_SAVED_VALUES / 2];
		}

		/*
		 * sorting array
		 */
		void isort(byte *a, byte n) {
			for (byte i = 1; i < n; ++i) {
				byte j = a[i];
				byte k;
				for (k = i - 1; (k >= 0) && (j < a[k]); k--) {
					a[k + 1] = a[k];
				}
				a[k + 1] = j;
			}
		}

		byte getArpCount() {
			return arpCount;
		}

};


#endif