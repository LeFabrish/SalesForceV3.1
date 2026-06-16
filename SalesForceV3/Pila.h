#pragma once
#pragma managed(push, off)

#include <vector>
#include <functional>
#include "NodoS.h"

using namespace std;

template <typename T>
class Pila {
private:
    NodoS<T>* tope;
    int       tamanio;

public:
    Pila() : tope(nullptr), tamanio(0) {}

    ~Pila() { limpiar(); }

    // ─── Operaciones principales ───────────────────────────────────
    void push(T dato) {
        NodoS<T>* nuevo = new NodoS<T>(dato);
        nuevo->siguiente = tope;
        tope = nuevo;
        tamanio++;
    }

    void pop() {
        if (!tope) return;
        NodoS<T>* temp = tope;
        tope = tope->siguiente;
        delete temp;
        tamanio--;
    }

    T& peek() const { return tope->dato; }

    void limpiar() {
        while (tope) pop();
    }

    // ─── Búsqueda lineal con lambda ────────────────────────────────
    template <typename Criterio>
    NodoS<T>* buscar(Criterio criterio) const {
        NodoS<T>* actual = tope;
        while (actual) {
            if (criterio(actual->dato)) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // ─── Búsqueda binaria (requiere la pila ordenada previamente) ──
    // O(log n) — útil después de ordenarBurbujaOptimizada
    int busquedaBinaria(const T& valorABuscar,
        function<bool(const T&, const T&)> esIgual,
        function<bool(const T&, const T&)> esMenor) const {
        if (estaVacia()) return -1;
        vector<T> arr = toVector();
        int izq = 0, der = static_cast<int>(arr.size()) - 1;
        while (izq <= der) {
            int mid = izq + (der - izq) / 2;
            if (esIgual(arr[mid], valorABuscar)) return mid;
            esMenor(arr[mid], valorABuscar) ? (izq = mid + 1) : (der = mid - 1);
        }
        return -1;
    }

    // ─── Ordenamiento burbuja optimizado ──────────────────────────
    void ordenarBurbujaOptimizada(function<bool(const T&, const T&)> comparadorMayor) {
        if (tamanio <= 1) return;
        bool intercambiado;
        NodoS<T>* ultimo = nullptr;
        do {
            intercambiado = false;
            NodoS<T>* actual = tope;
            while (actual->siguiente != ultimo) {
                if (comparadorMayor(actual->dato, actual->siguiente->dato)) {
                    swap(actual->dato, actual->siguiente->dato);
                    intercambiado = true;
                }
                actual = actual->siguiente;
            }
            ultimo = actual;
        } while (intercambiado);
    }

    // ─── Conversión y consulta ────────────────────────────────────
    vector<T> toVector() const {
        vector<T> v;
        NodoS<T>* actual = tope;
        while (actual) { v.push_back(actual->dato); actual = actual->siguiente; }
        return v;
    }

    void mostrar() const {
        NodoS<T>* actual = tope;
        while (actual) { actual->dato.mostrar(); actual = actual->siguiente; }
    }

    bool      estaVacia()  const { return tope == nullptr; }
    int       getTamanio() const { return tamanio; }
    NodoS<T>* getTope()    const { return tope; }
};

#pragma managed(pop)