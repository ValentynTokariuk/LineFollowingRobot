int STBY = 3;
int PWMA = 5; // PWM control for the right motor
int PWMB = 6; // PWM control for the left motor
int BIN1 = 8; // Direction for motor B
int AIN1 = 7; // Direction for motor A
int sensorLeft = A2; // Left sensor
int sensorMiddle = A1; // Middle sensor
int sensorRight = A0; // Right sensor
int lastDetectedDirection = 0; // 1 for left, 0 for right
#define BASE_SPEED 40 // Base speed of the motors
#define TURN_SPEED 20 // Reduced turning speed for finer correction
#define THRESHOLD 580 // Adjusted threshold for detecting the line
void setup() {
 pinMode(AIN1, OUTPUT);
 pinMode(BIN1, OUTPUT)
 pinMode(STBY, OUTPUT);
 pinMode(PWMA, OUTPUT);
 pinMode(PWMB, OUTPUT);
 digitalWrite(STBY, HIGH); // Enable motor control board
 pinMode(sensorLeft, INPUT);
 pinMode(sensorMiddle, INPUT);
 pinMode(sensorRight, INPUT);
 Serial.begin(9600); // Start serial communication at 9600 baud
}
void loop() {
 int leftVal = analogRead(sensorLeft);
 int middleVal = analogRead(sensorMiddle);
 int rightVal = analogRead(sensorRight);
 Serial.print("L: ");
 Serial.print(leftVal);
 Serial.print(" M: ");
 Serial.print(middleVal);
 Serial.print(" R: ");
 Serial.println(rightVal);
 // Assess sensor values against the threshold
 bool leftDetected = leftVal > THRESHOLD;
 bool middleDetected = middleVal > THRESHOLD;
 bool rightDetected = rightVal > THRESHOLD;
 // Update the last detected direction
 if (leftDetected) {
 lastDetectedDirection = 1; // Left
 } else if (rightDetected && !leftDetected) {
 lastDetectedDirection = 0; // Right
 }
 // Implement logic based on sensor readings
 if (leftDetected && middleDetected && rightDetected) {
 moveStraight(BASE_SPEED);
 Serial.println("Action: Moving Straight - All sensors");
 } else if (middleDetected && leftDetected) {
 adjustSlightlyRight(BASE_SPEED, TURN_SPEED / 2);
 Serial.println("Action: Adjusting Slightly Left - Middle and Left sensors");
 } else if (middleDetected && rightDetected) {
 adjustSlightlyLeft(BASE_SPEED, TURN_SPEED / 2);
 Serial.println("Action: Adjusting Slightly Right - Middle and Right sensors");
 } else if (leftDetected) {
 adjustRight(BASE_SPEED, TURN_SPEED);
 Serial.println("Action: Adjusting Left - Left sensor");
 } else if (rightDetected) {
 adjustLeft(BASE_SPEED, TURN_SPEED);
 Serial.println("Action: Adjusting Right - Right sensor");
 } else if (middleDetected) {
 moveStraight(BASE_SPEED);
 Serial.println("Action: Moving Straight - Middle sensor only");
 } else {
 Serial.println("Action: Stopped - No line detected");
 searchForLine();
}
}
// Define movement functions
void moveStraight(int speed) {
 analogWrite(PWMA, speed);
 analogWrite(PWMB, speed);
 digitalWrite(AIN1, HIGH);
 digitalWrite(BIN1, HIGH);
 Serial.println("Moving Straight");
}
void adjustLeft(int baseSpeed, int turnSpeed) {
 analogWrite(PWMA, baseSpeed - turnSpeed);
 analogWrite(PWMB, baseSpeed + turnSpeed);
 digitalWrite(AIN1, HIGH);
 digitalWrite(BIN1, HIGH);
 Serial.println("Adjusting Left");
}
void adjustRight(int baseSpeed, int turnSpeed) {
 analogWrite(PWMA, baseSpeed + turnSpeed);
 analogWrite(PWMB, baseSpeed - turnSpeed);
 digitalWrite(AIN1, HIGH);
 digitalWrite(BIN1, HIGH);
 Serial.println("Adjusting Right");
}
void adjustSlightlyLeft(int baseSpeed, int turnSpeed) {
 analogWrite(PWMA, baseSpeed - turnSpeed);
 analogWrite(PWMB, baseSpeed);
 digitalWrite(AIN1, HIGH);
 digitalWrite(BIN1, HIGH);
 Serial.println("Adjusting Slightly Left");
}
void adjustSlightlyRight(int baseSpeed, int turnSpeed) {
 analogWrite(PWMA, baseSpeed);
 analogWrite(PWMB, baseSpeed - turnSpeed);
 digitalWrite(AIN1, HIGH);
 digitalWrite(BIN1, HIGH);
 Serial.println("Adjusting Slightly Right");
}
void stopMotors() {
 analogWrite(PWMA, 0);
 analogWrite(PWMB, 0);
 digitalWrite(STBY, LOW); // Optionally disable standby to save power
 Serial.println("Motors Stopped");
}
void searchForLine() {
 Serial.println("Searching for line...");
 while (true) {
 int leftVal = analogRead(sensorLeft);
 int middleVal = analogRead(sensorMiddle);
 int rightVal = analogRead(sensorRight)
 Serial.print("L: ");
 Serial.print(leftVal);
 Serial.print(" M: ");
 Serial.print(middleVal);
 Serial.print(" R: ");
 Serial.println(rightVal);
 // If any sensor detects the line, break the loop
 if (leftVal > THRESHOLD || middleVal > THRESHOLD || rightVal > THRESHOLD) {
 Serial.println("Line found!");
 break;
 }
 // Spin based on the last known direction
 if (lastDetectedDirection == 1) {
 // Spin left
 analogWrite(PWMA, BASE_SPEED);
 digitalWrite(AIN1, LOW);
 digitalWrite(BIN1, HIGH);
 analogWrite(PWMB, BASE_SPEED);
 digitalWrite(AIN1, HIGH);
 digitalWrite(BIN1, LOW);
 } else {
 // Spin right
 analogWrite(PWMA, BASE_SPEED);
 digitalWrite(AIN1, HIGH);
 digitalWrite(BIN1, LOW);
 analogWrite(PWMB, BASE_SPEED);
 digitalWrite(AIN1, LOW);
 digitalWrite(BIN1, HIGH);
 }
 delay(100); // Short delay to allow spinning
 } }
