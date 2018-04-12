#include <Wire.h> 
#include <MPU6050.h> 
#include <Adafruit_NeoPixel.h> 

#define PINa 10
#define PINb 12
#define N_LEDS 9
int hue;
int sat;
MPU6050 mpu;

Adafruit_NeoPixel stripA = Adafruit_NeoPixel(9, PINa, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB = Adafruit_NeoPixel(9, PINb, NEO_GRB + NEO_KHZ800);

void setup () {
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

void loop() {
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
  
  hue = map(roll, -180, 180, 0, 255);
  sat = map(pitch, -180, 180, 0, 255);
  Serial.println(hue);
  Serial.println(sat);
  fullBright(stripA.Color(hue, sat, 100)); 
  fullBright(stripB.Color(hue, sat, 100)); 
}

static void fullBright(uint32_t c) {
  for(uint16_t i=0; i<stripA.numPixels()+4; i++) {
      stripA.setPixelColor(i  , c); // Draw new pixel
      stripB.setPixelColor(i  , c); // Draw new pixel
     // stripA.setPixelColor(i-4, 0); // Erase pixel a few steps back
      stripA.show();
      stripB.show();
  }
}

