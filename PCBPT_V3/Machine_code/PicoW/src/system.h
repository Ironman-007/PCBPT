//
// Created by l1534 on 9/14/2023.
//

#ifndef __SYSTEM_H
#define __SYSTEM_H

extern void debug_info(char * output);
extern void debug_info(int output);

#define LIMIT_SW_1_PIN D22
#define LIMIT_SW_2_PIN D19
#define LIMIT_SW_3_PIN D18
#define LIMIT_SW_4_PIN D26
#define LIMIT_SW_5_PIN D17
#define LIMIT_SW_6_PIN D16

extern void pin_init(void);

#endif //PICOW_SYSTEM_H
