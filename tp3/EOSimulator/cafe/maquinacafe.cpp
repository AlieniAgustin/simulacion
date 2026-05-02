#include "maquinacafe.hpp"
#include <eosim/core/entity.hpp>
#include "constantes.hpp"

using namespace eosim::core;
using namespace eosim::dist;

MaquinaCafe::MaquinaCafe(unsigned int c, double a, double mediaPrep, double desvPrep) : 
    maquina(c, c), 
    arribos(MT19937, a), 
    preparacion(MT19937, mediaPrep, desvPrep),
    llegC(*this),
    salC(*this),
    iniPrep(*this),
    lCola("Largos Medios de Colas", *this),
    tEspera("Tiempos de Espera"),
    usoMaquina("Maquina Ocupada", *this)
{
}

MaquinaCafe::~MaquinaCafe() {
}

void MaquinaCafe::init() {
	registerBEvent(&llegC);
	registerBEvent(&salC);
	registerCEvent(&iniPrep); // El framework ahora sabe que debe vigilar este evento
	
	registerDist(&arribos);
	registerDist(&preparacion);
}

void MaquinaCafe::doInitialSchedules() {
	schedule(0.0, new Entity(), llegadaC);
}