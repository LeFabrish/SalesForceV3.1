#pragma once
#pragma managed(push, off)
#include <string>
using namespace std;

class Interaccion {
private:
    int id, idContacto, idUsuario;
    string tipo, descripcion, fecha;

public:
    Interaccion() : id(0), idContacto(0), idUsuario(0) {}
    Interaccion(int id, string tipo, string descripcion,
        string fecha, int idContacto, int idUsuario)
        : id(id), tipo(tipo), descripcion(descripcion),
        fecha(fecha), idContacto(idContacto), idUsuario(idUsuario) {
    }

    int    getId()          const { return id; }
    string getTipo()        const { return tipo; }
    string getDescripcion() const { return descripcion; }
    string getFecha()       const { return fecha; }
    int    getIdContacto()  const { return idContacto; }
    int    getIdUsuario()   const { return idUsuario; }

    void setId(int v) { id = v; }
    void setTipo(string v) { tipo = v; }
    void setDescripcion(string v) { descripcion = v; }
    void setFecha(string v) { fecha = v; }
    void setIdContacto(int v) { idContacto = v; }
    void setIdUsuario(int v) { idUsuario = v; }
};
#pragma managed(pop)