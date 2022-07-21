#include <iarduino_OLED_txt.h>

extern uint8_t SmallFontRus[];

iarduino_OLED_txt oled(0x3C);

#define ADC_COUNT 1023
#define ADC_COUNT_HALF ADC_COUNT / 2
#define ADC_COUNT_ACS712_1A 37.88
#define RMS 0.707
#define SAMPLE_TIMES 32
#define FAULT_ADC 0.011
#define VOLT_PIN A1
#define VREF 5.102
#define V_COEF 1.045
#define DIV_R1 19680
#define DIV_R2 1176

int sensorValue;
float current;
float voltage;
int voltage_raw;

float readCurrentDC()
{
    sensorValue = 0;
    float sensorCurrent = 0.00;
    for (int i = 0; i < SAMPLE_TIMES; i++)
    {
        sensorValue += analogRead(A3) - 119;
    }

    sensorValue = sensorValue >> 5;
    sensorCurrent = ((float)sensorValue * 100 * 1.237) / (float)ADC_COUNT;
    return sensorCurrent;
}

float readVoltageDC()
{
    sensorValue = 0;
    float sensorVoltage = 0;
    for (int i = 0; i < SAMPLE_TIMES; i++)
    {
        sensorValue += analogRead(VOLT_PIN);
    }
    sensorValue = sensorValue >> 5;
    sensorVoltage = sensorValue * VREF * V_COEF * ((DIV_R1 + DIV_R2) / DIV_R2) / ADC_COUNT;
    return sensorVoltage;
}

void setup()
{

    oled.begin();
    oled.setFont(SmallFontRus);
}

void loop()
{

    current = readCurrentDC();
    // voltage = analogRead(VOLT_PIN) * VREF * V_COEF * ((DIV_R1 + DIV_R2) / DIV_R2) / 1024;
    // current = (100 * analogRead(A3)) / ADC_COUNT;

    oled.clrScr();
    oled.print("Ток,А:", 0, 0);
    oled.print(String(current), 50, 0);
    oled.print("ТокАЦП", 0, 1);
    oled.print(sensorValue, 50, 1);
    voltage = readVoltageDC();
    oled.print("Напр.:", 0, 2);
    oled.print(voltage, 50, 2);
    delay(1000);
}
