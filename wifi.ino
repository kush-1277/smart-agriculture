#include <SoftwareSerial.h>
#include <dht.h>
dht DHT;

#define airquality_sensor_pin 0
#define gas_sensor_pin 1
#define VAL_PROBE 2 // Analog pin 3
boolean DEBUG=true;
#define DHT11_PIN 4
#define RX 10
#define TX 11

String AP = "testnetwork";       // CHANGE ME
String PASS = "abcdefgh"; // CHANGE ME
String API = "SO4H0UGV0E6H92S5";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
String field2 = "field2";
String field3 = "field3";
String field4 = "field4";
String field5 = "field5";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX); 
 
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  DEBUG=true;
}
void loop() {
  int chk = DHT.read11(DHT11_PIN);
  int err;
  int airquality_value = analogRead(airquality_sensor_pin);
  int gas_value = analogRead(gas_sensor_pin);
  int moisture = analogRead(VAL_PROBE);
  Serial.print("Temperature = ");
  int temp=DHT.temperature;
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  int hum=DHT.humidity;
  Serial.println(DHT.humidity);
  Serial.print("Gas Value:> ");
  Serial.print(gas_value);
  Serial.println();
  Serial.print("Air Quality Value:> ");
  Serial.print(airquality_value);
  Serial.println();
  Serial.print("Soil Moisture Value:> ");
  int m=1000-moisture;
  Serial.println(1000-moisture);
  Serial.println("% send to Thingspeak");
  Serial.println();
  Serial.print("______________________________________\n");
  delay(1000);
  //valSensor = getSensorData();
  String getData = "GET /update?api_key=" + API +"&"+ field +"="+String(temp) +"&"+ field2 +"="+String(hum) +"&"+ field3 +"="+String(gas_value) +"&"+ field4 +"="+String(airquality_value) +"&"+ field5 +"="+String(m);
  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  esp8266.println(getData);delay(1500);countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0",5,"OK");
}
//int getSensorData(){
  //return random(1000); // Replace with 
//}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }

