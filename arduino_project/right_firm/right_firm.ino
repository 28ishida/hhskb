#include "MCP23017.h"

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


void setup() {

  Serial.begin(9600);

  // put your setup code here, to run once:
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

  // 左手側初期化
  mcp23017_init();
  //Write(GPB, GPB5, HIGH);
  //Write(GPB, GPB6, HIGH);
  //Write(GPB, GPB7, HIGH);

  pinMode(GPA, GPA0, INPUT_PULLUP);
  pinMode(GPA, GPA1, INPUT_PULLUP);
  pinMode(GPA, GPA2, INPUT_PULLUP);
  pinMode(GPA, GPA3, INPUT_PULLUP);
  pinMode(GPA, GPA4, INPUT_PULLUP);
  pinMode(GPA, GPA5, INPUT_PULLUP);
  pinMode(GPA, GPA6, INPUT_PULLUP);

  pinMode(GPB, GPB0, OUTPUT);
  pinMode(GPB, GPB1, OUTPUT);
  pinMode(GPB, GPB2, OUTPUT);
  pinMode(GPB, GPB3, OUTPUT);
  pinMode(GPB, GPB4, OUTPUT);

  Write(GPA, GPB0, HIGH);
  Write(GPA, GPB1, HIGH);
  Write(GPA, GPB2, HIGH);
  Write(GPA, GPB3, HIGH);
  Write(GPA, GPB4, HIGH);
}

void loop() {
  
  // 右手パース
  int connBuf = 0;
  digitalWrite( IP1, LOW );
  int sw11 = digitalRead( OP1 );
  int sw12 = digitalRead( OP2 );
  int sw13 = digitalRead( OP3 );
  int sw14 = digitalRead( OP4 );
  int sw15 = digitalRead( OP5 );
  int sw16 = digitalRead( OP6 );
  int sw17 = digitalRead( OP7 );
  int sw18 = digitalRead( OP8 );
  digitalWrite( IP1, HIGH );
  Serial.print( "row1:" );
  connBuf = ( sw11 << 7 ) | ( sw12 << 6 ) | ( sw13 << 5 ) | ( sw14 << 4 ) |
            ( sw15 << 3 ) | ( sw16 << 2 ) | ( sw17 << 1 ) | ( sw18 << 0 ) ;
  Serial.println( connBuf, BIN );
  connBuf = 0;
  
  digitalWrite( IP2, LOW );
  int sw21 = digitalRead( OP1 );
  int sw22 = digitalRead( OP2 );
  int sw23 = digitalRead( OP3 );
  int sw24 = digitalRead( OP4 );
  int sw25 = digitalRead( OP5 );
  int sw26 = digitalRead( OP6 );
  int sw27 = digitalRead( OP7 );
  int sw28 = digitalRead( OP8 );
  digitalWrite( IP2, HIGH );
  Serial.print( "row2:" );
  connBuf = ( sw21 << 7 ) | ( sw22 << 6 ) | ( sw23 << 5 ) | ( sw24 << 4 ) |
            ( sw25 << 3 ) | ( sw26 << 2 ) | ( sw27 << 1 ) | ( sw28 << 0 ) ;
  Serial.println( connBuf, BIN );
  connBuf = 0;

  digitalWrite( IP3, LOW );
  int sw31 = digitalRead( OP1 );
  int sw32 = digitalRead( OP2 );
  int sw33 = digitalRead( OP3 );
  int sw34 = digitalRead( OP4 );
  int sw35 = digitalRead( OP5 );
  int sw36 = digitalRead( OP6 );
  int sw37 = digitalRead( OP7 );
  int sw38 = digitalRead( OP8 );
  digitalWrite( IP3, HIGH );
  Serial.print( "row3:" );
  connBuf = ( sw31 << 7 ) | ( sw32 << 6 ) | ( sw33 << 5 ) | ( sw34 << 4 ) |
            ( sw35 << 3 ) | ( sw36 << 2 ) | ( sw37 << 1 ) | ( sw38 << 0 ) ;
  Serial.println( connBuf, BIN );
  connBuf = 0;

  digitalWrite( IP4, LOW );
  int sw41 = digitalRead( OP1 );
  int sw42 = digitalRead( OP2 );
  int sw43 = digitalRead( OP3 );
  int sw44 = digitalRead( OP4 );
  int sw45 = digitalRead( OP5 );
  int sw46 = digitalRead( OP6 );
  int sw47 = digitalRead( OP7 );
  int sw48 = digitalRead( OP8 );
  digitalWrite( IP4, HIGH );
  Serial.print( "row4:" );
  connBuf = ( sw41 << 7 ) | ( sw42 << 6 ) | ( sw43 << 5 ) | ( sw44 << 4 ) |
            ( sw45 << 3 ) | ( sw46 << 2 ) | ( sw47 << 1 ) | ( sw48 << 0 ) ;
  Serial.println( connBuf, BIN );
  connBuf = 0;

  digitalWrite( IP5, LOW );
  int sw51 = digitalRead( OP1 );
  int sw52 = digitalRead( OP2 );
  int sw53 = digitalRead( OP3 );
  int sw54 = digitalRead( OP4 );
  int sw55 = digitalRead( OP5 );
  int sw56 = digitalRead( OP6 );
  int sw57 = digitalRead( OP7 );
  int sw58 = digitalRead( OP8 );
  digitalWrite( IP5, HIGH );
  Serial.print( "row5:" );
  connBuf = ( sw51 << 7 ) | ( sw52 << 6 ) | ( sw53 << 5 ) | ( sw54 << 4 ) |
            ( sw55 << 3 ) | ( sw56 << 2 ) | ( sw57 << 1 ) | ( sw58 << 0 ) ;
  Serial.println( connBuf, BIN );
  connBuf = 0;

  // 左手パース
  Write(GPB, GPB0, LOW);
  int lsw11 = Read(GPA, GPA0);
  int lsw12 = Read(GPA, GPA1);
  int lsw13 = Read(GPA, GPA2);
  int lsw14 = Read(GPA, GPA3);
  int lsw15 = Read(GPA, GPA4);
  int lsw16 = Read(GPA, GPA5);
  int lsw17 = Read(GPA, GPA6);
  int lsw18 = 0;
  Write(GPB, GPB0, HIGH);
  Serial.print( "Lrow1:" );
  connBuf = ( lsw11 << 7 ) | ( lsw12 << 6 ) | ( lsw13 << 5 ) | ( lsw14 << 4 ) |
            ( lsw15 << 3 ) | ( lsw16 << 2 ) | ( lsw17 << 1 ) | ( lsw18 << 0 );
  Serial.println( connBuf, BIN );
  connBuf = 0;

  Write(GPB, GPB1, LOW);
  int lsw21 = Read(GPA, GPA0);
  int lsw22 = Read(GPA, GPA1);
  int lsw23 = Read(GPA, GPA2);
  int lsw24 = Read(GPA, GPA3);
  int lsw25 = Read(GPA, GPA4);
  int lsw26 = Read(GPA, GPA5);
  int lsw27 = 0;
  int lsw28 = 0;
  Write(GPB, GPB1, HIGH);
  Serial.print( "Lrow2:" );
  connBuf = ( lsw21 << 7 ) | ( lsw22 << 6 ) | ( lsw23 << 5 ) | ( lsw24 << 4 ) |
            ( lsw25 << 3 ) | ( lsw26 << 2 ) | ( lsw27 << 1 ) | ( lsw28 << 0 );
  Serial.println( connBuf, BIN );
  connBuf = 0;

  Write(GPB, GPB2, LOW);
  int lsw31 = Read(GPA, GPA0);
  int lsw32 = Read(GPA, GPA1);
  int lsw33 = Read(GPA, GPA2);
  int lsw34 = Read(GPA, GPA3);
  int lsw35 = Read(GPA, GPA4);
  int lsw36 = Read(GPA, GPA5);
  int lsw37 = 0;
  int lsw38 = 0;
  Write(GPB, GPB2, HIGH);
  Serial.print( "Lrow3:" );
  connBuf = ( lsw31 << 7 ) | ( lsw32 << 6 ) | ( lsw33 << 5 ) | ( lsw34 << 4 ) |
            ( lsw35 << 3 ) | ( lsw36 << 2 ) | ( lsw37 << 1 ) | ( lsw38 << 0 );
  Serial.println( connBuf, BIN );
  connBuf = 0;

  Write(GPB, GPB3, LOW);
  int lsw41 = Read(GPA, GPA0);
  int lsw42 = Read(GPA, GPA1);
  int lsw43 = Read(GPA, GPA2);
  int lsw44 = Read(GPA, GPA3);
  int lsw45 = Read(GPA, GPA4);
  int lsw46 = Read(GPA, GPA5);
  int lsw47 = 0;
  int lsw48 = 0;
  Write(GPB, GPB3, HIGH);
  Serial.print( "Lrow4:" );
  connBuf = ( lsw41 << 7 ) | ( lsw42 << 6 ) | ( lsw43 << 5 ) | ( lsw44 << 4 ) |
            ( lsw45 << 3 ) | ( lsw46 << 2 ) | ( lsw47 << 1 ) | ( lsw48 << 0 );
  Serial.println( connBuf, BIN );
  connBuf = 0;

  Write(GPB, GPB4, LOW);
  int lsw51 = Read(GPA, GPA0);
  int lsw52 = Read(GPA, GPA1);
  int lsw53 = Read(GPA, GPA2);
  int lsw54 = Read(GPA, GPA3);
  int lsw55 = Read(GPA, GPA4);
  int lsw56 = Read(GPA, GPA5);
  int lsw57 = 0;
  int lsw58 = 0;
  Write(GPB, GPB4, HIGH);
  Serial.print( "Lrow5:" );
  connBuf = ( lsw51 << 7 ) | ( lsw52 << 6 ) | ( lsw53
  << 5 ) | ( lsw54 << 4 ) |
            ( lsw55 << 3 ) | ( lsw56 << 2 ) | ( lsw57 << 1 ) | ( lsw58 << 0 );
  Serial.println( connBuf, BIN );
  Serial.println( "\n\n\n" );
  connBuf = 0;
 
  delay(500);
}
