# Minimech
This repository contains the firmware/software for a robotic toy 'Minimech', capable of being controlled via smart phone using bluetooth. It can also be prompted to speak phrases that have been entered by the user on startup. The product concept is outlined in the documents contained in `/Milestone_outlines`. As explained in these documents, the MCU required changing between Milestones 2 and 3.

All original code for the device is contained in the directory `/Project_code`. This software/firmware is built on top of the STM8S/A standard peripherals firmware library, described [here](https://documentation.help/STM8S-STM8A/index.html) and available [here](https://www.st.com/en/embedded-software/stsw-stm8069.html) (also included in the `/STM8S-SDCC-SPL` directory).

#### Hardware layout: 

<img align="middle" src="minimech_hardware_layout.png" alt="drawing1" width="700"/>

#### Software design: 

<img align="middle" src="minimech_software_design.png" alt="drawing1" width="700"/>

#### State machine: 

<img align="middle" src="minimech_state_machine.png" alt="drawing1" width="700"/>
