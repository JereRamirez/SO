################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FileSystem/src/FileSystem.c \
../FileSystem/src/funcionesFileSystem.c \
../FileSystem/src/sockets.c 

OBJS += \
./FileSystem/src/FileSystem.o \
./FileSystem/src/funcionesFileSystem.o \
./FileSystem/src/sockets.o 

C_DEPS += \
./FileSystem/src/FileSystem.d \
./FileSystem/src/funcionesFileSystem.d \
./FileSystem/src/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
FileSystem/src/%.o: ../FileSystem/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


