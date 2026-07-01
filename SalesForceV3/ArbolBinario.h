#pragma once
#pragma managed(push, off)

#include <functional>
#include <vector>
using namespace std;

// ─────────────────────────────────────────────────────────────────
// NodoArbol<T> — nodo de un árbol binario simple (sin balanceo).
// ─────────────────────────────────────────────────────────────────
template <typename T>
struct NodoArbol {
    T dato;
    NodoArbol<T>* izquierdo;
    NodoArbol<T>* derecho;
    NodoArbol(T d) : dato(d), izquierdo(nullptr), derecho(nullptr) {}
};

// ─────────────────────────────────────────────────────────────────
// ArbolBinario<T> — Árbol Binario de Búsqueda (BST) genérico.
//
// El criterio de orden NO lo conoce la estructura: se inyecta como una
// lambda "esMenor" en el constructor, igual criterio que ya usan
// ListaDoble::ordenar y Pila::buscar en este proyecto — la estructura
// solo sabe comparar mediante la función que le entrega el cliente,
// por lo que un mismo ArbolBinario<T> sirve para cualquier T y
// cualquier campo de ordenamiento.
//
// Uso en el proyecto: GestorVenta indexa las Oportunidad por
// valorEsperado. Esto permite listarlas ya ordenadas por valor
// (recorridoInOrden) sin tener que invocar un algoritmo de
// ordenamiento aparte cada vez que la vista Hito 2 se abre.
//
// Complejidad: insertar/buscar/eliminar → O(h), donde h es la altura
// del árbol. En el caso promedio (datos no ya ordenados) h ≈ log2(n);
// en el peor caso (inserciones ya ordenadas) degenera a O(n), que es
// precisamente la limitación que el AVL (ArbolAVL.h) corrige.
// ─────────────────────────────────────────────────────────────────
template <typename T>
class ArbolBinario {
private:
    NodoArbol<T>* raiz;
    int tamanio;
    function<bool(const T&, const T&)> esMenor; // true si a debe ir a la izquierda de b

    // ─── Recursión: insertar ────────────────────────────────────
    NodoArbol<T>* insertarRec(NodoArbol<T>* nodo, T dato) {
        if (!nodo) { tamanio++; return new NodoArbol<T>(dato); }
        if (esMenor(dato, nodo->dato)) nodo->izquierdo = insertarRec(nodo->izquierdo, dato);
        else                            nodo->derecho = insertarRec(nodo->derecho, dato);
        return nodo;
    }

    // ─── Recursión: buscar ──────────────────────────────────────
    NodoArbol<T>* buscarRec(NodoArbol<T>* nodo, const T& clave) const {
        if (!nodo) return nullptr;
        if (esMenor(clave, nodo->dato)) return buscarRec(nodo->izquierdo, clave);
        if (esMenor(nodo->dato, clave)) return buscarRec(nodo->derecho, clave);
        return nodo;
    }

    NodoArbol<T>* minimoNodo(NodoArbol<T>* nodo) const {
        while (nodo && nodo->izquierdo) nodo = nodo->izquierdo;
        return nodo;
    }

    // ─── Recursión: eliminar (con caso de dos hijos vía sucesor) ─
    NodoArbol<T>* eliminarRec(NodoArbol<T>* nodo, const T& clave, bool& eliminado) {
        if (!nodo) return nullptr;
        if (esMenor(clave, nodo->dato)) {
            nodo->izquierdo = eliminarRec(nodo->izquierdo, clave, eliminado);
            return nodo;
        }
        if (esMenor(nodo->dato, clave)) {
            nodo->derecho = eliminarRec(nodo->derecho, clave, eliminado);
            return nodo;
        }
        // Coincidencia encontrada
        eliminado = true;
        if (!nodo->izquierdo) { NodoArbol<T>* der = nodo->derecho; delete nodo; return der; }
        if (!nodo->derecho) { NodoArbol<T>* izq = nodo->izquierdo; delete nodo; return izq; }
        // Dos hijos: se reemplaza por el sucesor inorden (mínimo del subárbol derecho)
        NodoArbol<T>* sucesor = minimoNodo(nodo->derecho);
        nodo->dato = sucesor->dato;
        bool dummy = false;
        nodo->derecho = eliminarRec(nodo->derecho, sucesor->dato, dummy);
        return nodo;
    }

    // ─── Recursión: recorridos clásicos, cada uno recibe una lambda ──
    void inOrdenRec(NodoArbol<T>* nodo, function<void(const T&)> accion) const {
        if (!nodo) return;
        inOrdenRec(nodo->izquierdo, accion);
        accion(nodo->dato);
        inOrdenRec(nodo->derecho, accion);
    }

    void preOrdenRec(NodoArbol<T>* nodo, function<void(const T&)> accion) const {
        if (!nodo) return;
        accion(nodo->dato);
        preOrdenRec(nodo->izquierdo, accion);
        preOrdenRec(nodo->derecho, accion);
    }

    void postOrdenRec(NodoArbol<T>* nodo, function<void(const T&)> accion) const {
        if (!nodo) return;
        postOrdenRec(nodo->izquierdo, accion);
        postOrdenRec(nodo->derecho, accion);
        accion(nodo->dato);
    }

    int alturaRec(NodoArbol<T>* nodo) const {
        if (!nodo) return -1;
        int izq = alturaRec(nodo->izquierdo);
        int der = alturaRec(nodo->derecho);
        return 1 + (izq > der ? izq : der);
    }

    void liberarRec(NodoArbol<T>* nodo) {
        if (!nodo) return;
        liberarRec(nodo->izquierdo);
        liberarRec(nodo->derecho);
        delete nodo;
    }

public:
    explicit ArbolBinario(function<bool(const T&, const T&)> comparadorMenor)
        : raiz(nullptr), tamanio(0), esMenor(comparadorMenor) {
    }

    ~ArbolBinario() { vaciar(); }

    // ─── 1. Insertar ────────────────────────────────────────────
    void insertar(T dato) { raiz = insertarRec(raiz, dato); }

    // ─── 2. Buscar (devuelve puntero al dato o nullptr) ─────────
    T* buscar(const T& clave) const {
        NodoArbol<T>* n = buscarRec(raiz, clave);
        return n ? &n->dato : nullptr;
    }

    // ─── 3. Contiene ────────────────────────────────────────────
    bool contiene(const T& clave) const { return buscarRec(raiz, clave) != nullptr; }

    // ─── 4. Eliminar ────────────────────────────────────────────
    bool eliminar(const T& clave) {
        bool eliminado = false;
        raiz = eliminarRec(raiz, clave, eliminado);
        if (eliminado) tamanio--;
        return eliminado;
    }

    // ─── 5-7. Recorridos recursivos (reciben la acción por lambda) ──
    void recorridoInOrden(function<void(const T&)> accion) const { inOrdenRec(raiz, accion); }
    void recorridoPreOrden(function<void(const T&)> accion) const { preOrdenRec(raiz, accion); }
    void recorridoPostOrden(function<void(const T&)> accion) const { postOrdenRec(raiz, accion); }

    // ─── 8. Altura del árbol (hoja = 0, árbol vacío = -1) ───────
    int altura() const { return alturaRec(raiz); }

    // ─── 9. Vector ordenado, construido reutilizando InOrden ─────
    vector<T> toVectorOrdenado() const {
        vector<T> resultado;
        inOrdenRec(raiz, [&](const T& d) { resultado.push_back(d); });
        return resultado;
    }

    // ─── 10. Vector PreOrden ───────────────────────────────────
    vector<T> toVectorPreOrden() const {
        vector<T> resultado;
        preOrdenRec(raiz, [&](const T& d) { resultado.push_back(d); });
        return resultado;
    }

    // ─── 11. Vector PostOrden ──────────────────────────────────
    vector<T> toVectorPostOrden() const {
        vector<T> resultado;
        postOrdenRec(raiz, [&](const T& d) { resultado.push_back(d); });
        return resultado;
    }

    // ─── 10. Vaciar ─────────────────────────────────────────────
    void vaciar() { liberarRec(raiz); raiz = nullptr; tamanio = 0; }

    bool estaVacio()  const { return raiz == nullptr; }
    int  getTamanio() const { return tamanio; }
    NodoArbol<T>* getRaiz() const { return raiz; }
};

#pragma managed(pop)