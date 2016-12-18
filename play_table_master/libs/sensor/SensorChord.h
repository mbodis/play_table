#ifndef SENSORCHORD_H
#define SENSORCHORD_H

#include <Arduino.h>

/*
 * chord
 * play multiple tones anytime sensor range is reached
 */
class SensorChord : public Sensor {
	private:
		// to know if note is on
		bool noteIsOnChord = false;

		// arr of note values
		byte noteValues[6];

		// number of notes
		byte notesCount;

	public:

		/*
		 * id <byte> - id of a sensor
		 * notesSize <byte> - number of notes that are played at same time
		 * notesArr <byte[]> - array of note values
		 *
		 */
		SensorChord(byte id, byte notesSize, byte notesArr[]): Sensor(id, TONE_MODE_CHORD, id) {
			noteIsOnChord = false;
			notesCount = notesSize;
			for (byte i = 0; i < notesCount; i++) {
				noteValues[i] = notesArr[i];
			}
		}

		virtual void sensorOff(MyMidi &mMyMidi) {
			for (byte note = 0; note < notesCount; note++) {
				mMyMidi.noteOff(getChannel(), noteValues[note], mMyMidi.velocity);
			}
			noteIsOnChord = false;
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

			setVolume(pressure);

			// note ON
			if (noteIsOnChord == false && thresholdRaw > 2) {
				noteIsOnChord = true;
				for (byte note = 0; note < notesCount; note++) {
					mMyMidi.noteOn(getChannel(), noteValues[note], mMyMidi.velocity);
				}

				// note OFF
			}
			else if (noteIsOnChord == true && thresholdRaw < 2) {
				noteIsOnChord = false;
				for (byte note = 0; note < notesCount; note++) {
					mMyMidi.noteOff(getChannel(), noteValues[note], mMyMidi.velocity);
				}
			}

			// note pressure
			// if (noteIsOnChord == true){
			// 	for (byte note = 0; note < notesCount; note++) {
			// 		mMyMidi.afterTouch(getChannel(), noteValues[note], pressure);
			// 	}
			// }

		}
};


#endif