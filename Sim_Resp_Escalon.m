% -------------------------------------------------------------------------
% Simulacion de la Respuesta al escalon del Sistema de 1era Orden 
% -------------------------------------------------------------------------

clc
clear
close all

% Par�metros del sistema
k = 67.88;
tao = 33.95;
theta = 2;

% Crear la funci�n de transferencia continua
num = k;
den = [tao 1];
Gs = tf(num, den);

% Agregar un retraso al sistema y mostrar las ra�ces del denominador
Gs.IODelay = theta;
disp('Raiz')
display(roots(den))

% Discretizar la funci�n de transferencia
Gs_disc = c2d(Gs, 1, 'zoh');
Gs_disc.IODelay = theta;

% Mostrar las caracter�sticas de los sistemas continuo y discreto
display(Gs)
display(Gs_disc)

% Crear una nueva figura para la simulaci�n de la respuesta al escal�n
figure

% Configurar opciones para la simulaci�n de la respuesta al escal�n
ampli = stepDataOptions('StepAmplitude', 1.49);

% Simular y graficar la respuesta al escal�n del sistema continuo
step(Gs, 250, ampli), grid
