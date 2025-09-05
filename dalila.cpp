#include <Arduino.h>
#include <Keypad.h>
#include <Servo.h>

// TECLADO + SERVO
const byte ROWS = 4, COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int SERVO_PIN = 10;
const int ANG_CLOSE = 0;   
const int ANG_OPEN  = 90;

Servo servo;

const char SECRET[] = "2846";  
char entrada[8];               
byte lenEntrada = 0;
bool estaAberto = false;

void resetEntrada() {
  lenEntrada = 0;
  entrada[0] = '\0';
}

void openServo() {
  estaAberto = true;
  Serial.print("ABRE SERVO");
  servo.write(ANG_OPEN);
}

void closeServo() {
  estaAberto = false;
  Serial.print("FECHA SERVO");
  servo.write(ANG_CLOSE);
}

bool senhaCorreta() {
  if (strlen(entrada) != strlen(SECRET)) {
    Serial.print("Tamanho diferente (");
    Serial.print(strlen(entrada));
    Serial.print(" vs ");
    Serial.print(strlen(SECRET));
    Serial.println(")");
    return false;
  }
  for (byte i = 0; i < strlen(SECRET); i++) {
    if (entrada[i] != SECRET[i]) {
      Serial.print("Mismatch no idx "); Serial.print(i);
      Serial.print(": '"); Serial.print(entrada[i]);
      Serial.print("' vs '"); Serial.print(SECRET[i]); Serial.println("'");
      return false;
    }
  }
  return true;
}

void setup() {

  Serial.begin(9600);
  Serial.println("Debug SENHA + SERVO");
  Serial.print("Senha configurada: "); Serial.println(SECRET);

  servo.attach(SERVO_PIN);
  closeServo();            
  resetEntrada();
}

void loop() {
  char k = keypad.getKey();      
  if (!k) return;                

  Serial.print("Tecla: "); Serial.println(k);

  if (k == 'D') {           
    closeServo();
    resetEntrada();
    return;
  }

  if (k == '*') {               
    resetEntrada();
    Serial.println("Entrada limpa (*)");
    return;
  }

  if (k == '#') {                
    Serial.print("Confirmar (#). Digitado: '");
    Serial.print(entrada);
    Serial.println("'");
    if (senhaCorreta()) {
      Serial.println("Senha CORRETA!");
      openServo();
    } else {
      Serial.println("Senha INCORRETA!");
    }
    resetEntrada();
    return;           
  }

  if ( (k >= '0' && k <= '9') || (k >= 'A' && k <= 'D') ) {
    if (lenEntrada < sizeof(entrada) - 1) {
      entrada[lenEntrada++] = k;
      entrada[lenEntrada] = '\0';
      Serial.print("Buffer: '"); Serial.print(entrada); Serial.println("'");
    } else {
      Serial.println("Buffer cheio -> reset");
      resetEntrada();
    }
  }
}

// Dalila é foda!