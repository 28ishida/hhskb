#pragma once

// 右手通常文字の配置定義
static int RSymbol[ROWMAX][COLMAX] =
{
	{ '7',   '8',  '9',  '0',  '-',  '=', '\\', '`' },
	{ 'y',   'u',  'i',  'o',  'p',  '[',  ']', DELE },
	{ 'h',   'j',  'k',  'l',  ';', '\'', RET_, NASB },
	{ 'b',   'n',  'm',  ',',  '.',  '/', RSFT, Fn },
	{ SPC_, RALT, RCTL, NASB, RGUI, NASB, NASB, NASB }
};

// 右手用ファンクション押下時シンボル 
// 通常側でFnを定義している箇所はここで変更出来ません。
static int RFnSymbol[ROWMAX][COLMAX] =
{
	{ F7__, F8__, F9__, F10_, F11_, F12_, INST, DELE },
	{ NOP_, NOP_, PRTS, NOP_, NOP_, UP__, NOP_, BS__ },
	{ NOP_, NOP_, HOME, PUP_, LEFT, RIGT, NOP_, NASB },
	{ NOP_, NOP_, NOP_, END_, PDWN, DOWN, RSFT, Fn },
	{ SPC_, RALT, RCTL, NASB, RGUI, NASB, NASB, NASB }
};

// 右手修飾キー
// 同時押しで発動する修飾キー。修飾キーしか格納出来ません。
static int RModSymbol[ROWMAX][COLMAX] =
{
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_ },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_ },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_ },
	{ RCTL, RALT, NOP_, NASB, NOP_, NASB, NASB, NASB },
};

// 左手用シンボル
static int LSymbol[ROWMAX][COLMAX] =
{
	{ ESC_, '1',  '2',  '3',  '4',  '5',  '6',  NASB },
	{ TAB_, 'q',  'w',  'e',  'r',  't',  NASB, NASB },
	{ LCTL, 'a',  's',  'd',  'f',  'g',  NASB, NASB },
	{ LSFT, 'z',  'x',  'c',  'v',  'b',  NASB, NASB },
	{ NASB, NASB, LGUI, NOP_, LALT, SPC_, NASB, NASB }
};

// 左手用Fn押下時シンボル
// 通常側でFnを定義している箇所はここで変更出来ません。
static int LFnSymbol[ROWMAX][COLMAX] =
{
	{ ESC_, F1__, F2__, F3__, F4__, F5__, F6__, NASB },
	{ TAB_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ LCTL, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ LSFT, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NASB, NASB, LGUI, NOP_, LALT, LCTL, NASB, NASB }
};

// 左手修飾キー
// 同時押しで発動する修飾キー。修飾キーしか格納できません
static int LModSymbol[ROWMAX][COLMAX] =
{
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NASB, NASB, NOP_, NOP_, NOP_, LCTL, NASB, NASB },
};
