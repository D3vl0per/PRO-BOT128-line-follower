#define m_right  6       //motor jobb                 !!csak pwm!!
#define m_left  3        //motor bal                  !!csak pwm!!
#define m_enable  4
void setup() {
  // put your setup code here, to run once:
   pinMode(m_right, OUTPUT);
   pinMode(m_left, OUTPUT);
   pinMode(m_enable, OUTPUT);
   digitalWrite(m_enable, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
   analogWrite(m_right, 160);
   analogWrite(m_left, 255);
   delay(500);
   analogWrite(m_left, 0);
   delay(500);
   analogWrite(m_left, 160);
   delay(3000);
}
