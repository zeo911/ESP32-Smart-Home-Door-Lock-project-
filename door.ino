#include <Servo.h>
#include <Keypad.h>

#define LOCKED_POSITION 0
#define UNLOCKED_POSITION 90

const int servoPin = 2; 
Servo doorLockServo;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

bool doorLocked = true;
const String correctPin = "1234";

void setup() {
  Serial.begin(9600);
  doorLockServo.attach(servoPin);
  lockDoor();
}

void loop() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    if (key == '*' && doorLocked) {
      if (checkPin()) {
        unlockDoor();
      } else {
        Serial.println("Incorrect PIN");
      }
    } else if (key == '#') {
      lockDoor();
    }
  }
}

void lockDoor() {
  doorLockServo.write(LOCKED_POSITION);
  doorLocked = true;
  Serial.println("Door locked");
}

void unlockDoor() {
  doorLockServo.write(UNLOCKED_POSITION);
  doorLocked = false;
  Serial.println("Door unlocked");
}

bool checkPin() {
  String enteredPin = "";
  char key;
  while ((key = keypad.getKey()) != '#') {
    if (key != NO_KEY) {
      enteredPin += key;
    }
  }
  return enteredPin == correctPin;
}
