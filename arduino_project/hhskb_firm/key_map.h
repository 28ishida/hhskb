#pragma once

// 右手通常文字の配置定義
static int RSymbol[ROWMAX][COLMAX] =
{
	{ '7',   '8',  '9',  '0',  '-',  '=', '\\', '`' },
	{ 'y',   'u',  'i',  'o',  'p',  '[',  ']', DELE },
	{ 'h',   'j',  'k',  'l',  ';', '\'', RET_, NASB },
	{ 'b',   'n',  'm',  ',',  '.',  '/', RSFT, Fn   },
	{ RCTL, RALT, SPC_, NASB, RGUI, NASB, NASB, NASB }		// oneshot用
	//{ SPC_, RALT, SPC_, NASB, RGUI, NASB, NASB, NASB }	// 通常用
};
 
// 右手用ファンクション押下時シンボル 
// 通常側でFnを定義している箇所はここで変更出来ません。
static int RFnSymbol[ROWMAX][COLMAX] =
{
	{ F7__, F8__, F9__, F10_, F11_, F12_, INST, DELE },
	{ NOP_, NOP_, PRTS, NOP_, NOP_, UP__, NOP_, BS__ },
	{ NOP_, NOP_, HOME, PUP_, LEFT, RIGT, NOP_, NASB },
	{ NOP_, NOP_, NOP_, END_, PDWN, DOWN, RSFT, Fn   },
	{ RCTL, RALT, RCTL, NASB, RGUI, NASB, NASB, NASB }
};

// 右手用Layer1押下時シンボル 
static int RLayer1Symbol[ROWMAX][COLMAX] =
{
	{ F7__, F8__, F9__, F10_, F11_, F12_, NOP_, NOP_ },
	{ NOP_,  '_', NOP_, NOP_,  '+', NOP_, NOP_, NOP_ },
	{ LEFT, DOWN, UP__, RIGT, NOP_, NOP_, NOP_, NOP_ },
	{ '\\', NOP_,  '-', NOP_, NOP_, NOP_, NOP_, Fn   },
	{ NOP_, NOP_, NOP_, NASB, NOP_, NASB, NASB, NASB }
};

// 右手ワンショットキー
// 修飾キーが単独で押されたときに動くワンショットキー。
// 通常テーブルが修飾キーになっている所にしか定義出来ません。
static int ROSymbol[ROWMAX][COLMAX] =
{
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_ },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_ },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_ },
	{ SPC_, NOP_, NOP_, NASB, NOP_, NASB, NASB, NASB },
};

// 左手用シンボル
static int LSymbol[ROWMAX][COLMAX] =
{
	{ ESC_, '1',  '2',  '3',  '4',  '5',  '6',  NASB },
	{ TAB_, 'q',  'w',  'e',  'r',  't',  NASB, NASB },
	{ LY1_, 'a',  's',  'd',  'f',  'g',  NASB, NASB },
	{ LSFT, 'z',  'x',  'c',  'v',  'b',  NASB, NASB },
	{ NASB, NASB, LGUI, NOP_, LALT, LCTL, NASB, NASB }
};

// 左手用Fn押下時シンボル
// 通常側でFnを定義している箇所はここで変更出来ません。
static int LFnSymbol[ROWMAX][COLMAX] =
{
	{ ESC_, F1__, F2__, F3__, F4__, F5__, F6__, NASB },
	{ TAB_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ LY1_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ LSFT, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NASB, NASB, LGUI, NOP_, LALT, LCTL, NASB, NASB }
};

// 左手用Layer1押下時シンボル 
static int LLayer1Symbol[ROWMAX][COLMAX] =
{
	{ ESC_, F1__, F2__, F3__, F4__, F5__, F6__, NASB },
	{ NOP_, '\'', NOP_,  '=',  '(',  ')', NOP_, NOP_ },
	{ LY1_, NOP_, NOP_,  '"',  '{',  '}', NOP_, NOP_ },
	{ NOP_, NOP_, NOP_, NOP_,  '[',  ']', NOP_, NOP_ },
	{ NASB, NASB, LGUI, NOP_, LALT, LCTL, NASB, NASB }
};

// 左手ワンショットキー
// 修飾キーが単独で押されたときに動くワンショットキー。
// 通常テーブルが修飾キーになっている所にしか定義出来ません。
static int LOSymbol[ROWMAX][COLMAX] =
{
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NASB, NASB, NOP_, NOP_, NOP_, SPC_, NASB, NASB },
};
