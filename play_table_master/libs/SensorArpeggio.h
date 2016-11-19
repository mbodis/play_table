#ifndef SENSORARPEGGIO_H
#define SENSORARPEGGIO_H

#include <Arduino.h>

const byte HAND_DISATNCE_SAVED_VALUES = 11;

/*
 * arpeggio
 * toggle between tones, increase tempo by distance
 */   
class SensorArpeggio : public Sensor{
	private:
	    
	    // arpeggio counter
	    byte arpCount = 0;
	    
	    // bool arr to know what note is on
	    bool noteIsOnMulti[18];

	    // current selected note setup
	    byte notesValueMulti[18];

	    // total number of notes
	    byte notesCount;
	    
	    // looping part
	    bool loopIsOn = false;
	    byte tempo = 125; //default
	    byte lastTouchSensorValues[HAND_DISATNCE_SAVED_VALUES];
	    byte lastTouchSensorValuesIdx = 0;
	    byte userTouchSensorLocked = HAND_DISATNCE_SAVED_VALUES;
  	public:

  		/*
	     * id <byte> - id of a sensor
	     * notesSize <byte> - number of notes that are played at same time
	     * notesArr <byte[]> - array of note values      
	     *
	     */
	    SensorArpeggio(byte id, byte notesCount, byte notesArr[]):Sensor(id, TONE_MODE_ARPEGGIO, id){
	      
	      this->notesCount = notesCount;

	      for (byte i=0; i<notesCount; i++){
	        notesValueMulti[i] = notesArr[i];
	        noteIsOnMulti[i] = false;  
	      }

	    }


        virtual void sensorOff(MyMidi &mMyMidi){
			for (byte note = 0; note < notesCount; note++) {
				noteIsOnMulti[note] = false;            
				mMyMidi.noteOff(getChannel(), notesValueMulti[note], mMyMidi.velocity);    
			}
        }

        /*
	     *
	     * byte thresholdRaw <0 - 50>
	     * byte thresholdFiltered <2-255>
	     */
        virtual void playNote(MyMidi &mMyMidi, byte thresholdRaw, byte thresholdFiltered){			

        	if (userTouchSensorLocked > 0){
        		userTouchSensorLocked--;
        		return;
        	}

			saveTouchSensorNewValue(thresholdFiltered);


			for (byte note = 0; note < notesCount; note++) {
				bool inRange = false;            

				// if too close just play acord
				// if (thresholdFiltered > 235) inRange = true;

				if (thresholdRaw > 2 || loopIsOn){

					// play only one note at time
					if ((arpCount > (note*(255/notesCount))) 
						&& (arpCount < ((note+1)*(255/notesCount))) ){
						inRange = true;
						
					}
				}
				
				// note ON
				if (noteIsOnMulti[note] == false && inRange) {             
					noteIsOnMulti[note] = true;
					mMyMidi.noteOn(getChannel(), notesValueMulti[note], mMyMidi.velocity);

				// note OFF  
				} else if (noteIsOnMulti[note] == true && !inRange) {               
					noteIsOnMulti[note] = false;            
					mMyMidi.noteOff(getChannel(), notesValueMulti[note], mMyMidi.velocity);    
				}
			}


			
        	if (thresholdFiltered >= 253){
				loopIsOn = !loopIsOn;
				tempo = getMedianFromLastValues(); 
				userTouchSensorLocked = 10;
			}

			// if loop is ON
			if (loopIsOn){
				arpCount += tempo/40;

			// if loop is OFF
			}else{
				if (thresholdRaw <= 2 ){
					arpCount = 0;
				}

				// "thresholdFiltered/10" is increasing faster than "thresholdFiltered/20"
				// thresholdFiltered/40 - best 
				arpCount += thresholdFiltered/40;

			}

			if (arpCount == 255){
				arpCount = 0;
			}		
        }


        void saveTouchSensorNewValue(byte touchSensorValue){
        	lastTouchSensorValues[lastTouchSensorValuesIdx] = touchSensorValue;
        	lastTouchSensorValuesIdx ++;
        	if (lastTouchSensorValuesIdx == HAND_DISATNCE_SAVED_VALUES)
        		lastTouchSensorValuesIdx = 0;
        }


        /*
         * sort array, return middle value
         */
        byte getMedianFromLastValues(){
			isort(lastTouchSensorValues, HAND_DISATNCE_SAVED_VALUES);
        	return lastTouchSensorValues[HAND_DISATNCE_SAVED_VALUES/2];
        }

        /*
         * sorting array
         */
        void isort(byte *a, int n){
			for (int i = 1; i < n; ++i){
				int j = a[i];
				int k;
				for (k = i - 1; (k >= 0) && (j < a[k]); k--){
					a[k + 1] = a[k];
				}
				a[k + 1] = j;
			}
		}
    
};


#endif