#include <Arduino.h>

#include "s42b-2.0-pins.h" // s42b 2.0 specific details

#include "SimpleFOC.h"
#include "TLE5012b.h"

TLE5012B sensor = TLE5012B();

//  StepperDriver4PWM( int ph1A,int ph1B,int ph2A,int ph2B, int en1 (optional), int en2 (optional))
//  - ph1A, ph1B - phase 1 pwm pins
//  - ph2A, ph2B - phase 2 pwm pins
//  - en1, en2  - enable pins (optional input)
StepperDriver4PWM driver = StepperDriver4PWM(COIL_A_DIR_1, COIL_A_DIR_2, COIL_B_DIR_1, COIL_B_DIR_2, COIL_A_POWER_OUTPUT, COIL_B_POWER_OUTPUT);

// 200 steps/4 poles = 50 pole pairs
StepperMotor motor = StepperMotor(50, 2.2);

Commander commander = Commander(Serial);
void doMotor(char* cmd) { commander.motor(&motor, cmd); }
void onPid(char* cmd){commander.pid(&motor.PID_velocity, cmd);}
void onLpf(char* cmd){commander.lpf(&motor.LPF_velocity, cmd);}
void onTarget(char* cmd){commander.scalar(&motor.target, cmd);}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(115200);
  Serial.println("Init begin...");

  sensor.init();

  // power supply voltage [V]
  driver.pwm_frequency = 50000;
  driver.voltage_power_supply = 12;
  // Max DC voltage allowed - default voltage_power_supply
  // driver init
  driver.init();

  // init sensor
  // link the motor to the sensor
  motor.linkSensor(&sensor);

  // init driver
  // link the motor to the driver
  motor.linkDriver(&driver);

  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

  // set control loop type to be used
  motor.controller = MotionControlType::velocity;

  motor.useMonitoring(Serial);

  commander.add('M', doMotor, "motor");
  commander.add('C',onPid,"PID vel");
  commander.add('L',onLpf,"LPF vel");
  commander.add('T',onTarget,"target vel");
  motor.monitor_downsample = 0;

  motor.voltage_sensor_align = 9;
  motor.current_limit = 1800;

  // initialize motor
  motor.init();

  // align encoder and start FOC
  motor.initFOC();

  Serial.println("Done. RUNNING!");
  digitalWrite(LED_PIN, LOW);

}

void loop() {
    commander.run();
    // FOC algorithm function
    motor.loopFOC();

    // velocity control loop function
    motor.monitor();

    // setting the target velocity or 2rad/s
    motor.move(20);
}