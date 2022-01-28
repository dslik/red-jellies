# Lesson 8 Assignment

## Stacks and Heaps

Compile your project code so it prints out the heap pointer and the stack pointer. Print out a global variable address. If you can find it, modify your linker file to swap your uninitialized variables and initialized variables. Verify it is as expected in the memory map. See how that changes your code output. 

# Assignment Submission

The Pi Pico memory map is defined in section 2.2 of the [RP2040 Datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf), as shown below:

![image](https://user-images.githubusercontent.com/5757591/151506316-bdce0558-1752-4ea3-bf7c-8acfeb3614a6.png)

Some of these values are available in the default linker script:
* https://github.com/raspberrypi/pico-sdk/blob/master/src/rp2_common/pico_standard_link/memmap_default.ld

This gives us enough information to write a quick exploratory program:

## Simple Exploratory Program

```c
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"

int global;
int globalInit = 0;

int main() {
    extern int __end__;
    extern int __HeapLimit;
    extern int __bss_start__;
    extern int __bss_end__;
    extern int __stack;
    extern int __StackLimit;

    const char* constString = "  Test #2: Address of a constant string: 0x%08X\n";
    int uninitialized;
    int initialized = 0;

    volatile int stackPointer;

    stdio_init_all();
    printf("Raspberry Pi Pico Address Map Exploration - Exercise 8\n");
    printf("------------------------------------------------------------------------------\n");
    printf("The Pi Pico datasheet indicates that External Flash starts at 0x10000000\n");
    printf("We would thus expect all .text and .data to exist close to this address.\n");
    printf("  Test #1: Address of main():            0x%08X\n", (uint32_t) &main);
    printf(constString, constString);
    printf("------------------------------------------------------------------------------\n");
    printf("The Pi Pico datasheet indicates that SRAM starts at 0x20000000\n");
    printf("We would thus expect all .bss, heap and stack to exist close to this address.\n");
    printf("Checking initialized and uninitialized global variables:\n");
    printf("  Test #3: Uninitialized global:    0x%08X\n", (uint32_t) &global);
    printf("  Test #4: Initialized global:      0x%08X\n", (uint32_t) &globalInit);
    printf("These should exist within the bss range. The Pi Pico build system defines two\n");
    printf("variables indicating where the bss starts and ends: __bss_start__ and __bss_end__.\n");
    printf("  Test #5: bss start:               0x%08X\n", (uint32_t) &__bss_start__);
    printf("  Test #6: bss end:                 0x%08X\n", (uint32_t) &__bss_end__);
    printf("  Test #7: bss size:                %lu Bytes\n", (uint32_t) &__bss_end__ - (uint32_t) &__bss_start__);
    printf("------------------------------------------------------------------------------\n");
    printf("The Pi Pico build system defines two variables indicating where the heap\n");
    printf("starts and ends: __end__ and __HeapLimit.\n");
    printf("  Test #8: Heap start:              0x%08X\n", (uint32_t) &__end__);
    printf("  Test #9: Heap end:                0x%08X\n", (uint32_t) &__HeapLimit);
    printf("  Test #10: heap size:              %lu Bytes\n", (uint32_t) &__HeapLimit - (uint32_t) &__end__);
    printf("As expected, the heap start matches the bss end.\n");
    printf("------------------------------------------------------------------------------\n");
    printf("Checking initialized and uninitialized variables (on the stack):\n");
    printf("  Test #11: Uninitialized variable: 0x%08X\n", (uint32_t) &uninitialized);
    printf("  Test #12: Initialized variable:   0x%08X\n", (uint32_t) &initialized);
    printf("These should exist within the stack range. The Pi Pico build system defines two\n");
    printf("variables indicating where the stack starts and ends: __stack and __StackLimit.\n");
    printf("  Test #13: Stack start:            0x%08X\n", (uint32_t) &__StackLimit);
    printf("  Test #14: Stack end:              0x%08X\n", (uint32_t) &__stack);
    printf("  Test #15: Stack size:             %lu Bytes\n", (uint32_t) &__stack - (uint32_t) &__StackLimit);
    printf("The stack pointer can be accessed from the ARM r13 register with a little fiddling:\n");

    asm volatile ("mov %0, r13;" : "=r"(stackPointer));

    printf("  Test #16: Stack pointer:          0x%08X\n", (uint32_t) stackPointer);
    printf("------------------------------------------------------------------------------\n");

    // Do nothing
    while (true) {
        sleep_ms(1000);
    }
}
```

## Program Output

![image](https://user-images.githubusercontent.com/5757591/151507151-f4ae4a1a-4115-4603-9f32-639d8f305511.png)

Here we see the requested information:

* Heap pointer - There is no subsystem using the heap (no malloc or similar). If one were to write a heap-based allocator, the start and end values would indicate how much memory (and what memory) you have to work with.
* Stack pointer - Printed. This had to be obtained from the r13 (sp) register using some inline assembly.
* Print out a global variable address - Printed.

## Discussion

As uninitialized variables and initialized variables were all in the stack, swaping them around did not make sense. Changing the order of the variable declarations would switch around the memory addresses, but I don't think that was what was being asked for.

Uninitialized and initialized global variables were are separated by a fair number of bytes in the .bss. However, I was unable to identify what changes to the linker script would swap them around. All of the .bss contents were added by a single line.

Thus, I was unable to modify my linker file to swap your uninitialized variables and initialized global variables.
