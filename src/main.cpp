#include <Arduino.h>

#include "config.h" // s42b 2.0 specific details

#include "SimpleFOC.h"
#include "TLE5012b.h"

TLE5012B sensor = TLE5012B();

StepperDriver2PWM driver = StepperDriver2PWM(COIL_A_PWM, COIL_A_DIR_1, COIL_A_DIR_2, COIL_B_PWM, COIL_B_DIR_1, COIL_B_DIR_2, NOT_SET, NOT_SET);

// 200 steps/4 poles = 50 pole pairs
StepperMotor motor = StepperMotor(50, 2.2);

// Commander interface constructor
Commander commander = Commander(Serial);
void doMotor(char* cmd) { commander.motor(&motor, cmd); }
void onPid(char* cmd){commander.pid(&motor.PID_velocity, cmd);}
void onLpf(char* cmd){commander.lpf(&motor.LPF_velocity, cmd);}
void onTarget(char* cmd){commander.scalar(&motor.target, cmd);}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.setTx(USART1_TX);
  Serial.setRx(USART1_RX);
  Serial.begin(115200);
  Serial.println("Init begin...");

  sensor.init();

  // power supply voltage [V]
  // driver.pwm_frequency = 50000;
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
  motor.controller = MotionControlType::angle;
  motor.torque_controller = TorqueControlType::foc_current;

  motor.useMonitoring(Serial);

  commander.add('M', doMotor, "motor");
  commander.add('C',onPid,"PID vel");
  commander.add('L',onLpf,"LPF vel");
  commander.add('T',onTarget,"target vel");
  // motor.monitor_downsample = 0;

  motor.voltage_sensor_align = 9;
  motor.current_limit = 180;
  motor.velocity_limit = 10;

  // initialize motor
  motor.init();

  // align encoder and start FOC
  motor.initFOC();

  Serial.println("Done. RUNNING!");
  digitalWrite(LED_PIN, LOW);

}

bool dir = true;

void loop() {
    commander.run();

    // FOC algorithm function
    motor.loopFOC();

    // velocity control loop function
    motor.monitor();

    if (dir) {
        motor.move(motor.shaft_angle_sp + 1);
        if (motor.shaft_angle_sp > 360)
            dir = !dir;
    } else {
        motor.move(motor.shaft_angle_sp - 1);
        if (motor.shaft_angle_sp < 0)
            dir = !dir;
    }
}
