#pragma once
#pragma managed(push, off)
#include <string>
using namespace std;

class Direccion {
public:
    string pais, ciudad, distrito, calle;

    Direccion() = default;
    Direccion(string pais, string ciudad, string distrito, string calle)
        : pais(pais), ciudad(ciudad), distrito(distrito), calle(calle) {
    }

    string getPais()     const { return pais; }
    string getCiudad()   const { return ciudad; }
    string getDistrito() const { return distrito; }
    string getCalle()    const { return calle; }

    void setPais(string v) { pais = v; }
    void setCiudad(string v) { ciudad = v; }
    void setDistrito(string v) { distrito = v; }
    void setCalle(string v) { calle = v; }
};
#pragma managed(pop)