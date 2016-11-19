#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor {
	private:
  		
    
  	public:
  		byte sensorId = -1;
    	byte mode = -1;
    	byte channel = 0;
    	byte volume = 0;
        
        Sensor(byte mId, byte mMode, byte mChannel){
        	sensorId = mId;
        	mode = mMode;
        	channel = mChannel;
        }

        virtual void sensorOff(MyMidi&);
        virtual void playNote(MyMidi&, byte, byte);
        
        byte getVolume(){
        	return volume;
        }
        
        void setVolume(byte mVolume){
        	volume = mVolume;
        }

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

	    byte getChannel(){
	    	return channel;
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