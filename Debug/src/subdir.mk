################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SerialPort.cpp \
../src/SignalInterpretation.cpp \
../src/TrackHandler.cpp \
../src/Utils.cpp \
../src/main.cpp 

OBJS += \
./src/SerialPort.o \
./src/SignalInterpretation.o \
./src/TrackHandler.o \
./src/Utils.o \
./src/main.o 

CPP_DEPS += \
./src/SerialPort.d \
./src/SignalInterpretation.d \
./src/TrackHandler.d \
./src/Utils.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


