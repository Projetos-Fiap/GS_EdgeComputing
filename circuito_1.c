// GLOBAL SOLUTION

// A senha padrão é "456"

// Inclusão das bibliotecas utilizadas no código
#include <Servo.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
Servo ServoMotor;

// Definimos a senha
char* password = "456"; 

// Endereço e dimensões do LCD
LiquidCrystal_I2C lcd(0x20, 16, 2); 

// Definições para o Keypad 4x4
int position = 0;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Variáveis para os LEDs e Buzzer
const int redledPin = 12;
const int greenledPin = 10;
const int buzzerPin = 13;

// Variável para controle do tempo de acionamento do buzzer
unsigned long buzzerStartTime = 0;

void setup() {
// Inicialização do Display LCD  
lcd.init();
lcd.backlight();
  
// Definições dos pinos  
pinMode(redledPin, OUTPUT);
pinMode(greenledPin, OUTPUT);
pinMode(buzzerPin, OUTPUT);
ServoMotor.attach(11);
LockedPosition(true);
}

void loop() {
  char key = keypad.getKey();

  if (key == '*' || key == '#') {
    position = 0;
    LockedPosition(true);
  }

  if (key == password[position]) {
    position++;
  }

  if (position == 3) {
    LockedPosition(false);
  }

  delay(100);
}

void LockedPosition(int locked) {
  if (locked) {
    digitalWrite(redledPin, HIGH);
    digitalWrite(greenledPin, LOW);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("ACESSO NEGADO");
    ServoMotor.write(90);
  } else {
    digitalWrite(redledPin, LOW);
    digitalWrite(greenledPin, HIGH);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("ACESSO LIBERADO");
    ServoMotor.write(0);
	for (int i = 0; i < 3; i++) {
  		tone(buzzerPin, 1500);
  		delay(200);
  		noTone(buzzerPin);
  		delay(100);
		}
    
  }
}