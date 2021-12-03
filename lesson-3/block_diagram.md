# Lesson 3 Assignments

## Exercise 3a: Investigate Project Board

Look at the documents for the board you are considering for your final project (or any ST
Discovery Board), draw the hardware block diagram for the board. For peripherals, note the
communication paths (SPI, I2C, etc).

Look at the datasheet for the processor and other documents. Answer these questions:
* What kind of processor is it?
* How much Flash and RAM does it have? Any other memory types?
* Does it have any special peripherals? (List 3-5 that you noted as being interesting.)
* Does it have a floating point unit?
* If it has an ADC, what are the features?

## Assignment Submission

I have selected the ST [B-L475E-IOT01A2](https://www.st.com/resource/en/user_manual/dm00347848-discovery-kit-for-iot-node-multichannel-communication-with-stm32l4-stmicroelectronics.pdf) evaluation board:

![image](https://user-images.githubusercontent.com/5757591/144546051-57bb46e5-681d-4da5-a920-8237f2a7a011.png)


### What kind of processor is it?

The board uses an STM32L475VGTx processor.

### How much Flash and RAM does it have? Any other memory types?

The STM32L475VGTx has 1 MB of internal Flash memory and 128 KB of internal SRAM

The board has an external 8 Mbyte Flash memory IC

### Does it have any special peripherals? (List 3-5 that you noted as being interesting.)

The board contains many interesting peripherals, such as a magnetometer, gyroscope, NFC reader, and dual microphones that are capable of beamforming.

### Does it have a floating point unit?

Yes. The STM32L475 includes a single precision FPU which supports all Arm single-precision data-processing instructions and data types.

### If it has an ADC, what are the features?

The STM32L475 has three 12-bit ADCs capable of running up to 5 Msps at full resolution, and up to 8 Msps at 6 bit resolution. The ADCs have an independent clock domain, support out-of-range detection, and support an external trigger clock input. Support single-ended and differential modes.
