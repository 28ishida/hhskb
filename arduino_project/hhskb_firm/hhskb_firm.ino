#include <Keyboard.h>
#include "right_firm.h"
#include "left_firm.h"

static void ReleaseAllKey();

// 一般的な定義
static const int OFF = 1;
static const int ON = 0;    // 今回の回路はアクティブロー
static const int NO_ASMBL = -1;

// 特殊文字宣言
static const char TAB = 0x09;
static const char SPC = 0x20;
static const int Fn   = 0xf0;   // 適当
static const int PRTSC = 0xce;  // print screen?

// ファンクションキー関連
static bool IsFnEnable = false;

// 右手通常文字の配置定義
static int RSymbol[5][8] = 
{
  { '7', '8', '9', '0', '-', '=', '\\', '`' },
  { 'y', 'u', 'i', 'o', 'p', '[', ']', KEY_DELETE },
  { 'h', 'j', 'k', 'l', ';', '\'', KEY_RETURN, NO_ASMBL },
  { 'b', 'n', 'm', ',', '.', '/', KEY_RIGHT_SHIFT, Fn },
  { SPC, KEY_RIGHT_ALT, KEY_RIGHT_CTRL, NO_ASMBL, KEY_RIGHT_GUI, NO_ASMBL, NO_ASMBL, NO_ASMBL } 
};

// 右手用ファンクション押下時シンボル
static int RFnSymbol[5][8] =
{
  { KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_INSERT, KEY_DELETE },
  { 0, 0, PRTSC, 0, 0, KEY_UP_ARROW, 0, KEY_BACKSPACE },
  { 0, 0, KEY_HOME, KEY_PAGE_UP, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 0, NO_ASMBL },
  { 0, 0, 0, KEY_END, KEY_PAGE_DOWN, KEY_DOWN_ARROW, 0, Fn },
  { SPC, KEY_RIGHT_ALT, KEY_RIGHT_CTRL, NO_ASMBL, KEY_RIGHT_GUI, NO_ASMBL, NO_ASMBL, NO_ASMBL } 
};

// 右手用入力バッファ
static char RKey[5][8];

// 右手用入力バッファの列の先頭アドレスを格納する変数
static char* RKeyBuf[5];

// ひとつ前のパース結果を格納する変数
static char OldRKey[5][8];

// 左手用シンボル
static int LSymbol[5][7] =
{
  { KEY_ESC, '1', '2', '3', '4', '5', '6' },
  { TAB, 'q', 'w', 'e', 'r', 't', NO_ASMBL },
  { KEY_LEFT_CTRL, 'a', 's', 'd', 'f', 'g', NO_ASMBL },
  { KEY_LEFT_SHIFT, 'z', 'x', 'c', 'v', 'b', NO_ASMBL },
  { NO_ASMBL, NO_ASMBL, KEY_LEFT_GUI, 0, KEY_RIGHT_ALT, KEY_RIGHT_CTRL, NO_ASMBL }
};

// 左手用Fn押下時シンボル
static int LFnSymbol[5][7] =
{
  { 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6 },
  { 0, 0, 0, 0, 0, 0, NO_ASMBL },
  { 0, 0, 0, 0, 0, 0, NO_ASMBL },
  { 0, 0, 0, 0, 0, 0, NO_ASMBL },
  { NO_ASMBL, NO_ASMBL, KEY_LEFT_GUI, 0, KEY_RIGHT_ALT, KEY_RIGHT_CTRL, NO_ASMBL }
};

// 左手用入力バッファ
static char LKey[5][7];

// 左手用入力バッファの列の先頭アドレスを格納する変数
static char* LKeyBuf[5];

// ひとつ前のパース結果を格納する変数
static char OldLKey[5][8];

void setup() {
  // put your setup code here, to run once:
  memset( RKey, (char)OFF, 40 );
  memset( LKey, (char)OFF, 35 );
  memset( OldRKey, (char)OFF, 40 );
  memset( OldLKey, (char)OFF, 35 );

  // アドレス変数の初期化
  for( int i = 0; i < 5; i++ )
  {
    RKeyBuf[i] = RKey[i];
    LKeyBuf[i] = LKey[i];
  }
  InitRightFirm();
  InitLeftFirm();
  
  Keyboard.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  // キーのパース
  ParseRightKey(RKeyBuf);
  ParseLeftKey(LKeyBuf);
  
  for ( int row = 0; row < 5; row++ )
  {
    for ( int col = 0; col < 8; col++ )
    {
      if ( ( RKey[row][col] == ON ) && ( OldRKey[row][col] == OFF ) )
      {
        if ( RSymbol[row][col] == Fn )
        {
          if ( !IsFnEnable )
          {
            IsFnEnable = true;
            TurnOnStatusLed(2);
          }
          else
          {
            IsFnEnable = false;
            TurnOffStatusLed(2);
            ReleaseAllKey();
          }
        }
        if (!IsFnEnable )
        {
          Keyboard.press(RSymbol[row][col]);
        }
        else
        {
          Keyboard.press(RFnSymbol[row][col]);
        }
        OldRKey[row][col] = ON;
      }

      if ( ( OldRKey[row][col] == ON ) && ( RKey[row][col] == OFF ) )
      {
        if ( !IsFnEnable )
        {
          Keyboard.release(RSymbol[row][col]);
        }
        else
        {
          Keyboard.release(RFnSymbol[row][col]);
        }
                
        OldRKey[row][col] = OFF;
      }
    }
  }

  for ( int row = 0; row < 5; row++ )
  {
    for ( int col = 0; col < 7; col++ )
    {
      if ( ( LKey[row][col] == ON ) && ( OldLKey[row][col] == OFF ) )
      {
        if ( !IsFnEnable )
        {
          Keyboard.press(LSymbol[row][col]);
        }
        else
        {
          Keyboard.press(LFnSymbol[row][col]);
        }
        OldLKey[row][col] = ON;
      }
      if ( ( OldLKey[row][col] == ON ) && ( LKey[row][col] == OFF ) )
      {
        if ( !IsFnEnable )
        {
          Keyboard.release(LSymbol[row][col]);
        }
        else
        {
          Keyboard.release(LFnSymbol[row][col]);
        }
        OldLKey[row][col] = OFF;
      }
    }
  }
}

static void ReleaseAllKey()
{
  Keyboard.releaseAll();
  memset( OldRKey, (char)OFF, 40 );
  memset( OldLKey, (char)OFF, 35 );
}

