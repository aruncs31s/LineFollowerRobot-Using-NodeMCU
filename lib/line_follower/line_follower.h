#include <Arduino.h>

class LineFollower {
public:
    LineFollower(
        uint8_t in1,
         uint8_t in2,
          uint8_t in3,
          uint8_t in4,
          uint8_t ena,
          uint8_t enb,
          uint8_t irLeftPin,
          uint8_t irRightPin);
    void init();
    void moveForward();
    void moveBackward();
    void shouldITurnLeft();
    void shouldITurnRight();
    void turnLeft();
    void turnRight();
    void stop();
private:
    void steerStraight();
    uint8_t in1;
    uint8_t in2;
    uint8_t in3;
    uint8_t in4;
    uint8_t ena;
    uint8_t enb;
    uint8_t irLeftPin;
    uint8_t irRightPin;
};