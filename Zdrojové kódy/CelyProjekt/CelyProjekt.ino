//***********************************************************************************************  KNIHOVNY
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Servo.h>     
//***********************************************************************************************  ESP
#define DEBUG true
String mySSID = "Klodos";       // TVOJE WIFI
String myPWD = "Mondeo2015"; // TVOJE WIFI HESLO
String myAPI = "J7SG6JSBU7THVQ19";   //Tvoje write api z thingspeak
String myHOST = "184.106.153.149";
String myPORT = "80";

void esp8266(int zustatekPetrzelka,int zustatekKloc,int zustatekBen){
  String sendData = "GET /update?api_key="+ myAPI +"&"+ "field1" +"="+String(zustatekPetrzelka)+"&"+ "field2" +"="+String(zustatekKloc)+"&"+ "field3" +"="+String(zustatekBen);
  espData("AT+CIPMUX=1", 1000, DEBUG); 
  espData("AT+CIPSTART=\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
  espData("AT+CIPSEND=66," +String(sendData.length()),1000,DEBUG); 
  Serial1.println("AT+CIPSEND=51");
  Serial1.find(">"); 
  Serial1.println(sendData);
  espData("AT+CIPCLOSE",1000,DEBUG);
}



String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  Serial1.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      response += c;
    }
  }

    Serial.print(response);

  return response;
}



//***********************************************************************************************  SERVA
Servo myservo; //každý motor má svou instanci třídy Servo
Servo myservo1; 
Servo myservo2; 
//***********************************************************************************************  DISPLAY
// Declare LCD object for software SPI
// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);
Adafruit_PCD8544 display = Adafruit_PCD8544(12, 11, 10, 9, 8);

//***********************************************************************************************  NUMPAD

const byte ROWS = 4; // 4 ŘÁDKY
const byte COLS = 3; // 3 SLOUPCE
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'w','0','w'}
};
byte rowPins[ROWS] = {28, 27, 26, 25}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {24, 23, 22}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//***********************************************************************************************  ČTEČKA
// RFID čtečka - ukázka čtení adres tagů
// definování pinů pro SDA a RST
#define SDA_PIN 7
#define RST_PIN 6

// vytvoření instance RFID čtečky z knihovny
MFRC522 rfid(SDA_PIN, RST_PIN);


//***********************************************************************************************  Zůstatky
// ID 1
int zustatekPetrzelka = 37;   


// ID 2
int zustatekKloc = 50; 


// ID 3
int zustatekBen = 20;          


//***************************************************************************************************************************************** METODA KONTROLUJÍCÍ PIN

int zadanikodu(int pin){
int vysledek =0;
int pomocna = 0;

  
 
  
  do{
 
int key = keypad.getKey();
  if (key != NO_KEY){
    display.setTextColor(WHITE,BLACK);
  display.setTextSize(2);  
  display.print("*");    //VYPÍŠE TEXT
  display.display();
  
    if(key==pin){
      
      vysledek=1;
    }else{
      
      vysledek=2;
    }
  }
  }while(vysledek ==0);
  if (vysledek==2){
    vysledek =0;
  }
return vysledek;

}

//***************************************************************************************************************************************** METODA KONTROLUJÍCÍ TRANSAKCE

int KontrolaZustatku(int ID){
//--------------------------------------- Toto se provede ve vyvolané metodě pouze jednou
display.setTextSize(2);                                                                                        // To tady myslím nemusí být -  U Lukáše se podívám a smažu (SetTextSize)
int a=0;                   // Pomocná proměnná pro (while)
int i = 0;                 // Pomocná proměnná pro 
int pomocnajedna;          // Pomocná proměnná, do které se ukládá Key (stisknutí numpadu)
int pomocnadva;            // Druhá pomocná proměnná, do které se ukládá Key (stisknutí numpadu)
int help;
String SpojeniCisel;
String ZachrannyStringJedna;
String ZachrannyStringDva;
String KvuliEsp;


//---------------------------------------

do{
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-* Zde čekáme na uživatele, než zadá částku kterou chce vybrat. Se samotnou častkou pracujeme až po tom co je uložená do proměnných
  char key = keypad.getKey(); // Do proměnné key si uložíme zmáčknutou klávesu
  if (key != NO_KEY){         // Podmínká ptající se jestli byla nějáká klávesa z NUMPADU zmáčknuta
    

    if(i==1){                 // Tahle podmínka se kontroluje jako druhá, protože se v první podmínce nastavuje i=1;
  
     help = key;
      if(help !=119){         //Proměnná která kontroluje, zda není zmáčknutý * nebo #
      
    display.print(key);
    display.display();
     pomocnadva = key -48;  // Převádíme druhý key, který je v datovým typu char. Key převadíme do Integeru do pomocdva proměnné.
      ZachrannyStringDva= pomocnadva;
      i=2;                    // Do proměnné i nastavíme 2, což znamená, že se provede podmínka, která je o něco níže.
      }else{
        
        
      }
    }
    
    if(i==0){                 // Tahle podmínka se kontroluje jako první, protože jsme na začátku nastavili do proměnné i = 0;
     help = key;
      if(help !=119){         //Proměnná která kontroluje, zda není zmáčknutý * nebo #
      display.setTextSize(2);
      display.clearDisplay();
      display.print(key);
    display.display();
    pomocnajedna = key -48;   // Převádíme key, který je v datovým typu char. Key převadíme do Integeru do pomocnajedna proměnné.
    ZachrannyStringJedna= pomocnajedna;
      i=1;                    // Do proměnné i nastavíme 1, což znamená, že se provede podmínka nad náma, po tom, co se cyklus (do - while) zopakuje
      }else{
        
      }
    }
    
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

if(i==2){             // Tato podmínka už tedy přímo kontroluje transakce
  delay(1000);


SpojeniCisel = ZachrannyStringJedna +ZachrannyStringDva;
int SpojeniCiselvIntu =  SpojeniCisel.toInt();



  
                      // ID nám tu slouží k tomu, aby rozpoznalo uživatele. Každý uživatel má svoje ID.
  //********************************************VITULINEK PETRZELKA**********************************************************************************
   if(ID == 1){
        
        if(SpojeniCiselvIntu>zustatekPetrzelka){
        display.setTextSize(1);
          display.clearDisplay();
          display.println("Transkace");
          display.display();
          display.println("zamitnuta");
          display.display();
          display.println("Zadejte castkuznovu:");
          display.display();
            a = 0;                        // Zajistí, že se bude loop opakovat dál
          delay(2000);    
          i = 0;                        // Zajistí, že se bude podmínka zopakuje
   }else{
        zustatekPetrzelka= zustatekPetrzelka-SpojeniCiselvIntu;
        
        
       // esp8266("field1",KvuliEsp);
          a = 2;
         // VypadPenez(pomocnajedna,pomocnadva);
   }
   }
    
//********************************************LUKY KLOC**********************************************************************************
    if(ID ==2){
      
        if(SpojeniCiselvIntu>zustatekKloc){
        display.setTextSize(1);
          display.clearDisplay();
          display.println("Transkace");
          display.display();
          display.println("zamitnuta");
          display.display();
          display.println("Zadejte castkuznovu:");
          display.display();
            a = 0;                        // Zajistí, že se bude loop opakovat dál
          delay(2000);    
          i = 0;                        // Zajistí, že se bude podmínka zopakuje
   }else{
        zustatekKloc= zustatekKloc-SpojeniCiselvIntu;
    //    esp8266("field2",zustatekKloc);
    a = 2;
  //  VypadPenez(pomocnajedna,pomocnadva);
   }
    }

//******************************************** Beník **********************************************************************************

   if(ID ==3){
    
   if(SpojeniCiselvIntu>zustatekBen){
        display.setTextSize(1);
          display.clearDisplay();
          display.println("Transkace");
          display.display();
          display.println("zamitnuta");
          display.display();
          display.println("Zadejte castkuznovu:");
          display.display();
            a = 0;                        // Zajistí, že se bude loop opakovat dál
          delay(2000);    
          i = 0;                        // Zajistí, že se bude podmínka zopakuje
   }else{
        zustatekBen= zustatekBen-SpojeniCiselvIntu;
   //   esp8266("field3",zustatekBen);
    a = 2;
                    //    VypadPenez(pomocnajedna,pomocnadva);
   }
       
 
    
    }
   //********************************************************************************************************************************   
  } // KONEC Vstupniho HLAVNIHO  If
  //********************************************************************************************************************************   
  
    }
  
  }while(a!=2);
  //******************************************************************************************************************************** 
  } //Konec metody
  //********************************************************************************************************************************
  
//***************************************************************************************************************************************** Výpadek peněz
int VypadPenez(int PrvniCifra,int DruhaCifra){
int pomocna = 0;
int PetiKoruna = 0;
int DvouKoruna = 0;
int JednoKoruna = 0;


  if (PrvniCifra>=1){
  pomocna = PrvniCifra * 10;
  pomocna = pomocna/5;
  PetiKoruna= PetiKoruna + pomocna;                              // Do proměnný ukládám kolik pětikorun to vyhodí
}else if(PrvniCifra==0){
  PetiKoruna= PetiKoruna+ 0;
}


if(DruhaCifra==1){
  JednoKoruna = JednoKoruna +1;
}
if(DruhaCifra==2){
  DvouKoruna = DvouKoruna + 1;
  
}
if(DruhaCifra==3){
JednoKoruna = JednoKoruna +1;
DvouKoruna = DvouKoruna + 1;
  
}
if(DruhaCifra==4){
  DvouKoruna = DvouKoruna + 2;
}
if(DruhaCifra==5){
  PetiKoruna= PetiKoruna + 1;
}
if(DruhaCifra==6){
JednoKoruna = JednoKoruna +1;
  PetiKoruna= PetiKoruna + 1;
}
if(DruhaCifra==7){
  PetiKoruna= PetiKoruna + 1;
  DvouKoruna = DvouKoruna + 1;
}
if(DruhaCifra==8){
JednoKoruna = JednoKoruna +1;
DvouKoruna = DvouKoruna + 1;
PetiKoruna= PetiKoruna + 1;
  
}
if(DruhaCifra==9){
DvouKoruna = DvouKoruna + 2;
PetiKoruna= PetiKoruna + 1;
  
}

Serial.print("Dohromady vyhodím: ");
Serial.print(JednoKoruna);
Serial.println(" JednoKorun");
Serial.print("Dohromady vyhodím: ");
Serial.print(DvouKoruna);
Serial.println(" DvouKorun");
Serial.print("Dohromady vyhodím: ");
Serial.print(PetiKoruna);
Serial.println(" PetiKorun");

// Cyklus pro JednoKoruny
if(JednoKoruna!=0){
  for(int j = 0; j <= JednoKoruna; j += 1) //je od úhlu 0 do úhlu 180
  {for(int pos = 0; pos <= 180; pos += 1) //je od úhlu 0 do úhlu 180
  {
    myservo1.write(pos);  //natočení motoru na aktuální úhel
    delay(5);  
     myservo1.write(pos);  //natočení motoru na aktuální úhel
    delay(5);          
  } 
  for(int pos = 180; pos >= 0; pos -= 1) //je od úhlu 180 zpět do úhlu 0
  {
    myservo1.write(pos);  //natočení motoru na aktuální úhel
    delay(5);           //chvilka čekání než se motor natočí

  }  
  } 
}
// Cyklus pro DvouKoruny
if(DvouKoruna!=0){
  for(int j = 0; j <= DvouKoruna; j ++) //je od úhlu 0 do úhlu 180
  {for(int pos = 0; pos <= 180; pos ++) //je od úhlu 0 do úhlu 180
  {
    myservo2.write(pos);  //natočení motoru na aktuální úhel
    delay(5);           //chvilka čekání než se motor natočí
        myservo2.write(pos);  //natočení motoru na aktuální úhel
    delay(5);           //chvilka čekání než se motor natočí       
  }  
    for(int pos = 180; pos >= 0; pos --) //je od úhlu 180 zpět do úhlu 0
  {
    myservo2.write(pos);  //natočení motoru na aktuální úhel
    delay(5);           //chvilka čekání než se motor natočí

  } 
  } 
}
 // Cyklus pro PetiKoruny
 if(PetiKoruna!=0){
 for(int j = 0; j <= PetiKoruna; j ++) //je od úhlu 0 do úhlu 180
 {for(int pos = 0; pos <= 180; pos ++) //je od úhlu 0 do úhlu 180
  {
   myservo.write(pos);  //natočení motoru na aktuální úhel
   delay(5);  
   myservo.write(pos);  //natočení motoru na aktuální úhel
   delay(5);          
  } 
  for(int pos = 180; pos >= 0; pos --) //je od úhlu 180 zpět do úhlu 0
  {
    myservo.write(pos);  //natočení motoru na aktuální úhel
    delay(5);           //chvilka čekání než se motor natočí

  }  
   
 }
 } 



  
  
}

   
//ČÍSLA = ASCII ZNAKY
//***********************************************************************************************
//1=49 ; 2=50 ; 3=51 ; 4=52 ; 5=53 ; 6=54 ; 7=55 ;8=56 ; 9=57 ; *=35; 0=48; #=42;
//***********************************************************************************************
//*****************************************************************************************************************************************

void setup() {
  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600);
    Serial1.begin(115200);

  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  espData("AT+CIFSR", 1000, DEBUG);
  //delay(1000);
 esp8266(zustatekPetrzelka,zustatekKloc,zustatekBen);
    

  // inicializace komunikace přes SPI
  SPI.begin();
  // inicializace komunikace s RFID čtečkou
  rfid.PCD_Init();

  myservo.attach(5);   
  myservo1.attach(4);
  myservo2.attach(3);

}

int vypis=0;      //Proměnná vypis pomáhá k nastavení displeje

void loop() {
// U UŽIVATELU NA KONCI VŽDYCKY VYRESETOVAT NA 0!!!
  if(vypis == 0){
    
          display.begin();
  display.setContrast(65);
  display.clearDisplay();
  display.setTextSize(1);
    display.setTextColor(BLACK);
  display.setCursor(0,0);
display.println("Prilozte kartu");    //VYPÍŠE TEXT
  display.display();
  vypis=1;
  }

  
  
  // kontrola RFID tagů v okolí modulu,
  // pokud není žádný tag v okolí, volá se loop funkce od začátku,
  // celá část s RFID by tedy měla být na konci programu
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  // kontrola správného přečtení RFID tagu
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  // výpis informace o verzi RFID tagu
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.print("RFID tag typu: ");
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // kontrola podporovaných typů RFID tagu
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("Tento RFID tag neni podporovany (typ MIFARE Classic).");
    return;
  }
  // výpis adresy RFID tagu v hexa formátu
  Serial.print("Adresa RFID tagu: ");
  vypisHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

 //**************************************************************************************************************************************************************************************************   
 //************************************************************************************************************************************************************************************************** UŽIVATEL PETRŽELKA
  if(rfid.uid.uidByte[0] == 0x87 & rfid.uid.uidByte[1] == 0xFE & rfid.uid.uidByte[2] == 0x48 & rfid.uid.uidByte[3] == 0x63) {
  //PIN UŽIVATELE
  int aa =49;  //První pin      1  
  int bb =50; //Druhý pin       2
   int cc =51; //Třetí pin      3
    int dd =52; //Čtvrtý pin    4
    int vysledkos=0;
 //*************************************************
    Serial.println("Detekovana bila karta!");
    Serial.println("Přihlášen uživatel Vitulinek Petrželka");
    //zadáte pin, pokud zadáte pin správně zeptá se vás to kolik budete chtít vybrat a pokud zadáte pin špatně napíše to že jste zadal pin špatně a budeš ho muset opravit (po 5 špatných pokusech se karta zablokuje)
       display.begin();
  display.setContrast(65); // ABY BYL VIDĚT TEXT
  display.clearDisplay();  // ABY VYČISTIL DISPLAY
  display.setTextSize(1);   // VELIKOST TEXTU
    display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Zadejte pin:");    //VYPÍŠE TEXT NA DISPLEJ
  display.display();
    do{
 int a =zadanikodu(aa);  //První pin        
  int b =zadanikodu(bb); //Druhý pin
   int c =zadanikodu(cc); //Třetí pin
    int d =zadanikodu(dd); //Čtvrtý pin
     vysledkos = a + b + c + d;
    //SPRÁVNÝ PIN = 4, ŠPATNÝ POD 4
    delay(1000);
          display.begin();
  display.setContrast(65);
  display.clearDisplay();
  display.setTextSize(1);
    display.setTextColor(BLACK);
  display.setCursor(0,0);
 if(vysledkos ==4){

  display.println("Uspesne si se prihlasil!");    //VYPÍŠE TEXT
  display.display();
  
  
  
 }else{

  display.println("Spatny pin");
  display.println("Zadej pin");
  display.println("znovu");
  display.display();
 }
    }while(vysledkos!=4);
        delay(2000);
          display.begin();        
  display.setContrast(65);
  display.clearDisplay();
  display.setTextSize(1);
    display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Vas zustatek:");    //VYPÍŠE TEXT
  display.display();
  display.print(zustatekPetrzelka); 
  display.println(" Kc");
  display.display();
  display.println("Zadejte castku");
  display.display();
  int IDVitulinka = 1;
  KontrolaZustatku(IDVitulinka);
 
  delay(1000);
  display.clearDisplay(); 
  display.setTextSize(1);
        display.print("Potvrzuji");
        display.display();
    delay(1000);
    display.print(".");
    display.display();
    delay(1000);
    display.print(".");
    display.display();
    delay(1000);
    display.print(".");
    display.display();
  
  }
 //************************************************************************************************************************************************************************************************** UŽIVATEL KLOC
 //************************************************************************************************************************************************************************************************** 
  else if(rfid.uid.uidByte[0] == 0xC9 & rfid.uid.uidByte[1] == 0xE0 & rfid.uid.uidByte[2] == 0x95 & rfid.uid.uidByte[3] == 0xC2) {
   //PIN  uživatele
   int aa =52;  //První pin    4  
   int bb =51; //Druhý pin       3
   int cc =50; //Třetí pin      2
   int dd =49; //Čtvrtý pin    1
   int vysledkos=0;
   //*************************************************  
    Serial.println("Detekovan modry privesek!");
     Serial.println("Přihlášen uživatel Luky Kloc");
     //zadáte pin, pokud zadáte pin správně zeptá se vás to kolik budete chtít vybrat a pokud zadáte pin špatně napíše to že jste zadal pin špatně a budeš ho muset opravit (po 5 špatných pokusech se karta zablokuje)
    
       display.begin();
  display.setContrast(65);
  display.clearDisplay();
  display.setTextSize(1);
    display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Zadejte pin:");    //VYPÍŠE TEXT
  display.display();
   do{
 int a =zadanikodu(aa);  //První pin        
  int b =zadanikodu(bb); //Druhý pin
   int c =zadanikodu(cc); //Třetí pin
    int d =zadanikodu(dd); //Čtvrtý pin
     vysledkos = a + b + c + d;
    //SPRÁVNÝ PIN = 4, ŠPATNÝ POD 4
  delay(1000);
              display.begin();
  display.setContrast(65);
  display.clearDisplay();
  display.setTextSize(1);
    display.setTextColor(BLACK);
  display.setCursor(0,0);
 if(vysledkos ==4){

  display.println("Uspesne si se prihlasil!");    //VYPÍŠE TEXT
  display.display();
  
  
 }else{
  display.println("Spatny pin");
  display.println("Zadej pin");
  display.println("znovu");
  display.display();
 }
 }while(vysledkos!=4);
      delay(2000);
       display.begin();
  display.setContrast(65);
  display.clearDisplay();
  display.setTextSize(1);
    display.setTextColor(BLACK);
  display.setCursor(0,0);
    display.println("Vas zustatek:");    //VYPÍŠE TEXT
  display.display();
  display.print(zustatekKloc); 
  display.println(" Kc");
  display.display(); 
  display.println("Zadejte castku:");    //VYPÍŠE TEXT
   display.display();
  int IDLukase = 2;
  KontrolaZustatku(IDLukase);
  delay(1000);
  display.clearDisplay(); 
  display.setTextSize(1);
        display.print("Potvrzuji");
        display.display();
    delay(1000);
    display.print(".");
    display.display();
    delay(1000);
    display.print(".");
    display.display();
    delay(1000);
    display.print(".");
    display.display();
 
  }
 //**************************************************************************************************************************************************************************************************   //Benjamin Saaad
 //************************************************************************************************************************************************************************************************** 
 else if(rfid.uid.uidByte[0] == 0xF7 & rfid.uid.uidByte[1] == 0x3F & rfid.uid.uidByte[2] == 0xC1 & rfid.uid.uidByte[3] == 0xA6) {
   //PIN  uživatele
   int aa =56;  //První pin    8  
   int bb =55; //Druhý pin       7
   int cc =54; //Třetí pin      6
   int dd =53; //Čtvrtý pin    5
   int vysledkos=0;
   //*************************************************  
    Serial.println("Detekovana bílá karta 2");
    Serial.println("Přihlášen uživatel Benjamin Saaad");
   //zadáte pin, pokud zadáte pin správně zeptá se vás to kolik budete chtít vybrat a pokud zadáte pin špatně napíše to že jste zadal pin špatně a budeš ho muset opravit (po 5 špatných pokusech se karta zablokuje)
    
    display.begin();
    display.setContrast(65);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.println("Zadejte pin:");    //VYPÍŠE TEXT
    display.display();
    
   do{
 int a =zadanikodu(aa);  //První pin        
  int b =zadanikodu(bb); //Druhý pin
   int c =zadanikodu(cc); //Třetí pin
    int d =zadanikodu(dd); //Čtvrtý pin
     vysledkos = a + b + c + d;
    //SPRÁVNÝ PIN = 4, ŠPATNÝ POD 4
    
  delay(1000);
  display.begin();
  display.setContrast(65);
  display.clearDisplay();
  display.setTextSize(1);
    display.setTextColor(BLACK);
  display.setCursor(0,0);
 if(vysledkos ==4){

  display.println("Uspesne si se prihlasil!");    //VYPÍŠE TEXT
  display.display();
  
  
 }else{
  display.println("Spatny pin");
  display.println("Zadej pin");
  display.println("znovu");
  display.display();
 }
 }while(vysledkos!=4);
      delay(2000);
       display.begin();
  display.setContrast(65);
  display.clearDisplay();
  display.setTextSize(1);
    display.setTextColor(BLACK);
  display.setCursor(0,0);
    display.println("Vas zustatek:");    //VYPÍŠE TEXT
  display.display();
  display.print(zustatekBen); 
  display.println(" Kc");
  display.display(); 
  display.println("Zadejte castku:");    //VYPÍŠE TEXT
   display.display();
  int IDBenika = 3;
  KontrolaZustatku(IDBenika);
  delay(1000);
  display.clearDisplay(); 
  display.setTextSize(1);
        display.print("Potvrzuji");
        display.display();
    delay(1000);
    display.print(".");
    display.display();
    delay(1000);
    display.print(".");
    display.display();
    delay(1000);
    display.print(".");
    display.display();
 
  }
 
  else {
    Serial.println("Detekovan neznamy RFID tag!");
  }
  Serial.println();
  // ukončení komunikace a jejího zabezpečení
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
// podprogram pro výpis adresy RFID tagu v hexa formátu
void vypisHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
