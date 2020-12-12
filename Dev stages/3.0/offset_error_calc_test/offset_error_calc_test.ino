#define m_right  6       //motor jobb                 !!csak pwm!!
#define m_left  3        //motor bal                  !!csak pwm!!
#define m_enable  4      //motor kapcsló

#define analogLeft A1    //analog érzékelő
#define analogRight A0   //analog érzékelő

float black_right = 0.0; //x1 
float white_right = 0.0; //x2
float black_left = 0.0;  //y1
float white_left = 0.0;  //y2
float sensor_error_multiply = 0.0;
float sensor_error_plus = 0.0;

const int stay= 160;
const int test_go= 255; 
const int test_back= 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(m_right, OUTPUT);
  pinMode(m_left, OUTPUT);
  pinMode(m_enable, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(m_enable, HIGH);
  analogWrite(m_right, stay);
  analogWrite(m_left, test_go);
  delay(500);
  digitalWrite(m_enable, LOW);
  
  white_right = analogRead(analogRight);
  white_left = analogRead(analogLeft);
  
  digitalWrite(m_enable, HIGH);
  analogWrite(m_left, test_back);
  delay(500);
  digitalWrite(m_enable, LOW);
  
  black_right = analogRead(analogRight);
  black_left = analogRead(analogLeft);
  
  delay(1000);
  sensor_error_plus = (((black_right*white_left)-(black_left*white_right))/(black_right-white_right));
  sensor_error_multiply = ((black_left-sensor_error_plus)/(black_right));
  
  Serial.print("black_right: ");
  Serial.println(black_right);
  Serial.print("black_left: ");
  Serial.println(black_left);
  Serial.print("white_right: ");
  Serial.println(white_right);
  Serial.print("white_left: ");
  Serial.println(white_left);
  Serial.print("sensor_error_plus: ");
  Serial.println(sensor_error_plus);
  Serial.print("sensor_error_multiply: ");
  Serial.println(sensor_error_multiply);
  
  
  
  
  
  delay(10000);
}
