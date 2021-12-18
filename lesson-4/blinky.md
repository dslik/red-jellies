## Exercise 4: Make Blinky

On your final project board, make blinky for yourself. Then add a button to turn the LED on and off. Bonus points for making the button cause an interrupt. Triple bonus points for debouncing the button signal.

## Assignment Submission

The included [source code](https://github.com/dslik/red-jellies/blob/main/lesson-4/debounce.c) runs on a Raspberry Pi Pico mounted on a custom carrier PCB that I was using for a different project. A GPIO line is connected to a button, and a second GPIO line is connected to an LED.

![image](https://user-images.githubusercontent.com/5757591/146642491-6fc7eec7-8c74-41b4-ad9d-6412d58b99cd.png)

The program blinks the LED once per second while the buttom is not pressed. When the button is pressed, the LED remains on.

A combination of an interrupt service routine and a timer callback is used to debounce the button.

![image](https://user-images.githubusercontent.com/5757591/146642191-5163ada7-58d9-4fa1-9e3b-800b77d27a5e.png)

Here we see the button bouncing all over the place (yellow trace), and the clean debounced output (red trace). The delay between the first rising edge of the yellow trace and the rising edge of the red trace is the debounce delay (20 ms).

Here is an example of debut output showing multiple interrupts resulting from button bouncing that are being ignored by the debouncing code.

![image](https://user-images.githubusercontent.com/5757591/146642327-3801d144-f6cb-4100-a1eb-99990b788528.png)

In this example, 0x8 is a rising edge, 0x4 is a falling edge, and 0xc is when both a rising and falling edge have occured.

## Exercise 4 Questions:

Q. What are the hardware registers that cause the LED to turn on and off? (From the processor manual, don’t worry about initialization.)

A. GPIO output is written to 0xd0000014

See Table 2.3.1.7. "List of Registers" in https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf

Q. What are the button registers that you read?

A. GPIO input is read from 0xd0000004

Q. Can you read that memory directly and see the button change in a debugger or by printing out the associated memory?

A. Yes. Using gdb, I can set a breakpoint, and inspect memory directly.

