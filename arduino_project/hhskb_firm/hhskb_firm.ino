#include <Keyboard.h>
#include "right_firm.h"
#include "left_firm.h"

#define ROWMAX  5
#define COLMAX  8
static const int SUM = 40;

// 一般的な定義
static const int OFF = 1;
static const int ON = 0;    // 今回の回路はアクティブロー
static const int NO_ASMBL = -1;

// 特殊文字宣言
static const char TAB = 0x09;
static const char SPC = 0x20;
static const int Fn = 0xf0;   // 適当
static const int PRTSC = 0xce;  // print screen?

// ファンクションキー関連
static bool IsFnEnable = false;

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
static int RFnSymbol[ROWMAX][COLMAX] =
{
  { KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_INSERT, KEY_DELETE },
  { 0, 0, PRTSC, 0, 0, KEY_UP_ARROW, 0, KEY_BACKSPACE },
  { 0, 0, KEY_HOME, KEY_PAGE_UP, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 0, NO_ASMBL },
  { 0, 0, 0, KEY_END, KEY_PAGE_DOWN, KEY_DOWN_ARROW, KEY_RIGHT_SHIFT, Fn },
  { SPC, KEY_RIGHT_ALT, KEY_RIGHT_CTRL, NO_ASMBL, KEY_RIGHT_GUI, NO_ASMBL, NO_ASMBL, NO_ASMBL }
};

// ひとつ前のパース結果を格納する変数
static char OldRKey[ROWMAX][COLMAX];
static char OldLKey[ROWMAX][COLMAX];

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
static int LFnSymbol[ROWMAX][COLMAX] =
{
  { KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, NO_ASMBL },
  { TAB, 0, 0, 0, 0, 0, NO_ASMBL, NO_ASMBL },
  { KEY_LEFT_CTRL, 0, 0, 0, 0, 0, NO_ASMBL, NO_ASMBL },
  { KEY_LEFT_SHIFT, 0, 0, 0, 0, 0, NO_ASMBL, NO_ASMBL },
  { NO_ASMBL, NO_ASMBL, KEY_LEFT_GUI, 0, KEY_LEFT_ALT, KEY_LEFT_CTRL, NO_ASMBL, NO_ASMBL }
};

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
	char RKey[ROWMAX][COLMAX];
	memset(RKey, (char)OFF, SUM);
	ParseRightKey(RKey);
	if (keyboardAction(RKey, OldRKey, RSymbol, RFnSymbol) == 1)
	{
		releaseFuncKeyAction(OldRKey, RFnSymbol);
		releaseFuncKeyAction(OldLKey, LFnSymbol);
		memset(OldRKey, (char)OFF, SUM);
		memset(OldLKey, (char)OFF, SUM);
	}
	else
	{
		memcpy(OldRKey, RKey, SUM);
	}

	char LKey[ROWMAX][COLMAX];
	memset(LKey, (char)OFF, SUM);
	ParseLeftKey(LKey);
	if (keyboardAction(LKey, OldLKey, LSymbol, LFnSymbol) == 1)
	{
		releaseFuncKeyAction(OldRKey, RFnSymbol);
		releaseFuncKeyAction(OldLKey, LFnSymbol);
		memset(OldRKey, (char)OFF, SUM);
		memset(OldLKey, (char)OFF, SUM);
	}
	else
	{
		memcpy(OldLKey, LKey, SUM);
	}
}

// シンボルの検索、キーコードのプレス、リリースコードの発射を行います。
// 戻り値 
//  0:正常終了。
//  1:ファンクションがリリースされた事を示す終了値。途中で処理が終了しています。
int keyboardAction(char Parsed[][COLMAX], char OldParsed[][COLMAX], int Symbol[][COLMAX], int FnSymbol[][COLMAX])
{
	for (int row = 0; row < ROWMAX; row++)
	{
		for (int col = 0; col < COLMAX; col++)
		{
			if ((Parsed[row][col] == ON) && (OldParsed[row][col] == OFF))
			{
				if (Symbol[row][col] == Fn)
				{
					IsFnEnable = true;
					TurnOnStatusLed(2);
				}
				if (IsFnEnable)
				{
					Keyboard.press(FnSymbol[row][col]);
				}
				else
				{
					Keyboard.press(Symbol[row][col]);
				}
			}
			else if ((Parsed[row][col] == OFF) && (OldParsed[row][col] == ON))
			{
				if (Symbol[row][col] == Fn)
				{
					IsFnEnable = false;
					TurnOffStatusLed(2);
					return 1;
				}
				if (IsFnEnable)
				{
					Keyboard.release(FnSymbol[row][col]);
				}
				else
				{
					Keyboard.release(Symbol[row][col]);
				}
			}
		}
	}
	return 0;
}

// Fnキーがリリースされた事によるアクション
void releaseFuncKeyAction(char PressingKey[][COLMAX], int FnSymbol[][COLMAX])
{
	// 同時押しされていたキーをすべてリリースします。(Fnキーコードとしてリリースを発射します。)
	for (int row = 0; row < ROWMAX; row++)
	{
		for (int col = 0; col < COLMAX; col++)
		{
			if (PressingKey[row][col] == ON)
			{
				Keyboard.release(FnSymbol[row][col]);
			}
		}
	}
}











