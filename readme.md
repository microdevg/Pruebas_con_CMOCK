# Práctica: Uso de Ceedling para Testing de Librería Modbus RTU

## Descripción

Este repositorio contiene el material y el código necesarios para la práctica de la clase de Desarrollo y Testing de Software para Sistemas Embebidos. El objetivo es que los estudiantes aprendan a realizar pruebas unitarias para una librería llamada modbus_rtu, que implementa el protocolo de comunicación Modbus RTU. Usaremos Ceedling, junto con Unity y CMock, para configurar y ejecutar las pruebas necesarias para validar la librería.


## Estructura del Proyecto
- src/: Contiene los archivos de la librería Modbus RTU, con las funciones a testear.
- esp_base.h: Archivo de encabezado que define estructuras y constantes utilizadas por  modbus_rtu. No tiene archivo .c, por lo que no requiere CMock.
- esp_uart.h: Archivo de encabezado para funciones UART. Usamos CMock para simular su comportamiento, ya que esp_uart.c no está disponible.
- test/: Carpeta donde se ubican los archivos de prueba utilizados por Ceedling.



## Herramientas Utilizadas
- Ceedling: Framework de testing para C, utilizado para organizar y ejecutar pruebas unitarias.
- Unity: Biblioteca de aserciones para validar los resultados de pruebas.
- CMock: Herramienta para generar mocks y simular dependencias que no están implementadas.



## Prerrequisitos

Antes de comenzar, asegúrate de tener Docker instalado en tu sistema. Puedes descargarlo desde [aquí](https://www.docker.com/get-started).


## Ejecución de Pruebas

Para ejecutar las pruebas, sigue estos pasos:

1. Clona el repositorio y accede al directorio del proyecto:

```bash
    git clone <URL-del-repositorio>
    cd <nombre-del-repositorio>
```

2. Iniciar el contenedor de Docker y correr las pruebas:
```bash
   docker run -it --rm \
    --user root \
    -v ${PWD}:/home/dev/project \
    throwtheswitch/madsciencelab bash

```
3.  Dentro del contenedor, ejecuta todas las pruebas con:

```bash 
    apt update; apt install gcovr
    ceedling test:all
    ceedling gcov:all
```

### Documentacion
La documentacion de ``CMock``: [link](https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md)