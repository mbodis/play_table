#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

/*
 * Abstract class for sensor
 */
class Sensor : public AutoMode, public DynamicMode {
	private:
		byte sensorId = -1;
		byte mode = -1;
		byte channel = 0;
		byte volume = 0;

	public:

		Sensor(byte sensorId, byte mode, byte channel, bool dynamicMode = false): AutoMode(), DynamicMode(dynamicMode) {
			this->sensorId = sensorId;
			this->mode = mode;
			this->channel = channel;
		}

		virtual void sensorOff(MyMidi&);
		virtual void playNote(MyMidi&, byte, byte);

		byte getVolume() {
			return this->volume;
		}

		void setVolume(byte volume) {
			this->volume = volume;
		}

		/*
		 * toggle note setup helper for visualization
		 */
		byte getSensorId() {
			return this->sensorId;
		}

		byte visualLedsHelper = 0;

		void setSensorId(byte id) {
			sensorId = id;
		}

		byte getMode() {
			return mode;
		}

		void setMode(byte newMode) {
			mode = newMode;
		}

		byte getVisualLedHelper() {
			return visualLedsHelper;
		}

		void setVisualLedHelper(byte vh) {
			visualLedsHelper = vh;
		}

		byte getChannel() {
			return channel;
		}

		/*
		 * toggle note setup helper for visualization method
		 */
		void incVisualLedHelper(byte inc) {
			visualLedsHelper += inc;
			if (visualLedsHelper > 255) {
				visualLedsHelper = 255;
			}
		}

};


#endif