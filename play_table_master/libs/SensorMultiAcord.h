#ifndef SENSORMULTIACORD_H
#define SENSORMULTIACORD_H

#include <Arduino.h>

/*
 * multi acord
 * plays multiple tones based on distance from sensor
 */     
class SensorMultiAcord : public Sensor{
	private:
		
	    // number of acords
	    byte acordCount;
	    
	    // to know if note is on
	    bool noteIsOn[3];
	    
	    // arr of note values
	    byte noteValue[3][6];
	    
	    // total notes count
	    byte notesCount;
	    
		// size of threshold array
	    byte thresholdCount;
	    
	    // threshold sensor array
	    byte thresholdValues[18];

  	public:

  		/*
	     * id <byte> - id of a sensor
	     * notesSize <byte> - number of notes that are played at same time
	     * notesArr <byte[][]> - array of note values
	     * threshSize <byte> - size of threshold array 
	     * thresholdArr <byte[]> - threshold values 
	     *
	     * NOTE: if you want to play less notes just duplicate the value
	     * NOTE: threshold example: {20, 60, 120} -> 
	     *  thhreshold level 1: <20-60>     
	     *  thhreshold level 2: <60-120>     
	     *  thhreshold level 3: <120,999>     
	     */
	    template< size_t N > 
	    SensorMultiAcord(byte id, byte acordCount, byte notesCount, byte notesArr[N][N], byte threshSize, byte thresholdArr[]):Sensor(id, TONE_MODE_MULTI_ACORD){

	      this->acordCount = acordCount;
	      this->notesCount = notesCount;
	      
	      for (byte i=0; i<acordCount; i++){

	        noteIsOn[i] = false; // acord is default off
	        
	        for (byte j=0; j<notesCount; j++){
	          noteValue[i][j] = notesArr[i][j];
	        }
	      }

	      thresholdCount = threshSize;
	      for (byte i=0; i<thresholdCount; i++){
	        thresholdValues[i] = thresholdArr[i];
	      }
	      
	    }

        virtual void sensorOff(MyMidi &mMyMidi){
			for (byte acord = 0; acord < notesCount; acord++) {
				noteIsOn[acord] = false;

				for (byte note = 0; note < notesCount; note++) {
					mMyMidi.noteOff(0, noteValue[acord][note], mMyMidi.velocity);         
				}  
			}
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


			for (byte acord = 0; acord < acordCount; acord++) {
				bool inRange = false;

				// loop through thresholh values
				for (byte th = 0; th < thresholdCount; th++) {
					if (acord == th){
						// threhold in middle
						if (th+1 < thresholdCount){
							if (thresholdFiltered >= thresholdValues[th] && thresholdFiltered < thresholdValues[th+1]){
								inRange = true;
							}
						// last threshold    
						}else{                  
							if (thresholdFiltered >= thresholdValues[th]){
								inRange = true;
							}
						}
					}
				}

				// note ON
				if (noteIsOn[acord] == false && inRange) {
					noteIsOn[acord] = true;
					for (byte note = 0; note < notesCount; note++) {
						mMyMidi.noteOn(0, noteValue[acord][note], mMyMidi.velocity);         
				}              

				// note OFF  
				} else if (noteIsOn[acord] == true && !inRange) {
					noteIsOn[acord] = false;                          
					for (byte note = 0; note < notesCount; note++) {
						mMyMidi.noteOff(0, noteValue[acord][note], mMyMidi.velocity);         
					}   
				}

				// note pressure - disabled, because tones are changing by distance
			}
		}


};


#endif