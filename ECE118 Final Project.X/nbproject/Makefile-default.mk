#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ECE118_Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ECE118_Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=AD.c BOARD.c ES_CheckEvents.c ES_Framework.c ES_KeyboardInput.c ES_PostList.c ES_Queue.c ES_TattleTale.c ES_Timers.c EventChecker.c IO_Ports.c LED.c RC_Servo.c Stepper.c Tank_DriveTrain.c TapeService.c TemplateFSM.c TemplateHSM.c TemplateService.c TemplateSubHSM.c ping.c pwm.c serial.c timers.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/AD.o ${OBJECTDIR}/BOARD.o ${OBJECTDIR}/ES_CheckEvents.o ${OBJECTDIR}/ES_Framework.o ${OBJECTDIR}/ES_KeyboardInput.o ${OBJECTDIR}/ES_PostList.o ${OBJECTDIR}/ES_Queue.o ${OBJECTDIR}/ES_TattleTale.o ${OBJECTDIR}/ES_Timers.o ${OBJECTDIR}/EventChecker.o ${OBJECTDIR}/IO_Ports.o ${OBJECTDIR}/LED.o ${OBJECTDIR}/RC_Servo.o ${OBJECTDIR}/Stepper.o ${OBJECTDIR}/Tank_DriveTrain.o ${OBJECTDIR}/TapeService.o ${OBJECTDIR}/TemplateFSM.o ${OBJECTDIR}/TemplateHSM.o ${OBJECTDIR}/TemplateService.o ${OBJECTDIR}/TemplateSubHSM.o ${OBJECTDIR}/ping.o ${OBJECTDIR}/pwm.o ${OBJECTDIR}/serial.o ${OBJECTDIR}/timers.o
POSSIBLE_DEPFILES=${OBJECTDIR}/AD.o.d ${OBJECTDIR}/BOARD.o.d ${OBJECTDIR}/ES_CheckEvents.o.d ${OBJECTDIR}/ES_Framework.o.d ${OBJECTDIR}/ES_KeyboardInput.o.d ${OBJECTDIR}/ES_PostList.o.d ${OBJECTDIR}/ES_Queue.o.d ${OBJECTDIR}/ES_TattleTale.o.d ${OBJECTDIR}/ES_Timers.o.d ${OBJECTDIR}/EventChecker.o.d ${OBJECTDIR}/IO_Ports.o.d ${OBJECTDIR}/LED.o.d ${OBJECTDIR}/RC_Servo.o.d ${OBJECTDIR}/Stepper.o.d ${OBJECTDIR}/Tank_DriveTrain.o.d ${OBJECTDIR}/TapeService.o.d ${OBJECTDIR}/TemplateFSM.o.d ${OBJECTDIR}/TemplateHSM.o.d ${OBJECTDIR}/TemplateService.o.d ${OBJECTDIR}/TemplateSubHSM.o.d ${OBJECTDIR}/ping.o.d ${OBJECTDIR}/pwm.o.d ${OBJECTDIR}/serial.o.d ${OBJECTDIR}/timers.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/AD.o ${OBJECTDIR}/BOARD.o ${OBJECTDIR}/ES_CheckEvents.o ${OBJECTDIR}/ES_Framework.o ${OBJECTDIR}/ES_KeyboardInput.o ${OBJECTDIR}/ES_PostList.o ${OBJECTDIR}/ES_Queue.o ${OBJECTDIR}/ES_TattleTale.o ${OBJECTDIR}/ES_Timers.o ${OBJECTDIR}/EventChecker.o ${OBJECTDIR}/IO_Ports.o ${OBJECTDIR}/LED.o ${OBJECTDIR}/RC_Servo.o ${OBJECTDIR}/Stepper.o ${OBJECTDIR}/Tank_DriveTrain.o ${OBJECTDIR}/TapeService.o ${OBJECTDIR}/TemplateFSM.o ${OBJECTDIR}/TemplateHSM.o ${OBJECTDIR}/TemplateService.o ${OBJECTDIR}/TemplateSubHSM.o ${OBJECTDIR}/ping.o ${OBJECTDIR}/pwm.o ${OBJECTDIR}/serial.o ${OBJECTDIR}/timers.o

# Source Files
SOURCEFILES=AD.c BOARD.c ES_CheckEvents.c ES_Framework.c ES_KeyboardInput.c ES_PostList.c ES_Queue.c ES_TattleTale.c ES_Timers.c EventChecker.c IO_Ports.c LED.c RC_Servo.c Stepper.c Tank_DriveTrain.c TapeService.c TemplateFSM.c TemplateHSM.c TemplateService.c TemplateSubHSM.c ping.c pwm.c serial.c timers.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/ECE118_Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=,--script="bootloader320.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/AD.o: AD.c  .generated_files/flags/default/d89af279e107a80862e0e4ba7b524d2c58e035bf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AD.o.d 
	@${RM} ${OBJECTDIR}/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/AD.o.d" -o ${OBJECTDIR}/AD.o AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BOARD.o: BOARD.c  .generated_files/flags/default/a7666e217e1825bf7a3a9676368b3a5b89b0a301 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BOARD.o.d 
	@${RM} ${OBJECTDIR}/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/BOARD.o.d" -o ${OBJECTDIR}/BOARD.o BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_CheckEvents.o: ES_CheckEvents.c  .generated_files/flags/default/bf2b4173bab061bb7e7a193044976b938f518be4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_CheckEvents.o.d" -o ${OBJECTDIR}/ES_CheckEvents.o ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_Framework.o: ES_Framework.c  .generated_files/flags/default/f5711314a53ec0281960ed12fec2106d23b070b5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_Framework.o.d" -o ${OBJECTDIR}/ES_Framework.o ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_KeyboardInput.o: ES_KeyboardInput.c  .generated_files/flags/default/3eb169527e18839e0879bb0b404cfe88359854e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/ES_KeyboardInput.o ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_PostList.o: ES_PostList.c  .generated_files/flags/default/b1ded87c7d5103f51d19c646ddadc42923bb2e0e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_PostList.o.d" -o ${OBJECTDIR}/ES_PostList.o ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_Queue.o: ES_Queue.c  .generated_files/flags/default/2f8178b9e9d6768a9ef1a2de7386776cd71f980a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_Queue.o.d" -o ${OBJECTDIR}/ES_Queue.o ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_TattleTale.o: ES_TattleTale.c  .generated_files/flags/default/f9b7f1925fc870b400b4a1218cd74e2e58a6068b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_TattleTale.o.d" -o ${OBJECTDIR}/ES_TattleTale.o ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_Timers.o: ES_Timers.c  .generated_files/flags/default/ccb4afcb4b92cb81be523a47b7ed86141afaa953 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_Timers.o.d" -o ${OBJECTDIR}/ES_Timers.o ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/EventChecker.o: EventChecker.c  .generated_files/flags/default/e7a9450d35bdb86ed6ad5ad9736f7f3298641d7a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/EventChecker.o.d 
	@${RM} ${OBJECTDIR}/EventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/EventChecker.o.d" -o ${OBJECTDIR}/EventChecker.o EventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/IO_Ports.o: IO_Ports.c  .generated_files/flags/default/208ca6c13ccb1d92a6d51f707d457972cd695b56 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/IO_Ports.o.d" -o ${OBJECTDIR}/IO_Ports.o IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LED.o: LED.c  .generated_files/flags/default/3b760dc15cfa593ec697246958d0800073f3562 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LED.o.d 
	@${RM} ${OBJECTDIR}/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/LED.o.d" -o ${OBJECTDIR}/LED.o LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/RC_Servo.o: RC_Servo.c  .generated_files/flags/default/e4f4616c7f948fa4f5091d67b82d41a8d54fa448 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/RC_Servo.o.d" -o ${OBJECTDIR}/RC_Servo.o RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Stepper.o: Stepper.c  .generated_files/flags/default/c4369ae7defa6fe01ffcbf99305918b0be996001 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Stepper.o.d 
	@${RM} ${OBJECTDIR}/Stepper.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/Stepper.o.d" -o ${OBJECTDIR}/Stepper.o Stepper.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Tank_DriveTrain.o: Tank_DriveTrain.c  .generated_files/flags/default/6727041e46bc7ed72f4936ce1b7956c8cf1d937 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Tank_DriveTrain.o.d 
	@${RM} ${OBJECTDIR}/Tank_DriveTrain.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/Tank_DriveTrain.o.d" -o ${OBJECTDIR}/Tank_DriveTrain.o Tank_DriveTrain.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TapeService.o: TapeService.c  .generated_files/flags/default/1225934bc737dc68c00ca80103fba20eee3bd4b6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TapeService.o.d 
	@${RM} ${OBJECTDIR}/TapeService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TapeService.o.d" -o ${OBJECTDIR}/TapeService.o TapeService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateFSM.o: TemplateFSM.c  .generated_files/flags/default/3e8f504a2b0d5d1041015ede163a19a6f7ce5655 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateFSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateFSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TemplateFSM.o.d" -o ${OBJECTDIR}/TemplateFSM.o TemplateFSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateHSM.o: TemplateHSM.c  .generated_files/flags/default/4887f1c371eb6df755f7b90ed6c4667013aeb6d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TemplateHSM.o.d" -o ${OBJECTDIR}/TemplateHSM.o TemplateHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateService.o: TemplateService.c  .generated_files/flags/default/b6386fc1f2f0c894f49838d4633e22c5a4b7dadc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateService.o.d 
	@${RM} ${OBJECTDIR}/TemplateService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TemplateService.o.d" -o ${OBJECTDIR}/TemplateService.o TemplateService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateSubHSM.o: TemplateSubHSM.c  .generated_files/flags/default/8785cd14cb2c9e20f7a690f3c2fee3375ddeb898 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TemplateSubHSM.o.d" -o ${OBJECTDIR}/TemplateSubHSM.o TemplateSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ping.o: ping.c  .generated_files/flags/default/7adf294a7dd2a344be67c320528cdd486bade48b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ping.o.d 
	@${RM} ${OBJECTDIR}/ping.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ping.o.d" -o ${OBJECTDIR}/ping.o ping.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/pwm.o: pwm.c  .generated_files/flags/default/6e3c73c69e595551d570f717cea4d55f2a4da35a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm.o.d 
	@${RM} ${OBJECTDIR}/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/pwm.o.d" -o ${OBJECTDIR}/pwm.o pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/serial.o: serial.c  .generated_files/flags/default/4e5d5da9c49b30dff5640b0c48a0d5197f3d2deb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/serial.o.d 
	@${RM} ${OBJECTDIR}/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/serial.o.d" -o ${OBJECTDIR}/serial.o serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/timers.o: timers.c  .generated_files/flags/default/331ae0c08a0a5cc172a81efaadb628fb4c291106 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timers.o.d 
	@${RM} ${OBJECTDIR}/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/timers.o.d" -o ${OBJECTDIR}/timers.o timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/AD.o: AD.c  .generated_files/flags/default/c20ac6d95408f0706f32f44e037e16fcbe5a817b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AD.o.d 
	@${RM} ${OBJECTDIR}/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/AD.o.d" -o ${OBJECTDIR}/AD.o AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/BOARD.o: BOARD.c  .generated_files/flags/default/32645c1cbebb5617a7ac31375b2abd85041a626d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BOARD.o.d 
	@${RM} ${OBJECTDIR}/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/BOARD.o.d" -o ${OBJECTDIR}/BOARD.o BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_CheckEvents.o: ES_CheckEvents.c  .generated_files/flags/default/b09fa8ef69552cc96a55b64b5f6f117afa7b9df9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_CheckEvents.o.d" -o ${OBJECTDIR}/ES_CheckEvents.o ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_Framework.o: ES_Framework.c  .generated_files/flags/default/d46059c763728bba01c401f45b32cee2dfa8dcd8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_Framework.o.d" -o ${OBJECTDIR}/ES_Framework.o ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_KeyboardInput.o: ES_KeyboardInput.c  .generated_files/flags/default/9b4254e4b3b602b0f5f5702a85343464f86ae29d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/ES_KeyboardInput.o ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_PostList.o: ES_PostList.c  .generated_files/flags/default/216985569d6049b00b020a3ca3b713b799205490 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_PostList.o.d" -o ${OBJECTDIR}/ES_PostList.o ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_Queue.o: ES_Queue.c  .generated_files/flags/default/34abd926b66254ad4cdd7f5960e4438bd3f30a40 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_Queue.o.d" -o ${OBJECTDIR}/ES_Queue.o ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_TattleTale.o: ES_TattleTale.c  .generated_files/flags/default/82db12530dcfd8b917a89eb5f6ca9a80ba338a24 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_TattleTale.o.d" -o ${OBJECTDIR}/ES_TattleTale.o ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ES_Timers.o: ES_Timers.c  .generated_files/flags/default/267bf1d5c9fc1481b6c8261b3a3d0ae5b2fcd2e3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ES_Timers.o.d" -o ${OBJECTDIR}/ES_Timers.o ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/EventChecker.o: EventChecker.c  .generated_files/flags/default/1f1eb3f2b168961b3ee6373eef876101feba61b7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/EventChecker.o.d 
	@${RM} ${OBJECTDIR}/EventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/EventChecker.o.d" -o ${OBJECTDIR}/EventChecker.o EventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/IO_Ports.o: IO_Ports.c  .generated_files/flags/default/7685c62f3a06d9a2877eeeac9280391f75cf54d6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/IO_Ports.o.d" -o ${OBJECTDIR}/IO_Ports.o IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LED.o: LED.c  .generated_files/flags/default/58452e853f4af237dbeded25404c74c13496b1be .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LED.o.d 
	@${RM} ${OBJECTDIR}/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/LED.o.d" -o ${OBJECTDIR}/LED.o LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/RC_Servo.o: RC_Servo.c  .generated_files/flags/default/919ae0c2addb66238f199e0ad30f6dc50e267be5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/RC_Servo.o.d" -o ${OBJECTDIR}/RC_Servo.o RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Stepper.o: Stepper.c  .generated_files/flags/default/df83cdfe5bfe42895179e5dbd6c686222c1ff6bd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Stepper.o.d 
	@${RM} ${OBJECTDIR}/Stepper.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/Stepper.o.d" -o ${OBJECTDIR}/Stepper.o Stepper.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Tank_DriveTrain.o: Tank_DriveTrain.c  .generated_files/flags/default/436c5927cadd8319903f450a7b986d7fffab56df .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Tank_DriveTrain.o.d 
	@${RM} ${OBJECTDIR}/Tank_DriveTrain.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/Tank_DriveTrain.o.d" -o ${OBJECTDIR}/Tank_DriveTrain.o Tank_DriveTrain.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TapeService.o: TapeService.c  .generated_files/flags/default/77b663db3b86e0516a93da47feb284876e2a26e9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TapeService.o.d 
	@${RM} ${OBJECTDIR}/TapeService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TapeService.o.d" -o ${OBJECTDIR}/TapeService.o TapeService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateFSM.o: TemplateFSM.c  .generated_files/flags/default/c46747ca3096f5498be0df17ab258438431f263c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateFSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateFSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TemplateFSM.o.d" -o ${OBJECTDIR}/TemplateFSM.o TemplateFSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateHSM.o: TemplateHSM.c  .generated_files/flags/default/c5b0e5610643f67b1f471c886f880ef393a2c61a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TemplateHSM.o.d" -o ${OBJECTDIR}/TemplateHSM.o TemplateHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateService.o: TemplateService.c  .generated_files/flags/default/7e90eab4191ec9727daf67c60a785ecb173f0a8c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateService.o.d 
	@${RM} ${OBJECTDIR}/TemplateService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TemplateService.o.d" -o ${OBJECTDIR}/TemplateService.o TemplateService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateSubHSM.o: TemplateSubHSM.c  .generated_files/flags/default/e98bfae53e62318b450170a39009c0e73068b73e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/TemplateSubHSM.o.d" -o ${OBJECTDIR}/TemplateSubHSM.o TemplateSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ping.o: ping.c  .generated_files/flags/default/eaf0e19e293f874082f92bb5976389028f9f7163 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ping.o.d 
	@${RM} ${OBJECTDIR}/ping.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/ping.o.d" -o ${OBJECTDIR}/ping.o ping.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/pwm.o: pwm.c  .generated_files/flags/default/e6920014872840d9370a010591509851bc90c448 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm.o.d 
	@${RM} ${OBJECTDIR}/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/pwm.o.d" -o ${OBJECTDIR}/pwm.o pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/serial.o: serial.c  .generated_files/flags/default/2e4172e2f97e7e9537867c253b66d6a3b178fbf6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/serial.o.d 
	@${RM} ${OBJECTDIR}/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/serial.o.d" -o ${OBJECTDIR}/serial.o serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/timers.o: timers.c  .generated_files/flags/default/8b01c072a8c2135e32341d9985c2103636df9e14 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timers.o.d 
	@${RM} ${OBJECTDIR}/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -I"." -I"../ECE118/include" -MP -MMD -MF "${OBJECTDIR}/timers.o.d" -o ${OBJECTDIR}/timers.o timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/ECE118_Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/ECE118_Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=4096,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/ECE118_Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/ECE118_Final_Project.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=4096,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex ${DISTDIR}/ECE118_Final_Project.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
