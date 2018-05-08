#include <Keyboard.h>
#include "hhskb_firm.h"
#include "right_firm.h"
#include "left_firm.h"
#include "key_definition.h"
#include "key_map.h"

// Fnキー状態
static bool IsFnEnable = false;

// 現在のキー状態
static char RKey[ROWMAX][COLMAX];
static char LKey[ROWMAX][COLMAX];

// ひとつ前のキー状態のバッファ
static char OldRKey[ROWMAX][COLMAX];
static char OldLKey[ROWMAX][COLMAX];

// 発射予約キー
static char RsvdOnKey[RESERVED_MAX];
static char RsvdOnModKey[RESERVED_MAX];
static char RsvdOffKey[RESERVED_MAX];
static char RsvdOffModKey[RESERVED_MAX];


// ワンショットの予約
static bool OneShotReserveR[ROWMAX][COLMAX];

// ワンショットが実行される際に発行されるコードの予約値
static int OneShotReserveCode = 0;
// ワンショットが実行されなかった際に発行されるコードの予約値
static int OneShotCancelReserveCode = 0;
// ワンショットが生きているループ数
static int OneShotActiveCounter = 0;

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

	memset(RKey, (char)OFF, SUM);
	ParseRightKey(RKey);
	memset(LKey, (char)OFF, SUM);
	ParseLeftKey(LKey);

	keyboardAction(Right);
	memcpy(OldRKey, RKey, SUM);  
	keyboardAction(Left);
	memcpy(OldLKey, LKey, SUM);

	//oneShotActiveCounterExec();

	sendCode();
}

// 左右どちらかのモジュールに対してシンボルの検索、キーコードのプレス/リリースの発射を行います。
static void keyboardAction(LorR lr)
{
	int tgt = 0;
	int oTgt = 0;
	for (int row = 0; row < ROWMAX; row++)
	{
		for (int col = 0; col < COLMAX; col++)
		{
			if (isTurnOn(row, col, lr))
			{
				if ((tgt = getSymbol(row, col, lr)) == Fn)
				{
					IsFnEnable = true;
					TurnOnStatusLed(2);
				}
				else
				{
					reserveKey(tgt, true);
				}

			}
			else if (isTurnOff(row, col, lr))
			{
				if ((tgt = getSymbol(row, col, lr)) == Fn)
				{
					forceClear();			// IsFnEnableの前に実施する事が大事
					IsFnEnable = false;
					TurnOffStatusLed(2);
				}
				else
				{
					reserveKey(tgt, false);
				}
			}
		}
	}
}

// 予約されたキーを一気に送ります
static void sendCode()
{
	// 押す方は修飾キーから押す
	for (int cnt = 0; cnt < RESERVED_MAX; cnt++)
	{
		if (RsvdOnModKey[cnt] == 0)
		{
			break;
		}
		else
		{
			Keyboard.press(RsvdOnModKey[cnt]);
		}
	}
	for (int cnt = 0; cnt < RESERVED_MAX; cnt++)
	{
		if (RsvdOnKey[cnt] == 0)
		{
			break;
		}
		else
		{
			Keyboard.press(RsvdOnKey[cnt]);
		}
	}
	
	// 離すほうは通常キーから離す
	for (int cnt = 0; cnt < RESERVED_MAX; cnt++)
	{
		if (RsvdOffKey[cnt] == 0)
		{
			break;
		}
		else
		{
			Keyboard.release(RsvdOffKey[cnt]);
		}
	}
	for (int cnt = 0; cnt < RESERVED_MAX; cnt++)
	{
		if (RsvdOffModKey[cnt] == 0)
		{
			break;
		}
		else
		{
			Keyboard.release(RsvdOffModKey[cnt]);
		}
	}
	memset(RsvdOffKey, 0, sizeof(char)*RESERVED_MAX);
	memset(RsvdOffModKey, 0, sizeof(char)*RESERVED_MAX);
	memset(RsvdOnKey, 0, sizeof(char)*RESERVED_MAX);
	memset(RsvdOnModKey, 0, sizeof(char)*RESERVED_MAX);
}

// 発射予約
static void reserveKey(int tgt, bool isOn)
{
	if (isOn)
	{
		if (isModifier(tgt))
		{
			reserveKeyPrimitive(RsvdOnModKey, tgt);
		}
		else
		{
			reserveKeyPrimitive(RsvdOnKey, tgt);
		}
	}
	else
	{
		if (isModifier(tgt))
		{
			reserveKeyPrimitive(RsvdOffModKey, tgt);
		}
		else
		{
			reserveKeyPrimitive(RsvdOffKey, tgt);
		}

	}
}

// 予約の実体
static void reserveKeyPrimitive(char keyArray[], int val)
{
	// 予約満帆なら無視でいいや
	for (int cnt = 0; cnt < RESERVED_MAX; cnt++)
	{
		if (keyArray[cnt] == 0)
		{
			keyArray[cnt] = val;
		}
	}
}

// ModShotの定義があるか
static bool hasModShot(int row, int col, LorR lr)
{
	bool ret = false;
	int val = getModShotSymbol(row, col, lr);
	if ((val != NOP_) && (val != NASB))
	{
		ret = true;
	}
	return ret;
}

// 今回のシーケンスでONになったキーの数
static int ternOnKeyCount(LorR lr)
{
	int cnt = 0;
	for (int row = 0; row < ROWMAX; row++)
	{
		for (int col = 0; col < COLMAX; col++)
		{
			if (lr == Right)
			{
				if ((RKey[row][col]) && (!OldRKey[row][col]))
				{
					cnt++;
				}
			}
			else
			{
				if ((LKey[row][col]) && (!OldLKey[row][col]))
				{
					cnt++;
				}
			}
		}
	}
	return cnt;
}

// OneShot予約 (古いOneShotのキャンセル)
// 戻り値:
//  0 OneShot予約は動かなかった。以後通常の処理を行う
//  1 OneShot予約が動いた。以後通常の処理は行わなくてOK。
// 引数: 
//  今押下されたキーの位置情報
static int oneShotReserve(int row, int col, LorR lr)
{
	int ret = 0;
	if (isOneShotReserved())	// 予約済のところに新しい予約 = 予約はキャンセルされ新しいコードは通常の値として処理される
	{
		execOneShotCancel();
	}
	else
	{
		int oTgt = getModShotSymbol(row, col, lr);
		if ((oTgt != 0) && (oTgt != NASB))
		{
			OneShotReserveCode = oTgt;
			OneShotCancelReserveCode = getSymbol(row, col, lr);
			OneShotActiveCounter = ACTIVE_LOOP;
			ret = 1;
		}
	}
	return ret;
}

// OneShotの実行 
// 戻り値:
//  0 OneShotが実行されなかった。以後通常の処理を行う
//  1 OneShotが実行された。以後通常の処理を行わなくてOK。
// 引数: 
//  今リリースされたキーの位置情報
static int oneShotAction(int row, int col, LorR lr)
{
	int ret = 0;
	if (isOneShotReserved())
	{
		int oTgt = getModShotSymbol(row, col, lr);
		if (oTgt == OneShotReserveCode)
		{
			execOneShot();
			ret = 1;
		}
	}
	return ret;
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
		if (!IsFnEnable)
		{
			ret = LSymbol[row][col];
		}
		else
		{
			ret = LFnSymbol[row][col];
		}
	}
	else
	{
		if (!IsFnEnable)
		{
			ret = RSymbol[row][col];
		}
		else
		{
			ret = RFnSymbol[row][col];
		}
	}
	return ret;
}

// 修飾キーかどうか
static bool isModifier(int tgt)
{
	bool ret = false;
	if ((tgt == RSFT) ||
		(tgt == LSFT) ||
		(tgt == RCTL) ||
		(tgt == LCTL) ||
		(tgt == RALT) ||
		(tgt == LALT))
	{
		ret = true;
	}
	return ret;
}

static int getModShotSymbol(int row, int col, LorR lr)
{
	int ret = 0;
	if (lr == Left)
	{
		ret = LModSymbol[row][col];
	}
	else
	{
		ret = RModSymbol[row][col];
	}
	return ret;
}

// ワンショットカウンタの処理
static int  oneShotActiveCounterExec()
{
	if (isOneShotReserved())
	{
		if (OneShotActiveCounter != 0)
		{
			OneShotActiveCounter--;
		}
		else
		{
			// ワンショットの自動キャンセル
			execOneShotCancel();
		}
	}
}

// ワンショットが予約されているか
static bool isOneShotReserved()
{
	return OneShotReserveCode != 0;
}

// ワンショットのキャンセル
static void execOneShotCancel()
{
	if (isOneShotReserved())
	{
		Keyboard.press(OneShotCancelReserveCode);
		OneShotReserveCode = 0;
		OneShotCancelReserveCode = 0;
		OneShotActiveCounter = 0;
		delay(1);
	}
}

// ワンショットの実行
static void execOneShot()
{
	if (isOneShotReserved())
	{
		Keyboard.write(OneShotReserveCode);
		OneShotReserveCode = 0;
		OneShotCancelReserveCode = 0;
		OneShotActiveCounter = 0;
	}
}



