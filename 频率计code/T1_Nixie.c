#include <REGX52.H>
#include "T1_Nixie.h"//anan da wang smg han shu
// 数码管段选表(共阴极)，0-F
unsigned char DS_T[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
unsigned char N_Buf[8] = {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};  																		// 显示缓存，默认全灭
unsigned char N_Wei  =  0;     																																					// 当前扫描位


// 定时器0初始化
void T0_Init(void)
{
	TMOD &= 0xF0;		// 清空di4位
	TMOD |= 0x01;		// 定时器0，模式1
	TL0 = 0x66;			// 定时1ms初值
	TH0 = 0xFC;			// 定时1ms初值
	TF0 = 0;			  // 清标志
	TR0 = 1;				// 启动定时器
	ET0 = 1;				// 开中断
	EA = 1;					// 开总中断
	PT0 = 0;				// 低优先级
}

void N_T(unsigned char loc,unsigned char num)
{
	if(loc >= 1 && loc <= 8)  // 参数检查
	{
		N_Buf[loc-1] = num;  // 设置显示数字
	}
}

// 全部熄灭
void N_AllOff(void)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		N_Buf[i] = 0x10;  // 全部熄灭
	}
}

// 定时器0中断函数
void T0_Time() interrupt 1
{ 
	static unsigned int T0_Num = 0;	// 计数变量	
	TL0 = 0x66;											// 重装初值
	TH0 = 0xFC;											// 重装初值
	T0_Num++;
	if(T0_Num >= 1)  								// 1ms扫描
	{
		T0_Num = 0;
		
		P0 = 0x00;										// 消隐
		
		// 检查是否需要显示
		if(N_Buf[N_Wei] != 0x10)  // 不是熄灭标志
		{
			// 位选
			switch(N_Wei + 1)
			{
				case 1: P2_2=1; P2_3=1; P2_4=1; break;	// 第1位
				case 2: P2_2=0; P2_3=1; P2_4=1; break;	// 第2位
				case 3: P2_2=1; P2_3=0; P2_4=1; break;	// 第3位
				case 4: P2_2=0; P2_3=0; P2_4=1; break;	// 第4位
				case 5: P2_2=1; P2_3=1; P2_4=0; break;	// 第5位
				case 6: P2_2=0; P2_3=1; P2_4=0; break;	// 第6位
				case 7: P2_2=1; P2_3=0; P2_4=0; break;	// 第7位
				case 8: P2_2=0; P2_3=0; P2_4=0; break;	// 第8位
			}
			
			// 段选
			P0 = DS_T[N_Buf[N_Wei]];
		}
		
		// 更新扫描位
		N_Wei++;
		if(N_Wei >= 8)
		{
			N_Wei = 0;
		}
	}
}