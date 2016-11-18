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
    bool noteIsOnSingle = false;
    byte noteValueSingle = 0;

    /*
     * multi
     * plays one tone based on distance from sensor
     */ 
    // bool arr to know what note is on
    bool noteIsOnMulti[18];
    // current selected note setup
    byte notesValueMulti[18];

    /*
     * acord
     * play multiple tones anytime sensor range is reached
     */     
    // to know if note is on
    bool noteIsOnAcord = false;
    // current selected note setup
    byte noteValueAcord[6];
   
    /*
     * multi acord
     * plays multiple tones based on distance from sensor
     */     
    byte ACORD_COUNT_ACORD_MULTI;
    bool noteIsOnAcordMulti[3];
    byte noteValueAcordMulti[3][6];

    /*
     * arpeggio
     * toggle between tones, increase tempo by distance
     */   
    // arpeggio counter
    byte arpCount = 0;
    bool arpeggioSingleMode = false;
    // bool noteIsOnMulti[18]; // for memory limits uses vars from multi
    // byte notesValueMulti[18]; // for memory limits uses vars from multi

    /*
     * multi, multi acord
     */
    byte NOTES_COUNT;
    byte THRESHOLD_COUNT;
    byte NOTES_THRESHOLD_LEVEL[18];

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
      noteIsOnSingle = false;
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

      NOTES_COUNT = notesSize;       
      for (byte i=0; i<NOTES_COUNT; i++){
        notesValueMulti[i] = notesArr[i];
        noteIsOnMulti[i] = false;      
      }

      THRESHOLD_COUNT = threshSize;
      for (byte i=0; i<THRESHOLD_COUNT; i++){
        NOTES_THRESHOLD_LEVEL[i] = thresholdArr[i];     
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
      
      noteIsOnAcord = false;
      NOTES_COUNT = notesSize;       
      for (byte i=0; i<NOTES_COUNT; i++){
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
      NOTES_COUNT = notesSize;
      
      for (byte i=0; i<ACORD_COUNT_ACORD_MULTI; i++){

        noteIsOnAcordMulti[i] = false; // acord is default off
        
        for (byte j=0; j<NOTES_COUNT; j++){
          noteValueAcordMulti[i][j] = notesArr[i][j];
        }
      }

      THRESHOLD_COUNT = threshSize;
      for (byte i=0; i<THRESHOLD_COUNT; i++){
        NOTES_THRESHOLD_LEVEL[i] = thresholdArr[i];
      }
      
    }

    /*
     * id <byte> - id of a sensor
     * notesSize <byte> - number of notes that are played at same time
     * notesArr <byte[]> - array of note values      
     *
     */
    void setupArpeggio(byte id, byte notesSize, byte notesArr[], bool singleToneMode){
      sensorId = id;
      mode = TONE_MODE_ARPEGGIO;
      
      arpeggioSingleMode = singleToneMode;
      NOTES_COUNT = notesSize;       
      for (byte i=0; i<NOTES_COUNT; i++){
        notesValueMulti[i] = notesArr[i];
        noteIsOnMulti[i] = false;  
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
          if (noteIsOnSingle == false && thresholdRaw > 2) {
            noteIsOnSingle = true;
            mMyMidi.noteOn(0, noteValueSingle, mMyMidi.velocity);
        
          // note OFF  
          } else if (noteIsOnSingle == true && thresholdRaw < 2) {
            noteIsOnSingle = false;
            mMyMidi.noteOff(0, noteValueSingle, mMyMidi.velocity);    
          }

          // note pressure
          if (noteIsOnSingle){            
            mMyMidi.afterTouch(0, noteValueSingle, pressure);
          }
        break;


        case TONE_MODE_MULTI:
          for (byte note = 0; note < NOTES_COUNT; note++) {
            bool inRange = false;
      
            // loop through thresholh values
            for (byte th = 0; th < THRESHOLD_COUNT; th++) {
             
              
              if (note == th){
                // threhold in middle
                if (th+1 < NOTES_COUNT){
                    if (thresholdFiltered >= NOTES_THRESHOLD_LEVEL[th] && thresholdFiltered < NOTES_THRESHOLD_LEVEL[th+1]){
                      inRange = true;
                    }
                // last threshold    
                }else{
                  if (thresholdFiltered >= NOTES_THRESHOLD_LEVEL[th]){
                    inRange = true;
                  }
                }
              }
            }
            
        
            // note ON
            if (noteIsOnMulti[note] == false && inRange) {
              noteIsOnMulti[note] = true;
              mMyMidi.noteOn(0, notesValueMulti[note], mMyMidi.velocity);
          
            // note OFF  
            } else if (noteIsOnMulti[note] == true && !inRange) {
              noteIsOnMulti[note] = false;            
              mMyMidi.noteOff(0, notesValueMulti[note], mMyMidi.velocity);    
            }

            // note pressure - disabled, because tones are changing by distance

         }
          
        break;


        case TONE_MODE_ACORD:     
          // note ON
          if (noteIsOnAcord == false && thresholdRaw > 2) {         
              noteIsOnAcord = true;
              for (byte note = 0; note < NOTES_COUNT; note++) {
                mMyMidi.noteOn(0, noteValueAcord[note], mMyMidi.velocity);      
              }
        
          // note OFF  
          } else if (noteIsOnAcord == true && thresholdRaw < 2) {
              noteIsOnAcord = false;      
              for (byte note = 0; note < NOTES_COUNT; note++) {
                mMyMidi.noteOff(0, noteValueAcord[note], mMyMidi.velocity);         
              }
          }

          // note pressure
          if (noteIsOnAcord == true){
            for (byte note = 0; note < NOTES_COUNT; note++) {
              mMyMidi.afterTouch(0, noteValueAcord[note], pressure);
            }
          }
          
        break;


        case TONE_MODE_MULTI_ACORD:     
          for (byte acord = 0; acord < ACORD_COUNT_ACORD_MULTI; acord++) {
              bool inRange = false;
        
              // loop through thresholh values
              for (byte th = 0; th < THRESHOLD_COUNT; th++) {
                if (acord == th){
                  // threhold in middle
                  if (th+1 < THRESHOLD_COUNT){
                      if (thresholdFiltered >= NOTES_THRESHOLD_LEVEL[th] && thresholdFiltered < NOTES_THRESHOLD_LEVEL[th+1]){
                        inRange = true;
                      }
                  // last threshold    
                  }else{                  
                    if (thresholdFiltered >= NOTES_THRESHOLD_LEVEL[th]){
                      inRange = true;
                    }
                  }
                }
              }
          
              // note ON
              if (noteIsOnAcordMulti[acord] == false && inRange) {
                noteIsOnAcordMulti[acord] = true;
                for (byte note = 0; note < NOTES_COUNT; note++) {
                  mMyMidi.noteOn(0, noteValueAcordMulti[acord][note], mMyMidi.velocity);         
                }              
            
              // note OFF  
              } else if (noteIsOnAcordMulti[acord] == true && !inRange) {
                noteIsOnAcordMulti[acord] = false;                          
                for (byte note = 0; note < NOTES_COUNT; note++) {
                  mMyMidi.noteOff(0, noteValueAcordMulti[acord][note], mMyMidi.velocity);         
                }   
              }

              // note pressure - disabled, because tones are changing by distance
           }
        break;

        case TONE_MODE_ARPEGGIO:

          for (byte note = 0; note < NOTES_COUNT; note++) {
            bool inRange = false;            

            if (thresholdFiltered > 240) inRange = true;

            if (thresholdRaw > 2){

              // play only one note at time
              if (arpeggioSingleMode == true){
                if ((arpCount > (note*(255/NOTES_COUNT))) 
                  && (arpCount < ((note+1)*(255/NOTES_COUNT))) ){
                    inRange = true;
                }

              // play more notes at time
              }else{
                if (arpCount > note*255/NOTES_COUNT && arpCount<255){
                    inRange = true;
                }                
              }

            }
        
            // note ON
            if (noteIsOnMulti[note] == false && inRange) {             
              noteIsOnMulti[note] = true;
              mMyMidi.noteOn(0, notesValueMulti[note], mMyMidi.velocity);
          
            // note OFF  
            } else if (noteIsOnMulti[note] == true && !inRange) {               
              noteIsOnMulti[note] = false;            
              mMyMidi.noteOff(0, notesValueMulti[note], mMyMidi.velocity);    
            }
            // note pressure - disabled, because tones are changing by distance
          }
  
          if (thresholdRaw <= 2 ) 
            arpCount = 0;
          
          // "thresholdFiltered/10" is increasing faster than "thresholdFiltered/20"
          arpCount += thresholdFiltered/40;
          
          if (arpCount == 255){
            arpCount = 0;
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
          for (byte note = 0; note < NOTES_COUNT; note++) {
            noteIsOnMulti[note] = false;            
            mMyMidi.noteOff(0, notesValueMulti[note], mMyMidi.velocity);    
          }
        break;

        case TONE_MODE_ACORD:
          for (byte note = 0; note < NOTES_COUNT; note++) {
            mMyMidi.noteOff(0, noteValueAcord[note], mMyMidi.velocity);         
          }
          noteIsOnAcord = false;
        break;

        case TONE_MODE_MULTI_ACORD:
          for (byte acord = 0; acord < NOTES_COUNT; acord++) {
            
            noteIsOnAcordMulti[acord] = false;
            
            for (byte note = 0; note < NOTES_COUNT; note++) {
              mMyMidi.noteOff(0, noteValueAcordMulti[acord][note], mMyMidi.velocity);         
            }  
          }
        break;

        case TONE_MODE_ARPEGGIO:
          for (byte note = 0; note < NOTES_COUNT; note++) {
            noteIsOnMulti[note] = false;            
            mMyMidi.noteOff(0, notesValueMulti[note], mMyMidi.velocity);    
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