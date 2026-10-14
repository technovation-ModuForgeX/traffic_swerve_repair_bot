// Traffic-swerving crack-repair robot (Uniko DVI / ATmega32A)
// IR sensors: A1 = left, A0 = right (value < THRESH means obstacle/car on that side)
// Motor pins: 12, 13 (left motor), 14, 15 (right motor)

const int THRESH = 100;

// ---- Timing (milliseconds) - tune these ----
const unsigned long DRIVE_TIME  = 15000;  // drive/swerve this long
const unsigned long REPAIR_TIME = 8000;   // then stop this long ("fixing the crack")

const int SWERVE_OUT  = 350;  // turn away from the car
const int SWERVE_FWD  = 400;  // pass the car
const int SWERVE_BACK = 350;  // turn back into lane

unsigned long phaseStart;

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  phaseStart = millis();
}

void loop() {
  // ---- Repair stop: checked at the top of every loop ----
  if (millis() - phaseStart >= DRIVE_TIME) {
    stopBot();
    delay(REPAIR_TIME);          // robot sits still = "fixing the crack"
    phaseStart = millis();       // restart the driving timer
  }

  int LS = analogRead(A1);
  int RS = analogRead(A0);

  if (LS > THRESH && RS > THRESH) {
    // Road clear
    forward();
  }
  else if (LS < THRESH && RS > THRESH) {
    // Car on left -> swerve right, pass, return to lane
    right();   delay(SWERVE_OUT);
    forward(); delay(SWERVE_FWD);
    left();    delay(SWERVE_BACK);
  }
  else if (LS > THRESH && RS < THRESH) {
    // Car on right -> swerve left, pass, return to lane
    left();    delay(SWERVE_OUT);
    forward(); delay(SWERVE_FWD);
    right();   delay(SWERVE_BACK);
  }
  else {
    // Blocked on both sides -> stop, back up, turn, continue
    stopBot();    delay(300);
    backward();   delay(800);
    right();      delay(500);
  }
}

void forward() {
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, LOW);
}
void backward() {
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  digitalWrite(15, HIGH);
}
void left() {
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(14, HIGH);
  digitalWrite(15, LOW);
}
void right() {
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(14, LOW);
  digitalWrite(15, HIGH);
}
void stopBot() {
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
}
