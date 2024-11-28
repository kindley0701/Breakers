#include <ps5Controller.h>

// GPIO設定
// モーター用
int OutPutPWM_pin1 = 32; //右車輪前進用
int OutPutPWM_pin2 = 33; //右車輪後退用
int OutPutPWM_pin3 = 25; //左車輪前進用
int OutPutPWM_pin4 = 26; //左車輪後退用
int OutPutPWM_pin5 = 12; //回転機構右用
int OutPutPWM_pin6 = 14; //回転機構左用
// ボタン出力用
int OutPutPWM_pin11 = 19; //上ボタン用
int OutPutPWM_pin12 = 18; //下ボタン用
int OutPutPWM_pin13 = 17; //△ボタン用
int OutPutPWM_pin14 = 16; //□ボタン用

void setup() {
  // GPIO出力設定
  pinMode(OutPutPWM_pin1, OUTPUT);
  pinMode(OutPutPWM_pin2, OUTPUT);
  pinMode(OutPutPWM_pin3, OUTPUT);
  pinMode(OutPutPWM_pin4, OUTPUT);
  pinMode(OutPutPWM_pin5, OUTPUT);
  pinMode(OutPutPWM_pin6, OUTPUT);
  pinMode(OutPutPWM_pin11, OUTPUT);
  pinMode(OutPutPWM_pin12, OUTPUT);
  pinMode(OutPutPWM_pin13, OUTPUT);
  pinMode(OutPutPWM_pin14, OUTPUT);

  Serial.begin(921600);
  ps5.begin("10:18:49:57:08:F4"); //replace with MAC address of your controller
  Serial.println("Ready.");
}
 
void loop() {
//  while (ps5.isConnected() == false) { // commented out as ps5 controller seems to connect quicker when microcontroller is doing nothing
//    Serial.println("PS5 controller not found");
//    delay(300);
//  }

  // DualSenceが正しく接続できていれば
  while (ps5.isConnected() == true) {
    
    // 下ボタン応答：VolumeDOWN
    if (ps5.Down()) {
      Serial.println("Down Button");
      analogWrite(OutPutPWM_pin12, 255); //電装側のA1にHighを送信：VolumeDOWN
    } else {
      analogWrite(OutPutPWM_pin12, 0); //電装側のA1にLOWを送信：何も起こらない
    }

    // 上ボタン応答：VolumeUP
    if (ps5.Up()) {
      Serial.println("Up Button");
      analogWrite(OutPutPWM_pin11, 255); //電装側のA0にHighを送信：VolumeDOWN
    } else {
      analogWrite(OutPutPWM_pin11, 0); //電装側のA0にLOWを送信：何も起こらない
    }
    if (ps5.Left()) Serial.println("Left Button");
 
    // 四角ボタン応答：音楽の停止
    if (ps5.Square()) {
      Serial.println("Square Button");
      analogWrite(OutPutPWM_pin14, 255); //電装側のA3にHighを送信：MusicSTOP
    } else {
      analogWrite(OutPutPWM_pin14, 0); //電装側のA3にLOWを送信：何も起こらない
    }

    // ×ボタン応答：回転機構用モーターの逆回転
    if (ps5.Cross()) {
      Serial.println("Cross Button");
      //モータードライバーに逆回転入力
      analogWrite(OutPutPWM_pin5, 0);
      analogWrite(OutPutPWM_pin6, 255);
      delay(400);
      //一定時間後停止
      analogWrite(OutPutPWM_pin5, 0);
      analogWrite(OutPutPWM_pin6, 0);
    }

    // 〇ボタン応答：回転機構用モーターの順回転
    if (ps5.Circle()) {
      Serial.println("Circle Button");
      //モータードライバーに逆回転入力
      analogWrite(OutPutPWM_pin5, 255);
      analogWrite(OutPutPWM_pin6, 0);
      delay(1000);
      //一定時間後停止
      analogWrite(OutPutPWM_pin5, 0);
      analogWrite(OutPutPWM_pin6, 0);
    }

    // △ボタン応答：次曲再生
    if (ps5.Triangle()) {
      Serial.println("Triangle Button");
      analogWrite(OutPutPWM_pin13, 255); //電装側のA2にHighを送信：NextMusic
    } else {
      analogWrite(OutPutPWM_pin13, 0); //電装側のA2にLOWを送信：何も起こらない
    }

    // R3ボタン応答：駆動用モーターの停止
    if (ps5.R3())     {
      Serial.println("R3 Button");
      // 駆動用モータードライバーに停止指示
      analogWrite(OutPutPWM_pin1, 0);
      analogWrite(OutPutPWM_pin2, 0);
      analogWrite(OutPutPWM_pin3, 0);
      analogWrite(OutPutPWM_pin4, 0);
      };

    // Rスティック応答：車体の移動
    if (ps5.RStickY()) {
      Serial.printf("Right Stick y at %d\n", ps5.RStickY());
      
      // 前方向入力時
      if (ps5.RStickY()>10){
        if (ps5.RStickX()>0){ //右前へ
          analogWrite(OutPutPWM_pin1, 255-abs(ps5.RStickX()));
          analogWrite(OutPutPWM_pin2, 0);
          analogWrite(OutPutPWM_pin3, 255);
          analogWrite(OutPutPWM_pin4, 0);
        }else{ //左前へ
          analogWrite(OutPutPWM_pin1, 255);
          analogWrite(OutPutPWM_pin2, 0);
          analogWrite(OutPutPWM_pin3, 255-abs(ps5.RStickX()));
          analogWrite(OutPutPWM_pin4, 0);
        }
      }
      // 後ろ方向入力時
      if (ps5.RStickY()<-10){
        if (ps5.RStickX()>0){ //右後ろへ
          analogWrite(OutPutPWM_pin1, 0);
          analogWrite(OutPutPWM_pin2, 255-abs(ps5.RStickX()));
          analogWrite(OutPutPWM_pin3, 0);
          analogWrite(OutPutPWM_pin4, 255);
        }else{ //左後ろへ
          analogWrite(OutPutPWM_pin1, 0);
          analogWrite(OutPutPWM_pin2, 255);
          analogWrite(OutPutPWM_pin3, 0);
          analogWrite(OutPutPWM_pin4, 255-abs(ps5.RStickX()));
        }
      }
    }

    // // PSボタン応答：コントローラ一時停止
    // if (ps5.PSButton()) {
    //   Serial.println("PS Button");
    //   while (true) {
    //     delay(100);
    //     // タッチパッド応答：GPIOリセット(電源オフ前に使用)
    //     if (ps5.Touchpad()) {
    //       Serial.println("Touch Pad Button");
    //       // GPIOリセット

    //       // もう一度タッチパッドを押すまで停止
    //       while(true){
    //         delay(500);
    //         if (ps5.Touchpad()) {
    //           // 再度GPIO設定
    //           pinMode(OutPutPWM_pin1, OUTPUT);
    //           pinMode(OutPutPWM_pin2, OUTPUT);
    //           pinMode(OutPutPWM_pin3, OUTPUT);
    //           pinMode(OutPutPWM_pin4, OUTPUT);
    //           pinMode(OutPutPWM_pin5, OUTPUT);
    //           pinMode(OutPutPWM_pin6, OUTPUT);
    //           pinMode(OutPutPWM_pin11, OUTPUT);
    //           pinMode(OutPutPWM_pin12, OUTPUT);
    //           pinMode(OutPutPWM_pin13, OUTPUT);
    //           pinMode(OutPutPWM_pin14, OUTPUT);
    //           break;
    //         }
    //       }
    //     }
    //     // L3ボタン応答：一時停止解除
    //     if (ps5.L3()) {
    //       Serial.println("L3 Button");
    //       break;
    //     }
    //   }
    // }
    // 割り当てなし一覧
    // if (ps5.Right()) Serial.println("Right Button");
    // if (ps5.UpRight()) Serial.println("Up Right");
    // if (ps5.DownRight()) Serial.println("Down Right");
    // if (ps5.UpLeft()) Serial.println("Up Left");
    // if (ps5.DownLeft()) Serial.println("Down Left");
    // if (ps5.L1()) Serial.println("L1 Button");
    // if (ps5.R1()) Serial.println("R1 Button");
    // if (ps5.Share()) Serial.println("Share Button");
    // if (ps5.Options()) Serial.println("Options Button");
    // 
    // if (ps5.L2()) {
    //   Serial.printf("L2 button at %d\n", ps5.L2Value());
    // }
    // if (ps5.R2()) {
    //   Serial.printf("R2 button at %d\n", ps5.R2Value());
    // }
    // if (ps5.LStickX()) {
    //   Serial.printf("Left Stick x at %d\n", ps5.LStickX());
    // }
    // if (ps5.LStickY()) {
    //   Serial.printf("Left Stick y at %d\n", ps5.LStickY());
    // }
    // if (ps5.RStickX()) {
    //   Serial.printf("Right Stick x at %d\n", ps5.RStickX());
    // }
    // if (ps5.Charging()) Serial.println("The controller is charging"); //doesn't work
    // if (ps5.Audio()) Serial.println("The controller has headphones attached"); //doesn't work
    // if (ps5.Mic()) Serial.println("The controller has a mic attached"); //doesn't work
 
    Serial.printf("Battery Level : %d\n", ps5.Battery()); //doesn't work
    Serial.println();
    // This delay is to make the output more human readable
    // Remove it when you're not trying to see the output
    delay(300);
  }
}
 