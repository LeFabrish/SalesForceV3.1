#pragma once
#pragma managed(push, off)

template <typename T>
class NodoS {
public:
    T        dato;
    NodoS<T>* siguiente;

    NodoS(T dato) : dato(dato), siguiente(nullptr) {}
};

#pragma managed(pop)