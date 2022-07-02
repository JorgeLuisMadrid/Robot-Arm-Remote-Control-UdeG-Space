//Jorge Luis Madrid Gómez 05/06/22
//UdeG Space
// Librerías necesarias
#include <ros.h>
#include <Arduino.h>
#include <sensor_msgs/Joy.h>
#include <Servo.h>

ros::NodeHandle nh;

//Pines para la Base (Motor DC)
int EN_BaseR = 2;     //Pin Enable 1 (PWM)
int EN_BaseL = 3;     //Pin Enable 2 (PWM)
int in1_Base = 22;    //Pin que controla el sentido
int in2_Base = 23;    //Pin que controla el sentido
//Pines para el Hombro (Motor a pasos)
int PUL = 24;           //Pin para la señal de pulso
int DIR = 25;           //define Direction pin
int EN = 26;            //define Enable Pin
//Pines para el Codo (Motor DC)
int EN_CodoL = 4;     //Pin Enable 1 (PWM)
int EN_CodoR = 5;     //Pin Enable 2 (PWM)
int in1_Codo = 27;    //Pin que controla el sentido
int in2_Codo = 28;    //Pin que controla el sentido
//Pines para la Muñeca (Motor DC)
int EN_Muneca = 6;    //Pin Enable (PWM)
int in3_Muneca = 29;  //Pin que controla el sentido
int in4_Muneca = 30;  //Pin que controla el sentido
//Mano
Servo Servo_Mano;     //Servomotor Mano
//Pinza
Servo Servo_Pinza;    //Servomotor Pinza
//Posiciones iniciales para los Servos
int it5=90;         //Posición inicial Mano
int it6=30;         //Posición inicial Pinza (abierta)
//Condiciones iniciales para el control PWM
int motorVelSelect = 1; //Variable para la selección del motor a variar
float motorVel = 255;   //Valor de velocidad del motor seleccionado (0 - 255)

void joyControls( const sensor_msgs::Joy &msg){
  // Selección del motor a variar su PWM (Solo para los motores DC; Base, codo y mano)
  if(msg.buttons[2]==1 && msg.buttons[3]==0 && msg.buttons[5]==0){      //Motor Base
    motorVelSelect = 1;
  }
  else if(msg.buttons[2]==0 && msg.buttons[3]==1 && msg.buttons[5]==0){ //Motor Codo
    motorVelSelect = 2;
  }
  else if(msg.buttons[2]==0 && msg.buttons[3]==0 && msg.buttons[5]==1){ //Motor Muñeca
    motorVelSelect = 3;
  }
  //Mapeo del valor del eje PWM del control (-1 a 1) a valor PWM (0 a 255)
  motorVel = map(msg.axes[3],-1,1,0,255);
  //Asignación de velocidad al motor seleccionado
  switch(motorVelSelect){
    case 1: //Motor Base
      analogWrite(EN_BaseR,motorVel);
      analogWrite(EN_BaseL,motorVel);
      break;
    case 2: //Motor Codo
      analogWrite(EN_CodoR,motorVel);
      analogWrite(EN_CodoL,motorVel);
      break;
    case 3: //Motor Muñeca
      analogWrite(EN_Muneca,motorVel);
      break;
    default:
      analogWrite(EN_BaseL,255);
      analogWrite(EN_BaseR,255);
      analogWrite(EN_CodoL,255);
      analogWrite(EN_CodoR,255);
      analogWrite(EN_Muneca,255);
      break;
  }
  //Control del giro de los motores
  
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
    digitalWrite(DIR,LOW);
    for (int i=0; i<80; i++)   
    {
      digitalWrite(PUL,HIGH);
      delayMicroseconds(600);
      digitalWrite(PUL,LOW);
      delayMicroseconds(600);
    }
  }
  else if(msg.axes[1]>=0.5){      //Movimiento hacia arriba
    digitalWrite(DIR,HIGH);
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
    it5 = it5 - 4;    // La posición decrementa 4 grados
    //delay(50);
    if(it5<0)         // Codición para saturar la posición mínima
      it5 = 0;
  }
  else if(msg.axes[4]<=-0.9){           //Boton derecha
    it5 = it5 + 4;  // La posición incrementa 4 grados
    //delay(50);
    if(it5>180)     // Codición para saturar la posición máxima
      it5 = 180;
  }
  Servo_Mano.write(it5);
  //Motor Pinza
  if(msg.buttons[0]==0 && msg.buttons[1]==1){      //Abrir
    it6 = it6 - 4;  // La posición decrementa 4 grados
    delay(50);
    if(it6<30)      // Codición para saturar la posición mínima
      it6 = 30;       
  }
  else if(msg.buttons[1]==0 && msg.buttons[0]==1){ //Cerrar
    it6 = it6 + 4;  // La posición incrementa 4 grados
    delay(50);
    if(it6>100)    // Codición para saturar la posición máxima
      it6 = 100;  
  }
  Servo_Pinza.write(it6); 
}

ros::Subscriber<sensor_msgs::Joy> sub("joy", &joyControls );

// Función para configurar los pines
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
