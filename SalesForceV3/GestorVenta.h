#pragma once
#pragma managed(push, off)

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include "ListaDoble.h"
#include "ListaSimple.h"
#include "Cola.h"
#include "ArbolBinario.h"
#include "Ordenador.h"
#include "GestorArchivos.h"
#include "Cliente_Potencial.h"
#include "Producto.h"
#include "Oportunidad.h"
#include "Contrato.h"
#include "Cotizacion.h"

using namespace std;

class GestorVenta {
private:
    Cola<Cliente_Potencial>  colaClientes;
    ListaDoble<Oportunidad>  oportunidades;
    ListaSimple<Producto>    catalogoProductos;
    ListaSimple<Cotizacion>  cotizaciones;
    ListaSimple<Contrato>    contratos;
    GestorArchivos           gestor;
    vector<double>           dataSetPrecios;
    ArbolBinario<Oportunidad> arbolOportunidades; // HITO "2
    int contCliente = 1, contOportunidad = 1, contProducto = 1;
    int contCotizacion = 1, contContrato = 1;

public:
    // ─── CLIENTES POTENCIALES (Cola FIFO) ─────────────────────────
    void encolarCliente(Cliente_Potencial c) {
        if (c.getId() == 0) c.setId(contCliente++);
        colaClientes.enqueue(c);
        if (c.getId() >= contCliente) contCliente = c.getId() + 1;
    }

    void atenderCliente() { if (!colaClientes.estaVacia()) colaClientes.dequeue(); }
    Cola<Cliente_Potencial>* getColaClientes() { return &colaClientes; }
    int getProximoIdCliente() { return contCliente; }

    // ==== GUARDADO ====
    void guardarClientes() const {
        auto* lineas = new ListaSimple<string>();
        NodoS<Cliente_Potencial>* n = colaClientes.getFrenteNodo();
        while (n) {
            Cliente_Potencial& c = n->dato;
            ostringstream ss;
            ss << c.getId() << "," << c.getNombre() << "," << c.getCorreo() << "," << c.getInteres();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("clientesPotenciales.txt", lineas);
        delete lineas;
    }

    void cargarClientes() {
        colaClientes.limpiar();
        ListaSimple<string>* lineas = gestor.cargarLineas("clientesPotenciales.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Cliente_Potencial c(stoi(GestorArchivos::campo(l, 0)),
                GestorArchivos::campo(l, 1), GestorArchivos::campo(l, 2),
                GestorArchivos::campo(l, 3));
            encolarCliente(c);
            n = n->siguiente;
        }
        delete lineas;
    }

    // ─── OPORTUNIDADES ────────────────────────────────────────────
    GestorVenta() : arbolOportunidades([](const Oportunidad& a, const Oportunidad& b) {
        return a.getValorEsperado() < b.getValorEsperado();
        }) {
    }
    
    void insertarOportunidad(Oportunidad o) {
        oportunidades.insertar(o);
        arbolOportunidades.insertar(o);
        if (o.getId() >= contOportunidad) contOportunidad = o.getId() + 1;
    }

    void limpiarOportunidades() {
        oportunidades.limpiar(); contOportunidad = 1;
        arbolOportunidades.vaciar();
    }

    void guardarOportunidades() const {
        auto* lineas = new ListaSimple<string>();
        NodoD<Oportunidad>* n = oportunidades.getCabeza();
        while (n) {
            Oportunidad& o = n->dato;
            ostringstream ss;
            ss << o.getId() << "," << o.getTitulo() << "," << fixed << setprecision(2)
                << o.getValorEsperado() << "," << o.getFase();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("oportunidades.txt", lineas);
        delete lineas;
    }

    void cargarOportunidades() {
        limpiarOportunidades();
        ListaSimple<string>* lineas = gestor.cargarLineas("oportunidades.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Oportunidad o(stoi(GestorArchivos::campo(l, 0)),
                GestorArchivos::campo(l, 1), stod(GestorArchivos::campo(l, 2)),
                GestorArchivos::campo(l, 3));
            insertarOportunidad(o);
            n = n->siguiente;
        }
        delete lineas;
    }

    // Recursión: suma total del pipeline de ventas
    double sumarOportunidadesRec(NodoD<Oportunidad>* nodo) {
        if (!nodo) return 0.0;
        return nodo->dato.getValorEsperado() + sumarOportunidadesRec(nodo->siguiente);
    }

    double totalPipeline() { return sumarOportunidadesRec(oportunidades.getCabeza()); }

    // lambda 6: buscar oportunidad por título
    NodoD<Oportunidad>* buscarOportunidad(const string& titulo) {
        return oportunidades.buscar([&](Oportunidad o) { return o.getTitulo() == titulo; });
    }

    // QuickSort sobre oportunidades — lambda 7
    vector<Oportunidad> ordenarOportunidadesQuickSort() {
        vector<Oportunidad> v = oportunidades.toVector();
        auto comp = [](const Oportunidad& a, const Oportunidad& b) {
            return a.getValorEsperado() < b.getValorEsperado();
            };
        Ordenador<Oportunidad>::quickSort(v, comp);
        return v;
    }

    // MergeSort sobre oportunidades — lambda 8
    vector<Oportunidad> ordenarOportunidadesMergeSort() {
        vector<Oportunidad> v = oportunidades.toVector();
        auto comp = [](const Oportunidad& a, const Oportunidad& b) {
            return a.getValorEsperado() < b.getValorEsperado();
            };
        return Ordenador<Oportunidad>::mergeSort(v, comp);
    }

    ListaDoble<Oportunidad>* getOportunidades() { return &oportunidades; }
    ArbolBinario<Oportunidad>* getArbolOportunidades() { return &arbolOportunidades; }
    int getProximoIdOportunidad() { return contOportunidad; }

    // ─── PRODUCTOS ────────────────────────────────────────────────
    void insertarProducto(Producto p) {
        catalogoProductos.insertar(p);
        if (p.getId() >= contProducto) contProducto = p.getId() + 1;
    }

    void limpiarProductos() { catalogoProductos.limpiar(); contProducto = 1; }

    // lambda 9 reutilizable: filtrar productos premium por precio
    vector<Producto> filtrarProductosPremium(double precioMinimo) {
        vector<Producto> resultado;
        auto es = [=](Producto p) { return p.getPrecio() >= precioMinimo; };
        NodoS<Producto>* n = catalogoProductos.getCabeza();
        while (n) { if (es(n->dato)) resultado.push_back(n->dato); n = n->siguiente; }
        return resultado;
    }

    void guardarProductos() const {
        auto* lineas = new ListaSimple<string>();
        NodoS<Producto>* n = catalogoProductos.getCabeza();
        while (n) {
            Producto& p = n->dato;
            ostringstream ss;
            ss << p.getId() << "," << p.getNombre() << ","
                << fixed << setprecision(2) << p.getPrecio() << "," << p.getCategoria();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("productos.txt", lineas);
        delete lineas;
    }

    void cargarProductos() {
        limpiarProductos();
        ListaSimple<string>* lineas = gestor.cargarLineas("productos.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Producto p(stoi(GestorArchivos::campo(l, 0)),
                GestorArchivos::campo(l, 1),
                stod(GestorArchivos::campo(l, 2)),
                GestorArchivos::campo(l, 3));
            insertarProducto(p);
            n = n->siguiente;
        }
        delete lineas;
    }

    ListaSimple<Producto>* getProductos() { return &catalogoProductos; }
    int getProximoIdProducto() { return contProducto; }

    // ─── COTIZACIONES ─────────────────────────────────────────────
    void insertarCotizacion(Cotizacion c) {
        cotizaciones.insertar(c);
        if (c.getId() >= contCotizacion) contCotizacion = c.getId() + 1;
    }
    void limpiarCotizaciones() { cotizaciones.limpiar(); contCotizacion = 1; }

    void guardarCotizaciones() const {
        auto* lineas = new ListaSimple<string>();
        NodoS<Cotizacion>* n = cotizaciones.getCabeza();
        while (n) {
            Cotizacion& c = n->dato;
            ostringstream ss;
            ss << c.getId() << "," << fixed << setprecision(2) << c.getTotal() << ","
                << c.getFechaVencimiento() << "," << c.getEstado();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("cotizaciones.txt", lineas);
        delete lineas;
    }

    void cargarCotizaciones() {
        limpiarCotizaciones();
        ListaSimple<string>* lineas = gestor.cargarLineas("cotizaciones.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Cotizacion c(stoi(GestorArchivos::campo(l, 0)),
                stod(GestorArchivos::campo(l, 1)), GestorArchivos::campo(l, 2),
                GestorArchivos::campo(l, 3));
            insertarCotizacion(c);
            n = n->siguiente;
        }
        delete lineas;
    }

    ListaSimple<Cotizacion>* getCotizaciones() { return &cotizaciones; }
    int getProximoIdCotizacion() { return contCotizacion; }

    // ─── CONTRATOS ────────────────────────────────────────────────
    void insertarContrato(Contrato c) {
        contratos.insertar(c);
        if (c.getId() >= contContrato) contContrato = c.getId() + 1;
    }
    void limpiarContratos() { contratos.limpiar(); contContrato = 1; }

    void guardarContratos() const {
        auto* lineas = new ListaSimple<string>();
        NodoS<Contrato>* n = contratos.getCabeza();
        while (n) {
            Contrato& c = n->dato;
            ostringstream ss;
            ss << c.getId() << "," << c.getFechaFirma() << "," << c.getTerminos() << ","
                << fixed << setprecision(2) << c.getMontoTotal();
            lineas->insertar(ss.str());
            n = n->siguiente;
        }
        gestor.guardarLineas("contratos.txt", lineas);
        delete lineas;
    }

    void cargarContratos() {
        limpiarContratos();
        ListaSimple<string>* lineas = gestor.cargarLineas("contratos.txt");
        NodoS<string>* n = lineas->getCabeza();
        while (n) {
            string& l = n->dato;
            Contrato c(stoi(GestorArchivos::campo(l, 0)), GestorArchivos::campo(l, 1),
                GestorArchivos::campo(l, 2), stod(GestorArchivos::campo(l, 3)));
            insertarContrato(c);
            n = n->siguiente;
        }
        delete lineas;
    }

    ListaSimple<Contrato>* getContratos() { return &contratos; }
    int getProximoIdContrato() { return contContrato; }

    // ─── DATASET DE PRECIOS ───────────────────────────────────────
    void generarDataSet() {
        mt19937 rng(42);
        uniform_real_distribution<double> dist(10.0, 9999.0);
        dataSetPrecios.clear();
        for (int i = 0; i < 1000; i++) dataSetPrecios.push_back(dist(rng));

        ofstream f("dataset_precios.txt");
        for (double v : dataSetPrecios) f << fixed << setprecision(2) << v << "\n";
        f.close();
    }

    vector<double>& getDataSet() {
        if (dataSetPrecios.empty()) generarDataSet();
        return dataSetPrecios;
    }

    vector<double> ordenarConQuickSort() {
        vector<double> v = getDataSet();
        auto comp = [](const double& a, const double& b) { return a < b; };
        Ordenador<double>::quickSort(v, comp);
        return v;
    }

    vector<double> ordenarConMergeSort() {
        auto comp = [](const double& a, const double& b) { return a < b; };
        return Ordenador<double>::mergeSort(getDataSet(), comp);
    }

    vector<double> ordenarConHeapSort() {
        vector<double> v = getDataSet();
        auto comp = [](const double& a, const double& b) { return a < b; };
        Ordenador<double>::heapSort(v, comp);
        return v;
    }
    void guardarTodo() const {
        guardarClientes();
        guardarOportunidades();
        guardarProductos();
        guardarCotizaciones();
        guardarContratos();
    }

    void cargarTodo() {
        cargarClientes();
        cargarOportunidades();
        cargarProductos();
        cargarCotizaciones();
        cargarContratos();
    }
};

#pragma managed(pop)