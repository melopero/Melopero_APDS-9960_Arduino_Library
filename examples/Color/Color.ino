/* Author: Leonardo La Rocca */
#include "Melopero_APDS9960.h"

Melopero_APDS9960 device;

void setup() {
  Serial.begin(9600); // Initialize serial comunication
  while (!Serial); // wait for serial to be ready

  device.init(); // Initialize the comunication library
  device.reset(); // Reset all interrupt settings and power off the device

  device.enableAlsEngine();
  device.setAlsIntegrationTime(450);
  device.updateSaturation(); // updates the saturation value, stored in device.alsSaturation

  device.wakeUp();

}

void loop() {
  delay(450);

  device.updateColorData();
  
  float r = ((float) device.red) / device.alsSaturation; 
  float g = ((float) device.green) / device.alsSaturation; 
  float b = ((float) device.blue) / device.alsSaturation; 
  float c = ((float) device.clear) / device.alsSaturation; 

  Serial.println("Color data:");
  printColor(r,g,b,c);
}

void printColor(float r, float g, float b, float c){
  Serial.print("R: ");
  Serial.print(r);
  Serial.print(" G: ");
  Serial.print(g);
  Serial.print(" B: ");
  Serial.print(b);
  Serial.print(" C: ");
  Serial.println(c);
}