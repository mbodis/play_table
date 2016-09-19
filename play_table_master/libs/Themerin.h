#ifndef THEMERIN_H
#define THEMERIN_H
 
#include <Arduino.h>

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

#endif