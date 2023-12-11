#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int ThermistorPin = A0;
const int outputPin = 9; // Pin PWM de salida
const int Pot = A1;

const double R1 = 10000;
const double c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
const double voltageConversion = 5.0 / 1023.0; // Convertir lectura analógica a voltaje
const double temperatureConversion = 100.0; // Convertir voltaje a temperatura (10mV = 1°C)

// Parámetros PID
double Kp = 0.0185; // Ganancia proporcional
double Ki = 6.61e-05; // Ganancia integral
double Kd = 0.00231; // Ganancia derivativa

// Variables PID
double input, output, setpoint;

// Variables para debouncing
double lastError = 0; // Variable para término derivativo
unsigned long lastPrintTime = 0; // Tiempo de última impresión
const unsigned long printInterval = 500; // Intervalo entre impresiones (500 ms)

// Función para calcular el término derivativo del PID
double derivativeWithTs(double error, double lastError, double ts) {
  return (error - lastError) / ts;
}

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(outputPin, OUTPUT);
  Serial.begin(9600);

  lcd.init();  // inicializar el lcd
  lcd.backlight();
/*
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Coffe Hot Contrl");
  lcd.setCursor(0, 1);
  lcd.print("Elec. Potencia");
  delay(5000);
*/
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(0, 1);
  lcd.print("Tiempo:");
}

void loop() {
  if (millis() - lastPrintTime > printInterval) {
    // Lectura del potenciómetro para el setpoint
    setpoint = analogRead(Pot) * voltageConversion * temperatureConversion / 5;

    // Lectura de la temperatura del sensor
    int Vo = analogRead(ThermistorPin);
    double R2 = R1 * (1023.0 / Vo - 1.0);
    double logR2 = log(R2);
    double T = 1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2);
    T = T - 273.15;
    input = T;

    // Cálculo manual del control PID
    // Error
    double error = setpoint - input;

    // Término proporcional
    double pTerm = Kp * error;

    // Término integral
    static double integral = 0; // Variable estática para mantener el valor de la integral
    integral += error * printInterval;
    // Limitar la integral para evitar overflow/underflow
    integral = constrain(integral, -1000, 1000);
    double iTerm = Ki * integral;

    // Cálculo del término derivativo
    double derivative = derivativeWithTs(error, lastError, 0.25); // Cambiar el valor de ts a 0.25 segundos
    lastError = error;

    // Salida del PID
    output = pTerm + iTerm + derivative;

    // Limitar la salida del PID para el PWM
    output = constrain(output, 0, 255);

    // Verificar si ha pasado suficiente tiempo desde la última impresión
    if (millis() - lastPrintTime > printInterval) {
      // Imprimir valores de entrada y salida del PID
      Serial.print("Temp: ");
      Serial.println(T);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp:");
      lcd.setCursor(6, 0);
      lcd.print(input, 1);
      lcd.print("C");

      lcd.setCursor(0, 1);
      lcd.print("PWM:");
      lcd.setCursor(8, 1);
      lcd.print(output);

      lastPrintTime = millis();
    }

    // Aplicación del PWM para controlar la potencia del elemento calefactor
    analogWrite(outputPin, output);
  }
}
