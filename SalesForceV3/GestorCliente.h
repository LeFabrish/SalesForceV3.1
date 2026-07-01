#pragma once
#pragma managed(push, off)

#include <string>
#include <sstream>
#include "ListaDoble.h"
#include "HashTable.h"
#include "Arbolavl.h"
#include "Grafo.h"
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
	ArbolAVL<Cuenta>        avlCuentas; // HITO "2 O (log n)
	Grafo<string>           grafoCuentas; // HITO "2 cuentas
    GestorArchivos gestor;
    int contCuenta = 1, contContacto = 1, contUsuario = 1, contInteraccion = 1;

public:
    // ─── CUENTAS ──────────────────────────────────────────────────
    GestorCliente() : avlCuentas([](const Cuenta& a, const Cuenta& b) {
        return a.getNombre() < b.getNombre();
        }) {
    }
    void insertarCuenta(Cuenta c) {
        cuentas.insertar(c);
        hashCuentas.insertar(c.getNombre(), c);
        avlCuentas.insertar(c);
        
        if (c.getId() >= contCuenta) contCuenta = c.getId() + 1;
    }
    // refleja el estado actual sin recalcular desde cero en cada alta

    void actualizarGrafoParaCuenta(const Cuenta& nueva) {
		grafoCuentas.insertarVertice(nueva.getNombre());
        NodoD<Cuenta>* n = cuentas.getCabeza();
        while (n) {
            if(n->dato.getId() != nueva.getId() &&
				n->dato.getIndustria() == nueva.getIndustria()) {
                grafoCuentas.insertarArista(nueva.getNombre(), n->dato.getNombre(), n->dato.getIndustria());
            }
            n = n->siguiente;
        }
    }


    void limpiarCuentas() {
        cuentas.limpiar();
        hashCuentas.limpiar();
        avlCuentas.vaciar();
        grafoCuentas.vaciar();
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
    ArbolAVL<Cuenta>* getAvlCuentas() { return &avlCuentas; }
    Grafo<string>* getGrafoCuentas() { return &grafoCuentas; }
    int    getProximoIdCuenta() { return contCuenta; }

    // ─── CONTACTOS ────────────────────────────────────────────────
    void insertarContacto(Contacto c) {
        contactos.insertar(c);
        if (c.getId() >= contContacto) contContacto = c.getId() + 1;
    }

    void limpiarContactos() { contactos.limpiar(); contContacto = 1; }

    void guardarContactos() const {
        auto* lineas = new ListaSimple<string>();
        NodoD<Contacto>* n = contactos.getCabeza();
        while (n) {
            Contacto& c = n->dato;
            ostringstream ss;
            ss << c.getId() << "," << c.getNombre() << "," << c.getApellido() << ","
                << c.getCargo() << "," << c.getTelefono() << "," << c.getEmail() << ","
                << c.getIdCuenta();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("contactos.txt", lineas);
        delete lineas;
    }

    void cargarContactos() {
        limpiarContactos();
        ListaSimple<string>* lineas = gestor.cargarLineas("contactos.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Contacto c(stoi(GestorArchivos::campo(l, 0)),
                GestorArchivos::campo(l, 1), GestorArchivos::campo(l, 2),
                GestorArchivos::campo(l, 3), GestorArchivos::campo(l, 4),
                GestorArchivos::campo(l, 5), stoi(GestorArchivos::campo(l, 6)),
                Direccion("", "", "", ""));
            insertarContacto(c);
            n = n->siguiente;
        }
        delete lineas;
    }

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

    void guardarUsuarios() const {
        auto* lineas = new ListaSimple<string>();
        NodoD<UsuarioCRM>* n = usuarios.getCabeza();
        while (n) {
            UsuarioCRM& u = n->dato;
            ostringstream ss;
            ss << u.getId() << "," << u.getNombre() << "," << u.getApellido() << ","
                << u.getRol() << "," << u.getUsername() << "," << u.getPassword();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("usuarios.txt", lineas);
        delete lineas;
    }

    void cargarUsuarios() {
        limpiarUsuarios();
        ListaSimple<string>* lineas = gestor.cargarLineas("usuarios.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            UsuarioCRM u(stoi(GestorArchivos::campo(l, 0)),
                GestorArchivos::campo(l, 1), GestorArchivos::campo(l, 2),
                GestorArchivos::campo(l, 3), GestorArchivos::campo(l, 4),
                GestorArchivos::campo(l, 5));
            insertarUsuario(u);
            n = n->siguiente;
        }
        delete lineas;
    }

    ListaDoble<UsuarioCRM>* getUsuarios() { return &usuarios; }
    int getProximoIdUsuario() { return contUsuario; }

    // ─── INTERACCIONES ────────────────────────────────────────────
    void insertarInteraccion(Interaccion i) {
        interacciones.insertar(i);
        if (i.getId() >= contInteraccion) contInteraccion = i.getId() + 1;
    }

    void limpiarInteracciones() { interacciones.limpiar(); contInteraccion = 1; }

    void guardarInteracciones() const {
        auto* lineas = new ListaSimple<string>();
        NodoD<Interaccion>* n = interacciones.getCabeza();
        while (n) {
            Interaccion& i = n->dato;
            ostringstream ss;
            ss << i.getId() << "," << i.getTipo() << "," << i.getDescripcion() << ","
                << i.getFecha() << "," << i.getIdContacto() << "," << i.getIdUsuario();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("interacciones.txt", lineas);
        delete lineas;
    }

    void cargarInteracciones() {
        limpiarInteracciones();
        ListaSimple<string>* lineas = gestor.cargarLineas("interacciones.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Interaccion i(stoi(GestorArchivos::campo(l, 0)),
                GestorArchivos::campo(l, 1), GestorArchivos::campo(l, 2),
                GestorArchivos::campo(l, 3), stoi(GestorArchivos::campo(l, 4)),
                stoi(GestorArchivos::campo(l, 5)));
            insertarInteraccion(i);
            n = n->siguiente;
        }
        delete lineas;
    }

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

    // ===== PERSISTENCIA =====
    void guardarTodo() const {
        guardarCuentas();
        guardarContactos();
        guardarUsuarios();
        guardarInteracciones();
    }

    void cargarTodo() {
        cargarCuentas();
        cargarContactos();
        cargarUsuarios();
        cargarInteracciones();
    }
};

#pragma managed(pop)