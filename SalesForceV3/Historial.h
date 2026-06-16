#pragma once
#include "iostream"
#include "string"
using namespace std;

class Historial {
private:
    int id;
    string moduloAsociado;
    string accion;
    string fecha;

public:
    Historial() {
        id = 0;
        moduloAsociado = "";
        accion = "";
        fecha = "";
    }

    Historial(int id, string moduloAsociado, string accion, string fecha) {
        this->id = id;
        this->moduloAsociado = moduloAsociado;
        this->accion = accion;
        this->fecha = fecha;
    }

    int getId() { return id; }
    string getModuloAsociado() { return moduloAsociado; }
    string getAccion() { return accion; }
    string getFecha() { return fecha; }

    void setId(int i) { id = i; }
    void setModuloAsociado(string m) { moduloAsociado = m; }
    void setAccion(string a) { accion = a; }
    void setFecha(string f) { fecha = f; }

    void mostrar() {
        cout << "  ID      : " << id << endl;
        cout << "  Modulo  : " << moduloAsociado << endl;
        cout << "  Accion  : " << accion << endl;
        cout << "  Fecha   : " << fecha << endl;
        cout << "  ----------------------------------------" << endl;
    }

    void ingresar(int nuevoId) {
        id = nuevoId;
        cout << "  Modulo Asociado           : "; getline(cin, moduloAsociado);
        cout << "  Accion realizada          : "; getline(cin, accion);
        cout << "  Fecha (DD/MM/AAAA)        : "; getline(cin, fecha);
    }
};