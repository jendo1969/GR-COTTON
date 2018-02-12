const int PIR = 5;

const int LED = 4;
const int STATE_ON = 1;
const int STATE_OFF = 0;

int PIR_STATE;

// 赤外線リモコンのOFF・ONデータ
int data[2][39] = {
 //  1   2  3  4  5  6  7   8  9 10 11  12 13  14 15 16 17  18 19   20  21  22 23 24 25 26 27  28 29 30 31  32 33  34 35 36 37  38 39  
  {273,252,98,70,96,72,96,175,96,72,96,175,96,172,99,72,96,175,97,5378,270,252,96,72,96,71,97,175,96,72,96,175,96,175,96,71,96,175,96},
  {262,257,91,79,88,79,89,79,89,183,88,183,89,182,89,180,91,79,89,5385,262,260,88,77,91,79,89,79,88,182,89,183,88,183,88,182,89,79,89}
};

/**
 * setup
 * 起動時に一度だけ実行
 */
void setup()
{
  Serial.begin(9600);
    
  // LED接続ピンを出力に設定
  pinMode(LED, OUTPUT);

  // 焦電センサ接続ピンを入力に設定
  pinMode(PIR, INPUT);
}

/**
 * loop
 * 繰り返し実行
 */
void loop()
{
  PIR_STATE = digitalRead(PIR);
  
  if(PIR_STATE == LOW)
  {
    // ON 
    sendSignal(STATE_ON);
    Serial.print("ON\n");
  }
  else
  {
    // OFF
    sendSignal(STATE_OFF);
    Serial.print("OFF\n");
  }
  
  // 3秒間スリープ
  delay(3000);
}

// リモコン信号を送信
void sendSignal(int mode) {
  int dataSize = sizeof(data[mode]) / sizeof(data[mode][0]);
  for (int cnt = 0; cnt < dataSize; cnt++) {
    unsigned long len = data[mode][cnt]*10;
    unsigned long us = micros();
    do {
      digitalWrite(LED, (cnt%2) ? HIGH : LOW);
      delayMicroseconds(8);
      digitalWrite(LED, HIGH);
      delayMicroseconds(7);
    } while (long(us + len - micros()) > 0);
  }
}



