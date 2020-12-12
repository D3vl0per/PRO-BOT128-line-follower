#define m_right  9    //motor jobb
#define m_left  6      //motor bal
#define m_enable  7    //motor kapcsló
#define analogLeft A1    //fordítás elött deklarálni!!!
#define analogRight A0   //fordítás elött deklarálni!!!
#define leftled 11
#define rightled 12
#define button 2
//////////////////////////////////////////////////////
float sensor_right = 0.0; 
float sensor_left = 0.0;
float sensor_diff_scale = 2.5;
const int max_speed = 255;
const int min_speed = 195;
const int allowedDiffSpeed = max_speed-min_speed;
const float sensorDiffThreshold = 6.0;
boolean bLastLeftTurned = true;
////////////////////////////////////////////////////

/////////////////////////////////////////////////////
void setup() {
  //setup
  pinMode(m_right, OUTPUT);
  pinMode(m_left, OUTPUT);
  pinMode(m_enable, OUTPUT);
  pinMode(leftled, OUTPUT);
  pinMode(rightled, OUTPUT);
  pinMode(button, INPUT);
  delay(3000);
  while (button == HIGH);
  digitalWrite(m_enable, HIGH);
}
/////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////
void loop() {
  //beolvasás
  sensor_right = analogRead(analogRight);
  sensor_left = analogRead(analogLeft);
  //beolvasás

  //kalibrálás
  sensor_right = (sensor_right * 0.904459) + 0.2966158;
  //kalibrálás
  
  float sensorDiff = (sensor_left - sensor_right);

  

  // debug
  if (abs(sensorDiff) > sensorDiffThreshold){
    if (sensorDiff > 0) {
      bLastLeftTurned = true;
      digitalWrite(leftled, HIGH);
      digitalWrite(rightled, LOW);
    } else {
      bLastLeftTurned = false;
      digitalWrite(leftled, LOW);
      digitalWrite(rightled, HIGH);
    }
  } else {
    digitalWrite(leftled, LOW);
    digitalWrite(rightled, LOW);   
  }
  //debug

  sensorDiff = sensorDiff*sensor_diff_scale; //Range 0-1023
  
  setDiffSpeeds(sensorDiff);


  
}
