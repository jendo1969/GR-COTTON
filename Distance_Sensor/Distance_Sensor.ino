const int PIR = 5;

const int LED = 4;
const int STATE_ON = 1;
const int STATE_OFF = 0;

int PIR_STATE;

const int sensorPin = A5;    // select the input pin for the light sensor
int sensorValue = 0;  // variable to store the value coming from the sensor

int be_cnt = 0;
int no_cnt = 0;

const int off_cnt = 30;
const int on_cnt = 10;

const int distancePin = A2;
int distanceValue = 0;

// 赤外線リモコンのOFF・ONデータ
int data[2][39] = {
  //  1   2  3  4  5  6  7   8  9 10 11  12 13  14 15 16 17  18 19   20  21  22 23 24 25 26 27  28 29 30 31  32 33  34 35 36 37  38 39  
  {
    273,252,98,70,96,72,96,175,96,72,96,175,96,172,99,72,96,175,97,5378,270,252,96,72,96,71,97,175,96,72,96,175,96,175,96,71,96,175,96  }
  ,
  {
    262,257,91,79,88,79,89,79,89,183,88,183,89,182,89,180,91,79,89,5385,262,260,88,77,91,79,89,79,88,182,89,183,88,183,88,182,89,79,89  }
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
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);  
  distanceValue = analogRead(distancePin);  

  PIR_STATE = digitalRead(PIR);
  if(PIR_STATE == LOW)
  {
    be_cnt++;
    no_cnt = 0;
    Serial.print("Be!!:");
    Serial.print(be_cnt);
    Serial.print(" : ");
  }
  else
  {
    no_cnt++;
    be_cnt = 0;
    Serial.print("No Man!!:");
    Serial.print(no_cnt);
    Serial.print(" : ");
  }
//  Serial.println(sensorValue); 
  Serial.print(sensorValue); 
  Serial.print(" : ");
  Serial.println(distanceValue); 

  if(be_cnt >= on_cnt)
  {
    be_cnt = 0;
    if(sensorValue <= 7)
    {
      // ON 
      sendSignal(STATE_ON);
      Serial.println("ON ");
    }
  }

  if(distanceValue > 150)
  {
    no_cnt = 0;
  }
 
  if(no_cnt >= off_cnt)
  {
    no_cnt = 0;
    if(sensorValue > 5)
    {
      // OFF
      sendSignal(STATE_OFF);
      Serial.println("OFF1");
    }
  }

  if(sensorValue > 270)
  {
    // OFF
    sendSignal(STATE_OFF);
    Serial.println("OFF2");
  }

  // 100m秒間スリープ
  delay(100);
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
      delayMicroseconds(17);
    } 
    while (long(us + len - micros()) > 0);
  }
}




