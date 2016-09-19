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
          for (int ele = 30; ele<38; ele++){    
            mMyTable.sendMessageToSlave(ele, val);      
          }
            delay(25);
            val+=10;    
            if (val > 255)break;
        }
      
        val = 255;
        while(true){
          for (byte ele = 30; ele<38; ele++){    
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
          mMyTable.sendMessageToSlave(30, (val+15)%255);
          mMyTable.sendMessageToSlave(31, (val+30)%255);
          mMyTable.sendMessageToSlave(32, (val+45)%255);
          mMyTable.sendMessageToSlave(33, (val+60)%255);
          mMyTable.sendMessageToSlave(34, (val+75)%255);
          mMyTable.sendMessageToSlave(35, (val+90)%255);
          mMyTable.sendMessageToSlave(36, (val+105)%255);
          delay(25);
          val+=10;    
          if (val > 2000)break;
        }
    }
};


#endif