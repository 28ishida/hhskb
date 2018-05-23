#include "left_firm.h"
#include <arduino.h>
#include <skMCP230.h>
#include <Wire.h>

#define LED1 GPB5
#define LED2 GPB6
#define LED3 GPB7

// デバイスの７ビットアドレス(0100A2A1A0)
#define MCP230_ADDRESS  0b0100000   // MCP23017のI2Cアドレス(A2=0,A1=0,A0=0)
skMCP230 expIO(MCP230_ADDRESS);        // MCP23017ライブラリの生成を行う

// 左手モジュールの初期化。一度だけ呼び出してください
void InitLeftFirm()
{
	// Ｉ２Ｃの初期化
	Wire.begin();                     // マスターとする
	// MCP23017の初期化(ピンは全て出力で初期化されます)
	expIO.Init();

	expIO.pinMode(GPA, GPA0, INPUT_PULLUP);
	expIO.pinMode(GPA, GPA1, INPUT_PULLUP);
	expIO.pinMode(GPA, GPA2, INPUT_PULLUP);
	expIO.pinMode(GPA, GPA3, INPUT_PULLUP);
	expIO.pinMode(GPA, GPA4, INPUT_PULLUP);
	expIO.pinMode(GPA, GPA5, INPUT_PULLUP);
	expIO.pinMode(GPA, GPA6, INPUT_PULLUP);

	expIO.pinMode(GPB, GPB0, OUTPUT);
	expIO.pinMode(GPB, GPB1, OUTPUT);
	expIO.pinMode(GPB, GPB2, OUTPUT);
	expIO.pinMode(GPB, GPB3, OUTPUT);
	expIO.pinMode(GPB, GPB4, OUTPUT);

	expIO.Write(GPA, GPB0, HIGH);
	expIO.Write(GPA, GPB1, HIGH);
	expIO.Write(GPA, GPB2, HIGH);
	expIO.Write(GPA, GPB3, HIGH);
	expIO.Write(GPA, GPB4, HIGH);

	expIO.Write(GPB, LED1, HIGH);
	expIO.Write(GPB, LED2, LOW);
	expIO.Write(GPB, LED3, LOW);
}

// キーのパース 引数は5x7の行列分の領域を割り当て済である必要があります。
int ParseLeftKey(char ans[][8])
{
	unsigned char readbuf;
	char writebuf[8];
	memset(writebuf, -1, 8);

	unsigned char writebuf3;

	// 左手パース
	//expIO.Write(GPB, GPB0, LOW);
	
	//writebuf[4] = HIGH;
	//writebuf[0] = LOW;
	//expIO.Write2(GPB, writebuf);

	// この値がベースになる
	writebuf3 = expIO.Read(GPB);

	writebuf3 |= (0x01 << 4);
	writebuf3 &= ~(0x01 << 0);
	expIO.Write3(GPB, writebuf3);

	readbuf = expIO.Read(GPA);
	ans[0][0] = (readbuf >> GPA0) & 0x01;
	ans[0][1] = (readbuf >> GPA1) & 0x01;
	ans[0][2] = (readbuf >> GPA2) & 0x01;
	ans[0][3] = (readbuf >> GPA3) & 0x01;
	ans[0][4] = (readbuf >> GPA4) & 0x01;
	ans[0][5] = (readbuf >> GPA5) & 0x01;
	ans[0][6] = (readbuf >> GPA6) & 0x01;
	
	//expIO.Write(GPB, GPB0, HIGH);
	//expIO.Write(GPB, GPB1, LOW);
	//writebuf[0] = HIGH;
	//writebuf[1] = LOW;
	//expIO.Write2(GPB, writebuf);

	writebuf3 |= (0x01 << 0);
	writebuf3 &= ~(0x01 << 1);
	expIO.Write3(GPB, writebuf3);

	readbuf = expIO.Read(GPA);
	ans[1][0] = (readbuf >> GPA0) & 0x01;
	ans[1][1] = (readbuf >> GPA1) & 0x01;
	ans[1][2] = (readbuf >> GPA2) & 0x01;
	ans[1][3] = (readbuf >> GPA3) & 0x01;
	ans[1][4] = (readbuf >> GPA4) & 0x01;
	ans[1][5] = (readbuf >> GPA5) & 0x01;
	ans[1][6] = 0;
	//expIO.Write(GPB, GPB1, HIGH);
	//expIO.Write(GPB, GPB2, LOW);
	//writebuf[0] = -1;
	//writebuf[1] = HIGH;
	//writebuf[2] = LOW;
	//expIO.Write2(GPB, writebuf);
	writebuf3 |= (0x01 << 1);
	writebuf3 &= ~(0x01 << 2);
	expIO.Write3(GPB, writebuf3);

	readbuf = expIO.Read(GPA);
	ans[2][0] = (readbuf >> GPA0) & 0x01;
	ans[2][1] = (readbuf >> GPA1) & 0x01;
	ans[2][2] = (readbuf >> GPA2) & 0x01;
	ans[2][3] = (readbuf >> GPA3) & 0x01;
	ans[2][4] = (readbuf >> GPA4) & 0x01;
	ans[2][5] = (readbuf >> GPA5) & 0x01;
	ans[2][6] = 0;
	//expIO.Write(GPB, GPB2, HIGH);
	//expIO.Write(GPB, GPB3, LOW);
	//writebuf[1] = -1;
	//writebuf[2] = HIGH;
	//writebuf[3] = LOW;
	//expIO.Write2(GPB, writebuf);
	writebuf3 |= (0x01 << 2);
	writebuf3 &= ~(0x01 << 3);
	expIO.Write3(GPB, writebuf3);

	readbuf = expIO.Read(GPA);
	ans[3][0] = (readbuf >> GPA0) & 0x01;
	ans[3][1] = (readbuf >> GPA1) & 0x01;
	ans[3][2] = (readbuf >> GPA2) & 0x01;
	ans[3][3] = (readbuf >> GPA3) & 0x01;
	ans[3][4] = (readbuf >> GPA4) & 0x01;
	ans[3][5] = (readbuf >> GPA5) & 0x01;
	ans[3][6] = 0;
	//expIO.Write(GPB, GPB3, HIGH);
	//expIO.Write(GPB, GPB4, LOW);
	//writebuf[2] = -1;
	//writebuf[3] = HIGH;
	//writebuf[4] = LOW;
	//expIO.Write2(GPB, writebuf);
	writebuf3 |= (0x01 << 3);
	writebuf3 &= ~(0x01 << 4);
	expIO.Write3(GPB, writebuf3);

	readbuf = expIO.Read(GPA);
	ans[4][0] = (readbuf >> GPA0) & 0x01;
	ans[4][1] = (readbuf >> GPA1) & 0x01;
	ans[4][2] = (readbuf >> GPA2) & 0x01;
	ans[4][3] = (readbuf >> GPA3) & 0x01;
	ans[4][4] = (readbuf >> GPA4) & 0x01;
	ans[4][5] = (readbuf >> GPA5) & 0x01;
	ans[4][6] = 0;
	//expIO.Write(GPB, GPB4, HIGH);
}

int TurnOnStatusLed(int ledNum)
{
	if (ledNum == 0)
	{
		expIO.Write(GPB, LED1, HIGH);
	}
	else if (ledNum == 1)
	{
		expIO.Write(GPB, LED2, HIGH);
	}
	else if (ledNum == 2)
	{
		expIO.Write(GPB, LED3, HIGH);
	}
}
int TurnOffStatusLed(int ledNum)
{
	if (ledNum == 0)
	{
		expIO.Write(GPB, LED1, LOW);
	}
	else if (ledNum == 1)
	{
		expIO.Write(GPB, LED2, LOW);
	}
	else if (ledNum == 2)
	{
		expIO.Write(GPB, LED3, LOW);
	}
}



