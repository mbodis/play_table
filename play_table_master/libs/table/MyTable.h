#ifndef MY_TABLE_H
#define MY_TABLE_H

#include <Arduino.h>

/*
 *
 */
class MyTable {

	private:
		byte buttonState = LOW;

	public:
		byte noteSetupSelectedIdx = -1;
		float electrodeLastValueRaw[ELECTRODES_COUNT] = { 0, 0, 0, 0, 0, 0, 0}; // 0 - 50
		int electrodeLastValues[ELECTRODES_COUNT] = {0, 0, 0, 0, 0, 0, 0}; // 2-255
		byte selectedMode = PLAY_MODE_TONES;
		Sensor *mTableSensors[ELECTRODES_COUNT];
		NoteGroup *mNoteGroup;

		MyTable() {
			mNoteGroup = new NoteGroup();
		}

		void setupButton() {
			pinMode(PIN_BUTTON_TABLE_BOTTOM_SIDE, INPUT);
		}

		void toggleTones(byte velocity, MyMidi &mMyMidi) {
			if (digitalRead(PIN_BUTTON_TABLE_BOTTOM_SIDE) == HIGH && buttonState == LOW) {
				buttonState = HIGH;
				selectedMode = (selectedMode + 1) % MODE_COUNT;

				// play beeps to know what  mode is on
				for (byte i = 0; i < selectedMode + 1; i++) {
					mMyMidi.noteOn(0, 60, velocity);
					delay(500);
					mMyMidi.noteOff(0, 60, velocity);
				}

			}
			else {
				buttonState = LOW;
			}
		}

		void setVolumeByPotentiometer(MyMidi &mMyMidi) {
			int volume = analogRead(PIN_KNOB_VOLUME);
			volume = abs(1000 - volume) / 8;

			int vv = 0;
			// Serial.print("volume:");
			// Serial.println(volume);

			for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {

				if (mTableSensors[elc]->isDynamicModeEnable()) {
					if (mTableSensors[elc]->getMode() == TONE_MODE_SINGLE) {
						vv = (volume - 15) + (mTableSensors[elc]->getVolume() / 6);
						if (vv <= 0) vv = 1;
						if (vv > 127) vv = 127;

						mMyMidi.setVolume(elc, vv);

					}
					else if (mTableSensors[elc]->getMode() == TONE_MODE_CHORD) {

						vv = (volume - 20) + (mTableSensors[elc]->getVolume() / 4);
						if (vv <= 0) vv = 1;
						if (vv > 127) vv = 127;

						mMyMidi.setVolume(elc, vv);

					}else{
						mMyMidi.setVolume(elc, mTableSensors[elc]->getVolume());
					}
				}
				else {
					mMyMidi.setVolume(elc, volume);
				}

			}
		}

		void setupNotes(MyMidi &mMyMidi, byte newNoteSetupIdx, bool playTones) {

			if (playTones) {
				// play beeps to know what  mode is on
				for (byte i = 0; i < newNoteSetupIdx + 1; i++) {
					mMyMidi.noteOn(0, 60, mMyMidi.velocity);
					delay(250);
					mMyMidi.noteOff(0, 60, mMyMidi.velocity);
				}
			}
			NoteSetup::setupNotes(newNoteSetupIdx, mMyMidi, mTableSensors, mNoteGroup);
		}

		/*
		 * chord and multi note mode
		 */
		void setMidiNotesByPotentiometer(MyMidi &mMyMidi) {
			// v == <44, 922>
			int v = abs(1000 - analogRead(PIN_KNOB_NOTES));

			// Serial.print("knob: ");
			// Serial.println(v);

			byte newNotesIdx = -1;
			if (v < 90) {
				newNotesIdx = 0;
			}
			else if (v > 210 && v < 290) {
				newNotesIdx = 1;
			}
			else if (v > 385 && v < 480) {
				newNotesIdx = 2;
			}
			else if ( v > 586 && v < 658) {
				newNotesIdx = 3;
			}
			else if ( v > 742) {
				newNotesIdx = 4;
			}
			else {
				newNotesIdx = noteSetupSelectedIdx;
			}

			// initial state
			if (noteSetupSelectedIdx == -1) {
				noteSetupSelectedIdx = newNotesIdx;
				setupNotes(mMyMidi, noteSetupSelectedIdx, false);

				// toggle by user
			}
			else if (newNotesIdx != -1 && newNotesIdx != noteSetupSelectedIdx) {
				noteSetupSelectedIdx = newNotesIdx;
				turnOffAllNotes(mMyMidi);
				setupNotes(mMyMidi, noteSetupSelectedIdx, true);
				VisualEffects::initScreenSaverSensorType(mTableSensors);
			}
		}

		/*
		 * switching to another mode
		 */
		void turnOffAllNotes(MyMidi &mMyMidi) {
			for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {
				mTableSensors[elc]->sensorOff(mMyMidi);
			}
		}

		void playSelectedMode(Themerin &mThemerin, MyMidi &mMyMidi) {
			// play tones
			if (selectedMode == PLAY_MODE_TONES) {
				playToneMode(mMyMidi);
			}
			else if (selectedMode == PLAY_MODE_THEMERIN) {
				mThemerin.playThereminMode(mMyMidi);
			}
		}

		/*
		 *
		 * loop all touch sensors and play mode
		 *
		 */
		void playToneMode(MyMidi &mMyMidi) {

			// TOME MODE GROUP
			if (mNoteGroup->groupMode) {
				playToneModeGroups(mMyMidi);

				// TONE MODE NORMAL
			}
			else {
				playToneModeNormal(mMyMidi);
			}
		}

		/*
		 *
		 */
		void playToneModeNormal(MyMidi &mMyMidi) {
			for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {
				mTableSensors[elc]->playNote(mMyMidi, (byte)electrodeLastValueRaw[elc], electrodeLastValues[elc]);
			}
		}

		/*
		 *
		 */
		void playToneModeGroups(MyMidi &mMyMidi) {
			// update status
			mNoteGroup->changeSensorStatus(electrodeLastValueRaw);
			for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {

				// play by status - automatic mode
				if (mTableSensors[elc]->hasAutoMode()
				        && !mTableSensors[elc]->isLoopingEnabled()
				        && mNoteGroup->sensorStatus[elc] == SENSOR_STATE_HOVER) {
					// turn OFF all sensors from same group
					for (byte elc2 = 0; elc2 < ELECTRODES_COUNT; elc2++) {
						if (mNoteGroup->groupIds[elc] == mNoteGroup->groupIds[elc2] && elc2 != elc) {
							mTableSensors[elc2]->sensorOff(mMyMidi);
						}
					}
				}

				// play by status - simple hover
				if (mNoteGroup->sensorStatus[elc] == SENSOR_STATE_HOVER
				        || mNoteGroup->sensorStatus[elc] == SENSOR_STATE_HAND_ABOVE
				        || mNoteGroup->sensorStatus[elc] == SENSOR_STATE_RELEASING ) {

					if (!mTableSensors[elc]->isLoopingEnabled()) {
						mTableSensors[elc]->playNote(mMyMidi, (byte)electrodeLastValueRaw[elc], electrodeLastValues[elc]);
					}

					// stop by status - simple
				}
				else if (mNoteGroup->sensorStatus[elc] == SENSOR_STATE_START_BLOCKING_BY_GROUP) {
					mTableSensors[elc]->sensorOff(mMyMidi);
				}

				// play auto mode
				if (mTableSensors[elc]->isLoopingEnabled()) {
					mTableSensors[elc]->playNote(mMyMidi, (byte)electrodeLastValueRaw[elc], electrodeLastValues[elc]);
				}
			}
		}

		/*
		 * init communication
		 */
		void setupSerialCommunication() {
			Serial.begin(9600);
			Serial1.begin(9600);
		}

		/*
		 * 0-11 electrodes
		 * 30-41 IDs
		 */
		void sendValuesToSlaves() {
			for (byte i = 0; i < ELECTRODES_COUNT; i++) {

				// auto mode
				if (mTableSensors[i]->getMode() == TONE_MODE_ARPEGGIO
				        && mTableSensors[i]->isLoopingEnabled()) {
					MyCommunicator::sendMessageToSlave(i + SLAVE_FIRST_ID, static_cast<SensorArpeggio*>(mTableSensors[i])->getArpCount() );

					// default value from sensor
				}
				else {
					MyCommunicator::sendMessageToSlave(i + SLAVE_FIRST_ID, electrodeLastValues[i]);
				}
			}

		}
};
#endif