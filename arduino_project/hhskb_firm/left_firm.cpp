#include "left_firm.h"
#include <arduino.h>
#include <skMCP230.h>
#include <Wire.h>

#define LED1 GPB5
#define LED2 GPB6
#define LED3 GPB7

// デバイスの７ビットアドレス(0100A2A1A0)
#define MCP230_ADDRESS  0b0100000   // MCP23017のI2Cアドレス(A2=0,A1=0,A0=0)
skMCP230 expIO(MCP230_ADDRESS) ;        // MCP23017ライブラリの生成を行う

// 左手モジュールの初期化。一度だけ呼び出してください
void InitLeftFirm()
{
  // Ｉ２Ｃの初期化
  Wire.begin() ;                     // マスターとする
  // MCP23017の初期化(ピンは全て出力で初期化されます)
  expIO.Init() ;

  expIO.pinMode(GPA, GPA0, INPUT_PULLUP);
  expIO.pinMode(GPA, GPA1, INPUT_PULLUP);
  expIO.pinMode(GPA, GPA2, INPUT_PULLUP);
  expIO.pinMode(GPA, GPA3, INPUT_PULLUP);
  expIO.pinMode(GPA, GPA4, INPUT_PULLUP);
  expIO.pinMode(GPA, GPA5, INPUT_PULLUP);
  expIO.pinMode(GPA, GPA6, INPUT_PULLUP);

  expIO.pinMode(GPB, GPB0, OUTPUT);
  expIO.pinMode(GPB, GPB1, OUTPUT);
  expIO.pinMode(GPB, GPB2, OUTPUT);
  expIO.pinMode(GPB, GPB3, OUTPUT);
  expIO.pinMode(GPB, GPB4, OUTPUT);

  expIO.Write(GPA, GPB0, HIGH);
  expIO.Write(GPA, GPB1, HIGH);
  expIO.Write(GPA, GPB2, HIGH);
  expIO.Write(GPA, GPB3, HIGH);
  expIO.Write(GPA, GPB4, HIGH);

  expIO.Write(GPB, LED1, HIGH);
  expIO.Write(GPB, LED2, LOW);
  expIO.Write(GPB, LED3, LOW);
}

// キーのパース 引数は5x7の行列分の領域を割り当て済である必要があります。
int ParseLeftKey(char** ans)
{
  // 左手パース
  expIO.Write(GPB, GPB0, LOW);
  ans[0][0] = expIO.Read(GPA, GPA0);
  ans[0][1] = expIO.Read(GPA, GPA1);
  ans[0][2] = expIO.Read(GPA, GPA2);
  ans[0][3] = expIO.Read(GPA, GPA3);
  ans[0][4] = expIO.Read(GPA, GPA4);
  ans[0][5] = expIO.Read(GPA, GPA5);
  ans[0][6] = expIO.Read(GPA, GPA6);
  expIO.Write(GPB, GPB0, HIGH);

  expIO.Write(GPB, GPB1, LOW);
  ans[1][0] = expIO.Read(GPA, GPA0);
  ans[1][1] = expIO.Read(GPA, GPA1);
  ans[1][2] = expIO.Read(GPA, GPA2);
  ans[1][3] = expIO.Read(GPA, GPA3);
  ans[1][4] = expIO.Read(GPA, GPA4);
  ans[1][5] = expIO.Read(GPA, GPA5);
  ans[1][6] = 0;
  expIO.Write(GPB, GPB1, HIGH);

  expIO.Write(GPB, GPB2, LOW);
  ans[2][0] = expIO.Read(GPA, GPA0);
  ans[2][1] = expIO.Read(GPA, GPA1);
  ans[2][2] = expIO.Read(GPA, GPA2);
  ans[2][3] = expIO.Read(GPA, GPA3);
  ans[2][4] = expIO.Read(GPA, GPA4);
  ans[2][5] = expIO.Read(GPA, GPA5);
  ans[2][6] = 0;
  expIO.Write(GPB, GPB2, HIGH);

  expIO.Write(GPB, GPB3, LOW);
  ans[3][0] = expIO.Read(GPA, GPA0);
  ans[3][1] = expIO.Read(GPA, GPA1);
  ans[3][2] = expIO.Read(GPA, GPA2);
  ans[3][3] = expIO.Read(GPA, GPA3);
  ans[3][4] = expIO.Read(GPA, GPA4);
  ans[3][5] = expIO.Read(GPA, GPA5);
  ans[3][6] = 0;
  expIO.Write(GPB, GPB3, HIGH);

  expIO.Write(GPB, GPB4, LOW);
  ans[4][0] = expIO.Read(GPA, GPA0);
  ans[4][1] = expIO.Read(GPA, GPA1);
  ans[4][2] = expIO.Read(GPA, GPA2);
  ans[4][3] = expIO.Read(GPA, GPA3);
  ans[4][4] = expIO.Read(GPA, GPA4);
  ans[4][5] = expIO.Read(GPA, GPA5);
  ans[4][6] = 0;
  expIO.Write(GPB, GPB4, HIGH);
}

int TurnOnStatusLed( int ledNum )
{
  if ( ledNum == 0 )
  {
    expIO.Write(GPB, LED1, HIGH);
  }
  else if ( ledNum == 1 )
  {
    expIO.Write(GPB, LED2, HIGH); 
  }
  else if ( ledNum == 2 )
  {
    expIO.Write(GPB, LED3, HIGH);
  }
}
int TurnOffStatusLed( int ledNum )
{
  if ( ledNum == 0 )
  {
    expIO.Write(GPB, LED1, LOW);
  }
  else if ( ledNum == 1 )
  {
    expIO.Write(GPB, LED2, LOW);
  }
  else if ( ledNum == 2 )
  {
    expIO.Write(GPB, LED3, LOW);
  }  
}



