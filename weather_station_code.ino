#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SD.h>

// ============= LCD =================
LiquidCrystal_I2C lcd(0x27, 20, 4);  // 20x4 LCD at I2C addr 0x27

// ============= DHT22 ================
#define DHTPIN 53        // DHT22 Data pin
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ============= MQ-2 ================
#define MQ2_AO A0
#define MQ2_DO 44

// ============= LDR =================
#define LDR_AO A2
#define LDR_DO 45

// ============= SD Card =============
#define SD_CS 49
File logFile;

void setup() {
  Serial.begin(9600);

  // LCD setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Weather Station");

  // Start DHT
  dht.begin();

  // Start SD
  if (!SD.begin(SD_CS)) {
    lcd.setCursor(0, 1);
    lcd.print("SD Init Fail!");
    Serial.println("SD initialization failed!");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("SD Ready");
    Serial.println("SD initialized.");
  }

  delay(2000);
}

void loop() {
  // ==== DHT22 ====
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  // ==== MQ-2 ====
  int mq2_analog = analogRead(MQ2_AO);
  int mq2_digital = digitalRead(MQ2_DO);

  // ==== LDR ====
  int ldr_analog = analogRead(LDR_AO);
  int ldr_digital = digitalRead(LDR_DO);

  // ==== LCD Output ====
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C H:");
  lcd.print(hum, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Gas A:");
  lcd.print(mq2_analog);
  lcd.setCursor(14, 1);
  lcd.print("D:");
  lcd.print(mq2_digital);

  lcd.setCursor(0, 2);
  lcd.print("Light A:");
  lcd.print(ldr_analog);
  lcd.setCursor(14, 2);
  lcd.print("D:");
  lcd.print(ldr_digital);

  lcd.setCursor(0, 3);
  lcd.print("Logging...");

  // ==== SD Logging ====
  logFile = SD.open("weather.txt", FILE_WRITE);
  if (logFile) {
    logFile.print("Temp=");
    logFile.print(temp);
    logFile.print("C, Hum=");
    logFile.print(hum);
    logFile.print("%, MQ2=");
    logFile.print(mq2_analog);
    logFile.print("/");
    logFile.print(mq2_digital);
    logFile.print(", LDR=");
    logFile.print(ldr_analog);
    logFile.print("/");
    logFile.println(ldr_digital);
    logFile.close();
  } else {
    Serial.println("Error opening weather.txt");
  }

  delay(2000); // log every 2s
}