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
  
  return (pulseIn(HCSR04_ECHOPIN, HIGH)*(0.17) > 130); // en millimetres
}

void message(String message, int col, int row, bool clearLcd) {
  if (clearLcd) {
    lcd.clear();
  }
  lcd.setCursor(col, row);
  lcd.print(message);
}

void displayHumidityTemperature(int humidity, int temperature) {
  message("HUM  " + String(humidity) + " % ", 0, 0, true);
  
  humidityOK = false;
  Serial.print("H");
  if (humidity < 25) {
    lcd.write(byte(0));
    lcd.print(" " + String(humidity - 25) + " %");
    Serial.print("H-");
  } else if (humidity > 40) {
    lcd.write(byte(1));
    lcd.print(" " + String(humidity - 40) + " %");
    Serial.print("H-");
  } else {
    humidityOK = true;
  }
  Serial.println(humidity);
  
  message("TEMP " + String(temperature) + (char)223 + "C ", 0, 1, false);

  temperatureOK = false;
  Serial.print("T");
  if (temperature > 5) {
    lcd.write(byte(1));
    lcd.print(" " + String(temperature - 5) + (char)223 + "C");
    Serial.print("H-");
  } else if (temperature == 0) {
    lcd.write(byte(0));
    Serial.print("L-");
  } else {
    temperatureOK = true;
  }
  Serial.println(temperature);

  if (humidityOK && temperatureOK) {
      digitalWrite(LED_PIN, LOW);
  } else {
      digitalWrite(LED_PIN, HIGH);
  }
  
  delay(2000);
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
  
  Serial.begin(9600);
}

void loop() {
  start = millis();
  while(doorIsOpen()) {
    if (millis() - start > 5000) {
      tone(BUZZER_PIN, 300);
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      noTone(BUZZER_PIN);
      digitalWrite(LED_PIN, LOW);
      delay(500);
    }
    message("Door open !", 0, 0, true);
    Serial.println("D-1");
    delay(100);
  }
  
  int chk = DHT.read11(DHT11_PIN);
  displayHumidityTemperature(int(DHT.humidity), int(DHT.temperature));
}
