#ifndef CONSTANTS_H
#define CONSTANTS_H
 
/* table setup */
const byte ELECTRODES_COUNT = 7;
/* slaves ids <30-36> */
const byte SLAVE_FIRST_ID = 30; 

/* midi setup */
const byte MAX_NOTES_TO_PLAY_AT_ONCE = 30;

/* table sensor */
const byte TONE_MODE_SINGLE = 0;
const byte TONE_MODE_MULTI = 1;
const byte TONE_MODE_ACORD = 2;
const byte TONE_MODE_MULTI_ACORD = 3;

#endif