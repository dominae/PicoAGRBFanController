#include <Adafruit_NeoPixel.h>
#include <serial-readline.h>
#include <EEPROM.h>

#define LED_PIN 16
#define LED_COUNT 1
uint8_t LED_MEM = 0;
volatile uint32_t colorInt = (0,0,0);
volatile uint32_t colorInt2 = (0,0,0);
volatile int brightness = 50;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(LED_MEM, 0, NEO_GRB + NEO_KHZ800);
void received(char*);
SerialLineReader reader(Serial, 18 , received);

void setup1(){
    
}

void received(char* line) {
  char c = line[0]; 
  if (c == 'V')
  {
    uint8_t  val = atoi(String(line).substring(1).c_str());
    EEPROM.write(0, val );
    EEPROM.commit();
    return;
  }
    if (c == 'P')
  {
    Serial.write("Pong\n");
    return;
  }
	char *colorTok = strtok(line,  ",");
  int colors[4];
  int i = 0;
  while (colorTok != NULL)
  {
      colors[i] = atoi(colorTok);
      i++;
      colorTok=strtok(NULL, ",");
  }
  colorInt = strip.Color(colors[0], colors[1], colors[2]);
  colorInt2 = strip1.Color(colors[0], colors[1], colors[2]);
  brightness = colors[3];
}

void loop1() {
	reader.poll();
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  EEPROM.get(0, LED_MEM);
  Serial.print(LED_MEM);
  strip1.updateLength(LED_MEM);
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip1.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip1.show();            // Turn OFF all pixels ASAP
    strip1.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
rainbow(10);
}

void colorWipe(int wait) { 
    for(int i=0; i<strip.numPixels(); i++) { 
        strip.setPixelColor(i, colorInt);                       
    }     
    strip.setBrightness(map(round(brightness+ .5), 0,101,0,255));
    strip.show(); 
    for(int i=0; i<8; i++) { 
        strip1.setPixelColor(i, colorInt2);                       
    }     
    strip1.setBrightness(map(round(brightness+ .5), 0,101,0,255));
   strip1.show(); 
   delay(wait);  
}

void rainbow(int wait) 
{
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    while (colorInt != (0,0,0)){
      colorWipe(10);
    }
    strip.setBrightness(255);
    strip.rainbow(firstPixelHue);
    strip.show(); // Update strip with new contents
    strip1.setBrightness(255);
    strip1.rainbow(firstPixelHue);
    strip1.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}