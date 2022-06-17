//Jorge Luis Madrid Gómez 05/06/22
//UdeG Space
#include <ros.h>
#include <sensor_msgs/Joy.h>
#include <Servo.h>

ros::NodeHandle nh;

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

void joyControls( const sensor_msgs::Joy &msg){
  //Motor Base (Articulación 1)
  if(msg.axes[2]<=-0.5){          //Movimiento hacia la izquierda
    digitalWrite(in1_Base, LOW);
    digitalWrite(in2_Base, HIGH);
  }
  else if(msg.axes[2]>=0.5){      //Movimiento hacia la derecha
    digitalWrite(in1_Base, HIGH);
    digitalWrite(in2_Base, LOW);
  }
  else{                           //Sin movimiento
    digitalWrite(in1_Base, LOW);
    digitalWrite(in2_Base, LOW);
  }
  //Motor Hombro (Articulación 2)
  if(msg.axes[1]<=-0.5){          //Movimiento hacia abajo
    digitalWrite(DIR,HIGH);
    for (int i=0; i<80; i++)   
    {
      digitalWrite(PUL,HIGH);
      delayMicroseconds(600);
      digitalWrite(PUL,LOW);
      delayMicroseconds(600);
    }
  }
  else if(msg.axes[1]>=0.5){      //Movimiento hacia arriba
    digitalWrite(DIR,LOW);
    for (int i=0; i<80; i++) 
    {
      digitalWrite(PUL,HIGH);
      delayMicroseconds(600);
      digitalWrite(PUL,LOW);
      delayMicroseconds(600);
    }  
  }
  //Motor Codo (Articulación 3)
  if(msg.axes[0]<=-0.5){          //Movimiento hacia abajo
    digitalWrite(in1_Codo, LOW);
    digitalWrite(in2_Codo, HIGH);       
  }
  else if(msg.axes[0]>=0.5){       //Movimiento hacia arriba
    digitalWrite(in1_Codo, HIGH);
    digitalWrite(in2_Codo, LOW);
  }
  else{                           //Sin movimiento
    digitalWrite(in1_Codo, LOW);
    digitalWrite(in2_Codo, LOW); 
  }
  //Motor Muñeca (Articulación 4)
  if(msg.axes[5]<=-0.5){          //Movimiento hacia abajo
    digitalWrite(in3_Muneca, LOW);
    digitalWrite(in4_Muneca, HIGH);       
  }
  else if(msg.axes[5]>=0.5){       //Movimiento hacia arriba
    digitalWrite(in3_Muneca, HIGH);
    digitalWrite(in4_Muneca, LOW);
  }
  else{                           //Sin movimiento
    digitalWrite(in3_Muneca, LOW);
    digitalWrite(in4_Muneca, LOW); 
  }
  //Motor Mano (Articulación 5)
  if(msg.axes[4]>=0.9){               //Boton izquierda
    it5 = it5 - 4;
    //delay(50);
    if(it5<0)
      it5 = 0;
  }
  else if(msg.axes[4]<=-0.9){           //Boton derecha
    it5 = it5 + 4;
    //delay(50);
    if(it5>180)
      it5 = 180;
  }
  Servo_Mano.write(it5);
  //Motor Pinza
  if(msg.buttons[0]==0 && msg.buttons[1]==1){      //Abrir
    it6 = it6 - 4;
    delay(50);
    if(it6<30)
      it6 = 30;       
  }
  else if(msg.buttons[1]==0 && msg.buttons[0]==1){ //Cerrar
    it6 = it6 + 4;
    delay(50);
    if(it6>100)
      it6 = 100;  
  }
  Servo_Pinza.write(it6); 
}

ros::Subscriber<sensor_msgs::Joy> sub("joy", &joyControls );

void setup() {
  nh.initNode();
  nh.subscribe(sub);
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

void loop() {
  nh.spinOnce();
}
