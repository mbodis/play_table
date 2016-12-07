#ifndef AUTOMODE_H
#define AUTOMODE_H

#include <Arduino.h>

/*
 * Abstract class for auto mode
 */
class AutoMode {
	private:
		bool autoMode = false;
		bool loopingIsOn = false;

	public:
		AutoMode() {

		}

		bool hasAutoMode() {
			return this->autoMode;
		}

		void setAutoMode(bool autoMode) {
			this->autoMode = autoMode;
		}

		bool isLoopingEnabled() {
			return this->loopingIsOn;
		}

		void setLooping(bool loopState) {
			this->loopingIsOn = loopState;
		}
};


#endif