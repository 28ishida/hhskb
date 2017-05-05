#include "right_firm.h"
#include "left_firm.h"

static const int OFF = 1;
static const int ON = 0;    // 今回の回路はアクティブロー

// 特殊文字宣言
static const char BS  = 0x08;
static const char TAB = 0x09;
static const char ESC = 0x1b;
static const char SPC = 0x20;
static const char DEL = 0x7f;
static const char LF  = 0x12;
static const char CR  = 0x15;

// キーをパースするウェイト時間(msec)
static const char PARSE_WAIT = 20;
// 連打抑制パース回数
static const char SILENT_COUNT = 400 / PARSE_WAIT; // パース時間が換算されていないので、500msecにはならない

// 右手通常文字の配置定義
static char RSymbol[5][8] = 
{
  { '7', '8', '9', '0', '-', '=', '\\', '`' },
  { 'y', 'u', 'i', 'o', 'p', '[', ']',  DEL },
  { 'h', 'j', 'k', 'l', ';', '\'', CR,  0 },
  { 'b', 'n', 'm', ',', '.', '/',  0 ,  0 },
  {  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 } 
};
// 右手用入力バッファ
static char RKey[5][8];

// 右手用入力バッファの列の先頭アドレスを格納する変数
static char* RKeyBuf[5];

// 右手用連打制御変数
static char RWaiter[5][8];

// 左手用シンボル
static char LSymbol[5][7] =
{
  { ESC, '1', '2', '3', '4', '5', '6' },
  { TAB, 'q', 'w', 'e', 'r', 't',  0  },
  { 0  , 'a', 's', 'd', 'f', 'g',  0  },
  { 0  , 'z', 'x', 'c', 'v', 'b',  0  },
  { 0  ,  0 ,  0 ,  0 ,  0 ,  0,   0  }
};

// 左手用入力バッファ
static char LKey[5][7];

// 左手用入力バッファの列の先頭アドレスを格納する変数
static char* LKeyBuf[5];

// 左手用入力連打抑制変数
static char LWaiter[5][7];

void setup() {
  // put your setup code here, to run once:
  memset( RWaiter, SILENT_COUNT, 40 );
  memset( LWaiter, SILENT_COUNT, 35 );
  memset( RKey, (char)1, 40 );
  memset( LKey, (char)1, 35 );

  // アドレス変数の初期化
  for( int i = 0; i < 5; i++ )
  {
    RKeyBuf[i] = RKey[i];
    LKeyBuf[i] = LKey[i];
  }
 
  InitRightFirm();
  InitLeftFirm();
  
  Serial.begin(9600);
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
      if ( ( RKey[row][col] == OFF ) && ( RWaiter[row][col] != SILENT_COUNT ) )
      {
        RWaiter[row][col] = SILENT_COUNT;
      }
      
      if ( RKey[row][col] == ON )
      {
        if ( RWaiter[row][col] == SILENT_COUNT ) 
        {
          Serial.print(RSymbol[row][col]);
          RWaiter[row][col]--;
        }
        else if ( RWaiter[row][col] > 0 )
        {
          RWaiter[row][col]--;
        }
        else
        {
          Serial.print(RSymbol[row][col]);
        }
      }
    }
  }

  for ( int row = 0; row < 5; row++ )
  {
    for ( int col = 0; col < 7; col++ )
    {
      if ( ( LKey[row][col] == OFF ) && ( LWaiter[row][col] != SILENT_COUNT ) )
      {
        LWaiter[row][col] = SILENT_COUNT;
      }
      
      if ( LKey[row][col] == ON )
      {
        if ( LWaiter[row][col] == SILENT_COUNT ) 
        {
          Serial.print(LSymbol[row][col]);
          LWaiter[row][col]--;
        }
        else if ( LWaiter[row][col] > 0 )
        {
          LWaiter[row][col]--;
        }
        else
        {
          Serial.print(LSymbol[row][col]);
        }
      }
    }
  }

  delay(PARSE_WAIT);
}
