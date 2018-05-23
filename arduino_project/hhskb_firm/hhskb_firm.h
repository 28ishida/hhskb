#pragma once

// hhskb_firmで使う一般的な型、定数定義

enum LorR
{
	Left,
	Right,
};

static const int OFF = 1;
static const int ON = 0;    // 今回の回路はアクティブロー
static const int NO_ASMBL = -1;

// 特殊文字宣言
static const char TAB = 0x09;
static const char SPC = 0x20;
static const int Fn = 0xf0;		// 適当
static const int LAYER1 = 0xf1;	// 適当
static const int PRTSC = 0xce;  // print screen?
static const int KANA = 0x15;
static const int KANJ = 0x19;
static const int CONV = 0x1C;
static const int NCON = 0x1D;


// キー配列の最大数
#define ROWMAX  5
#define COLMAX  8
static const int SUM = 40;


