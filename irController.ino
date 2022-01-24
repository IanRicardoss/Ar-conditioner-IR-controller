/*
 * IRremote: IRsendRawDemo - demonstrates sending IR codes with sendRaw
 * An IR LED must be connected to Arduino PWM pin 9.
 * Initially coded 2009 Ken Shirriff http://www.righto.com
 *
 * IRsendRawDemo - added by AnalysIR (via www.AnalysIR.com), 24 August 2015
 *
 * This example shows how to send a RAW signal using the IRremote library.
 * The example signal is actually a 32 bit NEC signal.
 * Remote Control button: LGTV Power On/Off.
 * Hex Value: 0x20DF10EF, 32 bits
 *
 * It is more efficient to use the sendNEC function to send NEC signals.
 * Use of sendRaw here, serves only as an example of using the function.
 *
 */

#include <Wire.h>
#include "IRremote.h"
#include <RTClib.h>
#include <AM2320.h> 

AM2320 th(&Wire);
RTC_DS1307 rtc;
IRsend irsend;
int khz = 38; // 38kHz carrier frequency for the NEC protocol - antes estava no loop


const unsigned int irTemp20[] = { 9044, 4488, 572, 1680, 572, 1680, 572, 544, 568, 544, 568, 544, 572, 540, 572, 1680, 572, 1680, 
    572, 1680, 572, 1680, 572, 1680, 572, 540, 572, 540, 572, 1680, 572, 1680, 548, 568, 544, 564, 572, 540, 576, 536, 576, 540, 572, 
    564, 548, 1680, 572, 1680, 572, 1680, 572, 564, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 572, 544, 
    568, 544, 568, 544, 568, 544, 568, 544, 1708, 544, 568, 544, 1708, 544, 572, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 
    572, 540, 572, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 1708, 548, 568, 544, 568, 544, 568, 544, 568, 544, 
    568, 544, 568, 544, 572, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 572, 544, 568, 544, 568, 544, 568, 544, 
    568, 544, 568, 544, 568, 544, 568, 548, 568, 544, 1704, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 572, 544, 
    568, 544, 568, 544, 568, 544, 1708, 544, 568, 544, 1708, 544, 572, 540, 572, 544, 568, 544, 568, 544, 568, 544, 1708, 544, 1708, 544, 
    1708, 544, 1708, 544, 568, 548, 564, 544, 572, 544, 568, 544}; // AnalysIR Batch Export (IRremote) - RAW

const unsigned int irTemp21[] = {9020, 4516, 544, 1708, 544, 1708, 544, 568, 548, 564, 548, 568, 544, 568, 544, 1684, 568, 
    1684, 568, 1684, 568, 1684, 572, 1680, 572, 1680, 572, 544, 568, 1680, 572, 1680, 548, 568, 544, 568, 544, 568, 544, 564, 548, 
    568, 572, 564, 548, 1680, 572, 1704, 548, 1704, 548, 568, 544, 568, 544, 568, 544, 568, 544, 596, 520, 592, 520, 592, 520, 592, 
    520, 592, 520, 592, 520, 596, 516, 596, 516, 596, 520, 1732, 520, 592, 520, 1708, 544, 592, 520, 592, 520, 596, 516, 596, 516, 
    596, 520, 592, 520, 592, 520, 592, 520, 592, 520, 596, 516, 596, 516, 596, 520, 592, 520, 592, 520, 1732, 520, 592, 520, 596, 
    516, 596, 520, 592, 520, 592, 520, 592, 520, 592, 520, 592, 520, 596, 516, 596, 516, 596, 520, 592, 520, 592, 520, 592, 520, 
    596, 516, 596, 516, 596, 520, 592, 520, 592, 520, 592, 520, 592, 520, 596, 516, 1736, 516, 596, 520, 592, 520, 592, 520, 592, 
    520, 592, 520, 596, 516, 596, 516, 596, 520, 592, 520, 592, 520, 1732, 520, 592, 520, 1712, 540, 592, 520, 596, 516, 596, 520, 
    592, 520, 592, 520, 1732, 520, 1708, 544, 1708, 544, 592, 520, 1708, 544, 592, 520, 596, 516, 596, 516}; // AnalysIR Batch Export (IRremote) - RAW

const unsigned int irTemp22[] = {9044, 4488, 572, 1680, 572, 1680, 572, 544, 568, 544, 568, 544, 568, 544, 568, 1680, 572, 
    1680, 572, 1680, 576, 1676, 572, 1680, 572, 544, 568, 1680, 572, 1680, 572, 1680, 572, 544, 544, 564, 548, 564, 576, 536, 576, 
    540,572, 564, 548, 1676, 576, 1680, 572, 1680, 572, 564, 548, 564, 548, 564, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 
    544, 568, 548, 564, 548, 564, 548, 568, 544, 568, 544, 1708, 544, 568, 544, 1708, 544, 568, 544, 568, 544, 568, 544, 568, 544, 
    572, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 548, 564, 548, 564, 548, 1704, 548, 568, 544, 568, 544, 
    568, 544, 568, 544, 568, 544, 568, 548, 564, 548, 564, 548, 564, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 
    568, 544, 568, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 1708, 544, 568, 544, 568, 544, 568, 548, 568, 544, 568, 544, 
    568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 1708, 548, 564, 548, 1704, 548, 564, 548, 568, 544, 568, 544, 568, 544, 568, 544, 
    1708, 544, 1708, 544, 1708, 544, 1708, 544, 1708, 544, 568, 544, 568, 548, 564, 548}; // AnalysIR Batch Export (IRremote) - RAW

const unsigned int irTemp23[] = {9040, 4492, 568, 1684, 568, 1684, 568, 568, 544, 568, 548, 564, 548, 564, 548, 1680, 572, 1680, 
    572, 1680, 572, 1680, 572, 1680, 572, 1680, 572, 1680, 572, 1680, 572, 1680, 572, 544, 544, 568, 544, 568, 544, 564, 576, 540, 572, 
    564, 548, 1680, 572, 1680, 572, 1704, 548, 564, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 572, 540, 572, 540, 596, 520, 
    592, 520, 592, 520, 592, 520, 592, 520, 1708, 544, 592, 520, 1708, 544, 592, 520, 592, 520, 596, 516, 596, 516, 596, 520, 592, 520, 
    592, 520, 592, 520, 592, 520, 592, 520, 596, 516, 596, 516, 596, 516, 596, 520, 1708, 544, 592, 520, 592, 520, 592, 520, 592, 520, 
    592, 520, 592, 520, 596, 516, 596, 516, 596, 516, 596, 520, 592, 520, 592, 520, 592, 520, 592, 520, 592, 520, 596, 516, 596, 516, 
    596, 520, 592, 520, 592, 520, 592, 520, 592, 520, 1708, 544, 592, 520, 596, 516, 596, 516, 596, 520, 592, 520, 592, 520, 592, 520, 
    592, 520, 592, 520, 596, 520, 1704, 544, 596, 516, 1712, 540, 596, 520, 592, 520, 592, 520, 592, 520, 592, 520, 1708, 544, 1708, 544, 
    1708, 544, 592, 520, 596, 516, 1708, 544, 596, 520, 592, 520}; // AnalysIR Batch Export (IRremote) - RAW

const unsigned int irTemp24[] = {9024, 4508, 544, 1708, 544, 1708, 544, 568, 544, 568, 548, 540, 572, 540, 572, 1704, 548, 1680, 
    572, 1704, 544, 1708, 548, 1704, 548, 564, 544, 548, 568, 544, 568, 564, 548, 1684, 568, 568, 544, 544, 568, 568, 544, 548, 568, 564, 
    548, 1704, 548, 1680, 572, 1704, 548, 564, 548, 540, 572, 564, 548, 564, 548, 544, 568, 568, 544, 568, 544, 568, 544, 544, 572, 540, 
    572, 540, 572, 568, 544, 564, 548, 1704, 548, 544, 568, 1684, 568, 568, 544, 540, 572, 568, 544, 544, 568, 568, 548, 564, 548, 564, 
    548, 544, 568, 544, 568, 568, 544, 568, 544, 568, 544, 568, 544, 568, 548, 1704, 548, 564, 548, 564, 548, 564, 548, 568, 544, 544, 
    568, 568, 544, 568, 544, 544, 568, 568, 544, 568, 544, 548, 568, 544, 568, 544, 568, 568, 544, 568, 544, 568, 544, 568, 544, 568, 
    548, 564, 544, 568, 548, 540, 572, 564, 548, 564, 548, 544, 568, 568, 544, 544, 568, 544, 568, 568, 544, 568, 548, 540, 572, 540, 
    568, 572, 544, 544, 568, 1704, 548, 564, 548, 1708, 544, 544, 568, 568, 544, 568, 544, 568, 544, 544, 568, 1684, 568, 1708, 544, 
    1708, 544, 1684, 568, 544, 568, 568, 544, 568, 544, 568, 548}; // AnalysIR Batch Export (IRremote) - RAW

const unsigned int irTemp25[] = { 9016, 4508, 548, 1704, 548, 1704, 548, 564, 548, 564, 548, 564, 548, 564, 544, 1684, 568, 1684, 568, 
    1680, 572, 1680, 572, 1680, 568, 1684, 568, 544, 568, 544, 568, 544, 544, 1704, 548, 564, 548, 564, 548, 564, 572, 540, 572, 564, 
    548, 1680, 572, 1700, 548, 1704, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 592, 520, 592, 520, 592, 520, 592, 520, 592, 
    520, 592, 520, 592, 520, 592, 520, 1708, 540, 596, 516, 1736, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 
    516, 596, 516, 596, 516, 596, 516, 596, 520, 592, 516, 596, 516, 596, 516, 1712, 540, 596, 516, 596, 516, 596, 516, 596, 516, 596, 
    516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 520, 592, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 
    516, 596, 516, 596, 516, 596, 516, 596, 516, 1736, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 516, 596, 
    516, 596, 516, 596, 516, 1736, 516, 596, 516, 1732, 520, 592, 520, 592, 520, 592, 520, 592, 520, 596, 516, 1732, 520, 1732, 520, 
    1708, 544, 592, 516, 1712, 540, 1712, 540, 596, 516, 596, 516 }; // AnalysIR Batch Export (IRremote) - RAW

//----------------------------------------------------------------------------------------------------------------------------------------

const unsigned int irDesliga20[] = { 9048, 4484, 572, 1680, 572, 1680, 572, 540, 572, 540, 576, 536, 576, 540, 572, 1676, 576,
    1676, 576, 1676, 576, 1676, 576, 1676, 576, 536, 576, 540, 572, 1676, 576, 1676, 576, 536, 572, 540, 572, 540, 576, 540, 572, 540,
    572, 540, 572, 1676, 576, 1676, 576, 1676, 572, 568, 548, 564, 548, 564, 548, 564, 548, 564, 548, 564, 548, 564, 548, 564, 548, 568,
    544, 568, 544, 568, 544, 568, 544, 568, 544, 1708, 544, 568, 544, 1708, 544, 568, 544, 568, 544, 568, 548, 564, 548, 564, 548, 568,
    544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 548, 1704, 548, 564, 548, 564, 548, 564, 548, 568,
    544, 568, 544, 568, 544, 568, 544, 568, 544, 572, 540, 568, 544, 568, 544, 568, 548, 564, 548, 564, 548, 568, 544, 568, 544, 568, 544,
    568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 548, 564, 548, 564, 548, 564, 552, 564, 544, 568, 544, 568, 544, 568, 544, 568,
    544, 568, 544, 568, 544, 1708, 544, 568, 544, 1708, 544, 568, 548, 564, 548, 564, 548, 564, 548, 568, 544, 1704, 548, 1704, 548, 1704,
    548, 1704, 544, 568, 548, 1704, 544, 1708, 544, 1708, 544}; // AnalysIR Batch Export (IRremote) - RAW


const unsigned int irDesliga25[] = { 9048, 4488, 572, 1680, 572, 1680, 572, 540, 572, 544, 572, 540, 572, 540, 572, 1680, 572, 1680, 572,
    1680, 572, 1680, 572, 1680, 572, 1680, 572, 540, 572, 540, 572, 540, 572, 1680, 548, 564, 572, 540, 576, 540, 572, 540, 572, 540, 572, 1680,
    572, 1680, 572, 1680, 572, 564, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 548, 568, 544, 568, 544, 568,
    544, 568, 544, 1708, 544, 568, 544, 1708, 544, 568, 544, 568, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 548, 568,
    544, 568, 544, 568, 544, 568, 544, 568, 544, 1708, 544, 568, 548, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 572,
    544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 572, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 568, 548,
    568, 544, 568, 544, 568, 544, 568, 544, 568, 544, 572, 544, 564, 548, 568, 544, 568, 544, 568, 544, 1708, 544, 568, 544, 1708, 544, 568, 544, 568,
    544, 568, 544, 568, 548, 568, 544, 1704, 548, 1704, 548, 1704, 548, 568, 544, 1708, 544, 568, 544, 568, 544, 568, 544}; // AnalysIR Batch Export (IRremote) - RAW

int irPin = 9; // pin IR led

float temperatura;
float umidade;
float temp_hor_min;
float temp_hor_max;

//-----------------------------------------------------------
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

const int Manha_inicio = 0;
const int Manha_fim = 30;

const int Noite_inicio = 31;
const int Noite_fim = 59;

bool flag_manha = false;
bool flag_noite = false;
bool flag_desligado_manha = false;
bool flag_desligado_noite = false;
bool flag_ajustar_manha = false;
bool flag_ajustar_noite = false;

int Hor;
int Min;
int Seg;

//-----------------------------------------------------------
    
void setup() {
   Serial.begin(9600);
   Wire.begin();
   rtc.begin();
 
  pinMode(irPin, OUTPUT);
   
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2020, 9, 11, 18, 46, 40));
  }
    
    
    Serial.println(F("START " __FILE__ " from " __DATE__));
    Serial.print(F("Ready to send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
}

void loop() {

    DateTime now = rtc.now();
    Hor = rtc.now().hour();
    Min = rtc.now().minute();
    Seg = rtc.now().second();

    TempUmi_leitura();

    //-------------------------------------------------------------

    Serial.print("Temperatura Ar= ");
    Serial.print(temperatura);
    Serial.println("*C");
    
    Serial.print("Umidade = ");
    Serial.print(umidade);
    Serial.println("%");

    //-------------------------------------------------------------

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    //-------------------------------------------------------------
    
    if (Min >= Manha_inicio && Seg <= Manha_fim){

        if(!flag_manha){
          Serial.println("temp min 25 e temp max 26");
          temp_hor_min = 25.0;
          temp_hor_max = 26.0;
          flag_manha = true;
          flag_noite = false;

          flag_desligado_noite = false;
          flag_ajustar_noite = false;
        }

     // dentro dos parametros
     if (temperatura >= temp_hor_min && temperatura <= temp_hor_max && flag_desligado_manha == false){
          
          flag_desligado_manha = true;
          flag_ajustar_manha = false;

          Serial.println();
          Serial.println(" Temperatura Ok - Desligando ar condicionado");
          Serial.println();
          
          for(int i=0;i<3;i++){
            Desliga_25();
          delay(1000);
          }
          
          delay(10000);
          Serial.println("Dando delay de 30 min");
        }
        
        // fora dos parametros
      if ((temperatura < temp_hor_min || temperatura > temp_hor_min) && flag_ajustar_manha == false){                                                                            
          
          flag_desligado_manha = false;
          flag_ajustar_manha = true;
          
          
          Serial.println();
          Serial.println(" Temperatura errada - Alterando temperatura para manhã");
          Serial.println();
        
          for(int i=0;i<3;i++){
            Temp_25();
           delay(1000);
          }
          delay(10000);
          Serial.println("Dando delay de 30 min");
       }
    }
     
     //-------------------------------------------------------------
     
     if (Min >= Noite_inicio && Seg <= Noite_fim){

        if(!flag_noite){
          Serial.println("temp min 20 e temp max 21");
          temp_hor_min = 20.0;
          temp_hor_max =21.0;
          
          flag_manha = false;
          flag_noite = true;

          flag_desligado_manha = false;
          flag_ajustar_manha = false;
        }

     // dentro dos parametros
     if (temperatura >= temp_hor_min && temperatura <= temp_hor_max && flag_desligado_noite == false){
          
          flag_desligado_noite = true;
          flag_ajustar_noite = false;

          Serial.println();
          Serial.println(" Temperatura Ok - Desligando ar condicionado");
          Serial.println();
          
          for(int i=0;i<3;i++){
            Desliga_20();
          delay(1000);
          }
          
          delay(10000);
          Serial.println("Dando delay de 30 min");
        }
        
        // fora dos parametros
      if ((temperatura < temp_hor_min || temperatura > temp_hor_min) && flag_ajustar_noite == false){                                                                            
          
          flag_desligado_noite = false;
          flag_ajustar_noite = true;
          
          
          Serial.println();
          Serial.println(" Temperatura errada - Alterando temperatura para noite");
          Serial.println();
        
          for(int i=0;i<3;i++){
            Temp_20();
           delay(1000);
          }
          delay(10000);
          Serial.println("Dando delay de 30 min");
       }
    }
     
     //-------------------------------------------------------------

    delay(1000);

/*
    Temp_20();
    delay(1000);
    
    Temp_21();
    delay(1000);
    
    Temp_22();
    delay(1000);
    
    Temp_23();
    delay(1000);
    
    Temp_24();
    delay(1000);
    
    Temp_25();
    delay(1000);
*/

    }
    
void Temp_20(){
    irsend.sendRaw(irTemp20, sizeof(irTemp20) / sizeof(irTemp20[0]), khz); // Note the approach used to automatically calculate the size of the array.
}

void Temp_21(){
     irsend.sendRaw(irTemp21, sizeof(irTemp21) / sizeof(irTemp21[0]), khz); // Note the approach used to automatically calculate the size of the array.
}

void Temp_22(){
    irsend.sendRaw(irTemp22, sizeof(irTemp22) / sizeof(irTemp22[0]), khz); // Note the approach used to automatically calculate the size of the array.
}

void Temp_23(){
    irsend.sendRaw(irTemp23, sizeof(irTemp23) / sizeof(irTemp23[0]), khz); // Note the approach used to automatically calculate the size of the array.
}
void Temp_24(){
    irsend.sendRaw(irTemp24, sizeof(irTemp24) / sizeof(irTemp24[0]), khz); // Note the approach used to automatically calculate the size of the array.
}

void Temp_25(){    
    irsend.sendRaw(irTemp25, sizeof(irTemp25) / sizeof(irTemp25[0]), khz); // Note the approach used to automatically calculate the size of the array.
}
void Desliga_20(){
    irsend.sendRaw(irDesliga20, sizeof(irDesliga20) / sizeof(irDesliga20[0]), khz); // Note the approach used to automatically calculate the size of the array.
}
void Desliga_25(){
    irsend.sendRaw(irDesliga25, sizeof(irDesliga25) / sizeof(irDesliga25[0]), khz); // Note the approach used to automatically calculate the size of the array.
}

void TempUmi_leitura()
{
  for(int i=0; i<=2; i++)
  {
  temperatura = th.cTemp;
  umidade = th.Humidity;

  delay(250);
  
  switch(th.Read()) 
  {
    case 2:
      Serial.println(" CRC failed");
    break;
  
  case 1:
  Serial.println(" Sensor offline");
  
  break;
  
  case 0:
  //Serial.println("Leitura Ok");
  break;
  }
  } 
}
