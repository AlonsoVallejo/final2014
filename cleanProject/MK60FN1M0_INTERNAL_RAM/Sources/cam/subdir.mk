################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/cam/cam.c" \
"../Sources/cam/inits.c" \
"../Sources/cam/main.c" \

C_SRCS += \
../Sources/cam/cam.c \
../Sources/cam/inits.c \
../Sources/cam/main.c \

OBJS += \
./Sources/cam/cam.obj \
./Sources/cam/inits.obj \
./Sources/cam/main.obj \

OBJS_QUOTED += \
"./Sources/cam/cam.obj" \
"./Sources/cam/inits.obj" \
"./Sources/cam/main.obj" \

C_DEPS += \
./Sources/cam/cam.d \
./Sources/cam/inits.d \
./Sources/cam/main.d \

C_DEPS_QUOTED += \
"./Sources/cam/cam.d" \
"./Sources/cam/inits.d" \
"./Sources/cam/main.d" \

OBJS_OS_FORMAT += \
./Sources/cam/cam.obj \
./Sources/cam/inits.obj \
./Sources/cam/main.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/cam/cam.obj: ../Sources/cam/cam.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/cam/cam.args" -o "Sources/cam/cam.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/cam/%.d: ../Sources/cam/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/cam/inits.obj: ../Sources/cam/inits.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/cam/inits.args" -o "Sources/cam/inits.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/cam/main.obj: ../Sources/cam/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/cam/main.args" -o "Sources/cam/main.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


