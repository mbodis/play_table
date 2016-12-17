#ifndef SENSORARPEGGIOAUTO_H
#define SENSORARPEGGIOAUTO_H

#include <Arduino.h>

/*
 * arpeggio
 * toggle between tones, increase tempo by distance
 */
class SensorArpeggioAuto : public Sensor {
	private:
		// arpeggio counter
		int arpCount = 0;

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
		SensorArpeggioAuto(byte id, byte notesCount, byte notesArr[]): Sensor(id, TONE_MODE_ARPEGGIO, id) {

			this->notesCount = notesCount;

			for (byte i = 0; i < notesCount; i++) {
				notesValueMulti[i] = notesArr[i];
				noteIsOnMulti[i] = false;
			}
			setAutoMode(true);
			setLooping(false);

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

			for (byte note = 0; note < notesCount; note++) {
				bool inRange = false;

				if (isLoopingEnabled()) {

					// play only one note at time
					if ((arpCount >= (note * (255 / notesCount)))
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

			// hover to start, touch to exit
			if ( (thresholdFiltered >= 5 && !isLoopingEnabled())
				|| (thresholdFiltered >= 253 && userTouchSensorLocked == 0) ){
				arpCount = 0;
				setLooping(!isLoopingEnabled());
				userTouchSensorLocked = USER_TOUCH_LOCK_LIMIT;

				if (!isLoopingEnabled()) {
					sensorOff(mMyMidi);
				}
			}

			// if loop is ON
			if (isLoopingEnabled()) {
				arpCount += tempo / 40;

			}
			// if loop is OFF
			else {
				arpCount = 0;
			}

			if (arpCount >= tempoLimit) {
				arpCount = arpCount%tempoLimit;
			}

		}

};


#endif