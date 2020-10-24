#include <DallasTemperature.h>
#include <OneWire.h>

// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS_1 12
#define ONE_WIRE_BUS_2 13 
#define SHIM_PELTIE 10 


int peltie_voltage_value = 0;
int dx_peltie_voltage_value = 1;

int peltie_voltage_value_req = -1;

// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire_1(ONE_WIRE_BUS_1);
OneWire oneWire_2(ONE_WIRE_BUS_2);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors_1(&oneWire_1);
DallasTemperature sensors_2(&oneWire_2);

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
   // Start up the library 
  sensors_1.begin(); 
  sensors_2.begin(); 

  // declare SHIM_PELTIE to be an output:
  //pinMode(SHIM_PELTIE, OUTPUT);
}

int ff = 0;

void loop() {
  // print the string when a newline arrives:
  Serial.println("<v_" + String(peltie_voltage_value) + ">");
  
  if (stringComplete) {
    int value = atoi(inputString.c_str());
    peltie_voltage_value_req = value;
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  //peltie
  analogWrite(SHIM_PELTIE, peltie_voltage_value);

  if (peltie_voltage_value_req != -1){
    if (peltie_voltage_value < peltie_voltage_value_req) {
      peltie_voltage_value += dx_peltie_voltage_value;
    } else if (peltie_voltage_value > peltie_voltage_value_req) {
      peltie_voltage_value -= dx_peltie_voltage_value;
    } else {
      peltie_voltage_value_req = -1;
    }
  }

  //delaaaaaaay
  ff++;
  if (ff > 200) {
    ff = 0;
  } else return;

  sensors_1.requestTemperatures(); // Send the command to get temperature readings
  sensors_2.requestTemperatures();
  
  Serial.println("<s1_" + String(sensors_1.getTempCByIndex(0)) + ">");
  Serial.println("<s2_" + String(sensors_2.getTempCByIndex(0)) + ">");
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
