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
			notesSetupPaanko();
			break;

			case 1:
			noteSetupArabic();
			break;      

			case 2:
			// noteSetup_3();
			break;      

			case 3:
			// noteSetup_4();
			// testSetupAcordMultiMode();
			break;      
		}
  	}

  	void notesSetupPaanko(){
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

  	void noteSetupArabic(){
  		byte THRESHOLD_LEVEL_0[] = {10, 40, 70, 100, 130, 160, 190, 220};    
		byte arr0[] = {72, 74, 73, 78, 79, 81, 82, 84};
		mTableSensors[0].setupMulti(0, 8, arr0, 8, THRESHOLD_LEVEL_0);

		byte arr_1[] = {50, 62, 63, 66, 69};
		mTableSensors[1].setupAcord(1, 4, arr_1);

		byte arr_2[] = {51, 67, 70, 72, 74};
		mTableSensors[2].setupAcord(2, 4, arr_2);
  		
  		mTableSensors[3].setupSingle(3, 79);
  		mTableSensors[4].setupSingle(4, 78);
  		mTableSensors[5].setupSingle(5, 75);
  		mTableSensors[6].setupSingle(6, 74);
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

  	void testAcordMode(){
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
	    // acord multi mode - 3 acords, 5 notes
		byte AMM_THRESHOLD_LEVEL_0[] = {20, 60, 120};
		byte arr_0[3][5] = { {50, 62, 63, 66, 69}, {51, 67, 70, 72, 74}, {50, 62, 63, 66, 69}};    
		mTableSensors[0].setupAcordMulti(0, 3, 5, arr_0, 3, AMM_THRESHOLD_LEVEL_0);

		// acord multi mode - 2 acords, 5 notes
		byte AMM_THRESHOLD_LEVEL_1[] = {20, 120};
		byte arr_1[2][5] = { {50, 62, 63, 66, 69}, {51, 67, 70, 72, 74}};    
		mTableSensors[1].setupAcordMulti(1, 2, 5, arr_1, 2, AMM_THRESHOLD_LEVEL_1);

		// acord multi mode - 3 acords, 4 notes
		byte AMM_THRESHOLD_LEVEL_2[] = {20, 60, 120};
		byte arr_2[3][4] = { {50, 62, 63, 66}, {51, 67, 70, 72}, {50, 62, 63, 66}};    
		mTableSensors[2].setupAcordMulti(2, 3, 4, arr_2, 3, AMM_THRESHOLD_LEVEL_2);

  	}

  	void testSetupMix(){
		// single mode
		mTableSensors[0].setupSingle(0, 60);
		mTableSensors[1].setupSingle(1, 67);


		// multi mode
		byte arr2[] = {53, 55, 57};
		byte THRESHOLD_LEVEL_2[] = {20, 60, 120};    
		mTableSensors[2].setupMulti(2, 3, arr2, 3, THRESHOLD_LEVEL_2);

		byte THRESHOLD_LEVEL_3[] = {20, 60, 120};    
		byte arr3[] = {74, 76, 77};
		mTableSensors[3].setupMulti(3, 3, arr3, 3, THRESHOLD_LEVEL_3);

		byte THRESHOLD_LEVEL_4[] = {20, 60, 120};    
		byte arr4[] = {79, 81, 52};
		mTableSensors[4].setupMulti(4, 3, arr4, 3, THRESHOLD_LEVEL_4);


		// acord mode
		byte arr5[] = {57, 61, 64, 67};
		mTableSensors[5].setupAcord(5, 4, arr5);


		// acord multi mode
		byte AMM_THRESHOLD_LEVEL_6[] = {20, 60, 120};
		byte arr6[3][5] = { {48, 50, 52, 48, 48}, {57, 61, 64, 67, 67}, {59, 60, 62, 59, 59}};    
		mTableSensors[6].setupAcordMulti(6, 3, 5, arr6, 3, AMM_THRESHOLD_LEVEL_6); 
	}
};

#endif