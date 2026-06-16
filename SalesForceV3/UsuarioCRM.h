#pragma once
#pragma managed(push, off)
#include <string>
using namespace std;

class UsuarioCRM {
private:
    int id;
    string nombre, apellido, rol, username, password;

public:
    UsuarioCRM() : id(0) {}
    UsuarioCRM(int id, string nombre, string apellido,
        string rol, string username, string password)
        : id(id), nombre(nombre), apellido(apellido),
        rol(rol), username(username), password(password) {
    }

    int    getId()       const { return id; }
    string getNombre()   const { return nombre; }
    string getApellido() const { return apellido; }
    string getRol()      const { return rol; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }

    void setId(int v) { id = v; }
    void setNombre(string v) { nombre = v; }
    void setApellido(string v) { apellido = v; }
    void setRol(string v) { rol = v; }
    void setUsername(string v) { username = v; }
    void setPassword(string v) { password = v; }
};
#pragma managed(pop)