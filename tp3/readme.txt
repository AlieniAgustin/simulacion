en eosim 1.1.1: 

g++ -I./include -L./lib ../EOSimulator/hospital2/main.cpp ../EOSimulator/hospital2/hospitalsimple.cpp ../EOSimulator/hospital2/paciente.cpp -leosim1_1_1 -no-pie -o hospital_demo

Una vez que el proceso termine, puedes ejecutar el binario generado:

./hospital_demo

