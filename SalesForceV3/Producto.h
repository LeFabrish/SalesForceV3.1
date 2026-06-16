#pragma once
#include "iostream"
#include "string"
using namespace std;

class Producto {
private:
    int id;
    string nombre;
    double precio;
    string categoria;

public:
    Producto() {
        id = 0;
        nombre = "";
        precio = 0.0;
        categoria = "";
    }

    Producto(int id, string nombre, double precio, string categoria) {
        this->id = id;
        this->nombre = nombre;
        this->precio = precio;
        this->categoria = categoria;
    }

    // Getters
    int getId() { return id; }
    string getNombre() { return nombre; }
    double getPrecio() { return precio; }
    string getCategoria() { return categoria; }

    // Setters
    void setId(int i) { id = i; }
    void setNombre(string n) { nombre = n; }
    void setPrecio(double p) { precio = p; }
    void setCategoria(string c) { categoria = c; }

    void mostrar() {
        cout << "  ID        : " << id << endl;
        cout << "  Nombre    : " << nombre << endl;
        cout << "  Precio    : $" << precio << endl;
        cout << "  Categoria : " << categoria << endl;
        cout << "  ----------------------------------------" << endl;
    }

    void ingresar(int nuevoId) {
        id = nuevoId;
        cout << "  Nombre    : "; cin >> nombre;
        cout << "  Precio    : "; cin >> precio;
        cout << "  Categoria : "; cin >> categoria;
    }
};