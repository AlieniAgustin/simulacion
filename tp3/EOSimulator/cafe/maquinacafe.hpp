#ifndef MAQUINACAFE_HPP_
#define MAQUINACAFE_HPP_

#include <eosim/core/model.hpp>
#include <eosim/core/renewable.hpp>
#include <eosim/dist/negexpdist.hpp>
#include <eosim/dist/normaldist.hpp>
#include <eosim/utils/entityqueuefifo.hpp>
#include <eosim/statics/timeweighted.hpp>
#include <eosim/statics/observation.hpp>

#include "cliente.hpp" 

class MaquinaCafe : public eosim::core::Model {
public:
	eosim::core::Renewable maquina;
	eosim::dist::NegexpDist arribos;
	eosim::dist::NormalDist preparacion;

	eosim::utils::EntityQueueFifo cola;

	eosim::statics::TimeWeighted lCola;
	eosim::statics::Observation tEspera;
	eosim::statics::TimeWeighted usoMaquina;

	MaquinaCafe(unsigned int c, double a, double mediaPrep, double desvPrep);
	~MaquinaCafe();
	
	void init();
	void doInitialSchedules();

private:
	LlegadaCliente llegC;
	SalidaCliente salC;
	InicioPreparacion iniPrep; // El evento condicionado
};

#endif