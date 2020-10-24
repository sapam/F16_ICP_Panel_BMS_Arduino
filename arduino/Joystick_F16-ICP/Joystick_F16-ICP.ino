// F16-ICP Joystick by Chingkhei Nganba Sapam
// Ref Credit : Les O/Reilly
// 
// 4 - Analog Axis for potentiometers.
// 32 - Buttons as Matrix but only 31 will be used as buttons.
// 
// 
// R4:C5 - Led1 and Led2 Control (states: 0-off, 1-on (Led01), 2-on (Led02))
// 
// 


#include <Keypad.h>
#include <Joystick.h>

#define NUMBUTTONS 32
#define NUMROWS 4
#define NUMCOLS 8
#define NUMHATS 0
#define LED_ROW 3
#define LED_COL 4
#define UPDATEAXIS 1000 
#define LED_BUTTON 0
#define TOGGLE02_DN 23

//set AXIS positions
int VAL_X = 0;
int VAL_Y = 0;
int VAL_RX = 0;
int VAL_RY = 0;
long LASTMIL = 0;

byte POT_X = A0;
byte POT_Y = A1;
byte POT_RX = A2;
byte POT_RY = A3;
byte LED01 = 9;
byte LED02 = 0;

byte ledSwitch = 0;
int potVal = 0;
int potValX = 0;
int potValY = 0;
int potValRx = 0;
int potValRy = 0;

byte backlitMode = 0; // 0: off, 1: dim LED, 2: bright LED

unsigned long prevTime = millis();
unsigned long curTime = millis();

//define the symbols on the buttons of the keypads
byte buttons[NUMROWS][NUMCOLS] = {
  { 0, 1, 2, 3, 4, 5, 6, 7},
  { 8, 9,10,11,12,13,14,15},
  {16,17,18,19,20,21,22,23},
  {24,25,26,27,28,29,30,31},
};

byte rowPins[NUMROWS] = {10,16,14,15}; //connect to the row pinouts of the keypad
byte colPins[NUMCOLS] = {1,2,3,4,5,6,7,8}; //connect to the column pinouts of the keypad

Keypad buttIPC = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

//initialize an Joystick with 32 buttons;
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_JOYSTICK, NUMBUTTONS, NUMHATS,
  true, true, false, true, true, false,
  false, false, false, false, false);

void setup() {
  /**
  pinMode(LED01, OUTPUT);
  pinMode(LED02, OUTPUT);
  **/
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  analogWrite(LED01, 125);
  
  // put your setup code here, to run once:
  Joystick.setXAxisRange(0, 1023);
  Joystick.setYAxisRange(0, 1023);
  Joystick.setRxAxisRange(0, 1023);
  Joystick.setRyAxisRange(0, 1023);
  Joystick.begin();

  // initialize the pots
  //potVal = analogRead(POT_X);
  Joystick.setXAxis(1);
  //VAL_X = potVal;

  //potVal = analogRead(POT_Y);
  Joystick.setYAxis(1);
  //VAL_Y = potVal;

  //potVal = analogRead(POT_RX);
  Joystick.setRxAxis(1);
  //VAL_RX = potVal;

  //potVal = analogRead(POT_RY);
  Joystick.setRxAxis(1);
  //VAL_RY = potVal;

}

void loop() {
  // put your main code here, to run repeatedly:
  CheckAllButtons();
  CheckAllAxis();
}

void CheckAllAxis(void) {
  // Check and update the position of all axis.
  //curTime = millis();

  potValX = analogRead(POT_X);
  if ( potValX != VAL_X ) {
    Joystick.setXAxis(potValX);
    VAL_X = potValX;
  }


  potValY = analogRead(POT_Y);
  if ( potValY != VAL_Y ) {
    Joystick.setYAxis(potValY);
    VAL_Y = potValY;
  }

  potValRx = analogRead(POT_RX);
  if ( potValRx != VAL_RX ) {
    Joystick.setRxAxis(potValRx);
    VAL_RX = potValRx;
  }

  potValRy = analogRead(POT_RY);
  if ( potValRy != VAL_RY ) {
    Joystick.setRyAxis(potValRy);
    VAL_RY = potValRy;
    int intensity = map(potValRy, 0, 1023, 0, 255);
    analogWrite(LED01, intensity);
  }
  
}

/**
void UpdateLedBacklit(void) {
  backlitMode = (backlitMode + 1)%3;
  analogWrite(LED01, intensity);
  switch (backlitMode) {
    case 0:
          digitalWrite(LED01, LOW);
          digitalWrite(LED02, LOW);
          break;
    case 1:
          digitalWrite(LED01, HIGH);
          digitalWrite(LED02, LOW);
          break;
    case 2:
          digitalWrite(LED01, LOW);
          digitalWrite(LED02, HIGH);
          break;
  }
}
**/

void CheckAllButtons(void) {
    
    if (buttIPC.getKeys())
    {
       for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
           if ( buttIPC.key[i].stateChanged )   // Only find keys that have changed state.
            {
              byte key_val = byte(buttIPC.key[i].kchar);
            switch (buttIPC.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    case HOLD:                              
                              Joystick.setButton(buttIPC.key[i].kchar, 1);
                              
                              Serial.println(key_val);
                              /**
                              switch ( key_val ) {
                                case 22:
                                case 23:
                                case 29:
                                case 30:
                                      // Toggle Latch Switch so release it after 100ms
                                      delay(100);
                                      Joystick.setButton(buttIPC.key[i].kchar, 0);
                              }  
                              **/
                              break;
                    case RELEASED:
                    case IDLE:
                              Joystick.setButton(buttIPC.key[i].kchar, 0);
                              /**
                              switch ( key_val ) {
                                case 22:
                                case 23:
                                      Joystick.setButton(32, 1);
                                      Serial.println(33);
                                      delay(100);
                                      Joystick.setButton(32, 0);
                                      break;
                                case 29:
                                case 30:
                                      Joystick.setButton(33, 1);
                                      Serial.println(34);
                                      delay(100);
                                      Joystick.setButton(33, 0);
                                      break;
                              }      
                              **/                          
                              break;
                              
            }
           }   
         }
         
     }
}
