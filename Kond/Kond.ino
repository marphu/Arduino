
int led = 13;  //объявление переменной целого типа, содержащей номер порта к которому мы подключили второй провод

void setup(){  //обязательная процедура setup, запускаемая в начале программы; объявление процедур начинается словом void
  pinMode(led, OUTPUT); //объявление используемого порта, led - номер порта, второй аргумент - тип использования порта - на вход (INPUT) или на выход (OUTPUT)
}

void loop(){ //обязательная процедура loop, запускаемая циклично после процедуры setup
  digitalWrite(led, HIGH); //эта команда используется для включения или выключения напряжения на цифровом порте; led - номер порта, второй аргумент - включение (HIGH) или выключение (LOW)
  delay(1000); //эта команда используется для ожидания между действиями, аргумент - время ожидания в миллисекундах
  digitalWrite(led, LOW);
  delay(1000);
}

