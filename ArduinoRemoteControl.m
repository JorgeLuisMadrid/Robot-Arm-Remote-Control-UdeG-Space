% Jorge Luis Madrid G�mez 31/05/2022
% Estructura del c�digo y funcionamiento del HW hecha por:
% Luis Fernando Arellano Zaragoza 27/05/22
clc;clear;close all
a = arduino ();     % Inicializaci�n de arduino

joy = vrjoystick(1);    % Inicia objeto joystick
c = caps(joy);          % botones y ejes detectados

%% Pines
% Base
in1_Base = 'D2';    % Pin que controla la direcci�n
in2_Base = 'D3';    % Pin que controla la direcci�n
% Hombro
PUL = 'D4';         % Pin para la se�al de pulso
DIR = 'D5';         % define direcci�n pin
EN = 'D6';          % define enable pin
% Codo
in1_Codo = 'D7';    % Pin que controla la direcci�n
in2_Codo = 'D8';    % Pin que controla la direcci�n
% Mu�eca
in3_Muneca = 'D11';  % Pin que controla la direcci�n
in4_Muneca = 'D12'; % Pin que controla la direcci�n
% Mano
q5= servo(a,'D9');  % Servo Articulaci�n 5
q6= servo(a,'D10');  % Servo Pinza


it5 = 0.5;              % Posici�n inicial Servo Articulaci�n 5 (90 grados)
it6 = 0.17;             % Posici�n inicial Servo Pinza (30 grados)

while(true)
    Displays(joy);
    %% Articulaci�n 1 (Motor Base)
    if axis(joy,3)<=-0.5                % Direcci�n Izquierda
        writeDigitalPin(a,in1_Base,0);
        writeDigitalPin(a,in2_Base,1);
    elseif axis(joy,3)>=0.5             % Direcci�n Derecha
        writeDigitalPin(a,in1_Base,1);
        writeDigitalPin(a,in2_Base,0);
    else                                % Se detiene
        writeDigitalPin(a,in1_Base,0);
        writeDigitalPin(a,in2_Base,0);  
    end
    %% Articulaci�n 2 (Motor Hombro)
    if axis(joy,2)<=-0.5                % Direcci�n abajo
        writeDigitalPin(a,DIR,0);
        for i=0:79
            writeDigitalPin(a,PUL,1);
            pause(0.0006);
            writeDigitalPin(a,PUL,0);
            pause(0.0006);
        end
    elseif axis(joy,2)>=0.5             % Direcci�n arriba
        writeDigitalPin(a,DIR,1);
        for i=0:79
            writeDigitalPin(a,PUL,1);
            pause(0.0006);
            writeDigitalPin(a,PUL,0);
            pause(0.0006);
        end
    end
    %% Articulaci�n 3 (Motor Codo)
    if axis(joy,1)<=-0.5                % Direcci�n abajo
        writeDigitalPin(a,in1_Codo,1);
        writeDigitalPin(a,in2_Codo,0);
    elseif axis(joy,1)>=0.5             % Direcci�n arriba
        writeDigitalPin(a,in1_Codo,0);
        writeDigitalPin(a,in2_Codo,1);
    else
        writeDigitalPin(a,in1_Codo,0);
        writeDigitalPin(a,in2_Codo,0);
    end
    %% Articulaci�n 4 (Motor Mu�eca)
    if pov(joy)==0                      % Direcci�n abajo
        writeDigitalPin(a,in3_Muneca,1);
        writeDigitalPin(a,in4_Muneca,0);
    elseif pov(joy)==180                % Direcci�n arriba
        writeDigitalPin(a,in3_Muneca,0);
        writeDigitalPin(a,in4_Muneca,1);
    else                                % Se detiene
        writeDigitalPin(a,in3_Muneca,0);
        writeDigitalPin(a,in4_Muneca,0);
    end
    %% Articulaci�n 5 (Servo Mu�eca)
    if pov(joy)==90
       it5 = it5 + 0.01;
       if it5 > 135/180
           it5 = 135/180;
       end
    elseif pov(joy)==270
       it5 = it5 -0.01;
       if it5 < 45/180
           it5 = 45/180;
       end
    end
    %% Pinza
    if button(joy,1)==1 && button(joy,2)==0
        it6 = it6 + 0.01;
        if it6 > 0.55
            it6 = 0.55;
        end
    elseif button(joy,2)==1 &&  button(joy,1)==0
        it6 = it6 - 0.01;
        if it6 < 0.17
            it6 = 0.17;
        end
    end
    %% Bot�n para parar comunicaci�n
    if button(joy,7) == 1
       break;
    end
    writePosition(q5,it5);
    writePosition(q6,it6);
end

function Displays(joy)
    disp('Axis R (Joint 1) (-1 left, 1 right)');
    disp(axis(joy,3));
    disp('Axis X (Joint 3) (-1 left, 1 right)');
    disp(axis(joy,1));
    disp('Axis Y (Joint 2) (-1 front, 1 back)');
    disp(axis(joy,2));
    disp('Button 1 (Close Gripper)');
    disp(button(joy,1));
    disp('Button 2 (Open Gripper)');
    disp(button(joy,2));
    disp('POV 1 (-1: 0, 0: up, 90: right, 180: down, 270: left)');
    disp(pov(joy,1));
%     clc;
end