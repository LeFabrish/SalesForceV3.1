#pragma once

#pragma managed(push, off)
#include "SistemaCRM.h"
#pragma managed(pop)

#include "MashallHelper.h"

// Inclusión de hijos al final para evitar dependencias circulares
namespace SalesForceV3 { ref class FrmClientes; }
namespace SalesForceV3 { ref class FrmVentas; }
namespace SalesForceV3 { ref class FrmSoporte; }

#include "FrmClientes.h"
#include "FrmVentas.h"
#include "FrmSoporte.h"

namespace SalesForceV3 {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class FrmPrincipal : public Form {
    private:
        SistemaCRM* sistema;
        FrmClientes^ modClientes;
        FrmVentas^ modVentas;
        FrmSoporte^ modSoporte;

        Panel^ panelSidebar;
        Button^ btnClientes;
        Button^ btnVentas;
        Button^ btnSoporte;
        Button^ btnActivo;

    public:
        FrmPrincipal() {
            sistema = new SistemaCRM();
            this->IsMdiContainer = true;
            CrearUI();
            this->Load += gcnew EventHandler(this, &FrmPrincipal::OnLoad);
        }

    protected:
        ~FrmPrincipal() { this->!FrmPrincipal(); }
        !FrmPrincipal() { delete sistema; }

    private:
        void OnLoad(Object^, EventArgs^) {
            modClientes = gcnew FrmClientes(&sistema->gestorCliente);
            modVentas = gcnew FrmVentas(&sistema->gestorVenta);
            modSoporte = gcnew FrmSoporte(&sistema->gestorSoporte);
            ConfigurarHijo(modClientes);
            ConfigurarHijo(modVentas);
            ConfigurarHijo(modSoporte);
            MostrarModulo(modClientes, btnClientes);
        }

        void ConfigurarHijo(Form^ frm) {
            frm->MdiParent = this;
            frm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            frm->Show();
            frm->WindowState = FormWindowState::Maximized;
            frm->Visible = false;
        }

        void MostrarModulo(Form^ frm, Button^ boton) {
            if (modClientes) modClientes->Visible = false;
            if (modVentas)   modVentas->Visible = false;
            if (modSoporte)  modSoporte->Visible = false;

            if (btnActivo) {
                btnActivo->BackColor = EstiloCRM::AzulOscuro();
                btnActivo->Font = gcnew Drawing::Font("Segoe UI", 10.5f);
            }
            boton->BackColor = Color::FromArgb(0, 70, 150);
            boton->Font = gcnew Drawing::Font("Segoe UI Semibold", 10.5f);
            btnActivo = boton;

            frm->Visible = true;
            frm->BringToFront();
            AjustarTamanioHijos();
        }

        void AjustarTamanioHijos() {
            int w = this->ClientSize.Width - panelSidebar->Width;
            int h = this->ClientSize.Height;
            for each (Form ^ hijo in this->MdiChildren) {
                hijo->Location = Point(0, 0);
                hijo->Size = Drawing::Size(w, h);
            }
        }

        // ─── Construcción de UI ───────────────────────────────────
        void CrearUI() {
            this->Text = "SalesForce CRM  v3.0";
            this->Size = Drawing::Size(1280, 768);
            this->MinimumSize = Drawing::Size(900, 600);
            this->BackColor = EstiloCRM::GrisClaro();
            this->StartPosition = FormStartPosition::CenterScreen;

            CrearSidebar();
            this->SizeChanged += gcnew EventHandler(this, &FrmPrincipal::OnResize);
        }

        void CrearSidebar() {
            panelSidebar = gcnew Panel();
            panelSidebar->Dock = DockStyle::Left;
            panelSidebar->Width = 210;
            panelSidebar->BackColor = EstiloCRM::AzulOscuro();

            // ── Header del sidebar ────────────────────────────────
            Panel^ pnlHeader = gcnew Panel();
            pnlHeader->Dock = DockStyle::Top;
            pnlHeader->Height = 70;
            pnlHeader->BackColor = Color::FromArgb(0, 60, 130);

            Label^ lblApp = gcnew Label();
            lblApp->Text = "SalesForce CRM";
            lblApp->Font = gcnew Drawing::Font("Segoe UI Semibold", 13.0f);
            lblApp->ForeColor = Color::White;
            lblApp->Dock = DockStyle::Fill;
            lblApp->TextAlign = ContentAlignment::MiddleCenter;
            pnlHeader->Controls->Add(lblApp);

            // ── Botones de módulo (orden inverso = DockStyle::Top) ─
            btnSoporte = CrearBotonSidebar("  Operaciones y Soporte");
            btnVentas = CrearBotonSidebar("  Embudo de Ventas");
            btnClientes = CrearBotonSidebar("  Gestion de Clientes");

            btnClientes->Click += gcnew EventHandler(this, &FrmPrincipal::btnClientes_Click);
            btnVentas->Click += gcnew EventHandler(this, &FrmPrincipal::btnVentas_Click);
            btnSoporte->Click += gcnew EventHandler(this, &FrmPrincipal::btnSoporte_Click);

            // ── Footer ────────────────────────────────────────────
            Label^ lblVer = gcnew Label();
            lblVer->Text = "v3.0  |  Hito 1";
            lblVer->ForeColor = Color::FromArgb(180, 210, 240);
            lblVer->Font = gcnew Drawing::Font("Segoe UI", 8.0f);
            lblVer->Dock = DockStyle::Bottom;
            lblVer->Height = 28;
            lblVer->TextAlign = ContentAlignment::MiddleCenter;

            panelSidebar->Controls->Add(btnSoporte);
            panelSidebar->Controls->Add(btnVentas);
            panelSidebar->Controls->Add(btnClientes);
            panelSidebar->Controls->Add(pnlHeader);
            panelSidebar->Controls->Add(lblVer);

            this->Controls->Add(panelSidebar);
        }

        Button^ CrearBotonSidebar(String^ texto) {
            Button^ btn = gcnew Button();
            btn->Text = texto;
            btn->Dock = DockStyle::Top;
            btn->Height = 48;
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderSize = 0;
            btn->FlatAppearance->MouseOverBackColor = Color::FromArgb(0, 82, 165);
            btn->BackColor = EstiloCRM::AzulOscuro();
            btn->ForeColor = Color::White;
            btn->Font = gcnew Drawing::Font("Segoe UI", 10.5f);
            btn->TextAlign = ContentAlignment::MiddleLeft;
            btn->Padding = System::Windows::Forms::Padding(14, 0, 0, 0);
            btn->Cursor = Cursors::Hand;
            return btn;
        }

        void btnClientes_Click(Object^, EventArgs^) { MostrarModulo(modClientes, btnClientes); }
        void btnVentas_Click(Object^, EventArgs^) { MostrarModulo(modVentas, btnVentas); }
        void btnSoporte_Click(Object^, EventArgs^) { MostrarModulo(modSoporte, btnSoporte); }
        void OnResize(Object^, EventArgs^) { AjustarTamanioHijos(); }
    };
}