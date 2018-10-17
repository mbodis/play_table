#ifndef TOUCH_SENSORS_H
#define TOUCH_SENSORS_H

#include <Arduino.h>

#define LOW_DIFF 0
#define HIGH_DIFF 50
#define FILTER_WEIGHT 0.3f // 0.0f to 1.0f - higher value = more smoothing
#define MPR121_ADDR 0x5C

class TouchSensors {
	private:

	public:

		void updateReading() {
			MPR121.updateAll();
		}

		void readValuesFromElectrodes(MyTable &mMyTable) {
			for (byte i = 0; i < ELECTRODES_COUNT; i++) {
				readElectrodeValue(mMyTable, ELECTRODES_ARR[i]);
			}

		}

		void readElectrodeValue(MyTable &mMyTable, byte electrode) {

			// read the difference between the measured baseline and the measured continuous data
			int reading = MPR121.getBaselineData(electrode) - MPR121.getFilteredData(electrode);

			// print out the reading value for debug
			//  Serial.print("Proximity electrode: ");
			//  Serial.println(reading);

			// constrain the reading between our low and high mapping values
			int prox = constrain(reading, LOW_DIFF, HIGH_DIFF);

			// implement a simple (IIR lowpass) smoothing filter
			float newProx = (FILTER_WEIGHT * mMyTable.electrodeLastValueRaw[electrode]) + ((1 - FILTER_WEIGHT) * (float)prox);

			//newProx  - is now between 0 - 50
			mMyTable.electrodeLastValueRaw[electrode] = newProx;

			// save values for slaves
			if ((int)newProx > 0) {
				// software hack - some sensor are
				if (electrode == 0 || electrode == 2 || electrode == 4 || electrode == 6){
					mMyTable.electrodeLastValues[electrode] = (int)(newProx * 5.5 * 3); // value is now between 0-255
				}else{
					mMyTable.electrodeLastValues[electrode] = (int)(newProx * 5.5); // value is now between 0-255
				}
			}
			else {
				mMyTable.electrodeLastValues[electrode] = (int)newProx;
			}

			// do some limit filtering
			if (mMyTable.electrodeLastValues[electrode] > 255) {
				mMyTable.electrodeLastValues[electrode] = 255;
			}
			if (mMyTable.electrodeLastValues[electrode] < 2) {
				mMyTable.electrodeLastValues[electrode] = 2;
			}

		}

		void setupTouchElectrodes() {
			if (!MPR121.begin(MPR121_ADDR)) {
				Serial.println("error setting up MPR121");
				switch (MPR121.getError()) {
				case NO_ERROR:
					Serial.println("no error");
					break;
				case ADDRESS_UNKNOWN:
					Serial.println("incorrect address");
					break;
				case READBACK_FAIL:
					Serial.println("readback failure");
					break;
				case OVERCURRENT_FLAG:
					Serial.println("overcurrent on REXT pin");
					break;
				case OUT_OF_RANGE:
					Serial.println("electrode out of range");
					break;
				case NOT_INITED:
					Serial.println("not initialised");
					break;
				default:
					Serial.println("unknown error");
					break;
				}
				while (1);
			}

			MPR121.updateAll();

		}

		void setupProximity() {
			// slow down some of the MPR121 baseline filtering to avoid
			// filtering out slow hand movements
			//  MPR121.setRegister(NHDF, 0x01); //noise half delta (falling)
			//  MPR121.setRegister(FDLF, 0x3F); //filter delay limit (falling)
		}
};

#endif