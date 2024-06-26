<b>STM32H745ZI_M7</b>

This project aims to bring up the M7 core of a STM32H745ZI dual core microcontroller, the other (M4) core is covered in my STM32H745ZI_M4 repo. The goal is to set up the chip from power-on boot working mostly at register level, and bring it to the max clock rate with several peripherals running in bare metal mode, and finally to enable the two cores to talk to each other.

This project references the Common project in the M4 repo for code that is used by both M7 and M4 cores.

Toolchain is Visual Studio with the VisualGDB plugin for talking to the microcontroller. The target hardware is a Nucleo-H645ZI-Q development board.

This is also my first time using GitHub, so that will be a fun learning process too.
