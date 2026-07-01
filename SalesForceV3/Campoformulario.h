#pragma once
// ============================================================================
//  CampoFormulario.h
//  Describe UN campo de entrada para FrmAgregarGenerico (ver FrmAgregarGenerico.h).
//  Cada Frm* (Clientes / Ventas / Soporte) arma una List<CampoFormulario^> con
//  los campos que su entidad necesita; el formulario generico los lee y
//  construye el control adecuado (TextBox, ComboBox o MonthCalendar). Esto
//  evita escribir un Form de "Agregar" distinto para cada una de las
//  entidades del sistema (Cuenta, Contacto, Oportunidad, Caso, etc.) — un
//  mismo componente generico sirve para las 14, cumpliendo DRY.
// ============================================================================
namespace SalesForceV3 {
    using namespace System;
    using namespace System::Collections::Generic;

    // Tipo de control que FrmAgregarGenerico debe construir para el campo.
    public enum class TipoCampo { Texto, ComboFijo, ComboDinamico, Fecha };

    public ref class CampoFormulario sealed {
    public:
        String^ Etiqueta;          // Texto mostrado junto al control (ej. "Nombre")
        String^ Clave;             // Identificador interno para leer el valor (ej. "nombre")
        TipoCampo Tipo;
        bool Obligatorio;          // false solo para campos "Descripcion"
        bool Numerico;             // true si el texto ingresado debe ser un numero valido
        String^ Grupo;             // Encabezado visual opcional (ej. "Direccion")
        String^ ValorPorDefecto;   // Preseleccion / texto inicial opcional
        List<String^>^ Opciones;   // ComboFijo: valores fijos | ComboDinamico: "ID - Texto"

        CampoFormulario(String^ etiqueta, String^ clave, TipoCampo tipo, bool obligatorio,
            String^ grupo, String^ valorPorDefecto)
            : Etiqueta(etiqueta), Clave(clave), Tipo(tipo), Obligatorio(obligatorio),
            Numerico(false), Grupo(grupo), ValorPorDefecto(valorPorDefecto) {
            Opciones = gcnew List<String^>();
        }

        // ─── Fabricas (mantienen legible el armado de cada formulario) ─────

        // Campo de texto libre.
        static CampoFormulario^ Texto(String^ etiqueta, String^ clave, bool obligatorio,
            String^ grupo, String^ valorPorDefecto) {
            return gcnew CampoFormulario(etiqueta, clave, TipoCampo::Texto, obligatorio, grupo, valorPorDefecto);
        }
        static CampoFormulario^ Texto(String^ etiqueta, String^ clave, bool obligatorio) {
            return Texto(etiqueta, clave, obligatorio, nullptr, nullptr);
        }

        // Campo de texto libre cuyo valor debe poder interpretarse como numero (ej. montos).
        static CampoFormulario^ Numero(String^ etiqueta, String^ clave) {
            CampoFormulario^ c = Texto(etiqueta, clave, true, nullptr, nullptr);
            c->Numerico = true;
            return c;
        }

        // ComboBox de opciones fijas y cerradas (ej. tipos, estados, roles).
        static CampoFormulario^ Combo(String^ etiqueta, String^ clave, cli::array<String^>^ opciones,
            String^ grupo) {
            CampoFormulario^ c = gcnew CampoFormulario(etiqueta, clave, TipoCampo::ComboFijo, true, grupo, nullptr);
            for each (String ^ o in opciones) c->Opciones->Add(o);
            return c;
        }
        static CampoFormulario^ Combo(String^ etiqueta, String^ clave, cli::array<String^>^ opciones) {
            return Combo(etiqueta, clave, opciones, nullptr);
        }

        // ComboBox dinamico: lista construida en tiempo de ejecucion con los IDs
        // ya existentes en el sistema (ej. cuentas para elegir al crear un contacto).
        static CampoFormulario^ ComboDinamico(String^ etiqueta, String^ clave, List<String^>^ opciones) {
            CampoFormulario^ c = gcnew CampoFormulario(etiqueta, clave, TipoCampo::ComboDinamico, true, nullptr, nullptr);
            c->Opciones = opciones;
            return c;
        }

        // Selector de fecha mediante MonthCalendar.
        static CampoFormulario^ Fecha(String^ etiqueta, String^ clave) {
            return gcnew CampoFormulario(etiqueta, clave, TipoCampo::Fecha, true, nullptr, nullptr);
        }
    };
}