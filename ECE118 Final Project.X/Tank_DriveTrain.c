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
#include "math.h"

//#define DRIVETRAIN_TEST

#define DRIVE_MULTIPLIER_R 1
#define DRIVE_MULTIPLIER_L 1
#define SPEED_TO_PWM 10

#define RIGHT_DRIVE_PIN PWM_PORTY10
#define LEFT_DRIVE_PIN PWM_PORTY12

#define H_BRIDGE_PORT PORTX
#define R_H_BRIDGE_IN1 PIN3
#define R_H_BRIDGE_IN2 PIN4
#define L_H_BRIDGE_IN1 PIN5
#define L_H_BRIDGE_IN2 PIN6

#define BATTERY_MAX 1024
#define WHEEL_DIST 11000

static int16_t right_speed;
static int16_t left_speed;

/**
 * @Function DT_Init(void)
 * @param n/a
 * @return SUCCESS OR ERROR
 * @brief  Initializes the drivetrain: adds PWM pins and H-Bridge pins, sets drives to 0
 * @author Aarush Banerjee 5.19.2024 */
char DT_Init(void) {
    PWM_AddPins(RIGHT_DRIVE_PIN | LEFT_DRIVE_PIN);
    IO_PortsSetPortOutputs(H_BRIDGE_PORT, R_H_BRIDGE_IN1 | R_H_BRIDGE_IN2 | L_H_BRIDGE_IN1 | L_H_BRIDGE_IN2);
    IO_PortsWritePort(H_BRIDGE_PORT, 0);
    DT_SetRightDrive(0);
    DT_SetLeftDrive(0);
    return SUCCESS;
}

/**
 * @Function DT_SetRightDrive(int16_t speed)
 * @param speed - the linear speed to set the edge of the right wheel to, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief  sets the right wheel to the indicated speed, in milli-inches/second; negative values spin in reverse
 * @author Aarush Banerjee 5.19.2024 */
char DT_SetRightDrive(int16_t speed) {
    if (speed < 0) {
        IO_PortsWritePort(H_BRIDGE_PORT, IO_PortsReadPort(H_BRIDGE_PORT) | R_H_BRIDGE_IN2 & !(R_H_BRIDGE_IN1));
        speed *= -1;
    } else {
        IO_PortsWritePort(H_BRIDGE_PORT, IO_PortsReadPort(H_BRIDGE_PORT) | R_H_BRIDGE_IN1 & !(R_H_BRIDGE_IN2));
    }
    if (speed * SPEED_TO_PWM > 1000) return ERROR;
    unsigned int pwmDutyCycle = (speed * SPEED_TO_PWM * DRIVE_MULTIPLIER_R * (BATTERY_MAX << 4) / AD_ReadADPin(BAT_VOLTAGE)) >> 4; // bitshifting done to prevent float division or integer imprecision
    PWM_SetDutyCycle(RIGHT_DRIVE_PIN, pwmDutyCycle);
    right_speed = speed;
    return SUCCESS;
}

/**
 * @Function DT_SetLeftDrive(int16_t speed)
 * @param speed - the linear speed to set the edge of the left wheel to, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief  sets the left wheel to the indicated speed, in milli-inches/second; negative values spin in reverse
 * @author Aarush Banerjee 5.19.2024 */
char DT_SetLeftDrive(int16_t speed) {
    if (speed < 0) {
        IO_PortsWritePort(H_BRIDGE_PORT, IO_PortsReadPort(H_BRIDGE_PORT) | L_H_BRIDGE_IN2 & !(L_H_BRIDGE_IN1));
        speed *= -1;
    } else {
        IO_PortsWritePort(H_BRIDGE_PORT, IO_PortsReadPort(H_BRIDGE_PORT) | L_H_BRIDGE_IN1 & !(L_H_BRIDGE_IN2));
    }
    unsigned int pwmDutyCycle = (speed * SPEED_TO_PWM * DRIVE_MULTIPLIER_R * (BATTERY_MAX << 4) / AD_ReadADPin(BAT_VOLTAGE)) >> 4;
    PWM_SetDutyCycle(RIGHT_DRIVE_PIN, pwmDutyCycle);
    left_speed = speed;
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
    int v1 = (int) (speed * sqrt(((double) radius)/((double) (radius + (WHEEL_DIST >> 1)))));
    int v2 = (int) (speed * sqrt(((double) (radius + WHEEL_DIST))/((double) radius + (WHEEL_DIST >> 1))));
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
    int v1 = (int) (speed * sqrt(((double) radius)/((double) (radius + (WHEEL_DIST >> 1)))));
    int v2 = (int) (speed * sqrt(((double) (radius + WHEEL_DIST))/((double) radius + (WHEEL_DIST >> 1))));
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

#ifdef DRIVETRAIN_TEST
#define DELAY(x) for (int i = 0; i < x; i++) asm("nop");
void main(void) {
    DT_Init();
    while (1) {
        DT_DriveFwd(1000);
        // remainder of the test harness
    }
}
#endif // DRIVETRAIN_TEST