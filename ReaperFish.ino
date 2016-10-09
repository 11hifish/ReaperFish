#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>

#define MELODY_LENGTH 95

unsigned char note[MELODY_LENGTH] = 
{
  NOTE_E(5), SILENT_NOTE, NOTE_E(5), SILENT_NOTE, 
  NOTE_E(5), SILENT_NOTE, NOTE_C(5), NOTE_E(5),
  NOTE_G(5), SILENT_NOTE, NOTE_G(4), SILENT_NOTE,
 
  NOTE_C(5), NOTE_G(4), SILENT_NOTE, NOTE_E(4), NOTE_A(4), 
  NOTE_B(4), NOTE_B_FLAT(4), NOTE_A(4), NOTE_G(4),
  NOTE_E(5), NOTE_G(5), NOTE_A(5), NOTE_F(5), NOTE_G(5), 
  SILENT_NOTE, NOTE_E(5), NOTE_C(5), NOTE_D(5), NOTE_B(4),
 
  NOTE_C(5), NOTE_G(4), SILENT_NOTE, NOTE_E(4), NOTE_A(4), 
  NOTE_B(4), NOTE_B_FLAT(4), NOTE_A(4), NOTE_G(4),
  NOTE_E(5), NOTE_G(5), NOTE_A(5), NOTE_F(5), NOTE_G(5), 
  SILENT_NOTE, NOTE_E(5), NOTE_C(5), NOTE_D(5), NOTE_B(4),
 
  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5),
  NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE, NOTE_G_SHARP(4),
  NOTE_A(4), NOTE_C(5), SILENT_NOTE, NOTE_A(4), NOTE_C(5), NOTE_D(5),
 
  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5),
  NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_C(6), SILENT_NOTE, NOTE_C(6), SILENT_NOTE, NOTE_C(6),
 
  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5),
  NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_G_SHARP(4), NOTE_A(4), NOTE_C(5), SILENT_NOTE, 
  NOTE_A(4), NOTE_C(5), NOTE_D(5),
 
  SILENT_NOTE, NOTE_E_FLAT(5), SILENT_NOTE, NOTE_D(5), NOTE_C(5)
};

unsigned char pent[6] = {NOTE_C(5), NOTE_D(5), NOTE_E(5), NOTE_G(5), NOTE_A(5), NOTE_C(6)};
 
unsigned int duration[MELODY_LENGTH] =
{
  100, 25, 125, 125, 125, 125, 125, 250, 250, 250, 250, 250,
 
  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,
 
  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,
 
  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,
 
  250, 125, 125, 125, 250, 125, 125, 200, 50, 100, 25, 500,
 
  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,
 
  250, 250, 125, 375, 500
};

OrangutanLCD lcd;

OrangutanBuzzer buzzer;
unsigned char currentIdx;
int LEDpin = 0;
int redLEDpin = 1;

//photo sensor: below 520
int inputPin = A5;
void setup() {
  // put your setup code here, to run once:
  currentIdx = 0;
  pinMode(inputPin, INPUT);
  pinMode(LEDpin, OUTPUT);
   pinMode(redLEDpin, OUTPUT);
}

void ScaredWhenLightIsOn()
{
  digitalWrite(LEDpin, LOW);
  int turnSpeed = 150;
  int turnDuration = 1000;
  int backSpeed = 80;
  int backSuration = 200;

  
  for (int i = 0; i < 5; i++)
  {
    int duration = rand() % 500;
    int freq = rand() % 400 + 800;
    play_frequency(freq, duration, 15);
    digitalWrite(redLEDpin, HIGH);
    delay(60);
    digitalWrite(redLEDpin, LOW);
    delay(60);
  }
  
  digitalWrite(redLEDpin, HIGH);
  OrangutanMotors::setSpeeds(-turnSpeed, turnSpeed);
  delay(turnDuration);
  OrangutanMotors::setSpeeds(-backSpeed, -backSpeed);
  delay(backSuration);
  digitalWrite(redLEDpin, LOW);

  OrangutanMotors::setSpeeds(0,0);
}

//moving behavior when light is off
// Globals
int behaviourDuration = 1000;
int threshold = 400;
int pauseDuration = 2000;
int happySpeed = 30;

void isThereAWall()
{
  if (analogRead(A7) > threshold)
  {
    int j = rand() % 2;
    if (j)
    {
      OrangutanMotors::setSpeeds(-happySpeed*5,happySpeed*5);
      delay(150);
    }
    else
    {
      OrangutanMotors::setSpeeds(happySpeed*5,-happySpeed*5);
      delay(150);
    }
  }
}

int lightThreadshold = 600; //600 for darkness, 870 unpredicatable

bool firstTime = true;
void loop() {
  // put your main code here, to run repeatedly:
  int p = analogRead(inputPin);
  
  OrangutanLCD::clear();
  OrangutanLCD::print(p);
  delay(50);
  
  if(p < lightThreadshold)
  {

    if(firstTime)
    {
      int delayAmount = 2000 + rand() % 2000;
      delay(delayAmount);
    }
    digitalWrite(LEDpin, HIGH);
    
    if(firstTime)
    {
      int delayAmount = 2000 + rand() % 2000;
      delay(delayAmount);
      //probing the environemnt
      OrangutanMotors::setSpeeds(-30,30);
      
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      
      //delay(1000) in total;
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }

      OrangutanMotors::setSpeeds(30,-30);
      //delay(1000) in total;
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }

      OrangutanMotors::setSpeeds(-30,30);
      
      //delay(1000) in total;
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      delay(250);
      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }
      
      OrangutanMotors::setSpeeds(0, 0);

      if(analogRead(inputPin) >= lightThreadshold)
      {
        OrangutanMotors::setSpeeds(0,0);
        firstTime = true;
        ScaredWhenLightIsOn();
        return;
      }

      firstTime = false;
    }


    
    for (int count = 0; count < 100; count++)
    {  
       if (currentIdx < MELODY_LENGTH && !buzzer.isPlaying())
      {
        // play note at max volume
        //buzzer.playNote(note[currentIdx], duration[currentIdx]*1.3, 15);
        // Play random note from pentatonic scale
        int randIndex = rand() % 7;
        if (randIndex != 6) {
          buzzer.playNote(pent[randIndex], duration[currentIdx]*1.3, 15);
        }
         
        currentIdx++;
      }
      if (currentIdx >= MELODY_LENGTH)
      {
        currentIdx = 0;
      }
    }
//    OrangutanMotors::setSpeeds(15, 15);
//    MoveForwardHappliy();


    //move forward:
    int i = rand() % 4;
      switch(i) {
        case 1:
        OrangutanMotors::setSpeeds(15,25);
        delay(80);
        if(analogRead(inputPin) >= lightThreadshold)
        {
          OrangutanMotors::setSpeeds(0,0);
          firstTime = true;
          ScaredWhenLightIsOn();
          return;
        }
        
          isThereAWall();
          if(analogRead(inputPin) >= lightThreadshold)
          {
            OrangutanMotors::setSpeeds(0,0);
            firstTime = true;
            ScaredWhenLightIsOn();
            return;
          }
          OrangutanMotors::setSpeeds(happySpeed, 0.5 * happySpeed);
          delay(120);
          isThereAWall();
          OrangutanMotors::setSpeeds(0.5 * happySpeed, happySpeed);
          delay(120);
          isThereAWall();
        
        break;
    
        case 2:
        OrangutanMotors::setSpeeds(25,15);
        delay(80);
        if(analogRead(inputPin) >= lightThreadshold)
        {
          OrangutanMotors::setSpeeds(0,0);
          firstTime = true;
          ScaredWhenLightIsOn();
          return;
        }
        
        
          isThereAWall();
          if(analogRead(inputPin) >= lightThreadshold)
          {
            OrangutanMotors::setSpeeds(0,0);
            firstTime = true;
            ScaredWhenLightIsOn();
            return;
          }
          OrangutanMotors::setSpeeds(happySpeed, 0.5 * happySpeed);
          delay(120);
          isThereAWall();
          OrangutanMotors::setSpeeds(0.5 * happySpeed, happySpeed);
          delay(120);
          isThereAWall();
        
        break;
    
        default: // case 0 or 3
        
          isThereAWall();
          if(analogRead(inputPin) >= lightThreadshold)
          {
            OrangutanMotors::setSpeeds(0,0);
            firstTime = true;
            ScaredWhenLightIsOn();
            return;
          }
          OrangutanMotors::setSpeeds(happySpeed, 0.5 * happySpeed);
          delay(120);
          isThereAWall();
          OrangutanMotors::setSpeeds(0.5 * happySpeed, happySpeed);
          delay(120);
          isThereAWall();

        break;
    }
  }
  
  if(p >= lightThreadshold && firstTime == false)
  {
    firstTime = true;
    digitalWrite(LEDpin, LOW);
    ScaredWhenLightIsOn();
  }
 
}
