#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor {
	private:
  		
    
  	public:
  		byte sensorId = -1;
    	byte mode = -1;
        
        Sensor(byte id, byte sMode){
        	sensorId = id;
        	mode = sMode;
        }

        virtual void sensorOff(MyMidi&);
        virtual void playNote(MyMidi&, byte, byte);
        
        /*
	     * toggle note setup helper for visualization
	     */
	    byte getSensorId(){
	      	return sensorId;
	    }

	    byte visualLedsHelper = 0;

		void setSensorId(byte id){
			sensorId = id;
		}
	    
	    byte getMode(){
			return mode;
	    }
	      
	    void setMode(byte newMode){
			mode = newMode;
	    }

	    byte getVisualLedHelper(){
	    	return visualLedsHelper;
	    }
	    
	    void setVisualLedHelper(byte vh){
	    	visualLedsHelper = vh;
	    }

	    /*
	     * toggle note setup helper for visualization method
	     */
        void incVisualLedHelper(byte inc){
			visualLedsHelper += inc;
			if (visualLedsHelper > 255){
				visualLedsHelper = 255;
			}
	    }
};


#endif