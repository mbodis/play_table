#ifndef SENSORSINGLE_H
#define SENSORSINGLE_H

#include <Arduino.h>

/*
 * single
 * play single tones anytime sensor range is reached
 */ 
class SensorSingle : public Sensor{
	private:
		// to know if note is on
	    bool noteIsOn = false;

	    // note value
	    byte noteValue = 0;

  	public:

		/*
	     * id <byte> - id of a sensor
	     * noteValue <byte> - sensor single tone note value
	     */
  		SensorSingle(byte id, byte noteValue):Sensor(id, TONE_MODE_SINGLE){
	      this->noteValue = noteValue;
	      this->noteIsOn = false;  
  		}

        virtual void sensorOff(MyMidi &mMyMidi){
			noteIsOn = false;
			mMyMidi.noteOff(0, noteValue, mMyMidi.velocity);
        }

        /*
	     *
	     * byte thresholdRaw <0 - 50>
	     * byte thresholdFiltered <2-255>
	     */
        virtual void playNote(MyMidi &mMyMidi, byte thresholdRaw, byte thresholdFiltered){
        	byte pressure = thresholdFiltered/1.5;
			if (pressure > 127){
				pressure = 127;
			}

	      
			// note ON
			if (noteIsOn == false && thresholdRaw > 2) {
				noteIsOn = true;
				mMyMidi.noteOn(0, noteValue, mMyMidi.velocity);

			// note OFF  
			} else if (noteIsOn == true && thresholdRaw < 2) {
				noteIsOn = false;
				mMyMidi.noteOff(0, noteValue, mMyMidi.velocity);    
			}

			// note pressure
			if (noteIsOn){
				mMyMidi.afterTouch(0, noteValue, pressure);
			}
	        
        }
};


#endif