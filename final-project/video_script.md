# Final Project Video Script

## Preparation

1. Clear the flash
2. Reset the program
3. Make sure pot is turned all the way to the clockwise
4. Clear the scope (set trigger to "Auto", then "Normal")
5. Get OBS ready (scene selector active)

## Script

Hello, and welcome to my final project video for the Classpert "Making Embedded Systems" class.

My final project is "Calor", a low-cost outdoor temperature logger. This video focuses on the software aspects of Calor.

As you can see on my right, (gesture to right) Calor is prototyped on a breadboard, using a Raspberry Pi RP2040 board I was working on for another project.

(switch to board image scene)

Let's take a closer a closer look.

The RP2040 (on a Pi Pico) is connected to an I2C temperature sensor. A simple voltage divider allows me to simulating the charging voltage.

Finally, an WS2812 smart LED allows me to generate different colours of light.

Since this class is focused on embedded systems, let's look at how these components work together.

(switch to scope scene)

In order to log the temperature, we need to read data from the temperature sensor. This is done via a protocol known as I squared C (I2C).

(trigger a temperature sample)

Here's what the I2C waveform looks like. The yellow trace is the I2C clock, and the red trace is the I2C data, with the protocol analysis on the bottom.

0xEF is the request to read a register, and 0x17 and 0x80 are the two's complement temperature value.

When processed, we see that the temperature is 23.5 degrees Celsius.

Calor has a UART that provides access to debugging output, and implements a simple command-line interpreter.

Let's take a look at the commands available:

(type help)

Calor allows us to check the supply voltages:

(type voltage supply)

And the charging voltage.

(Type voltage charge)

These are measured using the on-chip ADC.

We'll get back to this in few minutes.

Since Calor's primary purpose is to log temperatures, we need a way to store temperatures over time. This is done by programming them into flash.

Each time a new temperature is read, it is stored into a previously erased flash page.

Let's use our debugging button to store some samples:

(Press switch)

I'll put my finger on the temperature sensor so we see the temperature changing.

Now we can take a look at how many records we have stored:

(Enter "temp records count")

And we can dump the stored records:

(Enter "temp records print")

But one of the innovative aspects of Calor is that you don't have to connect any wires to read the temperatures. It's done optically.

(enter "temp optical print")

Let's take a closer look at what happened. But first, a warning that there will be flashing lights.

(switch to video scene)

This is recorded from an iPhone, which is what would be used to capture the temperature data.

The different colours are distinguishable from each other, and encode the temperature data.

So let's move on to the state machine.

(switch back to console)

(clear screen)

Calor is designed to spend most of its time off.

When it wakes up, it checks the charging voltage. Depending on the charging voltage it does things differently.

Let's demonstrate this:

(type "temp records clear")

(type "simulate")

Since the charging voltage is zero (as it would be when it's out in the field), Calor collects temperature values.

Let's increase the voltage to simulate a NFC phone being placed near the device:

Now it is transferring the stored data.

And if we increase the voltage again to simulate charging...

We see that it detects this, and clears the stored data, and is ready to deploy out in the field again.

That concludes my presentation. Thank you for watching.
