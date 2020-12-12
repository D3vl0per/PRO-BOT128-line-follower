//Created by Márk Zsibók
//My mentors: Tamás Szépe
//Created at: 2017.02.09

#include <EEPROM.h>

#define m_right  6       //motor jobb                 !!csak pwm!!
#define m_left  3        //motor bal                  !!csak pwm!!
#define m_enable  4      //motor kapcsló                 
#define analogLeft A0    //analog érzékelő 
#define analogRight A1   //analog érzékelő
#define led_12 12        //első balra led
#define led_13 8         //első jobbra led
#define led_14 11        //hátsó jobbra led
#define led_15 7         //hátsó balra led
#define buzzer 5         //Buzzer
#define button_res 2     //reset button
#define button_boot 9    //boot button

//////////////////////////////////////////////////////
const int EEPROM_SEP = 250;                         //EEPROM memória cím #1
const int EEPROM_SEM = 255;                         //EEPROM memória cím #2
const int max_speed = 255;                          //Motor maximális operációs sebessége 
const int min_speed = 190;                          //Motor minimális operációs sebessége
const int allowedDiffSpeed = max_speed-min_speed;   //Megengedett differencál sebesség
boolean bLastLeftTurned = true;                     //Led vezérlés
const float sensorDiffThreshold = 6.0;              //Sensor mérés küszöb
float sensorDiff = 0.0;                             //    
float sensor_right = 0.0;                           //Sensor beolvasott adatok #1
float sensor_left = 0.0;                            //Sensor beolvasott adatok #2
float sensor_diff_scale = 2.0;                      //
float sensor_error_multiply = 0.0;                  //Offset hiba mértéke #1
float sensor_error_plus = 0.0;                      //Offset hiba mértéke #2
float black_right = 0.0; //x1                       //
float white_right = 0.0; //x2                       //
float black_left = 0.0;  //y1                       //
float white_left = 0.0;  //y2                       //
int x = 0;                                          //
//////////////////////////////////////////////////////



void setup() {
  //setup
  pinMode(m_right, OUTPUT);
  pinMode(m_left, OUTPUT);
  pinMode(m_enable, OUTPUT);
  pinMode(led_12, OUTPUT);
  pinMode(led_13, OUTPUT);
  pinMode(led_14, OUTPUT);
  pinMode(led_15, OUTPUT);
  pinMode(button_res, INPUT_PULLUP);
  pinMode(button_boot, INPUT_PULLUP);
  led_blink();

  button_waiting(200, button_res, 1);
  while (digitalRead(button_res) == HIGH && digitalRead(button_boot) == HIGH);  //Offset_error adatok visszaállítása memóriából
  if (digitalRead(button_res) == LOW) reset_offset_data();                      //vagy
  else if (digitalRead(button_boot) == LOW) load_offset_data();                 //adatok újra írása a memóriába
           
  button_waiting(200, button_boot, 0);
  buzzer_blink(buzzer, 1200, 100);
  delay(100);
  buzzer_blink(buzzer, 1200, 100);
  
  digitalWrite(led_14, LOW);
  digitalWrite(led_15, LOW);
  digitalWrite(m_enable, HIGH);
}




void loop() {
  //beolvasás
  sensor_right = analogRead(analogRight);
  sensor_left = analogRead(analogLeft);
  
  //kalibrálás
  sensor_right = (sensor_right * sensor_error_multiply) + sensor_error_plus;
  
  
  sensorDiff = (sensor_left - sensor_right);
   
  //led vezérlés
  if (abs(sensorDiff) > sensorDiffThreshold){
    if (sensorDiff > 0) {
      bLastLeftTurned = true;
      digitalWrite(led_12, HIGH);
      digitalWrite(led_13, LOW);
    } else {
      bLastLeftTurned = false;
      digitalWrite(led_12, LOW);
      digitalWrite(led_13, HIGH);
    }
  } 
  
  sensorDiff = sensorDiff*sensor_diff_scale; //Range 0-1023
  setDiffSpeeds(sensorDiff);  //függvény meghívás
}
//------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////
void reset_offset_data(){
      black_right = analogRead(analogRight);
      black_left = analogRead(analogLeft);
      digitalWrite(led_15, LOW);
      digitalWrite(led_13, LOW);
      buzzer_blink(buzzer, 1200, 400);
      
      while (digitalRead(button_res) == LOW);
      while (digitalRead(button_res) == HIGH);
      
      
      white_right = analogRead(analogRight);
      white_left = analogRead(analogLeft);
      digitalWrite(led_14, LOW);
      digitalWrite(led_12, LOW);
      buzzer_blink(buzzer, 1200, 400);
      
      sensor_error_plus = (((black_right * white_left) - (black_left * white_right)) / (black_right - white_right));
      sensor_error_multiply = ((black_left - sensor_error_plus) / (black_right));
      //delay(100);
      EEPROM.put(EEPROM_SEP, sensor_error_plus);
      delay(100);
      EEPROM.put(EEPROM_SEM, sensor_error_multiply);
      digitalWrite(led_12, HIGH);
      digitalWrite(led_13, HIGH);
}
///////////////////////////////////////////////////////////////////////////////
void button_waiting (int delay_led, int button_select, int dual_button){
  if (dual_button == 1){
    while (digitalRead(button_boot) == HIGH && digitalRead(button_res) == HIGH){
      digitalWrite(led_13, HIGH);
      digitalWrite(led_12, LOW);
      delay(delay_led);
      digitalWrite(led_12, HIGH);
      digitalWrite(led_14, LOW);
      delay(delay_led);
      digitalWrite(led_14, HIGH);
      digitalWrite(led_15, LOW);
      delay(delay_led);
      digitalWrite(led_15, HIGH);
      digitalWrite(led_13, LOW);
      delay(delay_led);
    }
  }

  else {
      while (digitalRead(button_select) == HIGH){
      digitalWrite(led_13, HIGH);
      digitalWrite(led_12, LOW);
      delay(delay_led);
      digitalWrite(led_12, HIGH);
      digitalWrite(led_14, LOW);
      delay(delay_led);
      digitalWrite(led_14, HIGH);
      digitalWrite(led_15, LOW);
      delay(delay_led);
      digitalWrite(led_15, HIGH);
      digitalWrite(led_13, LOW);
      delay(delay_led);
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
void led_blink(){
  digitalWrite(led_12, LOW);
  digitalWrite(led_13, LOW);
  digitalWrite(led_14, LOW);
  digitalWrite(led_15, LOW);
  tone(buzzer, 1200);
  delay(900);
  digitalWrite(led_12, HIGH);
  digitalWrite(led_13, HIGH);
  digitalWrite(led_14, HIGH);
  digitalWrite(led_15, HIGH);
  noTone(buzzer);
}
//////////////////////////////////////////////////////////////////////////////
void setDiffSpeeds(int diffSpeed) {
  //Saturation
  if (diffSpeed >    allowedDiffSpeed) diffSpeed =  allowedDiffSpeed;
  if (diffSpeed < -1*allowedDiffSpeed) diffSpeed = -1*allowedDiffSpeed;
  //Saturation
  float base_speed=min_speed;
  //Proceeding
  analogWrite(m_right, base_speed+diffSpeed);
  analogWrite(m_left , base_speed-diffSpeed);
  //Proceeding
}
///////////////////////////////////////////////////////////////////////////////
void load_offset_data(){
      buzzer_blink(buzzer, 1200, 400);
      EEPROM.get(EEPROM_SEP, sensor_error_plus);
      EEPROM.get(EEPROM_SEM, sensor_error_multiply);
}
///////////////////////////////////////////////////////////////////////////////
void buzzer_blink(int pin, int freq, int delay_buzzer){
  tone(pin, freq);
  delay(delay_buzzer);
  noTone(pin);
}
