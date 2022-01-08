# Lesson 5 Assignment

## Final Project Planning

Start putting together a plan for your final project. Given the chapter, the main goal is to put together state machine documentation. This can be a flowchart or (preferably) a state table. You may also need to create the diagrams from Lesson 2 to flesh out the design. Note: this design planning, thinking through the system, what you have and what you need. Keep in mind that these will not be the final version!

# Assignment Submission

## Project Summary

My project involves the development of a temperature logger that will record the ambient temperature on a periodic basis. Groups of these temperature loggers can be placed in an external environment, and when collected, the data can be used to create a linear or two-dimensional view of temperature over time.

## Problem Statement

High-density temperature measurements of micro-climates is currently infeasible due to the high cost of temperature loggers. Due to these high costs, no infrastructure has been developed to support large scale grid, mesh, or linear deployments of sensors.

## Solution Statement

A low-cost sealed reusable temperature sensor can be inexpensively dropped in the field, with the location marked by a cell phone application. The cell phone application indicates where each sensor should be for collection. Data is transferred to the cell phone optically, then displayed in the corresponding spatial configuration. Once collected, sensors are wirelessly recharged in preparation for redeployment.

## Differentiating Features

* Sealed design allows use in outdoors (-10 C to 40 C) and in wet environments (NEMA 6P)
* Wireless charging of internal super capacitor eliminates battery waste
* Optical data transfer reduces collection time

## Challenges

* This is a miniaturized design, which requires high-tolerance assembly techniques that are at the edge of my current ability
* I have not worked with super capacitors before
* I have not worked with wireless power transfer before

## Scope

### In-Scope for Class Final Project

For the class final project, I will be focusing on the following areas:

* Microprocessor bring-up.
* Boot and state machine
* Data collection mode
* Data transfer mode

### Out of scope for Class Final Project

* Power subsystem
* Cell phone app
* Resin encapsulation
* Productization

## Product Usage Narrative

Alice has always wondered about temperature gradients across her field, so she purchases 100 temperature loggers, bulk-charges them, then walks out to her field to deploy them. In the cell phone app, she creates a new deployment of type “grid” (vs. “linear” or “free-form”), with a spacing of 10 meters. At a location close to where the cell phone suggests, she marks which sensor is being placed there by using the phone camera to optically receive the unique id of the sensor. This also records the exact location of where the sensor is placed. This is repeated 100 times to deploy all of the sensors.

One month later, Alice returns to her field to collect the sensors. She uses the cell phone app to find each sensor, and to optically receive the recorded data. Once back in the house, she recharges the sensors, and exports the collected sensor temperature data and sensor location data, which allows her to create an animation of temperature over time overplayed on top of aerial photography of her field.

## Key Technical Limitations

* Power consumption is a critical factor in product usability. The amount of charge stored in a super capacitor is quite low, so the number of temperature samples will be determined by the self-discharge rate plus the energy consumed per temperature sensor times the rate of taking temperature samples.

## Top Level Block Diagram

![image](https://user-images.githubusercontent.com/5757591/148631400-28eeb647-4658-4a58-8cb8-b28e84d96589.png)

### Charger

The charger performs the following functions:

* Uses magnetic resonant coupling to induce current into nearby sensor devices to recharge the super capacitor.

### Power Subsystem

The power subsystem performs the following functions:

* Protects the super capacitor from over-voltage
* Regulates charging voltage for the super capacitor
* Indicates super capacitor voltage to the microprocessor
* Indicates charge voltage to the microprocessor
* Boost converts the super capacitor voltage to a suitable supply voltage for the microprocessor
* Power-down and wakeup delay

### Microprocessor Subsystem

* Reads temperature values from a temperature sensor
* Stores temperature values in flash memory
* Monitors charging voltage and the super capacitor voltage
* Sends device identifier via an LED
* Sends sensor values via a LED
* Clears stored temperature values when charged

## Program Flow

### Flowchart

![image](https://user-images.githubusercontent.com/5757591/148633404-659b0f80-7a87-428a-8e35-b16497553e9d.png)

### State Machine

![image](https://user-images.githubusercontent.com/5757591/148632885-d45b7948-45ea-4071-92ee-a5cde1274ba1.png)

### State Table

```
State           | Actions                                            | Next State
                | System     | Memory        | LED       | Power     |                | >3v          | > 0.5v        | < 0.5v         |
----------------|------------|---------------|-----------|-----------|----------------|--------------|---------------|----------------|
Uninitialized   | Initialize |               |           |           | Initialized    |              |               |                |
Initialized     |            |               |           | Check Vca |                | Wait for Vcb | Data Sent     | Reading Stored |
Wait for Vcb    |            |               | Send Data |           | Memory Cleared |              |               |                |
Memory Cleared  |            | Clear Memory  | Blink     |           | Powered Off    |              |               |                |
Data Sent       |            |               | Send Data |           | Powered Off    |              |               |                |
Reading Stored  |            | Store Reading |           |           | Powered Off    |              |               |                |
Powered Off     |            |               |           | Power Off | Uninitialized  |              |               |                |
----------------|------------|---------------|-----------|-----------|----------------|--------------|---------------|----------------|
```


## Detailed Block Diagram

![image](https://user-images.githubusercontent.com/5757591/148632269-927e1d28-1843-4b77-b5be-181597f47de9.png)



