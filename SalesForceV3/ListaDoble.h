#pragma once
#pragma managed(push, off)

#include <vector>
#include <functional>
#include "NodoD.h"

using namespace std;

template <typename T>
class ListaDoble {
private:
    NodoD<T>* cabeza;
    NodoD<T>* cola;
    int       tamanio;

public:
    ListaDoble() : cabeza(nullptr), cola(nullptr), tamanio(0) {}

    ~ListaDoble() { limpiar(); }

    // ─── Inserción ────────────────────────────────────────────────
    void insertar(T dato) {
        NodoD<T>* nuevo = new NodoD<T>(dato);
        if (!cabeza) { cabeza = cola = nuevo; }
        else {
            nuevo->anterior = cola;
            cola->siguiente = nuevo;
            cola = nuevo;
        }
        tamanio++;
    }

    void insertarAlInicio(T dato) {
        NodoD<T>* nuevo = new NodoD<T>(dato);
        if (!cabeza) { cabeza = cola = nuevo; }
        else {
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
            cabeza = nuevo;
        }
        tamanio++;
    }

    // ─── Eliminación ──────────────────────────────────────────────
    void eliminar(int pos) {
        if (!cabeza) return;
        NodoD<T>* actual = cabeza;
        for (int i = 0; i < pos && actual; i++) actual = actual->siguiente;
        if (!actual) return;
        if (actual->anterior) actual->anterior->siguiente = actual->siguiente;
        else cabeza = actual->siguiente;
        if (actual->siguiente) actual->siguiente->anterior = actual->anterior;
        else cola = actual->anterior;
        delete actual;
        tamanio--;
    }

    void limpiar() {
        NodoD<T>* actual = cabeza;
        while (actual) {
            NodoD<T>* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
        cabeza = cola = nullptr;
        tamanio = 0;
    }

    // ─── Búsqueda ─────────────────────────────────────────────────
    template <typename Criterio>
    NodoD<T>* buscar(Criterio criterio) const {
        NodoD<T>* actual = cabeza;
        while (actual) {
            if (criterio(actual->dato)) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // ─── Ordenamiento ─────────────────────────────────────────────
    template <typename Comparador>
    void ordenar(Comparador comp) {
        if (!cabeza) return;
        bool cambio = true;
        while (cambio) {
            cambio = false;
            NodoD<T>* actual = cabeza;
            while (actual->siguiente) {
                if (comp(actual->dato, actual->siguiente->dato)) {
                    swap(actual->dato, actual->siguiente->dato);
                    cambio = true;
                }
                actual = actual->siguiente;
            }
        }
    }


    // ─── Recorridos y conversión ──────────────────────────────────
    void mostrar() const {
        NodoD<T>* actual = cabeza;
        while (actual) { actual->dato.mostrar(); actual = actual->siguiente; }
    }

    void mostrarInverso() const {
        NodoD<T>* actual = cola;
        while (actual) { actual->dato.mostrar(); actual = actual->anterior; }
    }

    vector<T> toVector() const {
        vector<T> v;
        NodoD<T>* actual = cabeza;
        while (actual) { v.push_back(actual->dato); actual = actual->siguiente; }
        return v;
    }

    bool      estaVacia()  const { return cabeza == nullptr; }
    int       getTamanio() const { return tamanio; }
    NodoD<T>* getCabeza()  const { return cabeza; }
    NodoD<T>* getCola()    const { return cola; }
};

#pragma managed(pop)