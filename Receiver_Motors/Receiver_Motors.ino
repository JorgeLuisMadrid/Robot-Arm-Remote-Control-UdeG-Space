//Luis Fernando Arellano Zaragoza 27/05/22
//Modificado por: Jorge Luis Madrid Gómez 31/05/22
//UdeG Space
#include <Servo.h>
byte data[6];

//Base
int in1_Base = 2;    //Pin que controla el sentido
int in2_Base = 3;    //Pin que controla el sentido
//Hombro
int PUL=4;           //Pin para la señal de pulso
int DIR=5;           //define Direction pin
int EN=6;            //define Enable Pin
//Codo
int in1_Codo = 7;    //Pin que controla el sentido
int in2_Codo = 8;    //Pin que controla el sentido
//Muñeca
int in3_Muneca = 9;  //Pin que controla el sentido
int in4_Muneca = 10;  //Pin que controla el sentido 12 en el uno, 10 en el nano
//Mano
Servo Servo_Mano;     //Servomotor Mano
//Pinza
Servo Servo_Pinza;     //Servomotor Pinza

int it5=90;         //Posición inicial Mano
int it6=30;         //Posición inicial Pinza (abierta)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);       //Declaración de comunicación serial
  pinMode(in1_Base,OUTPUT);
  pinMode(in2_Base,OUTPUT);
  pinMode(in1_Codo,OUTPUT);
  pinMode(in2_Codo,OUTPUT);
  pinMode(in3_Muneca,OUTPUT);
  pinMode(in4_Muneca,OUTPUT);
  pinMode(EN,OUTPUT);
  digitalWrite(EN,HIGH);
  pinMode(DIR,OUTPUT);
  pinMode(PUL,OUTPUT);
  //Mano
  Servo_Mano.attach(11);
  //Pinza
  Servo_Pinza.attach(12);  //10 en el uno, 12 en el nano
}

//49 (ASCII) = 1
//48 (ASCII) = 0
//50 (ASCII) = 2

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){             //Lee la comunicación serial
    Serial.readBytes(data,7);           //en bytes
    //Motor Base
    if(data[0]==49){                    //Boton Izquierda
      digitalWrite(in1_Base, LOW);
      digitalWrite(in2_Base, HIGH);
    }
    else if(data[0]==48){               //Boton Derecha
      digitalWrite(in1_Base, HIGH);
      digitalWrite(in2_Base, LOW);
    }
    else{
      digitalWrite(in1_Base, LOW);
      digitalWrite(in2_Base, LOW);
    }
    //Motor Hombro
    if(data[1]==49){                    //Boton Arriba 
      digitalWrite(DIR,HIGH);
      for (int i=0; i<80; i++)   
      {
        digitalWrite(PUL,HIGH);
        delayMicroseconds(600);
        digitalWrite(PUL,LOW);
        delayMicroseconds(600);
      }
    }
    if(data[1]==48){               //Boton Abajo
      digitalWrite(DIR,LOW);
      for (int i=0; i<80; i++) 
      {
        digitalWrite(PUL,HIGH);
        delayMicroseconds(600);
        digitalWrite(PUL,LOW);
        delayMicroseconds(600);
      }  
    } 
    //Motor codo
    if(data[2]==49){               //Boton Arriba
      digitalWrite(in1_Codo, LOW);
      digitalWrite(in2_Codo, HIGH);       
    }
    else if(data[2]==48){               //Boton Abajo
      digitalWrite(in1_Codo, HIGH);
      digitalWrite(in2_Codo, LOW);          
    }
    else{
      digitalWrite(in1_Codo, LOW);
      digitalWrite(in2_Codo, LOW); 
    }
    //Motor Muñeca
    if(data[3]==49){               //Boton Arriba 
      digitalWrite(in3_Muneca, LOW);
      digitalWrite(in4_Muneca, HIGH);        
    }
    else if(data[3]==48){               //Boton Abajo
      digitalWrite(in3_Muneca, HIGH);
      digitalWrite(in4_Muneca, LOW);        
    }
    else{
      digitalWrite(in3_Muneca, LOW);
      digitalWrite(in4_Muneca, LOW);  
    }
  }
  //Motor Mano
  if(data[4]==49){               //Boton izquierda
    it5 = it5 - 4;
    //delay(50);
    if(it5<0)
      it5 = 0;
  }
  else if(data[4]==48){           //Boton derecha
    it5 = it5 + 4;
    //delay(50);
    if(it5>180)
      it5 = 180;
  }
  Servo_Mano.write(it5);
  //Motor Pinza
  if(data[5]==49){               //Boton Abrir 
    it6 = it6 - 4;
    delay(50);
    if(it6<30)
      it6 = 30;       
  }
  else if(data[5]==48){               //Boton Cerrar
    it6 = it6 + 4;
    delay(50);
    if(it6>100)
      it6 = 100;  
  }
  Servo_Pinza.write(it6); 
}
