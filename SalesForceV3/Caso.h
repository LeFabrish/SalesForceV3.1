#pragma once
#include "iostream"
#include "string"
using namespace std;

class Caso {
private:
    int id;
    string asunto;
    string estado;
    string prioridad;

public:
    Caso() {
        id = 0;
        asunto = "";
        estado = "Abierto";
        prioridad = "Media";
    }

    Caso(int id, string asunto, string estado, string prioridad) {
        this->id = id;
        this->asunto = asunto;
        this->estado = estado;
        this->prioridad = prioridad;
    }

    int getId() { return id; }
    string getAsunto() { return asunto; }
    string getEstado() { return estado; }
    string getPrioridad() { return prioridad; }

    void setId(int i) { id = i; }
    void setAsunto(string a) { asunto = a; }
    void setEstado(string e) { estado = e; }
    void setPrioridad(string p) { prioridad = p; }

    void mostrar() {
        cout << "  ID        : " << id << endl;
        cout << "  Asunto    : " << asunto << endl;
        cout << "  Estado    : " << estado << endl;
        cout << "  Prioridad : " << prioridad << endl;
        cout << "  ----------------------------------------" << endl;
    }

    void ingresar(int nuevoId) {
        id = nuevoId;
        cout << "  Asunto                        : "; getline(cin, asunto);
        cout << "  Estado (Abierto/Cerrado)      : ";getline(cin, estado);
        cout << "  Prioridad (Alta/Media/Baja)   : "; getline(cin, prioridad);
        cout << "  ID del caso: "; cin >> id;
    }
};