#include <Wire.h>
#include <MPU6050.h>
#include <Adafruit_NeoPixel.h>

// Output pins for NeoPixel LED strips
#define PINa 10
#define PINb 12
#define N_LEDS 9 

MPU6050 mpu;

Adafruit_NeoPixel stripA = Adafruit_NeoPixel(9, PINa, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB = Adafruit_NeoPixel(9, PINb, NEO_GRB + NEO_KHZ800);

void setup() 
{
  Serial.begin(115200);
  stripA.clear();
  stripB.clear();

  stripA.begin();
  stripB.begin();
  
  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
}

void loop()
{
  // Read normalized values 
  Vector normAccel = mpu.readNormalizeAccel();

  // Calculate Pitch & Roll
  int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  int roll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;

  // Output
  Serial.print(" Pitch = ");
  Serial.print(pitch);
  Serial.print(" Roll = ");
  Serial.print(roll);
  Serial.println();
  delay(100);

  // Side 1 
  if (pitch < 20 and pitch > -10 and roll < 5 and roll > -20 ) {
    fullA(stripA.Color(255, 0, 0)); // Red
    fullB(stripB.Color(255, 0, 0)); 
  } 
  // Side 2
  else if (pitch > -89 and pitch < -80 and roll > 140 and roll < 170) {
    fullA(stripB.Color(0, 0, 255)); // Blue
    fullB(stripB.Color(0, 0, 255)); 
  }
  // Side 3
  else if (pitch < 90 and pitch > 70 and roll < -160 and roll > -167) {
    fullA(stripA.Color(0,255, 0)); // Green
    fullB(stripB.Color(0, 255, 0)); 
  }
  // Side 4
    else if (pitch < 10 and pitch > -10 and roll > 90 and roll < 110) {
    fullA(stripB.Color(100, 150,200)); 
    fullB(stripB.Color(100, 150,200)); 
  }
  // Side 5
  else if (pitch < 5 and pitch > -10 and roll > -110 and roll < -85) {
    fullA(stripA.Color(10,255, 255)); 
    fullB(stripB.Color(10, 255, 255)); 
  } 
  // Side 6 
  else if (pitch < 5 and pitch > -30 and roll > 150 and roll < 200) {
    fullA(stripA.Color(255, 255, 255)); 
    fullB(stripB.Color(255, 255, 255)); 
  // SideX
  } else {
    fullA(stripA.Color(random(255), random(255), random(255))); 
    fullB(stripB.Color(random(255), random(255), random(255))); 
  }
}

static void fullA(uint32_t c) {
  for(uint16_t i=0; i<stripA.numPixels()+4; i++) {
      stripA.setPixelColor(i  , c); // Draw new pixel
     // stripA.setPixelColor(i-4, 0); // Erase pixel a few steps back
      stripA.show();
  }
}

  static void fullB(uint32_t c) {
  for(uint16_t i=0; i<stripA.numPixels()+4; i++) {
      stripB.setPixelColor(i  , c); // Draw new pixel
    //  stripB.setPixelColor(i-4, 0); // Erase pixel a few steps back
      stripB.show();
  }
}
