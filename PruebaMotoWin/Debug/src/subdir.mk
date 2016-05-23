################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Model.cpp \
../src/camera.cpp \
../src/main.cpp \
../src/mainsdl.cpp 

OBJS += \
./src/Model.o \
./src/camera.o \
./src/main.o \
./src/mainsdl.o 

CPP_DEPS += \
./src/Model.d \
./src/camera.d \
./src/main.d \
./src/mainsdl.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


