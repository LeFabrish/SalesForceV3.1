#pragma once
#pragma managed(push, off)
#include <string>
#include "Direccion.h"
using namespace std;

class Cuenta {
private:
    int id;
    string nombre, industria, telefono, email;
    Direccion direccion;

public:
    Cuenta() : id(0) {}
    Cuenta(int id, string nombre, string industria,
        string telefono, string email, Direccion dir)
        : id(id), nombre(nombre), industria(industria),
        telefono(telefono), email(email), direccion(dir) {
    }

    int       getId()        const { return id; }
    string    getNombre()    const { return nombre; }
    string    getIndustria() const { return industria; }
    string    getTelefono()  const { return telefono; }
    string    getEmail()     const { return email; }
    Direccion getDireccion() const { return direccion; }

    void setId(int v) { id = v; }
    void setNombre(string v) { nombre = v; }
    void setIndustria(string v) { industria = v; }
    void setTelefono(string v) { telefono = v; }
    void setEmail(string v) { email = v; }
    void setDireccion(Direccion v) { direccion = v; }
};
#pragma managed(pop)