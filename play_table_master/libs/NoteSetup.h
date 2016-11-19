#ifndef NOTE_SETUP_H
#define NOTE_SETUP_H
 
#include <Arduino.h>

class NoteSetup {
	private:
   
  	public:
  	
  	Sensor *mTableSensors[ELECTRODES_COUNT];

  	void setupNotes(int idx, MyMidi &mMyMidi){
		switch(idx){
			case 0: 
			mMyMidi.setupMidi();
			notesSetup_0_Paanko();
			break;

			case 1:
			mMyMidi.setupMidi();
			noteSetup_1_Arabic();
			break;      

			case 2:
			mMyMidi.setupMidi();
			noteSetup_3_D_mol();
			break;      

			case 3:
			mMyMidi.setupMidi();
			noteSetup_4_Arpeggio();
			break; 

			case 4:
			mMyMidi.setupMidiPercussion();
			noteSetup_5_Drumm();
			break;      
		}

  		// testSetupSingle();
  		// testMultiMode();
  		// testAcordMode();
  		// testSetupAcordMultiMode();
  		// testSetupArpeggio();
  		// testSetupMix();
  	}

  	void notesSetup_0_Paanko(){

  		byte ID = 0;
		byte arr0[] = {52, 55, 59, 62};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr0);

		ID = 1;
		byte arr1[] = {55, 59, 62, 64};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr1);

		ID = 2;
		byte arr2[] = {57, 61, 64, 67};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr2);
		
		ID = 3;
  		mTableSensors[ID] = new SensorSingle(ID, 64);
		
		ID = 4;
  		mTableSensors[ID] = new SensorSingle(ID, 67);
		
		ID = 5;
  		mTableSensors[ID] = new SensorSingle(ID, 69);
		
		ID = 6;
  		mTableSensors[ID] = new SensorSingle(ID, 71);
	}

  	void noteSetup_1_Arabic(){
  		byte ID = 0;
  		byte THRESHOLD_LEVEL_0[] = {10, 40, 70, 100, 130, 160, 190, 220};    
		byte arr0[] = {72, 74, 75, 78, 79, 81, 82, 84};
		mTableSensors[ID] = new SensorMulti(ID, 8, arr0, 8, THRESHOLD_LEVEL_0);

		ID = 1;
		byte arr_1[] = {50, 62, 63, 66, 69};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr_1);

		ID = 2;
		byte arr_2[] = {51, 67, 70, 72, 74};
		mTableSensors[ID] = new SensorAcord(ID, 5, arr_2);
  		
		ID = 3;
  		mTableSensors[ID] = new SensorSingle(ID, 79);
		
		ID = 4;
  		mTableSensors[ID] = new SensorSingle(ID, 78);
		
		ID = 5;
  		mTableSensors[ID] = new SensorSingle(ID, 75);
		
		ID = 6;
  		mTableSensors[ID] = new SensorSingle(ID, 74);
  	}
  	
  	void noteSetup_3_D_mol(){  		
  		byte ID = 0;
		byte THRESHOLD_LEVEL_0[] = {20, 120, 180};
		byte arr_0[3][4] = {{50, 53, 57, 60}, {57, 60, 62, 65}, {62, 65, 69, 72}};    
		mTableSensors[ID] = new SensorMultiAcord(ID, 3, 4, arr_0, 3, THRESHOLD_LEVEL_0);

		ID = 1;
		byte THRESHOLD_LEVEL_1[] = {20,33,46,59,72,85,98,111,124,137,150,163,176,189,202,215,228,241};
		byte arr_1[] = {62, 64, 67, 69, 71, 72, 74, 76, 79, 81, 83, 84, 86, 88, 91, 93, 95, 96};
		mTableSensors[ID] = new SensorMulti(ID, 18, arr_1, 18, THRESHOLD_LEVEL_1);

		ID = 2;
		byte THRESHOLD_LEVEL_2[] = {20, 120};
		byte arr_2[2][4] = { {57, 60, 64, 67}, {69, 72, 76, 79}};    
		mTableSensors[ID] = new SensorMultiAcord(ID, 2, 4, arr_2, 2, THRESHOLD_LEVEL_2);

		ID = 3;
  		mTableSensors[ID] = new SensorSingle(ID, 79);
		
		ID = 4;
  		mTableSensors[ID] = new SensorSingle(ID, 78);
		
		ID = 5;
  		mTableSensors[ID] = new SensorSingle(ID, 75);
		
		ID = 6;
  		mTableSensors[ID] = new SensorSingle(ID, 74);
  	}
  	
  	void noteSetup_4_Arpeggio(){
  		byte ID = 0;
		byte arr_0[] = {53, 57, 60, 64, 65, 69, 72, 76, 72, 69, 65, 64, 60, 57};
		mTableSensors[ID] = new SensorArpeggio(ID, 14, arr_0);

		ID = 1;
		byte arr_1[] = {58, 62, 65, 69, 70, 74, 77, 81, 77, 74, 70, 69, 65, 62};
		mTableSensors[ID] = new SensorArpeggio(ID, 14, arr_1);

		ID = 2;
		byte arr_2[] = {56, 60, 63, 67, 68, 72, 75, 79, 75, 72, 68, 67, 63, 60};
		mTableSensors[ID] = new SensorArpeggio(ID, 14, arr_2);

		ID = 3;
  		mTableSensors[ID] = new SensorSingle(ID, 84);
		
		ID = 4;
  		mTableSensors[ID] = new SensorSingle(ID, 79);
		
		ID = 5;
  		mTableSensors[ID] = new SensorSingle(ID, 77);
		
		ID = 6;
  		mTableSensors[ID] = new SensorSingle(ID, 72);
  	}

  	void noteSetup_5_Drumm(){
  		byte ID = 0;
		mTableSensors[ID] = new SensorSingle(ID, 49);

  		ID = 1;
  		mTableSensors[ID] = new SensorSingle(ID, 47);
  		
  		ID = 2;
  		mTableSensors[ID] = new SensorSingle(ID, 41);

		ID = 3;
  		mTableSensors[ID] = new SensorSingle(ID, 46);
		
		ID = 4;
  		mTableSensors[ID] = new SensorSingle(ID, 42);
		
		ID = 5;
  		mTableSensors[ID] = new SensorSingle(ID, 40);
		
		ID = 6;
  		mTableSensors[ID] = new SensorSingle(ID, 36);
  	}

 //  	/* * * * * * TEST SETUP * * * * * */
  	
  	void testSetupSingle(){
  		
  		byte ID = 0;
		mTableSensors[ID] = new SensorSingle(ID, 60);

		ID = 1;
		mTableSensors[ID] = new SensorSingle(ID, 62);
		
		ID = 2;
		mTableSensors[ID] = new SensorSingle(ID, 64);
		
		ID = 3;
		mTableSensors[ID] = new SensorSingle(ID, 65);
		
		ID = 4;
		mTableSensors[ID] = new SensorSingle(ID, 67);
		
		ID = 5;
		mTableSensors[ID] = new SensorSingle(ID, 69);
		
		ID = 6;
		mTableSensors[ID] = new SensorSingle(ID, 71);
  	}

  	void testMultiMode(){
		byte THRESHOLD_LEVEL[] = {20, 60, 120};

  		byte ID = 0;
		byte arr0[] = {48, 50, 52};
		mTableSensors[ID] = new SensorMulti(ID, 3, arr0, 3, THRESHOLD_LEVEL);

  		ID = 1;
		byte arr1[] = {53, 55, 57};
		mTableSensors[ID] = new SensorMulti(ID, 3, arr1, 3, THRESHOLD_LEVEL);
  		
  		ID = 2;
		byte arr2[] = {59, 60, 62};
		mTableSensors[ID] = new SensorMulti(ID, 3, arr2, 3, THRESHOLD_LEVEL);
  		
  		ID = 3;
		byte arr3[] = {64, 65, 67};
		mTableSensors[ID] = new SensorMulti(ID, 3, arr3, 3, THRESHOLD_LEVEL);
  		
  		ID = 4;
		byte arr4[] = {69, 71, 72};
		mTableSensors[ID] = new SensorMulti(ID, 3, arr4, 3, THRESHOLD_LEVEL);
  		
  		ID = 5;
		byte arr5[] = {74, 76, 77};
		mTableSensors[5] = new SensorMulti(ID, 3, arr5, 3, THRESHOLD_LEVEL);
  		
  		ID = 6;
		byte arr6[] = {79, 81, 52};
		mTableSensors[ID] = new SensorMulti(ID, 3, arr6, 3, THRESHOLD_LEVEL);
  	}	

  	void testAcordMode(){
  		byte ID = 0;
		byte arr0[] = {52, 55, 59, 62};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr0);
  		
		ID = 1;
		byte arr1[] = {55, 59, 62, 64};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr1);
		
		ID = 2;
		byte arr2[] = {57, 61, 64, 67};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr2);
		
		ID = 3;
		byte arr3[] = {64, 64, 64, 64};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr3);
		
		ID = 4;
		byte arr4[] = {67, 67, 67, 67};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr4);
		
		ID = 5;
		byte arr5[] = {69, 69, 69, 69};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr5);

		ID = 6;
		byte arr6[] = {71, 71, 71, 71};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr6);
	}

	void testSetupAcordMultiMode(){
  		byte ID = 0;
	    // acord multi mode - 3 acords, 5 notes
		byte THRESHOLD_LEVEL_0[] = {20, 60, 120};
		byte arr_0[3][5] = { {50, 62, 63, 66, 69}, {51, 67, 70, 72, 74}, {50, 62, 63, 66, 69}};    
		mTableSensors[ID] = new SensorMultiAcord(ID, 3, 5, arr_0, 3, THRESHOLD_LEVEL_0);

		// acord multi mode - 2 acords, 5 notes
  		ID = 1;
		byte THRESHOLD_LEVEL_1[] = {20, 120};
		byte arr_1[2][5] = { {50, 62, 63, 66, 69}, {51, 67, 70, 72, 74}};    
		mTableSensors[ID] = new SensorMultiAcord(ID, 2, 5, arr_1, 2, THRESHOLD_LEVEL_1);

		// acord multi mode - 3 acords, 4 notes
  		ID = 2;
		byte THRESHOLD_LEVEL_2[] = {20, 60, 120};
		byte arr_2[3][4] = { {50, 62, 63, 66}, {51, 67, 70, 72}, {50, 62, 63, 66}};    
		mTableSensors[ID] = new SensorMultiAcord(ID, 3, 4, arr_2, 3, THRESHOLD_LEVEL_2);
  		
  		ID = 3; // not set
  		ID = 4; // not set
  		ID = 5; // not set
  		ID = 6; // not set
  	}

  	void testSetupArpeggio(){
  		byte ID = 0;
		byte arr_0[] = {50, 62, 63, 66, 69};
		mTableSensors[ID] = new SensorArpeggio(ID, 5, arr_0);

		ID = 1;
		byte arr_1[] = {50, 62, 63, 66, 69};
		mTableSensors[ID] = new SensorArpeggioAcord(ID, 5, arr_1);
  		
	    ID = 1; // not set
	    ID = 2; // not set
  		ID = 3; // not set
  		ID = 4; // not set
  		ID = 5; // not set
  		ID = 6; // not set
  	}

  	void testSetupMix(){

		// single mode
  		byte ID = 0;
		mTableSensors[ID] = new SensorSingle(ID, 67);

		ID = 1;
		mTableSensors[ID] = new SensorSingle(ID, 53);

		// multi mode  	
  		ID = 2;
		byte THRESHOLD_LEVEL_2[] = {20, 60, 120};    
		byte arr2[] = {74, 76, 77};
		mTableSensors[ID] = new SensorMulti(ID, 3, arr2, 3, THRESHOLD_LEVEL_2);

		// acord mode
  		ID = 3;
		byte arr3[] = {57, 61, 64, 67};
		mTableSensors[ID] = new SensorAcord(ID, 4, arr3);

		// acord multi mode
  		ID = 4;
		byte AMM_THRESHOLD_LEVEL_4[] = {20, 60, 120};
		byte arr4[3][5] = { {48, 50, 52, 48, 48}, {57, 61, 64, 67, 67}, {59, 60, 62, 59, 59}};    
		mTableSensors[ID] = new SensorMultiAcord(ID, 3, 5, arr4, 3, AMM_THRESHOLD_LEVEL_4); 
  		
  		// arpeggio mode
  		ID = 5;
		byte arr5[] = {50, 62, 63, 66, 69};
		mTableSensors[ID] = new SensorArpeggio(ID, 5, arr5);

		// arpeggio acord mode
  		ID = 6;
		byte arr6[] = {50, 62, 63, 66, 69};
		mTableSensors[ID] = new SensorArpeggioAcord(ID, 5, arr6);
	}
	
};

#endif