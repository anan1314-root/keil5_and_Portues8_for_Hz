#include <REGX52.H>
#include <INTRINS.H>
#include "delay.h"

void Delay(unsigned int xms)	
{
	unsigned char data i, j;
	while(xms--)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}

