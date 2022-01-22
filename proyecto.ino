#include "GyverWDT.h"
float temp;
void setup() {
Serial.begin(9600);//Comunicación serial
//Asignación de puertos como entrada y salida
DDRB |= ((1 << DDB5));
DDRB |= ((1 << DDB4));
DDRC&= ~(1<<DDC1); 
DDRC&= ~(1<<DDC3);
DDRD |= ((1 << DD7));
DDRD |= ((1 << DD6));
Watchdog.enable(INTERRUPT_MODE, WDT_PRESCALER_128); //Activación del watchdog

}

void loop() {
int luz=analogRead(A2);//Lectura del puerto A2
int Luzs=map(luz,0,1023,0,5000);

if(Luzs<=1000){//Condicional de las luces
  PORTB = ((0 << PB4));//Puerto PB4 sin salida de voltaje
  
}
else{
  PORTB = ((1 << PB4));
  Serial.println("Iluminación baja,activando luz");//Puerto PB4 con salida de voltaje

}

delay(500);
}

ISR(WATCHDOG) {
int y=analogRead(A0);//Lectura del puerto A0
int u=map(y,0,1023,0,255);
temp= analogRead(A1);//Lectura del puerto A1
temp = (5.0 * temp * 100.0)/1024.0; //Conversión del voltaje a celsius
if(temp>34.2){
Serial.println("Temperatura alta(Riesgo de incendio),activando ventiladores");//Imprimimos por el serial
PORTD = ((0 << PD7)); //asignar 0 al puerto D7
PORTD = ((1 << PD6));  //asignar 1 al puerto D6
}
else{
PORTD = ((0 << PD7)); //asignar 0 al puerto D7
PORTD = ((0 << PD6));   //asignar 0 al puerto D6
}
if (not(bit_is_clear(PINC, PC3))){ //Condición de que si el puerto recive voltaje del sensor de gas
  Serial.println("Presencia de gas,activando ventiladores");//Imprimimos por el serial
}
/*if (not(bit_is_clear(PINC, PC3))){
  PORTD = ((0 << PD7)); //asignar 0 al puertO D7 
PORTD = ((1 << PD6)); //asignar 1 al puerto D6 

}
else{
  PORTD = ((0 << PD7)); //asignar 0 al puerto D7 
PORTD = ((0 << PD6)); //asignar 0 al puerto D6
}*/
if(u>200){
PORTB = ((1 << PB5));//asignar 1 al puerto B5
Serial.println("Humedad baja,activando Bomba");//Imprimimos por el serial
}
if(u<200){
PORTB = ((0 << PB5)); //asignar 0 al puerto B5
}
}
