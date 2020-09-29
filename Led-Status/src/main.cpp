#include <Arduino.h>

/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
 
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

//#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
//#include <DHT.h>

Servo servo;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

// Select your pin with physical button
char auth[] = "X82gvvjut3FobcE3aJm6hOqoXDHORF9V";
char ssid[] = "TUGINO";
char pass[] = "ikhsan123";
const int btnPin1 = D1;
const int btnPin2 = D2;
//const int btnPin3 = D3;
const int btnPin4 = D4;
const int Pin6 = D6;
const int analogInPin = A0;
//#define DHT11_PIN D3
//#define DHTPIN 0          // D3
//#define DHTTYPE DHT11     // DHT 11
//DHT dht(DHTPIN, DHTTYPE);
// const int Suhu ;
int sensorValue = 0;

WidgetLED led1(V1);
WidgetLED led2(V2);
WidgetLED led3(V6);
WidgetLED led4(V7);

BlynkTimer timer;

void tweetUptime()
{
  long uptime = millis() / 60000L;
  Serial.println("Tweeting every 10 minutes ;)");

  // Actually send the message.
  // Note:
  //   We allow 1 tweet per 15 seconds for now.
  //   Twitter doesn't allow identical subsequent messages.
  Blynk.tweet(String("Running for ") + uptime + " minutes.");
}

/*void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, h);
}*/

void blinkLedWidget()
{
  if (led2.getValue()) {
    led2.off();
    Serial.println("LED on V2: off");
     digitalWrite(Pin6, LOW); 
  } else {
    led2.on();
    Serial.println("LED on V2: on");
    digitalWrite(Pin6, HIGH); 
  }
}

// V3 LED Widget represents the physical button state
boolean btnState = false;
void buttonLedWidget()
{
  // Read button
  boolean isPressed = (digitalRead(btnPin1) == HIGH);
 
  // If state has changed...
  if (isPressed != btnState) {
    if (isPressed) {
      led1.off();
    } else {
      led1.on();
    }
    btnState = isPressed;
  }

}


void setup()
{
  // Debug console
  Serial.begin(9600);

 Blynk.begin(auth, ssid, pass);
 servo.attach(15);
// dht.begin();
  // Setup physical button pin (active low)
  pinMode(btnPin1, INPUT_PULLUP);
  //pinMode(btnPin2, INPUT_PULLUP);
 // pinMode(btnPin3, INPUT_PULLUP);
 // pinMode(btnPin4, INPUT_PULLUP);
  pinMode(btnPin2, OUTPUT);
  // pinMode(btnPin3, OUTPUT);
  pinMode(btnPin4, OUTPUT);
  pinMode(Pin6, OUTPUT);
  timer.setInterval(500L, buttonLedWidget);
  timer.setInterval(1000L, blinkLedWidget);
  //timer.setInterval(1000L, sendSensor);
  
}

void loop()
{
  Blynk.run();
  timer.run();
  float Suhu;
  float Temp;
  Suhu = analogRead(A0);
  Temp = (Suhu - 23)/10;
  if (( Temp ) > ( 90 ))
 // if (( ( analogRead(A0) ) > ( 900 ) ))
  {
    Serial.println("ALARM"); 
    Blynk.notify("AWAS!! Temperature over 90C!");
    Blynk.email("tugino@gmail.com", "ESP8266 Alert", "AWAS!! Temperature over 90C!");
    Blynk.tweet("AWAS!! Temperature over 90C!");
   
  }
    else
  {
    Serial.println("AMAN"); 
  }
   Blynk.virtualWrite(V10, Temp);
   delay(100);

}
BLYNK_WRITE(V3)
{
servo.write(param.asInt());
}
BLYNK_WRITE(V4)
{
servo.write(0);
}
BLYNK_WRITE(V5)
{
servo.write(90);
}
