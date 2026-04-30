#ifndef PACIENTE_HPP_
#define PACIENTE_HPP_

#include <eosim/core/bevent.hpp>
#include <eosim/core/entity.hpp>
#include <string>

// identificador del evento fijo PacienteFeeder
const std::string pacienteF = "PacienteFeeder";

class PacienteFeeder: public eosim::core::BEvent {
public:
	// constructor
	PacienteFeeder(eosim::core::Model& model);
	// destructor
	~PacienteFeeder();
	// rutina del evento fijo
	void eventRoutine(eosim::core::Entity* who);
};

// identificador del evento fijo SalidaPaciente
const std::string salidaP = "SalidaPaciente";

class SalidaPaciente: public eosim::core::BEvent {
public:
	// constructor
	SalidaPaciente(eosim::core::Model& model);
	// destructor
	~SalidaPaciente();
	// rutina del evento fijo
	void eventRoutine(eosim::core::Entity* who);
};

const std::string tomaExtra = "ProcesoExternoToma";

class ProcesoExternoToma: public eosim::core::BEvent {
public:
	ProcesoExternoToma(eosim::core::Model& model);
	~ProcesoExternoToma();
	void eventRoutine(eosim::core::Entity* who); 
};

const std::string devuelveExtra = "ProcesoExternoDevuelve";
class ProcesoExternoDevuelve: public eosim::core::BEvent {
public:
	ProcesoExternoDevuelve(eosim::core::Model& model);
	~ProcesoExternoDevuelve();
	void eventRoutine(eosim::core::Entity* who);
};

#endif
