/*
 * MCP23017.h
 *This library is for MCP23017 I2C gpio expander
 * Author: Sharath H S
 */ 


#ifndef MCP23017_H_
#define MCP23017_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif

#include <Wire.h>

typedef enum {A,B} port_name;
typedef enum {pin0,pin1,pin2,pin3,pin4,pin5,pin6,pin7}pin_no;
typedef enum {enable,disable} status;
		  
class MCP23017 {

public:
  MCP23017(int MCP23017_addr);
  
/*
this is used to set the port or pin as input or as output
portname = A/B
port_state / pin_state =INPUT/OUTPUT
pinx = pin0/pin1/pin2/pin3/pin4/pin5/pin6/pin7
*/  
  void PortMode(port_name portname,uint8_t port_state);
  void PinMode(port_name portname,pin_no pinx,uint8_t pin_state);

/*
this is used to write to the port or pin for active high or active low
portname = A/B
port_logic / pin_logic = HIGH/LOW
pinx = pin0/pin1/pin2/pin3/pin4/pin5/pin6/pin7
*/
  void PortWrite(port_name portname,uint8_t port_logic);
  void PinWrite(port_name portname,pin_no pinx,uint8_t pin_logic);

/*
this is used to read the port or pin for active high or active low
portname = A/B
pinx = pin0/pin1/pin2/pin3/pin4/pin5/pin6/pin7

PortRead returns in decimal when typecast to binary corresponding bit place is 1 or 0 indicates high or low respectively 
           where LSB is pin0 and MSB is pin7 of respective port
PinRead returns 1 or 0 indicating high or low respectively 
*/  
  uint8_t PortRead(port_name portname);
  uint8_t PinRead(port_name portname,pin_no pinx);
 
 /*
this is used to enable or disable the port or pin's pull up resistors
port = A/B
pinx = pin0/pin1/pin2/pin3/pin4/pin5/pin6/pin7
state = enable,disable

 */
  void PullUpPort(port_name port,status state);
  void PullUpPin(port_name port,pin_no pinx,status state);
 
 
 /**************Interrupts***************/
 /* 
   if  state=enable:INTA and INTB pins are internally connected 
	   state=disable:The INT pins are not connected. INTA is associated with PortA and INTB is associated with PortB
*/

void SetInterruptPinMirroring(status state);

/*		
		INT pin = INTA or INTB
					
		opendrain=enable:On interrupt INT pin will send active high signal(default when their is no interrupt INT pin will have active low)
		opendrain=disable:depends on interrupt_polarity given by user
	
	only when opendrain=disable:
		interrupt_polarity=HIGH:On interrupt INT pin will send active high signal(default when their is no interrupt INT pin will have active low) 
		interrupt_polarity=LOW:On interrupt INT pin will send active low signal(default when their is no interrupt INT pin will have active high)
 */
  
  
  void SetInterruptPinA(status opendrain,uint8_t interrupt_polarity);
  void SetInterruptPinB(status opendrain,uint8_t interrupt_polarity);

/*
This is used to decide when interrupt should occur corresponding to gpio pins of A and B port 

different modes are :change,falling and rising
if mode=CHANGE:Corresponding pin value is compared with previous value,therefore whenever the logic changes from high to low or low to high interrupt is triggered
   mode=FALLING:If corresponding pin value goes to low interrupt is triggered
   mode=RISING:If corresponding pin value goes to high interrupt is triggered
*/
  void SetInterruptEventPin(port_name port,pin_no pin,uint8_t mode);

/**********NOTE*********/
//reading the interrupt(using the below two functions) will cause the interrupt condition to finish

/*
gets the pin which has caused the interrupt
for port A:0-7 is the return value(0->pin0.....7->pin7)
    port B:8-15 is the return value(0->pin8.....7->pin15)
If no pin has not caused any interrupt then it returns 200 as a garbage value
*/
uint8_t getInterruptPin();

/*
gets the value of pin which has caused the interrupt
If no pin has not caused any interrupt then it returns 200 as a garbage value
returns 0 or 1
*/
uint8_t ReadInterruptPinValue();

 
private:
  int _addr;
  
  void wiresend(uint8_t x);
  
  uint8_t wireread(void);
  
  void reg_write (uint8_t reg,uint8_t data);
  
  uint8_t status_of_reg(uint8_t reg);
  
  uint8_t pin_assign(uint8_t pinx);
};



#endif /* MCP23017_H_ */