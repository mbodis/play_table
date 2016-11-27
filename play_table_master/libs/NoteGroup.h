#ifndef NOTEGROUP_H
#define NOTEGROUP_H

#include <Arduino.h>

/*
 *
 * defines groups of notes
 * keeps list of notes in separate groups
 * only one note can be played from the group at a time
 */     
class NoteGroup{
	private:

  	public:

   		/*
		 * 0 == disabled
		 * <1-7> group identifier
		 *
		 * example: {1,1,1,2,2,2,2}- two groups (one with 3 notes, second with 4 notes) 
		 * example: {0,0,0,0,0,1,1}- one group (contains 2 notes) 
	  	 */
	  	byte groupIds[ELECTRODES_COUNT];

	  	/*
	  	 * SENSOR_STATE_NO_TOUCH = 0
		 * SENSOR_STATE_HOVER = 1
		 * SENSOR_STATE_HAND_ABOVE = 2
		 * SENSOR_STATE_RELEASING = 3
		 * SENSOR_STATE_START_BLOCKING_BY_GROUP = 4
		 * SENSOR_STATE_BLOCKED_BY_GROUP = 5
	  	 */
	  	byte sensorStatus[ELECTRODES_COUNT];

	    void setGroup(byte groupArr[]){
	  		resetToggleGroups();
	  		for (byte i = 0; i<ELECTRODES_COUNT; i++ ){
				groupIds[i] = groupArr[i];
			}
	    }
	  
	  	void resetToggleGroups(){
	  		for (byte i = 0; i<ELECTRODES_COUNT; i++ ){
				groupIds[i] = 0;  			
				sensorStatus[i] = SENSOR_STATE_NO_TOUCH;  			
	  		}
	  	}

	  	/*
	  	 * changes selected note/group by index and distance raw value
	  	 */
	  	void changeSensorStatus(float raw[]){
	  		
	  		bool toggleOn = false;

	  		for (byte idx = 0; idx<ELECTRODES_COUNT; idx++ ){
				
				// hand is over sensor
	  			if (raw[idx] > 2){
	  				if (sensorStatus[idx] == SENSOR_STATE_NO_TOUCH){
	  					sensorStatus[idx] = SENSOR_STATE_HOVER;
	  					toggleOn = true;	

	  				}else if (sensorStatus[idx] == SENSOR_STATE_HOVER){
	  					sensorStatus[idx] = SENSOR_STATE_HAND_ABOVE;	
	  				
	  				}else if (sensorStatus[idx] == SENSOR_STATE_HAND_ABOVE){
	  					// do not change
	  					
	  				}else if (sensorStatus[idx] == SENSOR_STATE_RELEASING){
	  					sensorStatus[idx] = SENSOR_STATE_HOVER;	

	  				}else if (sensorStatus[idx] == SENSOR_STATE_START_BLOCKING_BY_GROUP){
	  					sensorStatus[idx] = SENSOR_STATE_BLOCKED_BY_GROUP;	

	  				}else if (sensorStatus[idx] == SENSOR_STATE_BLOCKED_BY_GROUP){
	  					// do not change	

	  				}


				// hand is NOT over sensor
	  			}else{
	  				if (sensorStatus[idx] == SENSOR_STATE_NO_TOUCH){
	  					// do not change

	  				}else if (sensorStatus[idx] == SENSOR_STATE_HOVER){
	  					sensorStatus[idx] = SENSOR_STATE_NO_TOUCH;	
	  				
	  				}else if (sensorStatus[idx] == SENSOR_STATE_HAND_ABOVE){
	  					sensorStatus[idx] = SENSOR_STATE_RELEASING;	
	  					
	  				}else if (sensorStatus[idx] == SENSOR_STATE_RELEASING){
	  					sensorStatus[idx] = SENSOR_STATE_NO_TOUCH;	

	  				}else if (sensorStatus[idx] == SENSOR_STATE_BLOCKED_BY_GROUP){
	  					sensorStatus[idx] = SENSOR_STATE_NO_TOUCH;
	  					
	  				}else if (sensorStatus[idx] == SENSOR_STATE_START_BLOCKING_BY_GROUP){
	  					sensorStatus[idx] = SENSOR_STATE_NO_TOUCH;

	  				}
	  			}

	  			// a new sensor is beeing triggered
	  			if (toggleOn){
	  				bool hasChanges = false;
		  			for (byte i = 0; i<ELECTRODES_COUNT; i++ ){
						if (groupIds[idx] == groupIds[i] && i != idx && groupIds[idx] != 0){

							if (sensorStatus[i] == SENSOR_STATE_HOVER
								|| sensorStatus[i] == SENSOR_STATE_HAND_ABOVE){
							
								sensorStatus[i] = SENSOR_STATE_START_BLOCKING_BY_GROUP;
								hasChanges = true;
							}
						}  			
			  		}

			  		if (hasChanges ||toggleOn)
			  			return;
	  			}
	  		}
	  	}
       
};


#endif