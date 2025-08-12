// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Warning / 警告
// This code was created by AI. Make sure the code is safe before running it.
// このコードはAIに作成されました。コードの安全性を十分に確認してから実行してください。
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// - Gemini log (Japanese prompts): https://g.co/gemini/share/03447b343db2
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_PIXELS 64

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.show(); // すべてのLEDをオフに
  randomSeed(analogRead(A0)); // ランダムシードを初期化
}

void loop() {
  // すべてのLEDをオフにする
  pixels.clear();

  // ランダムなLEDを淡い青色で点灯させる
  for(int i=0; i<10; i++) { // 同時に点灯させるLEDの数
    int pixelId = random(NUM_PIXELS);
    int brightness = random(5, 30); // 輝度のランダム化 (5-30の間)
    pixels.setPixelColor(pixelId, pixels.Color(0, 0, brightness));
  }

  pixels.show();
  delay(100); // 点灯時間 (ミリ秒)
}