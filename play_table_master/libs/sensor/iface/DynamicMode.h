#ifndef DYNAMICMODE_H
#define DYNAMICMODE_H

#include <Arduino.h>

/*
 * Abstract class for dynamic mode
 */
class DynamicMode {
	private:
		bool dynamicMode = false;

	public:
		DynamicMode(bool dynamicMode) {
			this->dynamicMode = dynamicMode;
		}

		bool isDynamicModeEnable() {
			return this->dynamicMode;
		}
};


#endif