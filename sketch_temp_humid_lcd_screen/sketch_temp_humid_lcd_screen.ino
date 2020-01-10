#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTTYPE DHT11

LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

int DHPin = 8;
float lastHum = 0;
float lastTemp = 0;
bool isFahrenheit = true;

DHT dht(DHPin, DHTTYPE);

void setup() {
  lcd.begin(16, 2);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature(isFahrenheit);

  if(isnan(humidity) || isnan(temp))
  {
    lcd.print("Sensor Error!");
    delay(3000);
    lcd.clear();
    return;
  }

  if(lastHum != humidity || lastTemp != temp)
  {
    lcd.clear(); 
    lcd.print("H = ");
    lcd.print(humidity);
    lcd.print('%');
    lcd.setCursor(0, 1);
    lcd.print("T = ");
    lcd.print(temp);
    lcd.print(isFahrenheit ? 'F' : 'C');
  }
  lastHum = humidity;
  lastTemp = temp;
}
