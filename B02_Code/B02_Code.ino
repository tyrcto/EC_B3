#define BLYNK_PRINT SwSerial

#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); 

#include <BlynkSimpleStream.h>
#include <DHT.h>

// Authentication Token, needed for the Blynk App
char auth[] = "TYi42GYjaw9n0rEODuNUuH-RbmIexxVl";

//=======[DHT 11 Sensor]=======
#define DHTPIN 11          // DHT sensor pin
#define DHTTYPE DHT11     // defining the type of DHT sensor used
                          // in this case DHT 11 is used 
#define HIGH_TEMP 32      // If reached, will trigger High Temperature Alert to the Blynk App
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature(); 

  if (isnan(humidity) || isnan(temp)) {
    SwSerial.println("Failed to read from DHT sensor!");
    Blynk.notify("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, humidity);
  Blynk.virtualWrite(V6, temp);

  if(temp > HIGH_TEMP)Blynk.notify("Alert! High Temperature detected\n");
}
//=======[SOS & Basic ON Functions]======= 
#define LED_PIN 7

BLYNK_WRITE(V1){
  int button = param.asInt();
  digitalWrite(LED_PIN, button);
}

BLYNK_WRITE(V7){
  int on = param.asInt();
  if(on){
    for(int i=0;i<3;i++){
      digitalWrite(LED_PIN,HIGH);
      delay(100);
      digitalWrite(LED_PIN,LOW);
      delay(100);
    }
    delay(500);
     for(int i=0;i<3;i++){
      digitalWrite(LED_PIN,HIGH);
      delay(300);
      digitalWrite(LED_PIN,LOW);
      delay(100);
    }
    delay(500);
     for(int i=0;i<3;i++){
      digitalWrite(LED_PIN,HIGH);
      delay(100);
      digitalWrite(LED_PIN,LOW);
      delay(100);
    }
  }
  delay(1000);
}

//=======[RGB LED]=======
#define RED_PIN 2
#define GREEN_PIN 3
#define BLUE_PIN 4
int redVal = 0;
int greenVal = 0;
int blueVal = 0;


// "RGB LED color change" function
void RGB_color(int red, int green, int blue){
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

// Input from Virtual Pin 2 for the color RED
BLYNK_WRITE(V2){
  redVal = param.asInt();
  RGB_color(redVal, greenVal, blueVal);
}
// Same goes for BLUE and GREEN with their respective Virutal Pins
BLYNK_WRITE(V3){
  greenVal = param.asInt();
  RGB_color(redVal, greenVal, blueVal);
}

BLYNK_WRITE(V4){
  blueVal = param.asInt();
  RGB_color(redVal, greenVal, blueVal);
}

void setup()
{
  // Debug console
  SwSerial.begin(9600);
  pinMode(LED_PIN,OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
