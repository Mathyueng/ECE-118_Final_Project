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

#ifndef TAPE_H    /* Guard against multiple inclusion */
#define TAPE_H


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

#define FLTape 0b1000
#define FRTape 0b0100
#define BRTape 0b0010
#define BLTape 0b0001

uint8_t Tape_Init(void);

uint8_t Tape_CheckEvents(void);

uint8_t ReadTapeSensors(void);

#endif
/* *****************************************************************************
 End of File
 */
