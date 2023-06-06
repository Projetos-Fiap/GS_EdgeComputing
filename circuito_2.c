// GLOBAL SOLUTION

// Para simular o funcionamento de um refrigerador, 
// definimos como parâmetros ideais a temperatura entre
// 2 e 8 graus celsius, e a umidade entre 40 e 50%

// O potênciometro simula a mudança de umidade

#include <LiquidCrystal.h>

// Definições dos pinos
const int GREEN_LED_PIN = 11; 
const int RED_LED_PIN = 12; 
const int TEMP_SENSOR_PIN = A0;
const int HUMIDITY_SENSOR_PIN = A1;

// Definição limite umidade
const int HUM_OK_MIN_LEVEL = 40;
const int HUM_OK_MAX_LEVEL = 50;

// Definição limite temperatura
const float TEMP_OK_MIN_LEVEL = 2;
const float TEMP_OK_MAX_LEVEL = 8;

// LCD settings
const int RS = 9, EN = 8, DB4 = 5, DB5 = 4, DB6 = 3, DB7 = 2;
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

// Variáveis para leitura do sensor
int rawTempValue = 0;
int rawHumidityValue = 0;

// Variáveis para armazenar leituras
float humidityMeasurements[6] = {0,0,0,0,0,0};
float tempMeasurements[6] = {0,0,0,0,0,0};

// Variável para controlar mudança do LCD
int order = 0;

void setup() {
// Definição dos pinos dos LEDs
pinMode(GREEN_LED_PIN, OUTPUT);
pinMode(RED_LED_PIN, OUTPUT);

// Iniciar LCD
lcd.begin(16,2);
}


///// SENSORES /////

void loop() {
  int inicio = millis();
	while(true) { // Loop infinito 
  
  // Ler sensores
  rawTempValue = analogRead(TEMP_SENSOR_PIN);
  rawHumidityValue = analogRead(HUMIDITY_SENSOR_PIN);
  
  // Converter temperatura para Celsius
  float voltage = (rawTempValue / 1023.0) * 5000.0;
  float temperatureC = (voltage - 500.0) * 0.1;
  
  // Converter humidade para porcentagem
  int humidityPercentage = map(rawHumidityValue, 0, 1023, 10, 70);
  
      
  // Leitura de umidade
  humidityMeasurements[int(humidityMeasurements[0])+1] = float(humidityPercentage);
    
  if(humidityMeasurements[0] < 4){
    humidityMeasurements[0]++;
  } else {
  	humidityMeasurements[0] = 0;	
  }
      
  // Leitura de temperatura
  tempMeasurements[int(tempMeasurements[0])+1] = float(temperatureC);
  if(tempMeasurements[0] < 4){
    tempMeasurements[0]++;
  } else {
  	tempMeasurements[0] = 0;	
  }
      
           
      
///// LED DE AVISO    
                  
   // Temp fora = Led vermelho
   if (temperatureC < TEMP_OK_MIN_LEVEL || temperatureC > TEMP_OK_MAX_LEVEL) {
     digitalWrite(GREEN_LED_PIN, LOW);
     digitalWrite(RED_LED_PIN, HIGH);
   }

   // Umidade fora = Led Vermelho
   if (humidityPercentage < HUM_OK_MIN_LEVEL || humidityPercentage > HUM_OK_MAX_LEVEL) {
     digitalWrite(GREEN_LED_PIN, LOW);
     digitalWrite(RED_LED_PIN, HIGH);
   }                       
  
///// LCD /////    
      
  int agora = millis();
  if(agora - inicio > 2000){
    if(order < 2){
    	order++;
    } else {
    	order = 0;
    }
    inicio = millis();       
  }
      
      
      if(order == 0){
        // Gets average temperature level
        float tempAverage = (tempMeasurements[1] + tempMeasurements[2] + tempMeasurements[3] + tempMeasurements[4] + tempMeasurements[5])/5;
        
        // Prints temperature to LCD
        if (TEMP_OK_MIN_LEVEL <= tempAverage && tempAverage <= TEMP_OK_MAX_LEVEL) {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Temperatura:");
          lcd.setCursor(0,1);
          lcd.print(tempAverage, 1);
          lcd.print("C  OK");
        }
        else {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Temperatura:");
          lcd.setCursor(0,1);
          lcd.print(tempAverage, 1);
          lcd.print("C  ");
          if (tempAverage < TEMP_OK_MIN_LEVEL) {
            lcd.print("BAIXA");
          }
          else {
            lcd.print("ALTA");
          }
        }
      }

      if(order == 1) {
        // Gets Average humidity percentage
        float humidityAverage = (humidityMeasurements[1] + humidityMeasurements[2] + humidityMeasurements[3] + humidityMeasurements[4] + humidityMeasurements[5])/5;
        
      	// Prints humidity to LCD
        if (HUM_OK_MIN_LEVEL <= humidityAverage && humidityAverage <= HUM_OK_MAX_LEVEL) {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Umidade:");
          lcd.setCursor(0,1);
          lcd.print(humidityAverage, 1);
          lcd.print("%  OK");
        }
        else {
          lcd.clear(); 
          lcd.setCursor(0,0);
          lcd.print("Umidade:");
          lcd.setCursor(0,1);
          lcd.print(humidityAverage, 1);
          lcd.print("%  ");
          if (humidityAverage < HUM_OK_MIN_LEVEL) {
            lcd.print("BAIXA");
          }
          else {
            lcd.print("ALTA");
          }
        } 
      }

 } 
}
