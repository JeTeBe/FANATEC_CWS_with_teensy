/* Complete USB Joystick Example
   Teensy becomes a USB joystick with 16 or 32 buttons and 6 axis input

   You must select Joystick from the "Tools > USB Type" menu

   Pushbuttons should be connected between the digital pins and ground.
   Potentiometers should be connected to analog inputs 0 to 5.

   This example code is in the public domain.
*/


// Configure the number of buttons.  Be careful not
// to use a pin for both a digital button and analog
// axis.  The pullup resistor will interfere with
// the analog voltage.
const int numButtons = 1;  // 16 for Teensy, 32 for Teensy++
const int ButtonOffset = 4; // offset means starts with pin 4
const byte encoderPinA = 2;//outputA digital pin2
const byte encoderPinB = 3;//outoutB digital pin3

volatile int count = 32767;
unsigned int StepSize = 10;
unsigned int protectedCount = 0;
unsigned int previousCount = 0;
byte allButtons[numButtons];
byte prevButtons[numButtons];
int angle=0;
#define LED_PIN 13
#define readA digitalRead(encoderPinA) //faster than digitalRead()
#define readB digitalRead(encoderPinB) //faster than digitalRead()

void isrA() {
  if(readB != readA) {
    digitalWrite(LED_PIN, HIGH);  // LED on
    count += StepSize;
  } else {
    digitalWrite(LED_PIN, LOW);  // LED on
    count -= StepSize;
  }
}

void isrB() {
  if (readA == readB) {
    count += StepSize;
  //digitalWrite(LED_PIN, HIGH);  // LED on
  } else {
    count -= StepSize;
    //digitalWrite(LED_PIN, LOW);  // LED on
  }
}

void setup() {
  // you can print to the serial monitor while the joystick is active!
  Serial.begin(9600);
  while (!Serial && millis() < 4000 );
  Serial.println("\n" __FILE__ " " __DATE__ " " __TIME__);
  
  pinMode(LED_PIN, OUTPUT);
  
  //Serial.println("Begin Complete Joystick Test");
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  for (int i=0; i<numButtons; i++) {
    pinMode(i+ButtonOffset, INPUT_PULLUP);
  }
 
  attachInterrupt(digitalPinToInterrupt(encoderPinA), isrA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), isrB, CHANGE);
  Joystick.useManualSend(true);
}



void loop() {
  Joystick.X(count);
  Joystick.Y(count);
  
  for (int i=0; i<numButtons; i++) {
    if (digitalRead(i + ButtonOffset)) {
      // when a pin reads high, the button is not pressed
      // the pullup resistor creates the "on" signal
      allButtons[i] = 0;
    } else {
      // when a pin reads low, the button is connecting to ground.
      allButtons[i] = 1;
    }
    Joystick.button(i + ButtonOffset, allButtons[i]);
  }
  Joystick.send_now();
  noInterrupts();
  protectedCount = count;
  interrupts();

  
  if(protectedCount != previousCount) {
    
  }
  previousCount = protectedCount;
  //digitalWrite(LED_PIN, LOW);  // LED on
  delay(5);
  //digitalWrite(LED_PIN, HIGH); // LED off
  delay(5);
  if (count < 1000)
  {
    //count = count +1;
  }else{
    //count = 0;
  }
}
