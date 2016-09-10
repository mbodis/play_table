/*******************************************************************************

  2016-03-26
  Miroslav Bodis - play table - slave - testing led, and connections

*******************************************************************************/

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

const int ALL_LEDS[] = {
  MY_LED_1, MY_LED_2, 
  MY_LED_3, MY_LED_4, MY_LED_5, 
  MY_LED_6, MY_LED_7, MY_LED_8, 
  MY_LED_9, MY_LED_10, MY_LED_11,
  MY_LED_12, MY_LED_13
};

// testing
int testNumber = 2;
int testNumber2 = 128;

/*
 *  MAIN - SETUP
 */
void setup() {
//  setupLeds();
  setupSerialCommunication();
}

void setupLeds() {
  for (int i = 0; i < COUNT_LEDS; i++) {
    pinMode(ALL_LEDS[i], OUTPUT);
  }
}

void setupSerialCommunication() {
  Serial.begin(9600);
}

/*
 *  MAIN - LOOP
 */
void loop() {
//  testBlinkingInBulidLED();
//  testBlinking();
  testSerialCommunication();
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

void testSerialCommunication() {
  for (int i=30; i<38; i++){
    Serial.write(0); //ID DELIMETER
    Serial.write(i); //ID
    Serial.write(1); //VALUE DELIMETER
    if (i%2 ==0){
      Serial.write(testNumber);//VALUE
    }else{
      Serial.write(testNumber2);//VALUE  
    }
    
  }  

  testNumber ++;
  testNumber = testNumber % 256;
  if (testNumber < 2){
    testNumber = 2;
  }

  testNumber2 ++;
  testNumber2 = testNumber2 % 256;

  if (testNumber2 < 2){
    testNumber2 = 2;
  }

  delay(50);
}


