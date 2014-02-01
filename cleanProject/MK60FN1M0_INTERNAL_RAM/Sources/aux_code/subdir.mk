################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/aux_code/arm_cm4.c" \
"../Sources/aux_code/io.c" \
"../Sources/aux_code/mcg.c" \
"../Sources/aux_code/uart.c" \

C_SRCS += \
../Sources/aux_code/arm_cm4.c \
../Sources/aux_code/io.c \
../Sources/aux_code/mcg.c \
../Sources/aux_code/uart.c \

OBJS += \
./Sources/aux_code/arm_cm4.obj \
./Sources/aux_code/io.obj \
./Sources/aux_code/mcg.obj \
./Sources/aux_code/uart.obj \

OBJS_QUOTED += \
"./Sources/aux_code/arm_cm4.obj" \
"./Sources/aux_code/io.obj" \
"./Sources/aux_code/mcg.obj" \
"./Sources/aux_code/uart.obj" \

C_DEPS += \
./Sources/aux_code/arm_cm4.d \
./Sources/aux_code/io.d \
./Sources/aux_code/mcg.d \
./Sources/aux_code/uart.d \

C_DEPS_QUOTED += \
"./Sources/aux_code/arm_cm4.d" \
"./Sources/aux_code/io.d" \
"./Sources/aux_code/mcg.d" \
"./Sources/aux_code/uart.d" \

OBJS_OS_FORMAT += \
./Sources/aux_code/arm_cm4.obj \
./Sources/aux_code/io.obj \
./Sources/aux_code/mcg.obj \
./Sources/aux_code/uart.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/aux_code/arm_cm4.obj: ../Sources/aux_code/arm_cm4.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/aux_code/arm_cm4.args" -o "Sources/aux_code/arm_cm4.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/aux_code/%.d: ../Sources/aux_code/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/aux_code/io.obj: ../Sources/aux_code/io.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/aux_code/io.args" -o "Sources/aux_code/io.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/aux_code/mcg.obj: ../Sources/aux_code/mcg.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/aux_code/mcg.args" -o "Sources/aux_code/mcg.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/aux_code/uart.obj: ../Sources/aux_code/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/aux_code/uart.args" -o "Sources/aux_code/uart.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


