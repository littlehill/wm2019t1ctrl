/*
  LED Control

  This example scans for BLE peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the BLE Peripheral's LED, when the button is pressed or released.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-up resistor connected to pin 2.

  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

#define UUID_car "19b10000-e8f2-537e-4f6c-d104768a1220"
#define UUID_ctrl "19b10000-e8f2-537e-4f6c-d104768a1222"
#define UUID_huge "19b10001-e8f2-537e-4f6c-d104768a1220"

#define DATA_LENGTH 6
#define SAFE_PWM_VALUE 2*255/100
#define FAST_PWM_VALUE 40*255/100

#define UNINIT 0
#define SEQUENCE_WHITE  1
#define SEQUENCE_RED    2
#define SEQUENCE_GREEN  3
#define SEQUENCE_BLACK  4

#define RED 0
#define GREEN 1
#define BLUE 2

// variables for button
const int buttonPin = 2;
int oldButtonState = LOW;

// pins for the LEDs:
const int channelA = 5;
const int channelB = 6;
int chA_PWM =0;
int state = UNINIT;

void setSafeValue()
{
  analogWrite(channelA, SAFE_PWM_VALUE);
  analogWrite(channelB, SAFE_PWM_VALUE);
}

int recogniseColor(byte* datain)
{
  if(datain[GREEN] > 120 && datain[BLUE] < 70 )
  {
    return SEQUENCE_GREEN; 
  }
  return 0;
}

void setup() {
  Serial.begin(115200);
   // make the pins outputs:
  pinMode(channelA, OUTPUT);
  pinMode(channelB, OUTPUT);
  setSafeValue();
//  while (!Serial);

  // configure the button pin as input
  pinMode(buttonPin, INPUT);

  // initialize the BLE hardware
  BLE.begin();

  Serial.println("BLE Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid(UUID_car);
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "T800_RED") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid(UUID_car);
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }


  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    //peripheral.disconnect();
    //return;
  }
    Serial.println(peripheral.characteristicCount());
  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic(UUID_huge);
   
  if (!ledCharacteristic) {
    Serial.println("Peripheral does not have LED characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    Serial.println("Peripheral does not have a writable LED characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {
    // while the peripheral is connected

    // read the button pin
    byte data[DATA_LENGTH];
    ledCharacteristic.readValue(data,DATA_LENGTH);
//  data[0]=r;
//  data[1]=g;
//  data[2]=b;
//  data[3]=0;
//  data[4]=0;
//  data[5]=0;
    if (recogniseColor(data) == SEQUENCE_GREEN)
    {
      analogWrite(channelA, FAST_PWM_VALUE);
      analogWrite(channelB, FAST_PWM_VALUE);
    }
  
//        Serial.println("DATA Start");
//        for(int i = 0; i < DATA_LENGTH; i++)
//                {
//                  Serial.println(data[i]);
//                }
//        Serial.println("DATA END");
     
        // button is released, write 0x00 to turn the LED off
      }
  setSafeValue();
  Serial.println("Peripheral disconnected");
}
