// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Warning / 警告
// This code was created by AI. Make sure the code is safe before running it.
// このコードはAIに作成されました。コードの安全性を十分に確認してから実行してください。
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// - ChatGPT log (Japanese prompts): https://chatgpt.com/share/68959f6a-a920-8000-a119-877f27bdbf5b
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

#include <Adafruit_NeoPixel.h>
#define PIN        4      // データ出力ピン（適宜変更可）
#define NUMPIXELS 64      // LEDの総数（8x8）

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(30);  // 明るさをかなり抑えて暗めに
  strip.show();             // 初期化：全LED消灯
}

void loop() {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (random(10) < 2) {  // 約20%の確率で光る
      strip.setPixelColor(i, strip.Color(0, 0, 32)); // 暗め青
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));  // 消灯
    }
  }
  strip.show();
  delay(100);  // 少し間隔を開けてキラキラ感を演出
}
