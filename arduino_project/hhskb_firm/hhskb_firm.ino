#include <Keyboard.h>
#include "hhskb_firm.h"
#include "right_firm.h"
#include "left_firm.h"

// 右手通常文字の配置定義
static int RSymbol[ROWMAX][COLMAX] =
{
	{ '7', '8', '9', '0', '-', '=', '\\', '`' },
	{ 'y', 'u', 'i', 'o', 'p', '[', ']', KEY_DELETE },
	{ 'h', 'j', 'k', 'l', ';', '\'', KEY_RETURN, NO_ASMBL },
	{ 'b', 'n', 'm', ',', '.', '/', KEY_RIGHT_SHIFT, Fn },
	{ SPC, KEY_RIGHT_ALT, KEY_RIGHT_CTRL, NO_ASMBL, KEY_RIGHT_GUI, NO_ASMBL, NO_ASMBL, NO_ASMBL }
};

// 右手用ファンクション押下時シンボル 
// 通常側でFnを定義している箇所はここで変更出来ません。
static int RFnSymbol[ROWMAX][COLMAX] =
{
	{ KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_INSERT, KEY_DELETE },
	{ 0, 0, PRTSC, 0, 0, KEY_UP_ARROW, 0, KEY_BACKSPACE },
	{ 0, 0, KEY_HOME, KEY_PAGE_UP, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 0, NO_ASMBL },
	{ 0, 0, 0, KEY_END, KEY_PAGE_DOWN, KEY_DOWN_ARROW, KEY_RIGHT_SHIFT, Fn },
	{ SPC, KEY_RIGHT_ALT, KEY_RIGHT_CTRL, NO_ASMBL, KEY_RIGHT_GUI, NO_ASMBL, NO_ASMBL, NO_ASMBL }
};

// 右手ワンショット用テーブル
// 一つのキーが押下されて離されるまで他のキーが押下されなかった時に発射されるシンボル
static int ROSymbol[ROWMAX][COLMAX] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, NO_ASMBL },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, NO_ASMBL, 0, NO_ASMBL, NO_ASMBL, NO_ASMBL },
};

// 左手用シンボル
static int LSymbol[ROWMAX][COLMAX] =
{
	{ KEY_ESC, '1', '2', '3', '4', '5', '6', NO_ASMBL },
	{ TAB, 'q', 'w', 'e', 'r', 't', NO_ASMBL, NO_ASMBL },
	{ KEY_LEFT_CTRL, 'a', 's', 'd', 'f', 'g', NO_ASMBL, NO_ASMBL },
	{ KEY_LEFT_SHIFT, 'z', 'x', 'c', 'v', 'b', NO_ASMBL, NO_ASMBL },
	{ NO_ASMBL, NO_ASMBL, KEY_LEFT_GUI, 0, KEY_LEFT_ALT, KEY_LEFT_CTRL, NO_ASMBL, NO_ASMBL }
};

// 左手用Fn押下時シンボル
// 通常側でFnを定義している箇所はここで変更出来ません。
static int LFnSymbol[ROWMAX][COLMAX] =
{
	{ KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, NO_ASMBL },
	{ TAB, 0, 0, 0, 0, 0, NO_ASMBL, NO_ASMBL },
	{ KEY_LEFT_CTRL, 0, 0, 0, 0, 0, NO_ASMBL, NO_ASMBL },
	{ KEY_LEFT_SHIFT, 0, 0, 0, 0, 0, NO_ASMBL, NO_ASMBL },
	{ NO_ASMBL, NO_ASMBL, KEY_LEFT_GUI, 0, KEY_LEFT_ALT, KEY_LEFT_CTRL, NO_ASMBL, NO_ASMBL }
};

// 左手ワンショット用テーブル
// 一つのキーが押下されて離されるまで他のキーが押下されなかった時に発射されるシンボル
static int LOSymbol[ROWMAX][COLMAX] =
{
	{ 0, 0, 0, 0, 0, 0, 0, NO_ASMBL },
	{ 0, 0, 0, 0, 0, 0, NO_ASMBL, NO_ASMBL },
	{ 0, 0, 0, 0, 0, 0, NO_ASMBL, NO_ASMBL },
	{ 0, 0, 0, 0, 0, 0, NO_ASMBL, NO_ASMBL },
	{ NO_ASMBL, NO_ASMBL, 0, 0, 0, SPC, NO_ASMBL, NO_ASMBL },
};

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
					if ((tgt = getSymbol(row, col, lr, IsFnEnable)) == Fn)
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
					if ((tgt = getSymbol(row, col, lr, IsFnEnable)) == Fn)
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

// OneShot予約
// 戻り値:
//  0 OneShot予約は動かなかった。以後通常の処理を行う
//  1 OneShot予約が動いた。以後通常の処理は行わなくてOK。
// 引数: 
//  今押下されたキーの位置情報
static int oneShotReserve(int row, int col, LorR lr)
{
	int ret = 0;
	if (OneShotReserveCode != 0)	// 予約済のところに新しい予約 = 予約はキャンセルされ新しいコードは通常の値として処理される
	{
		Keyboard.press(OneShotCancelReserveCode);  
		OneShotReserveCode = 0;
		OneShotCancelReserveCode = 0;
	}
	else
	{
		int oTgt = getOneShotSymbol(row, col, lr);
		if ((oTgt != 0) && (oTgt != NO_ASMBL))
		{
			OneShotReserveCode = oTgt;
			OneShotCancelReserveCode = getSymbol(row, col, lr, IsFnEnable);
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
	if (OneShotReserveCode != 0)
	{
		int oTgt = getOneShotSymbol(row, col, lr);
		if (oTgt == OneShotReserveCode)
		{
			Keyboard.write(OneShotReserveCode);
			OneShotReserveCode = 0;
			OneShotCancelReserveCode = 0;
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
				Keyboard.release(getSymbol(row, col, Right, IsFnEnable));
			}
			if (OldLKey[row][col] == ON)
			{
				Keyboard.release(getSymbol(row, col, Left, IsFnEnable));
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
static int getSymbol(int row, int col, LorR lr, bool isFn)
{
	int ret = 0;
	if (lr == Left)
	{
		if (!isFn)
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
		if (!isFn)
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







