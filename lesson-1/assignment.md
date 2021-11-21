# Lesson 1 Assignment

## Invent an Imaginary Embedded System

Tell us about an embedded system that does not exist. It can be grandiose or strange; it can be
something just one step beyond what you have around you. Ideally it is something you
personally want to build (even if physics doesn’t allow it). What is it? What does it do? What
general components do you need?

This should be summarized in 3-6 sentences. A diagram would be nice but not required.
Write your idea in a Google doc or a md file on Github and submit your assignment link to the
#assignment-submission channel on Discord. Make sure your submission is publicly viewable.

# Assignment Submission

## Problem Statement

High resolution temperature monitoring of microclimates is currently infeasible due to the high cost of currently available equipment.

To be feasible, the following requirements need to be met:

* Total COGS cost per unit less than $10 (wireless)
* Total COGS cost per unit less than $5 (optical only)
* Deployable to measure a 625 square meter area with a granularity of 1 meter
* Run off a rechargeable or replaceable power source for a minimum of one month
* Acquire temperature measurements with a precision of 0.5 degree C and an accuracy of +/- 0.5 degree C
* Transfer temperature sensor measurements wirelessly to a collection station 1/2 km away, or, store sensor readings and transfer them out optically
* Transfer temperature sensor measurements at a minimum sampling rate of 120 seconds
* Transfer temperature sensor measurements securely
* Operate in an outdoor environment with full exposure to the elements (NEMA 6)
* Operate in a thermal environment ranging from -10C to 40C
* Easily be geo-registered using a mobile phone

If a low-cost temperature monitoring “mote”, which can be inexpensively deployed into the field, is designed and manufactured, it will enable many applications in agriculture, forestry, and other fields where dense medium resolution temperature sensing is required that are currently cost prohibitive.

## High-level Block Diagram

![image](https://user-images.githubusercontent.com/5757591/141662694-92c1493f-65ca-4880-b1ca-702c1588919c.png)

### Power Subsystem

Power is supplied by a super-capacitor, which is boosted to 3.3V by a boost converter such as a [TI TSP61010](https://www.ti.com/lit/ds/slvs314f/slvs314f.pdf). Recharging is performed using inductive coupling through a PCB coil, a rectifier, and a linear regulator. High efficiency during recharging is not required. Supply voltage to the µP subsystem is enabled every 120 seconds by a RC circuit, and is reset by the µP once processing has completed.

### Temperature Sensor Subsystem

Temperature sensors are acquired by an I2C device such as a [TI TEMP100](https://www.ti.com/lit/ds/symlink/tmp100.pdf).

### Microprocessor Subsystem

Data capture, transmission and shutdown is managed by a RP2040 embedded processor. The µP initializes, reads a temperature value, stores the temperature value, and transmits data via LORA (if present). When charging power is provided, the unit identifies itself via the LED and dumps all stored values.

### Data Transmission Subsystem

For wireless data transfer, a low-cost LORA module shall be used to wirelessly send the device ID and the most recently acquired temperature value. The device ID combined with the sample storage address shall be used as the encryption key.

For optical data transfer, a LED is modulated to be received by a smart-phone app using the smart phone’s camera.

### Mechanical Assembly

There will likely be two PCBs, one containing the power subsystem, and the second containing the µP and communication subsystems, with the super-capacitor sandwiched between the two boards. The assembly will be potted in a optically clear silicone other potting substance to provide environmental resistance.

### Programming and Test

Test and programming will be performed by placing the µP/comm board on a test fixture which has access to the SWDIO/SWCLK pins, as well as the I2C bus. This will allow the firmware to be loaded, and the unique device ID to be programmed in.

Once potted, devices will not be reprogrammable.
