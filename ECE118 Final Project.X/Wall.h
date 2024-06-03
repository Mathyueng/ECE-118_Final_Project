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

uint8_t Wall_Init(void);

uint8_t Wall_CheckEvents(void);

#endif
/* *****************************************************************************
 End of File
 */
