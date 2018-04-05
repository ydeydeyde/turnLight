#include <Wire.h>
#include <MPU6050.h>
#include <Adafruit_NeoPixel.h>

#define PINa 10 
#define PINb 12
#define N_LEDS 9

MPU6050 mpu;

// Timers
unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;

Adafruit_NeoPixel stripA = Adafruit_NeoPixel(9, PINa, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB = Adafruit_NeoPixel(9, PINb, NEO_GRB + NEO_KHZ800);

void setup() 
{
  // Clean slate, otherwise random pixels could be on when powered 
  stripA.clear();
  stripB.clear();
  
  Serial.begin(115200);
  stripA.begin();
  stripB.begin();
  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(10);
}

void loop()
{
  timer = millis();

  // Read normalized values
  Vector norm = mpu.readNormalizeGyro();

  // Calculate Pitch, Roll and Yaw
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;
  
  // Output raw
  //Serial.print(" Pitch = ");
  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);  
  Serial.print(",");
  Serial.println(yaw);

  if (pitch>5) {
  chaseA(stripA.Color(255, 0, 0)); // Red
  } 
  if (roll>2) {
  chaseB(stripB.Color(0, 0, 255)); // Red
  }

  // Wait to full timeStep period
  delay((timeStep*10000) - (millis() - timer));
}

static void chaseA(uint32_t c) {
  for(uint16_t i=0; i<stripA.numPixels()+4; i++) {
      stripA.setPixelColor(i  , c); // Draw new pixel
      stripA.setPixelColor(i-4, 0); // Erase pixel a few steps back
      stripA.show();
      //delay(25);
  }
}

  static void chaseB(uint32_t c) {
  for(uint16_t i=0; i<stripA.numPixels()+4; i++) {
      stripB.setPixelColor(i  , c); // Draw new pixel
      stripB.setPixelColor(i-4, 0); // Erase pixel a few steps back
      stripB.show();
      //delay(25);
  }
}
