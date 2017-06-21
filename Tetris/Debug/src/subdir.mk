# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Block.cpp \
../src/GameLogic.cpp \
../src/Tetris.cpp \
../src/myio.cpp 

OBJS += \
./src/Block.o \
./src/GameLogic.o \
./src/Tetris.o \
./src/myio.o 

CPP_DEPS += \
./src/Block.d \
./src/GameLogic.d \
./src/Tetris.d \
./src/myio.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
