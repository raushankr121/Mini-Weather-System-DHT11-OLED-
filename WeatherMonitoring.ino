#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// ---------------- OLED Settings ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- DHT11 Settings ----------------
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- Cloud Animation ----------------
int cloudX = 128; // start from right side

void drawCloud(int x, int y) {
  // Simple cloud using circles + rectangle
  display.fillCircle(x, y, 6, SSD1306_WHITE);
  display.fillCircle(x + 8, y - 4, 7, SSD1306_WHITE);
  display.fillCircle(x + 16, y, 6, SSD1306_WHITE);
  display.fillRect(x, y, 17, 8, SSD1306_WHITE);
}

void drawSun(int x, int y) {
  // Sun circle
  display.fillCircle(x, y, 10, SSD1306_WHITE);

  // Sun rays
  display.drawLine(x, y - 15, x, y - 22, SSD1306_WHITE);
  display.drawLine(x, y + 15, x, y + 22, SSD1306_WHITE);
  display.drawLine(x - 15, y, x - 22, y, SSD1306_WHITE);
  display.drawLine(x + 15, y, x + 22, y, SSD1306_WHITE);

  display.drawLine(x - 12, y - 12, x - 18, y - 18, SSD1306_WHITE);
  display.drawLine(x + 12, y - 12, x + 18, y - 18, SSD1306_WHITE);
  display.drawLine(x - 12, y + 12, x - 18, y + 18, SSD1306_WHITE);
  display.drawLine(x + 12, y + 12, x + 18, y + 18, SSD1306_WHITE);
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(20, 25);
  display.println("Weather Monitor");
  display.display();
  delay(2000);
}

void loop() {
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(); // Celsius

  // if sensor fails
  if (isnan(hum) || isnan(temp)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.println("DHT11 Read Error!");
    display.display();
    delay(1000);
    return;
  }

  // ---------------- Display Update ----------------
  display.clearDisplay();

  // Show Sun if temp > 40
  if (temp > 40) {
    drawSun(25, 20); // sun at top-left
  }

  // Cloud animation
  drawCloud(cloudX, 20);

  cloudX -= 2; // speed of cloud
  if (cloudX < -30) {
    cloudX = 128;
  }

  // Temperature + Humidity Text
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print("Temp: ");
  display.print(temp);
  display.print(" C");

  display.setCursor(0, 55);
  display.print("Hum : ");
  display.print(hum);
  display.print(" %");

  display.display();

  // Serial output
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C | Hum: ");
  Serial.print(hum);
  Serial.println(" %");

  delay(150);
}