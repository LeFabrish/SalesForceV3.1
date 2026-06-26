#pragma once
#pragma managed(push, off)

#include <fstream>
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
        // "cuentas.txt" se usa como bandera de "ya hubo una ejecucion anterior".
        // Si existe, se respeta lo que el usuario guardo (altas, ediciones y
        // eliminaciones son permanentes). Si no existe (primera vez que se abre
        // el programa), se siembra el dataset de ejemplo y se guarda de inmediato,
        // para que la siguiente apertura ya encuentre los archivos .txt.
        if (ExisteArchivoPrevio()) {
            gestorCliente.cargarTodo();
            gestorVenta.cargarTodo();
            gestorSoporte.cargarTodo();
        }
        else {
            DatasetGenerator::cargar(&gestorCliente, &gestorVenta, &gestorSoporte);
            gestorCliente.guardarTodo();
            gestorVenta.guardarTodo();
            gestorSoporte.guardarTodo();
        }
    }

private:
    static bool ExisteArchivoPrevio() {
        std::ifstream f("cuentas.txt");
        return f.good();
    }
};

#pragma managed(pop)