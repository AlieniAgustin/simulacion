#include "cliente.hpp"
#include "maquinacafe.hpp"

using namespace eosim::core;

LlegadaCliente::LlegadaCliente(Model& model) : BEvent(llegadaC, model) {}
LlegadaCliente::~LlegadaCliente() {}

SalidaCliente::SalidaCliente(Model& model) : BEvent(salidaC, model) {}
SalidaCliente::~SalidaCliente() {}

InicioPreparacion::InicioPreparacion(Model& model) : CEvent(model) {}
InicioPreparacion::~InicioPreparacion() {}


void LlegadaCliente::eventRoutine(Entity* who) {
    MaquinaCafe& m = dynamic_cast<MaquinaCafe&>(owner);
    
    // FASE B (Pura): Solo se mete en la cola. No pregunta por la máquina.
    m.cola.push(who);
    m.lCola.log(m.cola.size());
    
    // Generación en cadena del próximo arribo
    m.schedule(m.arribos.sample(), new Entity(), llegadaC);
}


void SalidaCliente::eventRoutine(Entity* who) {
    MaquinaCafe& m = dynamic_cast<MaquinaCafe&>(owner);
    
    // FASE B (Pura): Devuelve el recurso sin mirar la cola.
    m.maquina.returnBin(1);
    m.usoMaquina.log(m.maquina.getMax() - m.maquina.getQuantity());
    
    delete who;
}


void InicioPreparacion::eventRoutine() {
    MaquinaCafe& m = dynamic_cast<MaquinaCafe&>(owner);
    
    // FASE C: Se evalúa de manera automática luego de cada evento B
    // Si hay clientes esperando Y la máquina está libre
    while (!m.cola.empty() && m.maquina.isAvailable(1)) {
        
        m.maquina.acquire(1);
        m.usoMaquina.log(m.maquina.getMax() - m.maquina.getQuantity());
        
        Entity* ent = m.cola.pop();
        m.lCola.log(m.cola.size());
        m.tEspera.log(m.getSimTime() - ent->getClock());
        
        // Se agenda el fin del servicio (Evento B de salida)
        m.schedule(m.preparacion.sample(), ent, salidaC);
    }
}