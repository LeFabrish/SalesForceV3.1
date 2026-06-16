#pragma once
#pragma managed(push, off)

#include <string>
#include <sstream>
#include "ListaDoble.h"
#include "HashTable.h"
#include "GestorArchivos.h"
#include "Cuenta.h"
#include "Contacto.h"
#include "UsuarioCRM.h"
#include "Interaccion.h"

using namespace std;

class GestorCliente {
private:
    ListaDoble<Cuenta>      cuentas;
    ListaDoble<Contacto>    contactos;
    ListaDoble<UsuarioCRM>  usuarios;
    ListaDoble<Interaccion> interacciones;
    // HashTable indexa Cuentas por nombre → búsqueda O(1) vs O(n) de la lista
    HashTable<string, Cuenta> hashCuentas;
    GestorArchivos gestor;
    int contCuenta = 1, contContacto = 1, contUsuario = 1, contInteraccion = 1;

public:
    // ─── CUENTAS ──────────────────────────────────────────────────
    void insertarCuenta(Cuenta c) {
        cuentas.insertar(c);
        hashCuentas.insertar(c.getNombre(), c);
        if (c.getId() >= contCuenta) contCuenta = c.getId() + 1;
    }

    void limpiarCuentas() {
        cuentas.limpiar();
        hashCuentas.limpiar();
        contCuenta = 1;
    }

    void guardarCuentas() const {
        auto* lineas = new ListaSimple<string>();
        NodoD<Cuenta>* n = cuentas.getCabeza();
        while (n) {
            Cuenta& c = n->dato;
            ostringstream ss;
            ss << c.getId() << "," << c.getNombre() << "," << c.getIndustria()
                << "," << c.getTelefono() << "," << c.getEmail()
                << "," << c.getDireccion().getPais() << "," << c.getDireccion().getCiudad()
                << "," << c.getDireccion().getDistrito() << "," << c.getDireccion().getCalle();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("cuentas.txt", lineas);
        delete lineas;
    }

    void cargarCuentas() {
        limpiarCuentas();
        ListaSimple<string>* lineas = gestor.cargarLineas("cuentas.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Cuenta c(stoi(GestorArchivos::campo(l, 0)),
                GestorArchivos::campo(l, 1), GestorArchivos::campo(l, 2),
                GestorArchivos::campo(l, 3), GestorArchivos::campo(l, 4),
                Direccion(GestorArchivos::campo(l, 5), GestorArchivos::campo(l, 6),
                    GestorArchivos::campo(l, 7), GestorArchivos::campo(l, 8)));
            insertarCuenta(c);
            n = n->siguiente;
        }
        delete lineas;
    }

    // Búsqueda O(1) vía HashTable — lambda 1
    Cuenta* buscarCuentaHash(const string& nombre) { return hashCuentas.buscar(nombre); }

    // Búsqueda O(n) vía ListaDoble con lambda — lambda 2
    NodoD<Cuenta>* buscarCuentaLista(const string& nombre) {
        return cuentas.buscar([&](Cuenta c) { return c.getNombre() == nombre; });
    }

    void ordenarCuentasPorNombre() {
        // lambda 3
        cuentas.ordenar([](Cuenta a, Cuenta b) { return a.getNombre() > b.getNombre(); });
    }

    ListaDoble<Cuenta>* getCuentas() { return &cuentas; }
    double getHashFactorCarga() { return hashCuentas.factorCarga(); }
    int    getHashTamanio() { return hashCuentas.getTamanio(); }
    int    getHashColisiones() { return hashCuentas.colisionesMaximas(); }
    // Si necesita la capacidad real de la tabla hash, exponerlo mediante un método público
    int    getHashCapacidad() { return hashCuentas.getCapacidad(); }
    int    getProximoIdCuenta() { return contCuenta; }

    // ─── CONTACTOS ────────────────────────────────────────────────
    void insertarContacto(Contacto c) {
        contactos.insertar(c);
        if (c.getId() >= contContacto) contContacto = c.getId() + 1;
    }

    void limpiarContactos() { contactos.limpiar(); contContacto = 1; }

    // lambda 4
    NodoD<Contacto>* buscarContactoPorNombre(const string& nombre) {
        return contactos.buscar([&](Contacto c) { return c.getNombre() == nombre; });
    }

    void ordenarContactosPorApellido() {
        // lambda 5
        contactos.ordenar([](Contacto a, Contacto b) { return a.getApellido() > b.getApellido(); });
    }

    ListaDoble<Contacto>* getContactos() { return &contactos; }
    int getProximoIdContacto() { return contContacto; }

    // ─── USUARIOS ─────────────────────────────────────────────────
    void insertarUsuario(UsuarioCRM u) {
        usuarios.insertar(u);
        if (u.getId() >= contUsuario) contUsuario = u.getId() + 1;
    }

    void limpiarUsuarios() { usuarios.limpiar(); contUsuario = 1; }
    ListaDoble<UsuarioCRM>* getUsuarios() { return &usuarios; }
    int getProximoIdUsuario() { return contUsuario; }

    // ─── INTERACCIONES ────────────────────────────────────────────
    void insertarInteraccion(Interaccion i) {
        interacciones.insertar(i);
        if (i.getId() >= contInteraccion) contInteraccion = i.getId() + 1;
    }

    void limpiarInteracciones() { interacciones.limpiar(); contInteraccion = 1; }

    // Recursión: cuenta cuántas interacciones tiene un contacto dado — O(n)
    int contarInteraccionesRec(NodoD<Interaccion>* nodo, int idContacto) {
        if (!nodo) return 0;
        int cuenta = (nodo->dato.getIdContacto() == idContacto) ? 1 : 0;
        return cuenta + contarInteraccionesRec(nodo->siguiente, idContacto);
    }

    int contarInteraccionesDeContacto(int idContacto) {
        return contarInteraccionesRec(interacciones.getCabeza(), idContacto);
    }

    ListaDoble<Interaccion>* getInteracciones() { return &interacciones; }
    int getProximoIdInteraccion() { return contInteraccion; }
};

#pragma managed(pop)