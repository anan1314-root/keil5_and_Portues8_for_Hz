#ifndef __T1_NIXIE_H__
#define __T1_NIXIE_H__

void T0_Init(void);	// 定时器0初始化
void N_T(unsigned char loc, unsigned char num);	// 数码管显示
// 全部熄灭
void N_AllOff(void);

#endif
