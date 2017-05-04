#include <skMCP230.h>
#include <Wire.h>

// 右手通常文字の配置定義
char RSymbol[5][8] = 
{
  { '7', '8', '9', '0', '-', '=',  0 ,  0 },
  { 'y', 'u', 'i', 'o', 'p', '[', ']',  0 },
  { 'h', 'j', 'k', 'l', ';', '\'', 0 ,  0 },
  { 'b', 'n', 'm', ',', '.', '/',  0 ,  0 },
  {  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 } 
};
// 右手用入力バッファ
char RKey[5][8];

// 左手用シンボル
char LSymbol[5][7] =
{
  { 0, '1', '2', '3', '4', '5', '6' },
  { 0, 'q', 'w', 'e', 'r', 't',  0  },
  { 0, 'a', 's', 'd', 'f', 'g',  0  },
  { 0, 'z', 'x', 'c', 'v', 'b',  0  },
  { 0,  0 ,  0 ,  0 ,  0 ,  0,   0  }
};

// 左手用入力バッファ
char LKey[5][7];

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

// デバイスの７ビットアドレス(0100A2A1A0)
#define MCP230_ADDRESS  0b0100000   // MCP23017のI2Cアドレス(A2=0,A1=0,A0=0)
skMCP230 expIO(MCP230_ADDRESS) ;        // MCP23017ライブラリの生成を行う

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

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
}

void loop() {
  // put your main code here, to run repeatedly:
  // 右手パース
  int connBuf = 0;
  
  digitalWrite( IP1, LOW );
  RKey[0][0] = digitalRead( OP1 );
  RKey[0][1] = digitalRead( OP2 );
  RKey[0][2] = digitalRead( OP3 );
  RKey[0][3] = digitalRead( OP4 );
  RKey[0][4] = digitalRead( OP5 );
  RKey[0][5] = digitalRead( OP6 );
  RKey[0][6] = digitalRead( OP7 );
  RKey[0][7] = digitalRead( OP8 );
  digitalWrite( IP1, HIGH );
  
  digitalWrite( IP2, LOW );
  RKey[1][0] = digitalRead( OP1 );
  RKey[1][1] = digitalRead( OP2 );
  RKey[1][2] = digitalRead( OP3 );
  RKey[1][3] = digitalRead( OP4 );
  RKey[1][4] = digitalRead( OP5 );
  RKey[1][5] = digitalRead( OP6 );
  RKey[1][6] = digitalRead( OP7 );
  RKey[1][7] = digitalRead( OP8 );
  digitalWrite( IP2, HIGH );

  digitalWrite( IP3, LOW );
  RKey[2][0] = digitalRead( OP1 );
  RKey[2][1] = digitalRead( OP2 );
  RKey[2][2] = digitalRead( OP3 );
  RKey[2][3] = digitalRead( OP4 );
  RKey[2][4] = digitalRead( OP5 );
  RKey[2][5] = digitalRead( OP6 );
  RKey[2][6] = digitalRead( OP7 );
  RKey[2][7] = digitalRead( OP8 );
  digitalWrite( IP3, HIGH );

  digitalWrite( IP4, LOW );
  RKey[3][0] = digitalRead( OP1 );
  RKey[3][1] = digitalRead( OP2 );
  RKey[3][2] = digitalRead( OP3 );
  RKey[3][3] = digitalRead( OP4 );
  RKey[3][4] = digitalRead( OP5 );
  RKey[3][5] = digitalRead( OP6 );
  RKey[3][6] = digitalRead( OP7 );
  RKey[3][7] = digitalRead( OP8 );
  digitalWrite( IP4, HIGH );

  digitalWrite( IP5, LOW );
  RKey[4][0] = digitalRead( OP1 );
  RKey[4][1] = digitalRead( OP2 );
  RKey[4][2] = digitalRead( OP3 );
  RKey[4][3] = digitalRead( OP4 );
  RKey[4][4] = digitalRead( OP5 );
  RKey[4][5] = digitalRead( OP6 );
  RKey[4][6] = digitalRead( OP7 );
  RKey[4][7] = digitalRead( OP8 );
  digitalWrite( IP5, HIGH );

  for ( int row = 0; row < 5; row++ )
  {
    for ( int cow = 0; cow < 8; cow++ )
    {
      if ( RKey[row][cow] == 0 )
      {
        Serial.print(RSymbol[row][cow]);
      }
    }
  }
  
  // 左手パース
  expIO.Write(GPB, GPB0, LOW);
  LKey[0][0] = expIO.Read(GPA, GPA0);
  LKey[0][1] = expIO.Read(GPA, GPA1);
  LKey[0][2] = expIO.Read(GPA, GPA2);
  LKey[0][3] = expIO.Read(GPA, GPA3);
  LKey[0][4] = expIO.Read(GPA, GPA4);
  LKey[0][5] = expIO.Read(GPA, GPA5);
  LKey[0][6] = expIO.Read(GPA, GPA6);
  LKey[0][7] = 0;
  expIO.Write(GPB, GPB0, HIGH);

  expIO.Write(GPB, GPB1, LOW);
  LKey[1][0] = expIO.Read(GPA, GPA0);
  LKey[1][1] = expIO.Read(GPA, GPA1);
  LKey[1][2] = expIO.Read(GPA, GPA2);
  LKey[1][3] = expIO.Read(GPA, GPA3);
  LKey[1][4] = expIO.Read(GPA, GPA4);
  LKey[1][5] = expIO.Read(GPA, GPA5);
  LKey[1][6] = 0;
  LKey[1][7] = 0;
  expIO.Write(GPB, GPB1, HIGH);

  expIO.Write(GPB, GPB2, LOW);
  LKey[2][0] = expIO.Read(GPA, GPA0);
  LKey[2][1] = expIO.Read(GPA, GPA1);
  LKey[2][2] = expIO.Read(GPA, GPA2);
  LKey[2][3] = expIO.Read(GPA, GPA3);
  LKey[2][4] = expIO.Read(GPA, GPA4);
  LKey[2][5] = expIO.Read(GPA, GPA5);
  LKey[2][6] = 0;
  LKey[2][7] = 0;
  expIO.Write(GPB, GPB2, HIGH);

  expIO.Write(GPB, GPB3, LOW);
  LKey[3][0] = expIO.Read(GPA, GPA0);
  LKey[3][1] = expIO.Read(GPA, GPA1);
  LKey[3][2] = expIO.Read(GPA, GPA2);
  LKey[3][3] = expIO.Read(GPA, GPA3);
  LKey[3][4] = expIO.Read(GPA, GPA4);
  LKey[3][5] = expIO.Read(GPA, GPA5);
  LKey[3][6] = 0;
  LKey[3][7] = 0;
  expIO.Write(GPB, GPB3, HIGH);

  expIO.Write(GPB, GPB4, LOW);
  LKey[4][0] = expIO.Read(GPA, GPA0);
  LKey[4][1] = expIO.Read(GPA, GPA1);
  LKey[4][2] = expIO.Read(GPA, GPA2);
  LKey[4][3] = expIO.Read(GPA, GPA3);
  LKey[4][4] = expIO.Read(GPA, GPA4);
  LKey[4][5] = expIO.Read(GPA, GPA5);
  LKey[4][6] = 0;
  LKey[4][7] = 0;
  expIO.Write(GPB, GPB4, HIGH);

  for ( int row = 0; row < 5; row++ )
  {
    for ( int cow = 0; cow < 7; cow++ )
    {
      if ( LKey[row][cow] == 0 )
      {
        Serial.print(LSymbol[row][cow]);
      }
    }
  }
}
