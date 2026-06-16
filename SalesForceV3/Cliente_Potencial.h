#pragma once
#pragma managed(push, off)
#include <string>
using namespace std;

class Cliente_Potencial {
private:
    int id; string nombre, correo, interes;
public:
    Cliente_Potencial() : id(0) {}
    Cliente_Potencial(int id, string nombre, string correo, string interes)
        : id(id), nombre(nombre), correo(correo), interes(interes) {
    }
    int    getId()      const { return id; }
    string getNombre()  const { return nombre; }
    string getCorreo()  const { return correo; }
    string getInteres() const { return interes; }
    void setId(int v) { id = v; }
    void setNombre(string v) { nombre = v; }
    void setCorreo(string v) { correo = v; }
    void setInteres(string v) { interes = v; }
};
#pragma managed(pop)