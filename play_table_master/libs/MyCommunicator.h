#ifndef MY_COMMUNICATOR_H
#define MY_COMMUNICATOR_H
 
#include <Arduino.h>

class MyCommunicator{
  

  private:
    
  public:

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

#endif