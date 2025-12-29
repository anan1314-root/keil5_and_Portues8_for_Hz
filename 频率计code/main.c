#include <REGX52.H>
#include <intrins.h>
#include "Nixie.h"
#include "delay.h"
sfr16 DPTR=0x82;
unsigned char cnt_t1 ,    qian,bai,shi,ge,bb,wan,shiwan,anan;
unsigned long freq, freq_temp;
void init()
{
    IE = 0x8a; 
    ET0 = 1;   
    TMOD = 0x15;

    TH1 = 0x3c;  //定时器T1定时50ms (50ms x 20 = 1s)
    TL1 = 0xc4;  //补偿20us的误差（Proteus8.10版本下12MHz晶振实际测量值）
    TR1 = 1;

    TH0 = 0x00;  //T0计数
    TL0 = 0x00;
    TR0 = 1;
}
void T1_timer() interrupt 3
{
    TH1 = 0x3c;  //定时器T1定时50ms (50ms x 20 = 1s)
    TL1 = 0xc4;  
    cnt_t1++;
    if(cnt_t1 >= 20) //(50ms x 20 = 1s)
    {
        cnt_t1 = 0 ;
        TR0 = 0 ;
        freq = freq_temp + ((unsigned int)TH0 << 8 | TL0); // 合成32位频率
        freq_temp = 0;
        TH0 = 0x00;  // 重新计数
        TL0 = 0x00;
        TR0 = 1;
    }
}
// T0溢出中断，累加高位
void T0_ISR() interrupt 1
{
    freq_temp += 65536UL;
}
void freq_to_digits(unsigned long f)
{
    shiwan = (f / 100000) % 10;
    wan    = (f / 10000)  % 10;
    qian   = (f / 1000)   % 10;
    bai    = (f / 100)    % 10;
    shi    = (f / 10)     % 10;
    ge     = f % 10;
}
void main()
{  
    cnt_t1 = qian = bai = shi = ge = bb = wan = shiwan = 0;
    freq = 0;
    freq_temp = 0;
    init();
    while(1)
    {
  
        freq_to_digits(freq);
        N_D(1, shiwan);
        N_D(2, wan);
        N_D(3, qian);
        N_D(4, bai);
        N_D(5, shi);
        N_D(6, ge);
        Delay(1); // 防闪烁
    }
}