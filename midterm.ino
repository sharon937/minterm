#include"pitches.h"
#include"pictures.h"
#include <Wire.h>
#include "SSD1306Wire.h"
#define Sensor 14
#define Bee 27
#define Button 26
SSD1306Wire  display(0x3c, 21, 22);

int freq = 2000;
int channel = 0;
int resolution = 8;
int i=0;
int melody[] = {
  NOTE_C5, NOTE_C5, NOTE_G5, NOTE_G5, NOTE_A5, NOTE_A5, NOTE_G5,
  NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5,
  NOTE_G5, NOTE_G5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5,
  NOTE_G5, NOTE_G5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_G5, NOTE_G5, NOTE_A5, NOTE_A5, NOTE_G5,
  NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5,
};
int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
};

unsigned long currentTime = 0, tonePreviousTime = 0, picPreviousTime = 0;
int pauseBetweenNotes = 0;
int noteDuration = 0;
int thisNote = 0;
boolean nextTone = true;
boolean nextPic = true;

void setup() {
 pinMode(Sensor, INPUT);
 pinMode(Bee,OUTPUT);
 pinMode(Button,INPUT);
 Serial.begin(115200);
 ledcSetup(channel, freq, resolution);
 ledcAttachPin(Bee, channel);
 display.init();
 display.flipScreenVertically();
 display.setContrast(255); //數值介於0~255，調整對比
 display.clear();//清除螢幕和緩衝區(clear)
}
void warning() {
    display.clear();
    display.drawXbm(0,0, 128, 64,your_img1);
    display.display();
    display.clear();
    display.drawXbm(0,0, 128, 64,your_img2);
    display.display();
    display.clear();
    display.drawXbm(0,0, 128, 64,your_img3);
    display.display();
  }

void loop() {
  if(digitalRead(Sensor)==HIGH)
  {
      while(true)
      {
        if(nextTone)
        {
          noteDuration = 1300 / noteDurations[thisNote];
          ledcWriteTone(channel, melody[thisNote]);
          pauseBetweenNotes = noteDuration * 1.3;
          nextTone = false;
        }
        if(nextPic)
        {
          display.clear();
          warning();
          nextPic = false;
        }
        currentTime = millis(); //記錄現在時間
        if((currentTime - tonePreviousTime) > noteDuration)
        {
          ledcWriteTone(channel, 0);
        } //buzzer停止播放
        if((currentTime - tonePreviousTime)> (pauseBetweenNotes))
        {
          nextTone = true;
          thisNote++;
          if(thisNote == 41) thisNote = 0;
          tonePreviousTime = currentTime; 
        } //buzzer播放下一個音
        if((currentTime - picPreviousTime) > 300 )
        {
          nextPic = true;
          picPreviousTime = currentTime;//角色每150毫秒移動一次
          nextPic = false;
        }
        if (digitalRead(Button)==HIGH){ 
          thisNote=0;
          ledcWriteTone(channel,0);
          display.clear();
          break;}
        
      }
              
    }

}
