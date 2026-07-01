#pragma once
#pragma managed(push, off)

#include <functional>
#include <vector>
using namespace std;

// ─────────────────────────────────────────────────────────────────
// NodoAVL<T> — a diferencia de NodoArbol (ArbolBinario.h), guarda su
// propia altura para poder calcular el factor de balance en O(1) sin
// tener que recorrer subárboles en cada inserción/eliminación.
// ─────────────────────────────────────────────────────────────────
template <typename T>
struct NodoAVL {
    T dato;
    NodoAVL<T>* izquierdo;
    NodoAVL<T>* derecho;
    int altura; // altura del subárbol enraizado aquí (hoja = 0)
    NodoAVL(T d) : dato(d), izquierdo(nullptr), derecho(nullptr), altura(0) {}
};

// ─────────────────────────────────────────────────────────────────
// ArbolAVL<T> — Árbol Binario de Búsqueda Balanceado (AVL) genérico.
//
// Un BST simple (ArbolBinario.h) puede degenerar en una lista enlazada
// si los datos llegan ya ordenados (ej. cuentas dadas de alta en orden
// alfabético), quedando en O(n) por operación. El AVL evita esto: tras
// cada inserción o eliminación recalcula el factor de balance de cada
// ancestro y aplica rotaciones simples o dobles para que la diferencia
// de alturas entre subárbol izquierdo y derecho nunca supere 1, lo que
// garantiza O(log n) siempre.
//
// Uso en el proyecto: GestorCliente indexa las Cuenta por nombre en
// este árbol. Permite listar y buscar cuentas en orden alfabético en
// O(log n) incluso si se registran miles de altas consecutivas ya
// ordenadas — el escenario exacto donde un BST simple se degradaría.
//
// Complejidad: insertar/buscar/eliminar → O(log n) garantizado.
// ─────────────────────────────────────────────────────────────────
template <typename T>
class ArbolAVL {
private:
    NodoAVL<T>* raiz;
    int tamanio;
    function<bool(const T&, const T&)> esMenor;

    int altura(NodoAVL<T>* n) const { return n ? n->altura : -1; }

    int factorBalance(NodoAVL<T>* n) const {
        return n ? altura(n->izquierdo) - altura(n->derecho) : 0;
    }

    void actualizarAltura(NodoAVL<T>* n) {
        int izq = altura(n->izquierdo), der = altura(n->derecho);
        n->altura = 1 + (izq > der ? izq : der);
    }

    // ─── Rotaciones ─────────────────────────────────────────────
    NodoAVL<T>* rotarDerecha(NodoAVL<T>* y) {
        NodoAVL<T>* x = y->izquierdo;
        NodoAVL<T>* t2 = x->derecho;
        x->derecho = y;
        y->izquierdo = t2;
        actualizarAltura(y);
        actualizarAltura(x);
        return x;
    }

    NodoAVL<T>* rotarIzquierda(NodoAVL<T>* x) {
        NodoAVL<T>* y = x->derecho;
        NodoAVL<T>* t2 = y->izquierdo;
        y->izquierdo = x;
        x->derecho = t2;
        actualizarAltura(x);
        actualizarAltura(y);
        return y;
    }

    // Recalcula altura y aplica la rotación que corresponda según el
    // factor de balance (>1 o <-1) y el signo del hijo cargado.
    NodoAVL<T>* balancear(NodoAVL<T>* nodo) {
        actualizarAltura(nodo);
        int fb = factorBalance(nodo);

        if (fb > 1 && factorBalance(nodo->izquierdo) >= 0)        // Izquierda-Izquierda
            return rotarDerecha(nodo);
        if (fb > 1) {                                             // Izquierda-Derecha
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
            return rotarDerecha(nodo);
        }
        if (fb < -1 && factorBalance(nodo->derecho) <= 0)         // Derecha-Derecha
            return rotarIzquierda(nodo);
        if (fb < -1) {                                            // Derecha-Izquierda
            nodo->derecho = rotarDerecha(nodo->derecho);
            return rotarIzquierda(nodo);
        }
        return nodo;
    }

    NodoAVL<T>* insertarRec(NodoAVL<T>* nodo, T dato) {
        if (!nodo) { tamanio++; return new NodoAVL<T>(dato); }
        if (esMenor(dato, nodo->dato)) nodo->izquierdo = insertarRec(nodo->izquierdo, dato);
        else                            nodo->derecho = insertarRec(nodo->derecho, dato);
        return balancear(nodo);
    }

    NodoAVL<T>* minimoNodo(NodoAVL<T>* nodo) const {
        while (nodo && nodo->izquierdo) nodo = nodo->izquierdo;
        return nodo;
    }

    NodoAVL<T>* eliminarRec(NodoAVL<T>* nodo, const T& clave, bool& eliminado) {
        if (!nodo) return nullptr;
        if (esMenor(clave, nodo->dato)) {
            nodo->izquierdo = eliminarRec(nodo->izquierdo, clave, eliminado);
        }
        else if (esMenor(nodo->dato, clave)) {
            nodo->derecho = eliminarRec(nodo->derecho, clave, eliminado);
        }
        else {
            eliminado = true;
            if (!nodo->izquierdo || !nodo->derecho) {
                NodoAVL<T>* hijo = nodo->izquierdo ? nodo->izquierdo : nodo->derecho;
                delete nodo;
                return hijo;
            }
            NodoAVL<T>* sucesor = minimoNodo(nodo->derecho);
            nodo->dato = sucesor->dato;
            bool dummy = false;
            nodo->derecho = eliminarRec(nodo->derecho, sucesor->dato, dummy);
        }
        return balancear(nodo);
    }

    NodoAVL<T>* buscarRec(NodoAVL<T>* nodo, const T& clave) const {
        if (!nodo) return nullptr;
        if (esMenor(clave, nodo->dato)) return buscarRec(nodo->izquierdo, clave);
        if (esMenor(nodo->dato, clave)) return buscarRec(nodo->derecho, clave);
        return nodo;
    }

    void inOrdenRec(NodoAVL<T>* nodo, function<void(const T&)> accion) const {
        if (!nodo) return;
        inOrdenRec(nodo->izquierdo, accion);
        accion(nodo->dato);
        inOrdenRec(nodo->derecho, accion);
    }

    void preOrdenRec(NodoAVL<T>* nodo, function<void(const T&)> accion) const {
        if (!nodo) return;
        accion(nodo->dato);
        preOrdenRec(nodo->izquierdo, accion);
        preOrdenRec(nodo->derecho, accion);
    }

    void postOrdenRec(NodoAVL<T>* nodo, function<void(const T&)> accion) const {
        if (!nodo) return;
        postOrdenRec(nodo->izquierdo, accion);
        postOrdenRec(nodo->derecho, accion);
        accion(nodo->dato);
    }

    void liberarRec(NodoAVL<T>* nodo) {
        if (!nodo) return;
        liberarRec(nodo->izquierdo);
        liberarRec(nodo->derecho);
        delete nodo;
    }

public:
    explicit ArbolAVL(function<bool(const T&, const T&)> comparadorMenor)
        : raiz(nullptr), tamanio(0), esMenor(comparadorMenor) {
    }

    ~ArbolAVL() { vaciar(); }

    // ─── 1. Insertar (balancea automáticamente) ─────────────────
    void insertar(T dato) { raiz = insertarRec(raiz, dato); }

    // ─── 2. Eliminar (balancea automáticamente) ─────────────────
    bool eliminar(const T& clave) {
        bool eliminado = false;
        raiz = eliminarRec(raiz, clave, eliminado);
        if (eliminado) tamanio--;
        return eliminado;
    }

    // ─── 3. Buscar ──────────────────────────────────────────────
    T* buscar(const T& clave) const {
        NodoAVL<T>* n = buscarRec(raiz, clave);
        return n ? &n->dato : nullptr;
    }

    // ─── 4. Contiene ────────────────────────────────────────────
    bool contiene(const T& clave) const { return buscarRec(raiz, clave) != nullptr; }

    // ─── 5. Recorrido InOrden (produce orden alfabético/ascendente) ──
    void recorridoInOrden(function<void(const T&)> accion) const { inOrdenRec(raiz, accion); }

    // ─── 6. Vector ordenado ─────────────────────────────────────
    vector<T> toVectorOrdenado() const {
        vector<T> resultado;
        inOrdenRec(raiz, [&](const T& d) { resultado.push_back(d); });
        return resultado;
    }
    // ─── 6b. Recorrido PreOrden / PostOrden ──────────────────────
    void recorridoPreOrden(function<void(const T&)> accion) const { preOrdenRec(raiz, accion); }
    void recorridoPostOrden(function<void(const T&)> accion) const { postOrdenRec(raiz, accion); }

    vector<T> toVectorPreOrden() const {
        vector<T> resultado;
        preOrdenRec(raiz, [&](const T& d) { resultado.push_back(d); });
        return resultado;
    }

    vector<T> toVectorPostOrden() const {
        vector<T> resultado;
        postOrdenRec(raiz, [&](const T& d) { resultado.push_back(d); });
        return resultado;
    }

    // ─── 7. Altura del árbol (evidencia de que está balanceado) ─
    int alturaArbol() const { return altura(raiz); }

    // ─── 8. Factor de balance en la raíz (siempre entre -1 y 1) ─
    int getFactorBalanceRaiz() const { return factorBalance(raiz); }

    // ─── 9. Vaciar ──────────────────────────────────────────────
    void vaciar() { liberarRec(raiz); raiz = nullptr; tamanio = 0; }

    bool estaVacio()  const { return raiz == nullptr; }
    int  getTamanio() const { return tamanio; }
    NodoAVL<T>* getRaiz() const { return raiz; }
};

#pragma managed(pop)