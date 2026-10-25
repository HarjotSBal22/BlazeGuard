#include <Servo.h>

// ================= MOTOR PINS =================
#define IN1 10
#define IN2 9
#define IN3 8
#define IN4 7

#define ENA 11
#define ENB 6

// ================= FLAME SENSORS =================
#define FLAME_LEFT   A0
#define FLAME_CENTER A1
#define FLAME_RIGHT  A2

// ================= SERVO & RELAY =================
#define SERVO_PIN 5
#define RELAY_PIN 4

// ================= SETTINGS =================

// Adjust these after testing sensors
#define FIRE_DETECT 900
#define FIRE_STOP   750

int speedLeft = 180;
int speedRight = 180;

int servoLeft = 50;
int servoCenter = 90;
int servoRight = 130;

// Most relay modules are ACTIVE LOW
#define RELAY_ON LOW
#define RELAY_OFF HIGH

Servo fireServo;

// =================================================
// SETUP
// =================================================

void setup()
{
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  fireServo.attach(SERVO_PIN);
  fireServo.write(servoCenter);

  stopCar();

  Serial.println("Fire Fighter Ready");
  delay(2000);
}

// =================================================
// MAIN LOOP
// =================================================

void loop()
{
  int leftValue = analogRead(FLAME_LEFT);
  int centerValue = analogRead(FLAME_CENTER);
  int rightValue = analogRead(FLAME_RIGHT);

  Serial.print("L=");
  Serial.print(leftValue);

  Serial.print("  C=");
  Serial.print(centerValue);

  Serial.print("  R=");
  Serial.println(rightValue);

  int minValue = min(leftValue, min(centerValue, rightValue));

  // ================= NO FIRE =================

  if (leftValue > FIRE_DETECT &&
      centerValue > FIRE_DETECT &&
      rightValue > FIRE_DETECT)
  {
    stopCar();
    pumpOff();
    Serial.println("NO FIRE");
    delay(100);
    return;
  }

  // ================ FIRE CLOSE ================

  if (minValue <= FIRE_STOP)
  {
    Serial.println("FIRE CLOSE");

    stopCar();

    sprayWater();

    delay(300);

    return;
  }

  // =============== FIND DIRECTION =============

  if (leftValue == minValue)
  {
    Serial.println("TURN LEFT");

    turnLeft();

    delay(200);

    stopCar();

    return;
  }

  if (rightValue == minValue)
  {
    Serial.println("TURN RIGHT");

    turnRight();

    delay(200);

    stopCar();

    return;
  }

  Serial.println("MOVE FORWARD");

  forward();

  delay(200);

  stopCar();

  delay(50);
}

// =================================================
// MOTOR FUNCTIONS
// =================================================

void forward()
{
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward()
{
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft()
{
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight()
{
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopCar()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// =================================================
// RELAY / PUMP
// =================================================

void pumpOn()
{
  digitalWrite(RELAY_PIN, RELAY_ON);
  Serial.println("PUMP ON");
}

void pumpOff()
{
  digitalWrite(RELAY_PIN, RELAY_OFF);
  Serial.println("PUMP OFF");
}

// =================================================
// SPRAY WATER
// =================================================

void sprayWater()
{
  Serial.println("START SPRAY");

  stopCar();

  pumpOn();

  for (int i = 0; i < 5; i++)
  {
    fireServo.write(servoLeft);
    delay(300);

    fireServo.write(servoRight);
    delay(300);
  }

  fireServo.write(servoCenter);

  delay(500);

  pumpOff();

  Serial.println("SPRAY COMPLETE");
}
