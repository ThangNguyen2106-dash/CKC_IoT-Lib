#include <khaibao.h>
#include <CKC_IoT.hpp>
CKC CKC_IoT;
void setup()
{
  Serial.begin(115200);
  CKC_IoT.begin("MakerSpaceLab", "Maker2025");
  delay(20000);
  CKC_IoT.end();
}
void loop()
{
}
