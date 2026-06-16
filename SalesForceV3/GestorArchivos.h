#pragma once
#include "iostream"
#include "fstream"
#include "string"
#include "sstream"
#include "vector"
#include "ListaSimple.h"
using namespace std;

class GestorArchivos {
public:
    GestorArchivos() {}
    ~GestorArchivos() {}

    // Carga líneas de un archivo .txt en una ListaSimple<string>
    ListaSimple<string>* cargarLineas(string rutaArchivo) const {
        ListaSimple<string>* lineas = new ListaSimple<string>();
        ifstream archivo(rutaArchivo);
        string linea;

        if (archivo.is_open()) {
            while (getline(archivo, linea)) {
                lineas->insertar(linea);
            }
            archivo.close();
        }
        else {
            cout << "  [!] No se pudo abrir: " << rutaArchivo << endl;
        }
        return lineas;
    }

    // Guarda todas las líneas de la lista en un archivo .txt
    void guardarLineas(string rutaArchivo, ListaSimple<string>* lineas) const {
        ofstream archivo(rutaArchivo);

        if (archivo.is_open()) {
            NodoS<string>* actual = lineas->getCabeza();
            while (actual != nullptr) {
                archivo << actual->dato << "\n";
                actual = actual->siguiente;
            }
            archivo.close();
        }
        else {
            cout << "   No se pudo guardar en: " << rutaArchivo << endl;
        }
    }

    // Devuelve el campo índice (0-based) de una línea CSV simple separada por comas.
    // Si el índice no existe, devuelve cadena vacía.
    static string campo(const string& linea, int indice) {
        string token;
        stringstream ss(linea);
        int i = 0;
        while (getline(ss, token, ',')) {
            if (i == indice) return token;
            ++i;
        }
        return string("");
    }
};