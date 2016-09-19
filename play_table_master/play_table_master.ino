/*******************************************************************************

  created: 2016-03-26
  last modified: 2016-09-20
  Miroslav Bodis - play table - master

  MIDI docs:
  http://www.ccarh.org/whiteNotescourses/253/handout/midiprotocol/
  http://www.music-softwwhiteNotesare-development.com/midi-tutorial.html
  http://www.gweep.net/~whiteNotesprefect/eng/reference/protocol/midispec.html#Aft


  *** Protocol ***
  protocol byte stream - serial connection from mater to slaves
  
  0 | [ID] | 1 | [VALUE]
  ID = <20-31>      // ELECTRODE_0 == 20, ..., ELECTODE_31 == 31
  VALUE = <2 - 255> // value 0 is key for id, value 1 is key for value

  example:
  0 | 20 | 1 | 123
  key | value | key | value ...
  id=20 value=123
  
  *** *** *** *** *** ***

  
*******************************************************************************/
#include <MPR121.h>
#include <SoftwareSerial.h>

#include <Constants.h>
#include <MyMidi.h>
#include <Themerin.h>
#include <TableSensor.h>
#include <MyTable.h>
#include <TouchSensors.h>
#include <MyCommunicator.h>
#include <VisualEffects.h>

/* 
  CLASS DEFINITION
 */
MyMidi mMyMidi;
Themerin mThemerin;
MyTable mMyTable;
MyCommunicator mMyCommunicator;
TouchSensors mTouchSensors;
VisualEffects mVisualEffects;

/*
   MAIN - SETUP
*/
void setup() {
  // setup midi output
  mMyMidi.midi2UsbSetup();

  mMyTable.setupButton();

  // set default notes
  mMyTable.setupNotes(mMyMidi, 0);

  // setup comunication
  mMyTable.setupSerialCommunication();

  // setup touch electrodes
  mTouchSensors.setupTouchElectrodes();

  // setup proximity limits
  mTouchSensors.setupProximity();

  // initialise MIDI
  mMyMidi.setupMidi();

  // do some initial led effect
  mVisualEffects.initScreenSaver(mMyTable);  
}

/*
   MAIN - LOOP
*/
void loop() {  

  // read values from electrodes
  mTouchSensors.updateReading();
  mTouchSensors.readValuesFromElectrodes(mMyTable);

  mMyTable.playSelectedMode(mThemerin, mMyMidi);

  //toggle modes
  mMyTable.toggleTones(mMyMidi.velocity, mMyMidi);  

  // send values to slaves to show distance on leds
  mMyTable.sendValuesToSlaves();

  // knob - volume
  mMyTable.setVolumeByPotentiometer(mMyMidi);
  
  // knob - notes type
  mMyTable.setMidiNotesByPotentiometer(mMyMidi);
}

