#define analogLeft A1    //analog érzékelő
#define analogRight A0   //analog érzékelő
#define button_boot 9

float black_right = 0.0; //x1 
float white_right = 0.0; //x2
float black_left = 0.0;  //y1
float white_left = 0.0;  //y2
float sensor_error_multiply = 0.0;
float sensor_error_plus = 0.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button_boot, INPUT);
  digitalWrite(button_boot, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
    delay(3000);
    while (digitalRead(button_boot) == HIGH);
    white_right = analogRead(analogRight);
    white_left = analogRead(analogLeft);
    Serial.print("white_right: ");
    Serial.println(white_right);
    Serial.print("white_left: ");
    Serial.println(white_left);
    delay(1000);
    while (digitalRead(button_boot) == HIGH);
    black_right = analogRead(analogRight);
    black_left = analogRead(analogLeft);
    Serial.print("black_right: ");
    Serial.println(black_right);
    Serial.print("black_left: ");
    Serial.println(black_left);
    sensor_error_plus = (((black_right*white_left)-(black_left*white_right))/(black_right-white_right));
    sensor_error_multiply = ((black_left-sensor_error_plus)/(black_right));
   
    Serial.print("sensor_error_multiply: ");
    Serial.println(sensor_error_multiply);
    Serial.print("sensor_error_plus: ");
    Serial.println(sensor_error_plus);
    delay(10000);
  
}
