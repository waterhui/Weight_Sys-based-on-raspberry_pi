#ifndef HX711_H
#define HX711_H

#include <wiringPi.h>


class hx711
{
public:
    hx711();
    static void Setup();
    static float get_real_Weight();
private:
    static int HX711_SCK;
    static int HX711_DT;
    static long HX711_Buffer;
    static long Weight_Maopi;
    static long Weight_Shiwu;
    static int GapValue;
    static void Init_HX711();
    static unsigned long HX711_read();
    static long get_Weight();
    static void Get_Maopi();
    static float Weight;
    static float WeightTmp;
};

#endif // HX711_H
