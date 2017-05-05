#include "right_firm.h"
#include <arduino.h>

// 右手側ピン番号の定義
#define OP1 4
#define OP2 5
#define OP3 6
#define OP4 7
#define OP5 8
#define OP6 9
#define OP7 10
#define OP8 11
#define IP1 A5
#define IP2 A4
#define IP3 A3
#define IP4 A2
#define IP5 A1

// 右手モジュールの初期化。一度だけ呼び出してください
void InitRightFirm()
{
  pinMode( OP1, INPUT_PULLUP);
  pinMode( OP2, INPUT_PULLUP);
  pinMode( OP3, INPUT_PULLUP);
  pinMode( OP4, INPUT_PULLUP);
  pinMode( OP5, INPUT_PULLUP);
  pinMode( OP6, INPUT_PULLUP);
  pinMode( OP7, INPUT_PULLUP);
  pinMode( OP8, INPUT_PULLUP);
  pinMode( IP1, OUTPUT);
  pinMode( IP2, OUTPUT);
  pinMode( IP3, OUTPUT);
  pinMode( IP4, OUTPUT);
  pinMode( IP5, OUTPUT);

  digitalWrite( IP1, HIGH);
  digitalWrite( IP2, HIGH);
  digitalWrite( IP3, HIGH);
  digitalWrite( IP4, HIGH);
  digitalWrite( IP5, HIGH);
}

// キーのパース 引数は5x8の行列分の領域を割り当て済である必要があります。
int ParseRightKey(char** ans)
{
  digitalWrite( IP1, LOW );
  ans[0][0] = digitalRead( OP1 );
  ans[0][1] = digitalRead( OP2 );
  ans[0][2] = digitalRead( OP3 );
  ans[0][3] = digitalRead( OP4 );
  ans[0][4] = digitalRead( OP5 );
  ans[0][5] = digitalRead( OP6 );
  ans[0][6] = digitalRead( OP7 );
  ans[0][7] = digitalRead( OP8 );
  digitalWrite( IP1, HIGH );
  
  digitalWrite( IP2, LOW );
  ans[1][0] = digitalRead( OP1 );
  ans[1][1] = digitalRead( OP2 );
  ans[1][2] = digitalRead( OP3 );
  ans[1][3] = digitalRead( OP4 );
  ans[1][4] = digitalRead( OP5 );
  ans[1][5] = digitalRead( OP6 );
  ans[1][6] = digitalRead( OP7 );
  ans[1][7] = digitalRead( OP8 );
  digitalWrite( IP2, HIGH );

  digitalWrite( IP3, LOW );
  ans[2][0] = digitalRead( OP1 );
  ans[2][1] = digitalRead( OP2 );
  ans[2][2] = digitalRead( OP3 );
  ans[2][3] = digitalRead( OP4 );
  ans[2][4] = digitalRead( OP5 );
  ans[2][5] = digitalRead( OP6 );
  ans[2][6] = digitalRead( OP7 );
  ans[2][7] = digitalRead( OP8 );
  digitalWrite( IP3, HIGH );

  digitalWrite( IP4, LOW );
  ans[3][0] = digitalRead( OP1 );
  ans[3][1] = digitalRead( OP2 );
  ans[3][2] = digitalRead( OP3 );
  ans[3][3] = digitalRead( OP4 );
  ans[3][4] = digitalRead( OP5 );
  ans[3][5] = digitalRead( OP6 );
  ans[3][6] = digitalRead( OP7 );
  ans[3][7] = digitalRead( OP8 );
  digitalWrite( IP4, HIGH );

  digitalWrite( IP5, LOW );
  ans[4][0] = digitalRead( OP1 );
  ans[4][1] = digitalRead( OP2 );
  ans[4][2] = digitalRead( OP3 );
  ans[4][3] = digitalRead( OP4 );
  ans[4][4] = digitalRead( OP5 );
  ans[4][5] = digitalRead( OP6 );
  ans[4][6] = digitalRead( OP7 );
  ans[4][7] = digitalRead( OP8 );
  digitalWrite( IP5, HIGH );
}

