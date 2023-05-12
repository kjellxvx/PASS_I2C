#include <Wire.h>

#define NUM5
// NUM 1  1 >> 5 THIS IS NOT A TRUE SLAVE; it's the master
// NUM 2  6 >> 11
// NUM 3  11 >> 15
// NUM 4  15 >> 9
// NUM 5  19 >> 23
// NUM 6  23 >> 28

#if defined NUM1
int startNum = 1;
int sensAmount = 5;
int wireNumber = 1;
#endif
#if defined NUM2
int startNum = 6;
int sensAmount = 5;
int wireNumber = 2;
#endif
#if defined NUM3
int startNum = 11;
int sensAmount = 4;
int wireNumber = 3;
#endif
#if defined NUM4
int startNum = 15;
int sensAmount = 4;
int wireNumber = 4;
#endif
#if defined NUM5
int startNum = 19;
int sensAmount = 4;
int wireNumber = 5;
#endif
#if defined NUM6
int startNum = 23;
int sensAmount = 5;
int wireNumber = 6;
#endif

int pins[] = { 2, 4, 6, 8, 10 };
bool valueState[] = { 0, 0, 0, 0, 0 };
int data[] = { 0, 0, 0, 0, 0 };
int an = 0;
int aus = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("STARTED");
  Wire.begin(wireNumber);        // Join I2C bus with address specified by 'wire'
  Wire.onRequest(requestEvent);  // Register event
  for (int i = 0; i < 5; i++) {
    pinMode(pins[i], INPUT);
  }
}

void loop() {
  delay(500);
  for (int i = 0; i < sensAmount; i++) {
    aus = (startNum + i) * 10;
    an = aus + 1;
    valueState[i] = digitalRead(pins[i]);
    if (valueState[i] == 0) {
      data[i] = an;
      Serial.println(data[i]);
    }
    if (valueState[i] == 1) {
      data[i] = aus;
      Serial.println(data[i]);
    }
  }
}

void requestEvent() {
  Serial.println("Data requested from Master");
  Wire.write((uint8_t*)data, sizeof(data));  // Send the data array
}