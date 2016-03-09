// Your sketch must #include this library, and the Wire library.
// (Wire is a standard library included with Arduino.):

#include <SFE_BMP180.h>
#include <Wire.h>
#include <VirtualWire.h>  // you must download and install the VirtualWire.h to your hardware/libraries folder
 const int led_pin = 13;  
int numbers[3];

SFE_BMP180 pressure;
const int voltmetrupin = 0;

const float cap_inmagazinata = 500.0;
const float temp_referinta = 25.0;
const float fact_temp = 0.04;
const float tensiune_max = 9;
const float tensiune_min = 5;
const float peukert_number = 1.2;
const float R1 = 220.0; // valori rezistente divizor tensiune   
const float R2 = 220.0;

int val=0; // valoare achizitionata
float vout = 0.0; // tensiunea citita vout = (val * 5.0) / 1024.0;
float vin = 0.0; // tensiune baterie vin = vout / (R2/(R1+R2))
float maxvin = 0.0; // valoarea maxima a tensiunii bateriei
float i = 0.0; //intesitate curent in mA i = (vin * 1000) / (R1+R2)
float cap_ramasa;
float temperatura = 0;
float durata_viata;
void setup()
{
  Serial.begin(9600);
  Serial.println("REBOOT");
 vw_setup(2000);                


  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
}

void loop()
{
  char status;
  double T,P;
  
 

  status = pressure.startTemperature();
  if (status != 0)
  {
    
    delay(status);

   
    status = pressure.getTemperature(T);
    digitalWrite(led_pin, HIGH);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
       digitalWrite(led_pin, HIGH);
      Serial.println();
      numbers[0]=(int)T;
      
    
      status = pressure.startPressure(0);
      if (status != 0)
{
        delay(status);

  

        status = pressure.getPressure(P,T);
        if (status != 0)
        { Serial.print("Presiune: ");
          Serial.print(P,2);
          Serial.print(" mb");
          P=100*P;
          // Print out the measurement:
          Serial.print("Presiune: ");
          Serial.print(P,2);
          Serial.print(" PA, ");
          P=P*0.00750061561303;
          Serial.print(P);
          numbers[1]=int(P);
          Serial.println(" mmHg");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  val = analogRead(voltmetrupin);
     vout = (val * 5.0) / 1024.0;
     vin = vout / (R2/(R1+R2));
     // Serial.print(vin);
    // Serial.println();
     if (maxvin<vin) maxvin = vin;
     if (maxvin==0) {
  
  Serial.print("Conectati bateria:");
     
      }
      else {
        temperatura = T;
        // Pas 1 - calculare capacitate ramasa in functie de tensiune la borne
        cap_ramasa = max(0,((vin - tensiune_min) / (tensiune_max - tensiune_min)) * cap_inmagazinata);
         Serial.print(cap_ramasa);
     Serial.println();
        // Pas 2 - ajustare capacitate ramasa dupa temperatura
        cap_ramasa = cap_ramasa + (cap_ramasa * (temperatura - temp_referinta) * fact_temp);
        i = (vin * 1000) / (R1+R2);
        // Pas 3 - calculare  viata in zile dupa legea lui Peukert
        durata_viata = ((cap_ramasa * 1000.0) / pow(i,peukert_number)) / 24;
        durata_viata=durata_viata;
         numbers[2]=(int)durata_viata;
      }
vw_send( (uint8_t *)&numbers, sizeof(numbers));
 vw_wait_tx(); 
 digitalWrite(led_pin, LOW);// Wait for message to finish
  delay(1000);  // Pause for 5 seconds.
}
