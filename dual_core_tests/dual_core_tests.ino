#include "dualcore.h"

// --- Global Değişkenlerin Oluşturulması ---
veri paylasilanVeri;
SemaphoreHandle_t veriKilidi;

// Fonksiyon Tanımları
void loopCore0(); // Senin yazacağın LoRa döngüsü
void core0Task(void * pvParameters); // Arka plan işçisi (Task Wrapper)

void setup() {
  Serial.begin(115200);

  // Kilit (Mutex) Oluştur
  veriKilidi = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
    core0Task,   /* Arka plan fonksiyonu */
    "Core0_Isi", /* Görev adı */
    10000,       /* RAM miktarı (Stack size) */
    NULL,        /* Parametre */
    1,           /* Öncelik (1=Düşük, PID etkilenmesin) */
    NULL,        /* Handle */
    0            /* ---> CORE 0 <--- */
  );

  Serial.println("Sistem Hazir.");
}

void loop() {
  
  // // ÖRNEK: PID Hesaplamaları...
  // float hesaplanan_a = 45.5; // (Senin PID kodun burada olacak)
  
  // // --- VERİ AKTARIMI (YAZMA) ---
  // // Kilidi al ve veriyi kutuya koy
  // if (xSemaphoreTake(veriKilidi, (TickType_t)5) == pdTRUE) {
  //   paylasilanVeri.a = hesaplanan_a;
  //   // paylasilanVeri.ivme = ...
  //   xSemaphoreGive(veriKilidi);
  // }
  delay(1000);

  Serial.print("core: "); Serial.println(xPortGetCoreID());

  // PID döngü hızı (Örn: 10ms gecikme)
  // vTaskDelay, delay() fonksiyonunun daha verimli halidir.
  // vTaskDelay(1 / portTICK_PERIOD_MS); 
}

void loopCore0() {
  
  // veri gonderilecekVeri; // Yerel kopya

  // // --- VERİ OKUMA ---
  // // Kilidi al ve kutudan veriyi kopyala
  // if (xSemaphoreTake(veriKilidi, (TickType_t)5) == pdTRUE) {
  //   memcpy(&gonderilecekVeri, &paylasilanVeri, sizeof(veri));
  //   xSemaphoreGive(veriKilidi);
  // }

  // // --- LORA GÖNDERİMİ (Burada özgürsün) ---
  // // LoRa işlemleri 50-100ms sürse bile sorun değil.
  // // Serial2.print(...) veya LoRa.print(...)
  
  // Serial.print("[Core 0] Gonderilen Aci: ");
  // Serial.println(gonderilecekVeri.a);
  
  Serial.print("core: "); Serial.println(xPortGetCoreID());


  // Telemetri hızı (Örn: 100ms)
  // BURAYA DİKKAT: Core 0'da da mutlaka minik bir delay olmalı!
  vTaskDelay(500 / portTICK_PERIOD_MS);
}

void core0Task(void * pvParameters) {
  // Core 0 için özel setup kodları (varsa) buraya yazılabilir.
  // LoRa.begin() veya Serial2.begin() buraya da konabilir, setup'a da.
  
  for(;;) { // Sonsuz döngü
    loopCore0(); // Senin fonksiyonunu çağırır
    // Eğer loopCore0 içinde hiç delay yoksa, burası işlemciyi kurtarır.
    vTaskDelay(1); 
  }
}