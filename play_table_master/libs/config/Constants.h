#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
 * sensor types
 */
const byte TONE_MODE_SINGLE = 0;
const byte TONE_MODE_MULTI = 1;
const byte TONE_MODE_CHORD = 2;
const byte TONE_MODE_MULTI_CHORD = 3;
const byte TONE_MODE_ARPEGGIO = 4;

/*
 * sensor touch states
 */
const byte SENSOR_STATE_NO_TOUCH = 0;
const byte SENSOR_STATE_HOVER = 1;
const byte SENSOR_STATE_HAND_ABOVE = 2;
const byte SENSOR_STATE_RELEASING = 3;
const byte SENSOR_STATE_START_BLOCKING_BY_GROUP = 4;
const byte SENSOR_STATE_BLOCKED_BY_GROUP = 5;

/*
 * pin setup
 */
const byte PIN_KNOB_VOLUME = 0;
const byte PIN_KNOB_NOTES = 1;
const byte PIN_BUTTON_TABLE_BOTTOM_SIDE = 13;

const byte PLAY_MODE_TONES = 0;
const byte PLAY_MODE_THEMERIN = 1;
const byte MODE_COUNT = 2;

/*
 * protocol master / slave
 */
#define PROTOCOL_ID_DELIMETER 0
#define PROTOCOL_VALUE_DELIMETER 1
/* slaves ids <30-36> */
const byte SLAVE_FIRST_ID = 30;

/*
 * electrode setup
 */
const byte ELECTRODES_COUNT = 7;
#define PROX_ELECTRODE_0 0
#define PROX_ELECTRODE_1 1
#define PROX_ELECTRODE_2 2
#define PROX_ELECTRODE_3 3
#define PROX_ELECTRODE_4 4
#define PROX_ELECTRODE_5 5
#define PROX_ELECTRODE_6 6

const byte ELECTRODES_ARR[ELECTRODES_COUNT] = {
	PROX_ELECTRODE_0, PROX_ELECTRODE_1, PROX_ELECTRODE_2,
	PROX_ELECTRODE_3, PROX_ELECTRODE_4, PROX_ELECTRODE_5,
	PROX_ELECTRODE_6
};


/*
 * ARPEGGIO
 */
const byte HAND_DISATNCE_SAVED_VALUES = 11;
const byte USER_TOUCH_LOCK_LIMIT = 15;
const byte DEFAULT_TEMPO = 65; // 115

#endif