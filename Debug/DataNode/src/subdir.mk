################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DataNode/src/DataNode.c 

OBJS += \
./DataNode/src/DataNode.o 

C_DEPS += \
./DataNode/src/DataNode.d 


# Each subdirectory must supply rules for building sources it contributes
DataNode/src/%.o: ../DataNode/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


