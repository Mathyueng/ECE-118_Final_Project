/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    Obstacle.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef OBSTACLE_H    /* Guard against multiple inclusion */
#define OBSTACLE_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include "ES_Configure.h"
#include "BOARD.h"

#define LLObstacle 0b100000
#define FLObstacle 0b010000
#define CLObstacle 0b001000
#define CRObstacle 0b000100
#define FRObstacle 0b000010
#define RRObstacle 0b000001

uint8_t Obstacle_Init(void);

uint8_t Obstacle_CheckEvents(void);

uint8_t ReadObstacleSensors(void);

#endif
/* *****************************************************************************
 End of File
 */
