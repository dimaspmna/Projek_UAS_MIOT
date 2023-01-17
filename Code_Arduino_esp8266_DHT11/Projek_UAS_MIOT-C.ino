// Library 
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


// Mendefinisikan pin dan tipe sensor DHT
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const long utcOffsetInSeconds = 25200;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetInSeconds);

// Harus diisi
#define FIREBASE_HOST "https://fir-miotdht11-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "B3YaLOcvPxvfc0Tk6Oy7rCiokCL26pUSx0wtoOHt"
#define WIFI_SSID "Hujan"
#define WIFI_PASSWORD "hujan293"  

// mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;

void setup() {
  
  Serial.begin(115200);
  
  dht.begin();
  
  // Koneksi ke Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  timeClient.begin();
}

void loop(){

  //clock
    timeClient.update();
  
  // Sensor DHT11 membaca suhu dan kelembaban
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Memeriksa apakah sensor berhasil mambaca suhu dan kelembaban
  if (isnan(t) || isnan(h)) {
    Serial.println("Gagal membaca sensor DHT11");
    return;
  }

  //menampilkan waktu
  Serial.print("hari: ");
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print("jam: ");
  Serial.print(timeClient.getHours());
  Serial.print("menit: ");
  Serial.print(timeClient.getMinutes());
  Serial.print("detik: ");
  Serial.println(timeClient.getSeconds());
  Serial.println();
  
  // Menampilkan suhu dan kelembaban pada serial monitor
  Serial.print("suhu: ");
  Serial.print(t);
  Serial.println(" *C");
  Serial.print("kelembapan: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.println();

  // Memberikan status waktu kepada firebase
  if (Firebase.setString(firebaseData, "/Hasil_Pembacaan/hari", daysOfTheWeek[timeClient.getDay()])){
      Serial.println("Hari terkirim");
    } else{
      Serial.println("Hari tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    } 

  if (Firebase.setString(firebaseData, "/Hasil_Pembacaan/jam", timeClient.getHours())){
      Serial.println("Jam terkirim");
    } else{
      Serial.println("Jam tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }

  if (Firebase.setString(firebaseData, "/Hasil_Pembacaan/menit", timeClient.getMinutes())){
      Serial.println("Menit terkirim");
    } else{
      Serial.println("Menit tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }

  if (Firebase.setString(firebaseData, "/Hasil_Pembacaan/detik", timeClient.getSeconds())){
      Serial.println("Detik terkirim");
    } else{
      Serial.println("Detik tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }


  // Memberikan status suhu dan kelembaban kepada firebase
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/suhu", t)){
      Serial.println("Suhu terkirim");
    } else{
      Serial.println("Suhu tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    } 
    
  if (Firebase.setFloat(firebaseData, "/Hasil_Pembacaan/kelembaban", h)){
      Serial.println("Kelembaban terkirim");
      Serial.println();
    } else{
      Serial.println("Kelembaban tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }
    
  delay(1000);
}
