#include <Servo.h>
#include <Keypad.h>

#define LOCKED_POSITION 0
#define UNLOCKED_POSITION 90

const int servoPin = 2; // Pin connected to servo motor
Servo doorLockServo;

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; // Connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

bool doorLocked = true; // Initial state is locked
const String correctPin = "1234"; // Define your PIN code

void setup() {
  Serial.begin(9600);
  doorLockServo.attach(servoPin);
  lockDoor(); // Ensure the door is locked on startup
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
