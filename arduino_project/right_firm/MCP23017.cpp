#include "MCP23017.h"
#include <arduino.h>

static int Send(char reg_adrs,unsigned char *data,char kosu);
static int Receive(char reg_adrs,unsigned char *data,char kosu);

#define IODIRA    0b01111111  // GPA0-GPA6ピンは全て入力(0=OUTPUT 1=INPUT) 7は未使用
#define IODIRB    0b00000000  // GPB0-4、5,6,7も出力
#define IPOLA     0b00000000  // GPA0-GPA7ピンは全て正極性(0=正極性 1=逆極性)
#define IPOLB     0b00000000  // GPB0-GPB7ピンは全て正極性に設定
#define GPPUA     0b01111111  // GPA0-GPA6ピンは全てプルアップ有効(0=無効 1=有効)
#define GPPUB     0b00000000  // GPB0は有効、GPB1-GPB7ピンはプルアップ無効とする
// 割り込みレジスタ関連のコンフィグ
#define GPINTENA    0b00000000  // GPA0-GPA7ピンは全て割込み無効(0=無効 1=有効)
#define GPINTENB    0b00000000  // GPB0-GPB7ピンは全て割込み無効に設定
#define DEFVALA     0b00000000  // GPAピン(0="１"で割込み  1="０"で割込み)
#define DEFVALB     0b00000000  // GPBピン割込み時の方向比較値
#define INTCONA     0b00000000  // GPAピン(0=変化時に割込み  1=DEFVAL値で割込み)
#define INTCONB     0b00000000  // GPBピン変化方向の割込み制御
// I/Oコンフィグレーションレジスタ
// BANK0,自動インクリメント有効,SDAのスルーレートON,INT出力はアクティブHIGH,INTA/INTB個別出力
#define IOCON     0b00000010


// デバイスのレジスタアドレス(BANK=0)
#define IODIRA_ADRS   0x00      // GPAピンのI/O方向レジスタアドレス
#define IODIRB_ADRS   0x01      // GPBピンのI/O方向レジスタアドレス
#define IPOLA_ADRS    0x02      // GPAピンの入力極性ポートレジスタアドレス
#define IPOLB_ADRS    0x03      // GPBピンの入力極性ポートレジスタアドレス
#define GPPUA_ADRS    0x0C      // GPAピンのプルアップレジスタアドレス
#define GPPUB_ADRS    0x0D      // GPBピンのプルアップレジスタアドレス
#define GPIOA_ADRS    0x12      // GPAピンのポートレジスタアドレス
#define GPIOB_ADRS    0x13      // GPBピンのポートレジスタアドレス
#define OLATA_ADRS    0x14      // GPAピンのラッチレジスタアドレス
#define OLATB_ADRS    0x15      // GPBピンのラッチレジスタアドレス
// 状態変化割込み関連レジスタアドレス(BANK=0)
#define GPINTENA_ADRS 0x04      // GPAピンの状態変化割込み許可レジスタアドレス
#define GPINTENB_ADRS 0x05      // GPBピンの状態変化割込み許可レジスタアドレス
#define DEFVALA_ADRS  0x06      // GPAピンの変化方向比較レジスタアドレス
#define DEFVALB_ADRS  0x07      // GPBピンの変化方向比較レジスタアドレス
#define INTCONA_ADRS  0x08      // GPAピンの状態変化割込み制御レジスタアドレス
#define INTCONB_ADRS  0x09      // GPBピンの状態変化割込み制御レジスタアドレス
#define INTFA_ADRS    0x0E      // GPAピンの割込みフラグレジスタアドレス
#define INTFB_ADRS    0x0F      // GPBピンの割込みフラグレジスタアドレス
#define INTCAPA_ADRS  0x10      // GPAピンの割込み時のビット値レジスタアドレス
#define INTCAPB_ADRS  0x11      // GPBピンの割込み時のビット値レジスタアドレス
// I/Oコンフィグレーションレジスタアドレス(BANK=0)
#define IOCON_ADRS    0x0A      // I/Oコンフィグレーションレジスタアドレス


#include <Wire.h>

// デバイスアドレス
static int device_adrs = 0b0100000;

// mcp23017の初期化
void mcp23017_init()
{
  Wire.begin();

  int  ans ;
  unsigned char data[10] ;

  // I/Oコンフィグレーションを行う
  data[0] = IOCON ;
  ans = Send(IOCON_ADRS,data,1) ;
  if ( ans == 0 )
  {
    data[0] = IODIRA;
    data[1] = IODIRB;
    data[2] = IPOLA;
    data[3] = IPOLB;
    data[4] = GPINTENA;     // GPA0-GPA7ピンの状態変化割込み許可レジスタ値
    data[5] = GPINTENB;     // GPB0-GPB7ピンの状態変化割込み許可レジスタ値
    data[6] = DEFVALA;      // GPA0-GPA7ピンの変化方向比較レジスタ値
    data[7] = DEFVALB;      // GPB0-GPB7ピンの変化方向比較レジスタ値
    data[8] = INTCONA;      // GPA0-GPA7ピンの状態変化割込み制御レジスタ値
    data[9] = INTCONB;      // GPB0-GPB7ピンの状態変化割込み制御レジスタ値
    Send(IODIRA_ADRS,data,10) ;
    data[0] = GPPUA ;        // GPA0-GPA7ピンのプルアップレジスタ値
    data[1] = GPPUB ;        // GPB0-GPB7ピンのプルアップレジスタ値
    Send(GPPUA_ADRS,data,2) ;
  }
}

/*******************************************************************************
*  ans = Send(reg_adrs,*data,kosu)                                             *
*  デバイスに指定個数のデータを送信する処理                                    *
*    reg_adrs : 書出すデータのレジスタアドレスを指定する                       *
*               連続的に書出す場合は、書出すレジスタの先頭アドレスを指定       *
*    *data    : 書出すデータの格納先を指定する                                 *
*    kosu     : 書出すデータのバイト数を指定する                               *
*    ans      : 戻り値、0=正常終了　それ以外 Init() のans値を参照              *
*******************************************************************************/
static int Send(char reg_adrs,unsigned char *data,char kosu)
{
     int  ans , i ;

     Wire.beginTransmission(device_adrs) ;        // 通信の開始
     
     Wire.write(reg_adrs) ;                       // レジスタのアドレスを通知
     for (i=0 ; i<kosu ; i++) {
          Wire.write(*data) ;                     // データを通知
          data++ ;
     }
     
     ans = Wire.endTransmission() ;               // データの送信と通信の終了
     return ans ;
}
/*******************************************************************************
*  ans = Receive(reg_adrs,*data,kosu)                                          *
*  デバイスから指定個数のデータを受信する処理                                  *
*    reg_adrs : 読出すデータのレジスタアドレスを指定する                       *
*               連続的に読出す場合は、読出すレジスタの先頭アドレスを指定       *
*    *data    : 読出したデータの格納先を指定する                               *
*    kosu     : 読出すデータのバイト数を指定する                               *
*    ans      : 0=正常　1=異常(相手からACKが返ってこない)                      *
*    ans      : 戻り値、0=正常終了　それ以外 Init() のans値を参照              *
*******************************************************************************/
int Receive(char reg_adrs,unsigned char *data,char kosu)
{
     int  ans , i ;

     Wire.beginTransmission(device_adrs) ;             // 通信の開始
     Wire.write(reg_adrs) ;                            // レジスタアドレスを指定
     ans = Wire.endTransmission() ;                    // データの送信と通信の終了
     if (ans == 0) {
          ans = Wire.requestFrom(device_adrs,kosu) ;   // データの受信を行う
          if (ans == kosu) {
               for (i=0 ; i<kosu ; i++) {
                    *data = Wire.read() ;
                    data++ ;
               }
               ans = 0 ;
          } else ans = 5 ;                             // 受信バイト数がおかしい？
     }
     return ans ;
}

/*******************************************************************************
*  pinMode(port,pin,mode)                                                      *
*  指定ピンの動作を入力か出力に設定する処理                                    *
*    port  : ポートを指定する  0=GPA  1=GPB                                    *
*    pin   : ピンのビットの位置、右端(LSB)から数えて何ビット目か               *
*    mode  : 動作モードを指定 INPUT/OUTPUT/INPUT_PULLUP                        *
*******************************************************************************/
void pinMode(int port,int pin,uint8_t mode)
{
     unsigned char dt[4] ;
     int x1 , x2 , a1 , a2 ;

     if (port == 0) {
          // GPAポートへのアクセス
          a1 = IODIRA_ADRS ;
          a2 = GPPUA_ADRS ;
     } else {
          // GPBポートへのアクセス
          a1 = IODIRB_ADRS ;
          a2 = GPPUB_ADRS ;
     }
     Receive(a1,&dt[1],1) ;        // 現状のI/O方向レジスタを読み出す
     x1 = dt[1] ;
     Receive(a2,&dt[2],1) ;        // 現状のプルアップレジスタを読み出す
     x2 = dt[2] ;
     x2 &= ~(1 << pin) ;           // 一旦プルアップ無効でセット
     if (mode == OUTPUT) {
          // 出力モードの設定を行う
          x1 &= ~(1 << pin) ;      // 指定ピンはビット0(output)
     } else {
          // 入力モードの設定を行う
          x1 |= (1 << pin) ;       // 指定ピンはビット1(input)
          if (mode == INPUT_PULLUP) {
               // プルアップは有効に設定を行う
               x2 |= (1 << pin) ;
          }
     }
     dt[1] = x1 ;
     Send(a1,&dt[1],1) ;           // I/O方向レジスタに書き込む
     dt[2] = x2 ;
     Send(a2,&dt[2],1) ;           // プルアップレジスタに書き込む
}

/*******************************************************************************
*  Write(port,pin,value)                                                       *
*  指定したピンに出力を行う処理                                                *
*    port  : ポートを指定する  0=GPA  1=GPB                                    *
*    pin   : セットするビットの位置、右端(LSB)から数えて何ビット目か           *
*    value : セットする対象となる数値、０or１                                  *
*******************************************************************************/
void Write(int port,int pin,int value)
{
     unsigned char dt[2] ;
     int x , a1 , a2 ;

     if (port == 0) {
          // GPAポートへのアクセス
          a1 = GPIOA_ADRS ;
          a2 = OLATA_ADRS ;
     } else {
          // GPBポートへのアクセス
          a1 = GPIOB_ADRS ;
          a2 = OLATB_ADRS ;
     }
     Receive(a1,dt,1) ;                 // 現状を読み出す
     x = dt[0] ;
     if (value == 1) x |= (1 << pin) ;  // ビットを１にする
     else            x &= ~(1 << pin) ; // ビットを０にする
     dt[0] = x ;
     Send(a2,dt,1) ;                    // GPAポートに書き込む
}
/*******************************************************************************
*  ans = Read(port,pin)                                                        *
*  指定したピンの状態を得る処理                                                *
*    port  : ポートを指定する  0=GPA  1=GPB                                    *
*    pin   : 読み取るビットの位置、右端(LSB)から数えて何ビット目か             *
*    ans   : 返す値は読み取ったビット値 ０か１                                 *
*******************************************************************************/
int Read(int port,int pin)
{
     unsigned char dt[2] ;
     int x , a1 ;

     if (port == 0) {
          // GPAポートから読み込む
          a1 = GPIOA_ADRS ;
     } else {
          // GPBポートから読み込む
          a1 = GPIOB_ADRS ;
     }
     Receive(a1,dt,1) ;
     x = dt[0] ;
     return (x >> pin) & 0x01 ;
}


