/*
   Fil: irRemote
   Namn: Manne Laurin
   Datum: 2023-11-09
   Beskrivning: Arduino-kod som läser IR-signaler från en fjärrkontroll och sparar signalerna för att använda dem senare.
*/

#include <IRremote.hpp>

int irSendPin = 3;
int IR_RECEIVE_PIN = 7; // Receivern och sändaren är kopplade till pins 7 och 3

// Det går här att hårdkoda in signaler om man vill kunna sända utan att behöva läsa
int addressOne = 0x3000;
int commandOne = 0x4F;
String protocolOne = "NEC";

int addressTwo = 0x3000;
int commandTwo = 0x4E;
String protocolTwo = "NEC";

void setup() {
  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Starta receiver
  IrSender.begin(irSendPin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // Starta sändaren

  pinMode(2, INPUT);  // Aktiverar signal 1
  pinMode(8, INPUT);  // Aktiverar signal 2
  pinMode(4, INPUT);  // Spar-knapp
}

void loop() {
  irReceive();
  irSend();
}

/*
   Tar emot och sparar signaler
   Uppdaterar variabler för protokoll, adress och kommando som sedan används till irSend.
*/
void irReceive() {
  // Om spar-knappen trycks ned samtidigt som någon av signalknapparna sparas signaler
  if ((digitalRead(4) == HIGH) && (digitalRead(2) == HIGH)) {
    if (IrReceiver.decode()) {
      IrReceiver.printIRResultShort(&Serial); // Skriv ut komplett mottagen data i en rad (används mest för att kontrollera att variablerna får rätt värden)
      IrReceiver.printIRSendUsage(&Serial);   // Skriv ut instruktionen som krävs för att skicka dessa data

      addressOne = IrReceiver.decodedIRData.address;
      Serial.print("Address: ");
      Serial.println(addressOne, HEX);

      commandOne = IrReceiver.decodedIRData.command;
      Serial.print("Command: ");
      Serial.println(commandOne, HEX);

      // Protokollen lagras inte som strängar i biblioteket, därför används en if-sats för att uppdatera protokoll
      if (IrReceiver.decodedIRData.protocol == SONY) {
        protocolOne = "SONY";
      } else if (IrReceiver.decodedIRData.protocol == NEC) {
        protocolOne = "NEC";
      }

      Serial.print("Protocol: ");
      Serial.println(protocolOne);

      Serial.println(" "); // Gör så att det blir lite mellanrum så att terminalen blir lite mer läsbar

      IrReceiver.resume(); // Aktivera mottagning av nästa värde
    }
  } else if ((digitalRead(4) == HIGH) && (digitalRead(8) == HIGH)) {
    if (IrReceiver.decode()) {
      IrReceiver.printIRResultShort(&Serial); // Skriv ut komplett mottagen data i en rad (används mest för att kontrollera att variablerna får rätt värden)
      IrReceiver.printIRSendUsage(&Serial);   // Skriv ut instruktionen som krävs för att skicka dessa data

      addressTwo = IrReceiver.decodedIRData.address;
      Serial.print("Address: ");
      Serial.println(addressTwo, HEX);

      commandTwo = IrReceiver.decodedIRData.command;
      Serial.print("Command: ");
      Serial.println(commandTwo, HEX);

      if (IrReceiver.decodedIRData.protocol == SONY) {
        protocolTwo = "SONY";
      } else if (IrReceiver.decodedIRData.protocol == NEC) {
        protocolTwo = "NEC";
      }

      Serial.print("Protocol: ");
      Serial.println(protocolTwo);

      Serial.println(" "); // Gör så att det blir lite mellanrum så att terminalen blir lite mer läsbar

      IrReceiver.resume(); // Aktivera mottagning av nästa värde
    }
  }
}

/*
   Skickar signaler som tagits emot. Send-metoden varierar beroende på protokoll, därav if-satsen.
*/
void irSend() {
  if ((digitalRead(2) == HIGH) && (digitalRead(4) == LOW)) {
    if (protocolOne == "NEC") {
      IrSender.sendNEC(addressOne, commandOne, 3);
    } else if (protocolOne == "SONY") {
      IrSender.sendSony(addressOne, commandOne, 3);
    }
  } else if ((digitalRead(8) == HIGH) && (digitalRead(4) == LOW)) {
    if (protocolTwo == "NEC") {
      IrSender.sendNEC(addressTwo, commandTwo, 3);
    } else if (protocolTwo == "SONY") {
      IrSender.sendSony(addressTwo, commandTwo, 3);
    }
  }
}
