#include <lib_dmx.h>

#define    DMX512 (0)

void setup() 
{
  ArduinoDmx0.set_control_pin(11); 
  ArduinoDmx0.set_rx_address(1); 
  ArduinoDmx0.set_rx_channels(192);
  ArduinoDmx0.init_rx(DMX512);    
}

void loop()
{
  analogWrite(3, ArduinoDmx0.RxBuffer[0]);
  analogWrite(5, ArduinoDmx0.RxBuffer[1]);
  analogWrite(6, ArduinoDmx0.RxBuffer[2]);
} 

