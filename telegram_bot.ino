/*
   Dario Ciceri
   Codice per controllare qualsiasi cosa tramite Telegram e una scheda esp32
   Profilo Instragram: https://www.instagram.com/_dario.ciceri_/
   Pagina GitHub: https://github.com/Dario-Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
*/

#include <WiFi.h> //libreria per il wifi (meglio non modificare)
#include <WiFiClientSecure.h> //libreria per il wifi (meglio non modificare)
#include <UniversalTelegramBot.h> //libreria per telegram (meglio non modificare)

#define WIFI_SSID "NOME_WIFI" //nome wifi (potete modificarlo)
#define PASSWORD_WIFI "PASSWORD_WIFI" //password wifi (potete modificarlo)
#define BOT_API_TOKEN "TOKEN_BOT" //token chatbot telegram (potete modificarlo)

const unsigned long intervallo = 1000; //pausa per la ricezione dei messaggi (potete modificarlo)
unsigned long ultima_chiamata = 0; //gestione del millis (meglio non modificare)

WiFiClientSecure s_client; //wifi client (meglio non modificare)
UniversalTelegramBot bot(BOT_API_TOKEN, s_client); //telegram client (meglio non modificare)

void gestisci_messaggi(int num_nuovi_messaggi) { //funzione per la gestione della chat telegram

  for (int i = 0; i < num_nuovi_messaggi; i++) {

    String chat_id = bot.messages[i].chat_id; //id univoco della chat
    String text = bot.messages[i].text; //testo inviato in chat dall'utente
    String from_name = bot.messages[i].from_name; //chi manda il messaggio

    if (from_name == "") {
      from_name = "Sconosciuto"; //se non possiamo sapere chi ci scrive, allora è uno sconosciuto
    }

    if (text == "/start") { //se il testo ricevuto è /start allora invia il messaggio di benvenuto
      bot.sendMessage(chat_id, "Benvenuto");
    }

    Serial.println(from_name); //debug su seriale
    Serial.println(chat_id); //debug su seriale
    Serial.println(text); //debug su seriale
  }
}

void setup() {
  Serial.begin(115200); //inizializzo la seriale su 115200 baudrate

  Serial.println(); //debug su seriale
  Serial.print("Connetto alla rete wifi "); //debug su seriale
  Serial.println(WIFI_SSID); //debug su seriale

  WiFi.begin(WIFI_SSID, PASSWORD_WIFI); //connetto al wifi

  s_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); //creo una connessione con Telegram utilizzando un certificato di sicurezza

  while (WiFi.status() != WL_CONNECTED) { //finchè il wifi non è connesso attendi e manda dei puntini di sospensione su seriale
    Serial.print("."); //debug su seriale
    delay(500);
  }

  Serial.print("Wifi connesso. Indirizzo IP: "); //debug su seriale
  Serial.println(WiFi.localIP()); //debug su seriale
}

void loop() {
  if (millis() - ultima_chiamata > intervallo) { //se il valore del timer "millis" - l'ultimo valore in millisecondi di quando abbiamo eseguito le istruzioni, è maggiore del tempo di attesa "intervallo", allora esegui le istruzioni
    int num_nuovi_messaggi = bot.getUpdates(bot.last_message_received + 1); //aggiorna la lista dei messaggi in chat
    while (num_nuovi_messaggi) { //finchè ci sono nuovi messaggi, processali
      Serial.println("messaggio ricevuto"); //debug su seriale
      gestisci_messaggi(num_nuovi_messaggi);
      num_nuovi_messaggi = bot.getUpdates(bot.last_message_received + 1);
    }
    ultima_chiamata = millis(); //aggiorna l'ultimo periodo in millisecondi in cui sono state eseguite le istruzioni all'interno del ciclo while
  }
}
