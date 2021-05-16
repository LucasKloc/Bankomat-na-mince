#include <Servo.h>
//každý motor má svou instanci třídy Servo
Servo myservo;
Servo myservo1; 
Servo myservo2; 
int Koruna;
int DvouKoruna;
int PetiKoruna =0;
int pomocna =0;
int PrvniCifra = 6;
int lulec =2552;

void setup() {
  Serial.begin(9600);
  myservo.attach(5);   //tento motor je připojen na pin 
  myservo1.attach(4);
  myservo2.attach(3);


  if (PrvniCifra>=1){
  pomocna = PrvniCifra * 10;
  pomocna = pomocna/5;
  PetiKoruna= PetiKoruna + pomocna;                              // Do proměnný ukládám kolik pětikorun to vyhodí
  Serial.println(PetiKoruna);
}else if(PrvniCifra==0){
  PetiKoruna= PetiKoruna+ 0;
  Serial.println(PetiKoruna);
}
  
}
int i = 1;

void loop() {

}
  // put your main code here, to run repeatedly:
