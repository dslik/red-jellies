# Lesson 10 Assignment

## Power Consumption

The worksheet describing Wordy’s power use is demonstrated in the lecture. Make one for your system (either design or engineering version).

Estimates are ok in place of real numbers. If you have many components, limit it to the top 5-8 most likely to consume power. Even if you have a USB or wall powered system, identify the largest consumers of power and how you might reduce power consumption.

# Assignment Submission

TODO: Write test program that meaures power consumption of some of the different techniques outlined below.

## Power consumption by the RP2040

Approaches to reducing power consumption:

* Decrease clock frequency
* Use low power sleep mode
* Put processor 2 in low power wait state
* Use the internal oscillator (vs. external oscillator)
* Memory can be powered down
* Disable USB
* Reduce power consumed by external devices (e.g. LEDs)

## Resources

* https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
* https://raspberrypi.github.io/pico-sdk-doxygen/examples_page.html
