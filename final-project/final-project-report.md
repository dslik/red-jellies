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
    * Data Acquisition
    * Data Storage
    * Data Transfer
    * Attribution
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
* Need to collect temperature data over time (> 1 hour)
* Need to collect temperature data over medium to large areas (> 100 sq feet)
* Need to be able to handle hostile environmental conditions (submersion, freezing, direct sunlight)

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


# User's Guide

# Design

## System Components

## Hardware Subsystems

## Software Subsystems

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

### Data Acquisition

### Data Storage

### Data Transfer

### Attribution

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
