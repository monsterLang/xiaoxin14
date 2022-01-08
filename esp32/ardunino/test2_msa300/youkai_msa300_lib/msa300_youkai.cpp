#include <Arduino.h>
#include "math.h"
#include "Wire.h"
#include <msa300_youkai.h>

#define uint8_t unsigned char

static uint8_t i2cread(void)
{
    return Wire.read();
}

static void i2cwrite(uint8_t x)
{
    Wire.write((uint8_t)x);
}

static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint8_t value)
{
    Wire.setClock(400000);
    Wire.beginTransmission(i2cAddress);
    i2cwrite(reg);
    i2cwrite(value);
    Wire.endTransmission();
}
static int16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.setClock(400000);
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (int16_t)(i2cread() | (i2cread() << 8));
}


MSA300::MSA300()
{
    for (uint8_t i = 0; i < 7; i++)
    {
        GestureHandle[i] = NULL;
    }
}

void MSA300::init(void)
{
    Wire.begin();
    writeRegister(MSA300_I2C_ADDR, 0x0F, 0x08);
    writeRegister(MSA300_I2C_ADDR, 0x11, 0x00);
    delay(100);
}

float MSA300::getRawX(void)
{
    uint16_t v = 0, retry = 3;
    do
    {
        v = readRegister(MSA300_I2C_ADDR, 0x02);
        if (v == 0xffff)
        {
            init();
        }
    } while ((v == 0xffff) && (--retry));
    rawX = ((int16_t)v) * 1.0 / 4 / 4096;
    return rawX;
}

float MSA300::getRawY(void)
{
    uint16_t v = 0, retry = 3;
    do
    {
        v = readRegister(MSA300_I2C_ADDR, 0x04);
        if (v == 0xffff)
        {
            init();
        }
    } while ((v == 0xffff) && (--retry));
    rawY = ((int16_t)v) * 1.0 / 4 / 4096;
    return rawY;
}

float MSA300::getRawZ(void)
{
    uint16_t v = 0, retry = 3;
    do
    {
        v = readRegister(MSA300_I2C_ADDR, 0x06);
        if (v == 0xffff)
        {
            init();
        }
    } while ((v == 0xffff) && (--retry));
    rawZ = ((int16_t)v) * 1.0 / 4 / 4096;
    return rawZ;
}

float MSA300::getX(void)
{
    if (!isGestureEnable)
    {
        return getRawX() * 1000; // g to mg
    }
    return rawX * 1000;
}

float MSA300::getY(void)
{
    if (!isGestureEnable)
    {
        return getRawY() * 1000; // g to mg
    }
    return rawY * 1000;
}

float MSA300::getZ(void)
{
    if (!isGestureEnable)
    {
        return getRawZ() * 1000; // g to mg
    }
    return rawZ * 1000;
}

float MSA300::getStrength(void)
{
    float x = getX();
    float y = getY();
    float z = getZ();
    return sqrt(x * x + y * y + z * z);
}

void MSA300::onGesture(Gesture gesture, mpythonGestureHandlePtr body)
{
    if (!isGestureEnable)
    {
        xTaskCreatePinnedToCore(MSA300::taskLoop, "gestureTask", 2048, this, 1, NULL, ARDUINO_RUNNING_CORE);
        isGestureEnable = true;
    }
    GestureHandle[gesture] = body;
}

bool MSA300::isGesture(Gesture gesture)
{
    if (!isGestureEnable)
    {
        xTaskCreatePinnedToCore(MSA300::taskLoop, "gestureTask", 2048, this, 1, NULL, ARDUINO_RUNNING_CORE);
        isGestureEnable = true;
    }
    if (gesture != currentGesture)
        return false;
    return true;
}

MSA300 accelerometer;