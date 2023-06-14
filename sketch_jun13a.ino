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
      CircuitPlayground.setPixelColor(i, 200, 200, 0);
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
      CircuitPlayground.setPixelColor(i, 0, 200, 0);
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

  //Listen to sound (imitating car noise) or blowing the circuit playground and stores it as data
  float playerEngineRaw = CircuitPlayground.mic.soundPressureLevel(10);
  float playerEngine = constrain(playerEngineRaw, 60, 90);
  float distanceCovered = map(playerEngine, 60, 90, 0, 30);
  totalDistanceCovered = totalDistanceCovered + distanceCovered;
  
  //Lights up LED 13 based on sound that is heard
  float engineLightStrength = map(playerEngine, 60, 90, 0, 255);
  analogWrite(LEDpin, engineLightStrength);

  //turn data into integer to find the remainder (distance)
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

  if(distanceRemainder >= 400 && distanceRemainder <= 599){
    CircuitPlayground.setPixelColor(2, 0, 0, 50);
    CircuitPlayground.setPixelColor(7, 0, 0, 50);
  }
  if(distanceRemainder <= 399 || distanceRemainder >= 600){
    CircuitPlayground.setPixelColor(2, 0, 0, 0);
    CircuitPlayground.setPixelColor(7, 0, 0, 0);
  }
  
  if(distanceRemainder >= 600 && distanceRemainder <= 799){
    CircuitPlayground.setPixelColor(1, 0, 0, 50);
    CircuitPlayground.setPixelColor(8, 0, 0, 50);
  }
  if(distanceRemainder <= 599 || distanceRemainder >= 800){
    CircuitPlayground.setPixelColor(1, 0, 0, 0);
    CircuitPlayground.setPixelColor(8, 0, 0, 0);
  }

  //Warns you that a right turn is coming up
  if ((totalDistanceCovered >= 5000 && totalDistanceCovered <= 6000) || (totalDistanceCovered >= 11000 && totalDistanceCovered <= 12000) || (totalDistanceCovered >= 25000 && totalDistanceCovered <= 26000)){
    CircuitPlayground.setPixelColor(0, 30, 30, 0);
    rightWarn = true;
  }
  else{
    CircuitPlayground.setPixelColor(0, 0, 0, 0);
    rightWarn = false;
  }
  
  //Warns you that a left turn is coming up
  if ((totalDistanceCovered >= 8000 && totalDistanceCovered <= 9000) || (totalDistanceCovered >= 15000 && totalDistanceCovered <= 16000) || (totalDistanceCovered >= 19000 && totalDistanceCovered <= 20000)){
    CircuitPlayground.setPixelColor(9, 30, 30, 0);
    leftWarn = true;
  }
  else{
    CircuitPlayground.setPixelColor(9, 0, 0, 0);
    leftWarn = false;
  }

  //Turn Checker 
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

  float xAxis = CircuitPlayground.motionX();      //Recieves the data if player is turning (movement)

  if(rightWarn == false && xAxis >=.5){
    CircuitPlayground.setPixelColor(0, 0, 0, xAxis*5);       //Turn light react to player turning

  }
  if(leftWarn == false && xAxis <=-.5){
    CircuitPlayground.setPixelColor(9, 0, 0, abs(xAxis)*5);
  }

  if(rightWarn == true && xAxis >=.5){
    CircuitPlayground.setPixelColor(0, 30-xAxis, 30+xAxis*10, 0);     //Turn light react to player movement and flash warning
  }
  if(leftWarn == true && xAxis <=-.5){
    CircuitPlayground.setPixelColor(9, 30-xAxis, 30+abs(xAxis)*10, 0);
  }

  if(rightWarn == true && xAxis <=.5){       
    CircuitPlayground.setPixelColor(0, 30, 30, 0);     //Turn light with no player movement but warning
  }
  if(leftWarn == true && xAxis >=-.5){
    CircuitPlayground.setPixelColor(9, 30, 30, 0);
  }

  if(rightWarn == false && xAxis <=.5){
    CircuitPlayground.setPixelColor(0, 0, 0, 0);       //No movement and no warning lights off
  }
  if(leftWarn == false && xAxis >=-.5){
    CircuitPlayground.setPixelColor(9, 0, 0, 0);
  }

  

  if((rightTurn == true && xAxis <= 4) || (leftTurn == true && xAxis >= -4)){
    Serial.println("BAHAHA you crashed! You loose :(");          
    Serial.println("Make sure you turn your board to match the turn on the track (yellow light).");
    Serial.println("Reset and try again, you can do this XD!");    
    gameLoss = true;
    
    //crash animation
    for(int i = 0; i < 10; i++){
      CircuitPlayground.setPixelColor(0, 200, 0, 0);
      CircuitPlayground.setPixelColor(9, 200, 0, 0);
      CircuitPlayground.clearPixels();
      delay(50);
    }
    delay(1000);
  }

    // Loosing the game 
    while(gameLoss == true){
      for(int i = 0; i < 10; i++){
        CircuitPlayground.playTone(200, 200);      //Plays the loosing sound
        twinkle();
        delay(200);
        }
    }

    // Winning the game
    if (totalDistanceCovered >= 30000){
      gameWin = true;
      Serial.println("WHOOP WHOOP! WINNER ALERT!!");
      Serial.println("Reset your Arduino to try again if you wish, hehe");     //Messages onto the serial monitor indicating that you won

      // Game winning 
    while(gameWin == true){
      for(int i = 0; i < 10; i++){
        CircuitPlayground.playTone(600, 200);    //Plays the winning sound
        rainbow();
        delay(200);
        }
    }
  }
}

// Rainbow color pattern
void rainbow() {
  uint32_t offset = millis() / 100;
  for (int i = 0; i < 10; ++i) {
    CircuitPlayground.setPixelColor(i, CircuitPlayground.colorWheel(((i * 256 / 10) + offset) & 255));
  }
  delay(50);
}

// Twinkle color pattern (red colors flashing)
void twinkle() {
  int randomIndex = random(0, 10); // generate a random number between 0 and 9
  CircuitPlayground.setPixelColor(randomIndex, 255, 0, 0); // light up the randomly selected LED
  delay(500);
  CircuitPlayground.setPixelColor(randomIndex, 0, 0, 0); // turn off the LED
  delay(500);
}
