// MCP23017-I/O拡張ICのテスト
// SW(GPB0)を押している間だけLED(GPA7)を点灯させる
#include <Wire.h>
#include <skMCP230.h>

// デバイスの７ビットアドレス(0100A2A1A0)
#define MCP230_ADDRESS	0b0100000		// MCP23017のI2Cアドレス(A2=0,A1=0,A0=0)

skMCP230 expIO(MCP230_ADDRESS) ;        // MCP23017ライブラリの生成を行う

void setup()
{
     // Ｉ２Ｃの初期化
     Wire.begin() ;                     // マスターとする
     // MCP23017の初期化(ピンは全て出力で初期化されます)
     expIO.Init() ;
     expIO.pinMode(GPB,GPB0,INPUT_PULLUP) ;
}
void loop()
{
     if (expIO.Read(GPB,GPB0) == 0) {   // SWは押されたか？
          expIO.Write(GPA,GPA7,HIGH) ;  // SW=ON ならLED点灯
     } else expIO.Write(GPA,GPA7,LOW) ; // LED消灯
}
