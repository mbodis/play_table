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
    float electrodeLastValueRaw[ELECTRODES_COUNT] = { 0, 0, 0, 0, 0, 0, 0};
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
  
  TableSensor mTableSensors[ELECTRODES_COUNT];  
  
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
    mMyMidi.talkMIDI(0xB0, 0x07, abs(1000-volume)/8);
    //Serial.print("potentiometer: ");
    //Serial.println(vav);
  }

  void setupNotes(MyMidi &mMyMidi, byte newNoteSetupIdx){

    // play beeps to know what  mode is on 
    // for (byte i=0;i<newNoteSetupIdx+1;i++){
    //   mMyMidi.noteOn(0, 60, mMyMidi.velocity);
    //   delay(500);
    //   mMyMidi.noteOff(0, 60, mMyMidi.velocity);
    // }

    switch(newNoteSetupIdx){
      case 0:
      break;

      case 1:
      break;      
    }
    //TODO switch by idx
    testNotesSetup();
  }

  void testNotesSetup(){
    // single mode
    mTableSensors[0].setupSingle(0, 60);
//    mTableSensors[1].setupSingle(1, 62);
//    mTableSensors[2].setupSingle(2, 64);
//    mTableSensors[3].setupSingle(3, 65);
   mTableSensors[4].setupSingle(4, 67);
   // mTableSensors[5].setupSingle(5, 69);
   // mTableSensors[6].setupSingle(6, 71);

    // multi mode
    byte THRESHOLD_LEVEL[] = {20, 60, 120};    
//    byte arr0[] = {48, 50, 52};
//    mTableSensors[0].setupMulti(0, 3, arr0, 3, THRESHOLD_LEVEL);
    byte arr1[] = {53, 55, 57};
    mTableSensors[1].setupMulti(1, 3, arr1, 3, THRESHOLD_LEVEL);
//    byte arr2[] = {59, 60, 62};
//    mTableSensors[2].setupMulti(2, 3, arr2, 3, THRESHOLD_LEVEL);
//    byte arr3[] = {64, 65, 67};
//    mTableSensors[3].setupMulti(3, 3, arr3, 3, THRESHOLD_LEVEL);
//    byte arr4[] = {69, 71, 72};
//    mTableSensors[4].setupMulti(4, 3, arr4, 3, THRESHOLD_LEVEL);
   byte arr5[] = {74, 76, 77};
   mTableSensors[5].setupMulti(5, 3, arr5, 3, THRESHOLD_LEVEL);
   byte arr6[] = {79, 81, 52};
   mTableSensors[6].setupMulti(6, 3, arr6, 3, THRESHOLD_LEVEL);

    // acord mode
//    byte arr20[] = {52, 55, 59, 62};
//    mTableSensors[0].setupAcord(0, 4, arr20);
//    byte arr21[] = {55, 59, 62, 64};
//    mTableSensors[1].setupAcord(1, 4, arr21);
    byte arr22[] = {57, 61, 64, 67};
    mTableSensors[2].setupAcord(2, 4, arr22);
//    byte arr23[] = {64, 64, 64, 64};
//    mTableSensors[3].setupAcord(3, 4, arr23);
//    byte arr24[] = {67, 67, 67, 67};
//    mTableSensors[4].setupAcord(4, 4, arr24);
//    byte arr25[] = {69, 69, 69, 69};
//    mTableSensors[5].setupAcord(5, 4, arr25);
//    byte arr26[] = {71, 71, 71, 71};
//    mTableSensors[6].setupAcord(6, 4, arr26);

    // acord multi mode
    byte AMM_THRESHOLD_LEVEL[] = {20, 60, 120};
    byte arr33[3][4] = { {48, 50, 52, 48}, {57, 61, 64, 67}, {59, 60, 62, 59}};    
    mTableSensors[3].setupAcordMulti(3, 4, arr33, 3, AMM_THRESHOLD_LEVEL);    
   
  }
  
  /*
   * acord and multi note mode
   */ 
  void setMidiNotesByPotentiometer(MyMidi &mMyMidi){
    // v == <0, 1000>
    int v = abs(1000-analogRead(PIN_KNOB_NOTES));
    
    byte newNotesIdx = -1;
    if (v > 0 && v < 400){
      newNotesIdx = 0;
    }else if ( v > 600){
      newNotesIdx = 1;
    }
  
    // initial state
    if (noteSetupSelectedIdx == -1){
      noteSetupSelectedIdx = newNotesIdx;
      setupNotes(mMyMidi, noteSetupSelectedIdx);

    // toggle by user
    }else if (newNotesIdx != -1 && newNotesIdx != noteSetupSelectedIdx){
      noteSetupSelectedIdx = newNotesIdx;        
      turnOffAllNotes(mMyMidi);
      setupNotes(mMyMidi, noteSetupSelectedIdx);
      initScreenSaverSensorType();
    }    
  }

  /*
   * switching to another mode
   */
  void turnOffAllNotes(MyMidi mMyMidi){
    for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {
       mTableSensors[elc].sensorOff(mMyMidi);
    }
  }

  /*
   * 
   * loop all touch sensors
   *
   */
  void playToneMode(MyMidi &mMyMidi){
    for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {     
      mTableSensors[elc].playNote(mMyMidi, electrodeLastValueRaw[elc], electrodeLastValues[elc]);
      // TODO keep playing note that was released and is also somewhere played
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
        sendMessageToSlave(i+30, electrodeLastValues[i]);
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
        mTableSensors[sensor].visualLedsHelper = 2;
      }

      for (byte n = 0; n<13; n++){  
        for (byte sensor = 0; sensor<ELECTRODES_COUNT; sensor++){  
          
          // SINGLE TONE MODE 
          if (mTableSensors[sensor].getMode() == 0 ){
            sendMessageToSlave(mTableSensors[sensor].getSensorId()+30, 128);            
          }

          // ACORD TONE MODE
          if (mTableSensors[sensor].getMode() == 2){
            sendMessageToSlave(mTableSensors[sensor].getSensorId()+30, 255);            
          }

          // MULTI TONE
          if (mTableSensors[sensor].getMode() == 1){
            mTableSensors[sensor].incVisualLedHelper(9);                              
            sendMessageToSlave(mTableSensors[sensor].getSensorId()+30, mTableSensors[sensor].visualLedsHelper);
          }

          // ACORD MULTI TONE
          if (mTableSensors[sensor].getMode() == 3){            
            mTableSensors[sensor].incVisualLedHelper(18);                    
            sendMessageToSlave(mTableSensors[sensor].getSensorId()+30, mTableSensors[sensor].visualLedsHelper);
          }
                  
        }
        delay(300);
      }
    }
  
};
#endif