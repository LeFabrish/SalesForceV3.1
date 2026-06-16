#pragma once
#pragma managed(push, off)
#include <string>
#include "Direccion.h"
using namespace std;

class Contacto {
private:
    int id, idCuenta;
    string nombre, apellido, cargo, telefono, email;
    Direccion direccion;

public:
    Contacto() : id(0), idCuenta(0) {}
    Contacto(int id, string nombre, string apellido, string cargo,
        string telefono, string email, int idCuenta, Direccion dir)
        : id(id), nombre(nombre), apellido(apellido), cargo(cargo),
        telefono(telefono), email(email), idCuenta(idCuenta), direccion(dir) {
    }

    int       getId()        const { return id; }
    string    getNombre()    const { return nombre; }
    string    getApellido()  const { return apellido; }
    string    getCargo()     const { return cargo; }
    string    getTelefono()  const { return telefono; }
    string    getEmail()     const { return email; }
    int       getIdCuenta()  const { return idCuenta; }
    Direccion getDireccion() const { return direccion; }

    void setId(int v) { id = v; }
    void setNombre(string v) { nombre = v; }
    void setApellido(string v) { apellido = v; }
    void setCargo(string v) { cargo = v; }
    void setTelefono(string v) { telefono = v; }
    void setEmail(string v) { email = v; }
    void setIdCuenta(int v) { idCuenta = v; }
    void setDireccion(Direccion v) { direccion = v; }
};
#pragma managed(pop)