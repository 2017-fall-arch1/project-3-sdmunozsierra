# Project 3: Falling Balls

**NOTE: This repository has changed the location of the original includes
for the msp430 libraries to work in my personal Linux distribution (Arch).
This will probably not work for the ones running Fedora VM from class 
(not tested), but certainly this can be run in any Linux distro by installing
official msp430 libraries from ti at `/opt/ti`. You have been warned **
** ALSO you need gcc 7.2.0 or change the dependency from moving-demo assembly
file **

## Introduction

Continuing your effort to help ACME Solutions provide Christmas toys, 
your task is now to develop a game for the LCD screen.  You should 
utilize the knowledge you havegained working with the buttons and 
buzzer.  In addition, a series of libraries have been found to assist
you in creating your game.

Students can propose alternate projects that demonstrate
similar skills.  These proposals must approved by the course
instructor in writing (e.g. email) at least two weeks before the
project due date. 

## Requirements
__How to install and play__
1. Download or clone the repo to local machine
2. In root directory do `make install`
3. `cd` to myDemo
4. `make load2`
5. Play!

_note: this game has changed the location of the mps430 libraries
and it might not work under the provided local machine. See note
at the beggining of readme._


## Requirements
Your game should meet the following minimum criteria:

[x]- dynamically render graphical elements that move
[x]- handle colisions properly with other shapes and area boundaries
[x]- produce sounds triggered by game events
[x] communicate with the player using text
[x] include a state machine written in assembly language
[x] respond to user input (from buttons)

