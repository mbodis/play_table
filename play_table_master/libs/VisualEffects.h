#ifndef VISUAL_EFFECTS_H
#define VISUAL_EFFECTS_H
 
#include <Arduino.h>

class VisualEffects{

  private:

  public:    
    
    void initScreenSaver(MyTable &mMyTable){
    
      for (byte loop = 0; loop<2; loop++){  
        int val = 0;
        while(true){
          for (int ele = SLAVE_FIRST_ID; ele <= SLAVE_FIRST_ID + ELECTRODES_COUNT; ele++){    
            mMyTable.sendMessageToSlave(ele, val);      
          }
            delay(25);
            val+=10;    
            if (val > 255)break;
        }
      
        val = 255;
        while(true){
          for (byte ele = SLAVE_FIRST_ID; ele <= SLAVE_FIRST_ID + ELECTRODES_COUNT; ele++){    
            mMyTable.sendMessageToSlave(ele, val);      
          }
            delay(25);
            val-=10;
            if (val < 0)break;    
        }
      }  
    }
    
    void initScreenSaver2(MyTable &mMyTable){
      int val = 0;
      
        while(true){
          mMyTable.sendMessageToSlave(SLAVE_FIRST_ID + 0, (val+15)%255);
          mMyTable.sendMessageToSlave(SLAVE_FIRST_ID + 1, (val+30)%255);
          mMyTable.sendMessageToSlave(SLAVE_FIRST_ID + 2, (val+45)%255);
          mMyTable.sendMessageToSlave(SLAVE_FIRST_ID + 3, (val+60)%255);
          mMyTable.sendMessageToSlave(SLAVE_FIRST_ID + 4, (val+75)%255);
          mMyTable.sendMessageToSlave(SLAVE_FIRST_ID + 5, (val+90)%255);
          mMyTable.sendMessageToSlave(SLAVE_FIRST_ID + 6, (val+105)%255);
          delay(25);
          val+=10;    
          if (val > 2000)break;
        }
    }
};


#endif