# Lesson 9 Assignment

## Code Tradeoffs

In Quiz 1, multiple different implementations for bit reversal were given and you were asked to consider the algorithms with respect to speed, RAM, and code space. (You can see the code at Godbolt.)

Choose a small algorithm to implement in different ways. Describe when/why you might use each. You can choose any small algorithm.

# Assignment Submission

I had originally was going to try a few different approaches to bit-reversal that weren't covered in the quiz, but since there is a single ARM assembly instruction (RBIT) that will do this for you, it felt like cheating because there was one solution that would be better in every dimension.

As a second choice, I selected to take a look at convering a 32-bit integer into a string that contains the binary values of the integer. This is both useful (since many times you want to see the binary representation of a value when debugging), and non-trivial.

Some constraints:
* No heap allocation was used
* The implementations are not reentrant-safe or interrupt-safe or multi-processor safe. This is fairly easy to add by allowing a local string in the caller's stack frame to be passed in as an input, but since this was not the focus of the exercise, I did not implement this so as to focus on the core algorithm's performance and code size.

## Three implementations

### Approach 1: Basic logic

This implementation uses basic logic to count down from the 31st bit position to the 0th bit position, checking each against a mask to determine if the bit is set, and setting the corresponding bit position in the string to a "1" or "0".

This implementation has the most lines of code, and looks like it has the most operations, so I would expect this to be the slowest.

```c
char binary_string[] = "00000000000000000000000000000000";

// Int to binary string - Simple logic
const char* int_to_binary_string_1(uint32_t input)
{
    uint32_t bit_value = 1;
    uint32_t bit_count = 32;

    while(bit_count > 0)
    {
        if((input & bit_value) != 0)
        {
            binary_string[bit_count - 1] = '1';
        }
        else
        {
            binary_string[bit_count - 1] = '0';
        }

        bit_value = bit_value << 1;
        bit_count = bit_count - 1;
    }

    return (const char*) binary_string;
}
```

### Approach 2: Optimized logic

This implementation uses a similar control flow to the first approach, but reduces the number of C language lines of code from 16 to 7. It also eliminates the need to keep track of both a position counter and a mask, and gets rid of a branch, which should help with performance.

Since fewer operations are being performed, I figured that this implementation would be faster than the first one, but not significantly so.

```c
char binary_string[] = "00000000000000000000000000000000";

// Int to binary string - Optimized logic
const char* int_to_binary_string_2(uint32_t input)
{
    uint32_t bit_count = 32;

    while(bit_count > 0)
    {
        binary_string[bit_count - 1] = '0' + ((input & (1 << (32 - bit_count))) == (1 << (32 - bit_count)));
        bit_count = bit_count - 1;
    }

    return (const char*) binary_string;
}
```

### Approach 3: Unrolled loop

Since loops require branching, and branching often is a major performance issue, this implementation unrolls the loop, using a similar logic to implementation 2 to compare the bit against the mask

Since fewer operations are being performed, I figured that this implementation would be the fastest, at the expense of code size.

```c
char binary_string[] = "00000000000000000000000000000000";

// Int to binary string - Unrolled
const char* int_to_binary_string_3(uint32_t input)
{
    binary_string[0]  = '0' + ((input & (1 << 31)) == (1 << 31));
    binary_string[1]  = '0' + ((input & (1 << 30)) == (1 << 30));
    binary_string[2]  = '0' + ((input & (1 << 29)) == (1 << 29));
    binary_string[3]  = '0' + ((input & (1 << 28)) == (1 << 28));
    binary_string[4]  = '0' + ((input & (1 << 27)) == (1 << 27));
    binary_string[5]  = '0' + ((input & (1 << 26)) == (1 << 26));
    binary_string[6]  = '0' + ((input & (1 << 25)) == (1 << 25));
    binary_string[7]  = '0' + ((input & (1 << 24)) == (1 << 24));
    binary_string[8]  = '0' + ((input & (1 << 23)) == (1 << 23));
    binary_string[9]  = '0' + ((input & (1 << 22)) == (1 << 22));
    binary_string[10] = '0' + ((input & (1 << 21)) == (1 << 21));
    binary_string[11] = '0' + ((input & (1 << 20)) == (1 << 20));
    binary_string[12] = '0' + ((input & (1 << 19)) == (1 << 19));
    binary_string[13] = '0' + ((input & (1 << 18)) == (1 << 18));
    binary_string[14] = '0' + ((input & (1 << 17)) == (1 << 17));
    binary_string[15] = '0' + ((input & (1 << 16)) == (1 << 16));
    binary_string[16] = '0' + ((input & (1 << 15)) == (1 << 15));
    binary_string[17] = '0' + ((input & (1 << 14)) == (1 << 14));
    binary_string[18] = '0' + ((input & (1 << 13)) == (1 << 13));
    binary_string[19] = '0' + ((input & (1 << 12)) == (1 << 12));
    binary_string[20] = '0' + ((input & (1 << 11)) == (1 << 11));
    binary_string[21] = '0' + ((input & (1 << 10)) == (1 << 10));
    binary_string[22] = '0' + ((input & (1 << 9))  == (1 << 9));
    binary_string[23] = '0' + ((input & (1 << 8))  == (1 << 8));
    binary_string[24] = '0' + ((input & (1 << 7))  == (1 << 7));
    binary_string[25] = '0' + ((input & (1 << 6))  == (1 << 6));
    binary_string[26] = '0' + ((input & (1 << 5))  == (1 << 5));
    binary_string[27] = '0' + ((input & (1 << 4))  == (1 << 4));
    binary_string[28] = '0' + ((input & (1 << 3))  == (1 << 3));
    binary_string[29] = '0' + ((input & (1 << 2))  == (1 << 2));
    binary_string[30] = '0' + ((input & (1 << 1))  == (1 << 1));
    binary_string[31] = '0' + ((input & (1 << 0))  == (1 << 0));

    return (const char*) binary_string;
}
```

## Results

![image1](https://user-images.githubusercontent.com/5757591/152623811-e86f18b6-b274-4150-a721-43fa57ece430.png)

## Discussion

Implementation 2 was slower than implementation 1, and was (very slightly) larger. This was unexpected. I tried moving the (1 << (32 - bit_count)) into a temp variable, but that made no difference, so the compiler is likely optimizing this already. It would be interesting to dump the assembly to see why this is slower, despite the branch being removed.

Implementation 3 was the fastest, as expected. However the generated code size is almost five times larger.

All of these algorithms use the same amount of RAM.

Given the results, I would use the second implementation if flash space is constrained, and the third implementation if flash space constriants aren't quite as bad, and this code is in a performance-critical section.

## Full Source Code

```c
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

// The algorithm I selected to explore takes a uint32_t and 
// converts it into a string representation of the binary value
// A global variable is used to avoid any dependency on a heap allocator
// This makes this function non-reentrant and cannot be safely called
// from within an interrupt or by multiple cores.

char binary_string[] = "00000000000000000000000000000000";

// Int to binary string - Simple logic
const char* int_to_binary_string_1(uint32_t input)
{
    uint32_t bit_value = 1;
    uint32_t bit_count = 32;

    while(bit_count > 0)
    {
        if((input & bit_value) != 0)
        {
            binary_string[bit_count - 1] = '1';
        }
        else
        {
            binary_string[bit_count - 1] = '0';
        }

        bit_value = bit_value << 1;
        bit_count = bit_count - 1;
    }

    return (const char*) binary_string;
}

// Int to binary string - Optimized logic
const char* int_to_binary_string_2(uint32_t input)
{
    uint32_t bit_count = 32;

    while(bit_count > 0)
    {
        binary_string[bit_count - 1] = '0' + ((input & (1 << (32 - bit_count))) == (1 << (32 - bit_count)));
        bit_count = bit_count - 1;
    }

    return (const char*) binary_string;
}

// Int to binary string - Unrolled
const char* int_to_binary_string_3(uint32_t input)
{
    binary_string[0]  = '0' + ((input & (1 << 31)) == (1 << 31));
    binary_string[1]  = '0' + ((input & (1 << 30)) == (1 << 30));
    binary_string[2]  = '0' + ((input & (1 << 29)) == (1 << 29));
    binary_string[3]  = '0' + ((input & (1 << 28)) == (1 << 28));
    binary_string[4]  = '0' + ((input & (1 << 27)) == (1 << 27));
    binary_string[5]  = '0' + ((input & (1 << 26)) == (1 << 26));
    binary_string[6]  = '0' + ((input & (1 << 25)) == (1 << 25));
    binary_string[7]  = '0' + ((input & (1 << 24)) == (1 << 24));
    binary_string[8]  = '0' + ((input & (1 << 23)) == (1 << 23));
    binary_string[9]  = '0' + ((input & (1 << 22)) == (1 << 22));
    binary_string[10] = '0' + ((input & (1 << 21)) == (1 << 21));
    binary_string[11] = '0' + ((input & (1 << 20)) == (1 << 20));
    binary_string[12] = '0' + ((input & (1 << 19)) == (1 << 19));
    binary_string[13] = '0' + ((input & (1 << 18)) == (1 << 18));
    binary_string[14] = '0' + ((input & (1 << 17)) == (1 << 17));
    binary_string[15] = '0' + ((input & (1 << 16)) == (1 << 16));
    binary_string[16] = '0' + ((input & (1 << 15)) == (1 << 15));
    binary_string[17] = '0' + ((input & (1 << 14)) == (1 << 14));
    binary_string[18] = '0' + ((input & (1 << 13)) == (1 << 13));
    binary_string[19] = '0' + ((input & (1 << 12)) == (1 << 12));
    binary_string[20] = '0' + ((input & (1 << 11)) == (1 << 11));
    binary_string[21] = '0' + ((input & (1 << 10)) == (1 << 10));
    binary_string[22] = '0' + ((input & (1 << 9))  == (1 << 9));
    binary_string[23] = '0' + ((input & (1 << 8))  == (1 << 8));
    binary_string[24] = '0' + ((input & (1 << 7))  == (1 << 7));
    binary_string[25] = '0' + ((input & (1 << 6))  == (1 << 6));
    binary_string[26] = '0' + ((input & (1 << 5))  == (1 << 5));
    binary_string[27] = '0' + ((input & (1 << 4))  == (1 << 4));
    binary_string[28] = '0' + ((input & (1 << 3))  == (1 << 3));
    binary_string[29] = '0' + ((input & (1 << 2))  == (1 << 2));
    binary_string[30] = '0' + ((input & (1 << 1))  == (1 << 1));
    binary_string[31] = '0' + ((input & (1 << 0))  == (1 << 0));

    return (const char*) binary_string;
}

int main() {
    // Variables used to time each function
    uint64_t    startTime = 0;
    uint64_t    endTime = 0;
    uint32_t    counter = 0;
    uint64_t    samples = 100000;

    // Initialize the RP2040 subsystems
    // Calling sleep seems to be required for the tick counter to start counting
    clocks_init();
    sleep_ms(1);
    stdio_init_all();

    printf("------------------------------------------------------------------------------\n");
    printf("Testing int to binary string - Simple logic\n");
    printf("  Test #1: Address of function:               0x%08X\n", (uint32_t) &int_to_binary_string_1);
    printf("           Size of code:                      %lu Bytes\n", (uint32_t) &int_to_binary_string_2 - (uint32_t) &int_to_binary_string_1);

    counter = 0;
    startTime = time_us_64();
    while(counter <= 100000)
    {
        int_to_binary_string_1(counter);
        counter = counter + 1;
    }
    endTime = time_us_64();   

    printf("           Execution Time:                    %llu.%05llu us\n", (endTime - startTime) / samples, (endTime - startTime) % samples);
    printf("           Convert 100000 to binary:          %s\n", int_to_binary_string_1(10000));
    printf("           Convert 0xDEADBEEF to binary:      %s\n", int_to_binary_string_1(0xDEADBEEF));
    printf("           Convert 1<<31 to binary:           %s\n", int_to_binary_string_1(1<<31));

    printf("Testing int to binary string - Optimized logic\n");
    printf("  Test #2: Address of function:               0x%08X\n", (uint32_t) &int_to_binary_string_2);
    printf("           Size of code:                      %lu Bytes\n", (uint32_t) &int_to_binary_string_3 - (uint32_t) &int_to_binary_string_2);

    counter = 0;
    startTime = time_us_64();
    while(counter <= 100000)
    {
        int_to_binary_string_2(counter);
        counter = counter + 1;
    }
    endTime = time_us_64();   

    printf("           Execution Time:                    %llu.%05llu us\n", (endTime - startTime) / samples, (endTime - startTime) % samples);
    printf("           Convert 100000 to binary:          %s\n", int_to_binary_string_2(10000));
    printf("           Convert 0xDEADBEEF to binary:      %s\n", int_to_binary_string_2(0xDEADBEEF));
    printf("           Convert 1<<31 to binary:           %s\n", int_to_binary_string_2(1<<31));

    printf("Testing int to binary string - Unrolled logic\n");
    printf("  Test #3: Address of function:               0x%08X\n", (uint32_t) &int_to_binary_string_3);
    printf("           Size of code:                      %lu Bytes\n", (uint32_t) &main - (uint32_t) &int_to_binary_string_3);

    counter = 0;
    startTime = time_us_64();
    while(counter <= 100000)
    {
        int_to_binary_string_3(counter);
        counter = counter + 1;
    }
    endTime = time_us_64();   

    printf("           Execution Time:                    %llu.%05llu us\n", (endTime - startTime) / samples, (endTime - startTime) % samples);
    printf("           Convert 100000 to binary:          %s\n", int_to_binary_string_3(10000));
    printf("           Convert 0xDEADBEEF to binary:      %s\n", int_to_binary_string_3(0xDEADBEEF));
    printf("           Convert 1<<31 to binary:           %s\n", int_to_binary_string_3(1<<31));
    printf("------------------------------------------------------------------------------\n");

    // Do nothing
    while (true) {
        sleep_ms(1000);
    }
}
```
