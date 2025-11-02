#include "line_follower.h"
// The duration for turning maneuvers in milliseconds
#define TURN_DURATION_MS 100


LineFollower::LineFollower(
    uint8_t in1,
    uint8_t in2,
    uint8_t in3,
    uint8_t in4,
    uint8_t ena,
    uint8_t enb,
    uint8_t irLeftPin,
    uint8_t irRightPin) {
    this->in1 = in1;  // Rear motor forward
    this->in2 = in2;  // Rear motor backward
    this->ena = ena;  // Rear motor PWM
    this->in3 = in3;  // Front steering motor forward
    this->in4 = in4;  // Front steering motor backward
    this->irLeftPin = irLeftPin;
    this->irRightPin = irRightPin;
}

void LineFollower::init() {
    // Set motor pins as outputs
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(ena, OUTPUT);
    // Not using right now.
    if (enb) {
        pinMode(enb, OUTPUT);
    }

    // Set sensor pins as inputs
    pinMode(irLeftPin, INPUT);
    pinMode(irRightPin, INPUT);
}

void LineFollower::moveForward() {
    // Rear motor forward, steering straight
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(ena, 200);  // Adjust speed
    steerStraight();
}

void LineFollower::moveBackward() {
    // Rear motor backward, steering straight
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(ena, 200);
    steerStraight();
}
void LineFollower::turnLeft() {
    // Rear forward, steer left
    int64_t currentTime = millis();
    while (millis() - currentTime < TURN_DURATION_MS) {
        analogWrite(ena, 200);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        if (enb) {
            analogWrite(enb, 180);  // Steering speed
        }
    }
}

void LineFollower::turnRight() {
    int64_t currentTime = millis();
    int64_t duration = TURN_DURATION_MS;  // Duration to turn (in milliseconds)
    while (millis() - currentTime < duration) {
        analogWrite(ena, 200);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        if (enb) {
            analogWrite(enb, 180);  // Steering speed
        }
    }
}

void LineFollower::steerStraight() {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void LineFollower::shouldITurnLeft() {
    int leftSensorValue = analogRead(irLeftPin);
    int rightSensorValue = analogRead(irRightPin);
    if (leftSensorValue < 500 && rightSensorValue >= 500) {
        turnLeft();
    }
}

void LineFollower::shouldITurnRight() {
    int leftSensorValue = analogRead(irLeftPin);
    int rightSensorValue = analogRead(irRightPin);
    if (leftSensorValue >= 500 && rightSensorValue < 500) {
        turnRight();
    }
}

void LineFollower::stop() {
    // Stop rear motor
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(ena, 0);
    // Stop steering motor
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    if (enb) {
        analogWrite(enb, 0);
    }
}