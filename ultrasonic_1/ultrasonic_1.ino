// Arduino code for project
// 12/23

#include <IRremote.h>
#include <IRremoteInt.h>

int trigPin = 11;
int echoPin = 12;
int blueLed = 7;
int IRRx = 8;
double duration, cm, avg_distance;
int index = 0;
IRrecv irrecv(IRRx);
decode_results results;
unsigned long key_value = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //baud rate=9600
  // input and output pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(blueLed, OUTPUT);
  Serial.println("=====================");
  Serial.println("  Room Control v1.0");
  Serial.println("=====================");
  avg_distance = train();
  irrecv.enableIRIn();
  irrecv.blink13(true);
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
  Serial.println("cm");

  // turn on lights if distance detected shorter than training distance
  if(avg_distance*.95 > cm){
    digitalWrite(blueLed, HIGH);
    delay(3000);
    digitalWrite(blueLed, LOW);
  }

  // if lights are already/in process of turning on,
  // reset timeout timer (TODO)

  //limit ultrasonic readings to 10/sec
  delay(50);

  if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        switch(results.value){
          case 0xFFA25D:
          Serial.println("Power");
          break;
          case 0xFF629D:
          Serial.println("VOL+");
          break;
          case 0xFFE21D:
          Serial.println("FUNC/STOP+");
          break;
          case 0xFF22DD:
          Serial.println("<<");
          break;
          case 0xFF02FD:
          Serial.println(">|");
          break ;  
          case 0xFFC23D:
          Serial.println(">>");
          break ;               
          case 0xFFE01F:
          Serial.println("DOWN");
          break ;  
          case 0xFFA857:
          Serial.println("VOL-");
          break ;  
          case 0xFF906F:
          Serial.println("UP");
          break ;  
          case 0xFF6897:
          Serial.println("0");
          break ;  
          case 0xFF9867:
          Serial.println("EQ");
          break ;
          case 0xFFB04F:
          Serial.println("ST/REPT");
          break ;
          case 0xFF30CF:
          Serial.println("1");
          break ;
          case 0xFF18E7:
          Serial.println("2");
          break ;
          case 0xFF7A85:
          Serial.println("3");
          break ;
          case 0xFF10EF:
          Serial.println("4");
          break ;
          case 0xFF38C7:
          Serial.println("5");
          break ;
          case 0xFF5AA5:
          Serial.println("6");
          break ;
          case 0xFF42BD:
          Serial.println("7");
          break ;
          case 0xFF4AB5:
          Serial.println("8");
          break ;
          case 0xFF52AD:
          Serial.println("9");
          break ;      
        }
        key_value = results.value;
        irrecv.resume();
  }
}

// When place sensor in a new location, train() will find distance when no objects
// present and activate lights if distance is shorter than this avg_distance
double train(){
  double sum = 0;
  int num_samples = 50;
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
    // progress bar, kinda
    if(i % (num_samples / 10) == 0){
      Serial.print(".");
    }
  }
  Serial.println();
  return sum/num_samples;
}

