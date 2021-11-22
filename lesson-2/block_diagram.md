# Lesson 2 Assignments

## Exercise 2.1: Idea Project Diagram

Make a block diagram describing an embedded system you’d like to have or make. Be imaginative. Reality does not apply here.

## Assignment Submission

The below hardware block diagram is a refined version of the one in my [Lesson 1 assignment submission](https://github.com/dslik/red-jellies/blob/main/lesson-1/assignment.md):

![image](https://user-images.githubusercontent.com/5757591/142782077-c57d350a-1981-4baf-9988-78fe540510ca.png)

While it is unlikely that I would be able to build this entire device due to a combination of significant technical challenges and my current skill level, the µP subsystem is likely achievable.

## Exercise 2.2: Read mbed code, make diagram

* Go to mbed platforms https://os.mbed.com/platforms/
* Select any platform (hint: search for a processor similar to the one you are considering
for your final project)
* Read through the platform page
* Add to Mbed Compiler (button on right)
* Load a simple sounding template
* Create a block diagram for this program, briefly explain why you chose the platform
and template you did, be prepared to talk about this board in Live Class

## Assignment Submission

Selected Platform: ST-Discovery-L475E-IOT01A ([Docs](https://www.st.com/resource/en/user_manual/dm00347848-discovery-kit-for-iot-node-multichannel-communication-with-stm32l4-stmicroelectronics.pdf))

Selected Template: Sensor BSP

The below hardware block diagram shows the components of the board that are used by this program:

![image](https://user-images.githubusercontent.com/5757591/142782364-466b4c3c-b913-4695-86a9-4135fd6c4964.png)

The below software block diagram shows the relationships between software components (and hardware components):

![image](https://user-images.githubusercontent.com/5757591/142804162-006271f5-602f-4dac-879f-4d688aef8dc0.png)

I selected the IOT01A board because it had several on-board sensors and a non-trivial yet still relatively simple example program. My initial choice of the MAX32625PICO only had a single trivial sample program that blinked an LED, which I believe would not achieve the goal of this exercise. 
