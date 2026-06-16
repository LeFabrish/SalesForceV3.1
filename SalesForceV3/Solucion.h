#pragma once
#include "iostream"
#include "string"
using namespace std;

class Solucion {
private:
    int id;
    int idCasoAsociado;
    string descripcion;
    string fechaResolucion;

public:
    Solucion() {
        id = 0;
        idCasoAsociado = 0;
        descripcion = "";
        fechaResolucion = "";
    }

    Solucion(int id, int idCasoAsociado, string descripcion, string fechaResolucion) {
        this->id = id;
        this->idCasoAsociado = idCasoAsociado;
        this->descripcion = descripcion;
        this->fechaResolucion = fechaResolucion;
    }

    int getId() { return id; }
    int getIdCasoAsociado() { return idCasoAsociado; }
    string getDescripcion() { return descripcion; }
    string getFechaResolucion() { return fechaResolucion; }

    void setId(int i) { id = i; }
    void setIdCasoAsociado(int ic) { idCasoAsociado = ic; }
    void setDescripcion(string d) { descripcion = d; }
    void setFechaResolucion(string f) { fechaResolucion = f; }

    void mostrar() {
        cout << "  ID              : " << id << endl;
        cout << "  ID Caso         : " << idCasoAsociado << endl;
        cout << "  Descripcion     : " << descripcion << endl;
        cout << "  Fecha Resolucion: " << fechaResolucion << endl;
        cout << "  ----------------------------------------" << endl;
    }

    void ingresar(int nuevoId) {
        id = nuevoId;
        cout << "  ID Caso Asociado              : "; cin >> idCasoAsociado;
        cout << "  Descripcion                   : "; getline(cin, descripcion);
        cout << "  Fecha Resolucion (DD/MM/AAAA) : "; getline(cin, fechaResolucion);
    }
};