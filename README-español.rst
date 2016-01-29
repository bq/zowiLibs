
========================================
**Zowi**
========================

![Zowi](https://it-bqcom15-media.s3.amazonaws.com/prod/images/200_200/1/a/9/a/1a9ae51edfd369f598cab125e75906bb2813e4a2.jpg)
![enter image description here](http://zowi.bq.com/wp-content/themes/bq-zowi-wp/assets/images/01_inicio_RESPONSIVE_v2_1600x520.jpg)

((DESCRIPCIÓN GENERAL PARA EL README GENERAL))
Zowi nace como juguete, pero tras jugar con él se convierte en una plataforma robótica educativa con un largo recorrido didáctico. 

Zowi empieza enamorando y divirtiendo, pero a su vez crea curiosidad sobre el funcionamiento de los robots y también de los productos tecnológicos en general. Toca, entre sus proyectos y juegos, conceptos como el hardware, el software y el diseño 3D, pilares fundamentales de estos todo producto tecnológico.


Para resumir sus cualidades técnicas, Zowi es un robot bípedo capaz de bailar, medir distancias, detectar ruido, emitir sonidos, poner bocas mediante un panel de LED. Zowi tiene una placa cerebro que es reprogramable que se alimenta de una batería inalámbrica. Por último, Zowi puede comunicarse y controlarse vía Bluetooth desde una aplicación android (la que llamamos ZowiApp)


https://youtu.be/YlPRHd19NHI



**Programa básico**
===============

El programa básico de Zowi está escrito en **código arduino**.

El programa tiene el identificador *ZOWI_BASE_vXX* (programID) donde XX es la versión.

Variables necesarias en EEPROM:
--------------------------------

 - Calibración de los servos:
        > EEPROM[0] = TRIM_YL
        EEPROM[1] = TRIM_YR
        EEPROM[2] = TRIM_RL
        EEPROM[3] = TRIM_RR



 - Nombre de Zowi:
    A partir de la posición 5 de la EEPROM
    > EEPROM[5] = '#'

**Instalación y librerías**
======================

*ZOWI_BASE*  depende de las siguientes librerías arduino:

 - **Zowi.h**
    > Librería con todas las funciones de movimiento, gestos, sonidos y bocas que puede ejecutar Zowi.
    
 - **Oscillator.h**
    > Para generar oscilaciones sinusoidales en los servos. Creada por Obijuan (https://github.com/Obijuan)
    
 - **ZowiSerialCommand.h** 
> Para la lectura e interpretación de los comandos recibidos por puerto serie. Es una modificación de la original "SerialCommand.h" de Steven Cogswell (http://awtfy.com)

 - **BatReader.h**
 > Para la lectura del nivel de batería.

 - **US.h** 
 > Para la lectura del sensor de ultrasonidos.

 - **LedMatrix.h** 
 > Para la programación de la matriz LED de la boca de Zowi.

 - **EnableInterrupt.h** 
 > Para habilitar las interrupciones con los botones de Zowi.
    
Para poder utilizarlas, se deben añadir al programa o instalarlas. Una opción para instalar manualmente librerías, es copiar cada carpeta de zowi-librería dentro de la carpeta general de librerías de arduino, es decir, dentro de ***Arduino\libraries***
 
En Windows, es posible que se llame *My Documents\Arduino\libraries*. Para usuarios de Mac, seguramente se llame *Documents/Arduino/libraries*. En linux, será una carpeta como *libraries* dentro del '*sketchbook*'.
    




**Requisitos**
==========
 - ``Robot Zowi``

 - ``Arduino IDE`` 
          <http://arduino.cc/en/Main/Software>
          
 - ``Zowi App`` : Aplicación Android gratuita para controlar a Zowi 

[![](http://zowi.bq.com/wp-content/themes/bq-zowi-wp/assets/images/03_DescargalaApp_RESPONSIVE.png)](%3Chttps://play.google.com/store/apps/details?id=com.bq.zowi&hl=es%3E)

<https://play.google.com/store/apps/details?id=com.bq.zowi&hl=es> 


![enter image description here](http://zowi.bq.com/wp-content/themes/bq-zowi-wp/assets/images/app-web-app-big.png)        




             
**Protocolo de comunicación con ZowiApp**
===================================

*ZOWI_BASE* utiliza un lenguaje propio (una serie de comandos) para comunicarse con *ZowiApp*. Éstos comandos le permiten a la aplicación android monitorizar el estado de Zowi y poder enviarle instrucciones.

Configuración de la comunicación Arduino - Android:
---------------------------------------------------------------

Comandos o Instrucciones Zowi:
---------------------------------------------------------------



**Funcionamiento sin App**
========================
ZOWI_BASE tiene 3 minijuegos que funcionan sin App para que puedas utilizar a Zowi y ver cómo mueve su robo-esqueleto antes incluso de instalar la App.

Los minijuegos se ejecutan pulsando los botones traseros de Zowi:
![botones_zowi](http://zowi.bq.com/wp-content/uploads/2015/11/todosBotones-300x117.png)
    > Botón **A** = Baile aleatorio.
    > Botón **B** = Reacciona y esquiva obstáculos.
    > Botón **A + B**  = Reacciona a golpecitos o palmadas.



**Licencia**
========

Zowi es un robot libre, lo que significa que tanto su diseño físico como programación están a disposición de cualquiera que quiera verlo, estudiarlo y cambiarlo para mejorarlo.

Zowi es distribuido en términos de la licencia GPL. Consulte la web http://www.gnu.org/licenses/ para más detalles.

