#ifndef _MCP23017_H_
#define _MCP23017_H_
#include <arduino.h>

// ピンの定数定義
#define GPA       0
#define GPA0      0
#define GPA1      1
#define GPA2      2
#define GPA3      3
#define GPA4      4
#define GPA5      5
#define GPA6      6
#define GPA7      7
#define GPB       1
#define GPB0      0
#define GPB1      1
#define GPB2      2
#define GPB3      3
#define GPB4      4
#define GPB5      5
#define GPB6      6
#define GPB7      7

void mcp23017_init();
void pinMode(int port,int pin,uint8_t mode);
void Write(int port,int pin,int value);
int Read(int port,int pin);
#endif
