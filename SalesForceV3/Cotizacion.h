#pragma once
#include "iostream"
#include "string"
using namespace std;

class Cotizacion {
private:
    int id;
    double total;
    string fechaVencimiento;
    string estado;      // "Pendiente", "Aceptada", "Rechazada"

public:
    Cotizacion() {
        id = 0;
        total = 0.0;
        fechaVencimiento = "";
        estado = "Pendiente";
    }

    Cotizacion(int id, double total, string fechaVencimiento, string estado) {
        this->id = id;
        this->total = total;
        this->fechaVencimiento = fechaVencimiento;
        this->estado = estado;
    }

    // Getters
    int getId() { return id; }
    double getTotal() { return total; }
    string getFechaVencimiento() { return fechaVencimiento; }
    string getEstado() { return estado; }

    // Setters
    void setId(int i) { id = i; }
    void setTotal(double t) { total = t; }
    void setFechaVencimiento(string f) { fechaVencimiento = f; }
    void setEstado(string e) { estado = e; }

    void mostrar() {
        cout << "  ID               : " << id << endl;
        cout << "  Total            : S/." << total << endl;
        cout << "  Fecha Vencimiento: " << fechaVencimiento << endl;
        cout << "  Estado           : " << estado << endl;
        cout << "  ----------------------------------------" << endl;
    }

    void ingresar(int nuevoId) {

        id = nuevoId;
        cout << "  Total                      : "; cin >> total;
        cout << "  Fecha Vencimiento (DD/MM/AA): "; cin >> fechaVencimiento;
        cout << "  Estado (Pendiente/Aceptada/Rechazada): "; cin >> estado;
    }
};