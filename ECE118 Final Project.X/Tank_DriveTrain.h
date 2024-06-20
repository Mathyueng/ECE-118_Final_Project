/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#include <BOARD.h>

#ifndef TANK_DRIVE_H    /* Guard against multiple inclusion */
#define TANK_DRIVE_H

// Public Defines

// Speeds
#define REV_HI_SPEED    -100
#define REV_MID_SPEED   -60
#define REV_LOW_SPEED   -40
#define REV_CRAWL_SPEED -10
#define FULL_STOP       0
#define FWD_CRAWL_SPEED 10
#define FWD_LOW_SPEED   40
#define FWD_MID_SPEED   60
#define FWD_HI_SPEED    100

// Angles
#define NO_ANGLE        0
#define SMALL_ANGLE     30
#define MID_ANGLE       60
#define RIGHT_ANGLE     90
#define LARGE_ANGLE     120

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

// defined functions

/**
 * @Function DT_Init(void)
 * @param n/a
 * @return SUCCESS OR ERROR
 * @brief  Initializes the drivetrain: adds PWM pins and H-Bridge pins, sets drives to 0
 * @author Aarush Banerjee 5.19.2024 */
char DT_Init(void);

/**
 * @Function DT_SetRightDrive(int16_t speed)
 * @param speed - the linear speed to set the edge of the right wheel to, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief  sets the right wheel to the indicated speed, in milli-inches/second; negative values spin in reverse
 * @author Aarush Banerjee 5.19.2024 */
char DT_SetRightDrive(int16_t speed);

/**
 * @Function DT_SetLeftDrive(int16_t speed)
 * @param speed - the linear speed to set the edge of the left wheel to, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief  sets the left wheel to the indicated speed, in milli-inches/second; negative values spin in reverse
 * @author Aarush Banerjee 5.19.2024 */
char DT_SetLeftDrive(int16_t speed);

/**
 * @Function DT_GetRightDrive(void)
 * @param n/a
 * @return linear speed of the edge of the right wheel, in milli-inches/second; negative values indicate reverse 
 * @brief  wrapper function that gets the speed value most recently set to the right wheel
 * @author Aarush Banerjee 5.19.2024 */
int16_t DT_GetRightDrive(void);

/**
 * @Function DT_GetLeftDrive(void)
 * @param n/a
 * @return linear speed of the edge of the left wheel, in milli-inches/second; negative values indicate reverse 
 * @brief  wrapper function that gets the speed value most recently set to the left wheel
 * @author Aarush Banerjee 5.19.2024 */
int16_t DT_GetLeftDrive(void);

/**
 * @Function DT_Stop(void)
 * @param n/a
 * @return SUCCESS OR ERROR
 * @brief the robot stops moving
 * @author Aarush Banerjee 5.21.2024 */
char DT_Stop(void);

/**
 * @Function DT_DriveFwd(int16_t speed)
 * @param speed - the linear speed to drive the robot at, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief drives in the forward linear direction with speed specified in milli-inches/second; negative values drive backwards
 * @author Aarush Banerjee 5.19.2024 */
char DT_DriveFwd(int16_t speed);

/**
 * @Function DT_DriveRight(int16_t speed)
 * @param speed - the linear speed to drive the robot at, in +/- milli-inches/second
 * @param radius - the turning radius of the turn, measured by the radius between the right wheel and center of the turn arc, in milli-inches
 * @return SUCCESS OR ERROR
 * @brief drives forward and rightward with specified speed and turning radius; negative values drive backward and rightward
 * @author Aarush Banerjee 5.19.2024 */
char DT_DriveRight(int16_t speed, uint16_t radius);

/**
 * @Function DT_DriveLeft(int16_t speed)
 * @param speed - the linear speed to drive the robot at, in +/- milli-inches/second
 * @param radius - the turning radius of the turn, measured by the radius between the left wheel and center of the turn arc, in milli-inches
 * @return SUCCESS OR ERROR
 * @brief drives forward and leftward with specified speed and turning radius; negative values drive backward and leftward
 * @author Aarush Banerjee 5.19.2024 */
char DT_DriveLeft(int16_t speed, uint16_t radius);

/**
 * @Function DT_SpinCC(int16_t speed)
 * @param speed - the linear speed of the edge of each wheel, in +/- milli-inches/second
 * @return SUCCESS OR ERROR
 * @brief spins counter-clockwise at the specified speed; negative values spin clockwise
 * @author Aarush Banerjee 5.19.2024 */
char DT_SpinCC(int16_t speed);


char DT_ExtendArm(void);

char DT_RetractArm(void);

char DT_IntakeFwd(void);

char DT_IntakeBack(void);

char DT_IntakeStop(void);

#endif /* TANK_DRIVE_H */

/* *****************************************************************************
 End of File
 */
