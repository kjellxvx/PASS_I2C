#include <Wire.h>

#define NUM_SLAVES 6  // Number of slave devices

int data[NUM_SLAVES][5];          // Data array to store values from each slave, it has always 5, the slaves that only have 4 sensors use a 0 at the end
int previousData[NUM_SLAVES][5];  // Array to store previous data
int validData[NUM_SLAVES][5];     // Array to store previous data


#define NUM1
// NUM 1  1 >> 5 THIS IS NOT A TRUE SLAVE; it's the master
// NUM 2  6 >> 11
// NUM 3  11 >> 15
// NUM 4  15 >> 9
// NUM 5  19 >> 23
// NUM 6  23 >> 28

#if defined NUM1
int startNum = 1;
int sensAmount = 5;
int wire = 1;
#endif
#if defined NUM2
int startNum = 6;
int sensAmount = 5;
int wire = 2;
#endif
#if defined NUM3
int startNum = 11;
int sensAmount = 4;
int wire = 3;
#endif
#if defined NUM4
int startNum = 15;
int sensAmount = 4;
int wire = 4;
#endif
#if defined NUM5
int startNum = 19;
int sensAmount = 4;
int wire = 5;
#endif
#if defined NUM6
int startNum = 23;
int sensAmount = 5;
int wire = 6;
#endif

int pins[] = { 2, 4, 6, 8, 10 };
bool valueState[] = { 0, 0, 0, 0, 0 };
bool stairState[] = { 0, 0, 0, 0, 0 };
//int localData[] = { 0, 0, 0, 0, 0 };
int an = 0;
int aus = 0;

void setup() {
  Serial.begin(9600);
  //Serial.println("STARTED");
  Wire.begin();  // Join I2C bus as master
  for (int i = 0; i < 5; i++) {
    pinMode(pins[i], INPUT);
  }
}

void loop() {
  delay(400);
  getLocalData();
  requestSlaveData();
  validataSlaveData();
  printData();
}

void requestSlaveData() {
  //Serial.println("requesting slave data");
  long startTime = millis();
  unsigned long timeout = 500;  // Timeout value in milliseconds

  for (int slave = 2; slave <= NUM_SLAVES; slave++) {
    Wire.beginTransmission(slave);
    Wire.requestFrom(slave, sizeof(data[slave - 1]));
    unsigned long currentTime = millis();

    while (Wire.available() < sizeof(data[slave - 1])) {
      if (currentTime - startTime >= timeout) {
        Serial.print("Slave No. ");
        Serial.print(slave);
        Serial.println(" Communication timeout occurred");
        Wire.endTransmission(slave);
        break;
      }
      currentTime = millis();
    }

    if (Wire.available() == sizeof(data[slave - 1])) {
      int i = slave - 1;
      Wire.readBytes((uint8_t*)&data[i], sizeof(data[i]));
    }
    Wire.endTransmission(slave);
  }
  //Serial.println("Slave data request completed");
}



void getLocalData() {
  //Serial.println("getting local data");
  for (int i = 0; i < sensAmount; i++) {
    aus = (startNum + i) * 10;
    an = aus + 1;

    valueState[i] = digitalRead(pins[i]);

    if (valueState[i] == 0) {
      data[0][i] = an;
      //localData[i] = an;
      //Serial.println(localData[i]);
    }
    if (valueState[i] == 1) {
      data[0][i] = aus;
      //localData[i] = aus;
      //Serial.println(localData[i]);
    }
  }
}

void validataSlaveData() {
  //Serial.println("validating data");
  for (int i = 0; i < NUM_SLAVES; i++) {
    for (int j = 0; j < 5; j++) {
      if (data[i][j] > 0 && data[i][j] < 272) {
        validData[i][j] = data[i][j];
      }
    }
  }
}

void printData() {
  //Print the data nice and clean to read
  // for (int i = 0; i < NUM_SLAVES; i++) {
  //   Serial.print("Slave ");
  //   Serial.print(i + 1);
  //   Serial.print(": ");
  //   for (int j = 0; j < 5; j++) {
  //     Serial.print(validData[i][j]);
  //     Serial.print("\t");
  //   }
  //   Serial.println();
  // }
  // Only print data that changed
  for (int i = 0; i < NUM_SLAVES; i++) {
    for (int j = 0; j < 5; j++) {
      if (validData[i][j] != previousData[i][j]) {
        Serial.println(validData[i][j]);
        previousData[i][j] = validData[i][j];  // Update previous data
      }
    }
  }
}