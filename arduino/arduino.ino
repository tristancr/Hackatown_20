#include <dht.h>
#include <LiquidCrystal.h>

#define DHT11_PIN       6
#define BUZZER_PIN      9
#define LED_PIN         13
#define HCSR04_TRIGPIN  A2
#define HCSR04_ECHOPIN  A3

dht DHT;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
unsigned long start;
bool humidityOK = false;
bool temperatureOK = false;
int humidity = 0;
int temperature = 0;
int door = 0;

byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};

byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};

bool doorIsOpen() {
  digitalWrite(HCSR04_TRIGPIN, HIGH);
  delay(10);
  digitalWrite(HCSR04_TRIGPIN, LOW);

  int distance = pulseIn(HCSR04_ECHOPIN, HIGH)*(0.17); // en millimetres
  return (distance > 130);
}

void message(String message, int col, int row, bool clearLcd) {
  if (clearLcd) {
    lcd.clear();
  }
  lcd.setCursor(col, row);
  lcd.print(message);
}

void displayHumidityTemperature() {
  message("HUM  " + String(humidity) + " % ", 0, 0, true);
  
  humidityOK = false;
  if (humidity < 25) {
    lcd.write(byte(0));
    lcd.print(" " + String(humidity - 25) + " %");
  } else if (humidity > 40) {
    lcd.write(byte(1));
    lcd.print(" " + String(humidity - 40) + " %");
  } else {
    humidityOK = true;
  }
  
  message("TEMP " + String(temperature) + (char)223 + "C ", 0, 1, false);

  temperatureOK = false;
  if (temperature > 5) {
    lcd.write(byte(1));
    lcd.print(" " + String(temperature - 5) + (char)223 + "C");
  } else if (temperature == 0) {
    lcd.write(byte(0));
  } else {
    temperatureOK = true;
  }

  if (humidityOK && temperatureOK) {
      digitalWrite(LED_PIN, LOW);
  } else {
      digitalWrite(LED_PIN, HIGH);
  }
  
  delay(2000);
}

void alertHumidityTemperature() {
  message("HUM ", 0, 1, false);
  
  if (humidity < 25) {
    lcd.write(byte(0));
  } else if (humidity > 40) {
    lcd.write(byte(1));
  }
  
  message("TEMP ", 6, 1, false);

  if (temperature > 5) {
    lcd.write(byte(1));
  } else if (temperature == 0) {
    lcd.write(byte(0));
  }
  
  delay(2000);
}

void canal() {
  while (Serial.available()) {
    char inChar = Serial.read();
    Serial.println(inChar);
    
    if (inChar == 'A') {
      Serial.println("H-" + String(humidity));
      Serial.println("T-" + String(temperature));
      Serial.println("D-" + String(door));
    }
  }
}
 
void setup(){
  lcd.createChar(0, downArrow);
  lcd.createChar(1, upArrow);
  lcd.begin(16, 2);
  lcd.print("Loading");
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(HCSR04_TRIGPIN, OUTPUT);
  pinMode(HCSR04_ECHOPIN, INPUT);

  start = millis();
  
  Serial.begin(9600); // Computer
}

void loop() {
  canal();
  if(doorIsOpen()) {
    int alert = DHT.read11(DHT11_PIN);
    humidity = DHT.humidity;
    temperature = DHT.temperature;
    
    if (millis() - start > 5000) {
      tone(BUZZER_PIN, 300);
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      noTone(BUZZER_PIN);
      digitalWrite(LED_PIN, LOW);
      delay(500);

      door = 2;
      message("Door open !", 0, 0, true);
      delay(100);
      alertHumidityTemperature();
    } else {
      door = 1;
      displayHumidityTemperature();
    }
  } else {
    start = millis();
    door = 0;
  }
 
  canal();
  int chk = DHT.read11(DHT11_PIN);
  humidity = DHT.humidity;
  temperature = DHT.temperature;
  displayHumidityTemperature();
}
