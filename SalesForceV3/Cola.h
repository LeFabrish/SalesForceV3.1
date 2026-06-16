#pragma once
#pragma managed(push, off)

#include <vector>
#include <functional>
#include "NodoS.h"

using namespace std;

template <typename T>
class Cola {
private:
    NodoS<T>* frente;
    NodoS<T>* fin;
    int       tamanio;

public:
    Cola() : frente(nullptr), fin(nullptr), tamanio(0) {}

    ~Cola() { limpiar(); }

    // ─── Operaciones principales ───────────────────────────────────
    void enqueue(T dato) {
        NodoS<T>* nuevo = new NodoS<T>(dato);
        if (!frente) { frente = fin = nuevo; }
        else { fin->siguiente = nuevo; fin = nuevo; }
        tamanio++;
    }

    void dequeue() {
        if (!frente) return;
        NodoS<T>* temp = frente;
        frente = frente->siguiente;
        if (!frente) fin = nullptr;
        delete temp;
        tamanio--;
    }

    void limpiar() {
        while (frente) dequeue();
    }

    // ─── Acceso ────────────────────────────────────────────────────
    T& getFrente()     const { return frente->dato; }
    NodoS<T>* getFrenteNodo() const { return frente; }
    NodoS<T>* getFinNodo()    const { return fin; }

    // ─── Búsqueda con lambda ───────────────────────────────────────
    template <typename Criterio>
    NodoS<T>* buscar(Criterio criterio) const {
        NodoS<T>* actual = frente;
        while (actual) {
            if (criterio(actual->dato)) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // ─── Conversión ────────────────────────────────────────────────
    vector<T> toVector() const {
        vector<T> v;
        NodoS<T>* actual = frente;
        while (actual) { v.push_back(actual->dato); actual = actual->siguiente; }
        return v;
    }

    void mostrar() const {
        NodoS<T>* actual = frente;
        while (actual) { actual->dato.mostrar(); actual = actual->siguiente; }
    }

    bool estaVacia()  const { return frente == nullptr; }
    int  getTamanio() const { return tamanio; }
};

#pragma managed(pop)