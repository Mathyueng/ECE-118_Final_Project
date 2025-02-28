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

#ifndef PARALLEL_H    /* Guard against multiple inclusion */
#define PARALLEL_H


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

uint8_t Parallel_Init(void);

uint8_t Parallel_CheckEvents(void);

uint8_t ReadParallelSensors(void);

#endif
/* *****************************************************************************
 End of File
 */
