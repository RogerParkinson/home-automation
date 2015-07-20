################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/I2CHandler.cpp \
../src/Logger.cpp \
../src/RFReporter.cpp 

OBJS += \
./src/I2CHandler.o \
./src/Logger.o \
./src/RFReporter.o 

CPP_DEPS += \
./src/I2CHandler.d \
./src/Logger.d \
./src/RFReporter.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabi-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


