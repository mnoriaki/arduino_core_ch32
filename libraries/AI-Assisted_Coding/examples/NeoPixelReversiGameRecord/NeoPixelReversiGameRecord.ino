// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Warning / 警告
// This code was created by AI. Make sure the code is safe before running it.
// このコードはAIに作成されました。コードの安全性を十分に確認してから実行してください。
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// NeoPixel color LED board that plays Reversi game record.
// - ChatGPT log (Japanese prompts): https://chatgpt.com/share/689aea3f-17dc-8000-af3d-d76582b5aa20
// - Base code: https://chatgpt.com/s/t_689ad842e6388191b68b08856196e7eb
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define WIDTH 8
#define HEIGHT 8
#define NUMPIXELS (WIDTH * HEIGHT)

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const uint8_t coords[][2] = {
  {4,3},{3,3},{3,4},{4,4},{5,4},{5,5},{5,6},{6,6},
  {4,5},{5,7},{7,7},{5,3},{6,4},{7,5},{6,5},{2,5},
  {6,2},{5,2},{4,7},{7,1},{5,1},{6,1},{7,3},{5,0},
  {7,6},{7,4},{6,7},{6,3},{4,2},{4,6},{3,7},{3,1},
  {4,0},{4,1},{6,0},{7,0},{2,0},{3,6},{2,6},{2,7},
  {1,5},{1,6},{0,6},{3,0},{7,2},{2,1},{2,2},{0,5},
  {1,7},{1,3},{1,1},{2,4},{2,3},{0,1},{0,3},{0,4},
  {0,2},{1,4},{0,7},{0,0},{3,2},{1,0},{1,2},{3,5}
};
const int coords_length = sizeof(coords) / sizeof(coords[0]);

// 色定義
const uint32_t COLOR_BLUE     = Adafruit_NeoPixel::Color(0, 0, 255);
const uint32_t COLOR_RED      = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t COLOR_BLUE_DIM = Adafruit_NeoPixel::Color(0, 0, 50);
const uint32_t COLOR_RED_DIM  = Adafruit_NeoPixel::Color(50, 0, 0);
const uint32_t COLOR_WHITE    = Adafruit_NeoPixel::Color(255, 255, 255); // [*手修正] 行追加

// 状態管理
// 0:消灯, 1:青明, 2:赤明, 3:青暗, 4:赤暗
uint8_t ledState[NUMPIXELS] = {0};

int xyToIndex(int x, int y) {
  return y * WIDTH + x;
}

bool inBounds(int x, int y) {
  return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

// RGB値取得用
void colorToRGB(uint32_t c, uint8_t &r, uint8_t &g, uint8_t &b) {
  r = (c >> 16) & 0xFF;
  g = (c >>  8) & 0xFF;
  b =  c        & 0xFF;
}

// フェード反転
void fadeFlipLED(int idx, uint8_t newState, uint32_t targetColor, int steps, int delayMs) {
  uint8_t r1, g1, b1, r2, g2, b2;
  uint32_t currentColor = strip.getPixelColor(idx);
  colorToRGB(currentColor, r1, g1, b1);
  colorToRGB(targetColor, r2, g2, b2);

  for (int i = 1; i <= steps; i++) {
    float t = (float)i / steps;
    uint8_t r = r1 + (r2 - r1) * t;
    uint8_t g = g1 + (g2 - g1) * t;
    uint8_t b = b1 + (b2 - b1) * t;
    strip.setPixelColor(idx, r, g, b);
    strip.show();
    delay(delayMs);
  }
  ledState[idx] = newState;
}

void flipInDirection(int startX, int startY, int dx, int dy, uint8_t myBright, uint8_t myDim) {
  int x = startX + dx;
  int y = startY + dy;
  int path[WIDTH > HEIGHT ? WIDTH : HEIGHT][2];
  int pathLen = 0;

  uint8_t opponentBright = (myBright == 1) ? 2 : 1;
  uint8_t opponentDim    = (myBright == 1) ? 4 : 3;

  uint32_t myBrightColor = (myBright == 1) ? COLOR_BLUE : COLOR_RED;
  uint32_t myDimColor    = (myBright == 1) ? COLOR_BLUE_DIM : COLOR_RED_DIM;

  while (inBounds(x, y)) {
    int idx = xyToIndex(x, y);
    if (ledState[idx] == opponentBright || ledState[idx] == opponentDim) {
      path[pathLen][0] = x;
      path[pathLen][1] = y;
      pathLen++;
      x += dx;
      y += dy;
    }
    else if (ledState[idx] == myBright || ledState[idx] == myDim) {
      // フェードで反転
      for (int i = 0; i < pathLen; i++) {
        int fx = path[i][0];
        int fy = path[i][1];
        int fidx = xyToIndex(fx, fy);
        if (ledState[fidx] == opponentBright) {
          fadeFlipLED(fidx, myBright, myBrightColor, 10, 10); // [*手修正] delayMs 20 -> 10
        }
        else if (ledState[fidx] == opponentDim) {
          fadeFlipLED(fidx, myDim, myDimColor, 10, 10); // [*手修正] delayMs 20 -> 10
        }
      }
      return;
    }
    else {
      return;
    }
  }
}

void flipLikeReversi(int cx, int cy, uint8_t myBright) {
  uint8_t myDim = (myBright == 1) ? 3 : 4;
  int directions[8][2] = {
    {-1,-1},{0,-1},{1,-1},
    {-1,0},        {1,0},
    {-1,1}, {0,1}, {1,1}
  };
  for (int i = 0; i < 8; i++) {
    flipInDirection(cx, cy, directions[i][0], directions[i][1], myBright, myDim);
  }
}

void setup() {
  strip.begin();
  strip.setBrightness(20); // 1/3 輝度 [*手修正] 85 -> 20
  strip.show();
}

void loop() {
  static int current = 0;
  if(current >= coords_length) while(true);

  // 明るいLEDを暗くする
  for(int i=0; i<NUMPIXELS; i++) {
    if (ledState[i] == 1) { ledState[i] = 3; strip.setPixelColor(i, COLOR_BLUE_DIM); }
    else if (ledState[i] == 2) { ledState[i] = 4; strip.setPixelColor(i, COLOR_RED_DIM); }
  }

  // 新しいLEDを点灯
  int x = coords[current][0];
  int y = coords[current][1];
  int idx = xyToIndex(x, y);

  uint8_t myBright;
  if(current % 2 == 0) {
    ledState[idx] = 1;
    fadeFlipLED(idx, 1, COLOR_WHITE, 10, 5); // [*手修正] 行追加
    fadeFlipLED(idx, 1, COLOR_BLUE, 10, 5); // [*手修正] 行追加
    strip.setPixelColor(idx, COLOR_BLUE);
    myBright = 1;
  } else {
    ledState[idx] = 2;
    fadeFlipLED(idx, 2, COLOR_WHITE, 10, 5); // [*手修正] 行追加
    fadeFlipLED(idx, 2, COLOR_RED, 10, 10); // [*手修正] 行追加
    strip.setPixelColor(idx, COLOR_RED);
    myBright = 2;
  }

  strip.show();

  delay(400); // 点灯から反転までの待ち時間（少し長く）

  // フェード反転処理
  flipLikeReversi(x, y, myBright);

  strip.show();
  current++;

  delay(1000); // ★ 次の手まで 2.5秒待機（テンポ遅く） // [*手修正] 2500 -> 1000
}
