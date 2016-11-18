#ifndef MY_MIDI_H
#define MY_MIDI_H
 
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <MPR121.h>
#include <Wire.h>


/* MIDI INSTRUMENT LIBRARY:

  http://www.ccarh.org/courses/253/handout/midiprotocol/

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
  35  Acoustic Bass Drum         51  Ride Cymbal 1             67  High Agogo            83  Jingle bell
  36  Bass Drum 1                52  Chinese Cymbal            68  Low Agogo             84  Bell tree
  37  Side Stick                 53  Ride Bell                 69  Casbasa               85  Castanets
  38  Acoustic Snare             54  Tambourine                70  Maracas               86  Mute Surdo [EXC 6]
  39  Hand Clap                  55  Splash Cymbal             71  Short Whistle [EXC 2] 87  Open Surdo [EXC 6]
  40  Electric Snare             56  Cow bell                  72  Long Whistle [EXC 2]
  41  Low Floor Tom              57  Crash Cymbal 2            73  Short Guiro [EXC 3]
  42  Closed Hi-hat [EXC 1]      58  Vibra-slap                74  Long Guiro [EXC 3]

*/


/*
  some more docs: https://github.com/rkistner/arcore
 */
class MyMidi {

  private:
    MIDIEvent e;
    byte notesOn[MAX_NOTES_TO_PLAY_AT_ONCE];

  public:    
    byte resetMIDI = 8; // Tied to VS1053 Reset line
    byte velocity = 60;  // midi note velocity for turning on and off
    
    byte RX = 12;
    byte TX = 10;
    SoftwareSerial *mySerial;
    

   void init(){

      // setup Serial
      mySerial = new SoftwareSerial(RX, TX);

      // empty playing notes array 
      for (byte b=0; b<MAX_NOTES_TO_PLAY_AT_ONCE; b++){
        notesOn[b] = 0;
      }

      // setup midi output
      midi2UsbSetup();
   }

   void addNoteToPlayList(byte note){
      for (byte b=0; b<MAX_NOTES_TO_PLAY_AT_ONCE; b++){
        if (notesOn[b] == 0){
          notesOn[b] = note;
          break;
        }
      }
   }

   void removeNoteFromPlayList(byte note){
      for (byte b=0; b<MAX_NOTES_TO_PLAY_AT_ONCE; b++){
        if (notesOn[b] == note){
          notesOn[b] = 0;
          break;
        }
      }
   }

   bool isNoteOnPlayList(byte note){
      for (byte b=0; b<MAX_NOTES_TO_PLAY_AT_ONCE; b++){
        if (notesOn[b] == note){
          return true;
        }
      }
      return false;
   }

  // functions below are little helpers based on using the SoftwareSerial
  // as a MIDI stream input to the VS1053 - all based on stuff from Nathan Seidle

  /*
   * Send a MIDI note-on message.  Like pressing a piano key.
   * channel ranges from 0-15
   * attack_velocity <1-127>
   */
  void noteOn(byte channel, byte note, byte attack_velocity) {
      addNoteToPlayList(note);
      talkMIDI( (0x90 | channel), note, attack_velocity);
      midi2Usb(0x09, 0x90, note, attack_velocity);
  }
  
  /*
   * Send a MIDI note-off message.  Like releasing a piano key.
   * attack_velocity <1-127>
   */
  void noteOff(byte channel, byte note, byte release_velocity) {
    removeNoteFromPlayList(note);
    if (!isNoteOnPlayList(note)){
      talkMIDI( (0x80 | channel), note, release_velocity);
      midi2Usb(0x08, 0x80, note, release_velocity);
    }
  }
  
  /*
   * @svb
   * channel 0-15
   * note a value from 0 to 127
   * pressure amount 0-127 (where 127 is the most pressure)
   *
   * NOTE: touchboard si not supporting afterTouch
   */ 
  void afterTouch(byte channel, byte note, byte pressure) {
      midi2Usb(0x0A, 0xA0, note, pressure);
  }

  /*
   * @svb
   * Send a MIDI control-change message. 
   * channel ranges from 0-15
   * first data is the controller number (0 to 127), indicates which controller is affected by the received MIDI message.
   * second data byte is the value to which the controller should be set, a value from 0 to 127.
   * 
   * note: not used
   */
  void noteControllChange(byte channel, byte controllerNumber, byte set) {
    talkMIDI( (0xB0 | channel), controllerNumber, set);
  }
  
  /*
   * @svb
   * channel 0-15
   * pressure 0-127
   * 
   * note: not used
   */
  void channelPressure(byte channel, byte pressure) {
    talkMIDI( (0xD0 | channel), pressure, 0);
  }
  
  /*
   * @svb
   * 
   */
  void setVolume(byte volume){
    talkMIDI(0xB0, 0x07, volume);
  }

  // Sends a generic MIDI message. Doesn't check to see that cmd is greater than 127,
  // or that data values are less than 127.
  void talkMIDI(byte cmd, byte data1, byte data2) {
    mySerial->write(cmd);
    mySerial->write(data1);
  
    // Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
    // (sort of: http://253.ccarh.org/handout/midiprotocol/)
    if ( (cmd & 0xF0) <= 0xB0)
      mySerial->write(data2);
  
  }

  void midi2UsbSetup(){
    e.type = 0x08;
    e.m3 = 127; // max volume
  }
  
  void midi2Usb(byte type, byte command, byte note, byte data){
    e.type = type; // 
    e.m1 = command; // noteOn / noteOff / pressure
    e.m2 = note; // note value
    if (data != 0){
      e.m3 = data; // data      
    }
    MIDIUSB.write(e);

    // flush USB buffer to ensure all notes are sent
    MIDIUSB.flush();
  }

  /*
   * SETTING UP THE INSTRUMENT:
   * The below function "setupMidi()" is where the instrument bank is defined. Use the VS1053 instrument library
   * below to aid you in selecting your desire instrument from within the respective instrument bank
   */
  void setupMidi() {
    // Setup soft serial for MIDI control
    mySerial->begin(31250);

    Wire.begin();
  
    // Reset the VS1053
    pinMode(resetMIDI, OUTPUT);
    digitalWrite(resetMIDI, LOW);
    delay(100);
    digitalWrite(resetMIDI, HIGH);
    delay(100);
  
    // set Volume
    setVolume(127);    
    // Bank select: Default bank GM1
    talkMIDI(0xB0, 0, 0x00); 
    // Set instrument number. 79 == Whistle
    talkMIDI(0xC0, 79, 0); 
  }


  /*
   * SETTING UP THE INSTRUMENT:
   * The below function "setupMidi()" is where the instrument bank is defined. Use the VS1053 instrument library
   * below to aid you in selecting your desire instrument from within the respective instrument bank
   */
  void setupMidiPercussion() {
    // Setup soft serial for MIDI control
    mySerial->begin(31250);

    Wire.begin();
  
    // Reset the VS1053
    pinMode(resetMIDI, OUTPUT);
    digitalWrite(resetMIDI, LOW);
    delay(100);
    digitalWrite(resetMIDI, HIGH);
    delay(100);
  
    // set volume - don't comment out this code!
    setVolume(127); //0xB0 is channel message, set channel volume to max (127)
    // Bank select: Percussion Instruments (Drums, GM1 + GM2)
    talkMIDI(0xB0, 0, 0x78); 
    // Set a dummy instrument number
    talkMIDI(0xC0, 0, 36); 
  }
  

};

#endif