cmake_minimum_required(VERSION 3.0)

# Project name
project(64os)

# Set build directory (can be customized)
set(CMAKE_BINARY_DIR build)

# Include directories
include_directories(src/intf)

# Compiler flags
set(CMAKE_C_FLAGS "-ffreestanding -I${CMAKE_CURRENT_SOURCE_DIR}/src/intf")

# Define source file lists using GLOB
set(KERNEL_SOURCE_FILES)
GLOB(KERNEL_SOURCE_FILES "*.c" RELATIVE PATH src/impl/kernel)

set(X86_64_C_SOURCE_FILES)
GLOB(X86_64_C_SOURCE_FILES "*.c" RELATIVE PATH src/impl/x86_64)

set(X86_64_ASM_SOURCE_FILES)
GLOB(X86_64_ASM_SOURCE_FILES "*.asm" RELATIVE PATH src/impl/x86_64)

# Define object file lists based on source files
source_group(SOURCE_FILES FILES ${KERNEL_SOURCE_FILES} ${X86_64_C_SOURCE_FILES} ${X86_64_ASM_SOURCE_FILES})

set(KERNEL_OBJECT_FILES)
foreach(SOURCE_FILE ${KERNEL_SOURCE_FILES})
  get_filename_component(OBJECT_FILE ${SOURCE_FILE} NAME_WE)
  set(KERNEL_OBJECT_FILES ${KERNEL_OBJECT_FILES} "${CMAKE_BINARY_DIR}/kernel/${OBJECT_FILE}.o")
endforeach()

set(X86_64_C_OBJECT_FILES)
foreach(SOURCE_FILE ${X86_64_C_SOURCE_FILES})
  get_filename_component(OBJECT_FILE ${SOURCE_FILE} NAME_WE)
  set(X86_64_C_OBJECT_FILES ${X86_64_C_OBJECT_FILES} "${CMAKE_BINARY_DIR}/x86_64/${OBJECT_FILE}.o")
endforeach()

set(X86_64_ASM_OBJECT_FILES)
foreach(SOURCE_FILE ${X86_64_ASM_SOURCE_FILES})
  get_filename_component(OBJECT_FILE ${SOURCE_FILE} NAME_WE)
  set(X86_64_ASM_OBJECT_FILES ${X86_64_ASM_OBJECT_FILES} "${CMAKE_BINARY_DIR}/x86_64/${OBJECT_FILE}.o")
endforeach()

# Define Executable target
add_executable(kernel "${KERNEL_OBJECT_FILES}" "${X86_64_C_OBJECT_FILES}" "${X86_64_ASM_OBJECT_FILES}")

# Linker options (assuming linker script location)
target_link_libraries(kernel PRIVATE targets/x86_64/linker.ld)

# Custom target for building x86_64 image (can be adapted for your needs)
add_custom_target(build-x86_64 ALL
  COMMAND mkdir -p "${CMAKE_BINARY_DIR}/dist/x86_64"
  COMMAND x86_64-elf-ld -n -o "${CMAKE_BINARY_DIR}/dist/x86_64/kernel.bin" -T targets/x86_64/linker.ld $<
  COMMAND cp "${CMAKE_BINARY_DIR}/dist/x86_64/kernel.bin" targets/x86_64/iso/boot/kernel.bin
  # Adapt this command for your specific toolchain
  COMMAND grub-mkrescue /usr/lib/grub/i386-pc -o "${CMAKE_BINARY_DIR}/dist/x86_64/kernel.iso" targets/x86_64/iso
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# Make the custom target the default build target
set_target_properties(kernel PROPERTIES BUILD_TYPE CUSTOM)