#include <lib_dmx.h>
#include <Adafruit_NeoPixel.h>

#define    DMX512 (0)

#define LEDPIN 13
#define NUM_LEDS 64
#define BRIGHTNESS 255

#define stp1 2 //Motor Driver 1, Step Pin
#define dir1 3 //Motor Driver 1, Direction Pin
#define EN1  4 //Motor Driver 1, Enable Pin

#define stp2 5 //Motor Driver 2, Step Pin
#define dir2 6 //Motor Driver 2, Direction Pin
#define EN2  7 //Motor Driver 2, Enable Pin

#define MS11 8 //Motor Driver 1, Microstep Pin 1
#define MS21 9 //Motor Driver 1, Microstep Pin 2
#define MS12 10 //Motor Driver 2, Microstep Pin 1
#define MS22 11 //Motor Driver 2, Microstep Pin 2

#define PanChannel 0 //Channel numbers are Zero Indexed
#define TiltChannel 1
#define RedChannel 2
#define GreenChannel 3
#define BlueChannel 4
#define WhiteChannel 5

int Motor1Current = 0;
int Motor2Current = 0;

uint8_t RedCurrent=0;
uint8_t GreenCurrent=0;
uint8_t BlueCurrent=0;
uint8_t WhiteCurrent=0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDPIN, NEO_GRBW + NEO_KHZ800); //Enables Neopixels

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
  ArduinoDmx0.set_control_pin(12); //Set DMX control pin
  ArduinoDmx0.set_rx_address(1); 
  ArduinoDmx0.set_rx_channels(192); //Sets number of DMX channels
  ArduinoDmx0.init_rx(DMX512);
  pinMode(stp1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(stp2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(MS11, OUTPUT);
  pinMode(MS21, OUTPUT);
  pinMode(MS12, OUTPUT);
  pinMode(MS22, OUTPUT);
  resetEDPins(); 
}
void loop() {
  digitalWrite(EN1, LOW); //Enables motor 1
  digitalWrite(EN2, LOW); //Enables motor 1
  int DMXValue1 = ArduinoDmx0.RxBuffer[PanChannel];
  int DMXValue2 = ArduinoDmx0.RxBuffer[TiltChannel];
  int PanTarget = map (DMXValue1, 0, 255, 0, 400); //Pan motor is mapped to one full rotation from the DMX value
  int TiltTarget = map (DMXValue2, 0, 255, 0, 200); //Tilt motor is mapped to 1/2 of a rotation from the DMX value
  uint8_t RedTarget = ArduinoDmx0.RxBuffer[RedChannel];
  uint8_t GreenTarget = ArduinoDmx0.RxBuffer[GreenChannel];
  uint8_t BlueTarget = ArduinoDmx0.RxBuffer[BlueChannel];
  uint8_t WhiteTarget = ArduinoDmx0.RxBuffer[WhiteChannel];
  if(PanTarget != Motor1Current) //Checks to see if the current pan motor position is different than the target
  {
    Pan(PanTarget - Motor1Current);
  }
  if(TiltTarget != Motor2Current) //Checks to see if the current tilt motor position is different than the target
  {
    Tilt(TiltTarget - Motor2Current);
  }
  if (RedTarget != RedCurrent || GreenTarget != GreenCurrent || BlueTarget != BlueCurrent || WhiteTarget != WhiteCurrent) //Checks to see if the current colors are different than the targets
  {
    LEDS(RedTarget, GreenTarget, BlueTarget, WhiteTarget);
  }
  resetEDPins();
}

void resetEDPins()
{
  digitalWrite(stp1, LOW);
  digitalWrite(dir1, LOW);
  digitalWrite(EN1, HIGH);
  digitalWrite(stp2, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(EN2, HIGH);
  digitalWrite(MS11, LOW);
  digitalWrite(MS21, LOW);
  digitalWrite(MS12, LOW);
  digitalWrite(MS22, LOW);
}

void Pan(int move1) //Control logic for pan
{
  if (move1 > 0)
  {
    digitalWrite(dir1, LOW);
    digitalWrite(stp1,HIGH);
    delay(1);
    digitalWrite(stp1,LOW);
    
    delay(1);
    Motor1Current += 1;
  }
  if (move1 < 0)
  {
    digitalWrite(dir1, HIGH);
    digitalWrite(stp1,HIGH);
    delay(1);
    digitalWrite(stp1,LOW);
    
    delay(1);
    Motor1Current -= 1;
  }
}

void Tilt(int move2) //Control logic for tilt
{
  if (move2 > 0)
  {
    digitalWrite(dir2, LOW);
    digitalWrite(stp2,HIGH);
    delay(1);
    digitalWrite(stp2,LOW);
    delay(1);
    Motor2Current += 1;
  }
  if (move2 < 0)
  {
    digitalWrite(dir2, HIGH);
    digitalWrite(stp2,HIGH);
    delay(1);
    digitalWrite(stp2,LOW);
    delay(1);
    Motor2Current -= 1;
  }
}

void LEDS(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t White) //Control logic for LEDs
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Red, Green, Blue, White); 
  }
  strip.show();
  delay(32);
  RedCurrent = Red;
  GreenCurrent = Green;
  BlueCurrent = Blue;
  WhiteCurrent = White;
}

