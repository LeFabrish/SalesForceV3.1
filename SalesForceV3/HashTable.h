#pragma once
#pragma managed(push, off)

#include <string>
#include <functional>
#include "NodoS.h"

using namespace std;

// Función hash especializable por tipo de clave.
// Implementación por defecto: trata la clave como secuencia de bytes (FNV-1a).
template <typename TKey>
struct FuncionHash {
    static unsigned int calcular(const TKey& clave, int capacidad) {
        unsigned int h = 2166136261u;
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&clave);
        for (size_t i = 0; i < sizeof(TKey); i++) {
            h ^= bytes[i];
            h *= 16777619u;
        }
        // Compresión Fibonacci: distribuye uniformemente en [0, capacidad)
        return (h * 2654435761u) % static_cast<unsigned int>(capacidad);
    }
};

// Especialización para string: FNV-1a byte a byte + compresión Fibonacci
// FNV-1a garantiza avalancha completa (1 bit distinto → ~50% bits distintos en output)
template <>
struct FuncionHash<string> {
    static unsigned int calcular(const string& clave, int capacidad) {
        unsigned int h = 2166136261u;       // FNV offset basis
        for (unsigned char c : clave) {
            h ^= c;
            h *= 16777619u;                 // FNV prime (32-bit)
        }
        return (h * 2654435761u) % static_cast<unsigned int>(capacidad);
    }
};

// Especialización para int: mezcla de bits de Murmur3
template <>
struct FuncionHash<int> {
    static unsigned int calcular(int clave, int capacidad) {
        unsigned int h = static_cast<unsigned int>(clave);
        h ^= h >> 16;  h *= 0x45d9f3bu;
        h ^= h >> 16;  h *= 0x45d9f3bu;
        h ^= h >> 16;
        return h % static_cast<unsigned int>(capacidad);
    }
};

// ─────────────────────────────────────────────────────────────────
// HashTable<TKey, TValue>
// Encadenamiento separado con NodoS como bucket.
// Complejidad: insertar/buscar/eliminar → O(1) promedio, O(n) peor caso.
// ─────────────────────────────────────────────────────────────────
template <typename TKey, typename TValue>
class HashTable {
private:
    struct Entrada {
        TKey   clave;
        TValue valor;
        Entrada(TKey k, TValue v) : clave(k), valor(v) {}
    };

    NodoS<Entrada>** tabla;
    int              capacidad;
    int              tamanio;

    int calcularIndice(const TKey& clave) const {
        return static_cast<int>(FuncionHash<TKey>::calcular(clave, capacidad));
    }

    void liberarBucket(int i) {
        NodoS<Entrada>* actual = tabla[i];
        while (actual) {
            NodoS<Entrada>* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
        tabla[i] = nullptr;
    }

public:
    explicit HashTable(int capacidadInicial = 64) : capacidad(capacidadInicial), tamanio(0) {
        tabla = new NodoS<Entrada>*[capacidad]();
    }

    ~HashTable() {
        limpiar();
        delete[] tabla;
    }

    // ─── 10 métodos públicos ────────────────────────────────────────

    void insertar(const TKey& clave, const TValue& valor) {
        int idx = calcularIndice(clave);
        for (NodoS<Entrada>* n = tabla[idx]; n; n = n->siguiente) {
            if (n->dato.clave == clave) { n->dato.valor = valor; return; }
        }
        NodoS<Entrada>* nuevo = new NodoS<Entrada>(Entrada(clave, valor));
        nuevo->siguiente = tabla[idx];
        tabla[idx] = nuevo;
        tamanio++;
    }

    TValue* buscar(const TKey& clave) const {
        int idx = calcularIndice(clave);
        for (NodoS<Entrada>* n = tabla[idx]; n; n = n->siguiente)
            if (n->dato.clave == clave) return &n->dato.valor;
        return nullptr;
    }

    bool eliminar(const TKey& clave) {
        int idx = calcularIndice(clave);
        NodoS<Entrada>* actual = tabla[idx];
        NodoS<Entrada>* anterior = nullptr;
        while (actual) {
            if (actual->dato.clave == clave) {
                if (anterior) anterior->siguiente = actual->siguiente;
                else          tabla[idx] = actual->siguiente;
                delete actual;
                tamanio--;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    }

    bool contiene(const TKey& clave) const { return buscar(clave) != nullptr; }

    void limpiar() {
        for (int i = 0; i < capacidad; i++) liberarBucket(i);
        tamanio = 0;
    }

    // Itera sobre todas las entradas válidas — justifica lambda 7
    void paraCadaEntrada(function<void(const TKey&, TValue&)> accion) {
        for (int i = 0; i < capacidad; i++) {
            NodoS<Entrada>* actual = tabla[i];
            while (actual) {
                accion(actual->dato.clave, actual->dato.valor);
                actual = actual->siguiente;
            }
        }
    }

    // Longitud máxima de cadena — útil para análisis de colisiones
    int colisionesMaximas() const {
        int maxCadena = 0;
        for (int i = 0; i < capacidad; i++) {
            int len = 0;
            for (NodoS<Entrada>* n = tabla[i]; n; n = n->siguiente) len++;
            if (len > maxCadena) maxCadena = len;
        }
        return maxCadena;
    }

    double factorCarga()  const { return static_cast<double>(tamanio) / capacidad; }
    int    getTamanio()   const { return tamanio; }
    int    getCapacidad() const { return capacidad; }
    bool   estaVacia()    const { return tamanio == 0; }
};

#pragma managed(pop)