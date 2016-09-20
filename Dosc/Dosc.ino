//Осциллограф Rscope 1.0.1 ©RasyakRoman

#include <CyberLib.h>   // Живе тут: http://cyber-place.ru/showthread.php?t=550

unsigned long currentTime;
unsigned long loopTime;

byte N = 0;
byte Rb = 255;
uint8_t MyBuff[255];

void setup()
{
  UART_Init(256000);
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (0 << ADIF) | (1 << ADIE) | (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
  ADMUX =  (0 << REFS1) | (0 << REFS0) | (1 << ADLAR)  | (0 << MUX3) | (0 << MUX2)  | (0 << MUX1)  | (0 << MUX0) ;
}

void loop()
{

  currentTime = millis();
  if (currentTime >= (loopTime +100))  // сравниваем текущий таймер с переменной loopTime + 1 секунда
  {
    ADCSRA &= ~(1 << ADIE); //Выключить
    UART_SendByte(15); UART_SendByte(0); UART_SendByte(15);
    UART_SendArray(MyBuff, 255);
    UART_SendByte(OCR1A); //Не используются
    UART_SendByte(OCR1B); //Не используются
    UART_SendByte(N);  //Начальная точка
    UART_SendByte(1); UART_SendByte(15);UART_SendByte(1);
    ADCSRA |= (1 << ADIE); // Включить прерывание
    loopTime = currentTime;
  }
  if   (UART_ReadByte(Rb))     {
    ADCSRA = (ADCSRA >> 3) << 3 | Rb ;
  };

}

ISR(ADC_vect)
{
  MyBuff[N] =  ADCH;
  N++;
}


