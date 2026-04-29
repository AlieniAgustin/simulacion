en eosim 1.1.1: Para compilar el ejemplo del hospital desde la terminal, puedes usar g++ directamente, indicándole dónde están los archivos de cabecera (include/), la librería estática compilada (lib/) y los archivos fuente del ejemplo. Desde la raíz del proyecto, ejecuta:

g++ -I./include -L./lib Demos/hospital2/main.cpp Demos/hospital2/hospitalsimple.cpp Demos/hospital2/paciente.cpp -leosim1_1_1 -no-pie -o hospital_demo

Una vez que el proceso termine, puedes ejecutar el binario generado:

./hospital_demo

