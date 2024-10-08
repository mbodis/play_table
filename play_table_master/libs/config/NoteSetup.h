#ifndef NOTE_SETUP_H
#define NOTE_SETUP_H

#include <Arduino.h>

class NoteSetup {

	public:

		static void setupNotes(byte idx, MyMidi &mMyMidi, Sensor *mTableSensors[], NoteGroup *mNoteGroup) {

			freeExistingSensorsFromMemory(mTableSensors);

			switch (idx) {
			case 0:
				mMyMidi.setupMidiInit(false);
				notesSetup_0_Paanko(mTableSensors);
				noGroup(mNoteGroup);
				break;

			case 1:
				mMyMidi.setupMidiInit(false);
				noteSetup_1_Arabic(mTableSensors);
				noGroup(mNoteGroup);
				break;

			case 2:
				mMyMidi.setupMidiInit(false);
				noteSetup_3_D_mol(mTableSensors);
				setGroup_D_mol(mNoteGroup);
				break;

			case 3:
				mMyMidi.setupMidiInit(false);
				noteSetup_4_Arpeggio(mTableSensors);
				// setGroup_3_4(mNoteGroup);
				noGroup(mNoteGroup);
				break;

			case 4:
				mMyMidi.setupMidiPercussionInit(false);
				noteSetup_5_Drumm(mTableSensors);
				noGroup(mNoteGroup);
				break;
			}

			// testSetupSingle(mTableSensors);
			// testMultiMode(mTableSensors);
			// testChordMode(mTableSensors);
			// testSetupChordMultiMode(mTableSensors);
			// testSetupArpeggio(mTableSensors);
			// testSetupMix(mTableSensors);
			// noGroup(mNoteGroup);
		}

		static void freeExistingSensorsFromMemory(Sensor *mTableSensors[]) {
			for (byte elc = 0; elc < ELECTRODES_COUNT; elc++) {
				delete mTableSensors[elc];
			}
		}

		static void notesSetup_0_Paanko(Sensor *mTableSensors[]) {

			byte ID = 0;
			byte arr0[] = {52, 55, 59, 62};
			mTableSensors[ID] = new SensorChord(ID, 4, arr0);

			ID = 1;
			byte arr1[] = {55, 59, 62, 64};
			mTableSensors[ID] = new SensorChord(ID, 4, arr1);

			ID = 2;
			byte arr2[] = {57, 61, 64, 67};
			mTableSensors[ID] = new SensorChord(ID, 4, arr2);

			ID = 3;
			mTableSensors[ID] = new SensorSingle(ID, 71, false);

			ID = 4;
			mTableSensors[ID] = new SensorSingle(ID, 69, false);

			ID = 5;
			mTableSensors[ID] = new SensorSingle(ID, 67, false);

			ID = 6;
			mTableSensors[ID] = new SensorSingle(ID, 64, false);
		}

		static void noteSetup_1_Arabic(Sensor *mTableSensors[]) {
			byte ID = 0;
			byte arr_1[] = {50, 62, 63, 66, 69};
			mTableSensors[ID] = new SensorChord(ID, 4, arr_1);

			ID = 1;
			byte arr_2[] = {51, 67, 70, 72, 74};
			mTableSensors[ID] = new SensorChord(ID, 5, arr_2);

			ID = 2;
			byte THRESHOLD_LEVEL_0[] = {10, 40, 70, 100, 130, 160, 190, 220};
			byte arr0[] = {72, 74, 75, 78, 79, 81, 82, 84};
			mTableSensors[ID] = new SensorMulti(ID, 8, arr0, 8, THRESHOLD_LEVEL_0);

			ID = 3;
			mTableSensors[ID] = new SensorSingle(ID, 79, true);

			ID = 4;
			mTableSensors[ID] = new SensorSingle(ID, 78, true);

			ID = 5;
			mTableSensors[ID] = new SensorSingle(ID, 75, true);

			ID = 6;
			mTableSensors[ID] = new SensorSingle(ID, 74, true);
		}

		static void noteSetup_3_D_mol(Sensor *mTableSensors[]) {
			byte ID = 0;
			// byte arr0[] = {50, 53, 57, 60};
			// mTableSensors[ID] = new SensorChord(ID, 4, arr0);
			byte THRESHOLD_LEVEL_0[] = {20, 120, 180};
			byte arr_0[3][4] = {{50, 53, 57, 60}, {57, 60, 62, 65}, {62, 65, 69, 72}};
			mTableSensors[ID] = new SensorMultiChord(ID, 3, 4, arr_0, 3, THRESHOLD_LEVEL_0);

			ID = 1;
			// byte arr1[] = {57, 60, 64, 67};
			// mTableSensors[ID] = new SensorChord(ID, 4, arr1);
			byte THRESHOLD_LEVEL_1[] = {20, 120};
			byte arr_2[2][4] = { {57, 60, 64, 67}, {69, 72, 76, 79}};
			mTableSensors[ID] = new SensorMultiChord(ID, 2, 4, arr_2, 2, THRESHOLD_LEVEL_1);

			ID = 2;
			byte THRESHOLD_LEVEL_2[] = {20, 33, 46, 59, 72, 85, 98, 111, 124, 137, 150, 163, 176, 189, 202, 215, 228, 241};
			byte arr_1[] = {62, 64, 67, 69, 71, 72, 74, 76, 79, 81, 83, 84, 86, 88, 91, 93, 95, 96};
			mTableSensors[ID] = new SensorMulti(ID, 18, arr_1, 18, THRESHOLD_LEVEL_2);


			ID = 3;
			mTableSensors[ID] = new SensorSingle(ID, 81, true);

			ID = 4;
			mTableSensors[ID] = new SensorSingle(ID, 79, true);

			ID = 5;
			mTableSensors[ID] = new SensorSingle(ID, 77, true);

			ID = 6;
			mTableSensors[ID] = new SensorSingle(ID, 74, true);
		}

		static void noteSetup_4_Arpeggio(Sensor *mTableSensors[]) {
			byte ID = 0;
			// byte arr_0[] = {53, 57, 60, 64, 65, 69, 72, 76, 72, 69, 65, 64, 60, 57};
			// mTableSensors[ID] = new SensorArpeggioAuto(ID, 14, arr_0);
			byte arr_0[] = {53, 57, 60, 64, 65, 69, 72, 76, 72, 69, 65, 64, 60, 57, 58, 62, 65, 69, 70, 74, 77, 81, 77, 74, 70, 69, 65, 62, 56, 60, 63, 67, 68, 72, 75, 79, 75, 72, 68, 67, 63, 60};
			mTableSensors[ID] = new SensorArpeggio(ID, 42, arr_0);

			ID = 1;
			byte arr_1[] = {58, 62, 65, 69, 70, 74, 77, 81, 77, 74, 70, 69, 65, 62};
			mTableSensors[ID] = new SensorArpeggio(ID, 14, arr_1);

			ID = 2;
			byte arr_2[] = {56, 60, 63, 67, 68, 72, 75, 79, 75, 72, 68, 67, 63, 60};
			mTableSensors[ID] = new SensorArpeggio(ID, 14, arr_2);

			ID = 3;
			mTableSensors[ID] = new SensorSingle(ID, 84, true);

			ID = 4;
			mTableSensors[ID] = new SensorSingle(ID, 79, true);

			ID = 5;
			mTableSensors[ID] = new SensorSingle(ID, 77, true);

			ID = 6;
			mTableSensors[ID] = new SensorSingle(ID, 72, true);
		}

		static void noteSetup_5_Drumm(Sensor *mTableSensors[]) {
			byte ID = 0;
			mTableSensors[ID] = new SensorSingle(ID, 49, true);

			ID = 1;
			mTableSensors[ID] = new SensorSingle(ID, 47, true);

			ID = 2;
			mTableSensors[ID] = new SensorSingle(ID, 41, true);

			ID = 3;
			mTableSensors[ID] = new SensorSingle(ID, 46, true);

			ID = 4;
			mTableSensors[ID] = new SensorSingle(ID, 42, true);

			ID = 5;
			mTableSensors[ID] = new SensorSingle(ID, 40, true);

			ID = 6;
			mTableSensors[ID] = new SensorSingle(ID, 36, true);
		}

		/* * * * * * group setups * * * * * */

		static void noGroup(NoteGroup *mNoteGroup) {
			for (byte group = 0; group < ELECTRODES_COUNT; group++) {
				mNoteGroup->groupIds[group] = 0;
			}
			mNoteGroup->groupMode = false;
		}

		static void setGroup_1_6(NoteGroup *mNoteGroup) {
			mNoteGroup->groupIds[0] = 1; mNoteGroup->groupIds[1] = 2;
			mNoteGroup->groupIds[2] = 2; mNoteGroup->groupIds[3] = 2;
			mNoteGroup->groupIds[4] = 2; mNoteGroup->groupIds[5] = 2;
			mNoteGroup->groupIds[6] = 2; mNoteGroup->groupMode = true;
		}

		static void setGroup_2_5(NoteGroup *mNoteGroup) {
			mNoteGroup->groupIds[0] = 1; mNoteGroup->groupIds[1] = 1;
			mNoteGroup->groupIds[2] = 2; mNoteGroup->groupIds[3] = 2;
			mNoteGroup->groupIds[4] = 2; mNoteGroup->groupIds[5] = 2;
			mNoteGroup->groupIds[6] = 2; mNoteGroup->groupMode = true;
		}

		static void setGroup_3_4(NoteGroup *mNoteGroup) {
			mNoteGroup->groupIds[0] = 1; mNoteGroup->groupIds[1] = 1;
			mNoteGroup->groupIds[2] = 1; mNoteGroup->groupIds[3] = 2;
			mNoteGroup->groupIds[4] = 2; mNoteGroup->groupIds[5] = 2;
			mNoteGroup->groupIds[6] = 2; mNoteGroup->groupMode = true;
		}

		static void setGroup_4_3(NoteGroup *mNoteGroup) {
			mNoteGroup->groupIds[0] = 1; mNoteGroup->groupIds[1] = 1;
			mNoteGroup->groupIds[2] = 1; mNoteGroup->groupIds[3] = 1;
			mNoteGroup->groupIds[4] = 2; mNoteGroup->groupIds[5] = 2;
			mNoteGroup->groupIds[6] = 2; mNoteGroup->groupMode = true;
		}

		static void setGroup_5_2(NoteGroup *mNoteGroup) {
			mNoteGroup->groupIds[0] = 1; mNoteGroup->groupIds[1] = 1;
			mNoteGroup->groupIds[2] = 1; mNoteGroup->groupIds[3] = 1;
			mNoteGroup->groupIds[4] = 1; mNoteGroup->groupIds[5] = 2;
			mNoteGroup->groupIds[6] = 2; mNoteGroup->groupMode = true;
		}

		static void setGroup_6_1(NoteGroup *mNoteGroup) {
			mNoteGroup->groupIds[0] = 1; mNoteGroup->groupIds[1] = 1;
			mNoteGroup->groupIds[2] = 1; mNoteGroup->groupIds[3] = 1;
			mNoteGroup->groupIds[4] = 1; mNoteGroup->groupIds[5] = 1;
			mNoteGroup->groupIds[6] = 2; mNoteGroup->groupMode = true;
		}

		static void setGroup_D_mol(NoteGroup *mNoteGroup) {
			mNoteGroup->groupIds[0] = 1; mNoteGroup->groupIds[1] = 1;
			mNoteGroup->groupIds[2] = 2; mNoteGroup->groupIds[3] = 3;
			mNoteGroup->groupIds[4] = 3; mNoteGroup->groupIds[5] = 3;
			mNoteGroup->groupIds[6] = 3; mNoteGroup->groupMode = true;
		}

		/* * * * * * TEST SETUP * * * * * */

		static void testSetupSingle(Sensor *mTableSensors[]) {

			byte ID = 0;
			mTableSensors[ID] = new SensorSingle(ID, 60, true);

			ID = 1;
			mTableSensors[ID] = new SensorSingle(ID, 62, true);

			ID = 2;
			mTableSensors[ID] = new SensorSingle(ID, 64, true);

			ID = 3;
			mTableSensors[ID] = new SensorSingle(ID, 65, true);

			ID = 4;
			mTableSensors[ID] = new SensorSingle(ID, 67, true);

			ID = 5;
			mTableSensors[ID] = new SensorSingle(ID, 69, true);

			ID = 6;
			mTableSensors[ID] = new SensorSingle(ID, 71, true);
		}

		static void testMultiMode(Sensor *mTableSensors[]) {
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

		static void testChordMode(Sensor *mTableSensors[]) {
			byte ID = 0;
			byte arr0[] = {52, 55, 59, 62};
			mTableSensors[ID] = new SensorChord(ID, 4, arr0);

			ID = 1;
			byte arr1[] = {55, 59, 62, 64};
			mTableSensors[ID] = new SensorChord(ID, 4, arr1);

			ID = 2;
			byte arr2[] = {57, 61, 64, 67};
			mTableSensors[ID] = new SensorChord(ID, 4, arr2);

			ID = 3;
			byte arr3[] = {64, 64, 64, 64};
			mTableSensors[ID] = new SensorChord(ID, 4, arr3);

			ID = 4;
			byte arr4[] = {67, 67, 67, 67};
			mTableSensors[ID] = new SensorChord(ID, 4, arr4);

			ID = 5;
			byte arr5[] = {69, 69, 69, 69};
			mTableSensors[ID] = new SensorChord(ID, 4, arr5);

			ID = 6;
			byte arr6[] = {71, 71, 71, 71};
			mTableSensors[ID] = new SensorChord(ID, 4, arr6);
		}

		static void testSetupChordMultiMode(Sensor *mTableSensors[]) {
			byte ID = 0;
			// chord multi mode - 3 chords, 5 notes
			byte THRESHOLD_LEVEL_0[] = {20, 60, 120};
			byte arr_0[3][5] = { {50, 62, 63, 66, 69}, {51, 67, 70, 72, 74}, {50, 62, 63, 66, 69}};
			mTableSensors[ID] = new SensorMultiChord(ID, 3, 5, arr_0, 3, THRESHOLD_LEVEL_0);

			// chord multi mode - 2 chords, 5 notes
			ID = 1;
			byte THRESHOLD_LEVEL_1[] = {20, 120};
			byte arr_1[2][5] = { {50, 62, 63, 66, 69}, {51, 67, 70, 72, 74}};
			mTableSensors[ID] = new SensorMultiChord(ID, 2, 5, arr_1, 2, THRESHOLD_LEVEL_1);

			// chord multi mode - 3 chords, 4 notes
			ID = 2;
			byte THRESHOLD_LEVEL_2[] = {20, 60, 120};
			byte arr_2[3][4] = { {50, 62, 63, 66}, {51, 67, 70, 72}, {50, 62, 63, 66}};
			mTableSensors[ID] = new SensorMultiChord(ID, 3, 4, arr_2, 3, THRESHOLD_LEVEL_2);

			ID = 3; // not set
			ID = 4; // not set
			ID = 5; // not set
			ID = 6; // not set
		}

		static void testSetupArpeggio(Sensor *mTableSensors[]) {
			byte ID = 0;
			byte arr_0[] = {50, 62, 63, 66, 69};
			mTableSensors[ID] = new SensorArpeggio(ID, 5, arr_0);

			ID = 1;
			byte arr_1[] = {50, 62, 63, 66, 69};
			mTableSensors[ID] = new SensorArpeggioChord(ID, 5, arr_1);

			ID = 2;
			byte arr_2[] = {50, 62, 63, 66, 69};
			mTableSensors[ID] = new SensorArpeggioCombi(ID, 5, arr_2);

			ID = 3;
			byte arr_3[] = {50, 62, 63, 66, 69};
			mTableSensors[ID] = new SensorArpeggioAuto(ID, 5, arr_3);

			ID = 4; // not set
			ID = 5; // not set
			ID = 6; // not set
		}

		static void testSetupMix(Sensor *mTableSensors[]) {

			// single mode
			byte ID = 0;
			mTableSensors[ID] = new SensorSingle(ID, 67, true);

			ID = 1;
			mTableSensors[ID] = new SensorSingle(ID, 53, true);

			// multi mode
			ID = 2;
			byte THRESHOLD_LEVEL_2[] = {20, 60, 120};
			byte arr2[] = {74, 76, 77};
			mTableSensors[ID] = new SensorMulti(ID, 3, arr2, 3, THRESHOLD_LEVEL_2);

			// chord mode
			ID = 3;
			byte arr3[] = {57, 61, 64, 67};
			mTableSensors[ID] = new SensorChord(ID, 4, arr3);

			// chord multi mode
			ID = 4;
			byte AMM_THRESHOLD_LEVEL_4[] = {20, 60, 120};
			byte arr4[3][5] = { {48, 50, 52, 48, 48}, {57, 61, 64, 67, 67}, {59, 60, 62, 59, 59}};
			mTableSensors[ID] = new SensorMultiChord(ID, 3, 5, arr4, 3, AMM_THRESHOLD_LEVEL_4);

			// arpeggio mode
			ID = 5;
			byte arr5[] = {50, 62, 63, 66, 69};
			mTableSensors[ID] = new SensorArpeggio(ID, 5, arr5);

			// arpeggio chord mode
			ID = 6;
			byte arr6[] = {50, 62, 63, 66, 69};
			mTableSensors[ID] = new SensorArpeggioChord(ID, 5, arr6);
		}

};

#endif