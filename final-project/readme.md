# Final Project

## Final Project Deliverables

The final project will be delivered as:

(a) Video of the system working as intended (link to mp4 or youtube)

(b) Write up of the system (PDF or Google docs report).

(c) Link to the code

Bonus points are available by including one of these:

* An analysis of the power used in the system (expected vs actual)
* Implementation of firmware update with a description in the report of how it works
* A description of profiling the system to make it run faster

Additional bonus points are available by having a version control history showing the development process

# Final Submission

(a) Link to video - FIXME

(b) [Final Project Report](https://github.com/dslik/red-jellies/blob/main/final-project/final-project-report.md)

(c) [Source Code - Start here](https://github.com/dslik/red-jellies/blob/main/final-project/main.c)

## Project Requirements

While I did not complete the project as I originally defined it (e.g. all hardware components), I believe I met the defined class objectives for the project:

| Requirement | Link | Self-Assessment |
| ----------- | ---- | --------------- |
| Use a Cortex-M processor | Calor uses a [Raspberry Pi RP2040](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf) processor, which has dual Cortext M0+ cores | 2 - Criteria met |
| Have a button that causes an interrupt | Calor has a button as part of the test harness which [turns on and off data collection](https://github.com/dslik/red-jellies/blob/main/final-project/main.c#L264). [Switch Interrupt handling and debouncing](https://github.com/dslik/red-jellies/blob/main/final-project/utils.c#L271) is located in the utilities section. | 2 - Criteria met |
| Use at least three peripherals such as ADC, DAC, PWM LED, Smart LED, LCD, sensor, BLE | Calor demonstrates the following: ADC, Smart LED, Flash Programming, Temperature Sensor | 3 - Criteria exceeded |
| Have serial port output | Calor uses a [UART for logging and data output](https://github.com/dslik/red-jellies/blob/main/final-project/uart.c). Calor also implements a [simple command-line parser](https://github.com/dslik/red-jellies/blob/main/final-project/main.c#L95) for interacting with the system. | 3 - Criteria exceeded |
| Implement an algorithmic piece that makes the system interesting | Calor converts temperature data into frequency-modulated optical datastreams (blinking LED colours). The [basic algorithm](https://github.com/dslik/red-jellies/blob/main/final-project/main.c#L20) uses a lookup table to convert nibbles into distinguishable light frequencies that can be captured by a cell phone camera. While there is a *lot* more that could be done here (compression, Hamming distance, optical Manchester encoding, etc), it proves the concept. | 2 - Criteria met, but very simple |
| Implement a state machine | Calor implements a [voltage-controlled state machine](https://github.com/dslik/red-jellies/blob/main/final-project/main.c#L210) that coordinates operations across regular power-down/power-up cycles. State is preserved across power cycles. | 2 - Criteria met, but very simple |
| Bonus: An analysis of the power used in the system (expected vs actual) | The largest power consumer was the WS2812 smart LED. Details were submitted in [Assignment 10](https://github.com/dslik/red-jellies/tree/main/lesson-10). | 2 - Criteria met, but very simple |
| Bonus: Implementation of firmware update with a description in the report of how it works | I did implement anything beyond the SWD-based firmware upgrade system used by the RP2040. | 1 - Criteria not met |
| Bonus: A description of profiling the system to make it run faster | At no points did I run into any performance issues, so no profiling was done. | 1 - Criteria not met |
| Bonus: Having a version control history showing the development process | I used this GitHub repository to store my [final project source code](https://github.com/dslik/red-jellies/tree/main/final-project). | 2 - Criteria met |

## Project Grading Self-Assessment

| Criteria | Description | Self-Assessment |
| -------- | ----------- | --------------- |
| Project meets minimum project goals | Additional sensors were used. A command line interface on the serial port was implemented. | 3 - Exceeds Expectations |
| Completeness of deliverables | Code is readable without report. Report and video addresses project clearly and completely. | 3 - Exceeds Expectations |
| Clear intentions and working code | System performs as described. Code is clear for a maintainer. | 3 - Exceeds Expectations |
| Reusing code| Third-party code is used, is clearly labelled, along with the license. Build system works with standard examples provided with the Pi Pico. | 2 - Meets Expectations |
| Originality and scope of goals | Project is ambitious and original, but many of the novel aspects were out of the scope of this course (e.g. HW / smart phone app) | 2 - Meets Expectations |
| Power analysis, firmware update, or system profiling | Power profiling done for project as part of Exercise 10. No performance profiling done. | 2 - Meets Expectations |
| Version control was used | GitHub used for version control | 2 - Meets Expectations |
