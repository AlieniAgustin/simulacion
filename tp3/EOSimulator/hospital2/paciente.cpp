#include "paciente.hpp"
#include "hospitalsimple.hpp"
#include <iostream>
using namespace eosim::core;


// en el constructor se utiliza el identificador definido en paciente.hpp
// en el constructor se utiliza el identificador definido en pacientefeeder.hpp
PacienteFeeder::PacienteFeeder(Model& model): BEvent(pacienteF, model) {}

PacienteFeeder::~PacienteFeeder() {}

void PacienteFeeder::eventRoutine(Entity* who) {
	// se anuncia la llegada del paciente
	std::cout << "llego un paciente en " << who->getClock() << "\n";

	// 1. HARDCODEO (Lógica de 2 fases): En vez de dejar que una Fase C lo decida,
    // el evento de llegada pregunta DIRECTAMENTE si el recurso está libre.
	HospitalSimple& h = dynamic_cast<HospitalSimple&>(owner);

	if (h.camas.isAvailable(1)) {
		h.camas.acquire(1); // Toma el token del recurso
		std::cout << "un paciente fue aceptado en una cama " << h.getSimTime() << "\n";
		h.tEspera.log(h.getSimTime() - who->getClock());
		h.usoCamas.log(h.camas.getMax() - h.camas.getQuantity());

		// Programa el evento B de salida usando el tiempo de estadía
		h.schedule(h.estadia.sample(), who, salidaP);
	}
	else {
        // se acumulan datos en los histogramas
        h.lCola.log(h.cola.size());

		// Si no hay recurso, a la cola
		h.cola.push(who);
	}

	// 2. BOOTSTRAPPING (Generación en cadena)
    // Tal como me preguntaste antes, el paciente actual "programa" la llegada del próximo
	h.schedule(h.arribos.sample(), new Entity(), pacienteF);
}

// en el constructor se utiliza el identificador definido en paciente.hpp
SalidaPaciente::SalidaPaciente(Model& model): BEvent(salidaP, model) {}

SalidaPaciente::~SalidaPaciente() {}

void SalidaPaciente::eventRoutine(Entity* who) {
	// se informa la salida de un paciente
	std::cout << "un paciente se retira en " << who->getClock() << "\n";
	// se castea owner a un HospitalSimple
	HospitalSimple& h = dynamic_cast<HospitalSimple&>(owner);

	// 1. Devuelve el recurso (Token renovable intacto)
	h.camas.returnBin(1);
	h.usoCamas.log(h.camas.getMax() - h.camas.getQuantity());

	// le damos prioridad al proceso externo si ya estaba esperando por sus 2 camas
	if (!h.colaExtra.empty() && h.camas.isAvailable(2)) {
		h.camas.acquire(2); // toma las 2 camas q necesitaba
		h.usoCamas.log(h.camas.getMax() - h.camas.getQuantity());

		Entity* entExtra = h.colaExtra.pop(); // saca al proceso extra de su cola
		h.schedule(h.tiempoUsoExtra.sample(), entExtra, devuelveExtra);
	}
	else if (!h.cola.empty() && h.camas.isAvailable(1)) {
		h.camas.acquire(1); // Vuelve a tomar el token para el paciente encolado
		std::cout << "un paciente fue aceptado en una cama " << h.getSimTime() << "\n";
        h.lCola.log(h.cola.size());
		h.usoCamas.log(h.camas.getMax() - h.camas.getQuantity());

		Entity* ent = h.cola.pop(); // Lo saca de la cola
		h.tEspera.log(h.getSimTime() - ent->getClock());

		// Programa su fin de actividad
		h.schedule(h.estadia.sample(), ent, salidaP);
	}

	// 3. Destrucción de la entidad temporal
	delete who;
}

ProcesoExternoToma::ProcesoExternoToma(Model& model): BEvent(tomaExtra, model) {}
ProcesoExternoToma::~ProcesoExternoToma() {}

void ProcesoExternoToma::eventRoutine(Entity* who) {
	HospitalSimple& h = dynamic_cast<HospitalSimple&>(owner);

	// verificamos si hay 2 camas libres
	if (h.camas.isAvailable(2)) {
		h.camas.acquire(2); // toma 2 tokens
		h.usoCamas.log(h.camas.getMax() - h.camas.getQuantity());

		// programa la devolucion en un tiempo normalmente distribuido
		h.schedule(h.tiempoUsoExtra.sample(), who, devuelveExtra);
	} else {
		// si no hay 2 camas libres, el proceso se encola y queda esperando
		h.colaExtra.push(who);
	}
}

ProcesoExternoDevuelve::ProcesoExternoDevuelve(Model& model): BEvent(devuelveExtra, model) {}
ProcesoExternoDevuelve::~ProcesoExternoDevuelve() {}

void ProcesoExternoDevuelve::eventRoutine(Entity* who) {
	HospitalSimple& h = dynamic_cast<HospitalSimple&>(owner);

	// devuelve las 2 camas al pool
	h.camas.returnBin(2);
	h.usoCamas.log(h.camas.getMax() - h.camas.getQuantity());

	// BOOTSTRAPPING: creamos una nueva entidad y la mandamos a intentar tomas las camas en el futuro
	h.schedule(10.0, new Entity(), tomaExtra);

	// HARDCODEO: como devolvimos 2 camas, capaz hay pacientes esperando => vemos si podemos admitir a 2 pacientes regulares q necesitan 1 cama c/u
	while (!h.cola.empty() && h.camas.isAvailable(1)) {
		h.camas.acquire(1);
		h.usoCamas.log(h.camas.getMax() - h.camas.getQuantity());

		Entity* ent = h.cola.pop();
		h.tEspera.log(h.getSimTime() - ent->getClock());

		h.schedule(h.estadia.sample(), ent, salidaP);
	}
	
	delete who;
}


