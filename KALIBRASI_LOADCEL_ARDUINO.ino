#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

// Definisi Pin untuk ESP8266
#define DOUT D6 // PIN D3 untuk ESP8266
#define CLK D7   // PIN D4 untuk ESP8266
#define LCD_SDA D2  // SDA untuk LCD I2C
#define LCD_SCL D1  // SCL untuk LCD I2C

/* DEKLARASI LCD */
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C, 16 kolom, 2 baris

/* DEKLARASI LOAD CELL */
HX711 scale(DOUT, CLK);
float calibration_factor = -400;  // Nilai default kalibrasi
float berat;  // Variabel untuk menyimpan nilai berat

void setup() {
  Serial.begin(115200);  // Memulai komunikasi serial
  
  // Inisialisasi load cell
  scale.begin(DOUT, CLK);  // Inisialisasi HX711
  scale.set_scale(calibration_factor);
  scale.tare();  // Menetapkan nilai saat ini sebagai titik nol

  // Inisialisasi LCD
  Wire.begin(LCD_SDA, LCD_SCL); // Inisialisasi pin SDA dan SCL untuk I2C
  lcd.begin();  // Memulai LCD
  lcd.backlight(); // Menyalakan lampu latar LCD

  // Menampilkan pesan awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME    ");
  lcd.setCursor(0, 1);
  lcd.print(" SMART INFUS ");
  delay(2000);  // Tunda selama 2 detik
  lcd.clear();

  Serial.println("Memulai pembacaan berat dari load cell...");
}

void loop() {
  readGram();  // Panggil fungsi untuk membaca berat
  displayOnLCD();  // Tampilkan berat di LCD
  delay(500);  // Tunda 500ms antara pembacaan
}

void readGram() {
  if (scale.is_ready()) {  // Cek apakah HX711 siap
    berat = scale.get_units();  // Mengambil berat dari load cell
    if (berat < 0) {
      berat = 0.00;  // Jika berat negatif, set ke 0
    }

    // Tampilkan nilai berat di Serial Monitor
    Serial.print("Berat: ");
    Serial.print(berat);
    Serial.println(" gram");
  } else {
    Serial.println("HX711 tidak siap. Periksa koneksi.");
  }
}

void displayOnLCD() {
  lcd.clear();  // Bersihkan layar LCD
  lcd.setCursor(0, 0);
  lcd.print("Berat: ");
  lcd.print(berat);  // Tampilkan berat pada LCD
  lcd.print(" g");
}
