#pragma once
#include "iostream"
#include "string"
using namespace std;

class Evento {
private:
    int id;
    string titulo;
    string fechaHora;
    string ubicacion;

public:
    Evento() {
        id = 0;
        titulo = "";
        fechaHora = "";
        ubicacion = "";
    }

    Evento(int id, string titulo, string fechaHora, string ubicacion) {
        this->id = id;
        this->titulo = titulo;
        this->fechaHora = fechaHora;
        this->ubicacion = ubicacion;
    }

    int getId() { return id; }
    string getTitulo() { return titulo; }
    string getFechaHora() { return fechaHora; }
    string getUbicacion() { return ubicacion; }

    void setId(int i) { id = i; }
    void setTitulo(string t) { titulo = t; }
    void setFechaHora(string f) { fechaHora = f; }
    void setUbicacion(string u) { ubicacion = u; }

    void mostrar() {
        cout << "  ID        : " << id << endl;
        cout << "  Titulo    : " << titulo << endl;
        cout << "  Fecha/Hora: " << fechaHora << endl;
        cout << "  Ubicacion : " << ubicacion << endl;
        cout << "  ----------------------------------------" << endl;
    }

    void ingresar(int nuevoId) {
        id = nuevoId;
        cout << "  Titulo                    : "; getline(cin, titulo);
        cout << "  Fecha/Hora (DD/MM HH:MM)  : "; getline(cin, fechaHora);
        cout << "  Ubicacion                 : "; getline(cin, ubicacion);
    }
};