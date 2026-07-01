#pragma once
#pragma managed(push, off)

#include <vector>
#include <queue>
#include <functional>
#include <algorithm>
#include <string>
using namespace std;

// ─────────────────────────────────────────────────────────────────
// Arista<T> — conexión entre dos vértices, con una etiqueta que
// describe el motivo de la relación (en el proyecto: la industria
// compartida entre dos cuentas).
// ─────────────────────────────────────────────────────────────────
template <typename T>
struct Arista {
    T destino;
    string etiqueta;
    Arista(T d, string e) : destino(d), etiqueta(e) {}
};

// ─────────────────────────────────────────────────────────────────
// Grafo<T> — grafo NO dirigido, genérico, representado con lista de
// adyacencia (un vector<Arista<T>> por vértice).
//
// Uso en el proyecto: GestorCliente arma un Grafo<string> de Cuentas,
// donde dos cuentas quedan conectadas si comparten la misma industria.
// Esto modela la "red de relaciones" que un vendedor puede explorar:
// dado el nombre de una cuenta, encontrar todas las cuentas de la
// misma industria alcanzables (BFS/DFS), sin tener que comparar cada
// cuenta contra todas las demás manualmente. El panel Hito 2 de
// Soporte dibuja este grafo y permite recorrerlo desde cualquier
// cuenta seleccionada.
//
// Se usa lista de adyacencia (en vez de matriz de adyacencia) porque
// el número de relaciones reales entre cuentas es mucho menor que
// V², así que la lista evita desperdiciar memoria en pares de cuentas
// que jamás se conectan.
//
// Complejidad: insertarArista → O(1) amortizado.
//              BFS / DFS       → O(V + E).
//              existeCamino    → O(V + E) (reutiliza BFS).
// ─────────────────────────────────────────────────────────────────
template <typename T>
class Grafo {
private:
    vector<T> vertices;
    vector<vector<Arista<T>>> adyacencia; // adyacencia[i] = vecinos de vertices[i]

    int indiceDe(const T& v) const {
        for (size_t i = 0; i < vertices.size(); i++)
            if (vertices[i] == v) return static_cast<int>(i);
        return -1;
    }

    // ─── Recursión: DFS ─────────────────────────────────────────
    void dfsRec(int actual, vector<bool>& visitado, vector<T>& resultado) const {
        visitado[actual] = true;
        resultado.push_back(vertices[actual]);
        for (const Arista<T>& ar : adyacencia[actual]) {
            int iVecino = indiceDe(ar.destino);
            if (iVecino != -1 && !visitado[iVecino]) dfsRec(iVecino, visitado, resultado);
        }
    }

public:
    Grafo() {}

    // ─── 1. Insertar vértice (idempotente) ───────────────────────
    void insertarVertice(const T& v) {
        if (indiceDe(v) == -1) {
            vertices.push_back(v);
            adyacencia.push_back(vector<Arista<T>>());
        }
    }

    // ─── 2. Insertar arista no dirigida (crea vértices si faltan) ─
    void insertarArista(const T& origen, const T& destino, string etiqueta) {
        insertarVertice(origen);
        insertarVertice(destino);
        int io = indiceDe(origen), id = indiceDe(destino);
        if (io == id) return; // sin auto-lazos
        adyacencia[io].push_back(Arista<T>(destino, etiqueta));
        adyacencia[id].push_back(Arista<T>(origen, etiqueta));
    }

    // ─── 3. Existe arista directa entre dos vértices ─────────────
    bool existeArista(const T& a, const T& b) const {
        int ia = indiceDe(a);
        if (ia == -1) return false;
        for (const Arista<T>& ar : adyacencia[ia])
            if (ar.destino == b) return true;
        return false;
    }

    // ─── 4. Vecinos directos de un vértice ───────────────────────
    vector<T> obtenerVecinos(const T& v) const {
        vector<T> resultado;
        int i = indiceDe(v);
        if (i == -1) return resultado;
        for (const Arista<T>& ar : adyacencia[i]) resultado.push_back(ar.destino);
        return resultado;
    }

    // ─── 5. Grado del vértice (cantidad de conexiones) ───────────
    int gradoVertice(const T& v) const {
        int i = indiceDe(v);
        return (i == -1) ? 0 : static_cast<int>(adyacencia[i].size());
    }

    // ─── 6. BFS — recorrido por niveles con cola explícita ───────
    vector<T> bfs(const T& inicio) const {
        vector<T> resultado;
        int iInicio = indiceDe(inicio);
        if (iInicio == -1) return resultado;

        vector<bool> visitado(vertices.size(), false);
        queue<int> pendientes;
        visitado[iInicio] = true;
        pendientes.push(iInicio);

        while (!pendientes.empty()) {
            int actual = pendientes.front(); pendientes.pop();
            resultado.push_back(vertices[actual]);
            for (const Arista<T>& ar : adyacencia[actual]) {
                int iVecino = indiceDe(ar.destino);
                if (iVecino != -1 && !visitado[iVecino]) {
                    visitado[iVecino] = true;
                    pendientes.push(iVecino);
                }
            }
        }
        return resultado;
    }

    // ─── 7. DFS — recorrido en profundidad, recursivo ────────────
    vector<T> dfs(const T& inicio) const {
        vector<T> resultado;
        int iInicio = indiceDe(inicio);
        if (iInicio == -1) return resultado;
        vector<bool> visitado(vertices.size(), false);
        dfsRec(iInicio, visitado, resultado);
        return resultado;
    }

    // ─── 8. Existe camino entre dos vértices (reutiliza BFS) ─────
    bool existeCamino(const T& origen, const T& destino) const {
        vector<T> alcanzables = bfs(origen);
        return find(alcanzables.begin(), alcanzables.end(), destino) != alcanzables.end();
    }

    // ─── 9. Recorrer cada arista una sola vez (para dibujarlas) ──
    // Cada arista no dirigida está duplicada internamente (una entrada
    // por extremo); se reporta una única vez comparando índices.
    void paraCadaArista(function<void(const T&, const T&, const string&)> accion) const {
        for (size_t i = 0; i < vertices.size(); i++) {
            for (const Arista<T>& ar : adyacencia[i]) {
                int iDestino = indiceDe(ar.destino);
                if (iDestino > static_cast<int>(i)) accion(vertices[i], ar.destino, ar.etiqueta);
            }
        }
    }

    // ─── 10. Utilidades de consulta ──────────────────────────────
    vector<T> obtenerVertices() const { return vertices; }

    int contarVertices() const { return static_cast<int>(vertices.size()); }

    int contarAristas() const {
        int total = 0;
        for (const auto& lista : adyacencia) total += static_cast<int>(lista.size());
        return total / 2; // cada arista no dirigida se contó dos veces
    }

    bool estaVacio() const { return vertices.empty(); }
    void vaciar() { vertices.clear(); adyacencia.clear(); }
};

#pragma managed(pop)