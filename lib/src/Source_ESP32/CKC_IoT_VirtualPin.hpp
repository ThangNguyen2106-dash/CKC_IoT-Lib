#include <Source_ESP32/CKC_Class.hpp>
CKC_Virtualpin CKC_Virtual;

CKC_Virtualpin::CKC_Virtualpin()
{
    for (int i = 0; i < MAX_VIRTUAL_PIN; i++)
        vpCallback[i] = nullptr;
}

void CKC_Virtualpin::attach(uint8_t vPin, pinCallBack cb)
{
    if (vPin >= MAX_VIRTUAL_PIN)
        return;

    vpCallback[vPin] = cb;
}

void CKC_Virtualpin::write(uint8_t vPin, int value)
{
    if (vPin >= MAX_VIRTUAL_PIN)
        return;

    if (vpCallback[vPin])
        vpCallback[vPin](value);
}