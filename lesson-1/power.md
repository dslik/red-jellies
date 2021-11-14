# Preliminary Power Budget Analysis

Assuming the following:

* Two seconds to capture and store a temperature value
* µP and peripherals consume 15 mA when running

Each sample requires (15 / 3600 x 2) mAh, or 0.00833 mAh

Assume 30 samples per hour (720 per day), that’s 6 mAh per day.

To run for a month, that would require 180 mAh.

For reference, a CR2032 is 240 mAh, but this would make the unit either disposable or difficult to protect from the elements, and will prevent use in lower temperatures.

# Supercapacitor Options

Two supercapacitors that I have on hand are:

## Maxwell PC223

Given efficiencies and discharge curves, this super capacitor has approximately 4 mAh of usable power. This is far too low for this use case.

## Maxwell BCAP0150 P270 T07

Data Sheet: https://www.mouser.ca/datasheet/2/257/Maxwell_HCSeries_DS_1013793-9-1179788.pdf

Given efficiencies and discharge curves, this super capacitor has approximately 100 mAh of usable power.

This is in the ballpark of being feasible, and if the current draw per sample could be reduced by 50%, this would achieve the stated criteria.

One problem is that this capacitor is a little on the large side, and will thus have significant thermal mass, which will affect the responsiveness of temperature readings.
