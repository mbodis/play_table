#ifndef TABLE_SENSOR_H
#define TABLE_SENSOR_H
 
#include <Arduino.h>


class TableSensor {
  private:

  public:
    const byte TONE_MODE_SINGLE = 0;
    const byte TONE_MODE_MULTI = 1;
    const byte TONE_MODE_ACORD = 2;
    const byte TONE_MODE_MULTI_ACORD = 3;
  
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
    byte noteIsOnMulti[5];// 5==max we don't use all of them
    // current selected note setup
    byte notesValueMulti[5];// 5==max we don't use all of them

    /*
     * acord
     * play multiple tones anytime sensor range is reached
     */     
    byte NOTES_COUNT_ACORD;
    // to know if note is on
    byte noteIsOnAcord = 0;
    // current selected note setup
    byte noteValueAcord[5];// 5==max we don't use all of them
   
    /*
     * multi acord
     * plays multiple tones based on distance from sensor
     */     
    byte NOTES_COUNT_ACORD_MULTI;
    byte THRESHOLD_COUNT_ACORD_MULTI;
    byte NOTES_THRESHOLD_LEVEL_ACORD_MULTI[10];
    byte noteIsOnAcordMulti[5];// 10 ==max we don't use all of them
    byte noteValueAcordMulti[5][4];// 10 4==max we don't use all of them

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

  void setupSingle(byte id, byte noteValue){
    sensorId = id;
    mode = TONE_MODE_SINGLE;
    noteValueSingle = noteValue;
  }

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

  void setupAcord(byte id, byte notesSize, byte notesArr[]){
    sensorId = id;
    mode = TONE_MODE_ACORD;
    
    NOTES_COUNT_ACORD = notesSize;       
    for (byte i=0; i<NOTES_COUNT_ACORD; i++){
      noteValueAcord[i] = notesArr[i];
    }
  }
  
  void setupAcordMulti(byte id, byte notesSize, byte notesArr[3][4], byte threshSize, byte thresholdArr[]){
    sensorId = id;
    mode = TONE_MODE_MULTI_ACORD;
    
    NOTES_COUNT_ACORD_MULTI = notesSize;
    for (byte i=0; i<3; i++){

      noteIsOnAcordMulti[i] = 0; // acord is default off
      
      for (byte j=0; j<4; j++){
        noteValueAcordMulti[i][j] = notesArr[i][j];
      }
    }

    THRESHOLD_COUNT_ACORD_MULTI = threshSize;
    for (byte i=0; i<THRESHOLD_COUNT_ACORD_MULTI; i++){
      NOTES_THRESHOLD_LEVEL_ACORD_MULTI[i] = thresholdArr[i];
    }
    
  }
  
  void playNote(MyMidi &mMyMidi, int thresholdRaw, byte thresholdFiltered){
    
    switch(mode){

      /*
       * TONE_MODE_SINGLE
       */
      case 0:
        // note ON
        if (noteIsOnSingle == 0 && thresholdRaw > 2) {
          noteIsOnSingle = 1;
          mMyMidi.noteOn(0, noteValueSingle, mMyMidi.velocity);
      
        // note OFF  
        } else if (noteIsOnSingle == 1 && thresholdRaw < 2) {
          noteIsOnSingle = 0;
          mMyMidi.noteOff(0, noteValueSingle, mMyMidi.velocity);    
        }
      break;

      /*
       * TONE_MODE_MULTI
       */
      case 1:
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
       }
        
      break;

      /* 
       *  TONE_MODE_ACORD
       */
      case 2:     
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
        
      break;

      /*
       * TONE_MODE_MULTI_ACORD
       */
      case 3:     
        for (byte acord = 0; acord < NOTES_COUNT_ACORD_MULTI; acord++) {
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
                    inRange = false;
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
         }
      break;
    }
  }

  void sensorOff(MyMidi &mMyMidi){
    switch(mode){
      
      /*
       * TONE_MODE_SINGLE
       */
      case 0:
        noteIsOnSingle = false;
        mMyMidi.noteOff(0, noteValueSingle, mMyMidi.velocity);
      break;

      /*
       * TONE_MODE_MULTI
       */
      case 1:
        for (byte note = 0; note < NOTES_COUNT_MULTI; note++) {
          noteIsOnMulti[note] = 0;            
          mMyMidi.noteOff(0, notesValueMulti[note], mMyMidi.velocity);    
        }
      break;

      /*
       * TONE_MODE_ACORD
       */
      case 2:
        for (byte note = 0; note < NOTES_COUNT_ACORD; note++) {
          mMyMidi.noteOff(0, noteValueAcord[note], mMyMidi.velocity);         
        }
        noteIsOnAcord = 0;
      break;

      /*
       * TONE_MODE_MULTI_ACORD
       */
      case 3:
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
   toggle note setup helper for visualization
   */
  int visualLedsHelper = 0;

  /*
   toggle note setup helper for visualization method
   */
  void incVisualLedHelper(int inc){
    visualLedsHelper+=inc;
    if (visualLedsHelper >255){
      visualLedsHelper = 255;
    }
  }
};

#endif