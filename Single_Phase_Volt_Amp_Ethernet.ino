// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3


#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);


#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance

float Irms = emon1.Irms;             //extract Irms into Variable
float totpwr=0.0;                   //init variables
float avgamps=0.0;
float amphr=0.0;
float energywh=0.0;
float energykwh=0.0;
float totpwr2=0.0;

void setup()
{  
 
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  
  
  emon1.voltage(2, 114, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 39.4);       // Current: input pin, calibration.
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
                    // add a meta refresh tag, so the browser pulls again every 5 seconds:
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          // output the value of each analog input pin
        
          
    client.print("Total Watts is: ");
    client.print(totpwr2);
    client.println("<br />"); 
    client.print("Watt Hour: ");
    client.print(energywh);
    client.println("<br />"); 
    client.print("Kilo-Watt Hour: ");
    client.print(energykwh);
    client.println("<br />"); 
    client.print("Real Power:");
    client.print(emon1.realPower);
    client.println("<br />"); 
    client.print("Voltage:");
    client.print(emon1.Vrms);
    client.println("<br />"); 
    client.print("Current:");
    client.print(emon1.Irms);
    client.println("<br />"); 
    client.print("Power Factor:");
    client.print(emon1.powerFactor);
    client.println("<br />"); 
        
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
        
        
  long test = millis(); // calculate time in milisec
 long time=test/1000; // convert time to sec
  
   
  Serial.println("WattR WattA  Volts  Amps  PF ");
  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  Serial.println(" ");
  Serial.print(time);            //print time in seconds
  Serial.println(" Seconds ");
  
  
  float realPower       = emon1.realPower;        //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms;             //extract Irms into Variable
  
  totpwr=totpwr+realPower; // accumulate pwr per cycle, not per second.
  totpwr2=totpwr/3.65; //accumulateed power /3.65 ratio, = wATTS  
  energywh=totpwr2/3600; //wh =accumulated power /3600
  energykwh=energywh/1000; //wh/1000=kwh
  
  
  Serial.print(totpwr2);
  Serial.println(" Total Watts  ");
  Serial.print(energywh);
  Serial.println(" WH ");
  Serial.print(energykwh);
  Serial.println(" KWH ");
  }}}
}

