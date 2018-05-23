#include <Keyboard.h>
#include "hhskb_firm.h"
#include "right_firm.h"
#include "left_firm.h"
#include "key_definition.h"
#include "key_map.h"

#define ONESHOT_DEF
//#undef ONESHOT_DEF

// Fnキー状態
static bool IsFnEnable = false;

// Layer1キー状態
static bool IsLayer1Enable = false;

// 現在のキー状態
static char RKey[ROWMAX][COLMAX];
static char LKey[ROWMAX][COLMAX];

// ひとつ前のキー状態のバッファ
static char OldRKey[ROWMAX][COLMAX];
static char OldLKey[ROWMAX][COLMAX];

// ワンショットが実行される際に発行されるコードの予約値
static int OneShotReserveCode = 0;

// ワンショットが予約された元キー
static int OneShotBaseCode = 0;

// 一回前に発行されたワンショットキーシンボル
static int PrevOneShotCode = 0;

// ワンショットのリピート実行のキーシンボル
static int RepeatOneShotCode = 0;

static int speedCheck = 1000;

void setup() {
	// put your setup code here, to run once:
	memset(OldRKey, (char)OFF, SUM);
	memset(OldLKey, (char)OFF, SUM);

	InitRightFirm();
	InitLeftFirm();

	Keyboard.begin();
}

void loop() {
	// put your main code here, to run repeatedly:

	unsigned long start = micros();
	memset(RKey, (char)OFF, SUM);
	ParseRightKey(RKey);
	unsigned long end = micros();

	if (speedCheck > 0)
	{
		Serial.println("RIGHT parse");
		Serial.print(end - start);
		Serial.println("");
		Serial.println("");
	}

	start = micros();
	memset(LKey, (char)OFF, SUM);
	ParseLeftKey(LKey);
	end = micros();

	if (speedCheck > 0)
	{
		Serial.println("LEFT parse");
		Serial.print(end - start);
		Serial.println("");
		Serial.println("");
		speedCheck--;
	}
	
	keyboardAction(Right);
	memcpy(OldRKey, RKey, SUM);  
	keyboardAction(Left);
	memcpy(OldLKey, LKey, SUM);
}

// 左右どちらかのモジュールに対してシンボルの検索、キーコードのプレス/リリースの発射を行います。
static void keyboardAction(LorR lr)
{
	for (int row = 0; row < ROWMAX; row++)
	{
		for (int col = 0; col < COLMAX; col++)
		{
			int normSymb = getSymbol(row, col, lr);		// 通常のキーコード
			int osSymb = getOSymbol(row, col, lr);		// ワンショットキーキーコード

			if (isTurnOn(row, col, lr))
			{
				if (normSymb == Fn)
				{
					IsFnEnable = true;
					TurnOnStatusLed(2);
				}
				else if (normSymb == LY1_)
				{
					IsLayer1Enable = true;
					TurnOnStatusLed(1);
				}
				else
				{
					// ワンショットリピートモードであれば以後の処理は不要
					if (!repeatOneShotStart(osSymb))
					{
						reserveOneShot(normSymb, osSymb);
						sendKey(normSymb, true);
					}
				}
			}
			else if (isTurnOff(row, col, lr))
			{
				if (normSymb == Fn)
				{
					forceClear();			// IsFnEnableの前に実施する事が大事
					IsFnEnable = false;
					TurnOffStatusLed(2);
				}
				else if (normSymb == LY1_)
				{
					forceClear();			// IsFnEnableの前に実施する事が大事
					IsLayer1Enable = false;
					TurnOffStatusLed(1);
				}
				else
				{
					if (!repeatOneShotEnd(osSymb))
					{
						sendKey(normSymb, false);
						actionOneShot(normSymb, osSymb);
					}                                                           
				}
			}
		}
	}
}

// ワンショットリピートの開始処理
//  ワンショットキーを連打された処理。
//  戻り値 : true  リピート処理を実施。以後の処理は省略してください
//           false リピートは発生せず。以後は通常の処理を続けます
static bool repeatOneShotStart(int otgt)
{
	bool ret = false;
#ifdef ONESHOT_DEF
	if (PrevOneShotCode != 0)
	{
		if (PrevOneShotCode == otgt)
		{
			RepeatOneShotCode = otgt;
			sendKey(otgt, true);		// ワンショットキーを押しっぱなし
			ret = true;
		}
		else
		{
			PrevOneShotCode = 0;
		}
	}
#endif
	return ret;
}

// ワンショットリピートの終了処理
//  戻り値 : true  リピート処理を終了。以後の処理は省略してください
//           false リピート終了処理は発生せず。以後は通常の処理を続けます
static bool repeatOneShotEnd(int osSymb)
{
	bool ret = false;
#ifdef ONESHOT_DEF
	if (RepeatOneShotCode != 0)
	{
		if (RepeatOneShotCode == osSymb)
		{
			RepeatOneShotCode = 0;
			sendKey(osSymb, false);		// ワンショットキーを押しっぱなしを解除
			ret = true;
		}
	}
#endif
	return ret;
}

// ワンショットの実行
static void actionOneShot(int normSymb, int osSymb)
{
#ifdef ONESHOT_DEF 
	if ((OneShotReserveCode == osSymb) && (OneShotBaseCode == normSymb))
	{
		sendKey(OneShotReserveCode, true);
		sendKey(OneShotReserveCode, false);
		PrevOneShotCode = OneShotReserveCode;	// 発行したワンショットキーは連射に備えて保存しておきます
		clearOneShotReserve();
	}
#endif
}

// ワンショットの予約  
static void reserveOneShot(int normSymb, int osSymb)
{
#ifdef ONESHOT_DEF
	if (osSymb != NOP_ || osSymb != NASB)
	{
		OneShotReserveCode = osSymb;
		OneShotBaseCode = normSymb;
	}
	else
	{
		// 次のキーが打たれたので予約はキャンセル
		clearOneShotReserve();
	}
#endif // ONESHOT_DEF
}

// ワンショットの予約をクリア
static void clearOneShotReserve()
{
	OneShotReserveCode = 0;
	OneShotBaseCode = 0;
}

// 発射
static void sendKey(int code, bool isOn)
{
	if (isOn)
	{
		Keyboard.press(code);
	}
	else
	{
		Keyboard.release(code);
	}
}

// 左右すべての押下中のキーをすべてクリアします
static void forceClear()
{
	for (int row = 0; row < ROWMAX; row++)
	{
		for (int col = 0; col < COLMAX; col++)
		{
			if (OldRKey[row][col] == ON)
			{
				Keyboard.release(getSymbol(row, col, Right));
			}
			if (OldLKey[row][col] == ON)
			{
				Keyboard.release(getSymbol(row, col, Left));
			}
		}
	}
}

// キーが今越されたかを判定する
static bool isTurnOn(int row, int col, LorR lr)
{
	bool ret = false;
	if (lr == Left)
	{
		ret = (OldLKey[row][col] == OFF) && (LKey[row][col] == ON);
	}
	else
	{
		ret = (OldRKey[row][col] == OFF) && (RKey[row][col] == ON);
	}
	return ret;
}

// キーが今離されたかを判定する
static bool isTurnOff(int row, int col, LorR lr)
{
	bool ret = false;
	if (lr == Left)
	{
		ret = (OldLKey[row][col] == ON) && (LKey[row][col] == OFF);
	}
	else
	{
		ret = (OldRKey[row][col] == ON) && (RKey[row][col] == OFF);
	}
	return ret;
}

// キーシンボルを取得
static int getSymbol(int row, int col, LorR lr)
{
	int ret = 0;
	if (lr == Left)
	{
		if (IsFnEnable)
		{
			ret = LFnSymbol[row][col];
		}
		else if (IsLayer1Enable)
		{
			ret = LLayer1Symbol[row][col];
		}
		else
		{
			ret = LSymbol[row][col];
		}
	}
	else
	{
		if (IsFnEnable)
		{
			ret = RFnSymbol[row][col];
		}
		else if (IsLayer1Enable)
		{
			ret = RLayer1Symbol[row][col];
		}
		else
		{
			ret = RSymbol[row][col];
		}
	}
	return ret;
}

// ワンショットシンボル取得
static int getOSymbol(int row, int col, LorR lr)
{
	int ret = 0;
	if (lr == Left)
	{
		ret = LOSymbol[row][col];
	}
	else
	{
		ret = ROSymbol[row][col];
	}
	return ret;
}

