#ifndef DUALCORE_H
#define DUALCORE_H

#include <Arduino.h>
#include "freertos/semphr.h"

// Veri Paketi Yapısı
struct veri {
  float a;
  float b;
  int c;
};

// Bu değişkenler "Main.ino"da oluşturulacak ama herkes görebilecek
extern veri paylasilanVeri;
extern SemaphoreHandle_t veriKilidi;

#endif