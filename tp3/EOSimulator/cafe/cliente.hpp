#ifndef CLIENTE_HPP_
#define CLIENTE_HPP_

#include <eosim/core/bevent.hpp>
#include <eosim/core/cevent.hpp>
#include <eosim/core/entity.hpp>
#include <string>

const std::string llegadaC = "LlegadaCliente";
const std::string salidaC  = "SalidaCliente";

// Eventos B (Fijos)
class LlegadaCliente : public eosim::core::BEvent {
public:
	LlegadaCliente(eosim::core::Model& model);
	~LlegadaCliente();
	void eventRoutine(eosim::core::Entity* who);
};

class SalidaCliente : public eosim::core::BEvent {
public:
	SalidaCliente(eosim::core::Model& model);
	~SalidaCliente();
	void eventRoutine(eosim::core::Entity* who);
};

// Evento C (Condicionado)
class InicioPreparacion : public eosim::core::CEvent {
public:
	InicioPreparacion(eosim::core::Model& model);
	~InicioPreparacion();
	void eventRoutine(); // Nota: Los eventos C actúan sobre el modelo, no reciben un "who"
};

#endif