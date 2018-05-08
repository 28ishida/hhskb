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
static const int Fn = 0xf0;   // 適当
static const int PRTSC = 0xce;  // print screen?

// キー配列の最大数
#define ROWMAX  5
#define COLMAX  8
static const int SUM = 40;

#define RESERVED_MAX 10


// ワンショットを活かすループ数 
// このループ数が経過すると自動的にワンショットはキャンセルされます
// 感覚的に10カウントで0.3秒以下。
// 押下して離すまでにほんの一呼吸あるとワンショットは動かない感じ。
// 逆にワンショットを発動させようとキーを押下してすぐに離した場合で、入力にしくじる事はほぼ無い。
// 修飾キーとして押下して何も他のキーを幼かった際の誤爆が全く起きなくなった。
static const int ACTIVE_LOOP = 10;
