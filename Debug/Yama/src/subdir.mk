################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Yama/src/Yama.c 

OBJS += \
./Yama/src/Yama.o 

C_DEPS += \
./Yama/src/Yama.d 


# Each subdirectory must supply rules for building sources it contributes
Yama/src/%.o: ../Yama/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


