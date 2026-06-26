#pragma once
#pragma managed(push, off)

#include <string>
#include <sstream>
#include <vector>
#include "Cola.h"
#include "Pila.h"
#include "ListaSimple.h"
#include "GestorArchivos.h"
#include "Caso.h"
#include "Solucion.h"
#include "Tarea.h"
#include "Evento.h"
#include "Historial.h"

using namespace std;

class GestorSoporte {
private:
    Cola<Caso>        colaCasos;
    Pila<Solucion>    pilaSoluciones;
    Cola<Tarea>       colaTareas;
    ListaSimple<Evento> listaEventos;
    Pila<Historial>   pilaHistorial;
    GestorArchivos    gestor;
    int contCaso = 1, contSolucion = 1, contTarea = 1, contEvento = 1, contHistorial = 1;

public:
    // ─── CASOS (Cola FIFO) ────────────────────────────────────────
    void encolarCaso(Caso c) {
        if (c.getId() == 0) c.setId(contCaso++);
        colaCasos.enqueue(c);
        if (c.getId() >= contCaso) contCaso = c.getId() + 1;
    }

    void atenderCaso() { if (!colaCasos.estaVacia()) colaCasos.dequeue(); }

    // Recursión: busca un caso por ID recorriendo la cola nodo a nodo
    NodoS<Caso>* buscarCasoRec(NodoS<Caso>* nodo, int idBuscado) {
        if (!nodo) return nullptr;
        if (nodo->dato.getId() == idBuscado) return nodo;
        return buscarCasoRec(nodo->siguiente, idBuscado);
    }

    NodoS<Caso>* buscarCasoPorId(int id) {
        return buscarCasoRec(colaCasos.getFrenteNodo(), id);
    }

    void guardarCasos() const {
        auto* lineas = new ListaSimple<string>();
        NodoS<Caso>* n = colaCasos.getFrenteNodo();
        while (n) {
            Caso& c = n->dato;
            ostringstream ss;
            ss << c.getId() << "," << c.getAsunto() << "," << c.getEstado() << "," << c.getPrioridad();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("casos.txt", lineas);
        delete lineas;
    }

    void cargarCasos() {
        colaCasos.limpiar();
        ListaSimple<string>* lineas = gestor.cargarLineas("casos.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Caso c(stoi(GestorArchivos::campo(l, 0)),
                GestorArchivos::campo(l, 1),
                GestorArchivos::campo(l, 2),
                GestorArchivos::campo(l, 3));
            encolarCaso(c);
            n = n->siguiente;
        }
        delete lineas;
    }

    Cola<Caso>* getColaCasos() { return &colaCasos; }
    int getProximoIdCaso() { return contCaso; }

    // ─── SOLUCIONES (Pila LIFO) ───────────────────────────────────
    void apilarSolucion(Solucion s) {
        if (s.getId() == 0) s.setId(contSolucion++);
        pilaSoluciones.push(s);
        if (s.getId() >= contSolucion) contSolucion = s.getId() + 1;
    }

    void desapilarSolucion() { if (!pilaSoluciones.estaVacia()) pilaSoluciones.pop(); }
    Solucion& verTopeSolucion() { return pilaSoluciones.peek(); }
    Pila<Solucion>* getPilaSoluciones() { return &pilaSoluciones; }
    int getProximoIdSolucion() { return contSolucion; }

    void guardarSoluciones() const {
        auto* lineas = new ListaSimple<string>();
        std::vector<Solucion> v = pilaSoluciones.toVector();      // tope -> base
        for (auto it = v.rbegin(); it != v.rend(); ++it) {        // recorrido base -> tope
            Solucion& s = *it;
            ostringstream ss;
            ss << s.getId() << "," << s.getIdCasoAsociado() << "," << s.getDescripcion()
                << "," << s.getFechaResolucion();
            lineas->insertar(ss.str());
        }
        gestor.guardarLineas("soluciones.txt", lineas);
        delete lineas;
    }

    void cargarSoluciones() {
        while (!pilaSoluciones.estaVacia()) pilaSoluciones.pop();
        ListaSimple<string>* lineas = gestor.cargarLineas("soluciones.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Solucion s(stoi(GestorArchivos::campo(l, 0)), stoi(GestorArchivos::campo(l, 1)),
                GestorArchivos::campo(l, 2), GestorArchivos::campo(l, 3));
            apilarSolucion(s);
            n = n->siguiente;
        }
        delete lineas;
    }

    // ─── TAREAS (Cola FIFO) ───────────────────────────────────────
    void encolarTarea(Tarea t) {
        if (t.getId() == 0) t.setId(contTarea++);
        colaTareas.enqueue(t);
        if (t.getId() >= contTarea) contTarea = t.getId() + 1;
    }

    void atenderTarea() { if (!colaTareas.estaVacia()) colaTareas.dequeue(); }
    Cola<Tarea>* getColaTareas() { return &colaTareas; }
    int getProximoIdTarea() { return contTarea; }

    void guardarTareas() const {
        auto* lineas = new ListaSimple<string>();
        NodoS<Tarea>* n = colaTareas.getFrenteNodo();
        while (n) {
            Tarea& t = n->dato;
            ostringstream ss;
            ss << t.getId() << "," << t.getDescripcion() << "," << t.getEstado() << ","
                << t.getFechaLimite();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("tareas.txt", lineas);
        delete lineas;
    }

    void cargarTareas() {
        colaTareas.limpiar();
        ListaSimple<string>* lineas = gestor.cargarLineas("tareas.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Tarea t(stoi(GestorArchivos::campo(l, 0)), GestorArchivos::campo(l, 1),
                GestorArchivos::campo(l, 2), GestorArchivos::campo(l, 3));
            encolarTarea(t);
            n = n->siguiente;
        }
        delete lineas;
    }

    // ─── EVENTOS (ListaSimple) ────────────────────────────────────
    void insertarEvento(Evento e) {
        listaEventos.insertar(e);
        if (e.getId() >= contEvento) contEvento = e.getId() + 1;
    }

    void limpiarEventos() { listaEventos.limpiar(); contEvento = 1; }

    // lambda 9: buscar evento por título
    NodoS<Evento>* buscarEventoPorTitulo(const string& titulo) {
        return listaEventos.buscar([&](Evento e) { return e.getTitulo() == titulo; });
    }

    ListaSimple<Evento>* getListaEventos() { return &listaEventos; }
    int getProximoIdEvento() { return contEvento; }

    void guardarEventos() const {
        auto* lineas = new ListaSimple<string>();
        NodoS<Evento>* n = listaEventos.getCabeza();
        while (n) {
            Evento& e = n->dato;
            ostringstream ss;
            ss << e.getId() << "," << e.getTitulo() << "," << e.getFechaHora() << ","
                << e.getUbicacion();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("eventos.txt", lineas);
        delete lineas;
    }

    void cargarEventos() {
        limpiarEventos();
        ListaSimple<string>* lineas = gestor.cargarLineas("eventos.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Evento e(stoi(GestorArchivos::campo(l, 0)), GestorArchivos::campo(l, 1),
                GestorArchivos::campo(l, 2), GestorArchivos::campo(l, 3));
            insertarEvento(e);
            n = n->siguiente;
        }
        delete lineas;
    }

    // ─── HISTORIAL (Pila LIFO) ────────────────────────────────────
    void apilarHistorial(Historial h) {
        if (h.getId() == 0) h.setId(contHistorial++);
        pilaHistorial.push(h);
        if (h.getId() >= contHistorial) contHistorial = h.getId() + 1;
    }

    void desapilarHistorial() { if (!pilaHistorial.estaVacia()) pilaHistorial.pop(); }
    Historial& verTopeHistorial() { return pilaHistorial.peek(); }
    Pila<Historial>* getPilaHistorial() { return &pilaHistorial; }
    int getProximoIdHistorial() { return contHistorial; }

    void guardarHistorial() const {
        auto* lineas = new ListaSimple<string>();
        std::vector<Historial> v = pilaHistorial.toVector();      // tope -> base
        for (auto it = v.rbegin(); it != v.rend(); ++it) {        // recorrido base -> tope
            Historial& h = *it;
            ostringstream ss;
            ss << h.getId() << "," << h.getModuloAsociado() << "," << h.getAccion() << ","
                << h.getFecha();
            lineas->insertar(ss.str());
        }
        gestor.guardarLineas("historial.txt", lineas);
        delete lineas;
    }

    void cargarHistorial() {
        while (!pilaHistorial.estaVacia()) pilaHistorial.pop();
        ListaSimple<string>* lineas = gestor.cargarLineas("historial.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Historial h(stoi(GestorArchivos::campo(l, 0)), GestorArchivos::campo(l, 1),
                GestorArchivos::campo(l, 2), GestorArchivos::campo(l, 3));
            apilarHistorial(h);
            n = n->siguiente;
        }
        delete lineas;
    }

    void guardarTodo() const {
        guardarCasos();
        guardarSoluciones();
        guardarTareas();
        guardarEventos();
        guardarHistorial();
    }

    void cargarTodo() {
        cargarCasos();
        cargarSoluciones();
        cargarTareas();
        cargarEventos();
        cargarHistorial();
    }
};

#pragma managed(pop)