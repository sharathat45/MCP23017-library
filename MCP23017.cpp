#include "MCP23017.h"


MCP23017::MCP23017(int MCP23017_addr)
{
	_addr=MCP23017_addr;
}

//for Arduino compatibility
void MCP23017::wiresend(uint8_t x)
{
	#if ARDUINO >= 100
	Wire.write(x);
	#else
	Wire.send(x);
	#endif
}

uint8_t MCP23017::wireread(void)
{
	#if ARDUINO >= 100
	return Wire.read();
	#else
	return Wire.receive();
	#endif
}


//private functions
void MCP23017::reg_write (uint8_t reg,uint8_t data)
{
	Wire.begin();
	Wire.beginTransmission(_addr);
	wiresend(reg);
	wiresend(data);
	Wire.endTransmission();
}

uint8_t MCP23017::status_of_reg(uint8_t reg)
{
	Wire.begin();
	Wire.beginTransmission(_addr);
	wiresend(reg);
	Wire.endTransmission();
	Wire.requestFrom(_addr,1);
	return wireread();
}

uint8_t MCP23017::pin_assign(uint8_t pinx)
{
	switch(pinx)
	{
		case pin0: return 0x01;
		case pin1: return 0x02;
		case pin2: return 0x04;
		case pin3: return 0x08;
		case pin4: return 0x10;
		case pin5: return 0x20;
		case pin6: return 0x40;
		case pin7: return 0x80;
	}
}


/**************************************************************************************/
//public functions
uint8_t MCP23017::PortRead(port_name portname)
{
	switch(portname)
	{
		case A:                      //A
				reg_write(0x02,0x00);
		return  status_of_reg(0x12);       //GPIOA
		break;
		case B:                     //B
				reg_write(0x03,0x00);
		return  status_of_reg(0x13);     //GPIOB
		break;
	}
}

uint8_t MCP23017::PinRead(port_name portname,pin_no pinx)
{
if(portname==A)//IPOLA
{ reg_write(0x02,0x00); }
else//IPOLB
{ reg_write(0x03,0x00); }	
if(int(PortRead(portname) & pin_assign(pinx)) == int(pin_assign(pinx))) {return 1;}
else {return 0;}
 
 }
 

void MCP23017::PortMode(port_name portname,uint8_t port_state)
{
	switch(portname)
	{
		case A://IODIRA
	      if(port_state == OUTPUT){reg_write(0x00,0x00);}
          else{reg_write(0x00,0xFF);}
          break;
        case B://IODIRB
          if(port_state == OUTPUT){reg_write(0x01,0x00);}   
          else{reg_write(0x01,0xFF);}         
          break;
    }
 }

void MCP23017::PinMode(port_name portname,pin_no pinx,uint8_t pin_state)
{
  switch(portname)
  {
    case A://IODIRA
           if(pin_state == OUTPUT)
           {uint8_t temp1 = status_of_reg(0x00) & !pin_assign(pinx);
            reg_write(0x00,temp1);}   
           else
           {uint8_t temp2 =  status_of_reg(0x00) | pin_assign(pinx);
            reg_write(0x00,temp2);}         
           break;
    case B://IODIRB
        if(pin_state == OUTPUT)
           {uint8_t temp1 =  status_of_reg(0x01) & !pin_assign(pinx);
            reg_write(0x01,temp1);}   
           else
           {uint8_t temp2 = status_of_reg(0x01) | pin_assign(pinx);
            reg_write(0x01,temp2);}         
           break;
    }
 }    

 
void MCP23017::PortWrite(port_name portname,uint8_t port_logic)
{
  switch(portname)
  {
    case A://GPIOA
           if(port_logic == HIGH){reg_write(0x12,0xFF);}   
           else{reg_write(0x12,0x00);}         
           break;
    case B://GPIOB
         if(port_logic == HIGH){reg_write(0x13,0xFF);}   
         else{reg_write(0x13,0x00);}         
         break;
    }
 }



 void MCP23017::PinWrite(port_name portname,pin_no pinx,uint8_t pin_logic)    
{
  switch(portname)
  {
    case A: //GPIOA
           if(pin_logic == HIGH)
           {uint8_t temp1 = status_of_reg(0x12) | pin_assign(pinx);
            reg_write(0x12,temp1);}   
           else
           {uint8_t temp2 =  status_of_reg(0x12) & !pin_assign(pinx);
            reg_write(0x12,temp2);}         
           break;
    case B://GPIOB
        if(pin_logic == HIGH)
           {uint8_t temp1 =  status_of_reg(0x13) | pin_assign(pinx);
            reg_write(0x13,temp1);}   
           else
           {uint8_t temp2 = status_of_reg(0x13) & ! pin_assign(pinx);
            reg_write(0x13,temp2);}         
           break;
    }
 }

 void MCP23017::PullUpPort(port_name port,status state)
 {
	if(port==A)   
	{
	  if(state==enable){reg_write(0x0C,0xFF);}//GPPUA
	  else             {reg_write(0x0C,0x00);}	
    }
	else
	{
	  if(state==enable){reg_write(0x0D,0xFF);}//GPPUB
	  else             {reg_write(0x0D,0x00);}
    }	 
 }
 
 void MCP23017::PullUpPin(port_name port,pin_no pinx,status state)
 {
	 if(port==A)  //GPPUA
	 {
	  if(state==enable)
	  {
	      uint8_t temp1 =  status_of_reg(0x0C) | pin_assign(pinx);
	      reg_write(0x0C,temp1);
      }
	  else
	  {
		  uint8_t temp1 =  status_of_reg(0x0C)  & ! pin_assign(pinx);
		  reg_write(0x0C,temp1);
	  }
	}
	else   ///GPPUB
	 {
		 if(state==enable)
		 {
			 uint8_t temp1 =  status_of_reg(0x0D) | pin_assign(pinx);
			 reg_write(0x0D,temp1);
		 }
		 else
		 {
			 uint8_t temp1 =  status_of_reg(0x0D)  & ! pin_assign(pinx);
			 reg_write(0x0D,temp1);
		 }
	 }
 }
 
  /* 
   if  state=enable:INTA and INTB pins are internally connected 
	   state=disable:The INT pins are not connected. INTA is associated with PortA and INTB is associated with PortB
*/
 
void MCP23017::SetInterruptPinMirroring(status state)
 {
	 uint8_t  IOCON_A= status_of_reg(0x0A);
	 uint8_t  IOCON_B= status_of_reg(0x0B);

 if(state==enable)
 {
	 bitWrite(IOCON_A,6,1);
	 bitWrite(IOCON_B,6,1);
 }
 else
 {
	 bitWrite(IOCON_A,6,0);
	 bitWrite(IOCON_B,6,0);
 }
 
 }
 
/*
		INT pin = INTA or INTB
		
		opendrain=enable:On interrupt INT pin will send active high signal(default when their is no interrupt INT pin will have active low)
		opendrain=disable:depends on interrupt_polarity given by user
		
		only when opendrain=disable:
		interrupt_polarity=HIGH:On interrupt INT pin will send active high signal(default when their is no interrupt INT pin will have active low)
		interrupt_polarity=LOW:On interrupt INT pin will send active low signal(default when their is no interrupt INT pin will have active high)
*/
 
void MCP23017::SetInterruptPinA(status opendrain,uint8_t interrupt_polarity)
{
	uint8_t  IOCON_A= status_of_reg(0x0A);
	
	if(opendrain==enable){bitWrite(IOCON_A,2,1);}
	else
	{
		bitWrite(IOCON_A,2,0);
		
		if(interrupt_polarity==HIGH) {bitWrite(IOCON_A,1,1);}
		else  {bitWrite(IOCON_A,1,0);}
		
	}
	reg_write(0x0A,IOCON_A);
}
 
void MCP23017::SetInterruptPinB(status opendrain,uint8_t interrupt_polarity)
{
	uint8_t  IOCON_B= status_of_reg(0x0B);
    
	if(opendrain==enable)	{bitWrite(IOCON_B,2,1);}
	else
	{
		bitWrite(IOCON_B,2,0);
		if(interrupt_polarity==HIGH){bitWrite(IOCON_B,1,1);}
		else{bitWrite(IOCON_B,1,0);}
	}
	reg_write(0x0B,IOCON_B);
	
}
 

/*
different modes are :change,falling and rising
if mode=CHANGE:Corresponding pin value is compared with previous value,therefore whenever the logic changes from high to low or low to high interrupt is triggered
mode=FALLING:If corresponding pin value goes to low interrupt is triggered
mode=RISING:If corresponding pin value goes to high interrupt is triggered
 */

void MCP23017::SetInterruptEventPin(port_name port,pin_no pin,uint8_t mode)
{
	uint8_t GPINTEN,DEFVAL,INTCON;
if(port==A)
{
	GPINTEN=status_of_reg(0x04);
	DEFVAL=status_of_reg(0x06);
	INTCON=status_of_reg(0x08);
}
else
{
	GPINTEN=status_of_reg(0x05);
	DEFVAL=status_of_reg(0x07);
	INTCON=status_of_reg(0x09);
}	
if(mode==CHANGE)
{
bitWrite(INTCON,pin,0);
bitWrite(GPINTEN,pin,1);
}
else if(mode==FALLING)
{
bitWrite(INTCON,pin,1);
bitWrite(GPINTEN,pin,1);	
bitWrite(DEFVAL,pin,1);
}
else if(mode==RISING)
{
bitWrite(INTCON,pin,1);
bitWrite(GPINTEN,pin,1);
bitWrite(DEFVAL,pin,0);	
}

if(port==A)
{
	reg_write(0x04,GPINTEN);
	reg_write(0x06,DEFVAL);
	reg_write(0x08,INTCON);
}
else
{
	reg_write(0x05,GPINTEN);
	reg_write(0x07,DEFVAL);
	reg_write(0x09,INTCON);
}

}

/*
gets the pin which has caused the interrupt
for port A:0-7 is the return value(0->pin0.....7->pin7)
    port B:8-15 is the return value(0->pin8.....7->pin15)
If no pin has not caused any interrupt then it returns 200 as a garbage value
*/
uint8_t MCP23017::getInterruptPin()
{
uint8_t temp=status_of_reg(0x0E);//port A   INTFA

for(int i=0;i<=7;i++) 
{
	if (bitRead(temp,i)==1)
	{ return i;}
}

temp=status_of_reg(0x0F);//port B         INTFB

for(int i=0;i<=7;i++) 
{
	if (bitRead(temp,i)==1)
	{return i+8;}
}

return 200;

}

/*
gets the value of pin which has caused the interrupt
If no pin has not caused any interrupt then it returns 200 as a garbage value
*/
uint8_t MCP23017::ReadInterruptPinValue()
{
  uint8_t interrupt_Pin=getInterruptPin();
  
  if(interrupt_Pin==200)
  {return 200;}
  else
  {
	uint8_t interrupt_Port;
	
	if(interrupt_Pin<=7)//port A
	{
	 interrupt_Port=status_of_reg(0x10);//INTCAPA
	 return bitRead(interrupt_Port,interrupt_Pin);
	}
	else  //port B
	{
	 interrupt_Port=status_of_reg(0x11);//INTCAPB
	 return bitRead(interrupt_Port,(interrupt_Pin-8));	
	}  	
  }
}