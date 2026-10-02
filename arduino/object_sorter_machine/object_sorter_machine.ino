#include <Servo.h>

Servo gateServo;

const int IR_SENSOR_PIN = 2;
const int MOTOR_IN1 = 3;
const int MOTOR_IN2 = 4;
const int MOTOR_EN = 5;
const int SERVO_PIN = 9;
const int LED_RED = 10;
const int LED_GREEN = 11;
const int LED_BLUE = 12;

String inputBuffer = "";
bool conveyorRunning = false;
bool objectPresent = false;

struct BinPosition {
  int red;
  int green;
  int blue;
};

const BinPosition BIN_POSITIONS = {30, 95, 150};

void setup() {
  Serial.begin(9600);

  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_EN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  gateServo.attach(SERVO_PIN);
  gateServo.write(BIN_POSITIONS.green);
  stopConveyor();

  Serial.println("SYSTEM_READY");
}

void loop() {
  while (Serial.available() > 0) {
    char ch = (char)Serial.read();
    if (ch == '\n' || ch == '\r') {
      processCommand(inputBuffer);
      inputBuffer = "";
    } else {
      inputBuffer += ch;
    }
  }

  bool irState = digitalRead(IR_SENSOR_PIN) == HIGH;

  if (irState && !objectPresent) {
    objectPresent = true;
    Serial.println("OBJECT_DETECTED");
    startConveyor();
  }

  if (!irState) {
    objectPresent = false;
  }

  if (!conveyorRunning && !objectPresent) {
    stopConveyor();
  }

  delay(20);
}

void processCommand(String command) {
  command.trim();
  command.toUpperCase();

  if (command == "STATUS") {
    Serial.println("STATUS:ONLINE");
    return;
  }

  if (command == "START") {
    conveyorRunning = true;
    startConveyor();
    Serial.println("CONVEYOR_STARTED");
    return;
  }

  if (command == "STOP") {
    conveyorRunning = false;
    stopConveyor();
    gateServo.write(BIN_POSITIONS.green);
    Serial.println("CONVEYOR_STOPPED");
    return;
  }

  if (command.startsWith("SORT:")) {
    String label = command.substring(5);
    label.trim();

    if (label == "RED") {
      moveGate(BIN_POSITIONS.red);
      setIndicator(LED_RED, true);
      setIndicator(LED_GREEN, false);
      setIndicator(LED_BLUE, false);
      Serial.println("SORTED_RED");
    } else if (label == "GREEN") {
      moveGate(BIN_POSITIONS.green);
      setIndicator(LED_RED, false);
      setIndicator(LED_GREEN, true);
      setIndicator(LED_BLUE, false);
      Serial.println("SORTED_GREEN");
    } else if (label == "BLUE") {
      moveGate(BIN_POSITIONS.blue);
      setIndicator(LED_RED, false);
      setIndicator(LED_GREEN, false);
      setIndicator(LED_BLUE, true);
      Serial.println("SORTED_BLUE");
    } else {
      moveGate(BIN_POSITIONS.green);
      setIndicator(LED_RED, false);
      setIndicator(LED_GREEN, false);
      setIndicator(LED_BLUE, false);
      Serial.println("SORTED_DEFAULT");
    }

    delay(500);
    return;
  }
}

void moveGate(int angle) {
  gateServo.write(angle);
  delay(400);
}

void startConveyor() {
  conveyorRunning = true;
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_EN, 180);
}

void stopConveyor() {
  conveyorRunning = false;
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_EN, 0);
}

void setIndicator(int pin, bool state) {
  digitalWrite(pin, state ? HIGH : LOW);
}
