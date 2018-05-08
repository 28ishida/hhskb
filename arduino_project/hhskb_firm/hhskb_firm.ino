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

	oneShotActiveCounterExec();
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
				if (oneShotReserve(row, col, lr) == 0)
				{
					if ((tgt = getSymbol(row, col, lr)) == Fn)
					{
						IsFnEnable = true;
						TurnOnStatusLed(2);
					}
					else
					{
						Keyboard.press(tgt);
					}
				}
			}
			else if (isTurnOff(row, col, lr))
			{
				if (oneShotAction(row, col, lr) == 0)
				{
					if ((tgt = getSymbol(row, col, lr)) == Fn)
					{
						forceClear();			// IsFnEnableの前に実施する事が大事
						IsFnEnable = false;
						TurnOffStatusLed(2);
					}
					else
					{
						Keyboard.release(tgt);
					}
				}
			}
		}
	}
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
		int oTgt = getOneShotSymbol(row, col, lr);
		if ((oTgt != 0) && (oTgt != NO_ASMBL))
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
		int oTgt = getOneShotSymbol(row, col, lr);
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

static int getOneShotSymbol(int row, int col, LorR lr)
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



