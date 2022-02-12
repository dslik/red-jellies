# Lesson 10 Assignment

## Power Consumption

The worksheet describing Wordy’s power use is demonstrated in the lecture. Make one for your system (either design or engineering version).

Estimates are ok in place of real numbers. If you have many components, limit it to the top 5-8 most likely to consume power. Even if you have a USB or wall powered system, identify the largest consumers of power and how you might reduce power consumption.

# Assignment Submission

## Power Consumption Worksheet

Q. What are the different states the device can be in?

[Calor](https://github.com/dslik/red-jellies/blob/main/final-project/final-project-report.md) (my final project) primarily reduces power consumption by minimizing the amount of time that the processor and connected sensors are powered up.

Power States:
* Power off
* Power on, external power supply (fewer power constraints)
* Power on, power from supercapacitor

Sleep is not used, but rather, the entire microprocessor subsystem is powered down, with a hardware wakeup timer.

Q. How much will your device be in each state?

Power will be on for 1 - 2 seconds per 120 second interval. Optimization will be performed to minimize this time interval, since halving the interval doubles the length of time Calor devices can be deployed in the field.

Q. How much current is used in each state?

* Power off - 0 ma (+ power subsystem leakage, which I have not yet modelled or measured)
* Power on, external power supply - N/A (did not focus on this, since there are fewer restrictions
* Power on, power from supercapacitor - Approximately 10 ma with power consumption reduction measures enabled in code.

Q. How long will the device last given a 40mAh battery?

Assuming no leakage or self-discharge, the device will continue to operate for 20 days (1 second per 120 seconds runtime, 10 ma consumption).

In reality, leakage and self-discharge will significantly limit runtime..

## RP2040 Power Consumption Reduction

In order to get a good idea of what components would consume power, I used a combination of datasheet values and measured values.

| Component | Power consumption | Notes |
| --------- | ----------------- | ----- |
| RP2040 | 10 ma | Measured, with power reduction measures enabled |
| LED | 4ma | Measured |
| Temperature Sensor | 0.5 ms | From data sheet |
| Flash | N/A | Included in RP2040 measurement above |

## RP2040 Power Consumption Reduction

By reviewing the datasheet, I was able to identify the following approaches to reducing the power consumption of the RP2040 microprocessor.

* Decrease clock frequency
* Use low power sleep mode
* Put processor 2 in low power wait state
* Use the internal oscillator (vs. external oscillator)
* Memory can be powered down
* Disable USB
* Reduce power consumed by external devices (e.g. LEDs)

I wrote a quick test program to check some of these approaches, which is listed below.

I ran into the following challenges:
* The "sleep functions for delaying execution in a lower power state" did not result in a measurable reduction in power consumption.
* Deep sleep requires an external wakeup signal. Attempting to transition to deep sleep (sleep_run_from_xosc();) would leave the pico in an unstable state, and required a power cycle to resume.
* Processor 2 is already in low power wait state by default
* Internal oscillator is the "rosc". Switching over to this reduced power consumption, but prevented I/O, so additional work would be required here.

I was successfully able to half the power consumption by reducing the clock speed to 10 Mhz and reducing the internal voltage regulator. Turning off clocks had a minimal impact on power consumption. Reducing the internal voltage regulator had a significantly larger impact on power consumption for higher frequencies. 

![image](https://user-images.githubusercontent.com/5757591/153702435-521f7173-82f8-4277-ba9a-0e0c74ba0f77.png)

Power measurements were conducted using a Joulescope. Red lines represent maximum and minimum power consumption, and the yellow line represents the average power consumption for the measurement bin. Note that these measured numbers include power consumption for one LED, measured at 4 ma.

## Test Program Source

```c
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/rtc.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"
#include "hardware/structs/vreg_and_chip_reset.h"

#define LED_PIN     8

void led_delay(void)
{
    sleep_ms(1000);
    gpio_put(LED_PIN, 1);
    sleep_ms(1000);
    gpio_put(LED_PIN, 0);
    sleep_ms(2000);
}

int main() {

    // Initialize the RP2040 subsystems
    vreg_set_voltage(0b1011); // Default
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    printf("Starting Test\n");

    // Baseline measurements -----------------------
    printf("Baseline...\n");
    led_delay();
    
    // Reduce clock speed to 48 MHz ----------------
    printf("Change Clock to 48 MHz...\n");
    set_sys_clock_48mhz();
    sleep_ms(10);
    stdio_init_all();
    printf("Clock frequency changed\n");
    led_delay();

    // Turn off ADC Clock --------------------------
    printf("Turn off the ADC clock...\n");
    clock_stop(clk_adc);
    sleep_ms(10);
    stdio_init_all();
    printf("ADC clock off\n");
    led_delay();

    // Turn off USB Clock --------------------------
    printf("Turn off the USB clock...\n");
    clock_stop(clk_usb);
    // pll_deinit(pll_usb);
    sleep_ms(10);
    stdio_init_all();
    printf("USB clock off\n");
    led_delay();

    // Reduce clock speed to 10 MHz ----------------
    printf("Change Clock to 10 MHz...\n");
    set_sys_clock_khz(1000, true);
    sleep_ms(10);
    stdio_init_all();
    printf("Clock frequency changed\n");
    led_delay();

    // Reduce VREG voltage ----------------
    printf("Reduce core regulator voltage...\n");
    vreg_set_voltage(0b0111);
    sleep_ms(10);
    stdio_init_all();
    printf("Core voltage changed\n");
    led_delay();

/*    // Switch to interal oscillator --------------------------
    printf("Switching to internal oscillator...\n");
    clock_configure(clk_sys, CLOCKS_CLK_REF_CTRL_SRC_VALUE_ROSC_CLKSRC_PH, 0, 48 * MHZ, 48 * MHZ);
    //xosc_disable();
    sleep_ms(10);
    stdio_init_all();
    printf("Running on internal oscillator\n");
    led_delay();
*/

    // Fast blink to indicate final power state
    while (true) {
        sleep_ms(250);
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
    }
}
```

## Resources

* https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
* https://raspberrypi.github.io/pico-sdk-doxygen/examples_page.html
* https://github.com/raspberrypi/pico-playground/blob/master/sleep/hello_sleep/hello_sleep.c
* https://forums.raspberrypi.com/viewtopic.php?t=304283
* https://raspberrypi.github.io/pico-sdk-doxygen/vreg_8h.html
* https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__clocks.html
