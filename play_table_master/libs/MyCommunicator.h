#ifndef MY_COMMUNICATOR_H
#define MY_COMMUNICATOR_H

#include <Arduino.h>

/*

****** Protocol ******
  protocol byte stream - serial connection from mater to slaves

  0 | [ID] | 1 | [VALUE]
  ID = <20-31>      // ELECTRODE_0 == 20, ..., ELECTODE_31 == 31
  VALUE = <2 - 255> // value 0 is key for id, value 1 is key for value

  example:
  0 | 20 | 1 | 123
  key | value | key | value ...
  id=20 value=123

**********************

*/
class MyCommunicator {

	public:

		static void sendMessageToSlave(byte idSlave, byte valueSlave) {
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

#endif