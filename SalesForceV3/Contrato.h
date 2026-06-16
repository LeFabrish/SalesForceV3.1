#pragma once
#include "iostream"
#include "string"
using namespace std;

class Contrato {
private:
    int id;
    string fechaFirma;
    string terminos;
    double montoTotal;

public:
    Contrato() {
        id = 0;
        fechaFirma = "";
        terminos = "";
        montoTotal = 0.0;
    }

    Contrato(int id, string fechaFirma, string terminos, double montoTotal) {
        this->id = id;
        this->fechaFirma = fechaFirma;
        this->terminos = terminos;
        this->montoTotal = montoTotal;
    }

    // Getters
    int getId() { return id; }
    string getFechaFirma() { return fechaFirma; }
    string getTerminos() { return terminos; }
    double getMontoTotal() { return montoTotal; }

    // Setters
    void setId(int i) { id = i; }
    void setFechaFirma(string f) { fechaFirma = f; }
    void setTerminos(string t) { terminos = t; }
    void setMontoTotal(double m) { montoTotal = m; }

    void mostrar() {
        cout << "  ID          : " << id << endl;
        cout << "  Fecha Firma : " << fechaFirma << endl;
        cout << "  Terminos    : " << terminos << endl;
        cout << "  Monto Total : $" << montoTotal << endl;
        cout << "  ----------------------------------------" << endl;
    }

    void ingresar(int nuevoId) {
        id = nuevoId;
        cout << "  Fecha Firma (DD/MM/AAAA) : "; getline(cin, fechaFirma);
        cout << "  Terminos                 : "; getline(cin, terminos);
        cout << "  Monto Total              : "; cin >> montoTotal;
    }
};