/*******************************************************************************

  2016-03-26
  Miroslav Bodis - play table - slave


  protocol byte stream - serail connection
  0 | [ID] | 1 | [VALUE]
  ID = <30-41>
  VALUE = <2 - 255>

  example:
  0 | 30 | 1 | 123
  id=30 value=123
*******************************************************************************/

//TODO SLAVE IDENTIFICATOR <30 - 41>
#define MY_ID 30

#define MY_LED_1 1
#define MY_LED_2 2
#define MY_LED_3 3
#define MY_LED_4 4
#define MY_LED_5 5
#define MY_LED_6 6
#define MY_LED_7 7
#define MY_LED_8 8
#define MY_LED_9 9
#define MY_LED_10 10
#define MY_LED_11 11
#define MY_LED_12 12
#define MY_LED_13 13

#define COUNT_LEDS 13
#define UNDEF_ID -1
#define UNDEF_VALUE -1

const int ALL_LEDS[] = { 
  MY_LED_1, MY_LED_2, MY_LED_3,
  MY_LED_4, MY_LED_5, MY_LED_6,
  MY_LED_7, MY_LED_8, MY_LED_9,
  MY_LED_10, MY_LED_11, MY_LED_12,
  MY_LED_13
};

#define PROTOCOL_ID_DELIMETER 0
#define PROTOCOL_VALUE_DELIMETER 1

// incoming byte from serial connection
int inByte = UNDEF_VALUE;

// message identificator
boolean nextIsIdentificator = false;
int byteIdentificator = UNDEF_ID;

// message value
boolean nextIsValue = false;
int byteValue = UNDEF_VALUE;

int lastVal = UNDEF_VALUE;


/*
    MAIN - SETUP
*/
void setup() {

  // setup leds
  setupLeds();

  // setup serial connection
  setupConnection();
}

void setupLeds() {
  for (int i = 0; i < COUNT_LEDS; i++) {
    pinMode(ALL_LEDS[i], OUTPUT);
  }
}

void setupConnection() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
}

/*
    MAIN - LOOP
*/
void loop() {

  // read stream
  readValuesFromStream();

  // ligh up leds
  lightUpLeds();

  // end message
  clearMessage();
}

void readValuesFromStream() {

if (Serial.available() > 0) {  
    // get incoming byte:
    inByte = Serial.read();


    if (inByte == PROTOCOL_ID_DELIMETER) {
      nextIsIdentificator = true;

    } else if (nextIsIdentificator) {
      nextIsIdentificator = false;
      byteIdentificator = inByte;

    } else if (inByte == PROTOCOL_VALUE_DELIMETER) {
      nextIsValue = true;

    } else if (nextIsValue) {
      nextIsValue = false;
      byteValue = inByte;
      if (byteIdentificator == MY_ID){
        lastVal = byteValue;
      }

    }
}

}

void lightUpLeds() {

  if (lastVal != UNDEF_VALUE) {

    for (int i = 0; i < COUNT_LEDS; i++) {

      if ((255 / COUNT_LEDS * (i + 1)) < lastVal ) {
        digitalWrite(ALL_LEDS[i], HIGH);
      } else {
        digitalWrite(ALL_LEDS[i], LOW);
      }

    }

  }
}

void clearMessage() {

  if (nextIsIdentificator == false
      && nextIsValue == false) {

    if (byteIdentificator != UNDEF_ID
        && byteValue != UNDEF_VALUE) {

      byteIdentificator = UNDEF_ID;
      byteValue = UNDEF_VALUE;
    }
  }
}

/*
   testing circuit: in bulid led blinking
*/
void testBlinkingInBulidLED() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
}

/*
   testing circuit: light up all leds
*/
void testBlinking() {
  for (int i = 0; i < COUNT_LEDS; i++) {
    digitalWrite(ALL_LEDS[i], LOW);
    delay(500);
  }

  for (int i = 0; i < COUNT_LEDS; i++) {
    digitalWrite(ALL_LEDS[i], HIGH);
    delay(500);
  }
}


