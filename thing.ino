#include <SoftwareSerial.h>                                 
#include <dht11.h>                                            
String agAdi = "modem";                
String agSifresi = "sifre";          

int rxPin = 10;                                           
int txPin = 11;                                         
int dht11Pin = 2;

String ip = "184.106.153.149";                              
float sicaklik, nem;
dht11 DHT11;

SoftwareSerial esp(rxPin, txPin);                           

void setup() {  
  
  Serial.begin(9600);  
  Serial.println("Started");
  esp.begin(115200);                                       
  esp.println("AT");                                         
  Serial.println("AT Yollandı");
  while(!esp.find("OK")){                                   
    esp.println("AT");
    Serial.println("ESP8266 Bulunamadı.");
  }
  Serial.println("OK Komutu Alındı");
  esp.println("AT+CWMODE=1");                                 
  while(!esp.find("OK")){                                   
    esp.println("AT+CWMODE=1");
    Serial.println("Ayar Yapılıyor....");
  }
  Serial.println("Client olarak ayarlandı");
  Serial.println("Aga Baglaniliyor...");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");   
  while(!esp.find("OK"));                                    
  Serial.println("Aga Baglandi.");
  delay(1000);
}
void loop() {
  esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           
  if(esp.find("Error")){                                     
    Serial.println("AT+CIPSTART Error");
  }
  DHT11.read(dht11Pin);
  sicaklik = (float)DHT11.temperature;
  nem = (float)DHT11.humidity;
  String veri = "GET api key";                              
  veri += "&field1=";
  veri += String(sicaklik);
  veri += "&field2=";
  veri += String(nem);                                      
  veri += "\r\n\r\n"; 
  esp.print("AT+CIPSEND=");                                  
  esp.println(veri.length()+2);
  delay(2000);

  
  if(esp.find(">")){                                          
    esp.print(veri);                                         
    Serial.println(veri);
    Serial.println("Veri gonderildi.");
    delay(1000);
  }
  Serial.println("Baglantı Kapatildi.");
  esp.println("AT+CIPCLOSE");                                
  delay(1000);                                              
}
