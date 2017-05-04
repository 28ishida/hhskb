/*******************************************************************************
*  skMCP230.h - MCP23017(I2C)関数ライブラリのヘッダファイル                    *
*                                                                              *
* ============================================================================ *
*   VERSION  DATE        BY                    CHANGE/COMMENT                  *
* ---------------------------------------------------------------------------- *
*   1.00     2015-11-15  きむ茶工房     Create                                 *
* ============================================================================ *
*   Arduino IDE 1.7.3 (Ardino Duemilanove 328/UNO ZERO/M0)                     *
*******************************************************************************/
#ifndef _SKMCP230_H_
#define _SKMCP230_H_

#include "arduino.h"


// デバイスのＩ／Ｏレジスタコンフィグ
#define IODIRA			0b00000000	// GPA0-GPA7ピンは全て出力(0=OUTPUT 1=INPUT)
#define IODIRB			0b00000000	// GPB0は入力、GPB1-GPB7ピンは出力に割当てる
#define IPOLA			0b00000000	// GPA0-GPA7ピンは全て正極性(0=正極性 1=逆極性)
#define IPOLB			0b00000000	// GPB0-GPB7ピンは全て正極性に設定
#define GPPUA			0b00000000	// GPA0-GPA7ピンは全てプルアップ無効(0=無効 1=有効)
#define GPPUB			0b00000000	// GPB0は有効、GPB1-GPB7ピンはプルアップ無効とする
// 割り込みレジスタ関連のコンフィグ
#define GPINTENA		0b00000000	// GPA0-GPA7ピンは全て割込み無効(0=無効 1=有効)
#define GPINTENB		0b00000000	// GPB0-GPB7ピンは全て割込み無効に設定
#define DEFVALA 		0b00000000	// GPAピン(0="１"で割込み  1="０"で割込み)
#define DEFVALB 		0b00000001	// GPBピン割込み時の方向比較値
#define INTCONA 		0b00000000	// GPAピン(0=変化時に割込み  1=DEFVAL値で割込み)
#define INTCONB 		0b00000001	// GPBピン変化方向の割込み制御
// I/Oコンフィグレーションレジスタ
// BANK0,自動インクリメント有効,SDAのスルーレートON,INT出力はアクティブHIGH,INTA/INTB個別出力
#define IOCON			0b00000010


// デバイスのレジスタアドレス(BANK=0)
#define IODIRA_ADRS		0x00			// GPAピンのI/O方向レジスタアドレス
#define IODIRB_ADRS		0x01			// GPBピンのI/O方向レジスタアドレス
#define IPOLA_ADRS		0x02			// GPAピンの入力極性ポートレジスタアドレス
#define IPOLB_ADRS		0x03			// GPBピンの入力極性ポートレジスタアドレス
#define GPPUA_ADRS		0x0C			// GPAピンのプルアップレジスタアドレス
#define GPPUB_ADRS		0x0D			// GPBピンのプルアップレジスタアドレス
#define GPIOA_ADRS		0x12			// GPAピンのポートレジスタアドレス
#define GPIOB_ADRS		0x13			// GPBピンのポートレジスタアドレス
#define OLATA_ADRS		0x14			// GPAピンのラッチレジスタアドレス
#define OLATB_ADRS		0x15			// GPBピンのラッチレジスタアドレス
// 状態変化割込み関連レジスタアドレス(BANK=0)
#define GPINTENA_ADRS	0x04			// GPAピンの状態変化割込み許可レジスタアドレス
#define GPINTENB_ADRS	0x05			// GPBピンの状態変化割込み許可レジスタアドレス
#define DEFVALA_ADRS	0x06			// GPAピンの変化方向比較レジスタアドレス
#define DEFVALB_ADRS	0x07			// GPBピンの変化方向比較レジスタアドレス
#define INTCONA_ADRS	0x08			// GPAピンの状態変化割込み制御レジスタアドレス
#define INTCONB_ADRS	0x09			// GPBピンの状態変化割込み制御レジスタアドレス
#define INTFA_ADRS  	0x0E			// GPAピンの割込みフラグレジスタアドレス
#define INTFB_ADRS  	0x0F			// GPBピンの割込みフラグレジスタアドレス
#define INTCAPA_ADRS	0x10			// GPAピンの割込み時のビット値レジスタアドレス
#define INTCAPB_ADRS	0x11			// GPBピンの割込み時のビット値レジスタアドレス
// I/Oコンフィグレーションレジスタアドレス(BANK=0)
#define IOCON_ADRS		0x0A			// I/Oコンフィグレーションレジスタアドレス

// ピンの定数定義
#define GPA			0
#define GPA0			0
#define GPA1			1
#define GPA2			2
#define GPA3			3
#define GPA4			4
#define GPA5			5
#define GPA6			6
#define GPA7			7
#define GPB			1
#define GPB0			0
#define GPB1			1
#define GPB2			2
#define GPB3			3
#define GPB4			4
#define GPB5			5
#define GPB6			6
#define GPB7			7


// 関数のプロトタイプ宣言
/*******************************************************************************
*	クラスの定義                                                           *
*******************************************************************************/
class skMCP230
{
  private:
     int  device_adrs ;

     int  Send(char reg_adrs,unsigned char *data,char kosu) ;
     int  Receive(char reg_adrs,unsigned char *data,char kosu) ;

  public:
          skMCP230(int address) ;
     int  Init() ;
     void pinMode(int port,int pin,uint8_t mode) ;
     void Write(int port,int pin,int value) ;
     int  Read(int port,int pin) ;
     char ReadINTF(int port) ;
} ;

#endif
