Resumen- El proyecto propuesto se centra en mejorar la eficiencia energética de una cafetera a través de la implementación de un sistema de control de potencia mediante un relé de estado sólido con SCRs internamente y un controlador PID de temperatura.
La gestión optimizada del consumo eléctrico y la versatilidad en la aplicación del control de temperatura constituye el núcleo de esta iniciativa.
Destacando que el control de todo el sistema se realiza en una placa Arduino, ofreciendo una solución integral y eficiente para la operación de la cafetera.

Se encuentra 3 archivos explicados a continuación:
            Muestreo_Cafetera - Código utilizado para la captura de datos de la planta con termistor.
            Sim_Resp_Escalon - Código para matlab donde se discretiza y se simula la respuesta la escalon de la planta obtenida.
            UnoCafetera - Aqui se encuentra el código para aplicar el control PID via sintesis Dahlin con los parametros obtenidos al discretizar el sistema.
