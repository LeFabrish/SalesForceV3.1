#pragma once
#include "MashallHelper.h"

namespace SalesForceV3 {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    // ============================================================================
    //  FrmLogin.h
    //  Pantalla de bienvenida mostrada antes de FrmPrincipal (ver principal.cpp).
    //  No valida contra la lista de UsuarioCRM guardada en disco: hacerlo
    //  obligaria a cargar el SistemaCRM completo dos veces (una aqui, otra al
    //  abrir FrmPrincipal). En su lugar pide un nombre de operador solo para
    //  personalizar el saludo y dar una entrada mas amigable que abrir la
    //  aplicacion directo sobre un DataGridView.
    // ============================================================================
    public ref class FrmLogin : public Form {
    private:
        Panel^ pnlBanner;
        Panel^ pnlFormulario;
        Label^ lblLogo;
        Label^ lblSubtitulo;
        Label^ lblFooter;
        Label^ lblSaludo;
        Label^ lblUsuario;
        TextBox^ txtUsuario;
        Label^ lblModulo;
        ComboBox^ cmbModulo;
        Button^ btnIngresar;
        Button^ btnSalir;

    public:
        FrmLogin() {
            CrearUI();
        }

        // Modulo elegido en el combo, para que principal.cpp pueda usarlo
        // mas adelante si se desea abrir FrmPrincipal directo en ese modulo.
        property String^ ModuloSeleccionado {
            String^ get() { return cmbModulo->SelectedItem ? cmbModulo->SelectedItem->ToString() : "Clientes"; }
        }

    protected:
        ~FrmLogin() {}

    private:
        // ═══════════════════════════════════════════════════════════
        //  CONSTRUCCIÓN DE UI
        // ═══════════════════════════════════════════════════════════
        void CrearUI() {
            this->Text = "SalesForceV3 — Inicio de sesion";
            this->ClientSize = Drawing::Size(720, 420);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->BackColor = Color::White;

            CrearBanner();
            CrearFormulario();

            this->Controls->Add(pnlFormulario);
            this->Controls->Add(pnlBanner);
            this->AcceptButton = btnIngresar;
        }

        // ─── Panel izquierdo: marca / contexto del proyecto ─────────
        void CrearBanner() {
            pnlBanner = gcnew Panel();
            pnlBanner->Dock = DockStyle::Left;
            pnlBanner->Width = 300;
            pnlBanner->BackColor = EstiloCRM::AzulOscuro();

            lblLogo = gcnew Label();
            lblLogo->Text = "SalesForceV3";
            lblLogo->Font = gcnew Drawing::Font("Segoe UI Semibold", 22.0f);
            lblLogo->ForeColor = Color::White;
            lblLogo->AutoSize = false;
            lblLogo->Size = Drawing::Size(260, 70);
            lblLogo->Location = Point(20, 130);
            lblLogo->TextAlign = ContentAlignment::MiddleLeft;

            lblSubtitulo = gcnew Label();
            lblSubtitulo->Text = "Gestion global de clientes (CRM)\r\nEstructura de Datos Avanzadas — UPC";
            lblSubtitulo->Font = gcnew Drawing::Font("Segoe UI", 9.5f);
            lblSubtitulo->ForeColor = Color::FromArgb(190, 220, 250);
            lblSubtitulo->AutoSize = false;
            lblSubtitulo->Size = Drawing::Size(260, 60);
            lblSubtitulo->Location = Point(20, 205);

            lblFooter = gcnew Label();
            lblFooter->Text = "Modulos: Clientes  ·  Ventas  ·  Soporte";
            lblFooter->Font = gcnew Drawing::Font("Segoe UI", 8.5f);
            lblFooter->ForeColor = Color::FromArgb(150, 190, 230);
            lblFooter->Dock = DockStyle::Bottom;
            lblFooter->Height = 34;
            lblFooter->TextAlign = ContentAlignment::MiddleCenter;

            pnlBanner->Controls->Add(lblSubtitulo);
            pnlBanner->Controls->Add(lblLogo);
            pnlBanner->Controls->Add(lblFooter);
        }

        // ─── Panel derecho: formulario de entrada ───────────────────
        void CrearFormulario() {
            pnlFormulario = gcnew Panel();
            pnlFormulario->Dock = DockStyle::Fill;
            pnlFormulario->BackColor = Color::White;

            lblSaludo = gcnew Label();
            lblSaludo->Text = SaludoSegunHora() + ", bienvenido";
            lblSaludo->Font = gcnew Drawing::Font("Segoe UI Semibold", 15.0f);
            lblSaludo->ForeColor = Color::FromArgb(20, 50, 90);
            lblSaludo->AutoSize = false;
            lblSaludo->Size = Drawing::Size(340, 34);
            lblSaludo->Location = Point(40, 44);

            lblUsuario = gcnew Label();
            lblUsuario->Text = "Nombre de operador";
            lblUsuario->ForeColor = Color::FromArgb(90, 110, 140);
            lblUsuario->AutoSize = true;
            lblUsuario->Location = Point(40, 104);

            txtUsuario = gcnew TextBox();
            txtUsuario->Location = Point(40, 126);
            txtUsuario->Width = 340;
            txtUsuario->Font = gcnew Drawing::Font("Segoe UI", 10.5f);

            lblModulo = gcnew Label();
            lblModulo->Text = "Modulo con el que deseas empezar";
            lblModulo->ForeColor = Color::FromArgb(90, 110, 140);
            lblModulo->AutoSize = true;
            lblModulo->Location = Point(40, 168);

            cmbModulo = gcnew ComboBox();
            cmbModulo->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbModulo->Location = Point(40, 190);
            cmbModulo->Width = 340;
            cmbModulo->Font = gcnew Drawing::Font("Segoe UI", 10.5f);
            cmbModulo->Items->AddRange(gcnew cli::array<String^>{"Clientes", "Ventas", "Soporte"});
            cmbModulo->SelectedIndex = 0;

            btnIngresar = EstiloCRM::CrearBotonPrimario("  Ingresar al sistema");
            btnIngresar->Location = Point(40, 246);
            btnIngresar->Width = 340;
            btnIngresar->Height = 40;
            btnIngresar->Click += gcnew EventHandler(this, &FrmLogin::ingresar_Click);

            btnSalir = gcnew Button();
            btnSalir->Text = "Salir";
            btnSalir->FlatStyle = FlatStyle::Flat;
            btnSalir->FlatAppearance->BorderSize = 0;
            btnSalir->ForeColor = Color::FromArgb(120, 130, 140);
            btnSalir->Font = gcnew Drawing::Font("Segoe UI", 9.0f);
            btnSalir->Location = Point(40, 296);
            btnSalir->Width = 340;
            btnSalir->Cursor = Cursors::Hand;
            btnSalir->Click += gcnew EventHandler(this, &FrmLogin::salir_Click);

            pnlFormulario->Controls->Add(lblSaludo);
            pnlFormulario->Controls->Add(lblUsuario);
            pnlFormulario->Controls->Add(txtUsuario);
            pnlFormulario->Controls->Add(lblModulo);
            pnlFormulario->Controls->Add(cmbModulo);
            pnlFormulario->Controls->Add(btnIngresar);
            pnlFormulario->Controls->Add(btnSalir);
        }

        // Saludo contextual segun la hora del sistema — detalle de
        // usabilidad pedido para que la bienvenida se sienta personalizada.
        String^ SaludoSegunHora() {
            int hora = DateTime::Now.Hour;
            if (hora < 12) return "Buenos dias";
            if (hora < 19) return "Buenas tardes";
            return "Buenas noches";
        }

        // ═══════════════════════════════════════════════════════════
        //  EVENTOS
        // ═══════════════════════════════════════════════════════════
        void ingresar_Click(Object^, EventArgs^) {
            if (String::IsNullOrWhiteSpace(txtUsuario->Text)) {
                MessageBox::Show(this, "Ingresa tu nombre para continuar.",
                    "Falta un dato", MessageBoxButtons::OK, MessageBoxIcon::Information);
                txtUsuario->Focus();
                return;
            }
            this->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->Close();
        }

        void salir_Click(Object^, EventArgs^) {
            this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Close();
        }
    };
}