// MCP23017-I/O拡張ICのテスト
// SW(GPB0)を状態変化割込み(INTB)に設定しRA0:PICで受ける、割込み発生でLED(RA5)を点灯させる
#include <Wire.h>
#include <skMCP230.h>

// デバイスの７ビットアドレス(0100A2A1A0)
#define MCP230_ADDRESS	0b0100000	// MCP23017のI2Cアドレス(A2=0,A1=0,A0=0)

skMCP230 expIO(MCP230_ADDRESS) ;   // MCP23017ライブラリの生成を行う

void setup()
{
     pinMode(2,INPUT) ;            // MCP23017のINTBピンに接続
     pinMode(13,OUTPUT) ;
     // Ｉ２Ｃの初期化
     Wire.begin() ;                // マスターとする
     // MCP23017の初期化(ピンは全て出力で初期化されます)
     // 割り込みの設定は"skMCP230.h"に記述の#defineを変更します
     expIO.Init() ;
     expIO.pinMode(GPB,GPB0,INPUT_PULLUP) ;
     expIO.Write(GPA,GPA7,HIGH) ;  // LED点灯(起動確認用)
}
void loop()
{
     if (digitalRead(2) == HIGH) { // INTBの割込み発生か？
          // 割込み情報(INTF)を読込み、割込みもクリアする
          expIO.ReadINTF(GPB) ;
          digitalWrite(13,HIGH) ;
          delay(1000) ;            // １秒間LED点灯
     } else {
          digitalWrite(13,LOW) ;   // 割込みが発生していない時はLED消灯
     }
}
