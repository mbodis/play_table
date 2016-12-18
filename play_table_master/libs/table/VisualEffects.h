#ifndef VISUAL_EFFECTS_H
#define VISUAL_EFFECTS_H

#include <Arduino.h>

class VisualEffects {

	private:

	public:

		static void initScreenSaver() {
			for (byte loop = 0; loop < 2; loop++) {
				int val = 0;
				while (true) {
					for (int ele = SLAVE_FIRST_ID; ele <= SLAVE_FIRST_ID + ELECTRODES_COUNT; ele++) {
						MyCommunicator::sendMessageToSlave(ele, val);
					}
					delay(25);
					val += 10;
					if (val > 255)break;
				}

				val = 255;
				while (true) {
					for (byte ele = SLAVE_FIRST_ID; ele <= SLAVE_FIRST_ID + ELECTRODES_COUNT; ele++) {
						MyCommunicator::sendMessageToSlave(ele, val);
					}
					delay(25);
					val -= 10;
					if (val < 0)break;
				}
			}
		}

		static void initScreenSaver2() {
			int val = 0;

			while (true) {
				MyCommunicator::sendMessageToSlave(SLAVE_FIRST_ID + 0, (val + 15) % 255);
				MyCommunicator::sendMessageToSlave(SLAVE_FIRST_ID + 1, (val + 30) % 255);
				MyCommunicator::sendMessageToSlave(SLAVE_FIRST_ID + 2, (val + 45) % 255);
				MyCommunicator::sendMessageToSlave(SLAVE_FIRST_ID + 3, (val + 60) % 255);
				MyCommunicator::sendMessageToSlave(SLAVE_FIRST_ID + 4, (val + 75) % 255);
				MyCommunicator::sendMessageToSlave(SLAVE_FIRST_ID + 5, (val + 90) % 255);
				MyCommunicator::sendMessageToSlave(SLAVE_FIRST_ID + 6, (val + 105) % 255);
				delay(25);
				val += 10;
				if (val > 2000)break;
			}
		}

		static void initScreenSaverSensorType(Sensor *mTableSensors[]) {

			for (byte sensor = 0; sensor < ELECTRODES_COUNT; sensor++) {
				mTableSensors[sensor]->setVisualLedHelper(2);
			}

			for (byte n = 0; n < 13; n++) {
				for (byte sensor = 0; sensor < ELECTRODES_COUNT; sensor++) {

					/*
					* SINGLE TONE MODE
					*/
					if (mTableSensors[sensor]->getMode() == TONE_MODE_SINGLE) {
						MyCommunicator::sendMessageToSlave(mTableSensors[sensor]->getSensorId() + SLAVE_FIRST_ID, 128);
					}

					/*
					* CHORD TONE MODE
					*/
					if (mTableSensors[sensor]->getMode() == TONE_MODE_CHORD) {
						MyCommunicator::sendMessageToSlave(mTableSensors[sensor]->getSensorId() + SLAVE_FIRST_ID, 255);
					}

					/*
					* MULTI TONE
					*/
					if (mTableSensors[sensor]->getMode() == TONE_MODE_MULTI) {
						mTableSensors[sensor]->incVisualLedHelper(9);
						MyCommunicator::sendMessageToSlave(mTableSensors[sensor]->getSensorId() + SLAVE_FIRST_ID, mTableSensors[sensor]->getVisualLedHelper());
					}

					/*
					* CHORD MULTI TONE
					*/
					if (mTableSensors[sensor]->getMode() == TONE_MODE_MULTI_CHORD) {
						mTableSensors[sensor]->incVisualLedHelper(18);
						MyCommunicator::sendMessageToSlave(mTableSensors[sensor]->getSensorId() + SLAVE_FIRST_ID, mTableSensors[sensor]->getVisualLedHelper());
					}

					/*
					* ARPEGGIO
					*/
					if (mTableSensors[sensor]->getMode() == TONE_MODE_ARPEGGIO) {
						mTableSensors[sensor]->incVisualLedHelper(18);
						if (n % 2 == 0) {
							MyCommunicator::sendMessageToSlave(mTableSensors[sensor]->getSensorId() + SLAVE_FIRST_ID, 10);
						}
						else {
							MyCommunicator::sendMessageToSlave(mTableSensors[sensor]->getSensorId() + SLAVE_FIRST_ID, mTableSensors[sensor]->getVisualLedHelper());
						}
					}

				}
				//wait some time after loop all sensors
				delay(150);
			}
		}
};


#endif