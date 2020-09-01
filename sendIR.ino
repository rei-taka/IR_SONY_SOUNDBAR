#include <M5StickC.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>
 
const uint16_t kIrLed = 9;              // M5StickCはGPIO9にIRが内蔵
 
IRsend irsend(kIrLed);                  // IR送信を宣言
 
const uint32_t CUSTOMER_CODE = 0x00ff;  // カスタマーコードをセット
 
// リモコンコード保存用構造体
struct REMOTE {
  char name[11];    // コマンドの名称（適当）
  uint64_t command; // 送信コード
  uint32_t bit;     // 送信コードビット数
};
 
/* リモコンコード一覧
*   SONY サウンドバー HT-CT660
*   手探りで探したコマンド。
*   コマンドは他にもあると思われる。
*   また、機能の重複もある。
*/
REMOTE remote[] = {
  { "POWER"     , 0x540C, 15 }, // 電源ON/OFF
  { "VOL+"      , 0x240C, 15 }, // 音量UP
  { "VOL-"      , 0x640C, 15 }, // 音量DOWN
  { "DISP"      , 0x040D, 15 }, // ディスプレイ表示切替
  { "MODE"      , 0x3B0D, 15 }, // サウンドフィールド変更
  { "NITH"      , 0x020D, 15 }, // ナイトモード切替
  { "AMP MENU"  , 0x770D, 15 }, // アンプメニュー
  { "DOWN"      , 0x4f0D, 15 }, // 十字キー操作 下
  { "UP"        , 0x0f0D, 15 }, // 十字キー操作 上
  { "RIGHT"     , 0x6f0D, 15 }, // 十字キー操作 右
  { "LEFT"      , 0x2f0D, 15 }, // 十字キー操作 左
  { "SELECT"    , 0x180C, 15 }, // 決定キー
/* M5SticCでは画面に収まりきらないのでコメントアウトした。
  { "POWER ON"  , 0x7A0C, 15 }, // 電源ON
  { "POWER OFF" , 0x3A0C, 15 }, // 電源OFF
  { "MUTE"      , 0x140C, 15 }, // ミュート
  { "TV"        , 0x2B0C, 15 }, // 入力をTVに変更
  { "SF+"       , 0x3B0D, 15 }, // サウンドフィールド変更 
  { "SF-"       , 0x7B0D, 15 }, // サウンドフィールド変更 逆順
  { "MUSIC"     , 0x490D, 15 }, // サウンドフィールド 変更 MUSIC
  { "VOICE+"    , 0x150D, 15 }, // ボイスレベル調整UP
  { "VOICE-"    , 0x550D, 15 }, // ボイスレベル調整DOWN
  { "INPUT"     , 0x4B0D, 15 }, // 入力切替
  { "INPUT-"    , 0x2B0D, 15 }, // 入力変更　逆順
*/
};
 
int cursor = 0; // カーソル位置
 
void setup() {
  M5.begin();     // M5StickC初期化
  irsend.begin(); // IR初期化
 
  // リモコン項目表示
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 8);
  for ( int i = 0 ; i < ( sizeof(remote) / sizeof(REMOTE) ) ; i++ ) {
    M5.Lcd.print((cursor == i) ? ">" : " ");
    M5.Lcd.println(remote[i].name);
  }
}
 
void loop() {
  M5.update();  // ボタン状態更新
 
  // M5ボタンで送信
  if ( M5.BtnA.wasPressed() ) {
    // 送信
    irsend.sendSony(remote[cursor].command, remote[cursor].bit);
  }
 
  // 右ボタンでカーソル移動
  if ( M5.BtnB.wasPressed() ) {
    cursor++;
    cursor = cursor % ( sizeof(remote) / sizeof(REMOTE) );
 
    // カーソル描画
    M5.Lcd.setCursor(0, 8);
    for ( int i = 0 ; i < ( sizeof(remote) / sizeof(REMOTE) ) ; i++ ) {
      M5.Lcd.println((cursor == i) ? ">" : " ");
    }
  }
  delay(100);
}
