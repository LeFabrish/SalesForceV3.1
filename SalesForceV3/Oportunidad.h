#pragma once
#include "iostream"
#include "string"
using namespace std;

class Oportunidad {
private:
    int id;
    string titulo;
    double valorEsperado;
    string fase;    // "Prospeccion", "Negociacion", "Cerrado"

public:
    Oportunidad() {
        id = 0;
        titulo = "";
        valorEsperado = 0.0;
        fase = "Prospeccion";
    }

    Oportunidad(int id, string titulo, double valorEsperado, string fase) {
        this->id = id;
        this->titulo = titulo;
        this->valorEsperado = valorEsperado;
        this->fase = fase;
    }

    // Getters
    int getId() const { return id; }
    string getTitulo() const { return titulo; }
    double getValorEsperado() const { return valorEsperado; }
    string getFase() const { return fase; }

    // Setters
    void setId(int i) { id = i; }
    void setTitulo(string t) { titulo = t; }
    void setValorEsperado(double v) { valorEsperado = v; }
    void setFase(string f) { fase = f; }

    void mostrar() {
        cout << "  ID             : " << id << endl;
        cout << "  Titulo         : " << titulo << endl;
        cout << "  Valor Esperado : S/." << valorEsperado << endl;
        cout << "  Fase           : " << fase << endl;
        cout << "  ----------------------------------------" << endl;
    }

    void ingresar(int nuevoId) {
        id = nuevoId;
        cout << "  Titulo                              : "; cin >> titulo;
        cout << "  Valor Esperado                      : "; cin >> valorEsperado;
        cout << "  Fase (Prospeccion/Negociacion/Cerrado): "; cin >> fase;
    }
};