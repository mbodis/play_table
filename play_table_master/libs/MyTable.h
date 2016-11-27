#ifndef MY_TABLE_H
#define MY_TABLE_H
 
#include <Arduino.h>

/*
 *
 */
class MyTable {
   
    private:
        const byte buttonPin = 13;
        byte buttonState = LOW;    
  
    public:
        byte noteSetupSelectedIdx = -1;
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
  
    NoteSetup *mNoteSetup;
    NoteGroup *mNoteGroup;
  
    MyTable(){
        mNoteSetup = new NoteSetup();
        mNoteGroup = new NoteGroup();
    }

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
        volume = abs(1000-volume)/8;

        for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {

            // set volume for single tone / acord mode
            if (mNoteSetup->mTableSensors[elc]->getMode() == TONE_MODE_SINGLE 
                || mNoteSetup->mTableSensors[elc]->getMode() == TONE_MODE_ACORD){

                int vv = (volume -12) + (mNoteSetup->mTableSensors[elc]->getVolume()/8);
                if (vv <= 0) vv = 1;
                if (vv > 127) vv = 127;

                mMyMidi.setVolume(elc, vv);

            }else{
                mMyMidi.setVolume(elc, volume);
            }

        }
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

        mNoteSetup->setupNotes(newNoteSetupIdx, mMyMidi);
        mNoteGroup->setGroup(mNoteSetup->groupIds);
    }
  
    /*
     * acord and multi note mode
     */ 
    void setMidiNotesByPotentiometer(MyMidi &mMyMidi){
        // v == <44, 922>
        int v = abs(1000-analogRead(PIN_KNOB_NOTES));

        // Serial.print("knob: ");
        // Serial.println(v);

        byte newNotesIdx = -1;
        if (v < 90){
            newNotesIdx = 0;
        }else if (v > 210 && v < 290){
            newNotesIdx = 1;
        }else if (v > 385 && v < 480){
            newNotesIdx = 2;
        }else if ( v > 586 && v < 658){
            newNotesIdx = 3;
        }else if ( v > 742){
            newNotesIdx = 4;
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
    void turnOffAllNotes(MyMidi &mMyMidi){
        for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {
            mNoteSetup->mTableSensors[elc]->sensorOff(mMyMidi);
        }
    }

    void playSelectedMode(Themerin &mThemerin, MyMidi &mMyMidi){
        // play tones
        if (selectedMode == PLAY_MODE_TONES){      
            playToneMode(mMyMidi);            
        }else if (selectedMode == PLAY_MODE_THEMERIN){
            mThemerin.playThereminMode(mMyMidi); 
        }
    }

    /*
     * 
     * loop all touch sensors and play mode
     *
     */
    void playToneMode(MyMidi &mMyMidi){

        // TOME MODE GROUP
        if (mNoteSetup->groupMode){
            playToneModeGroups(mMyMidi);                   

        // TONE MODE NORMAL
        }else{
            playToneModeNormal(mMyMidi);
        }
    }

    /*
     *
     */
    void playToneModeNormal(MyMidi &mMyMidi){
        for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {     
            mNoteSetup->mTableSensors[elc]->playNote(mMyMidi, (byte)electrodeLastValueRaw[elc], electrodeLastValues[elc]);
        }
    }

    /*
     *
     */
    void playToneModeGroups(MyMidi &mMyMidi){
        // update status
        mNoteGroup->changeSensorStatus(electrodeLastValueRaw);
        for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {    

            // play by status - automatic mode
            if (mNoteSetup->mTableSensors[elc]->hasAutoMode() 
                && !mNoteSetup->mTableSensors[elc]->isLoopingEnabled() 
                && mNoteGroup->sensorStatus[elc] == SENSOR_STATE_HOVER){
                // turn OFF all sensors from same group
                for (byte elc2 = 0; elc2 < ELECTRODES_COUNT; elc2++) {    
                    if (mNoteGroup->groupIds[elc] == mNoteGroup->groupIds[elc2] && elc2 != elc){
                        mNoteSetup->mTableSensors[elc2]->sensorOff(mMyMidi);
                    }
                }
            }

            // play by status - simple hover
            if (mNoteGroup->sensorStatus[elc] == SENSOR_STATE_HOVER 
                || mNoteGroup->sensorStatus[elc] == SENSOR_STATE_HAND_ABOVE
                || mNoteGroup->sensorStatus[elc] == SENSOR_STATE_RELEASING ){
                mNoteSetup->mTableSensors[elc]->playNote(mMyMidi, (byte)electrodeLastValueRaw[elc], electrodeLastValues[elc]);
            
            // stop by status - simple
            }else if (mNoteGroup->sensorStatus[elc] == SENSOR_STATE_START_BLOCKING_BY_GROUP){
                mNoteSetup->mTableSensors[elc]->sensorOff(mMyMidi);
            }

            // play auto mode
            if(mNoteSetup->mTableSensors[elc]->isLoopingEnabled()){
                mNoteSetup->mTableSensors[elc]->playNote(mMyMidi, (byte)electrodeLastValueRaw[elc], electrodeLastValues[elc]);
            }
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

            // auto mode
            if(mNoteSetup->mTableSensors[i]->getMode() == TONE_MODE_ARPEGGIO
                && mNoteSetup->mTableSensors[i]->isLoopingEnabled()){
                sendMessageToSlave(i+SLAVE_FIRST_ID, static_cast<SensorArpeggio*>(mNoteSetup->mTableSensors[i])->getArpCount() );

            // default value from sensor
            }else{
                sendMessageToSlave(i+SLAVE_FIRST_ID, electrodeLastValues[i]);
            }
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
            mNoteSetup->mTableSensors[sensor]->setVisualLedHelper(2);
        }

        for (byte n = 0; n<13; n++){  
            for (byte sensor = 0; sensor<ELECTRODES_COUNT; sensor++){  

                /*
                * SINGLE TONE MODE
                */ 
                if (mNoteSetup->mTableSensors[sensor]->getMode() == TONE_MODE_SINGLE){
                    sendMessageToSlave(mNoteSetup->mTableSensors[sensor]->getSensorId()+SLAVE_FIRST_ID, 128);            
                }

                /*
                * ACORD TONE MODE
                */
                if (mNoteSetup->mTableSensors[sensor]->getMode() == TONE_MODE_ACORD){
                    sendMessageToSlave(mNoteSetup->mTableSensors[sensor]->getSensorId()+SLAVE_FIRST_ID, 255);            
                }

                /*
                * MULTI TONE
                */
                if (mNoteSetup->mTableSensors[sensor]->getMode() == TONE_MODE_MULTI){
                    mNoteSetup->mTableSensors[sensor]->incVisualLedHelper(9);                              
                    sendMessageToSlave(mNoteSetup->mTableSensors[sensor]->getSensorId()+SLAVE_FIRST_ID, mNoteSetup->mTableSensors[sensor]->getVisualLedHelper());
                }

                /*
                * ACORD MULTI TONE
                */
                if (mNoteSetup->mTableSensors[sensor]->getMode() == TONE_MODE_MULTI_ACORD){            
                    mNoteSetup->mTableSensors[sensor]->incVisualLedHelper(18);                    
                    sendMessageToSlave(mNoteSetup->mTableSensors[sensor]->getSensorId()+SLAVE_FIRST_ID, mNoteSetup->mTableSensors[sensor]->getVisualLedHelper());
                }

                /*
                * ARPEGGIO, ARPEGGIO ACORD
                */
                if (mNoteSetup->mTableSensors[sensor]->getMode() == TONE_MODE_ARPEGGIO
                    || mNoteSetup->mTableSensors[sensor]->getMode() == TONE_MODE_ARPEGGIO_ACORD){
                    mNoteSetup->mTableSensors[sensor]->incVisualLedHelper(18);         
                    if (n%2 == 0){
                        sendMessageToSlave(mNoteSetup->mTableSensors[sensor]->getSensorId()+SLAVE_FIRST_ID, 10);
                    }else{
                        sendMessageToSlave(mNoteSetup->mTableSensors[sensor]->getSensorId()+SLAVE_FIRST_ID, mNoteSetup->mTableSensors[sensor]->getVisualLedHelper());
                    }                    
                }

            }
            //wait some time after loop all sensors
            delay(150);
        }
    }
  
};
#endif