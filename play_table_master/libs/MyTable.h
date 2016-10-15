#ifndef MY_TABLE_H
#define MY_TABLE_H
 
#include <Arduino.h>

class MyTable {

  /*
   * the electrode to monitor for proximity
   */ 
  #define PROX_ELECTRODE_0 0
  #define PROX_ELECTRODE_1 1
  #define PROX_ELECTRODE_2 2
  #define PROX_ELECTRODE_3 3
  #define PROX_ELECTRODE_4 4
  #define PROX_ELECTRODE_5 5
  #define PROX_ELECTRODE_6 6
  
  #define PROTOCOL_ID_DELIMETER 0
  #define PROTOCOL_VALUE_DELIMETER 1

  private:
    const int buttonPin = 13;
    int buttonState = LOW;    
  
  public:
  	int noteSetupSelectedIdx = -1;
    float electrodeLastValueRaw[ELECTRODES_COUNT] = { 0, 0, 0, 0, 0, 0, 0}; // 0 - 50
    int electrodeLastValues[ELECTRODES_COUNT] = {0,0,0,0,0,0,0}; // 2-255

    /* KNOB SETUP */
    const byte PIN_KNOB_VOLUME = 0;
    const byte PIN_KNOB_NOTES = 1;
    
    const byte PLAY_MODE_TONES = 0;
    const byte PLAY_MODE_THEMERIN = 1;
    const byte MODE_COUNT = 2;
  
    byte selectedMode = PLAY_MODE_TONES;
  
    byte ELECTRODES_ARR[ELECTRODES_COUNT] = {
      PROX_ELECTRODE_0, PROX_ELECTRODE_1, PROX_ELECTRODE_2,
      PROX_ELECTRODE_3, PROX_ELECTRODE_4, PROX_ELECTRODE_5,
      PROX_ELECTRODE_6 
    };
  
  NoteSetup mNoteSetup; 

  // TableSensor mTableSensors[ELECTRODES_COUNT];  
  
  void setupButton(){
    pinMode(buttonPin, INPUT);
  }

  void toggleTones(byte velocity, MyMidi &mMyMidi){
    if (digitalRead(buttonPin) == HIGH && buttonState == LOW) {
      buttonState = HIGH;
      selectedMode = (selectedMode+1) % MODE_COUNT;

      // play beeps to know what  mode is on 
      for (byte i=0;i<selectedMode+1;i++){
        mMyMidi.noteOn(0, 60, velocity);
        delay(500);
        mMyMidi.noteOff(0, 60, velocity);
      }
      
    } else {
      buttonState = LOW;    
    }
  }

  void setVolumeByPotentiometer(MyMidi &mMyMidi){
    int volume = analogRead(PIN_KNOB_VOLUME);
    mMyMidi.setVolume(abs(1000-volume)/8);
    //Serial.print("potentiometer: ");
    //Serial.println(vav);
  }

  void setupNotes(MyMidi &mMyMidi, byte newNoteSetupIdx, bool playTones){

    if (playTones){      
      // play beeps to know what  mode is on 
      for (byte i=0;i<newNoteSetupIdx+1;i++){
        mMyMidi.noteOn(0, 60, mMyMidi.velocity);
        delay(250);
        mMyMidi.noteOff(0, 60, mMyMidi.velocity);
      }
    }

    mNoteSetup.setupNotes(newNoteSetupIdx);
  }
  
  /*
   * acord and multi note mode
   */ 
  void setMidiNotesByPotentiometer(MyMidi &mMyMidi){
    // v == <44, 922>
    int v = abs(1000-analogRead(PIN_KNOB_NOTES));
  
    byte newNotesIdx = -1;
    if (v < 224){
      newNotesIdx = 0;
    }else if (v > 276 && v < 456){
      newNotesIdx = 1;
    }else if (v > 508 && v < 688){
      newNotesIdx = 2;
    }else if ( v > 740){
      newNotesIdx = 3;
    }else{
      newNotesIdx = noteSetupSelectedIdx;
    }
  
    // initial state
    if (noteSetupSelectedIdx == -1){
      noteSetupSelectedIdx = newNotesIdx;
      setupNotes(mMyMidi, noteSetupSelectedIdx, false);

    // toggle by user
    }else if (newNotesIdx != -1 && newNotesIdx != noteSetupSelectedIdx){
      noteSetupSelectedIdx = newNotesIdx;        
      turnOffAllNotes(mMyMidi);
      setupNotes(mMyMidi, noteSetupSelectedIdx, true);
      initScreenSaverSensorType();
    }    
  }

  /*
   * switching to another mode
   */
  void turnOffAllNotes(MyMidi mMyMidi){
    for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {
       mNoteSetup.mTableSensors[elc].sensorOff(mMyMidi);
    }
  }

  /*
   * 
   * loop all touch sensors
   *
   */
  void playToneMode(MyMidi &mMyMidi){
    for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {     
      mNoteSetup.mTableSensors[elc].playNote(mMyMidi, electrodeLastValueRaw[elc], electrodeLastValues[elc]);
    }
  }

  void playSelectedMode(Themerin &mThemerin, MyMidi &mMyMidi){
    // play tones
    //Serial.print("selectedMode: ");
    //Serial.println(selectedMode);
    if (selectedMode == PLAY_MODE_TONES){      
      playToneMode(mMyMidi);
    }else if (selectedMode == PLAY_MODE_THEMERIN){
      mThemerin.playThereminMode(mMyMidi); 
    }
  }

  /*
   * init communication
   */
  void setupSerialCommunication() {
    Serial.begin(9600);
    Serial1.begin(9600);
  }

  /*
   * 0-11 electrodes
   * 30-41 IDs
   */
  void sendValuesToSlaves() {
    for (byte i = 0; i < ELECTRODES_COUNT; i++) {
      sendMessageToSlave(i+SLAVE_FIRST_ID, electrodeLastValues[i]);
    }
  }    

  void sendMessageToSlave(byte idSlave, byte valueSlave) {
    Serial1.write(PROTOCOL_ID_DELIMETER);
    Serial1.write(idSlave);
    Serial1.write(PROTOCOL_VALUE_DELIMETER);
    Serial1.write(valueSlave);
  }

  void initScreenSaverSensorType(){
    
    for (byte sensor = 0; sensor<ELECTRODES_COUNT; sensor++){  
      mNoteSetup.mTableSensors[sensor].visualLedsHelper = 2;
    }

    for (byte n = 0; n<13; n++){  
      for (byte sensor = 0; sensor<ELECTRODES_COUNT; sensor++){  
        
        /*
         * SINGLE TONE MODE
         */ 
        if (mNoteSetup.mTableSensors[sensor].getMode() == TONE_MODE_SINGLE){
          sendMessageToSlave(mNoteSetup.mTableSensors[sensor].getSensorId()+SLAVE_FIRST_ID, 128);            
        }

        /*
         * ACORD TONE MODE
         */
        if (mNoteSetup.mTableSensors[sensor].getMode() == TONE_MODE_ACORD){
          sendMessageToSlave(mNoteSetup.mTableSensors[sensor].getSensorId()+SLAVE_FIRST_ID, 255);            
        }

        /*
         * MULTI TONE
         */
        if (mNoteSetup.mTableSensors[sensor].getMode() == TONE_MODE_MULTI){
          mNoteSetup.mTableSensors[sensor].incVisualLedHelper(9);                              
          sendMessageToSlave(mNoteSetup.mTableSensors[sensor].getSensorId()+SLAVE_FIRST_ID, mNoteSetup.mTableSensors[sensor].visualLedsHelper);
        }

        /*
         * ACORD MULTI TONE
         */
        if (mNoteSetup.mTableSensors[sensor].getMode() == TONE_MODE_MULTI_ACORD){            
          mNoteSetup.mTableSensors[sensor].incVisualLedHelper(18);                    
          sendMessageToSlave(mNoteSetup.mTableSensors[sensor].getSensorId()+SLAVE_FIRST_ID, mNoteSetup.mTableSensors[sensor].visualLedsHelper);
        }
                
      }
      delay(150);
    }
  }
  
};
#endif