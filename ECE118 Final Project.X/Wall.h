/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    Wall.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef WALL_H    /* Guard against multiple inclusion */
#define WALL_H


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

#define LLWall 0b1000
#define FLWall 0b0100
#define FRWall 0b0010
#define RRWall 0b0001

uint8_t Wall_Init(void);

uint8_t Wall_CheckEvents(void);

uint8_t ReadWallSensors(void);

#endif
/* *****************************************************************************
 End of File
 */
