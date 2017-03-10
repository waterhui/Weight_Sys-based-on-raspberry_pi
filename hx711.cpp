#include "hx711.h"
#include <stdio.h>

hx711::hx711()
{
    Setup();
}
int hx711::HX711_SCK = 0;
int hx711::HX711_DT  = 4;
long hx711::HX711_Buffer = 0;
long hx711::Weight_Maopi = 0;
long hx711::Weight_Shiwu = 0;
//    GapValue =533.5;   //55   //528
int hx711::GapValue = 521;
float hx711::Weight = 0;
float hx711::WeightTmp = 0;


void hx711::Setup()
{
    Init_HX711();
    Get_Maopi();
}

void hx711::Init_HX711()
{
    wiringPiSetup();
    pinMode(HX711_SCK,OUTPUT);
    pinMode(HX711_DT,INPUT);
}

void hx711::Get_Maopi()
{
    Weight_Maopi = HX711_read();
    if(Weight_Maopi > 8458663)//8411000
    {
        Weight_Maopi = 8458663;
    }
}

long hx711::get_Weight()
{
    HX711_Buffer = HX711_read();
    Weight_Shiwu = HX711_Buffer;
    Weight_Shiwu = Weight_Shiwu - Weight_Maopi;
    Weight_Shiwu = (long)((float)(Weight_Shiwu/GapValue) * 10);
    return Weight_Shiwu;
}

unsigned long hx711::HX711_read()
{
    unsigned long count;
    unsigned char i;
    digitalWrite(HX711_DT,HIGH);
    delayMicroseconds(1);
    digitalWrite(HX711_SCK,LOW);
    delayMicroseconds(1);
    count = 0;
    while(digitalRead(HX711_DT));
    for(i = 0; i < 24; i++)
    {
        digitalWrite(HX711_SCK,HIGH);
        delayMicroseconds(1);
        count  = count<< 1;
        digitalWrite(HX711_SCK,LOW);
        delayMicroseconds(1);
        if(digitalRead(HX711_DT))
            count++;
    }
    digitalWrite(HX711_SCK,HIGH);
    count ^= 0x800000;
    delayMicroseconds(1);
    digitalWrite(HX711_SCK,LOW);
    delayMicroseconds(1);
    return count;
}

float hx711::get_real_Weight()
{
    WeightTmp = Weight;
    Weight = get_Weight();
    return Weight;
}
