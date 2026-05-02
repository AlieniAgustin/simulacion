#include <eosim/core/experiment.hpp>
#include "maquinacafe.hpp"
#include "constantes.hpp"
#include <iostream>

const unsigned int repeticiones = 4;

int main () {
    std::string s;
    using namespace eosim::core;

    for (int i = 0; i < repeticiones; i++) {
        MaquinaCafe m(cantMaquinas, mediaArribos, mediaPrep, desvPrep);
        Experiment e;
        
        std::cout << "Arranco ...\n";
        m.connectToExp(&e);
        e.setSeed((unsigned long) i + 129);
        e.run(10000.0);
        std::cout << "Termine ...\n\n\n";
        
        m.usoMaquina.print(10);
        std::cout << '\n';
        m.lCola.print(10);
        std::cout << '\n';
        m.tEspera.print(52);
        
        std::cin >> s;
    }
    return 0;
}