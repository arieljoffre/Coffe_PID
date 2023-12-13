// ------------ Def Variables Temperatura y PWM ------------
#define Termistor A0
#define SSR 9
#define POT A2
#define LCD 5

float Vo;
float R1 = 100000;
float logR2, R2, T, x, val;
int porc;
// Constantes TERMISTOR NTC 100K 3950
#define A   0.5308572593E-3
#define B   2.396039800E-4
#define C   0.4234340345E-7

unsigned int millis_before, millis_before_2;    //Actualizacion loop
unsigned int millis_now       = 0;

// ------------ Def de Const. y Variables de Control ------------

float Ts                      = 1000;           //Periodo en ms
#define euler 2.71828
#define UMAX            5
#define UMIN            0
#define tau_LC          33.95                            // Tau      
int q = pow(euler, -Ts / tau_LC);
float set_point;
float e[2]; //control error   e[0]=e(k) e[1]=e(k-1)
float u[2]; //control output  u[0]=u(k) u[1]=u(k-1)
float y[2]; //temperature     y[0]=y(k) y[1]=y(k-1)                                                   

// ------------ Def de Parametros de la PLANTA Dahlin ------------
const float P               = 1.971;                                 
const float Q               = 0.971;                               

// ------------ Def para LCD ------------

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

// ------------------------ SETUP ------------------------
void setup() {
  pinMode(LCD, OUTPUT);
  digitalWrite(LCD, HIGH);
  pinMode(SSR, OUTPUT);
  pinMode(Termistor, INPUT);
  pinMode(POT, INPUT);
  
  lcd.init();  // initialize the lcd
  lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Coffe Hot Contrl");
    lcd.setCursor(0, 1);
    lcd.print("Control 1");
    delay(5000);
    
    Serial.begin(9600);

  // Iniializar var control
  u[0] = 0.0;
  y[0] = 0.0;

}

void loop() {
  millis_now = millis();
  if (millis_now - millis_before_2 > Ts) {  //Refresh rate of the controller
    millis_before_2 = millis();
// ------------ Medicion de Temperatura con NTC -------------

  Vo = analogRead(Termistor)*(5.0 / 1023.0);
  R2 = R1 * ((5.0/Vo) - 1.0);
  logR2 = log(R2);
  T = (1.0 / (A + B*logR2 + C*logR2*logR2*logR2));
  T = T - 273.15;

// ------------ Set Point ------------
  x = analogRead(POT);
  set_point = map(x,0,1023,0,130);
  
// ------------ Cálculos Control ------------

  y[0] = T;
  
  e[0] = set_point - y[0];                        // Cálculo del error
  u[0] = (1 - q) * P * (e[0] - Q * e[1]) + u[1];  // Algoritmo de Control

  //Control Signal Saturation
      if (u[0] > UMAX) {
        u[0] = UMAX;
      }
      else if (u[0] < UMIN) {
        u[0] = UMIN;
      }

  //Enviar señal de control
  val = map(u[0], UMIN, UMAX, 0, 255);
  analogWrite(SSR, val);

  //Save actual data as previous data
      u[1] = u[0];
      e[1] = e[0];
      y[1] = y[0];
  } 
  
millis_now = millis();
  if (millis_now - millis_before > Ts) {        //Refresh rate of printiong on the LCD
    millis_before = millis();
// ------------ Impresion Serial ------------
  

  Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.print(" c");
  Serial.print("\tPWM: "); 
  Serial.print(val);
  Serial.print("\tTiempo: ");
  Serial.print(millis()/1000.);  // Envía el tiempo en milisegundos
  Serial.println(" s");

// ------------ Variables en LCD -----------
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.setCursor(8, 0);
  lcd.print("SP:");
  lcd.setCursor(0, 1);
  lcd.print("PWM:");
  
  lcd.setCursor(2, 0);
  lcd.print(T,0);
  lcd.setCursor(11, 0);
  lcd.print(set_point,1);
  lcd.print("C");
  lcd.setCursor(5, 1);
  lcd.print(val,0);
  }  
}
