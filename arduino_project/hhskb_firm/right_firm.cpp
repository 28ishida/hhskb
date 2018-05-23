#include "right_firm.h"
#include <arduino.h>

// 右手側ピン番号の定義
#define OP1 4
#define OP2 5
#define OP3 6
#define OP4 7
#define OP5 8
#define OP6 9
#define OP7 10
#define OP8 11
#define IP1 A5
#define IP2 A4
#define IP3 A3
#define IP4 A2
#define IP5 A1

static void digitalWritePortRegister(unsigned int pin, unsigned int hOrl);
static char digitalReadPortRegister(unsigned int pin);

// 右手モジュールの初期化。一度だけ呼び出してください
void InitRightFirm()
{
	pinMode(OP1, INPUT_PULLUP);
	pinMode(OP2, INPUT_PULLUP);
	pinMode(OP3, INPUT_PULLUP);
	pinMode(OP4, INPUT_PULLUP);
	pinMode(OP5, INPUT_PULLUP);
	pinMode(OP6, INPUT_PULLUP);
	pinMode(OP7, INPUT_PULLUP);
	pinMode(OP8, INPUT_PULLUP);
	pinMode(IP1, OUTPUT);
	pinMode(IP2, OUTPUT);
	pinMode(IP3, OUTPUT);
	pinMode(IP4, OUTPUT);
	pinMode(IP5, OUTPUT);

	digitalWrite(IP1, HIGH);
	digitalWrite(IP2, HIGH);
	digitalWrite(IP3, HIGH);
	digitalWrite(IP4, HIGH);
	digitalWrite(IP5, HIGH);
}

// キーのパース 引数は5x8の行列分の領域を割り当て済である必要があります。
int ParseRightKey(char ans[][8])
{
	digitalWritePortRegister(IP1, LOW);
	ans[0][0] = digitalRead(OP1);
	ans[0][1] = digitalReadPortRegister(OP2);
	ans[0][2] = digitalReadPortRegister(OP3);
	ans[0][3] = digitalReadPortRegister(OP4);
	ans[0][4] = digitalReadPortRegister(OP5);
	ans[0][5] = digitalReadPortRegister(OP6);
	ans[0][6] = digitalReadPortRegister(OP7);
	ans[0][7] = digitalReadPortRegister(OP8);
	digitalWritePortRegister(IP1, HIGH);

	digitalWritePortRegister(IP2, LOW);
	ans[1][0] = digitalRead(OP1);
	ans[1][1] = digitalReadPortRegister(OP2);
	ans[1][2] = digitalReadPortRegister(OP3);
	ans[1][3] = digitalReadPortRegister(OP4);
	ans[1][4] = digitalReadPortRegister(OP5);
	ans[1][5] = digitalReadPortRegister(OP6);
	ans[1][6] = digitalReadPortRegister(OP7);
	ans[1][7] = digitalReadPortRegister(OP8);
	digitalWritePortRegister(IP2, HIGH);
	
	digitalWritePortRegister(IP3, LOW);
	ans[2][0] = digitalRead(OP1);
	ans[2][1] = digitalReadPortRegister(OP2);
	ans[2][2] = digitalReadPortRegister(OP3);
	ans[2][3] = digitalReadPortRegister(OP4);
	ans[2][4] = digitalReadPortRegister(OP5);
	ans[2][5] = digitalReadPortRegister(OP6);
	ans[2][6] = digitalReadPortRegister(OP7);
	ans[2][7] = digitalReadPortRegister(OP8);
	digitalWritePortRegister(IP3, HIGH);

	digitalWritePortRegister(IP4, LOW);
	ans[3][0] = digitalRead(OP1);
	ans[3][1] = digitalReadPortRegister(OP2);
	ans[3][2] = digitalReadPortRegister(OP3);
	ans[3][3] = digitalReadPortRegister(OP4);
	ans[3][4] = digitalReadPortRegister(OP5);
	ans[3][5] = digitalReadPortRegister(OP6);
	ans[3][6] = digitalReadPortRegister(OP7);
	ans[3][7] = digitalReadPortRegister(OP8);
	digitalWritePortRegister(IP4, HIGH);

	digitalWritePortRegister(IP5, LOW);
	ans[4][0] = digitalRead(OP1);
	ans[4][1] = digitalReadPortRegister(OP2);
	ans[4][2] = digitalReadPortRegister(OP3);
	ans[4][3] = digitalReadPortRegister(OP4);
	ans[4][4] = digitalReadPortRegister(OP5);
	ans[4][5] = digitalReadPortRegister(OP6);
	ans[4][6] = digitalReadPortRegister(OP7);
	ans[4][7] = digitalReadPortRegister(OP8);
	digitalWritePortRegister(IP5, HIGH);
}

// ポートレジスタをいじってデジタルライトを実行します
// pinはA1～A5に対応します
static void digitalWritePortRegister(unsigned int pin, unsigned int hOrl)
{
	switch (pin)
	{
	case A1:
		if (hOrl == HIGH) PORTF |= (64);
		else PORTF &= ~(64);
		break;
	case A2:
		if (hOrl == HIGH) PORTF |= (32);
		else PORTF &= ~(32);
		break;
	case A3:
		if (hOrl == HIGH) PORTF |= (16);
		else PORTF &= ~(16);
		break;
	case A4:
		if (hOrl == HIGH) PORTF |= (2);
		else PORTF &= ~(2);
		break;
	case A5:
		if (hOrl == HIGH) PORTF |= (1);
		else PORTF &= ~(1);
		break;
	default:
		break;
	}
}

// ポートレジスタをいじってデジタルリードを実行します
// pin は 4～11に対応します
static char digitalReadPortRegister(unsigned int pin)
{
	char ret = LOW;
	switch (pin)
	{
	case 4:
		if (PIND & _BV(4)) ret = HIGH;
		else ret = LOW;
		break;
	case 5:
		if (PINC & _BV(6)) ret = HIGH;
		else ret = LOW;
		break;
	case 6:
		if (PIND & _BV(7)) ret = HIGH;
		else ret = LOW;
		break;
	case 7:
		if (PINE & _BV(6)) ret = HIGH;
		else ret = LOW;
		break;
	case 8:
		if (PINB & _BV(4)) ret = HIGH;
		else ret = LOW;
		break;
	case 9:
		if (PINB & _BV(5)) ret = HIGH;
		else ret = LOW;
		break;
	case 10:
		if (PINB & _BV(6)) ret = HIGH;
		else ret = LOW;
		break;
	case 11:
		if (PINB & _BV(7)) ret = HIGH;
		else ret = LOW;
		break;
	default:
		break;
	}
	return ret;
}