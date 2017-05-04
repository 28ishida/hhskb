/*******************************************************************************
*  skMCP230 - MCP23017(I2C)関数ライブラリ                                      *
*             このデバイスは、16チャンネルの汎用I/Oに拡張出来るICです。        *
*                                                                              *
*    skMCP230 - この関数ライブラリを生成する時の初期化処理                     *
*    Init     - デバイスの初期化を行う処理                                     *
*    pinMode  - 指定ピンの動作を入力か出力に設定する処理                       *
*    Read     - 指定したピンの状態を得る処理                                   *
*    Write    - 指定したピンに出力を行う処理                                   *
*    ReadINTF - 指定したピンの割り込み状態(INTF)を得る処理                     *
*    Send     - デバイスに指定個数のデータを送信する処理                       *
*    Receive  - デバイスから指定個数のデータを受信する処理                     *
* ============================================================================ *
*   VERSION  DATE        BY             CHANGE/COMMENT                         *
* ---------------------------------------------------------------------------- *
*   1.00     2015-11-15  きむ茶工房     Create                                 *
* ============================================================================ *
*   Arduino IDE 1.7.3 (Ardino Duemilanove 328/UNO ZERO/M0)                     *
*******************************************************************************/
#include <Wire.h>
#include "arduino.h"
#include "skMCP230.h"


/*******************************************************************************
*  skMCP230(address)                                                           *
*    この関数ライブラリを生成する時の初期化処理(コンストラクタ)                *
*    address : デバイス(スレーブ)のI2Cアドレスを指定します                     *
*******************************************************************************/
skMCP230::skMCP230(int address)
{
     device_adrs = address ;
}
/*******************************************************************************
*  ans = Init()                                                                *
*    デバイスの初期化を行う処理                                                *
*    デバイスのピン情報は"skMCP230.h"に記述します。                            *
*    ans  : 戻り値、0=正常終了　それ以外I2C通信エラー下記                      *
*                   1=送ろうとしたデータが送信バッファのサイズを超えた         *
*                   2=スレーブ・アドレスを送信し、NACKを受信した               *
*                   3=データ・バイトを送信し、NACKを受信した                   *
*                   4=その他のエラー                                           *
*                   5=データ受信エラー                                         *
*******************************************************************************/
int skMCP230::Init()
{
     int  ans ;
     unsigned char data[10] ;

     // I/Oコンフィグレーションを行う
     data[0] = IOCON ;
     ans = Send(IOCON_ADRS,data,1) ;
     if (ans == 0) {
          // デバイスのピン情報を設定する
          data[0] = IODIRA ;       // GPA0-GPA7ピンのI/O方向レジスタ値
          data[1] = IODIRB ;       // GPB0-GPB7ピンのI/O方向レジスタ値
          data[2] = IPOLA ;        // GPA0-GPA7ピンの入力極性ポートレジスタ値
          data[3] = IPOLB ;        // GPB0-GPB7ピンの入力極性ポートレジスタ値
          // ピンの割り込み情報を設定する
          data[4] = GPINTENA ;     // GPA0-GPA7ピンの状態変化割込み許可レジスタ値
          data[5] = GPINTENB ;     // GPB0-GPB7ピンの状態変化割込み許可レジスタ値
          data[6] = DEFVALA ;      // GPA0-GPA7ピンの変化方向比較レジスタ値
          data[7] = DEFVALB ;      // GPB0-GPB7ピンの変化方向比較レジスタ値
          data[8] = INTCONA ;      // GPA0-GPA7ピンの状態変化割込み制御レジスタ値
          data[9] = INTCONB ;      // GPB0-GPB7ピンの状態変化割込み制御レジスタ値
          Send(IODIRA_ADRS,data,10) ;
          data[0] = GPPUA ;        // GPA0-GPA7ピンのプルアップレジスタ値
          data[1] = GPPUB ;        // GPB0-GPB7ピンのプルアップレジスタ値
          Send(GPPUA_ADRS,data,2) ;
          // デバイスのI/Oに入出力問わず０を出力して置く
          data[0] = data[1] = 0 ;
          Send(OLATA_ADRS,data,2) ;
          // 状態変化割込みをクリアして置く
          while(1) {
               Receive(INTFA_ADRS,data,2) ;
               if (data[0]==0 && data[1]==0) break ;
               Receive(INTCAPA_ADRS,data,2) ;
          }
     }

     return ans ;
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
int skMCP230::Send(char reg_adrs,unsigned char *data,char kosu)
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
int skMCP230::Receive(char reg_adrs,unsigned char *data,char kosu)
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
void skMCP230::pinMode(int port,int pin,uint8_t mode)
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
void skMCP230::Write(int port,int pin,int value)
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
int skMCP230::Read(int port,int pin)
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
/*******************************************************************************
*  ans = ReadINTF(port)                                                        *
*  指定したポートの割り込み状態(INTF)を得る処理                                *
*    port  : ポートを指定する  0=GPA  1=GPB                                    *
*    ans   : 返す値はINTFレジスタの値                                          *
*******************************************************************************/
char skMCP230::ReadINTF(int port)
{
     unsigned char dt[2] ;
     int a1 , a2 ;

     if (port == 0) {
          // GPA割り込みフラグから読み込む
          a1 = INTFA_ADRS ;
          a2 = INTCAPA_ADRS ;
     } else {
          // GPB割り込みフラグから読み込む
          a1 = INTFB_ADRS ;
          a2 = INTCAPB_ADRS ;
     }
     Receive(a1,&dt[0],1) ;   // INTFレジスタを読込む
     Receive(a2,&dt[1],1) ;   // INTCAPレジスタを読込む(割込みのクリア)

     return dt[0] ;
}