#pragma once
#pragma managed(push, off)

#include <vector>
#include <functional>
#include <utility>

using namespace std;

// Ordenador<T>: algoritmos estáticos genéricos que trabajan sobre vector<T>.
// Comparador: función inyectada que evalúa si el primer argumento es "menor" que el segundo.
//
// Complejidades:
//   QuickSort  → O(n log n) promedio, O(n²) peor caso  — mejor para datos aleatorios
//   MergeSort  → O(n log n) garantizado, estable        — mejor cuando se requiere estabilidad
//   HeapSort   → O(n log n) garantizado, in-place       — mejor con memoria limitada
template <typename T>
class Ordenador {
private:
    // ─── QuickSort: partición de Lomuto ───────────────────────────
    // !comp(pivote, arr[j]) equivale a arr[j] <= pivote (sin necesitar ==)
    static int particion(vector<T>& arr, int bajo, int alto,
        function<bool(const T&, const T&)> comp) {
        T pivote = arr[alto];
        int i = bajo - 1;
        for (int j = bajo; j < alto; j++) {
            if (!comp(pivote, arr[j])) swap(arr[++i], arr[j]);
        }
        swap(arr[i + 1], arr[alto]);
        return i + 1;
    }

    static void quickSortRec(vector<T>& arr, int bajo, int alto,
        function<bool(const T&, const T&)> comp) {
        if (bajo >= alto) return;
        int pi = particion(arr, bajo, alto, comp);
        quickSortRec(arr, bajo, pi - 1, comp);
        quickSortRec(arr, pi + 1, alto, comp);
    }

    // ─── MergeSort: combinación de mitades ────────────────────────
    static vector<T> combinar(const vector<T>& izq, const vector<T>& der,
        function<bool(const T&, const T&)> comp) {
        vector<T> resultado;
        size_t i = 0, j = 0;
        while (i < izq.size() && j < der.size())
            resultado.push_back(comp(izq[i], der[j]) ? izq[i++] : der[j++]);
        while (i < izq.size()) resultado.push_back(izq[i++]);
        while (j < der.size()) resultado.push_back(der[j++]);
        return resultado;
    }

    static vector<T> mergeSortRec(const vector<T>& arr,
        function<bool(const T&, const T&)> comp) {
        if (arr.size() <= 1) return arr;
        size_t mid = arr.size() / 2;
        vector<T> izq(arr.begin(), arr.begin() + mid);
        vector<T> der(arr.begin() + mid, arr.end());
        return combinar(mergeSortRec(izq, comp), mergeSortRec(der, comp), comp);
    }

    // ─── HeapSort: max-heap con heapify recursivo ─────────────────
    // Recursión justificada: refleja la estructura recursiva del heap
    static void heapify(vector<T>& arr, int n, int i,
        function<bool(const T&, const T&)> comp) {
        int mayor = i, izq = 2 * i + 1, der = 2 * i + 2;
        if (izq < n && comp(arr[mayor], arr[izq])) mayor = izq;
        if (der < n && comp(arr[mayor], arr[der])) mayor = der;
        if (mayor != i) {
            swap(arr[i], arr[mayor]);
            heapify(arr, n, mayor, comp);
        }
    }

public:
    // QuickSort in-place sobre el vector
    static void quickSort(vector<T>& arr, function<bool(const T&, const T&)> comp) {
        if (arr.size() > 1) quickSortRec(arr, 0, static_cast<int>(arr.size()) - 1, comp);
    }

    // MergeSort — devuelve un vector nuevo (versión no in-place, estable)
    static vector<T> mergeSort(const vector<T>& arr, function<bool(const T&, const T&)> comp) {
        return mergeSortRec(arr, comp);
    }

    // HeapSort in-place sobre el vector
    static void heapSort(vector<T>& arr, function<bool(const T&, const T&)> comp) {
        int n = static_cast<int>(arr.size());
        for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, comp);
        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0, comp);
        }
    }
};

#pragma managed(pop)