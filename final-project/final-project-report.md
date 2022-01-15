# Introduction

Calor is a small re-usable device you can scatter across an area to measure how the temperature changes over time and space. Finally, high-density sensor networks are affordable, quick to deploy, and visualizing beautiful animated heatmaps have never been easier.

* TODO: Add photo of sensor device
* TODO: Add animation of heatmap visualization

Calor is the final project for the Classpert [Making Embedded Systems](https://classpert.com/classpertx/cohorts/making-embedded-systems) class, and focuses on the microcontroller aspects of Calor. The smartphone app and wireless charging aspects of this project are out of scope in the context of this class, and thus are only covered at a high level.

# Table of Contents

* Table of Contents
* Project Video
* Market Analysis
  * Target Market 
* Requirements
* User's Guide
* Design
  * System Components
  * Hardware Subsystems
  * Software Subsystems
    * Firmware
    * Smartphone Application
* Implementation
  * Power Subsystem
    * Charging
    * Power Storage
    * Power Regulation
    * Power Monitoring
    * Overvoltage Protection
    * Power Analysis
  * Microcontroller Subsystem
    * Microcontroller
    * Flash Storage
    * Temperature Sensor
    * Optical Signalling
  * Software
    * State Management
    * Charging
    * Activation (placement)
    * Acquisition
    * Transfer (collection)
    * Analysis
    * Software Dependencies
* Design for Manufacturing
  * Component Selection
  * Packaging
  * Assembly
  * Test
  * Firmware Update
* Build Instructions
  * Hardware
  * Software
* Futures
* Resources
* Grading

# Project Video

* TODO: Add embed of video

# Market Analysis

## Target Market

Measuring and recording outdoor temperature fluctuations over time in a single place is straightforward and inexpensive. Many products exist, such as the [Micron Tinytag TGP-4017](https://www.micronmeters.com/product/tgp-4017-internal-sensor-40-to-85-c-40-f-to-185-f), and the [Omega OM-EL-USB-1-RCG](https://www.omega.ca/en/temperature-measurement/temperature-and-humidity-data-loggers/p/OM-EL-USB-1-RCG-Logger), which retail at $163.93 and $164.98, respectively. Both of these products include a rechargeable battery, are rated for outdoor use, and are capable of recording tens of thousands of samples over time periods of 30 - 45 days, and have an accuracy of +/- 1 degree C. Deploying and collecting data from these devices are also relatively straightforward. Each device needs to be connected to a PC, configured with provided software, then activated once placed in the field. Upon collection, the collected samples are collected into a PC, and can be visualized using common software such as Microsoft Excel. Each of these processes takes approximately five minutes to complete.

However, consider a situation where a farmer wants to see how temperatures vary across a field over time. Different parts of the field may have very different temperature profiles depending on soil conditions, moisture levels, shade, topology and wind exposure. To get a level of data granularity, many sensors must be deployed. Even a very small one acre field is 43,560 square feet, and placing a sensor every twenty feet would require 100 sensors. As a result, the costs and difficulty of the deployment increasing substantially. The above commonly available products would now cost more than $16,000, and would take over eight hours to set up, not including the time to place the sensors, and another eight hours to download the data, not including the time to collect the sensors.. Visualizing the data also takes significantly more time.

As can be easily seen, this is cost prohibitive, and thus most "precision agriculture" applications have focused on remote sensing technologies such as airborne thermal or satellite-based IR platforms, which have their own advantages and drawbacks (the primary advantage is very high spatial resolution at the cost of very low temporal resolution).

![image](https://user-images.githubusercontent.com/5757591/149609589-c5b4aaff-603e-43e8-a5c4-e75594ccfc80.png)

Figure 1 - NVDI Infrared image of a farm ([CC-BY-SA-3.0 Attr](https://commons.wikimedia.org/wiki/File:SUAS_StardustII_Ndvi_sml.jpg))

Due to these difficulties, field-deployable day-to-month duration fine-grained temperature data collection has been uneconomical, and thus infrequently practiced for all except the most well-funded research group. Calor's mission is to change this.

The target market for Calor are individuals and organizations who meet a majority of the following criteria:

__Technical Criteria__
* Collect temperature data over time (> 1 hour)
* Collect temperature data over medium to large areas (> 100 sq feet)
* Handle hostile environmental conditions (submersion, freezing, direct sunlight)

Furthermore, to make the product usable by a larger target market:

__Economic Criteria__
* Low hardware costs (< $10 per sensor)
* Low deployment costs (< 30 minutes to deploy or collect 100 sensors)
* Easy visualization of data (interpolation, animation and and overlay onto a map)
* Easy retrieval (guiding to location of next placed sensor)

This translates to the following identified target markets:

* Environmental researchers, who need to place many sensors across a region of interest (usually in a grid or linear feature) to obtain data that will be correlated with other observations, such as plant health.
* Farmers, who need to gain an understanding of temperature trends and extremes over time across a large field or greenhouse.
* Health and safety officers, who want to identify temperature extremes across a factory floor or other large facility

It is anticipated that additional target markets will emerge when the above described economic criteria can be met.

# Requirements

Calor has the following requirements:

* As a user, I want to deploy sensors in wet, cold, hot and dirty environments without them failing, so I can successfully collect data.
  * Sensors shall be able to survive a wide range of temperatures (-20 to 50 C)
  * Sensors shall be able to survive direct sunlight for extended periods of time (months)
  * Sensors shall be able to survive liquid immersion (NEMA 6P)
  * Sensors shall be able to survive freezing

* As a user, I want to collect data at 1 - 2 minute per sample, for durations up to months, so I have sufficient data for analysis.
  * Sensors must power down when not collecting data
  * Sensors must have sufficient stored energy to collect the desired number of samples
  * Sensors must store collected data in a non-volatile storage medium
  * Sensors must be time synchronized such that sensor-to-sensor sample rate drift is minimized and can be effectively removed through post-processing

* As a user, I want the sensors to last a long time, so that their lifetime cost and environmental impact is minimized.
  * Sensors shall have a lifespan measured in years
  * Sensors shall minimize and eliminate use of toxic materials

* As a user, I want to quickly recharge sensors, so they can quickly be re-deployed.
  * Sensors shall be able to be bulk charged in parallel
  * Sensor charging minimizes handling time
  * Sensor charge state shall be easily visible

* As a user, I want to keep track of where sensors are placed, so I can find them again.
  * When a sensor is placed into the environment, when it is activated, the GPS location of the sensor is recorded
  * When a sensor is to be collected, the GPS location is used to indicate the location of the sensor

* As a user, I want to easily upload and process the sensor data, so I can quickly see results.
  * When sensors are collected, data should be transmitted wirelessly to avoid fiddling with cables and ports
  * When sensors are collected, the temperature time series data is associated with the GPS location of the sensor
  * When sensors are collected, the locations and temperature time series data can be overlaid on a map

# User's Guide

Thank you for purchasing Calor temperature sensors. While Calor has been designed to be easy to use and deploy, you will find it easier to perform a production deployment if you have first had practice with a trial deployment first. This guide will walk you through the steps involved.

1. Unpack your sensors

Each sensor ships in an egg carton. While sensors are rugged and difficult to damage, they should be visually inspected before use. A cracked or abraided sensor should not be deployed, and a spare or replacement should be used instead.

TODO: Photo of sensors in packaging

Sensors are completely interchangable, and do not need to be individually kept track of.

2. Unpack your charger

Calor sensors are wirelessly charged. The charger can be placed on any non-conductive flat surface, and requires 2 amps of 12 VDC.

TODO: Photo of charging plate with supplied power adapter

Tip: Charging works best when their are no matellic objects within three feet of the charger.

3. Charge your sensors

Connect power to the charger. The LEDs around the edge will illuminate green to indicate that power is available for charging.

Place your sensors upright on top of the charging plate. The LED at the top will blink to indicate that charging is underway, and to indicate the current level of charge.

* Blinking red indicating low charge
* Blinking yellow indicating medium charge
* Blinking green indicates high charge
* Solid green indicates that the sensor is fully charged

4. Place your sensors

The first step to capturing temperature data in the field is to install the Calor App. When loaded, the app will present you with three options:

* New Session
* In Progress Sessions
* Completed Sessions

TODO: Add screen shot of app main screen

Select "New Session", and give the session a name.

TODO: Add screen shot of new session add name

Make sure you have your smart phone fully charged before you go out into the field.

Sensors should be placed in a carrying bag for deployment. We recommend [Bushpro Tree Planting Bags](https://www.forestry-suppliers.com/product_pages/products.php?mi=16020) from Forestry Suppliers, Inc.

When you are at the location were you want to place the sensor, select "In Progress Sessions", and select the session you named earlier.

TODO: Add screen shot of in progress session selection

Select "Drop a Sensor", and wait for the screen to go from red to yellow.

TODO: Add screen shot of drop a sensor screen

When the screen goes yellow, you will see the GPS coordinates of your current location, and you can take a sensor out of your bags and hold the top (rounded end) of the sensor up to the phone camera. The phone will activate the sensor, and record the time and serial number of the sensor. When this data is received, the phone will beep, the screen will change to green, and you will be able to capture a photo of the sensor in-situ if desired, then will return you to the selected session, ready for you to drop the next sensor.

TODO: Add screen shots of sensor drop process

You will be able to rapidly deploy sensors by using your thumb to navigate through the screens while you simultaniously place sensors using your dominant hand. With practice, deploying a sensor should take under 30 seconds.

TODO: Add video of rapid placement

Sensors can be added to a session at any time, but the best data is obtained when all of the sensors are placed together.

5. Collect your sensors

Sensors can be collected by selecting "In Progress Sessions", then selecting the session you named earlier, then selecting, "Collect Sensors".

The application will use your current GPS coordinates and the order in which you placed the sensors to guide you from sensor to sensor, showing you a live map with the position of each sensor, and a picture of the next sensor to collect when you have arrived at the location where the sensor is placed. You can choose to proceed in the original order or reverse order.

TODO: Add screen shot of collect sensor screen

When you have picked up the sensor, tap on the photo, and when the screen goes yellow, hold the top of the sensor up to the phone camera. The phone will beep and the screen will go green once the sensor data has been read, then the sensor will be deactivated.

Once all sensors have been collected (or if there are sensors missing), complete your session by selecting "Complete Session".

6. Visualize your data

Once all of the sensors have been collected, data can be visualized by selecting "Completed Sessions". Here you can see a heatmap of the sensor data on top of Google Street Maps or satellite maps, use a slider to see how temperature values change over time, and see charts of individual times.

TODO: Add screen shot of data visualizations

Raw sensor data, charts, maps and animations can be saved or shared via e-mail and instant messaging.

7. WHat happens when something goes wrong?

If a sensor doesn't respond when dropping a sensor, it may be defective. Put it away in a seperate pouch to check later.

If a sensor doesn't respond when collected, it may have become completely discharged. Holding your phone against the sensor for 1 minute should provide enough of a charge to allow the data to be read. If it still doesn't respond, put it away in a seperate pouch, and use the charger to recharge the sensor.

If a sensor doesn't blink when charging, it is likely defective, and should be recycled as e-waste.

# Design

## System Components

![image](https://user-images.githubusercontent.com/5757591/149612079-d7420201-e9ac-45f7-be58-dffd8b8d6fd5.png)

Calor consists of the following components:

Charger - Converts DC into a magnetic resonant coupling to induce current into nearby sensor devices to recharge the super capacitor.

Sensor - Sealed device containing a charging subsystem, a power storage and management subsystem, and a microprocessor subsystem, with an optical communication interface, and internal test pads for test and development.

Phone - A standard smartphone running a custom app that manages and visualizes data collected by the sensors.

Test System - A harness for testing and programming the sensor before it is encapsulated in resin.

> ⚠️ Only the microprocessor subsystem and the test system is in-scope for the class. Other components are being developed on a "best-effort" basis.


## Hardware Subsystems

https://user-images.githubusercontent.com/5757591/148632269-927e1d28-1843-4b77-b5be-181597f47de9.png![image](https://user-images.githubusercontent.com/5757591/149612333-b4f05702-f7a3-458e-999d-a4ef55c3d2ff.png)

__Charging Subsystem__

The charging subsystem contains the following components:
* Magnetic resonant coupling coil: Receives power from the charger, and to a lesser extent, from the smartphone NFC subsystem
* Rectifier: Transforms the AC voltage into a DC voltage

TODO: Board design and BOM

__Power Subsystem__

The power subsystem contains the following components:
* Charging Protection: A zener diode that prevents the voltage from the charging subsystem from exceeding the rated value of the linear regulator
* Charging Regulator: A linear regulator that steps down the charging voltage to a lower voltage suitable for charging the super capacitor
* Super capacitor: Stores charge to run the sensor while deployed
* Super capacitor protection: A zener diode that prevents the charging voltage from exceeding the rating of the super capacitor.
* Sense rails: Voltage monitoring to allow software measurement of the charging and super capacitor rails
* Boost converter: Supplies a regulated voltage for the microprocessor
* Power-down and Wakeup delay: Low-power logic that when activated, powers down the boost converter and waits for a period of time before powering it back up. This component is also activated when indictive power transfer is detected.

![image](https://user-images.githubusercontent.com/5757591/149612768-e282e87c-e3b5-48d2-8977-7c01a3bf3f5d.png)

Figure N - PCB Design

![image](https://user-images.githubusercontent.com/5757591/149612782-4a3a35d2-f76c-4d40-9bb3-8139e03a6e8c.png)

Figure N - PCB Rendering

Bill of Materials

```
U1			2.5v regulator			MIC5209-2.5BSTR
U2			Boost converter			TPS61010DGSR
U3			Schmitt Trigger Inverter	74LVC1G14GW		https://www.digikey.ca/en/products/detail/nexperia-usa-inc/74LVC1G14GW-Q1001/3679038
L1			10 mH inductor			ASPI-0602S-100M-T	https://www.digikey.ca/en/products/detail/abracon-llc/ASPI-0602S-100M-T/2343244
C1, C2			6.3V 10uF 20% Tant		TAJB106M006R	
R1			10 Ohm 1/2 Watt			RC1206FR-7W10RL
CN2, CN3		3 pin SMT angle header		TSM-103-02-L-SV		https://www.digikey.ca/en/products/detail/samtec-inc/TSM-103-02-L-SV/6679010
```

__Microprocessor Subsystem__

The microprocessor subsystem contains the following components:
* Microprocessor: A low-power ARM CORTEX-M microprocessor that runs the firmware for the sensor
* Flash Memory: QSPI Flash used to store the firmware, the sensor serial number, and temperature sensor readings
* Temperature Sensor: A I2C high-precision temperature sensor used to measure the temperature. Since the entire system is off 99% of the time, the temperature rise of the power and microprocessor subsystem does not affect readings.
* ADC I/O: Two ADC channels used to measure the inductive power charging rail and the super capacitor power storage rail
* SWD I/O: A debug port used for programming and testing
* UART I/O: A debug port used for testing
* RGB Smart LED: A smart LED used to indicate sensor status and to transfer the sensor serial number and stored data

![image](https://user-images.githubusercontent.com/5757591/149612875-07ddd5e6-81d5-46aa-b4a8-495df4d1ac63.png)

Figure N - PCB Design

![image](https://user-images.githubusercontent.com/5757591/149612896-923925ec-66c8-4d7a-bbff-9ae9d39783da.png)

Figure N - PCB Rendering

Bill of Materials

```
U1			Microcontroller			RP2040TR13		https://www.digikey.ca/en/products/detail/raspberry-pi/SC0914-13/14306010
U2			Flash Memory			W25Q16JVZPIQ	https://www.digikey.ca/en/products/detail/winbond-electronics/W25Q16JVZPIQ-TR/6193781
U3			Temperature Sensor		TMP117AIDRVR	https://www.digikey.ca/en/products/detail/texas-instruments/TMP117AIDRVR/9685284
U4			Temperature Sensor		LM75CIMX-3		https://www.digikey.ca/en/products/detail/texas-instruments/LM75CIMX-3-NOPB/2043437
X1			12 MHz oscillator		COM1305-12.000-EXT-T-TR  https://www.digikey.ca/en/products/detail/raltron-electronics/COM1305-12-000-EXT-T-TR/10272064
D1			RGB LED				WS2812C			
C1 - C3		1 uf thin film capacitor	
C4 - C16		100 nf thin film capacitor	
R1			10K Resistor			
R2, R3		4.7K I2C Pull-up
```

## Software Subsystems

### Firmware

### Smartphone Application

# Implementation

## Power Subsystem

### Charging

### Power Storage

### Power Regulation

### Power Monitoring

### Overvoltage Protection

### Power Analysis

## Microcontroller Subsystem

### Microcontroller

### Flash Storage

### Temperature Sensor

### Optical Signalling

## Software

### State Management

### Charging

### Activation (placement)

### Acquisition

### Transfer (collection)

### Analysis

### Software Dependencies

# Design for Manufacturing

## Component Selection

## Packaging

## Assembly

## Test

## Firmware Update

# Build Instructions

## Hardware

## Software

# Futures

# Resources

# Grading