#include <lib_dmx.h>
#include <Adafruit_NeoPixel.h>

#define    DMX512 (0)

#define PIN 13
#define NUM_LEDS 64
#define BRIGHTNESS 255

#define stp1 2
#define dir1 3
#define EN1  4

#define stp2 5
#define dir2 6
#define EN2  7

#define MS11 8
#define MS21 9
#define MS12 10
#define MS22 11

#define PanChannel 0
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

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
  ArduinoDmx0.set_control_pin(12); 
  ArduinoDmx0.set_rx_address(1); 
  ArduinoDmx0.set_rx_channels(192);
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
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
  int DMXValue1 = ArduinoDmx0.RxBuffer[PanChannel];
  int DMXValue2 = ArduinoDmx0.RxBuffer[TiltChannel];
  int Target1 = map (DMXValue1, 0, 255, 0, 800);
  int Target2 = map (DMXValue2, 0, 255, 0, 300);
  uint8_t RedTarget = ArduinoDmx0.RxBuffer[RedChannel];
  uint8_t GreenTarget = ArduinoDmx0.RxBuffer[GreenChannel];
  uint8_t BlueTarget = ArduinoDmx0.RxBuffer[BlueChannel];
  uint8_t WhiteTarget = ArduinoDmx0.RxBuffer[WhiteChannel];
  if(Target1 >= (Motor1Current + 3))
  {
    motor1(Target1 - Motor1Current);
  }
  else if (Target1 <= (Motor1Current - 3))
  {
    motor1(Target1 - Motor1Current);
  }
  if(Target2 >= (Motor2Current + 3))
  {
    motor2(Target2 - Motor2Current);
  }
  else if (Target2 <= (Motor2Current - 3))
  {
    motor2(Target2 - Motor2Current);
  }
  if (RedTarget != RedCurrent || GreenTarget != GreenCurrent || BlueTarget != BlueCurrent || WhiteTarget != WhiteCurrent)
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

void motor1(int move1)
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

void motor2(int move2)
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

void LEDS(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t White)
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

