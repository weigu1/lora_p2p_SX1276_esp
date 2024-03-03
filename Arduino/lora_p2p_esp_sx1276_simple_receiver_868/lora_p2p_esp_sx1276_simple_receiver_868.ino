/*
  lora_p2p_esp_sx1276_simple_receiver_868.ino
  
  weigu.lu 2024

  The receiver uses polling. Look at the transceiver example for callback.
  ESP32:   MH ET LIVE ESP32-MINI-KIT  with RFM95W (SX1276)
  ESP8266: LOLIN/WEMOS D1 mini pro with RFM95W (SX1276)
  
  
      MH ET Live   |---|   Mini Kit              LOLIN/WEMOS |---| D1 Mini Pro

  GND    |  RST    |---|  3 RxD  |    GND        RST         |---|       TxD
   NC    |  SVP 36 |---|  1 TxD  | 27                 A0     |---|       RxD 
  SVN 39 |      26 |---| 22 SCL  | 25                 D0 16  |---|  5 D1 SCL 
      35 |  SCK 18 |---| 21 SDA  | 32             SCK D5 14  |---|  4 D2 SDA
      33 | MISO 19 |---| 17 TxD2 | 12 TDI        MISO D6 12  |---|  0 D3     
      34 | MOSI 23 |---| 16 RxD2 |  4            MOSI D7 13  |---|  2 D4 LED
  TMS 14 |   SS  5 |---|    GND  |  0              SS D8 15  |---|       GND        
   NC    |  3V3    |---|    5V   |  2            3V3         |---|        5V       
  SD2  9 |  TCK 13 |---| 15 TD0  |  8 SD1
  CMD 11 |  SD3 10 |---|  7 SD0  |  6 CLK

  LoRa board     | ESP8266  | ESP32 |
  3.3V (Vin)     | 3.3V     | 3.3V
  GND            | GND      | GND
  MOSI           | 14       | 23
  MISO           | 12       | 19
  SCK            | 13       | 18 
  NSS (SS, CS)   | 16       | 5
  RST (Reset)    | nc       | 33
  DIO0 (G0, IRQ) | 15       | 26
  DIO1 (G1, IRQ) | only WAN | only needed for LoRaWAN!
  DIO2-5         | nc       | not connected (nc)
  ANT            | antenna  | antenna

  freq = 868MHz, SPI pins are default pins MOSI=13/23, MISO=12/19, SCK=14/18
*/

#include <SPI.h>
#include <LoRa.h>

#ifdef ESP8266
  const byte PIN_SS =  16;    // LoRa radio chip select
  const byte PIN_RST = -1;    // LoRa radio reset (not connected)
  const byte PIN_IRQ = 15;    // hardware interrupt pin!
#else
  const byte PIN_SS =  26;
  const byte PIN_RST = -1;    
  const byte PIN_IRQ =  5;
#endif // #ifdef ESP8266

//const byte PIN_SCK =  18;
//const byte PIN_MISO = 19;
//const byte PIN_MOSI = 23;

byte packet_size = 0;

void setup() {
  Serial.begin(115200);  
  Serial.println("LoRa receiver\n");
  //SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_SS); // if not default pins
  LoRa.setPins(PIN_SS, PIN_RST, PIN_IRQ);           // setup LoRa transceiver
  if (!LoRa.begin(868E6)) {
    Serial.println("Error starting LoRa!");
    while (true);                                   // endless loop
  }
}

void loop() {
  packet_size = LoRa.parsePacket();         // try to parse packet
  if (packet_size) {  // received a packet
    Serial.print("Received packet with ");
    Serial.print(packet_size);
    Serial.print(" byte: '");
    while (LoRa.available()) {              // read packet
      Serial.print((char)LoRa.read());
    }
    Serial.print("' with RSSI = ");
    Serial.print(LoRa.packetRssi());
    Serial.println("dBm");
  }
}
