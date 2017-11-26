################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Shared/Shared/clientes.c \
../Shared/Shared/protocolosNew.c \
../Shared/Shared/servidores.c \
../Shared/Shared/sockets.c 

OBJS += \
./Shared/Shared/clientes.o \
./Shared/Shared/protocolosNew.o \
./Shared/Shared/servidores.o \
./Shared/Shared/sockets.o 

C_DEPS += \
./Shared/Shared/clientes.d \
./Shared/Shared/protocolosNew.d \
./Shared/Shared/servidores.d \
./Shared/Shared/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
Shared/Shared/%.o: ../Shared/Shared/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


