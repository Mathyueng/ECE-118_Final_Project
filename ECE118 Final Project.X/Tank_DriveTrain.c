/*
 * File:   Tank_DriveTrain.c
 * Author: aanbaner
 *
 * Created on May 19, 2024, 5:38 PM
 */

#include "xc.h"
#include "Tank_DriveTrain.h"
#include "BOARD.h"
#include "pwm.h"
#include "IO_Ports.h"
#include "AD.h"
#include "RC_Servo.h"
#include "math.h"

//#define DEBUG
//#define DRIVETRAIN_MAIN

#ifdef DEBUG
#include <stdio.h>
#endif

#define DRIVE_MULTIPLIER_R 15/16
#define DRIVE_MULTIPLIER_L 1
#define SPEED_TO_PWM 10
#define INTAKE_PWM 600

#define RIGHT_DRIVE_PIN PWM_PORTY10
#define LEFT_DRIVE_PIN PWM_PORTY12
#define INTAKE_DRIVE_PIN PWM_PORTX11
#define SERVO_ARM_PIN RC_PORTY06

#define H_BRIDGE_PORT PORTW
#define INTAKE_IN1 PIN3
#define INTAKE_IN2 PIN4
#define R_H_BRIDGE_IN1 PIN5
#define R_H_BRIDGE_IN2 PIN6
#define L_H_BRIDGE_IN1 PIN7
#define L_H_BRIDGE_IN2 PIN8

#define BATTERY_MAX 1024
#define WHEEL_DIST 11000

static int16_t right_speed;
static int16_t left_speed;

// private helper functions

uint8_t ReadHBridge() {
    return (IO_PortsReadPort(H_BRIDGE_PORT));
}

uint8_t GetRightPWM() {
    return PWM_GetDutyCycle(RIGHT_DRIVE_PIN);
}

uint8_t GetLeftPWM() {
    return PWM_GetDutyCycle(LEFT_DRIVE_PIN);
}

// public drivetrain library functions

/**
 * @Function DT_Init(void)
 * @param n/a
 * @return SUCCESS OR ERROR
 * @brief  Initializes the drivetrain: adds PWM pins and H-Bridge pins, sets drives to 0
 * @author Aarush Banerjee 5.19.2024 */
char DT_Init(void) {
    PWM_AddPins(RIGHT_DRIVE_PIN | LEFT_DRIVE_PIN | INTAKE_DRIVE_PIN);
    PWM_SetFrequency(MIN_PWM_FREQ);
    IO_PortsSetPortOutputs(H_BRIDGE_PORT, INTAKE_IN1 | INTAKE_IN2 | R_H_BRIDGE_IN1 | R_H_BRIDGE_IN2 | L_H_BRIDGE_IN1 | L_H_BRIDGE_IN2);
    IO_PortsClearPortBits(H_BRIDGE_PORT, 0xFFFF);
//    IO_PortsSetPortOutputs(H_BRIDGE_PORT, 0x01E0);
    RC_AddPins(SERVO_ARM_PIN);
    RC_SetPulseTime(SERVO_ARM_PIN, 1000);
#ifdef DEBUG
    printf("\r\nPort Initialization Successful...\r\n");
#endif
    DT_Stop();
    DT_IntakeStop();
    DT_RetractArm();
    return SUCCESS;
}

/**
 * @Function DT_SetRightDrive(int16_t speed)
 * @param speed - the linear speed to set the edge of the right wheel to, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief  sets the right wheel to the indicated speed, in milli-inches/second; negative values spin in reverse
 * @author Aarush Banerjee 5.19.2024 */
char DT_SetRightDrive(int16_t speed) {
    right_speed = speed;
    if (speed < 0) {
        IO_PortsWritePort(H_BRIDGE_PORT, ((IO_PortsReadPort(H_BRIDGE_PORT) | R_H_BRIDGE_IN2) & ~(R_H_BRIDGE_IN1)));
        //        IO_PortsWritePort(H_BRIDGE_PORT, PIN5 | PIN6);       // inserted for debugging purposes 
        speed *= -1;
    } else {
        IO_PortsWritePort(H_BRIDGE_PORT, ((IO_PortsReadPort(H_BRIDGE_PORT) | R_H_BRIDGE_IN1) & ~(R_H_BRIDGE_IN2)));
        //        IO_PortsWritePort(H_BRIDGE_PORT, PIN5 | PIN6);       // inserted for debugging purposes
    }
    if (speed * SPEED_TO_PWM > MAX_PWM) return ERROR;
#ifdef DEBUG
    printf("\r\nDuty cycle for right motor: %d", speed * SPEED_TO_PWM * DRIVE_MULTIPLIER_R);
#endif
    PWM_SetDutyCycle(RIGHT_DRIVE_PIN, (speed * SPEED_TO_PWM * DRIVE_MULTIPLIER_R)); // bitshifting done to prevent float division or integer imprecision
#ifdef DEBUG
    printf("\r\nRight Speed: %d...\r\n", DT_GetRightDrive());
#endif
    return SUCCESS;
}

/** 
 * @Function DT_SetLeftDrive(int16_t speed)
 * @param speed - the linear speed to set the edge of the left wheel to, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief  sets the left wheel to the indicated speed, in milli-inches/second; negative values spin in reverse
 * @author Aarush Banerjee 5.19.2024 */
char DT_SetLeftDrive(int16_t speed) {
    left_speed = speed;
    if (speed < 0) {
        IO_PortsWritePort(H_BRIDGE_PORT, (IO_PortsReadPort(H_BRIDGE_PORT) | L_H_BRIDGE_IN2) & ~(L_H_BRIDGE_IN1));
        //        IO_PortsWritePort(H_BRIDGE_PORT, PIN7 | PIN8);       // inserted for debugging purposes 
        speed *= -1;
    } else {
        IO_PortsWritePort(H_BRIDGE_PORT, (IO_PortsReadPort(H_BRIDGE_PORT) | L_H_BRIDGE_IN1) & ~(L_H_BRIDGE_IN2));
        //        IO_PortsWritePort(H_BRIDGE_PORT, PIN7 | PIN8);       // inserted for debugging purposes
    }
    if (speed * SPEED_TO_PWM > MAX_PWM) return ERROR;
#ifdef DEBUG
    printf("\r\nDuty cycle for left motor: %d", speed * SPEED_TO_PWM * DRIVE_MULTIPLIER_L);
#endif
    PWM_SetDutyCycle(LEFT_DRIVE_PIN, (speed * SPEED_TO_PWM * DRIVE_MULTIPLIER_L)); // bitshifting done to prevent float division or integer imprecision
#ifdef DEBUG
    printf("\r\nLeft Speed: %d...\r\n", DT_GetLeftDrive());
#endif
    return SUCCESS;
}

/**
 * @Function DT_GetRightDrive(void)
 * @param n/a
 * @return linear speed of the edge of the right wheel, in milli-inches/second; negative values indicate reverse 
 * @brief  wrapper function that gets the speed value most recently set to the right wheel
 * @author Aarush Banerjee 5.19.2024 */
int16_t DT_GetRightDrive(void) {
    return right_speed;
}

/**
 * @Function DT_GetLeftDrive(void)
 * @param n/a
 * @return linear speed of the edge of the left wheel, in milli-inches/second; negative values indicate reverse 
 * @brief  wrapper function that gets the speed value most recently set to the left wheel
 * @author Aarush Banerjee 5.19.2024 */
int16_t DT_GetLeftDrive(void) {
    return left_speed;
}

/**
 * @Function DT_Stop(void)
 * @param n/a
 * @return SUCCESS OR ERROR
 * @brief the robot stops moving
 * @author Aarush Banerjee 5.21.2024 */
char DT_Stop(void) {
    return (DT_DriveFwd(0));
}

/**
 * @Function DT_DriveFwd(int16_t speed)
 * @param speed - the linear speed to drive the robot at, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief drives in the forward linear direction with speed specified in milli-inches/second; negative values drive backwards
 * @author Aarush Banerjee 5.19.2024 */
char DT_DriveFwd(int16_t speed) {
    return (DT_SetRightDrive(speed) && DT_SetLeftDrive(speed));
}

/**
 * @Function DT_DriveRight(int16_t speed)
 * @param speed - the linear speed to drive the robot at, in +/- milli-inches/second
 * @param radius - the turning radius of the turn, measured by the radius between the right wheel and center of the turn arc, in milli-inches
 * @return SUCCESS OR ERROR
 * @brief drives forward and rightward with specified speed and turning radius; negative values drive backward and rightward
 * @author Aarush Banerjee 5.19.2024 */
char DT_DriveRight(int16_t speed, uint16_t radius) {
    int v1 = (int) (speed * sqrt(((double) radius) / ((double) (radius + (WHEEL_DIST >> 1)))));
    int v2 = (int) (speed * sqrt(((double) (radius + WHEEL_DIST)) / ((double) radius + (WHEEL_DIST >> 1))));
    return (DT_SetLeftDrive(v2) && DT_SetRightDrive(v1));
}

/**
 * @Function DT_DriveLeft(int16_t speed)
 * @param speed - the linear speed to drive the robot at, in +/- milli-inches/second
 * @param radius - the turning radius of the turn, measured by the radius between the left wheel and center of the turn arc, in milli-inches
 * @return SUCCESS OR ERROR
 * @brief drives forward and leftward with specified speed and turning radius; negative values drive backward and leftward
 * @author Aarush Banerjee 5.19.2024 */
char DT_DriveLeft(int16_t speed, uint16_t radius) {
    int v1 = (int) (speed * sqrt(((double) radius) / ((double) (radius + (WHEEL_DIST >> 1)))));
    int v2 = (int) (speed * sqrt(((double) (radius + WHEEL_DIST)) / ((double) radius + (WHEEL_DIST >> 1))));
    return (DT_SetRightDrive(v2) && DT_SetLeftDrive(v1));
}

/**
 * @Function DT_SpinCC(int16_t speed)
 * @param speed - the linear speed of the edge of each wheel, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief spins counter-clockwise at the specified speed; negative values spin clockwise
 * @author Aarush Banerjee 5.19.2024 */
char DT_SpinCC(int16_t speed) {
    return (DT_SetRightDrive(speed) && DT_SetLeftDrive(-speed));
}

char DT_ExtendArm(void) {
    RC_SetPulseTime(SERVO_ARM_PIN, 2000);
}

char DT_RetractArm(void) {
    RC_SetPulseTime(SERVO_ARM_PIN, 1000);
}

char DT_IntakeFwd(void) {
    PWM_SetDutyCycle(INTAKE_DRIVE_PIN, INTAKE_PWM);
    IO_PortsWritePort(H_BRIDGE_PORT, ((IO_PortsReadPort(H_BRIDGE_PORT) | INTAKE_IN1) & ~(INTAKE_IN2)));
}

char DT_IntakeBack(void) {
    PWM_SetDutyCycle(INTAKE_DRIVE_PIN, INTAKE_PWM);
    IO_PortsWritePort(H_BRIDGE_PORT, ((IO_PortsReadPort(H_BRIDGE_PORT) | INTAKE_IN2) & ~(INTAKE_IN1)));
}

char DT_IntakeStop(void) {
    PWM_SetDutyCycle(INTAKE_DRIVE_PIN, 0);
    IO_PortsWritePort(H_BRIDGE_PORT, ((IO_PortsReadPort(H_BRIDGE_PORT) & ~(INTAKE_IN1) & ~(INTAKE_IN2))));
}


#ifdef DRIVETRAIN_MAIN
#define DELAY(x) for (int i=0;i<(400000*x);i++) asm("nop");
#include <stdio.h>

void main(void) {
    printf("\r\nDrivetrain.c test harness successfully compiled");
    BOARD_Init();
    PWM_Init();
    RC_Init();
    DT_Init();
    printf("\r\nDrivetrain successfully initalized");

    while (1) {
        int speed = 60;
        int turningRad = 8000;
        int delay = 5;
        
        DT_DriveFwd(speed);
        DT_IntakeFwd();
        DT_ExtendArm();
        DELAY(delay);
        
        DT_DriveFwd(-speed);
        DT_RetractArm();
        DELAY(delay);
        
        DT_DriveRight(speed, turningRad);
        DT_ExtendArm();
        DELAY(delay);
        
        DT_DriveRight(-speed, turningRad);
        DT_RetractArm();
        DELAY(delay);
        
        DT_IntakeBack();
        DT_DriveLeft(speed, turningRad);
        DT_ExtendArm();
        DELAY(delay);
        
        DT_DriveLeft(-speed, turningRad);
        DT_RetractArm();
        DELAY(delay);
        
        DT_SpinCC(speed);
        DELAY(delay);
        
        DT_SpinCC(-speed);
        DELAY(delay);
        
#ifdef DEBUG
        printf("\r\n");
        printf("\r\nH_Bridge Port: %x", ReadHBridge());
        printf("\r\nRight Duty Cycle: %d", GetRightPWM());
        printf("\r\nLeft Duty Cycle: %d", GetLeftPWM());
#endif
    }
}
#endif // DRIVETRAIN_TEST
