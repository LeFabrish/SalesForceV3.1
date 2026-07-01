#pragma once
#include "CampoFormulario.h"
#include "MashallHelper.h"

// ============================================================================
//  FrmAgregarGenerico.h
//  Formulario modal unico que reemplaza el viejo flujo de "+Agregar agrega una
//  fila vacia en la grilla". Recibe la lista de CampoFormulario de la entidad
//  (armada por el Frm* que lo invoca) y construye los controles -TextBox,
//  ComboBox o MonthCalendar- de forma dinamica. Al presionar "Listo" valida
//  que los campos obligatorios esten completos (Descripcion nunca lo es) y,
//  si todo es correcto, deja los valores disponibles para que el invocador
//  los lea con Texto()/IdSeleccionado()/FechaFormateada() y construya la
//  entidad real (Cuenta, Oportunidad, Caso, etc.).
// ============================================================================
namespace SalesForceV3 {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Collections::Generic;

    public ref class FrmAgregarGenerico : public Form {
    private:
        List<CampoFormulario^>^ campos;
        Dictionary<String^, Control^>^ controles;
        Button^ btnListo;
        Button^ btnCancelar;

    public:
        FrmAgregarGenerico(String^ tituloEntidad, List<CampoFormulario^>^ listaCampos)
            : campos(listaCampos) {
            controles = gcnew Dictionary<String^, Control^>();
            ConstruirUI(tituloEntidad);
        }

    protected:
        ~FrmAgregarGenerico() {}

    public:
        // ─── Lectura de resultados (llamar solo si ShowDialog() == OK) ─────

        // Texto del control, sea TextBox o ComboBox (DropDownList refleja la
        // seleccion actual en su propiedad Text).
        String^ Texto(String^ clave) {
            return controles[clave]->Text->Trim();
        }

        // Extrae el ID numerico de una seleccion de ComboDinamico ("3 - BCP" -> 3).
        int IdSeleccionado(String^ clave) {
            String^ seleccion = Texto(clave);
            int separador = seleccion->IndexOf(" - ");
            String^ idTexto = (separador == -1) ? seleccion : seleccion->Substring(0, separador);
            try { return Int32::Parse(idTexto); }
            catch (Exception^) { return 0; }
        }

        // Fecha elegida en el MonthCalendar, formateada segun el patron .NET dado
        // (ej. "dd/MM/yyyy" o "dd/MM HH:mm").
        String^ FechaFormateada(String^ clave, String^ formato) {
            MonthCalendar^ mc = static_cast<MonthCalendar^>(controles[clave]);
            return mc->SelectionStart.ToString(formato);
        }

        // Valor numerico de un campo Numerico. Se usa InvariantCulture para que
        // el separador decimal sea siempre "." sin importar la configuracion
        // regional de Windows (en es-PE el separador por defecto es ",").
        double Numero(String^ clave) {
            return Double::Parse(Texto(clave), Globalization::CultureInfo::InvariantCulture);
        }

    private:
        // ═══════════════════════════════════════════════════════════
        //  CONSTRUCCIÓN DE UI
        // ═══════════════════════════════════════════════════════════
        void ConstruirUI(String^ tituloEntidad) {
            int anchoCampo = 380;
            int margen = 18;

            this->Text = "Nuevo registro";
            this->StartPosition = FormStartPosition::CenterParent;
            //this->FormBorderStyle = FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->BackColor = Color::White;
            this->Font = gcnew Drawing::Font("Segoe UI", 9.5f);

            Label^ lblTitulo = gcnew Label();
            lblTitulo->Text = tituloEntidad;
            lblTitulo->Font = gcnew Drawing::Font("Segoe UI Semibold", 12.0f);
            lblTitulo->ForeColor = Color::FromArgb(20, 50, 90);
            lblTitulo->Location = Point(margen, 14);
            lblTitulo->Size = Drawing::Size(anchoCampo, 30);

            Panel^ pnlCampos = gcnew Panel();
            pnlCampos->Location = Point(margen, 54);
            pnlCampos->Width = anchoCampo;

            int y = 0;
            String^ grupoActual = nullptr;
            for each (CampoFormulario ^ campo in campos) {
                if (campo->Grupo != nullptr && campo->Grupo != grupoActual) {
                    y += AgregarSeparadorGrupo(pnlCampos, campo->Grupo, y, anchoCampo);
                    grupoActual = campo->Grupo;
                }
                y += AgregarFila(pnlCampos, campo, y, anchoCampo);
            }
            pnlCampos->Height = y;

            Panel^ pnlBotones = gcnew Panel();
            pnlBotones->Location = Point(margen, pnlCampos->Bottom + 20);
            pnlBotones->Size = Drawing::Size(anchoCampo, 36);

            btnListo = EstiloCRM::CrearBotonPrimario("  Listo");
            btnListo->Size = Drawing::Size(120, 34);
            btnListo->Location = Point(anchoCampo - 250, 0);
            btnListo->Click += gcnew EventHandler(this, &FrmAgregarGenerico::Listo_Click);

            btnCancelar = EstiloCRM::CrearBotonSecundario("  Cancelar");
            btnCancelar->Size = Drawing::Size(120, 34);
            btnCancelar->Location = Point(anchoCampo - 120, 0);
            btnCancelar->Click += gcnew EventHandler(this, &FrmAgregarGenerico::Cancelar_Click);

            pnlBotones->Controls->Add(btnListo);
            pnlBotones->Controls->Add(btnCancelar);

            this->Controls->Add(lblTitulo);
            this->Controls->Add(pnlCampos);
            this->Controls->Add(pnlBotones);
            this->ClientSize = Drawing::Size(anchoCampo + 2 * margen, pnlBotones->Bottom + margen);
            this->AcceptButton = btnListo;
            this->CancelButton = btnCancelar;
        }

        // Encabezado visual que agrupa campos relacionados (ej. "Direccion").
        int AgregarSeparadorGrupo(Panel^ contenedor, String^ texto, int y, int ancho) {
            Label^ lbl = gcnew Label();
            lbl->Text = texto;
            lbl->Font = gcnew Drawing::Font("Segoe UI Semibold", 9.0f);
            lbl->ForeColor = Color::FromArgb(0, 85, 170);
            lbl->Location = Point(0, y + 8);
            lbl->Size = Drawing::Size(ancho, 18);
            contenedor->Controls->Add(lbl);
            return 30;
        }

        // Construye Label + control de entrada para un campo; devuelve el alto ocupado.
        int AgregarFila(Panel^ contenedor, CampoFormulario^ campo, int y, int ancho) {
            Label^ lbl = gcnew Label();
            lbl->Text = campo->Etiqueta + (campo->Obligatorio ? " *" : "");
            lbl->Location = Point(0, y);
            lbl->Size = Drawing::Size(ancho, 18);
            lbl->ForeColor = Color::FromArgb(60, 70, 85);
            contenedor->Controls->Add(lbl);

            Control^ entrada = CrearControlEntrada(campo);
            entrada->Location = Point(0, y + 19);
            entrada->Width = ancho;
            contenedor->Controls->Add(entrada);
            controles[campo->Clave] = entrada;

            return entrada->Height + 19 + 12;
        }

        Control^ CrearControlEntrada(CampoFormulario^ campo) {
            switch (campo->Tipo) {
            case TipoCampo::ComboFijo:
            case TipoCampo::ComboDinamico: {
                ComboBox^ cb = gcnew ComboBox();
                cb->DropDownStyle = ComboBoxStyle::DropDownList;
                for each (String ^ o in campo->Opciones) cb->Items->Add(o);
                if (campo->ValorPorDefecto != nullptr) cb->SelectedItem = campo->ValorPorDefecto;
                else if (cb->Items->Count > 0) cb->SelectedIndex = 0;
                return cb;
            }
            case TipoCampo::Fecha: {
                MonthCalendar^ mc = gcnew MonthCalendar();
                mc->MaxSelectionCount = 1;
                return mc;
            }
            default: {
                TextBox^ tb = gcnew TextBox();
                if (campo->ValorPorDefecto != nullptr) tb->Text = campo->ValorPorDefecto;
                return tb;
            }
            }
        }

        // ═══════════════════════════════════════════════════════════
        //  VALIDACIÓN Y CIERRE
        // ═══════════════════════════════════════════════════════════
        void Listo_Click(Object^, EventArgs^) {
            for each (CampoFormulario ^ campo in campos) {
                if (campo->Tipo == TipoCampo::Fecha) continue; // MonthCalendar siempre tiene un valor
                if (campo->Obligatorio && Texto(campo->Clave) == "") {
                    MessageBox::Show("Es obligatorio llenar el formulario para agregar al sistema.",
                        "Campos incompletos", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    return;
                }
                if (campo->Numerico && Texto(campo->Clave) != "") {
                    try { Double::Parse(Texto(campo->Clave), Globalization::CultureInfo::InvariantCulture); }
                    catch (Exception^) {
                        MessageBox::Show("El campo '" + campo->Etiqueta + "' debe ser un numero valido.",
                            "Dato invalido", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                        return;
                    }
                }
            }
            this->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->Close();
        }

        void Cancelar_Click(Object^, EventArgs^) {
            this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Close();
        }
    };
}