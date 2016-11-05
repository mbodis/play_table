#ifndef TABLE_SENSOR_H
#define TABLE_SENSOR_H
 
#include <Arduino.h>


class TableSensor {
  private:

  public:
  
    byte sensorId = -1;
    byte mode = -1;

    /*
     * single
     * play single tones anytime sensor range is reached
     */ 
    byte noteIsOnSingle = 0;
    byte noteValueSingle = 0;

    /*
     * multi
     * plays one tone based on distance from sensor
     */ 
    byte NOTES_COUNT_MULTI;
    byte THRESHOLD_COUNT_MULTI;
    byte NOTES_THRESHOLD_LEVEL_MULTI[10];// 10==max we don't use all of them
    // bool arr to know what note is on
    byte noteIsOnMulti[10];// 5==max we don't use all of them
    // current selected note setup
    byte notesValueMulti[10];// 5==max we don't use all of them

    /*
     * acord
     * play multiple tones anytime sensor range is reached
     */     
    byte NOTES_COUNT_ACORD;
    // to know if note is on
    byte noteIsOnAcord = 0;
    // current selected note setup
    byte noteValueAcord[6];// 6==max we don't use all of them
   
    /*
     * multi acord
     * plays multiple tones based on distance from sensor
     */     
    byte NOTES_COUNT_ACORD_MULTI;
    byte ACORD_COUNT_ACORD_MULTI;
    byte THRESHOLD_COUNT_ACORD_MULTI;
    byte NOTES_THRESHOLD_LEVEL_ACORD_MULTI[10];
    byte noteIsOnAcordMulti[5];// 5==max we don't use all of them
    byte noteValueAcordMulti[5][5];// 5==max we don't use all of them

    /*
     * toggle note setup helper for visualization
     */
    byte visualLedsHelper = 0;

    void setSensorId(byte id){
      sensorId = id;
    }

    byte getSensorId(){
      return sensorId;
    }
    
    byte getMode(){
      return mode;
    }
      
    void setMode(byte newMode){
      mode = newMode;
    }

    /*
     * id <byte> - id of a sensor
     * noteValue <byte> - sensor single tone note value
     */
    void setupSingle(byte id, byte noteValue){
      sensorId = id;
      mode = TONE_MODE_SINGLE;
      noteValueSingle = noteValue;
    }

    /*
     * id <byte> - id of a sensor
     * notesSize <byte> - number of notes
     * notesArr <byte[]> - sensor multi - note values 
     * threshSize <byte> - size of threshold array 
     * thresholdArr <byte[]> - threshold values 
     *
     * NOTE: threshold example: {20, 60, 120} -> 
     *  thhreshold level 1: <20-60>     
     *  thhreshold level 2: <60-120>     
     *  thhreshold level 3: <120,999>     
     */
    void setupMulti(byte id, byte notesSize, byte notesArr[], byte threshSize, byte thresholdArr[]){
      sensorId = id;
      mode = TONE_MODE_MULTI;

      NOTES_COUNT_MULTI = notesSize;       
      for (byte i=0; i<NOTES_COUNT_MULTI; i++){
        notesValueMulti[i] = notesArr[i];
        noteIsOnMulti[i] = 0;      
      }

      THRESHOLD_COUNT_MULTI = threshSize;
      for (byte i=0; i<THRESHOLD_COUNT_MULTI; i++){
        NOTES_THRESHOLD_LEVEL_MULTI[i] = thresholdArr[i];     
      }

    }  

    /*
     * id <byte> - id of a sensor
     * notesSize <byte> - number of notes that are played at same time
     * notesArr <byte[]> - array of note values      
     *
     */
    void setupAcord(byte id, byte notesSize, byte notesArr[]){
      sensorId = id;
      mode = TONE_MODE_ACORD;
      
      NOTES_COUNT_ACORD = notesSize;       
      for (byte i=0; i<NOTES_COUNT_ACORD; i++){
        noteValueAcord[i] = notesArr[i];
      }
    }
    
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
    void setupAcordMulti(byte id, byte acordCount, byte notesSize, byte notesArr[N][N], byte threshSize, byte thresholdArr[]){
      sensorId = id;
      mode = TONE_MODE_MULTI_ACORD;
      ACORD_COUNT_ACORD_MULTI = acordCount;
      NOTES_COUNT_ACORD_MULTI = notesSize;
      
      for (byte i=0; i<ACORD_COUNT_ACORD_MULTI; i++){

        noteIsOnAcordMulti[i] = 0; // acord is default off
        
        for (byte j=0; j<NOTES_COUNT_ACORD_MULTI; j++){
          noteValueAcordMulti[i][j] = notesArr[i][j];
        }
      }

      THRESHOLD_COUNT_ACORD_MULTI = threshSize;
      for (byte i=0; i<THRESHOLD_COUNT_ACORD_MULTI; i++){
        NOTES_THRESHOLD_LEVEL_ACORD_MULTI[i] = thresholdArr[i];
      }
      
    }
    
    /*
     *
     * byte thresholdRaw <0 - 50>
     * byte thresholdFiltered <2-255>
     */
    void playNote(MyMidi &mMyMidi, byte thresholdRaw, byte thresholdFiltered){
    
      byte pressure = thresholdFiltered/1.5;
      if (pressure > 127){
        pressure = 127;
      }

      switch(mode){


        case TONE_MODE_SINGLE:
          // note ON
          if (noteIsOnSingle == 0 && thresholdRaw > 2) {
            noteIsOnSingle = 1;
            mMyMidi.noteOn(0, noteValueSingle, mMyMidi.velocity);
        
          // note OFF  
          } else if (noteIsOnSingle == 1 && thresholdRaw < 2) {
            noteIsOnSingle = 0;
            mMyMidi.noteOff(0, noteValueSingle, mMyMidi.velocity);    
          }

          // note pressure
          if (noteIsOnSingle){            
            mMyMidi.afterTouch(0, noteValueSingle, pressure);
          }
        break;


        case TONE_MODE_MULTI:
          for (byte note = 0; note < NOTES_COUNT_MULTI; note++) {
            bool inRange = false;
      
            // loop through thresholh values
            for (byte th = 0; th < THRESHOLD_COUNT_MULTI; th++) {
             
              
              if (note == th){
                // threhold in middle
                if (th+1 < NOTES_COUNT_MULTI){
                    if (thresholdFiltered >= NOTES_THRESHOLD_LEVEL_MULTI[th] && thresholdFiltered < NOTES_THRESHOLD_LEVEL_MULTI[th+1]){
                      inRange = true;
                    }
                // last threshold    
                }else{
                  if (thresholdFiltered >= NOTES_THRESHOLD_LEVEL_MULTI[th]){
                    inRange = true;
                  }
                }
              }
            }
            
        
            // note ON
            if (noteIsOnMulti[note] == 0 && inRange) {
              noteIsOnMulti[note] = 1;
              mMyMidi.noteOn(0, notesValueMulti[note], mMyMidi.velocity);
          
            // note OFF  
            } else if (noteIsOnMulti[note] == 1 && !inRange) {
              noteIsOnMulti[note] = 0;            
              mMyMidi.noteOff(0, notesValueMulti[note], mMyMidi.velocity);    
            }

            // note pressure - disabled, because tones are changing by distance

         }
          
        break;


        case TONE_MODE_ACORD:     
          // note ON
          if (noteIsOnAcord == 0 && thresholdRaw > 2) {         
              noteIsOnAcord = 1;
              for (byte note = 0; note < NOTES_COUNT_ACORD; note++) {
                mMyMidi.noteOn(0, noteValueAcord[note], mMyMidi.velocity);      
              }
        
          // note OFF  
          } else if (noteIsOnAcord == 1 && thresholdRaw < 2) {
              noteIsOnAcord = 0;      
              for (byte note = 0; note < NOTES_COUNT_ACORD; note++) {
                mMyMidi.noteOff(0, noteValueAcord[note], mMyMidi.velocity);         
              }
          }

          // note pressure
          if (noteIsOnAcord == 1){
            for (byte note = 0; note < NOTES_COUNT_ACORD; note++) {
              mMyMidi.afterTouch(0, noteValueAcord[note], pressure);
            }
          }
          
        break;


        case TONE_MODE_MULTI_ACORD:     
          for (byte acord = 0; acord < ACORD_COUNT_ACORD_MULTI; acord++) {
              bool inRange = false;
        
              // loop through thresholh values
              for (byte th = 0; th < THRESHOLD_COUNT_ACORD_MULTI; th++) {
                if (acord == th){
                  // threhold in middle
                  if (th+1 < THRESHOLD_COUNT_ACORD_MULTI){
                      if (thresholdFiltered >= NOTES_THRESHOLD_LEVEL_ACORD_MULTI[th] && thresholdFiltered < NOTES_THRESHOLD_LEVEL_ACORD_MULTI[th+1]){
                        inRange = true;
                      }
                  // last threshold    
                  }else{                  
                    if (thresholdFiltered >= NOTES_THRESHOLD_LEVEL_ACORD_MULTI[th]){
                      inRange = true;
                    }
                  }
                }
              }
          
              // note ON
              if (noteIsOnAcordMulti[acord] == 0 && inRange) {
                noteIsOnAcordMulti[acord] = 1;
                for (byte note = 0; note < NOTES_COUNT_ACORD_MULTI; note++) {
                  mMyMidi.noteOn(0, noteValueAcordMulti[acord][note], mMyMidi.velocity);         
                }              
            
              // note OFF  
              } else if (noteIsOnAcordMulti[acord] == 1 && !inRange) {
                noteIsOnAcordMulti[acord] = 0;                          
                for (byte note = 0; note < NOTES_COUNT_ACORD_MULTI; note++) {
                  mMyMidi.noteOff(0, noteValueAcordMulti[acord][note], mMyMidi.velocity);         
                }   
              }

              // note pressure - disabled, because tones are changing by distance
           }
        break;
      }
    }

    void sensorOff(MyMidi &mMyMidi){
      switch(mode){
        
        case TONE_MODE_SINGLE:
          noteIsOnSingle = false;
          mMyMidi.noteOff(0, noteValueSingle, mMyMidi.velocity);
        break;

        case TONE_MODE_MULTI:
          for (byte note = 0; note < NOTES_COUNT_MULTI; note++) {
            noteIsOnMulti[note] = 0;            
            mMyMidi.noteOff(0, notesValueMulti[note], mMyMidi.velocity);    
          }
        break;

        case TONE_MODE_ACORD:
          for (byte note = 0; note < NOTES_COUNT_ACORD; note++) {
            mMyMidi.noteOff(0, noteValueAcord[note], mMyMidi.velocity);         
          }
          noteIsOnAcord = 0;
        break;

        case TONE_MODE_MULTI_ACORD:
          for (byte acord = 0; acord < NOTES_COUNT_ACORD_MULTI; acord++) {
            
            noteIsOnAcordMulti[acord] = 0;
            
            for (byte note = 0; note < NOTES_COUNT_ACORD_MULTI; note++) {
              mMyMidi.noteOff(0, noteValueAcordMulti[acord][note], mMyMidi.velocity);         
            }  
          }
        break;
      }    
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