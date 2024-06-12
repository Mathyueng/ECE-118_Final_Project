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

uint8_t Obstacle_Init(void);

uint8_t Obstacle_CheckEvents(void);

uint8_t ReadObstacleSensors(void);

#endif
/* *****************************************************************************
 End of File
 */
