// Pines Utilizados
#define TERMISTOR_INPUT A0
#define SSR 9

// Constantes TERMISTOR NTC 100K 3950
#define A   0.5308572593E-3
#define B   2.396039800E-4
#define C   0.4234340345E-7
// --------------- VARIABLES TEMPERATURA ---------------
float V     = 0;
float temp  = 0;
float R1    = 100000;
float R2;
// VARIABLES PWM
int PWM;
float x;
// --------------- VARIABLES Ts ---------------
unsigned int millis_before;
unsigned int millis_now  = 0;
float Ts  = 1000; 

// ---------------/ SETUP /---------------
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

pinMode(TERMISTOR_INPUT, INPUT);
pinMode(SSR, OUTPUT);

analogWrite(SSR, 0);
Serial.println("Iniciando...");

// -------- PWM FIJO --------
analogWrite(SSR, 76);        //         <---------- DC
}

// ---------------/ LOOP /---------------
void loop() {
  
  millis_now = millis();
  if (millis_now - millis_before > Ts) {  //Refresh rate of the controller
    millis_before = millis();
    
// --------------- MEDICION TEMPERATURA ---------------
    V = analogRead(TERMISTOR_INPUT)*(5.0 / 1023.0);
    R2 = R1 * ((5.0 / V) - 1.0);
    temp = log(R2);
    temp = 1 / (A + (B * temp) + (C * temp * temp * temp));
    temp = temp - 273.15;       //Kelvin a grados Centigrados

    
// --------------- Impresion Valores ---------------
    Serial.print(R2);
    Serial.print("\t ");
    Serial.print(temp);
    Serial.print("\t ");
    Serial.println(millis()/1000.);
    } 
}
