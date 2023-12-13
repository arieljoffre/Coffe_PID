% -------------------------------------------------------------------------
% Simulacion de la Respuesta al escalon del Sistema de 1era Orden 
% -------------------------------------------------------------------------

clc
clear
close all

% Parámetros del sistema
k = 67.88;
tao = 33.95;
theta = 2;

% Crear la función de transferencia continua
num = k;
den = [tao 1];
Gs = tf(num, den);

% Agregar un retraso al sistema y mostrar las raíces del denominador
Gs.IODelay = theta;
disp('Raiz')
display(roots(den))

% Discretizar la función de transferencia
Gs_disc = c2d(Gs, 1, 'zoh');
Gs_disc.IODelay = theta;

% Mostrar las características de los sistemas continuo y discreto
display(Gs)
display(Gs_disc)

% Crear una nueva figura para la simulación de la respuesta al escalón
figure

% Configurar opciones para la simulación de la respuesta al escalón
ampli = stepDataOptions('StepAmplitude', 1.49);

% Simular y graficar la respuesta al escalón del sistema continuo
step(Gs, 250, ampli), grid
