# Lesson 7 Assignment

## Interview Questions

Each chapter in the book ends with an interview question followed by the analysis of what an interviewer would look for in an answer. Write one of these yourself.

Start with an interview question you have asked or one that you’ve been asked (or one from the internet). Then write up what an interviewer would look for in an answer. Be specific about how the interviewer might help the interviewee if they get stuck.

# Assignment Submission

## Interview Question

You have been asked to write code to control a rotary overhead crane.

![image](https://user-images.githubusercontent.com/5757591/150639708-250df14e-dd4c-4c5d-88ab-2dcc2544a854.png)

The crane has three degrees of freedom:
* Rotation angle (Bridge travel)
* Distance from pivot (Trolly travel)
* Lift (Hook travel)

What considerations would you need to take into account when moving items from one location on the work floor to another? How does this type of crane affect obstacle avoidance?

## What an interviewer would look for in an answer

Does the candidate immediately start thinking about the mechanics of crane motion, or do they first ask about why the crane is being controlled, and for what purpose? 

Does the candidate bring up safety aspects of such operation, especially given the hit about “obstacle avoidance”? Some examples of topics that need to be considered include:
* Safe handoff between when people are working around the crane (e.g. when attaching goods to the crane hook)
* Ensuring that the load will not swing into anyone or anything when first hoisted
* Ensuring that the route taken for moving loads across the floor will not collide with any objects
* Ensuring that there are no people under the load while being moved
* Ensuring that there is nothing under the load when lowering into place
* Do they bring up risks of deskilling?

Bonus answers (expected if the candidate has had experience working with cranes):
* Do they ask about tag lines, and how rotation, swinging and other uncontrolled motion will be avoided and minimized?
* Do they talk about safety lockouts, and how to return to a known safe configuration?
* Do they talk about measuring the load to prevent overloading?
* Does the candidate bring up the importance of smooth motion and acceleration?

Does the candidate recognize what types of calculations are required, and how this affects pathfinding? How do they propose to handle the coordinate system?

Does the candidate identify that precision and accuracy will be different at different areas of the work floor?

Does the candidate talk about control hardware and software failures? E.g. limit switches, ensuring safe stopping distances, maximum acceleration and travel speed (which are different for different areas of the floor)

## Hints for a stuck interviewee

* Let’s draw it out on a whiteboard. How would a load be moved from A to B?
* What sort of things could go wrong?
* Walk me through what you think happens when it’s controlled manually. What would need to change when computer-controlled? What would stay the same?
