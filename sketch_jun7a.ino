#include <Adafruit_Circuit_Playground.h>

const int LEDpin = 13;
float totalDistanceCovered = 0;
bool gameLoss = false;
bool gameWin = true;
bool rightWarn = false;
bool leftWarn = false;
bool rightTurn = false;
bool leftTurn = false;


void setup() {
  pinMode(LEDpin, OUTPUT);
  CircuitPlayground.begin();
  Serial.begin(9600); 
  while(!Serial){
    for(int i = 0; i < 10; i++){
      CircuitPlayground.clearPixels();
      CircuitPlayground.setPixelColor(i, 300, 300, 0);
      delay(100);
      if(i >= 10){
        i = 0;
      }
    }
  }

  for(int i = 0; i < 3; i++){
    CircuitPlayground.clearPixels();
    delay(200);
    for(int i = 0; i < 10; i++){
      CircuitPlayground.setPixelColor(i, 0, 300, 0);
    }
    delay(200);
  }
  CircuitPlayground.clearPixels();
  Serial.println("Serial console detected. Begining game...");
  delay(500);
  Serial.println("Go! Make engine sounds to make your car go forward!");
  Serial.println("Twist your device to turn your car.");
}


void loop() {
  float playerEngineRaw = CircuitPlayground.mic.soundPressureLevel(10);
  float playerEngine = constrain(playerEngineRaw, 40, 80);
  float distanceCovered = map(playerEngine, 40, 80, 0, 30);
  totalDistanceCovered = totalDistanceCovered + distanceCovered;
  
  float engineLightStrength = map(playerEngine, 40, 80, 0, 255);
  analogWrite(LEDpin, engineLightStrength);

  int integerizedDistance = (int)totalDistanceCovered;
  int distanceRemainder = integerizedDistance % 1000;

  if(distanceRemainder >= 0 && distanceRemainder <= 199){
    CircuitPlayground.setPixelColor(4, 0, 0, 50);
    CircuitPlayground.setPixelColor(5, 0, 0, 50);
  }
  if(distanceRemainder <= -1 || distanceRemainder >= 200){
    CircuitPlayground.setPixelColor(4, 0, 0, 0);
    CircuitPlayground.setPixelColor(5, 0, 0, 0);
  }
  

  if(distanceRemainder >= 200 && distanceRemainder <= 399){
    CircuitPlayground.setPixelColor(3, 0, 0, 50);
    CircuitPlayground.setPixelColor(6, 0, 0, 50);
  }
  if(distanceRemainder <= 199 || distanceRemainder >= 400){
    CircuitPlayground.setPixelColor(3, 0, 0, 0);
    CircuitPlayground.setPixelColor(6, 0, 0, 0);
  }
  if ((totalDistanceCovered >= 5000 && totalDistanceCovered <= 6000) || (totalDistanceCovered >= 11000 && totalDistanceCovered <= 12000) || (totalDistanceCovered >= 25000 && totalDistanceCovered <= 26000)){
    CircuitPlayground.setPixelColor(0, 30, 30, 0);
    rightWarn = true;
  }
  else{
    CircuitPlayground.setPixelColor(0, 0, 0, 0);
    rightWarn = false;
  }
  
  //check to warn of left turn at 9000, 16000, and 20000 units
  if ((totalDistanceCovered >= 8000 && totalDistanceCovered <= 9000) || (totalDistanceCovered >= 15000 && totalDistanceCovered <= 16000) || (totalDistanceCovered >= 19000 && totalDistanceCovered <= 20000)){
    CircuitPlayground.setPixelColor(9, 30, 30, 0);
    leftWarn = true;
  }
  else{
    CircuitPlayground.setPixelColor(9, 0, 0, 0);
    leftWarn = false;
  }

    if ((totalDistanceCovered >= 5900 && totalDistanceCovered <= 6000) || (totalDistanceCovered >= 11900 && totalDistanceCovered <= 12000) || (totalDistanceCovered >= 25900 && totalDistanceCovered <= 26000)){
    rightTurn = true;   
  }
  else{
    rightTurn = false;
  }
  
  if ((totalDistanceCovered >= 8900 && totalDistanceCovered <= 9000) || (totalDistanceCovered >= 15900 && totalDistanceCovered <= 16000) || (totalDistanceCovered >= 19900 && totalDistanceCovered <= 20000)){
    leftTurn = true;  
  }
  else{
    leftTurn = false;
  }

  float xAxis = CircuitPlayground.motionX();

  if((rightTurn == true && xAxis <= 4) || (leftTurn == true && xAxis >= -4)){
    Serial.println("BAHAHA you crashed! You loose :(");
    Serial.println("Make sure you turn your board to match the turn on the track (yellow light).");
    Serial.println("Reset and try again, you can do this XD!");
    gameLoss = true;
    
    //crash animation
    for(int i = 0; i < 10; i++){
      CircuitPlayground.setPixelColor(0, 300, 0, 0);
      CircuitPlayground.setPixelColor(9, 300, 0, 0);
      CircuitPlayground.clearPixels();
      delay(50);
    }
    delay(1000);
  }

    if (totalDistanceCovered >= 30000){
      gameWin = true;
      Serial.println("WHOOP WHOOP! WINNER ALERT!!");
      Serial.println("Reset your Arduino to try again if you wish, hehe");

    while(gameLoss == true){
      for(int i = 0; i < 10; i++){
        CircuitPlayground.clearPixels();
        CircuitPlayground.setPixelColor(i, 300, 0, 0);
        delay(500);
        if(i >= 10){
          i = 0;
          }
        }
    }
  }
}

