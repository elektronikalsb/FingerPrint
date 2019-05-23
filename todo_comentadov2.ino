


                                                  
                                                   /////////////////////////////////////////////////////////////////////////
                                                                 /*    DECLARACIÓN, VARIABLES Y LIBRERIAS   */
                                                  /////////////////////////////////////////////////////////////////////////


                                          
                                                         
#include <TimeLib.h>                      //  Libreria, necesária para trabajar con "reloj".
#include <ESP8266WiFi.h>                  //  Libreria, para poder programar el modulo wifi ESP8266.
#include <WiFiUdp.h>                      //  Librería, tipo de conexión datagrama, permite el envio de datagramas atraves de la red,(datagrama) paquete de datos que contituye un bloque de información.
#include <LiquidCrystal_I2C.h>            //  Librería, para utilizar LCD con el protocolo I2C.
#include <Wire.h>                         //  Librería, para comunicarse y transmitir datos al LCD.
#include <Adafruit_Fingerprint.h>         //  Librería, para el sensor de huella FPM10A.
#include <SoftwareSerial.h>               //  Librería, para tener comunicación con el sensor de huella, exacamente con los pines D4 y D3, en nuestro caso.
#include <WiFiClientSecure.h>             //  Librería, .............. 
String readString;                        //  Varible de tipo nombre, esto utilizamos para enlazar el ID con un nombre.
SoftwareSerial mySerial(D4, D3);          //  Indicamos los pines de la comunicación de datos del sensor huella.

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);    
                                          //  Traducción entre la librería del sensor de huellas y la comunicación serie "(datos)" del sensor. 


LiquidCrystal_I2C lcd(0x27, 20, 4);       //0x27 se utiliza para el funcionamineto del LCD con el I2C (LCM1602), y 0x20 se utiliza para el funcionamineto del LCD con el I2C (PCF8574)

const char ssid[] = "electrocutados";//"Lsb_irekia";    // Nombre del router wifi.
const char pass[] = "regletas";//"YL4d08BC";            //  Clave del router wifi.
const int buttonPin = 2;                                // the number of the pushbutton pin
int buttonState;                                        // the current reading from the input pin
int lastButtonState = LOW;                              // the previous reading from the input pin
unsigned long lastDebounceTime = 0;                     // the last time the output pin was toggled
unsigned long debounceDelay = 50;                       // the debounce time; increase if the output flickers
const char* host = "script.google.com";                 //  Nombre del hots, 
const int httpsPort = 443;                              //  Puerto del http, numero de comunicación.
// Use WiFiClientSecure class to create TLS connection
WiFiClientSecure client;
// SHA1 fingerprint of the certificate, don't care with your GAS service
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "AKfycbx6H_X1_00rQ39mtFXxYDjOtUhxYVusdTwdPw1LlWoz8-ccDss";  // Replace by your GAS service id. 
//Servidor NTP:
static const char ntpServerName[] = "us.pool.ntp.org";  // Es una variable de tipo dato, que nos dice a que servidor se conecta y no cambia el nombre.
const int timeZone = 1;                                 /* Horario Europeo Central, con esto le decimos que la zona horaria que recibe el NTP sea mas 1, esto es para que,
                                                           cojamos nuestra hora Europea.*/

unsigned long clearlcd;                                 //  Variable en la que hacemos un borrado de la pantalla LCD.               
uint8_t id;                                             //  La variable que usamos para obtener o guardar una ID, "(un registro)".

WiFiUDP Udp;
unsigned int localPort = 8888;                          //  Puerto local para hacerle caso al paquete UTP.
time_t getNtpTime();                                    //  El servidor nos da la hora.
void digitalClockDisplay();                             //  Función en la que guardamos el dato digito de la hora.
void printDigits(int digits);                           //  Variable que guarda el dígito y lo mete en una variable de tipo entero que lo llamamos digitos.
void sendNTPpacket(IPAddress &address);                 //  Declaración de una IP. Se utiliza para declarar direciones locales y remotas.
void estado();                                          
void estado1();
int rele=D7;                                            //  Declaración relé.
int ledrojo=D5;                                         //  Declaración ledrojo.
int ledverde=D6;                                        //  Declaración ledverde.
int buzzer=D8;                                          //  Declaración altavóz.
int llave=D0;                                           //  Declaración llave.
String Nombres[] = { "David",  "Inaki.M", "Jorge", "Ander.U", "Armando", "Ander.E",  "Josema.alumno ","Ronny", "Karim", "Nahuel"
                    ,"Inaki.A profe", "Josema.F profe"                                                                            };
                                                                                                                  
                                                        //  Aqui ponemos los nombres que querámos, en órden segun el ID, preferible dejarlo predeterminado.



                                                  /////////////////////////////////////////////////////////////////////////
                                                                                 //   S E T U P   //
                                                  /////////////////////////////////////////////////////////////////////////

                                                  

void setup()                              
  {
  lcd.init();                                           //  Inizializamos el LCD.
  lcd.backlight();                                      //  Encendemos la luz de la parte trasera del LCD.
  Serial.begin(9600);                                   /*  Inizializamos la comunicación serie, para poder ver datos en el terminal serie en el ordenador.
                                                            El 9600 es la velocidad de comunicación, con lo cual en el serial pondremos 9600 baudrates.*/
  delay(250);                                           //
  Serial.println("TimeNTP Example");                    //
  Serial.print("Connecting to ");                       //
  Serial.println(ssid);                                 //                       
  WiFi.mode(WIFI_STA);                                  //
  lcd.setCursor(0,0);                                   //                              
  lcd.print("INICIANDO....");                           //
  pinMode(rele,OUTPUT);                                 //
  pinMode(ledrojo,OUTPUT);
  pinMode(ledverde,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(llave,INPUT);
  pinMode(buttonPin, INPUT);
   digitalWrite(rele,LOW);
    digitalWrite(ledverde,LOW);
    digitalWrite(ledrojo,HIGH);
    digitalWrite(buzzer,LOW);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
    if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
   }
   
     finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  
  WiFi.begin(ssid, pass);
  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  //sendData(113,114);    // Send test data
}

time_t prevDisplay = 0; // when the digital clock was displayed

  uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

/*//////////////////////// Programa Principal //////////////////////////////////////////*/

void loop()
{
  if (digitalRead(llave)==1){
    enrrolando();
    Serial.println("Acabado Enrrolando");
  }
  else
  {
  if (timeStatus() != timeNotSet) 
    if (now() != prevDisplay) { //update the display only if time has changed
        prevDisplay = now();
        digitalClockDisplay();
     }
  
  int huella = getFingerprintIDez();
  Serial.println(huella);
  if (huella>=1 && huella<=127)
  {
    estado();
    serverloop(huella);
  }
  else
    estado1();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void digitalClockDisplay()
{
  // digital clock display of the time
  printDigits_lcd(hour(), 5, 1);    //
  lcd.setCursor(7,1);
  lcd.print(":");                   //
  printDigits_lcd(minute(), 8, 1);  //
  lcd.setCursor(10,1);
  lcd.print(":");
  printDigits_lcd(second(), 11, 1);
  
  printDigits_lcd(day(), 5, 2);    //
  lcd.setCursor(7,2);
  lcd.print("/");                   //
  printDigits_lcd(month(), 8, 2);  //
  lcd.setCursor(10,2);
  lcd.print("/");
  lcd.print(year()); 
  delay(1000);
  
  Serial.print("Time");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print("Date");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void printDigits_lcd(int digits, int pos, int fila)

{
  // utility for digital clock display: prints preceding colon and leading 0
  if (digits < 10)
  {
    lcd.setCursor(pos,fila);
    lcd.print("0");
    lcd.setCursor(pos+1, fila);
    lcd.print(digits);
  }
  else 
  {  
  lcd.setCursor(pos,fila);
  lcd.print(digits);
  }
}
    
/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void estado()

  
    {
 
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("PASA ZAITEZKE");
    lcd.setCursor(5,3);
    lcd.print(Nombres[finger.fingerID-1]);
    digitalWrite(ledrojo,LOW);
    digitalWrite(ledverde,HIGH);
    digitalWrite(rele,HIGH);
    digitalWrite(buzzer,HIGH);
     WiFi.mode(WIFI_STA);
     sendData(113,114);
    delay(2000);
    digitalWrite(rele,LOW);
    digitalWrite(buzzer,LOW);
    digitalWrite(ledverde,LOW);
    delay(500);
   
    
    digitalWrite(rele,LOW);
    
    
    digitalWrite(ledverde,LOW);
    digitalWrite(ledrojo,HIGH);
    digitalWrite(buzzer,LOW);
    lcd.init();
    delay(200);
    lcd.clear();
    lcd.setCursor(0,0);//lcd.setCursor(0,0);
    lcd.print("Introduzca huella");
    
    
    }
  
 
  void estado1()
 
    {  
    digitalWrite(rele,LOW);
    digitalWrite(ledverde,LOW);
    digitalWrite(ledrojo,HIGH);
    digitalWrite(buzzer,LOW);
    //lcd.clear();
    lcd.setCursor(0,0);//lcd.setCursor(0,0);
    lcd.print("Introduzca huella");
    }

 
 uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) 
 /* lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Acceso Denegado");
  delay(1500);
  lcd.clear();*/

  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

///////Funcion de enrroling

void enrrolando(){
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Introduzca");
  lcd.setCursor(13,0);
  lcd.print("Nueva");
  lcd.setCursor(0,1);
  lcd.print("Huella");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INDRODUCIENDO");
  lcd.setCursor(0,1);
  lcd.print("HUELLA");
 
  while (! getFingerprintEnroll() );
  
}


uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Suelta el dedo");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Vuelva a introducir");
  lcd.setCursor(0,1);
  lcd.print("Huella");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Huella Guardada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}

// Function for Send data into Google Spreadsheet
void sendData(int tem,int hum)
{
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
  Serial.println("certificate matches");
  } else {
  Serial.println("certificate doesn't match");
  }
  String string_temperature =  String(tem, DEC); 
  String string_humidity =  String(hum, DEC); 
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
  String line = client.readStringUntil('\n');
  if (line == "\r") {
    Serial.println("headers received");
    break;
  }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
  Serial.println("esp8266/Arduino CI successfull!");
  } else {
  Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}



 //función server loop

 void serverloop(int huella)
 {
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // Send test data when button is pressed
      if (buttonState == HIGH) 
   {
        sendData(huella,114);
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    Serial.println(readString);  //so you can see the captured string 
    int n = readString.toInt();  //convert readString into a number

    // auto select appropriate value, copied from someone elses code.
    sendData(analogRead(A0),n);
    readString=""; //empty for next input
  } 
}
