#include <Servo.h>     //zahrnutí knihovny pro ovládání servo motoru
Servo myservo;//každý motor má svou instanci třídy Servo
Servo myservo1; 
Servo myservo2; 
int pos = 0;           //proměnná obsahující pozici motoru (úhel natočení)

void setup()
{
  myservo.attach(5);   //tento motor je připojen na pin 9
  myservo1.attach(4);
  myservo2.attach(3);
}

void loop()
{
  for(pos = 0; pos <= 180; pos += 1) //je od úhlu 0 do úhlu 180
  {
    myservo.write(pos);  //natočení motoru na aktuální úhel
    delay(2);           //chvilka čekání než se motor natočí
    myservo1.write(pos);  //natočení motoru na aktuální úhel
    delay(2);           //chvilka čekání než se motor natočí
    myservo2.write(pos);  //natočení motoru na aktuální úhel
    delay(2);           //chvilka čekání než se motor natočí
  } 
  for(pos = 180; pos >= 0; pos -= 1) //je od úhlu 180 zpět do úhlu 0
  {
    myservo.write(pos);  //natočení motoru na aktuální úhel
    delay(2);           //chvilka čekání než se motor natočí
    myservo1.write(pos);  //natočení motoru na aktuální úhel
    delay(2);           //chvilka čekání než se motor natočí
    myservo2.write(pos);  //natočení motoru na aktuální úhel
    delay(2);           //chvilka čekání než se motor natočí

  }

}
