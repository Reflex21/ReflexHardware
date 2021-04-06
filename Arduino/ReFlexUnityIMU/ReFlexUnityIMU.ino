// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

String input;

Adafruit_MPU6050 mpu;
const double threshold = -0.05; // How far above the baseline do we count a key press
const double samplingRate = 20; // Sampling Freq in Hz
const double calibrationTime = 5000; // Calibration time in milliseconds
double baseline;

unsigned long debounceDelay = 100; // Min time between key presses (has to be greater than 1/samplingRate);
unsigned long lastDebounceTime = 0; 

const int numReadings = 10;
double readings[numReadings];      // the readings to use for the moving average
int readIndex = 0;              // the index of the current reading
double total = 0;                  // the running total
double average = 0;                // the average


void setup(void) {
  Serial.begin(115200);
  /*
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  while(!Serial.available()); // Wait until we receive calibration time
  int calibrationTime;
  // Read it in
  input = Serial.readString();

  if (input.length() > 0) {
    Serial.println(input);
    calibrationTime = input.toInt();
  }
  else {
    calibrationTime = 5000;
  }
  */
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); // Low-Pass Filter w/ Freq. Cutoff of 21Hz
  Serial.println("");
  delay(100);

  // Calibration
  Serial.println("Hold still...");
  double sum = 0;
  double num = 0;
  int startTime = millis();
  while((millis() - startTime) < calibrationTime){
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    sum += a.acceleration.z;
    num += 1;
  }

  baseline = sum/num;
  Serial.print("Baseline: ");
  Serial.println(baseline);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Moving Average
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = a.acceleration.z;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  
  if (average < threshold+baseline) {
    // Pseudo-Debouncing
    if((millis() - lastDebounceTime) > debounceDelay){
      lastDebounceTime = millis();
    }
  }

  Serial.println(millis() - lastDebounceTime);
  //Serial.println(average);
  delay((1/samplingRate)*1000);
}
