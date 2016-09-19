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

SoftwareSerial mySerial(12, 10); // Soft TX on 10, we don't use RX in this code
const byte ELECTRODES_COUNT = 7;
byte noteSetupSelectedIdx = 0;


/* MIDI INSTRUMENT LIBRARY:

  MELODIC INSTRUMENTS (GM1)
  When using the Melodic bank (0x79 - same as default), open chooses an instrument and the octave to map
  To use these instruments below change "number" in talkMIDI(0xC0, number, 0) in setupMidi()


  1   Acoustic Grand Piano       33  Acoustic Bass             65  Soprano Sax           97   Rain (FX 1)
  2   Bright Acoustic Piano      34  Electric Bass (finger)    66  Alto Sax              98   Sound Track (FX 2)
  3   Electric Grand Piano       35  Electric Bass (pick)      67  Tenor Sax             99   Crystal (FX 3)
  4   Honky-tonk Piano           36  Fretless Bass             68  Baritone Sax          100  Atmosphere (FX 4)
  5   Electric Piano 1           37  Slap Bass 1               69  Oboe                  101  Brigthness (FX 5)
  6   Electric Piano 2           38  Slap Bass 2               70  English Horn          102  Goblins (FX 6)
  7   Harpsichord                39  Synth Bass 1              71  Bassoon               103  Echoes (FX 7)
  8   Clavi                      40  Synth Bass 2              72  Clarinet              104  Sci-fi (FX 8)
  9   Celesta                    41  Violin                    73  Piccolo               105  Sitar
  10  Glockenspiel               42  Viola                     74  Flute                 106  Banjo
  11  Music Box                  43  Cello                     75  Recorder              107  Shamisen
  12  Vibraphone                 44  Contrabass                76  Pan Flute             108  Koto
  13  Marimba                    45  Tremolo Strings           77  Blown Bottle          109  Kalimba
  14  Xylophone                  46  Pizzicato Strings         78  Shakuhachi            110  Bag Pipe
  15  Tubular Bells              47  Orchestral Harp           79  Whistle               111  Fiddle
  16  Dulcimer                   48  Trimpani                  80  Ocarina               112  Shanai
  17  Drawbar Organ              49  String Ensembles 1        81  Square Lead (Lead 1)  113  Tinkle Bell
  18  Percussive Organ           50  String Ensembles 2        82  Saw Lead (Lead)       114  Agogo
  19  Rock Organ                 51  Synth Strings 1           83  Calliope (Lead 3)     115  Pitched Percussion
  20  Church Organ               52  Synth Strings 2           84  Chiff Lead (Lead 4)   116  Woodblock
  21  Reed Organ                 53  Choir Aahs                85  Charang Lead (Lead 5) 117  Taiko
  22  Accordion                  54  Voice oohs                86  Voice Lead (Lead)     118  Melodic Tom
  23  Harmonica                  55  Synth Voice               87  Fifths Lead (Lead 7)  119  Synth Drum
  24  Tango Accordion            56  Orchestra Hit             88  Bass + Lead (Lead 8)  120  Reverse Cymbal
  25  Acoustic Guitar (nylon)    57  Trumpet                   89  New Age (Pad 1)       121  Guitar Fret Noise
  26  Acoutstic Guitar (steel)   58  Trombone                  90  Warm Pad (Pad 2)      122  Breath Noise
  27  Electric Guitar (jazz)     59  Tuba                      91  Polysynth (Pad 3)     123  Seashore
  28  Electric Guitar (clean)    60  Muted Trumpet             92  Choir (Pad 4)         124  Bird Tweet
  29  Electric Guitar (muted)    61  French Horn               93  Bowed (Pad 5)         125  Telephone Ring
  30  Overdriven Guitar          62  Brass Section             94  Metallic (Pad 6)      126  Helicopter
  31  Distortion Guitar          63  Synth Brass 1             95  Halo (Pad 7)          127  Applause
  32  Guitar Harmonics           64  Synth Brass 2             96  Sweep (Pad 8)         128  Gunshot

  PERCUSSION INSTRUMENTS (GM1 + GM2)

  When in the drum bank (0x78), there are not different instruments, only different notes.
  To play the different sounds, select an instrument # like 5, then play notes 27 to 87.

  27  High Q                     43  High Floor Tom            59  Ride Cymbal 2         75  Claves
  28  Slap                       44  Pedal Hi-hat [EXC 1]      60  High Bongo            76  Hi Wood Block
  29  Scratch Push [EXC 7]       45  Low Tom                   61  Low Bongo             77  Low Wood Block
  30  Srcatch Pull [EXC 7]       46  Open Hi-hat [EXC 1]       62  Mute Hi Conga         78  Mute Cuica [EXC 4]
  31  Sticks                     47  Low-Mid Tom               63  Open Hi Conga         79  Open Cuica [EXC 4]
  32  Square Click               48  High Mid Tom              64  Low Conga             80  Mute Triangle [EXC 5]
  33  Metronome Click            49  Crash Cymbal 1            65  High Timbale          81  Open Triangle [EXC 5]
  34  Metronome Bell             50  High Tom                  66  Low Timbale           82  Shaker
  35  Acoustic Bass Drum         51  Ride Cymbal 1             67  High Agogo            83 Jingle bell
  36  Bass Drum 1                52  Chinese Cymbal            68  Low Agogo             84  Bell tree
  37  Side Stick                 53  Ride Bell                 69  Casbasa               85  Castanets
  38  Acoustic Snare             54  Tambourine                70  Maracas               86  Mute Surdo [EXC 6]
  39  Hand Clap                  55  Splash Cymbal             71  Short Whistle [EXC 2] 87  Open Surdo [EXC 6]
  40  Electric Snare             56  Cow bell                  72  Long Whistle [EXC 2]
  41  Low Floor Tom              57  Crash Cymbal 2            73  Short Guiro [EXC 3]
  42  Closed Hi-hat [EXC 1]      58  Vibra-slap                74  Long Guiro [EXC 3]

*/
/**************************************************************************/
/* MY MIDI                                                               */
/**************************************************************************/
//class MyMidi {
//  private:
//  
//  public:    
//    byte resetMIDI = 8; // Tied to VS1053 Reset line
//    byte velocity = 60;  // midi note velocity for turning on and off
//   
//  // functions below are little helpers based on using the SoftwareSerial
//  // as a MIDI stream input to the VS1053 - all based on stuff from Nathan Seidle
//  
//  // Send a MIDI control-change message. // svb
//  // channel ranges from 0-15
//  //first data is the controller number (0 to 127), indicates which controller is affected by the received MIDI message.
//  //second data byte is the value to which the controller should be set, a value from 0 to 127.
//  void noteControllChange(byte channel, byte controllerNumber, byte set) {
//    talkMIDI( (0xB0 | channel), controllerNumber, set);
//  }
//  
//  // channel 0-15
//  // pressure 0-127
//  void channelPressure(byte channel, byte pressure) {
//    talkMIDI( (0xD0 | channel), pressure, 0);
//  }
//  
//  // Send a MIDI note-on message.  Like pressing a piano key.
//  // channel ranges from 0-15
//  void noteOn(byte channel, byte note, byte attack_velocity) {
//    // Serial.print("Note on: ");
//    // Serial.println(note);
//    talkMIDI( (0x90 | channel), note, attack_velocity);
//  }
//  
//  // Send a MIDI note-off message.  Like releasing a piano key.
//  void noteOff(byte channel, byte note, byte release_velocity) {
//    // Serial.print("Note off: ");
//    // Serial.println(note);
//    talkMIDI( (0x80 | channel), note, release_velocity);
//  }
//  
//  // channel 0-15
//  // note number 0-127 (where 127 is the most pressure)
//  // pressure amount, a value from 0 to 127
//  void afterTouch(byte channel, byte note, byte release_velocity) {
//    talkMIDI( (0xA0 | channel), note, release_velocity);
//  }
//  
//  // Sends a generic MIDI message. Doesn't check to see that cmd is greater than 127,
//  // or that data values are less than 127.
//  void talkMIDI(byte cmd, byte data1, byte data2) {
//    mySerial.write(cmd);
//    mySerial.write(data1);
//  
//    // Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
//    // (sort of: http://253.ccarh.org/handout/midiprotocol/)
//    if ( (cmd & 0xF0) <= 0xB0)
//      mySerial.write(data2);
//  
//  }
//  
//  // SETTING UP THE INSTRUMENT:
//  // The below function "setupMidi()" is where the instrument bank is defined. Use the VS1053 instrument library
//  // below to aid you in selecting your desire instrument from within the respective instrument bank
//  
//  void setupMidi() {
//  
//    // Setup soft serial for MIDI control
//    mySerial.begin(31250);
//    Wire.begin();
//  
//    // Reset the VS1053
//    pinMode(resetMIDI, OUTPUT);
//    digitalWrite(resetMIDI, LOW);
//    delay(100);
//    digitalWrite(resetMIDI, HIGH);
//    delay(100);
//  
//  
//    // Volume - don't comment out this code!
//    talkMIDI(0xB0, 0x07, 127); //0xB0 is channel message, set channel volume to max (127)
//  
//    // ---------------------------------------------------------------------------------------------------------
//    // Melodic Instruments GM1
//    // ---------------------------------------------------------------------------------------------------------
//    // To Play "Electric Piano" (5):
//    talkMIDI(0xB0, 0, 0x00); // Default bank GM1
//    // We change the instrument by changing the middle number in the brackets
//    // talkMIDI(0xC0, number, 0); "number" can be any number from the melodic table below
//    byte INSTRUMENT_TYPE = 79;
//    talkMIDI(0xC0, INSTRUMENT_TYPE, 0); // Set instrument number. 0xC0 is a 1 data byte command(55,0)
//    // ---------------------------------------------------------------------------------------------------------
//    // Percussion Instruments (Drums, GM1 + GM2)
//    // ---------------------------------------------------------------------------------------------------------
//    // uncomment the two lines of code below to use - you will also need to comment out the two "talkMIDI" lines
//    // of code in the Melodic Instruments section above
//    // talkMIDI(0xB0, 0, 0x78); // Bank select: drums
//    // talkMIDI(0xC0, 0, 0); // Set a dummy instrument number
//    // ---------------------------------------------------------------------------------------------------------
//  }
//
//};

/**************************************************************************/
/* THEMERIN                                                               */
/**************************************************************************/
class Themerin  {
  
  private:
  
  public:
    //Touch Board pin setup
    byte pin_pitch = 0;       // pin to control pitch
    byte pin_volume = 1;      // pint to control volume
    byte pin_instr_up = 0;    // pin to control instrument change up
    byte pin_instr_down = 1;  // pin to control instrument change down
    // Theremin setup
    byte min_note = 40;  // lowest midi pitch
    byte max_note = 100;  // highest midi pitch
    byte min_volume = 0;  // lowest volume
    byte max_volume = 127;  // highest volume (max is 127)
    byte direction_pitch = 0;  // 0 = highest pitch with touch, 1 = lowest pitch with touch
    int nonlinear_pitch = -10;  // fscale 'curve' param, 0=linear, -10 = max change at large distance
    byte direction_volume = 0;  // 0 = loudest volume with touch, 1 = lowest volume with touch
    int nonlinear_volume = -10; // fscale 'curve' param, 0=linear, -10 = max change at large distance
    // midi instrument setup
    byte instrument = 79;//80 - 1;  // starting instrument (-1 b/c I think instrument list is off by 1)
    byte min_instrument = 0;  //
    byte max_instrument = 127;  // max is 127
    // initialization stuff for proximity
    int level_pitch = 0;
    int level_pitch_old = 0;
    int level_volume = 0;
    int level_volume_old = 0;
    byte note = 0;
    byte note_old = 0;
    byte volume = 0;
    byte volume_old = 0;
    int min_level_pitch = 1000; // dummy start values - updated during running
    int max_level_pitch = 0; // dummy start values - updated during running
    int min_level_volume = 1000; // dummy start values - updated during running
    int max_level_volume = 0; // dummy start values - updated during running
    //-------------------------------------------------------------------

    float fscale( float originalMin, float originalMax, float newBegin, float
              newEnd, float inputValue, float curve) {

      float OriginalRange = 0;
      float NewRange = 0;
      float zeroRefCurVal = 0;
      float normalizedCurVal = 0;
      float rangedValue = 0;
      boolean invFlag = 0;
    
    
      // condition curve parameter
      // limit range
    
      if (curve > 10) curve = 10;
      if (curve < -10) curve = -10;
    
      curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
      curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function
    
      /*
        Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution
        Serial.println();
      */
    
      // Check for out of range inputValues
      if (inputValue < originalMin) {
        inputValue = originalMin;
      }
      if (inputValue > originalMax) {
        inputValue = originalMax;
      }
    
      // Zero Refference the values
      OriginalRange = originalMax - originalMin;
    
      if (newEnd > newBegin) {
        NewRange = newEnd - newBegin;
      }
      else
      {
        NewRange = newBegin - newEnd;
        invFlag = 1;
      }
    
      zeroRefCurVal = inputValue - originalMin;
      normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float
    
      /*
        Serial.print(OriginalRange, DEC);
        Serial.print("   ");
        Serial.print(NewRange, DEC);
        Serial.print("   ");
        Serial.println(zeroRefCurVal, DEC);
        Serial.println();
      */
    
      // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
      if (originalMin > originalMax ) {
        return 0;
      }
    
      if (invFlag == 0) {
        rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
    
      }
      else     // invert the ranges
      {
        rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
      }
    
      return rangedValue;
    }
  
      void playThereminMode(MyMidi mMyMidi) {
    // pitch
    level_pitch_old = level_pitch;
    level_pitch = MPR121.getFilteredData(pin_pitch);
  
    if (level_pitch != level_pitch_old) {
      // dynamically setup level max and mins
      if (level_pitch > max_level_pitch) {
        max_level_pitch = level_pitch;
      }
      if (level_pitch < min_level_pitch) {
        min_level_pitch = level_pitch;
      }
  
      // turn off notes if level rises near baseline
      if (fscale(min_level_pitch, max_level_pitch, 0, 1, level_pitch, nonlinear_pitch) >= 0.95) {
        mMyMidi.noteOff(0, note_old, mMyMidi.velocity);
        mMyMidi.noteOff(0, note, mMyMidi.velocity);
        note_old = note;
      }
      // set note
      else {
        if (direction_pitch == 0) {
          note = fscale(min_level_pitch, max_level_pitch, max_note, min_note, level_pitch, nonlinear_pitch);
        }
        else if (direction_pitch == 1) {
          note = fscale(min_level_pitch, max_level_pitch, min_note, max_note, level_pitch, nonlinear_pitch);
        }
        if (note != note_old) {
          mMyMidi.noteOn(0, note, mMyMidi.velocity);  // turn on new note
          mMyMidi.noteOff(0, note_old, mMyMidi.velocity);  // turn off old note
          note_old = note;
        }
      }
    }
  
    // volume
    level_volume_old = level_volume;
    level_volume = MPR121.getFilteredData(pin_volume);
    if (level_volume != level_volume_old) {
      // dynamically setup level max and mins
      if (level_volume > max_level_volume) {
        max_level_volume = level_volume;
      }
      if (level_volume < min_level_volume) {
        min_level_volume = level_volume;
      }
      // set volume
      else {
        if (direction_volume == 0) {
          volume = fscale(min_level_volume, max_level_volume, max_volume, min_volume, level_volume, nonlinear_volume);
        }
        else if (direction_volume == 1) {
          volume = fscale(min_level_volume, max_level_volume, min_volume, max_volume, level_volume, nonlinear_volume);
        }
        if (volume != volume_old) {
          mMyMidi.talkMIDI(0xB0, 0x07, volume); //0xB0 is channel message, set channel volume to near max (127)
          volume_old = volume;
          // Serial.print("Volume: ");
          // Serial.println(volume);
        }
      }
    }
  }
};

/**************************************************************************/
/* TABLE SENSOR                                                           */
/**************************************************************************/
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
            mMyMidi.noteOn(0, noteValueAcordMulti[acord][note], mMyMidi.velocity);         
          }  
        }
      break;
    }    
  }
  
};

/**************************************************************************/
/* MY TABLE                                                               */
/**************************************************************************/
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
  
  private:
    const int buttonPin = 13;
    int buttonState = LOW;
  
  public:
    float electrodeLastValueRaw[ELECTRODES_COUNT] = { 0, 0, 0, 0, 0, 0, 0};
    int electrodeLastValues[ELECTRODES_COUNT] = {0,0,0,0,0,0,0}; // 2-255

    /* KNOB SETUP */
    const byte PIN_KNOB_VOLUME = 0;
    const byte PIN_KNOB_NOTES = 1;
    
    const byte PLAY_MODE_TONES = 0;
    const byte PLAY_MODE_THEMERIN = 1;
    const byte MODE_COUNT = 2;
  
    byte selectedMode = PLAY_MODE_TONES;
  
    const byte ELECTRODES_ARR[ELECTRODES_COUNT] = {
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

  void setupNotes(byte newNoteSetupIdx){
    //TODO switch by idx
    testNotesSetup();
  }

  void testNotesSetup(){
    // single mode
    mTableSensors[0].setupSingle(0, 60);
//    mTableSensors[1].setupSingle(1, 62);
//    mTableSensors[2].setupSingle(2, 64);
//    mTableSensors[3].setupSingle(3, 65);
//    mTableSensors[4].setupSingle(4, 67);
//    mTableSensors[5].setupSingle(5, 69);
//    mTableSensors[6].setupSingle(6, 71);

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
//    byte arr5[] = {74, 76, 77};
//    mTableSensors[5].setupMulti(5, 3, arr5, 3, THRESHOLD_LEVEL);
//    byte arr6[] = {79, 81, 52};
//    mTableSensors[6].setupMulti(6, 3, arr6, 3, THRESHOLD_LEVEL);

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
  void setMidiNotesByPotentiometer(MyMidi &mMyMidi){//, MyTable &mMyTable, MyCommunicator &mMyCommunicator){
    // v == <0, 1000>
    int v = abs(1000-analogRead(PIN_KNOB_NOTES));
    
    byte newNotesIdx = -1;
    if (v > 0 && v < 400){
      newNotesIdx = 0;
    }else if ( v > 600){
      newNotesIdx = 1;
    }
  
    if (newNotesIdx != -1 && newNotesIdx != noteSetupSelectedIdx){
      noteSetupSelectedIdx = newNotesIdx;        
      turnOffAllNotes(mMyMidi);
      setupNotes(noteSetupSelectedIdx);
      //initScreenSaverSensorType(mMyTable, mMyCommunicator);
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
  
};

/**************************************************************************/
/* TOUCH SENSORS                                                           */
/**************************************************************************/
class TouchSensors{
  #define LOW_DIFF 0
  #define HIGH_DIFF 50
  #define filterWeight 0.3f // 0.0f to 1.0f - higher value = more smoothing
  #define MPR121_ADDR 0x5C
  
  private:

  public:

    void updateReading() {
      MPR121.updateAll();
    }
    
    void readValuesFromElectrodes(MyTable &mMyTable) {
      for (byte i = 0; i < ELECTRODES_COUNT; i++) {
        readElectrodeValue(mMyTable, mMyTable.ELECTRODES_ARR[i]);        
      }
    
    }
    
    void readElectrodeValue(MyTable &mMyTable, byte electrode) {
    
      // read the difference between the measured baseline and the measured continuous data
      int reading = MPR121.getBaselineData(electrode) - MPR121.getFilteredData(electrode);
    
      // print out the reading value for debug
      //  Serial.print("Proximity electrode: ");
      //  Serial.println(reading);
    
      // constrain the reading between our low and high mapping values
      int prox = constrain(reading, LOW_DIFF, HIGH_DIFF);
    
      // implement a simple (IIR lowpass) smoothing filter
      float newProx = (filterWeight * mMyTable.electrodeLastValueRaw[electrode]) + ((1 - filterWeight) * (float)prox);
    
      //newProx  - is now between 0 - 50
      mMyTable.electrodeLastValueRaw[electrode] = newProx;  
    
      // save values for slaves
      if ((int)newProx > 0) {
        mMyTable.electrodeLastValues[electrode] = (int)(newProx * 5.5); // value is now between 0-255
      } else {
        mMyTable.electrodeLastValues[electrode] = (int)newProx;
      }
    
      // do some limit filtering
      if (mMyTable.electrodeLastValues[electrode] > 255) {
        mMyTable.electrodeLastValues[electrode] = 255;
      }
      if (mMyTable.electrodeLastValues[electrode] < 2) {
        mMyTable.electrodeLastValues[electrode] = 2;
      }
    
    }
    
    void setupTouchElectrodes() {
      if (!MPR121.begin(MPR121_ADDR)) {
        Serial.println("error setting up MPR121");
        switch (MPR121.getError()) {
          case NO_ERROR:
            Serial.println("no error");
            break;
          case ADDRESS_UNKNOWN:
            Serial.println("incorrect address");
            break;
          case READBACK_FAIL:
            Serial.println("readback failure");
            break;
          case OVERCURRENT_FLAG:
            Serial.println("overcurrent on REXT pin");
            break;
          case OUT_OF_RANGE:
            Serial.println("electrode out of range");
            break;
          case NOT_INITED:
            Serial.println("not initialised");
            break;
          default:
            Serial.println("unknown error");
            break;
        }
        while (1);
      }
    
      MPR121.updateAll();
    
    }
    
    void setupProximity() {
      // slow down some of the MPR121 baseline filtering to avoid
      // filtering out slow hand movements
      //  MPR121.setRegister(NHDF, 0x01); //noise half delta (falling)
      //  MPR121.setRegister(FDLF, 0x3F); //filter delay limit (falling)
    }
};

/**************************************************************************/
/* MY COMMUNICATOR                                                        */
/**************************************************************************/
class MyCommunicator{
  #define PROTOCOL_ID_DELIMETER 0
  #define PROTOCOL_VALUE_DELIMETER 1

  private:
    
  public:

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
    void sendValuesToSlaves(MyTable &mMyTable) {
      for (byte i = 0; i < ELECTRODES_COUNT; i++) {
        sendMessageToSlave(i+30, mMyTable.electrodeLastValues[i]);
      }
    }    
    
    void sendMessageToSlave(byte idSlave, byte valueSlave) {
      Serial1.write(PROTOCOL_ID_DELIMETER);
      Serial1.write(idSlave);
      Serial1.write(PROTOCOL_VALUE_DELIMETER);
      Serial1.write(valueSlave);
    }

    /*
     * testing communication
     */
     // testing
    byte testNumber = 0;
    byte testNumber2 = 128;
    void testSerialCommunication() {
      Serial1.write(PROTOCOL_ID_DELIMETER); //ID DELIMETER
      Serial1.write(20); //ID
      Serial1.write(PROTOCOL_VALUE_DELIMETER); //VALUE DELIMETER
      Serial1.write(testNumber);//VALUE
    
      Serial1.write(PROTOCOL_ID_DELIMETER); //ID DELIMETER
      Serial1.write(21); //ID
      Serial1.write(PROTOCOL_VALUE_DELIMETER); //VALUE DELIMETER
      Serial1.write(testNumber2);//VALUE
    
      testNumber ++;
      testNumber = testNumber % 256;
    
    
      testNumber2 ++;
      testNumber2 = testNumber2 % 256;
    
      delay(50);
    }

};

/**************************************************************************/
/* VISUAL EFFECTS                                                         */
/**************************************************************************/
class VisualEffects{

  private:

  public:

    void initScreenSaverSensorType(MyTable &mMyTable, MyCommunicator &mMyCommunicator){

      int multiTone = 42;
      int acordMultiTone = 42;
      
      for (byte n = 0; n<9; n++){  
        for (byte sensor = 0; sensor<ELECTRODES_COUNT; sensor++){  
          
          if (mMyTable.mTableSensors[sensor].getMode() == 0 || mMyTable.mTableSensors[sensor].getMode() == 2){
            if (n%2 == 0){
              mMyCommunicator.sendMessageToSlave(mMyTable.mTableSensors[sensor].getSensorId()+30, 255);
            }else{
              mMyCommunicator.sendMessageToSlave(mMyTable.mTableSensors[sensor].getSensorId()+30, 2);
            }
          }

          if (mMyTable.mTableSensors[sensor].getMode() == 1){
            if ((n%4)==0){
              multiTone+=80;  
              if (multiTone >255)multiTone=255;
            }
            mMyCommunicator.sendMessageToSlave(mMyTable.mTableSensors[sensor].getSensorId()+30, multiTone);
          }

          if (mMyTable.mTableSensors[sensor].getMode() == 3){
            if ((n%4)==0){
              acordMultiTone+=80;  
              if (acordMultiTone >255)acordMultiTone=255;
            }
            mMyCommunicator.sendMessageToSlave(mMyTable.mTableSensors[sensor].getSensorId()+30, multiTone);
          }
                  
        }
        delay(300);
      }
    }
    
    void initScreenSaver(MyCommunicator &mMyCommunicator){
    
      for (byte loop = 0; loop<2; loop++){  
        int val = 0;
        while(true){
          for (int ele = 30; ele<38; ele++){    
            mMyCommunicator.sendMessageToSlave(ele, val);      
          }
            delay(25);
            val+=10;    
            if (val > 255)break;
        }
      
        val = 255;
        while(true){
          for (byte ele = 30; ele<38; ele++){    
            mMyCommunicator.sendMessageToSlave(ele, val);      
          }
            delay(25);
            val-=10;
            if (val < 0)break;    
        }
      }  
    }
    
    void initScreenSaver2(MyCommunicator &mMyCommunicator){
      int val = 0;
      
        while(true){
          mMyCommunicator.sendMessageToSlave(30, (val+15)%255);
          mMyCommunicator.sendMessageToSlave(31, (val+30)%255);
          mMyCommunicator.sendMessageToSlave(32, (val+45)%255);
          mMyCommunicator.sendMessageToSlave(33, (val+60)%255);
          mMyCommunicator.sendMessageToSlave(34, (val+75)%255);
          mMyCommunicator.sendMessageToSlave(35, (val+90)%255);
          mMyCommunicator.sendMessageToSlave(36, (val+105)%255);
          delay(25);
          val+=10;    
          if (val > 2000)break;
        }
    }
};



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

  mMyTable.setupButton();

  // set default notes
  mMyTable.setupNotes(noteSetupSelectedIdx);

  // setup comunication
  mMyCommunicator.setupSerialCommunication();

  // setup touch electrodes
  mTouchSensors.setupTouchElectrodes();

  // setup proximity limits
  mTouchSensors.setupProximity();

  // initialise MIDI
  mMyMidi.setupMidi();

  // do some initial led effect
  mVisualEffects.initScreenSaver(mMyCommunicator);  
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
  mMyCommunicator.sendValuesToSlaves(mMyTable);

  // knob - volume
  mMyTable.setVolumeByPotentiometer(mMyMidi);
  
  // knob - notes type
  mMyTable.setMidiNotesByPotentiometer(mMyMidi);//, mMyTable, mMyCommunicator);
}





