#ifndef NOTE_SETUP_H
#define NOTE_SETUP_H
 
#include <Arduino.h>

class NoteSetup {
	private:
   
  	public:
  	
  	TableSensor mTableSensors[ELECTRODES_COUNT];

  	void setupNotes(int idx){
		switch(idx){
			case 0: 
			testSetupSingle();
			// noteSetup_1();
			break;

			case 1:
			testMultiMode();
			// noteSetup_2();
			break;      

			case 2:
			testSetupAcord();
			// noteSetup_3();
			break;      

			case 3:
			testSetupMix();
			// noteSetup_4();
			break;      
		}
  	}

  	void noteSetup_1(){
  		//TODO
  	}

  	void noteSetup_2(){
  		//TODO
  	}
  	
  	void noteSetup_3(){
  		//TODO
  	}
  	
  	void noteSetup_4(){
  		//TODO
  	}

  	/* * * * * * TEST SETUP * * * * * */

  	void testSetupSingle(){
		mTableSensors[0].setupSingle(0, 60);
		mTableSensors[1].setupSingle(1, 62);
		mTableSensors[2].setupSingle(2, 64);
		mTableSensors[3].setupSingle(3, 65);
		mTableSensors[4].setupSingle(4, 67);
		mTableSensors[5].setupSingle(5, 69);
		mTableSensors[6].setupSingle(6, 71);
  	}

  	void testMultiMode(){
		byte THRESHOLD_LEVEL[] = {20, 60, 120};    
		byte arr0[] = {48, 50, 52};
		mTableSensors[0].setupMulti(0, 3, arr0, 3, THRESHOLD_LEVEL);
		byte arr1[] = {53, 55, 57};
		mTableSensors[1].setupMulti(1, 3, arr1, 3, THRESHOLD_LEVEL);
		byte arr2[] = {59, 60, 62};
		mTableSensors[2].setupMulti(2, 3, arr2, 3, THRESHOLD_LEVEL);
		byte arr3[] = {64, 65, 67};
		mTableSensors[3].setupMulti(3, 3, arr3, 3, THRESHOLD_LEVEL);
		byte arr4[] = {69, 71, 72};
		mTableSensors[4].setupMulti(4, 3, arr4, 3, THRESHOLD_LEVEL);
		byte arr5[] = {74, 76, 77};
		mTableSensors[5].setupMulti(5, 3, arr5, 3, THRESHOLD_LEVEL);
		byte arr6[] = {79, 81, 52};
		mTableSensors[6].setupMulti(6, 3, arr6, 3, THRESHOLD_LEVEL);
  	}	

  	void testSetupAcord(){
		byte arr20[] = {52, 55, 59, 62};
		mTableSensors[0].setupAcord(0, 4, arr20);
		byte arr21[] = {55, 59, 62, 64};
		mTableSensors[1].setupAcord(1, 4, arr21);
		byte arr22[] = {57, 61, 64, 67};
		mTableSensors[2].setupAcord(2, 4, arr22);
		byte arr23[] = {64, 64, 64, 64};
		mTableSensors[3].setupAcord(3, 4, arr23);
		byte arr24[] = {67, 67, 67, 67};
		mTableSensors[4].setupAcord(4, 4, arr24);
		byte arr25[] = {69, 69, 69, 69};
		mTableSensors[5].setupAcord(5, 4, arr25);
		byte arr26[] = {71, 71, 71, 71};
		mTableSensors[6].setupAcord(6, 4, arr26);
	}

	void testSetupAcordMultiMode(){
	    byte AMM_THRESHOLD_LEVEL[] = {20, 60, 120};
	    byte arr33[3][4] = { {48, 50, 52, 48}, {57, 61, 64, 67}, {59, 60, 62, 59}};    
	    mTableSensors[3].setupAcordMulti(3, 4, arr33, 3, AMM_THRESHOLD_LEVEL);
  	}

  	void testSetupMix(){
		// single mode
		mTableSensors[0].setupSingle(0, 60);
		mTableSensors[4].setupSingle(4, 67);

		// multi mode
		byte THRESHOLD_LEVEL[] = {20, 60, 120};    
		byte arr1[] = {53, 55, 57};
		mTableSensors[1].setupMulti(1, 3, arr1, 3, THRESHOLD_LEVEL);
		byte arr5[] = {74, 76, 77};
		mTableSensors[5].setupMulti(5, 3, arr5, 3, THRESHOLD_LEVEL);
		byte arr6[] = {79, 81, 52};
		mTableSensors[6].setupMulti(6, 3, arr6, 3, THRESHOLD_LEVEL);

		// acord mode
		byte arr22[] = {57, 61, 64, 67};
		mTableSensors[2].setupAcord(2, 4, arr22);

		// acord multi mode
		byte AMM_THRESHOLD_LEVEL[] = {20, 60, 120};
		byte arr33[3][4] = { {48, 50, 52, 48}, {57, 61, 64, 67}, {59, 60, 62, 59}};    
		mTableSensors[3].setupAcordMulti(3, 4, arr33, 3, AMM_THRESHOLD_LEVEL); 
	}
};

#endif