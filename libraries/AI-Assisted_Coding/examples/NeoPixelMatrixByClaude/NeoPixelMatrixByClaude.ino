// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Warning / 警告
// This code was created by AI. Make sure the code is safe before running it.
// このコードはAIに作成されました。コードの安全性を十分に確認してから実行してください。
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// - Claude log (Japanese prompts): https://claude.ai/share/85963dff-45e9-4b50-a371-06f847270c01
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

/*
 * UIAPduino NeoPixel 8x8マトリックス キラキラスケッチ
 * 暗めの青色でランダムにキラキラ光る
 * 
 * 接続:
 * - NeoPixelマトリックスのDIN -> UIAPduino D2ピン
 * - NeoPixelマトリックスのVCC -> UIAPduino 5V
 * - NeoPixelマトリックスのGND -> UIAPduino GND
 * 
 * 必要ライブラリ: Adafruit NeoPixel
 */

#include <Adafruit_NeoPixel.h>

// 設定
#define LED_PIN     2     // NeoPixelが接続されているピン
#define LED_COUNT   64    // LEDの総数 (8x8 = 64)
#define BRIGHTNESS  50    // 明るさ (0-255)

// NeoPixelオブジェクトを作成
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// キラキラ用の変数
int twinkleSpeed = 100;        // キラキラの速さ（ミリ秒）
int maxTwinkles = 8;          // 同時に光るLEDの最大数
int twinklePixels[64];        // 各LEDの明度を記録
unsigned long lastUpdate = 0;  // 最後の更新時間

void setup() {
  // シリアル通信開始（デバッグ用）
  Serial.begin(9600);
  Serial.println("NeoPixel キラキラスケッチ開始");
  
  // NeoPixel初期化
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // 全て消灯
  
  // 乱数の初期化
  randomSeed(analogRead(0));
  
  // twinklePixels配列を初期化
  for(int i = 0; i < LED_COUNT; i++) {
    twinklePixels[i] = 0;
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  // 指定した時間が経過したら更新
  if(currentTime - lastUpdate >= twinkleSpeed) {
    
    // 既存のキラキラを減衰させる
    for(int i = 0; i < LED_COUNT; i++) {
      if(twinklePixels[i] > 0) {
        twinklePixels[i] -= 5; // 徐々に暗くする
        if(twinklePixels[i] < 0) {
          twinklePixels[i] = 0;
        }
      }
    }
    
    // 新しいキラキラを追加（確率的に）
    if(random(0, 3) == 0) { // 約33%の確率で新しいキラキラ
      int activeTwinkles = countActiveTwinkles();
      if(activeTwinkles < maxTwinkles) {
        int randomPixel = random(0, LED_COUNT);
        // そのLEDがまだ光っていなければ
        if(twinklePixels[randomPixel] == 0) {
          twinklePixels[randomPixel] = random(100, 255); // ランダムな明度
        }
      }
    }
    
    // LEDの色を更新
    updateLEDs();
    
    lastUpdate = currentTime;
  }
}

// アクティブなキラキラの数をカウント
int countActiveTwinkles() {
  int count = 0;
  for(int i = 0; i < LED_COUNT; i++) {
    if(twinklePixels[i] > 0) {
      count++;
    }
  }
  return count;
}

// LEDの色を更新する関数
void updateLEDs() {
  for(int i = 0; i < LED_COUNT; i++) {
    if(twinklePixels[i] > 0) {
      // 暗めの青色を設定
      // RGB値を調整して暗い青にする
      int brightness = twinklePixels[i];
      int red = 0;                           // 赤成分なし
      int green = brightness / 8;            // 少し緑を混ぜて自然な青に
      int blue = brightness;                 // 青成分をメインに
      
      strip.setPixelColor(i, strip.Color(red, green, blue));
    } else {
      // 消灯
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
  
  // LEDに反映
  strip.show();
}

/* 
 * 追加の関数例：
 * 以下の関数を呼び出すことで効果を変更できます
 */

// より激しいキラキラ効果
void fastTwinkle() {
  twinkleSpeed = 50;
  maxTwinkles = 15;
}

// ゆっくりしたキラキラ効果  
void slowTwinkle() {
  twinkleSpeed = 200;
  maxTwinkles = 5;
}

// 全消灯
void clearAll() {
  strip.clear();
  strip.show();
  for(int i = 0; i < LED_COUNT; i++) {
    twinklePixels[i] = 0;
  }
}