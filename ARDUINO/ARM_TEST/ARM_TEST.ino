     // Arduino Pins        // 74HCT595N Pins

#define MOTORLATCH 12  // DIR_LATCH 12

#define MOTORCLK    4  // DIR_CLK   11

#define MOTORENABLE 7  // DIR_EN    13

#define MOTORDATA   8  // DIR_SER   14

 

#define MOTOR1_PWM 11 // PWM2A (M1)

#define MOTOR2_PWM 3  // PWM2B (M2)

#define MOTOR3_PWM 6  // PWM0A (M4)

#define MOTOR4_PWM 5  // PWM0B (M3)

#define SERVO1_PWM 10 // PWM1B (SER1)

#define SERVO2_PWM 9  // PWM1A (SERVO2)

 

// 74HCT595N Pins

#define MOTOR1_A 2

#define MOTOR1_B 3

#define MOTOR2_A 1

#define MOTOR2_B 4

#define MOTOR3_A 5

#define MOTOR3_B 7

#define MOTOR4_A 0

#define MOTOR4_B 6

 

#define FORWARD  1

#define BACKWARD 2

#define BRAKE    3

#define RELEASE  4



#define Pin2  2
#define Pin13 13
#define Pin9  9
#define Pin10 10

 

void setup() {

    Serial.begin(9600);

    pinMode(Pin2, INPUT);
    pinMode(Pin13, INPUT);
    pinMode(Pin9, INPUT);
    pinMode(Pin10, INPUT);

}


 

void loop() {

   char key = 0;
    int a,b,c,d;
    int fg;

    fg=0;
    
    if ( Serial.available() ) {

       key = Serial.read();
       fg=1;

    }

    // 善待您的馬達，180度反轉向時請讓馬達稍微休眠一下

    motor(1, RELEASE, 0);
    motor(2, RELEASE, 0);

    delay(100);

    a = 1;
    b = 1;
    c = 1;
    d = 1;

    a = digitalRead(Pin2);
    b = digitalRead(Pin13);
    c = digitalRead(Pin9);
    d = digitalRead(Pin10);


    if (fg==0){
      if (a==0 and b==0 and c==0 and d==1){
        key = 'A';
      }
      if (a==0 and b==0 and c==1 and d==0){
        key = 'B';
      }
      if (a==0 and b==0 and c==1 and d==1){
        key = 'D';
      }
      if (a==0 and b==1 and c==0 and d==0){
        key = 'U';
      }
    }

        
    //'A'A馬達正轉
    //'B'A馬達反轉
    //'D'蒐集器下
    //'U'蒐集器上
    //'w'停止不動


    delay(1000);

    switch ( key ) {

        case 'A': // A馬達正轉一圈

            motor(1, FORWARD, 250); 
            
            delay(3000);

            break;
            
        case 'B': // A馬達反轉半圈
 
            motor(1, BACKWARD, 250); 
            
            delay(1000);

            break;
            
        case 'D': // 蒐集器下

            motor(2, BACKWARD, 250); 
            
            delay(150);

            break;

        case 'U': // 蒐集器上

            motor(2, FORWARD, 250); 
            
            delay(150);

            break;
            
        case 'w': // 停止不動
            motor(1, RELEASE, 0);
            motor(2, RELEASE, 0);
            
            delay(1000);

            break;
            
            
        
        default:

            break;

    }

}



 

void motor(int nMotor, int command, int speed) {

    int motorA, motorB;

    if (nMotor >= 1 && nMotor <= 4) {

        switch (nMotor) {

            case 1:

                motorA = MOTOR1_A;

                motorB = MOTOR1_B;

                break;

            case 2:

                motorA = MOTOR2_A;

                motorB = MOTOR2_B;

                break;

            case 3:

                motorA = MOTOR3_A;

                motorB = MOTOR3_B;

                break;

            case 4:

                motorA = MOTOR4_A;

                motorB = MOTOR4_B;

                break;

            default:

                break;

        }

        switch (command) {

            case FORWARD:

                motor_output (motorA, HIGH, speed);

                motor_output (motorB, LOW, -1); // -1: no PWM set

                break;

            case BACKWARD:

                motor_output (motorA, LOW, speed);

                motor_output (motorB, HIGH, -1); // -1: no PWM set

                break;

            case BRAKE:

                motor_output (motorA, LOW, 255); // 255: fully on.

                motor_output (motorB, LOW, -1); // -1: no PWM set

                break;

            case RELEASE:

                motor_output (motorA, LOW, 0); // 0: output floating.

                motor_output (motorB, LOW, -1); // -1: no PWM set

                break;

            default:

                break;

        }

    }

}

 

void motor_output (int output, int high_low, int speed) {

    int motorPWM;

    switch (output) {

        case MOTOR1_A:

        case MOTOR1_B:

            motorPWM = MOTOR1_PWM;

            break;

        case MOTOR2_A:

        case MOTOR2_B:

            motorPWM = MOTOR2_PWM;

            break;

        case MOTOR3_A:

        case MOTOR3_B:

            motorPWM = MOTOR3_PWM;

            break;

        case MOTOR4_A:

        case MOTOR4_B:

            motorPWM = MOTOR4_PWM;

            break;

        default:

            speed = -3333;

            break;

    }

    if ( speed != -3333 ) {

        shiftWrite(output, high_low);

        // set PWM only if it is valid

        if (speed >= 0 && speed <= 255) {

            analogWrite(motorPWM, speed);

        }

    }

}

 

void shiftWrite(int output, int high_low) {

    static int latch_copy;

    static int shift_register_initialized = false;

    // Do the initialization on the fly, at the first time it is used.

    if (!shift_register_initialized) {

        // Set pins for shift register to output

        pinMode(MOTORLATCH, OUTPUT);

        pinMode(MOTORENABLE, OUTPUT);

        pinMode(MOTORDATA, OUTPUT);

        pinMode(MOTORCLK, OUTPUT);

        // Set pins for shift register to default value (low);

        digitalWrite(MOTORDATA, LOW);

        digitalWrite(MOTORLATCH, LOW);

        digitalWrite(MOTORCLK, LOW);

        // Enable the shift register, set Enable pin Low.

        digitalWrite(MOTORENABLE, LOW);

        // start with all outputs (of the shift register) low

        latch_copy = 0;

        shift_register_initialized = true;

    }

    // The defines HIGH and LOW are 1 and 0.

    // So this is valid.

    bitWrite(latch_copy, output, high_low);

    shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);

    delayMicroseconds(5); // For safety, not really needed.

    digitalWrite(MOTORLATCH, HIGH);

    delayMicroseconds(5); // For safety, not really needed.

    digitalWrite(MOTORLATCH, LOW);

}
