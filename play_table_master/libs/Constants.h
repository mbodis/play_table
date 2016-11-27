#ifndef CONSTANTS_H
#define CONSTANTS_H
 
/* table setup */
const byte ELECTRODES_COUNT = 7;
/* slaves ids <30-36> */
const byte SLAVE_FIRST_ID = 30; 

/* table sensor */
const byte TONE_MODE_SINGLE = 0;
const byte TONE_MODE_MULTI = 1;
const byte TONE_MODE_ACORD = 2;
const byte TONE_MODE_MULTI_ACORD = 3;
const byte TONE_MODE_ARPEGGIO = 4;
const byte TONE_MODE_ARPEGGIO_ACORD = 5;

/*
 * sensor events
 */
const byte SENSOR_STATE_NO_TOUCH = 0;
const byte SENSOR_STATE_HOVER = 1;
const byte SENSOR_STATE_HAND_ABOVE = 2;
const byte SENSOR_STATE_RELEASING = 3;
const byte SENSOR_STATE_START_BLOCKING_BY_GROUP = 4;
const byte SENSOR_STATE_BLOCKED_BY_GROUP = 5;

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

#endif