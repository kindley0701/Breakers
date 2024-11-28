#include <SoftwareSerial.h>
// #include <DFRobotDFPlayerMini.h>

//シリアル通信用のピンの定義
SoftwareSerial mySerial(10, 11);
// DFRobotDFPlayerMini myDFPlayer;

//グローバル変数の定義
int vol = 0x05; //初期音量
int squareCount = 1; //楽曲の再生・停止切替用パリティ
int ledCount = 0; //LEDの光り方を切り替えるためのカウント
int musicCount = 1; //楽曲番号のカウント：LEDのパターンを変えるため。
 
//ピンアサイン用変数定義
// int ledPin = 9;
int ledPinR = 6; // LED_Rを接続するピン 
int ledPinG = 9; // LED_Gを接続するピン 
int ledPinB = 5; // LED_Bを接続するピン 
// int soundPin = A4; // マイクロフォンモジュールを接続するピン 

void setup() {

  // LEDへの出力設定
  // pinMode(ledPin, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);

  // シリアル通信の開始
  // mySerial.begin(115200);
  mySerial.begin(9600);
  // if (!myDFPlayer.begin(mySerial)) {
  //   Serial.println("DFPlayer Miniの接続に失敗しました。");
  //   while (true);
  // };
  volume(vol);//音量設定0~30：初期値はグローバルで設定
  
  //装着検知
  int AttachmentDetectValue;
  float AttachmentDetectVoltage;
  // 装着するまで先に進まない
  do {
    Serial.println("Dont start");
    AttachmentDetectValue = analogRead(A4); // A5ピンからアナログ値を読み取る
    AttachmentDetectVoltage = AttachmentDetectValue * (5.0 / 1023.0); // 読み取った値を電圧に変換する
    Serial.println(AttachmentDetectVoltage);
  } while(AttachmentDetectVoltage<3);
  
  //装着完了で音楽再生
  play(0x00);//Play the specified audio:0x01-file0001
  
}
 
 
void loop() {

  ledLight(ledCount, musicCount); //LEDの光り方を変える
  // digitalWrite(ledPinB, HIGH); // FETをオンにする
  // 1秒間のデータを集める
  delay(100);
  ledCount += 1;
  
  //電圧値の読み取り
  int UPbtnValue = analogRead(A0); // R2ボタンの出力をA0につなぐ：音量＋用
  float UPbtnVoltage = UPbtnValue * (5.0 / 1023.0); // 読み取った値を電圧に変換する
  int DOWNbtnValue = analogRead(A1); // L2ボタンの出力をA1につなぐ：音量-用
  float DOWNbtnVoltage = DOWNbtnValue * (5.0 / 1023.0); // 読み取った値を電圧に変換する
  int trianglebtnValue = analogRead(A2); // △ボタンの出力をA2につなぐ：音楽
  float trianglebtnVoltage = trianglebtnValue * (5.0 / 1023.0); // 読み取った値を電圧に変換する
  int squarebtnValue = analogRead(A3); // ■ボタンの出力をA3につなぐ
  float squarebtnVoltage = squarebtnValue * (5.0 / 1023.0); // 読み取った値を電圧に変換する
  // int circlebtnValue = analogRead(A4); // 〇ボタンの出力をA4につなぐ
  // float circlebtnVoltage = circlebtnValue * (5.0 / 1023.0); // 読み取った値を電圧に変換する
  // int crossbtnValue = analogRead(A5); // ✖ボタンの出力をA5につなぐ
  // float crossbtnVoltage = crossbtnValue * (5.0 / 1023.0); // 読み取った値を電圧に変換する

  // int sensorValue = analogRead(soundPin); // マイクロフォンモジュールからの値を読み取る   
  // int ledBrightness = map(sensorValue, 0, 1023, 0, 255); // 値を0から255にマッピング   
  // analogWrite(ledPin, ledBrightness); // LEDの明るさを設定

  //電圧値ログ
  Serial.println(UPbtnVoltage);
  Serial.println(DOWNbtnVoltage);
  Serial.println(trianglebtnVoltage);
  Serial.println(squarebtnVoltage);
  Serial.println();
 
  // 上ボタン応答：VolumeUP
  if (UPbtnVoltage>3 && vol < 20) {//音量UP入力があり、現状の音量が30(MAX)未満の場合
    //VolumeUP
    vol += 5;
    Serial.println("Volume UP: " + String(vol));
    volume(vol);
    delay(400);
    ledCount += 4; //LED点灯の時間調整
  };
  
  // 下ボタン応答：VolumeDOWN
  if (DOWNbtnVoltage>3 && vol > 0) {//音量UP入力があり、現状の音量が0(MIN)より大きい場合
    // VolumeDOWN
    vol -= 5;
    Serial.println("Volume DOWN: "+ String(vol));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 tings 0x00-0x1E
    volume(vol);
    delay(400);
    ledCount += 4; //LED点灯の時間調整
  };

  //△ボタン応答：NextMusic
  if (trianglebtnVoltage>3){
    nextMusic();
    if(musicCount >= 3){
      musicCount == 1;
    }else{
      musicCount += 1;
    }
    musicCount += 1;
    delay(400);
    ledCount += 4; //LED点灯の時間調整
  };

  // □ボタン応答：楽曲の停止
  if (squarebtnVoltage>3){
    // Serial.print("Stop Track");
    // myDFPlayer.play(1);
    if (squareCount == 1){ //奇数回で停止
      squareCount = 0; //次押されたら再開する用
      stopTrack(); //停止
      delay(400);
      ledCount += 4; //LED点灯の時間調整
    } else { //偶数回で再開
      squareCount = 1; //次押されたら停止する用
      restartMusic(); //初めから再生
    }
    
  };
}
 
// 関数定義
// 音楽再生(番号指定)
void play(unsigned char Track)
{
  Serial.println("Play Track: "+ String(Track));
  // unsigned char play[6] = {0xAA,0x07,0x02,0x00,Track,Track+0xB3};
  unsigned char play[4] = {0xAA,0x02,0x00,0xAC};
  mySerial.write(play,4);
}

// 次曲再生
void nextMusic()
{
  Serial.println("Play Next Track");
  unsigned char next[4] = {0xAA,0x06,0x00,0xB0};
  mySerial.write(next,4);
}

// 楽曲一時停止
void pauseMusic()
{
  Serial.println("Pause");
  unsigned char pause[4] = {0xAA,0x03,0x00,0xAD};
  mySerial.write(pause,4);
}

// 音楽再生再開
void restartMusic()
{
  Serial.println("Restart");
  unsigned char restart[4] = {0xAA,0x02,0x00,0xAC};
  mySerial.write(restart,4);
}

// 楽曲停止
void stopTrack()
{
  Serial.println("Stop Track");
  unsigned char stop[4] = {0xAA,0x04,0x00,0xAE};
  mySerial.write(stop,4);
}

// 音量変更
void volume( unsigned char vol)
{
  unsigned char volume[5] = {0xAA,0x13,0x01,vol,vol+0xBE};
  mySerial.write(volume,5);
}

// LED点灯用：楽曲によりパターン切替
void ledLight(int ledCount, int musicCount)
{
  if(ledCount % 5 == 0){
    int ledCount5 = ledCount / 5;
    if(musicCount == 3){
        //赤青緑のオンオフ組合せ７通りを繰り返す(全オフなし)
        switch(ledCount5 % 7){
        case 0:
          digitalWrite(ledPinR, HIGH); // FETをオンにする
          digitalWrite(ledPinG, HIGH); // FETをオンにする
          digitalWrite(ledPinB, HIGH); // FETをオンにする
          break;
        case 1:
          digitalWrite(ledPinR, HIGH); // FETをオンにする
          digitalWrite(ledPinG, LOW); // FETをオンにする
          digitalWrite(ledPinB, LOW); // FETをオンにする
          break;
        case 2:
          digitalWrite(ledPinR, LOW); // FETをオンにする
          digitalWrite(ledPinG, HIGH); // FETをオンにする
          digitalWrite(ledPinB, LOW); // FETをオンにする
          break;
        case 3:
          digitalWrite(ledPinR, LOW); // FETをオンにする
          digitalWrite(ledPinG, LOW); // FETをオンにする
          digitalWrite(ledPinB, HIGH); // FETをオンにする
          break;
        case 4:
          digitalWrite(ledPinR, HIGH); // FETをオンにする
          digitalWrite(ledPinG, HIGH); // FETをオンにする
          digitalWrite(ledPinB, LOW); // FETをオンにする
          break;
        case 5:
          digitalWrite(ledPinR, HIGH); // FETをオンにする
          digitalWrite(ledPinG, LOW); // FETをオンにする
          digitalWrite(ledPinB, HIGH); // FETをオンにする
          break;
        case 6:
          digitalWrite(ledPinR, LOW); // FETをオンにする
          digitalWrite(ledPinG, HIGH); // FETをオンにする
          digitalWrite(ledPinB, HIGH); // FETをオンにする
          break;
        // case 7:
        //   digitalWrite(ledPinR, LOW); // FETをオンにする
        //   digitalWrite(ledPinG, LOW); // FETをオンにする
        //   digitalWrite(ledPinB, LOW); // FETをオンにする
        //   break;
        }
    } else {
      //赤と緑を交互に点灯する
      switch(ledCount5 % 2){
        case 0:
          digitalWrite(ledPinR, LOW); // FETをオンにする
          digitalWrite(ledPinG, HIGH); // FETをオンにする
          digitalWrite(ledPinB, LOW); // FETをオンにする
          break;
        case 1:
          digitalWrite(ledPinR, HIGH); // FETをオンにする
          digitalWrite(ledPinG, LOW); // FETをオンにする
          digitalWrite(ledPinB, LOW); // FETをオンにする
          break;
      }
    }
  }
}
