% Jorge Luis Madrid Gómez 31/05/22
clc;clear;close all

%% Inicio de objeto Joystick
joy = vrjoystick(1);
c = caps(joy);

%% Limpieza de comunicaciones seriales
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end
%% Inicio de comunicación serial
P = serial('COM8');
P.BaudRate = 9600;
fopen(P);
% Dato inicial
data = ['2','2','2','2','2','2'];
aux = data;

% Mientras no se presione el botón 7 del joystick
% continua la comunicación serial
while(true)
    %Displays(joy);
    %% Articulación 1 (Motor Base)
    if axis(joy,3)<=-0.5                % Dirección Izquierda
        data(1) = '1';
    elseif axis(joy,3)>=0.5             % Dirección Derecha
        data(1) = '0'; 
    else                                % Se detiene
        data(1) = '2';
    end
    %% Articulación 2 (Motor Hombro)
    if axis(joy,2)<=-0.5                % Dirección abajo
        data(2) = '0';
    elseif axis(joy,2)>=0.5             % Dirección arriba
        data(2) = '1';
    else                                % Se detiene
        data(2) = '2';
    end
    %% Articulación 3 (Motor Codo)
    if axis(joy,1)<=-0.5                % Dirección abajo
        data(3) = '0';
    elseif axis(joy,1)>=0.5             % Dirección arriba
        data(3) = '1';
    else                                % Se detiene
        data(3) = '2';
    end
    %% Articulación 4 (Motor Muñeca)
    if pov(joy)==0                      % Dirección abajo
        data(4) = '0';
    elseif pov(joy)==180                % Dirección arriba
        data(4) = '1';
    else                                % Se detiene
        data(4) = '2';
    end
    %% Articulación 5 (Servo Mano)
    if pov(joy)==90                     % Dirección izquierda
       data(5) = '0';
    elseif pov(joy)==270                % Dirección derecha
       data(5) = '1';
    else                                % Se detiene
        data(5) = '2';
    end
    %% Pinza
    if button(joy,1)==1 && button(joy,2)==0         % Abre
        data(6) = '0';
    elseif button(joy,2)==1 &&  button(joy,1)==0    % Cierra
        data(6) = '1';
    else                                            % Se detiene
        data(6) = '2';
    end
    %% Si los datos cambian, se envia la data serial
    if data == aux
        m = 0;
    else
        data
        fwrite(P,data)
    end
    %% Si se presiona boton 7 se termina la comunicación
    % Presionar solo si el joystick está en reposo
    if button(joy,7) == 1
        data = ['2','2','2','2','2','2'];
        break;
    end
    if button(joy,8) == 1
        data = ['2','2','2','2','2','2'];
    end
    aux = data;
end
fclose(P);  % Termina el puerto serial
close(joy); % Termina el joystick

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
