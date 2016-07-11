// Encoder
int encoderAPin = 7;
int encoderBPin = 6;
int encoderButtonPin = 8;
int aVal;
int bVal;
int buttonVal;
int aPrev;
int bPrev;
int buttonPrev;
bool aChanged = false;
bool bChanged = false;
int dir = 1;
// Switch
int switchPin = A7;
int sVal;
int sPrev;
int sPending;
int sFinal;
int sChangeCounter;
// getState
int incomingByte = 0;

void setup() {
  Serial.begin(9600);
  // Encoder
  pinMode(encoderAPin, INPUT);
  pinMode(encoderBPin, INPUT);
  pinMode(encoderButtonPin, INPUT);
  aPrev = digitalRead(encoderAPin);
  bPrev = digitalRead(encoderAPin);
  buttonPrev = digitalRead(encoderButtonPin);
  // Switch
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, LOW);
  sPrev = analogRead(switchPin);
  sPending = sPrev;
  sFinal = sPrev;
}

void loop() {
  // Encoder
  aVal = digitalRead(encoderAPin);
  bVal = digitalRead(encoderBPin);
  buttonVal = digitalRead(encoderButtonPin);
  if (aVal != aPrev) {
    aChanged = true;
    if (!bChanged) dir = 1;
  }
  if (bVal != bPrev) {
    bChanged = true;
    if (!aChanged) dir = -1;
  }
  if (aChanged && bChanged) {
    if (dir == 1) {
      Serial.println('R');
    } else {
      Serial.println('L');
    }
    aChanged = false;
    bChanged = false;
  }
  if (buttonVal != buttonPrev) {
    Serial.print('B'); Serial.println(buttonVal, DEC);
  }
  aPrev = aVal;
  bPrev = bVal;
  buttonPrev = buttonVal;
  // Switch
  sVal = analogRead(switchPin);
  if (sVal != sPrev) {
    sPending = sVal;
  }
  if (sPending != sFinal) {
    sChangeCounter ++;
    if (sChangeCounter > 400) {
      sFinal = sPending;
      sChangeCounter = 0;
      Serial.print('S'); Serial.println(sFinal, DEC);
    }
  } else {
    if (sChangeCounter > 0) sChangeCounter = 0;
  }
  sPrev = sVal;

  // getState
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 50) { // "2" key
      Serial.print('B'); Serial.print(buttonVal, DEC); Serial.print(';');
      Serial.print('S'); Serial.println(sFinal, DEC);
    }
  }
}
