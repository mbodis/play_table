/*******************************************************************************

  created: 2016-03-26
  modified: 2016-11-25
  Miroslav Bodis - play table - master

  MIDI docs:
  http://www.ccarh.org/whiteNotescourses/253/handout/midiprotocol/
  http://www.music-softwwhiteNotesare-development.com/midi-tutorial.html
  http://www.gweep.net/~whiteNotesprefect/eng/reference/protocol/midispec.html#Aft

*******************************************************************************/
#include <MPR121.h>

#include <Constants.h>
#include <MyMidi.h>
#include <Themerin.h>
#include <Sensor.h>
#include <SensorSingle.h>
#include <SensorMulti.h>
#include <SensorAcord.h>
#include <SensorMultiAcord.h>
#include <SensorArpeggio.h>
#include <SensorArpeggioAcord.h>
#include <NoteSetup.h>
#include <NoteGroup.h>
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
  // setup midi
  mMyMidi.init();
  
  mMyTable.setupButton();

  // set default notes
  mMyTable.setupNotes(mMyMidi, 0, false);

  // setup comunication
  mMyTable.setupSerialCommunication();

  // setup touch electrodes
  mTouchSensors.setupTouchElectrodes();

  // setup proximity limits
  mTouchSensors.setupProximity();

  // initialise MIDI
  mMyMidi.setupMidiInit(true);

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

