# Nested Arithmetic Calculations
This project is a compiler developed in C++ that converts Nested RPN S-Expressions into Assembly. It has a built in method to display the results in 3 possible ways (Button, Timer e Display) when built into an Arduino UNO.

![image](https://user-images.githubusercontent.com/61120380/169628190-4e31e825-b363-4fdf-997d-64079a0115d5.png)
 <p align="center"> Our setup :) </p>
 
## Input
The input must follow the RPN S-Expressions format and must the following Methods:
|     Method     |      Parameters                 |            Usage                |
|----------------|---------------------------------|---------------------------------|
|Program         |     Output                      |   (Program _Output_)            |
|Output          |  Port, Output choice, Formula   |   (Output PD Display _Formula_) |
|Formula         |  _None_                         |   (3 (3 1 *) +)                 |

The nested RPN S-Expression cannot have two calculations in the same operator, meaning that you must always have a **Calculation** followed by a **Number** but you cannot have a **Calculation** followed by a **Calculation**.

Bellow are some examples of input:

`(Program (Output PC Botao (3 (4 (1.8 1.2 +) *) +)))` --- Results in 15.0

`(Program (Output PD Display (12.4 (3 (10.5 (5 2.3 +) -) *) /)))` --- Results in 1.3

`(Program (Output PB Temporizador (3 (1.2 (2 (12.4 (3 (10.5 (5 2.3 +) -) *) /) +) -) *)))` --- Results in -6.3

## Output
There are 3 types of output.
1. **Button**

		The button output works by showing the *result* binary form via LEDs.  There are 7 LEDs:
	``` mermaid
	flowchart LR 
	- --> 8  
	8 --> 4  
	4 --> 2
	2 --> 1
	1 --> .
	. --> Done
	
	style - fill:blue;
	style 8 fill:#d62929;
	style 4 fill:#d62929;
	style 2 fill:#d62929;
	style 1 fill:#d62929;
	style . fill:yellow;
	style Done fill:green;
	```
	The blue LED shows when the result is negative, the red LEDs show the result's binary form, the yellow LED represent the "." and finally the green LED shows when the ouput is over.
	
	So for instance, let us have the result be -15.3 . The sequence of LEDs displayed would be:
	
	``` mermaid
	flowchart LR 
	- --> 8  
	8 --> 4  
	4 --> 2
	2 --> 1
	1 --> .
	. --> Done
	style - fill:blue;
	```
	<p align="center"><strong>1 second</strong></p>
	
	``` mermaid
	flowchart LR 
	- --> 8  
	8 --> 4  
	4 --> 2
	2 --> 1
	1 --> .
	. --> Done
	style 1 fill:#d62929;
	```
	<p align="center"><strong>Push button</strong></p>
	
	``` mermaid
	flowchart LR 
	- --> 8  
	8 --> 4  
	4 --> 2
	2 --> 1
	1 --> .
	. --> Done
	style 4 fill:#d62929;
	style 1 fill:#d62929;
	```
	<p align="center"><strong>Push button</strong></p>
	
	``` mermaid
	flowchart LR 
	- --> 8  
	8 --> 4  
	4 --> 2
	2 --> 1
	1 --> .
	. --> Done
	style . fill:yellow;
	```
	<p align="center"><strong>Push button</strong></p>
	
	``` mermaid
	flowchart LR 
	- --> 8  
	8 --> 4  
	4 --> 2
	2 --> 1
	1 --> .
	. --> Done
	style 2 fill:#d62929;
	style 1 fill:#d62929;
	```
	<p align="center"><strong>Push button</strong></p>
	
	``` mermaid
	flowchart LR 
	- --> 8  
	8 --> 4  
	4 --> 2
	2 --> 1
	1 --> .
	. --> Done
	style Done fill:green;
	```
		
2. **Timer**

	The Timer output follows basically the same logic as the Button but instead of requiring the user to push the buttons so that the next number appears it does that automattically every 1 second.
	
3. **Display**

	The Display output is simply three 7-segment displays showing the result with the blue LED turning on when the result is negative.

  <br />
  
> **ATTENTION:**
> When chosing the desired output method be aware of the ports compatibility bellow:
> 
> **Button** : PB, PC <br />
> **Timer** : PB, PC <br />
> **Display** : PB, PD <br />

## Runing

To run this project you can simply download the repository and run locally in your computer. You will need to run the C++ code with your entries written in the *input.txt* file and then use a software like [Microship Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio) to build the generated ASM code and send it to your Arduino UNO.

> If you prefer you can directly use our C++ code via our [Replt.it](https://replit.com/@GiovanniPaiva/Tipos#main.cpp).



