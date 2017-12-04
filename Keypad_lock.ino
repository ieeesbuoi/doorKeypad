#include <Keypad.h>
#include <Servo.h>

Servo myServo;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
//define the symbols on the buttons of the keypads

char hexaKeys[ROWS][COLS] = {
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'*','0','#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

String inputPass = "";

//You can change these
String correctPass = "1234";
int triesLeft = 3;
int wrongPassDelay = 5000;  //in ms
int lockDegrees = 146;
int unlockDegrees = 60;

void setup() {
  Serial.begin(9600);
  myServo.attach(10);
}

void loop() {
  char typedKey = customKeypad.getKey();

  if(typedKey){
    Serial.println(typedKey);
    if(typedKey == '#'){  //'#' initiates password confirmation sequence
      Serial.println("Tried password: " + inputPass);
      if(inputPass == correctPass){
        triesLeft = 3;
        Serial.println("Password was correct! Unlocking...");
        myServo.write(unlockDegrees); //unlocks
      }
      else{
        triesLeft--;
        Serial.println("Wrong password! Tries left: " + String(triesLeft));
        if(triesLeft == 0){
          Serial.println("Too many wrong tries! Please wait " + String(wrongPassDelay/1000) + "s to retry.");
          delay(wrongPassDelay);
          triesLeft = 3;
        }
      }
      inputPass = ""; //clears the input string
    }
    
    else if (typedKey == '*'){
      Serial.println("Locking...");
      inputPass = ""; //clears the input string
      myServo.write(lockDegrees); //locks
    }
    
    else{
      inputPass += typedKey;
    }
  }
}
