## Exercise 3b: SPI Flash Test Code

Unfortunately, the SPI Flash we need to test the Key-Value storage system is on backorder. Create a small simulator so you can test the KV code.

Here is the SPI Flash information. https://www.digikey.com/en/products/detail/winbond-electronics/W25Q80DVSNIG-TR/5154948

We are going to use ST’s Standard Peripheral Flash Driver so here is the header file: https://drive.google.com/file/d/12dw-pZiKiK1vi4gv49W1x6np5uzCH_cV/view?usp=sharing

Your assignment is to create a small module that compiles runs on your computer that implements your flash simulator. You will also need a main file to test out your simulator. Important notes: You don’t need a full Flash simulator, only what is necessary to test the KV code. You aren’t writing the KV code, only testing it. You do not need to use CPPUTest or other unit test framework. If you are writing more than 50 lines of code, you are going in the wrong direction.

## Assignment Submission

I originally thought that this needed to be able to run the Mbed KV store code, so I worked towards implementing a QSPI layer inside of Mbed that simulated a flash device. After receiving clarification from the instructors, I wrote a simple layer that implements the spi_flash API as defined in the specified header file.

This program has been tested on OSX, and can be built using the "build.sh" shell script.

When run, it outputs the following:

![image](https://user-images.githubusercontent.com/5757591/144774956-50ff7733-33f8-4597-9572-085642381c7b.png)

The contents of the emulated flash device are stored in a file named "flash_data". This allows the output of the emulator to be inspected (and manipulated) using standard file tools.

![image](https://user-images.githubusercontent.com/5757591/144775060-b7ca0bc6-1df1-4344-8805-8c8d8ab6e444.png)

### File Descriptions

* spi_flash.h - The provided STM header file defining what should be implemented.
* build.sh - A shell script which builds the tests
* flash_data - A file that contains the contents of the flash. This is automatically created (and set to all 0xFF) when the tests are first run
* main - The built executable
* main.cc - The test program
* spi_flash.cc - The implementation of the simple flash emulator
* stm32f2xx.h - A shim header to allow the provided STM header to be used unmodified.
* test_utils.cc - Some simple test utility functions
* test_utils.h - Headers for the test utility functions

