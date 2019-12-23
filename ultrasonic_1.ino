// First attempt at ultrasonic sensor 12/19

int trigPin = 11;
int echoPin = 12;
int blueLed = 7;
double duration, cm, avg_distance;
int index = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //baud rate 9600
  // input and output pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(blueLed, OUTPUT);
  avg_distance = train();
  Serial.print("Done training");
}

void loop() {
  // put your main code here, to run repeatedly:
  // Triggered by HIGH pulse of 10+ us  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  //duration is sending of ping to recpetion after bouncing
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
 
  if(avg_distance < cm*0.8 && cm < 2000){
    digitalWrite(blueLed, HIGH);
    delay(1000);
    digitalWrite(blueLed, LOW);
  }
  
  delay(50);
}

double train(){
  double sum = 0;
  int num_samples = 100;
  for(int i = 0; i < num_samples; i++){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    //duration is sending of ping to recpetion after bouncing
    duration = pulseIn(echoPin, HIGH);
    sum += (duration/2) / 29.1;
    delay(100);
  }
  return sum/num_samples;
}
