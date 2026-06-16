#pragma once
#pragma managed(push, off)

template <typename T>
class NodoD {
public:
    T         dato;
    NodoD<T>* siguiente;
    NodoD<T>* anterior;

    NodoD(T dato) : dato(dato), siguiente(nullptr), anterior(nullptr) {}
};

#pragma managed(pop)