#pragma once
#include "iostream"
#include "string"
using namespace std;

class Tarea {
private:
    int id;
    string descripcion;
    string estado;
    string fechaLimite;

public:
    Tarea() {
        id = 0;
        descripcion = "";
        estado = "Pendiente";
        fechaLimite = "";
    }

    Tarea(int id, string descripcion, string estado, string fechaLimite) {
        this->id = id;
        this->descripcion = descripcion;
        this->estado = estado;
        this->fechaLimite = fechaLimite;
    }

    int getId() { return id; }
    string getDescripcion() { return descripcion; }
    string getEstado() { return estado; }
    string getFechaLimite() { return fechaLimite; }

    void setId(int i) { id = i; }
    void setDescripcion(string d) { descripcion = d; }
    void setEstado(string e) { estado = e; }
    void setFechaLimite(string f) { fechaLimite = f; }

    void mostrar() {
        cout << "  ID          : " << id << endl;
        cout << "  Descripcion : " << descripcion << endl;
        cout << "  Estado      : " << estado << endl;
        cout << "  Fecha Limite: " << fechaLimite << endl;
        cout << "  ----------------------------------------" << endl;
    }

    void ingresar(int nuevoId) {
        id = nuevoId;
        cout << "  Descripcion                      : "; getline(cin, descripcion);
        cout << "  Estado (Pendiente/Completada)    : "; getline(cin, estado);
        cout << "  Fecha Limite (DD/MM/AAAA)        : "; getline(cin, fechaLimite);
    }
};