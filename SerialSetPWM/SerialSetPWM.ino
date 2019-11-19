/*
  Reading a serial ASCII-encoded string.

  This sketch demonstrates the Serial parseInt() function.
  It looks for an ASCII string of comma-separated values.
  It parses them into ints, and uses those to fade an RGB LED.

  Circuit: Common-Cathode RGB LED wired like so:
  - red anode: digital pin 3
  - green anode: digital pin 5
  - blue anode: digital pin 6
  - cathode: GND

  created 13 Apr 2012
  by Tom Igoe
  modified 14 Mar 2016
  by Arturo Guadalupi

  This example code is in the public domain.
*/

// pins for the LEDs:
const int channelA = 5;
const int channelB = 6;
int setPWMchA=0;
int chA_PWM =0;
int timer1 = 0; 

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // make the pins outputs:
  pinMode(channelA, OUTPUT);
  pinMode(channelB, OUTPUT);
  timer1 = millis();
}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    
    // look for the next valid integer in the incoming serial stream:
    chA_PWM = Serial.parseInt();

    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {
      // constrain the values to 0 - 255 and invert
      // if you're using a common-cathode LED, just use "constrain(color, 0, 255);"
      chA_PWM = (255*constrain(chA_PWM, 0, 100))/100;
     
      // fade the red, green, and blue legs of the LED:
      analogWrite(channelA, chA_PWM);
      analogWrite(channelB, chA_PWM);
  

      // print the three numbers in one string as hexadecimal:
      Serial.print(chA_PWM);
    }
  }
//  if (timer1 + 50 < millis())
//  {
//    if (chA_PWM > setPWMchA)
//    {
//      setPWMchA++;
//      analogWrite(channelA, setPWMchA);
//    }
//    else if (chA_PWM < setPWMchA)
//    {
//      setPWMchA--;
//      analogWrite(channelA, setPWMchA);
//    }
//    timer1 = millis();
//  }
}
