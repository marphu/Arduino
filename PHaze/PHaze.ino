// Пины подключения выключателей
unsigned short int Phaze[] = { 0, 1, 2};
// Переменная для циклов
unsigned short int i;
// Переменная длительности определения изменения состояний
unsigned long int t;
// Начальное состояние
unsigned short int State[3] = {0, 0, 0};
// Начальное старое состояние
unsigned short int oStat[3] = {0, 0, 0};
//
unsigned short int Res[3] = {0, 0, 0};
// Размер буффера
unsigned short BSize = 10;
// Инициализация буффера для приема данных
char bufer[10];
// Размер выводной строки (может нужно сделать поменьше)
char output[20];
// Массив пинов отключения
//int pino[] = { 22, 24, 26, 28, 30, 32, 35};
unsigned short int pino[] = { 11,  9,  7,  5,  3};
// Массив пинов включения
//int pinc[] = { 23, 25, 27, 29, 31, 34, 36};
unsigned short int pinc[] = { 10,  8,  6,  4,  2};

unsigned short int n = 5;
// Строка состояний реле. При каждом изменении
// меняется соответствующая позиция в строке и
// выводиться на ком порт
char state[6] = "iiiii";
// Задержка для реле, мс
int RDelay = 200;

#define helpm true

void setup() {
  // Инициализация последовательного порта
  Serial.begin(9600);
  for (i = 0; i < 3; i++) {
    // Перебор фаз, Включение пина на прием
    // И запись старого состояния
    pinMode(Phaze[i], INPUT);
    oStat[i] = State[i];
  }
  // переборр элементов массива пинов включения/отключения
  for (i = 0; i < n ; i++ ) {
    // Пины включения отключения конфигурируются на выход
    pinMode(pino[i], OUTPUT);
    pinMode(pinc[i], OUTPUT);
    // На пинах задается логический ноль
    digitalWrite(pino[i], LOW);
    digitalWrite(pinc[i], LOW);
  }
  for (i = 0; i < n ; i++ ) {
    // но не всегда срабатывает реле
    // На пине замыкания логический ноль
    digitalWrite(pinc[i], HIGH);
  }
  // Задержка
  delay(RDelay);
  for (i = 0; i < n ; i++ ) {
    // но не всегда срабатывает реле
    // На пине замыкания логический ноль
    digitalWrite(pinc[i], LOW);
  }
  // Задержка
  delay(RDelay);
  for (i = 0; i < n ; i++ ) {
    // На пине Размыкания логическая 1
    digitalWrite(pino[i], HIGH);
    // Изменение строки состояния
    state[i] = 'O';
    //delay(RDelay/5);
  }
  // Задержка
  delay(RDelay);
  for (i = 0; i < n ; i++ ) {
    // Логический ноль на пине размыкания
    digitalWrite(pino[i], LOW);
  }
  // вывод строки состояния
  // Serial.println(state);
}

void KeyScan() {
  // Проверка состояний выключателя
  for (i = 0; i < 3; i++) {
    if ( analogRead(Phaze[i]) > 512) {
      State[i] = 1;
    } else {
      State[i] = 0;
    }
  }
}

void Sled(unsigned int num) {
  // Начало слежения
  t = millis();
  KeyScan();
  for (i = 0; i < 3; i++) {
    oStat[i] = State[i];
  }
  long int n = millis();
  while ((millis() - t) < num * 1000) {
    KeyScan();
    for (i = 0; i < 3; i++) {
      if (State[i] != oStat[i]) {
        sprintf(output, "%d: %d (%ld)", i + 1, State[i], (millis() - n));
        Serial.println(output);
        t++;
      }
    }
    for (i = 0; i < 3; i++) {
      oStat[i] = State[i];
    }
  }
}

void ReadComPort() {
  // Вслучае если в ком порте есть какие-то данные
  if (Serial.available()) {
    i = 0;
    // ЗадержкаЮ для получения всех данных
    delay(100);
    // Пока есть данные и пока емть место в буфере
    while ( Serial.available() && i < (BSize - 1)) {
      // Читать данные из ком-порта в буфер
      bufer[i++] = Serial.read();
    }
    // Добавление в конец прочтенных данных нулевого символа
    // для коректной обработки строки
    bufer[i++] = '\0';
  }
}

int StrLength(char * Str) {
  int len;
  for (len = 0; len <= 10; len++) {
    if ( Str[len] == '\0' ) {
      break;
    }
  }
  return len;
}

int NumLast(char * Str) {
  unsigned short int nn;
  nn = StrLength(Str);
  char tmpstr[nn - 2];
  for (i = 2; i < nn; i++) {
    tmpstr[i - 2] = Str[i];
  }
  return atoi(tmpstr);
}

void CORele(bool action, int rele) {
  if ( rele != 0 ) {
    rele--;
    int pc, po;
    if (action) {
      pc = LOW;
      po = HIGH;
      // Изменение строки состояния
      state[rele] = 'o';
    } else {
      po = LOW;
      pc = HIGH;
      // Изменение строки состояния
      state[rele] = 'c';
    }
    // На пине отключения pc
    digitalWrite(pino[rele], pc);
    // На пине включения po
    digitalWrite(pinc[rele], po);
    // Задержка
    delay(RDelay);
    // На пине включения и отключения 0
    digitalWrite(pinc[rele], LOW);
    digitalWrite(pino[rele], LOW);
    //
    Serial.println(state);
  }
}
#if helpm
void Usage() {
  Serial.println("Commands: ");
  Serial.println("  a   - State of Switches (Voltage on pins > 0.5 AREF)");
  Serial.println("  s,t - Time of Changing State of switches");
  Serial.println("        t seconds");
  Serial.println("  c,n - Close Rele n");
  Serial.println("  o,n - Open Rele n");
  Serial.println("");
  Serial.println("  Switch pins:");
  Serial.println("    1: A0, 2: A1, 3: A2");
  Serial.println("  Open pins:");
  Serial.println("    11,  9,  7,  5,  3");
  Serial.println("  Close pins:");
  Serial.println("    10,  8,  6,  4,  2");
  Serial.println("");
}
#endif
void loop() {
  ReadComPort();
  switch (bufer[0]) {
    case 's':
      bufer[0] = 'q';
      Sled(NumLast(bufer));
      break;
    case 'a':
      bufer[0] = 'q';
      KeyScan();
      Serial.print(State[0]);
      Serial.print(State[1]);
      Serial.println(State[2]);
      break;
    case 'o':
      bufer[0] = 'q';
      i = NumLast(bufer);
      if ( i > n || i < 1) {
        i = 0;
      }
      CORele(true, i );
      break;
    case 'c':
      bufer[0] = 'q';
      i = NumLast(bufer);
      if ( i > n || i < 1) {
        i = 0;
      }
      CORele(false, i );
      break;
    case 'h':
#if helpm
      Usage();
#endif
      break;
  }
  memset(bufer, 0, sizeof(bufer));
}
