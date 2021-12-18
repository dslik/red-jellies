## Exercise 4: Make Blinky

On your final project board, make blinky for yourself. Then add a button to turn the LED on and off. Bonus points for making the button cause an interrupt. Triple bonus points for debouncing the button signal.

## Assignment Submission

The included source code runs on a Raspberry Pi Pico mounted on a custom carrier PCB that I was using for a different project.

It blinks the LED once per second until the switch is pressed, with the LED remaining on while the switch is held down.

A combination of an interrupt service routine and a timer callback is used to debounce the button.

![image](https://user-images.githubusercontent.com/5757591/146642191-5163ada7-58d9-4fa1-9e3b-800b77d27a5e.png)

Here we see the button bouncing all over the place (yellow trace), and the clean debounced output (red trace). The delay between the first rising edge of the yellow trace and the rising edge of the red trace is the debounce delay (20 ms).

## Exercise 4 Questions:


