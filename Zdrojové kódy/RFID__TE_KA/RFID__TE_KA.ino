// RFID čtečka - ukázka čtení adres tagů

// připojení knihoven SPI a MFRC522
#include <SPI.h>
#include <MFRC522.h>

// definování pinů pro SDA a RST
#define SDA_PIN 10
#define RST_PIN 9

// vytvoření instance RFID čtečky z knihovny
MFRC522 rfid(SDA_PIN, RST_PIN);

void setup() {
  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600);
  // inicializace komunikace přes SPI
  SPI.begin();
  // inicializace komunikace s RFID čtečkou
  rfid.PCD_Init();
}

void loop() {
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

  if(rfid.uid.uidByte[0] == 0x87 & rfid.uid.uidByte[1] == 0xFE & rfid.uid.uidByte[2] == 0x48 & rfid.uid.uidByte[3] == 0x63) {
    Serial.println("Detekovana bila karta!");
    Serial.println("Přihlášen uživatel Vitulinek Petrželka, prosím zadejte pin:");
    //zadáte pin, pokud zadáte pin správně zeptá se vás to kolik budete chtít vybrat a pokud zadáte pin špatně napíše to že jste zadal pin špatně a budeš ho muset opravit (po 5 špatných pokusech se karta zablokuje)
  }
  else if(rfid.uid.uidByte[0] == 0xC9 & rfid.uid.uidByte[1] == 0xE0 & rfid.uid.uidByte[2] == 0x95 & rfid.uid.uidByte[3] == 0xC2) {
    Serial.println("Detekovan modry privesek!");
     Serial.println("Přihlášen uživatel Luky Kloc, prosím zadejte pin:");
     //zadáte pin, pokud zadáte pin správně zeptá se vás to kolik budete chtít vybrat a pokud zadáte pin špatně napíše to že jste zadal pin špatně a budeš ho muset opravit (po 5 špatných pokusech se karta zablokuje)
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
