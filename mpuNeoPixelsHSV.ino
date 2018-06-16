#include <Wire.h> 
#include <MPU6050.h> 
#include <Adafruit_NeoPixel.h> 

#define PIN D5
#define N_LEDS 100
int hue;
int sat;
MPU6050 mpu;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup () {
  Serial.begin(115200);
  
  strip.clear();
  strip.begin();

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
  delay(50);
  
  hue = map(roll, -80, 80, 0, 255);
  sat = map(pitch, -80, 80, 0, 255);
  Serial.println(hue);
  Serial.println(sat);
  fullBright(strip.Color(hue, sat, 100)); 
}

static void fullBright(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
     // stripA.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
  }
}
