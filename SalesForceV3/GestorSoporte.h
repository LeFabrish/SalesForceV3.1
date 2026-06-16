#pragma once
#pragma managed(push, off)

#include <string>
#include <sstream>
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

    // ─── TAREAS (Cola FIFO) ───────────────────────────────────────
    void encolarTarea(Tarea t) {
        if (t.getId() == 0) t.setId(contTarea++);
        colaTareas.enqueue(t);
        if (t.getId() >= contTarea) contTarea = t.getId() + 1;
    }

    void atenderTarea() { if (!colaTareas.estaVacia()) colaTareas.dequeue(); }
    Cola<Tarea>* getColaTareas() { return &colaTareas; }
    int getProximoIdTarea() { return contTarea; }

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
};

#pragma managed(pop)