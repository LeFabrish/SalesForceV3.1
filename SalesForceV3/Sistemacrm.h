#pragma once
#pragma managed(push, off)

#include "GestorCliente.h"
#include "GestorVenta.h"
#include "GestorSoporte.h"
#include "DatasetGenerator.h"

// Punto de entrada único a la lógica de negocio.
// FrmPrincipal crea una instancia y pasa punteros a los formularios hijos.
class SistemaCRM {
public:
    GestorCliente  gestorCliente;
    GestorVenta    gestorVenta;
    GestorSoporte  gestorSoporte;

    SistemaCRM() {
        DatasetGenerator::cargar(&gestorCliente, &gestorVenta, &gestorSoporte);
    }
};

#pragma managed(pop)