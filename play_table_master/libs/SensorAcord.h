#ifndef SENSORACORD_H
#define SENSORACORD_H

#include <Arduino.h>

/*
 * acord
 * play multiple tones anytime sensor range is reached
 */     
class SensorAcord : public Sensor{
	private:
	    // to know if note is on
	    bool noteIsOnAcord = false;
	    
	    // arr of note values
	    byte noteValueAcord[6];

		// number of notes	  
	    byte notesCount;
   
  	public:

  		/*
	     * id <byte> - id of a sensor
	     * notesSize <byte> - number of notes that are played at same time
	     * notesArr <byte[]> - array of note values      
	     *
	     */
	    SensorAcord(byte id, byte notesSize, byte notesArr[]):Sensor(id, TONE_MODE_ACORD){
			noteIsOnAcord = false;
			notesCount = notesSize;       
			for (byte i=0; i<notesCount; i++){
				noteValueAcord[i] = notesArr[i];
			}
	    }

        virtual void sensorOff(MyMidi &mMyMidi){
			for (byte note = 0; note < notesCount; note++) {
				mMyMidi.noteOff(0, noteValueAcord[note], mMyMidi.velocity);         
			}
			noteIsOnAcord = false;
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
			if (noteIsOnAcord == false && thresholdRaw > 2) {         
				noteIsOnAcord = true;
				for (byte note = 0; note < notesCount; note++) {
					mMyMidi.noteOn(0, noteValueAcord[note], mMyMidi.velocity);      
			}

			// note OFF  
			} else if (noteIsOnAcord == true && thresholdRaw < 2) {
				noteIsOnAcord = false;      
				for (byte note = 0; note < notesCount; note++) {
					mMyMidi.noteOff(0, noteValueAcord[note], mMyMidi.velocity);         
				}
			}

			// note pressure
			if (noteIsOnAcord == true){
				for (byte note = 0; note < notesCount; note++) {
					mMyMidi.afterTouch(0, noteValueAcord[note], pressure);
				}
			}

		}
};


#endif