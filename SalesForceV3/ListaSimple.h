#pragma once
#pragma managed(push, off)

#include <vector>
#include <functional>
#include "NodoS.h"

using namespace std;

template <typename T>
class ListaSimple {
private:
    NodoS<T>* cabeza;
    int       tamanio;

public:
    ListaSimple() : cabeza(nullptr), tamanio(0) {}

    ~ListaSimple() { limpiar(); }

    // ─── Inserción ────────────────────────────────────────────────
    void insertar(T dato) {
        NodoS<T>* nuevo = new NodoS<T>(dato);
        if (!cabeza) { cabeza = nuevo; }
        else {
            NodoS<T>* actual = cabeza;
            while (actual->siguiente) actual = actual->siguiente;
            actual->siguiente = nuevo;
        }
        tamanio++;
    }

    void insertarAlInicio(T dato) {
        NodoS<T>* nuevo = new NodoS<T>(dato);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        tamanio++;
    }

    // ─── Eliminación ──────────────────────────────────────────────
    void eliminar(int pos) {
        if (!cabeza) return;
        if (pos == 0) {
            NodoS<T>* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            tamanio--;
            return;
        }
        NodoS<T>* actual = cabeza;
        for (int i = 0; i < pos - 1 && actual->siguiente; i++)
            actual = actual->siguiente;
        if (!actual->siguiente) return;
        NodoS<T>* temp = actual->siguiente;
        actual->siguiente = temp->siguiente;
        delete temp;
        tamanio--;
    }

    void limpiar() {
        NodoS<T>* actual = cabeza;
        while (actual) {
            NodoS<T>* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
        cabeza = nullptr;
        tamanio = 0;
    }

    // ─── Búsqueda ─────────────────────────────────────────────────
    template <typename Criterio>
    NodoS<T>* buscar(Criterio criterio) const {
        NodoS<T>* actual = cabeza;
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
            NodoS<T>* actual = cabeza;
            while (actual->siguiente) {
                if (comp(actual->dato, actual->siguiente->dato)) {
                    swap(actual->dato, actual->siguiente->dato);
                    cambio = true;
                }
                actual = actual->siguiente;
            }
        }
    }

    // ─── Conversión y consulta ────────────────────────────────────
    vector<T> toVector() const {
        vector<T> v;
        NodoS<T>* actual = cabeza;
        while (actual) { v.push_back(actual->dato); actual = actual->siguiente; }
        return v;
    }

    void mostrar() const {
        NodoS<T>* actual = cabeza;
        int i = 0;
        while (actual) { actual->dato.mostrar(); actual = actual->siguiente; i++; }
    }

    bool      estaVacia()  const { return cabeza == nullptr; }
    int       getTamanio() const { return tamanio; }
    NodoS<T>* getCabeza()  const { return cabeza; }
};

#pragma managed(pop)