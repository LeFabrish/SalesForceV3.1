#pragma once

#pragma managed(push, off)
#include "GestorCliente.h"
#pragma managed(pop)

#include "MashallHelper.h"
#include "Frmagregargenerico.h"

namespace SalesForceV3 {
    using namespace System::Collections::Generic;
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace cli;

    public ref class FrmClientes : public Form {
    private:
        GestorCliente* gestor;

        // ─── Controles principales ─────────────────────────────────
        Panel^ pnlMiniSidebar;
        Panel^ pnlContenido;
        Label^ lblTitulo;

        // ─── Vista lista (Cuentas, Contactos, Usuarios, Interacciones)
        Panel^ pnlGrid;
        DataGridView^ dgvDatos;
        Panel^ pnlBotones;
        Button^ btnAccion1;   // Agregar
        Button^ btnAccion2;   // Eliminar
        Button^ btnAccion3;   // Guardar

        // ─── Vista HashTable ───────────────────────────────────────
        Panel^ pnlHash;
        TextBox^ txtBusqueda;
        Label^ lblResultado;
        Label^ lblHashStats;

        // Mini-sidebar buttons
        Button^ btnNavCuentas, ^ btnNavContactos, ^ btnNavUsuarios,
            ^ btnNavInteracciones, ^ btnNavHash;
        Button^ btnNavActivo;

        enum class Vista { Cuentas, Contactos, Usuarios, Interacciones, HashBusqueda };
        Vista vistaActual;

    public:
        FrmClientes(GestorCliente* g) : gestor(g) {
            CrearUI();
            CambiarVista(Vista::Cuentas, btnNavCuentas);
        }

    protected:
        ~FrmClientes() {}

    private:
        // ═══════════════════════════════════════════════════════════
        //  CONSTRUCCIÓN DE UI
        // ═══════════════════════════════════════════════════════════
        void CrearUI() {
            this->Text = "Gestion de Clientes";
            this->BackColor = EstiloCRM::GrisClaro();
            CrearAreaContenido();
            CrearMiniSidebar();
        }

        void CrearMiniSidebar() {
            pnlMiniSidebar = gcnew Panel();
            pnlMiniSidebar->Dock = DockStyle::Left;
            pnlMiniSidebar->Width = 175;
            pnlMiniSidebar->BackColor = Color::FromArgb(0, 85, 170);

            Label^ lbl = gcnew Label();
            lbl->Text = "CLIENTES";
            lbl->Font = gcnew Drawing::Font("Segoe UI Semibold", 9.5f);
            lbl->ForeColor = Color::FromArgb(180, 215, 255);
            lbl->Dock = DockStyle::Top;
            lbl->Height = 38;
            lbl->TextAlign = ContentAlignment::MiddleCenter;

            // Orden inverso para DockStyle::Top (último añadido = posición superior)
            btnNavHash = CrearNavBtn("  HashTable  [O(1)]");
            btnNavInteracciones = CrearNavBtn("  Interacciones");
            btnNavUsuarios = CrearNavBtn("  Usuarios CRM");
            btnNavContactos = CrearNavBtn("  Contactos");
            btnNavCuentas = CrearNavBtn("  Cuentas");

            btnNavCuentas->Click += gcnew EventHandler(this, &FrmClientes::navCuentas_Click);
            btnNavContactos->Click += gcnew EventHandler(this, &FrmClientes::navContactos_Click);
            btnNavUsuarios->Click += gcnew EventHandler(this, &FrmClientes::navUsuarios_Click);
            btnNavInteracciones->Click += gcnew EventHandler(this, &FrmClientes::navInteracciones_Click);
            btnNavHash->Click += gcnew EventHandler(this, &FrmClientes::navHash_Click);

            pnlMiniSidebar->Controls->Add(btnNavHash);
            pnlMiniSidebar->Controls->Add(btnNavInteracciones);
            pnlMiniSidebar->Controls->Add(btnNavUsuarios);
            pnlMiniSidebar->Controls->Add(btnNavContactos);
            pnlMiniSidebar->Controls->Add(btnNavCuentas);
            pnlMiniSidebar->Controls->Add(lbl);

            this->Controls->Add(pnlMiniSidebar);
        }

        Button^ CrearNavBtn(String^ texto) {
            Button^ b = gcnew Button();
            b->Text = texto;
            b->Dock = DockStyle::Top;
            b->Height = 42;
            b->FlatStyle = FlatStyle::Flat;
            b->FlatAppearance->BorderSize = 0;
            b->FlatAppearance->MouseOverBackColor = Color::FromArgb(0, 65, 140);
            b->BackColor = Color::FromArgb(0, 85, 170);
            b->ForeColor = Color::White;
            b->Font = gcnew Drawing::Font("Segoe UI", 9.5f);
            b->TextAlign = ContentAlignment::MiddleLeft;
            b->Padding = System::Windows::Forms::Padding(10, 0, 0, 0);
            b->Cursor = Cursors::Hand;
            return b;
        }

        void CrearAreaContenido() {
            pnlContenido = gcnew Panel();
            pnlContenido->Dock = DockStyle::Fill;
            pnlContenido->BackColor = EstiloCRM::GrisClaro();
            pnlContenido->Padding = System::Windows::Forms::Padding(12, 10, 12, 10);

            lblTitulo = gcnew Label();
            lblTitulo->Dock = DockStyle::Top;
            lblTitulo->Height = 38;
            lblTitulo->Font = gcnew Drawing::Font("Segoe UI Semibold", 12.0f);
            lblTitulo->ForeColor = Color::FromArgb(20, 50, 90);
            lblTitulo->BackColor = Color::Transparent;

            CrearPanelGrid();
            CrearPanelHash();

            pnlContenido->Controls->Add(pnlGrid);
            pnlContenido->Controls->Add(pnlHash);
            pnlContenido->Controls->Add(lblTitulo);
            this->Controls->Add(pnlContenido);
        }

        void CrearPanelGrid() {
            pnlGrid = gcnew Panel();
            pnlGrid->Dock = DockStyle::Fill;

            dgvDatos = gcnew DataGridView();
            dgvDatos->Dock = DockStyle::Fill;
            EstiloCRM::AplicarDGV(dgvDatos);

            pnlBotones = gcnew Panel();
            pnlBotones->Dock = DockStyle::Bottom;
            pnlBotones->Height = 50;
            pnlBotones->BackColor = Color::White;
            pnlBotones->Padding = System::Windows::Forms::Padding(8, 8, 8, 8);

            btnAccion1 = EstiloCRM::CrearBotonPrimario("  + Agregar");
            btnAccion2 = EstiloCRM::CrearBotonPeligro("  Eliminar");
            btnAccion3 = EstiloCRM::CrearBotonSecundario("  Guardar");
            btnAccion1->Width = 120;
            btnAccion2->Width = 110;
            btnAccion3->Width = 110;
            btnAccion1->Location = Point(8, 8);
            btnAccion2->Location = Point(138, 8);
            btnAccion3->Location = Point(258, 8);

            btnAccion1->Click += gcnew EventHandler(this, &FrmClientes::accion1_Click);
            btnAccion2->Click += gcnew EventHandler(this, &FrmClientes::accion2_Click);
            btnAccion3->Click += gcnew EventHandler(this, &FrmClientes::accion3_Click);

            pnlBotones->Controls->Add(btnAccion1);
            pnlBotones->Controls->Add(btnAccion2);
            pnlBotones->Controls->Add(btnAccion3);

            pnlGrid->Controls->Add(dgvDatos);
            pnlGrid->Controls->Add(pnlBotones);
        }

        void CrearPanelHash() {
            pnlHash = gcnew Panel();
            pnlHash->Dock = DockStyle::Fill;
            pnlHash->BackColor = Color::White;
            pnlHash->Padding = System::Windows::Forms::Padding(20);
            pnlHash->Visible = false;

            Label^ lbl1 = gcnew Label();
            lbl1->Text = "Buscar cuenta por nombre (Hash Table — O(1) promedio):";
            lbl1->Font = gcnew Drawing::Font("Segoe UI", 10.0f);
            lbl1->AutoSize = true;
            lbl1->Location = Point(20, 20);

            txtBusqueda = gcnew TextBox();
            txtBusqueda->Font = gcnew Drawing::Font("Segoe UI", 10.0f);
            txtBusqueda->Size = Drawing::Size(280, 30);
            txtBusqueda->Location = Point(20, 50);

            Button^ btnBuscar = EstiloCRM::CrearBotonPrimario("  Buscar");
            btnBuscar->Size = Drawing::Size(100, 34);
            btnBuscar->Location = Point(310, 48);
            btnBuscar->Click += gcnew EventHandler(this, &FrmClientes::buscarHash_Click);

            lblResultado = gcnew Label();
            lblResultado->Font = gcnew Drawing::Font("Segoe UI", 10.0f);
            lblResultado->ForeColor = Color::FromArgb(0, 80, 0);
            lblResultado->AutoSize = false;
            lblResultado->Size = Drawing::Size(600, 60);
            lblResultado->Location = Point(20, 100);

            lblHashStats = gcnew Label();
            lblHashStats->Font = gcnew Drawing::Font("Segoe UI", 9.5f);
            lblHashStats->ForeColor = Color::FromArgb(60, 80, 110);
            lblHashStats->AutoSize = false;
            lblHashStats->Size = Drawing::Size(600, 80);
            lblHashStats->Location = Point(20, 175);

            pnlHash->Controls->Add(lbl1);
            pnlHash->Controls->Add(txtBusqueda);
            pnlHash->Controls->Add(btnBuscar);
            pnlHash->Controls->Add(lblResultado);
            pnlHash->Controls->Add(lblHashStats);
        }

        // ═══════════════════════════════════════════════════════════
        //  LÓGICA DE VISTAS
        // ═══════════════════════════════════════════════════════════
        void CambiarVista(Vista v, Button^ boton) {
            vistaActual = v;
            HighlightNav(boton);

            pnlGrid->Visible = (v != Vista::HashBusqueda);
            pnlHash->Visible = (v == Vista::HashBusqueda);

            switch (v) {
            case Vista::Cuentas:       ConfigurarCuentas();       break;
            case Vista::Contactos:     ConfigurarContactos();     break;
            case Vista::Usuarios:      ConfigurarUsuarios();      break;
            case Vista::Interacciones: ConfigurarInteracciones(); break;
            case Vista::HashBusqueda:  ConfigurarHash();          break;
            }
        }

        void HighlightNav(Button^ b) {
            cli::array<Button^>^ navs = gcnew cli::array<Button^>(5);
            navs[0] = btnNavCuentas; navs[1] = btnNavContactos; navs[2] = btnNavUsuarios;
            navs[3] = btnNavInteracciones; navs[4] = btnNavHash;
            for each (Button ^ n in navs)
                n->BackColor = Color::FromArgb(0, 85, 170);
            b->BackColor = Color::FromArgb(0, 55, 120);
        }

        // ─── Configuración de columnas y carga de datos ────────────
        void ConfigurarCuentas() {
            lblTitulo->Text = "Cuentas  (" + gestor->getCuentas()->getTamanio() + " registros)";
            dgvDatos->Columns->Clear();
            cli::array<String^>^ cols = gcnew cli::array<String^>(9);
            cols[0] = "ID"; cols[1] = "Nombre"; cols[2] = "Industria"; cols[3] = "Telefono"; cols[4] = "Email";
            cols[5] = "Pais"; cols[6] = "Ciudad"; cols[7] = "Distrito"; cols[8] = "Calle";
            for each (String ^ c in cols) dgvDatos->Columns->Add(c, c);
            dgvDatos->Columns[0]->ReadOnly = true;
            dgvDatos->Columns[0]->Width = 45;

            dgvDatos->Rows->Clear();
            NodoD<Cuenta>* n = gestor->getCuentas()->getCabeza();
            while (n) {
                Cuenta& c = n->dato;
                dgvDatos->Rows->Add(c.getId(), Str::M(c.getNombre()), Str::M(c.getIndustria()),
                    Str::M(c.getTelefono()), Str::M(c.getEmail()),
                    Str::M(c.getDireccion().getPais()), Str::M(c.getDireccion().getCiudad()),
                    Str::M(c.getDireccion().getDistrito()), Str::M(c.getDireccion().getCalle()));
                n = n->siguiente;
            }
        }

        void ConfigurarContactos() {
            lblTitulo->Text = "Contactos  (" + gestor->getContactos()->getTamanio() + " registros)";
            dgvDatos->Columns->Clear();
            cli::array<String^>^ cols = gcnew cli::array<String^>(7);
            cols[0] = "ID"; cols[1] = "Nombre"; cols[2] = "Apellido"; cols[3] = "Cargo"; cols[4] = "Telefono"; cols[5] = "Email"; cols[6] = "ID Cuenta";
            for each (String ^ c in cols) dgvDatos->Columns->Add(c, c);
            dgvDatos->Columns[0]->ReadOnly = true;
            dgvDatos->Columns[0]->Width = 45;

            dgvDatos->Rows->Clear();
            NodoD<Contacto>* n = gestor->getContactos()->getCabeza();
            while (n) {
                Contacto& c = n->dato;
                dgvDatos->Rows->Add(c.getId(), Str::M(c.getNombre()), Str::M(c.getApellido()),
                    Str::M(c.getCargo()), Str::M(c.getTelefono()), Str::M(c.getEmail()), c.getIdCuenta());
                n = n->siguiente;
            }
        }

        void ConfigurarUsuarios() {
            lblTitulo->Text = "Usuarios CRM  (" + gestor->getUsuarios()->getTamanio() + " registros)";
            dgvDatos->Columns->Clear();
            cli::array<String^>^ cols = gcnew cli::array<String^>(6);
            cols[0] = "ID"; cols[1] = "Nombre"; cols[2] = "Apellido"; cols[3] = "Rol"; cols[4] = "Username"; cols[5] = "Password";
            for each (String ^ c in cols) dgvDatos->Columns->Add(c, c);
            dgvDatos->Columns[0]->ReadOnly = true;
            dgvDatos->Columns[0]->Width = 45;

            dgvDatos->Rows->Clear();
            NodoD<UsuarioCRM>* n = gestor->getUsuarios()->getCabeza();
            while (n) {
                UsuarioCRM& u = n->dato;
                dgvDatos->Rows->Add(u.getId(), Str::M(u.getNombre()), Str::M(u.getApellido()),
                    Str::M(u.getRol()), Str::M(u.getUsername()), "********");
                n = n->siguiente;
            }
        }

        void ConfigurarInteracciones() {
            lblTitulo->Text = "Interacciones  (" + gestor->getInteracciones()->getTamanio() + " registros)";
            dgvDatos->Columns->Clear();
            cli::array<String^>^ cols = gcnew cli::array<String^>(6);
            cols[0] = "ID"; cols[1] = "Tipo"; cols[2] = "Descripcion"; cols[3] = "Fecha"; cols[4] = "ID Contacto"; cols[5] = "ID Usuario";
            for each (String ^ c in cols) dgvDatos->Columns->Add(c, c);
            dgvDatos->Columns[0]->ReadOnly = true;
            dgvDatos->Columns[0]->Width = 45;

            dgvDatos->Rows->Clear();
            NodoD<Interaccion>* n = gestor->getInteracciones()->getCabeza();
            while (n) {
                Interaccion& i = n->dato;
                dgvDatos->Rows->Add(i.getId(), Str::M(i.getTipo()), Str::M(i.getDescripcion()),
                    Str::M(i.getFecha()), i.getIdContacto(), i.getIdUsuario());
                n = n->siguiente;
            }
        }

        void ConfigurarHash() {
            lblTitulo->Text = "Busqueda O(1)  —  Hash Table";
            ActualizarEstadisticasHash();
        }

        void ActualizarEstadisticasHash() {
            // No acceder directamente a miembros privados de GestorCliente (hashCuentas)
            lblHashStats->Text =
                "Estadisticas de la Hash Table (encadenamiento separado  |  hash polinomial propio h=h*31+caracter):\r\n"
                "  Elementos: " + gestor->getHashTamanio() + "\r\n"
                "  Capacidad: " + gestor->getHashCapacidad() + " buckets\r\n"
                "  Factor de carga: " + (gestor->getHashFactorCarga() * 100).ToString("F1") + " %\r\n"
                "  Cadena maxima: " + gestor->getHashColisiones() + " colisiones";
        }

        int SiguienteId() {
            int mx = 0;
            for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                if (r->IsNewRow) continue;
                Object^ v = r->Cells[0]->Value;
                if (v != nullptr) mx = Math::Max(mx, Int32::Parse(v->ToString()));
            }
            return mx + 1;
        }

        // ═══════════════════════════════════════════════════════════
        //  EVENTOS — BOTONES ACCIÓN
        // ═══════════════════════════════════════════════════════════
        void accion1_Click(Object^, EventArgs^) {
            switch (vistaActual) {
            case Vista::Cuentas:       AgregarCuenta();      break;
            case Vista::Contactos:     AgregarContacto();    break;
            case Vista::Usuarios:      AgregarUsuario();     break;
            case Vista::Interacciones: AgregarInteraccion(); break;
            }
        }
        void accion2_Click(Object^, EventArgs^) {
            dgvDatos->EndEdit();
            if (dgvDatos->SelectedRows->Count == 0) return;
            if (MessageBox::Show("¿Eliminar la fila seleccionada?", "Confirmar",
                MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes)
                dgvDatos->Rows->Remove(dgvDatos->SelectedRows[0]);
        }

        void accion3_Click(Object^, EventArgs^) {
            // EndEdit() confirma el valor que el usuario esta escribiendo en la celda
            // activa. Sin esto, si se hace clic en "Guardar" justo despues de teclear
            // (sin Tab/Enter), esa ultima celda no se copio aun a Cells[i]->Value y el
            // dato se pierde o se lee como vacio/0 (la causa mas probable del "se
            // guarda con indice 0" reportado).
            dgvDatos->EndEdit();
            GuardarVistaActual();
            MessageBox::Show("Datos guardados correctamente.", "OK",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
            CambiarVista(vistaActual, btnNavActivo != nullptr ? btnNavActivo : btnNavCuentas);
        }

        void GuardarVistaActual() {
            switch (vistaActual) {
            case Vista::Cuentas:       GuardarCuentas();       break;
            case Vista::Contactos:     GuardarContactos();     break;
            case Vista::Usuarios:      GuardarUsuarios();      break;
            case Vista::Interacciones: GuardarInteracciones(); break;
            }
        }

        void GuardarCuentas() {
            gestor->limpiarCuentas();
            for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                if (r->IsNewRow || Str::Celda(r->Cells[1]) == "") continue;
                Cuenta c(Str::CeldaInt(r->Cells[0]), Str::N(Str::Celda(r->Cells[1])),
                    Str::N(Str::Celda(r->Cells[2])), Str::N(Str::Celda(r->Cells[3])),
                    Str::N(Str::Celda(r->Cells[4])),
                    Direccion(Str::N(Str::Celda(r->Cells[5])), Str::N(Str::Celda(r->Cells[6])),
                        Str::N(Str::Celda(r->Cells[7])), Str::N(Str::Celda(r->Cells[8]))));
                gestor->insertarCuenta(c);
            }
            gestor->guardarCuentas();
        }

        void GuardarContactos() {
            gestor->limpiarContactos();
            bool hayContactoSinCuenta = false;
            for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                if (r->IsNewRow || Str::Celda(r->Cells[1]) == "") continue;
                int idCuenta = Str::CeldaInt(r->Cells[6]);
                // "ID Cuenta" en blanco se lee como 0 (ver Str::CeldaInt). Se guarda el
                // contacto igual, para no perder lo que el usuario escribio, pero se
                // avisa para que no quede vinculado a una cuenta inexistente sin darse
                // cuenta — esto es lo que se percibia como "se guarda con indice 0".
                if (idCuenta == 0) hayContactoSinCuenta = true;
                Contacto c(Str::CeldaInt(r->Cells[0]), Str::N(Str::Celda(r->Cells[1])),
                    Str::N(Str::Celda(r->Cells[2])), Str::N(Str::Celda(r->Cells[3])),
                    Str::N(Str::Celda(r->Cells[4])), Str::N(Str::Celda(r->Cells[5])),
                    idCuenta, Direccion("", "", "", ""));
                gestor->insertarContacto(c);
            }
            gestor->guardarContactos();
            if (hayContactoSinCuenta) {
                MessageBox::Show(
                    "Uno o mas contactos se guardaron sin una 'ID Cuenta' valida (quedo en 0).\n"
                    "Revisa esa columna si el contacto debe estar vinculado a una cuenta.",
                    "Aviso", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            }
        }

        void GuardarUsuarios() {
            gestor->limpiarUsuarios();
            for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                if (r->IsNewRow || Str::Celda(r->Cells[1]) == "") continue;
                UsuarioCRM u(Str::CeldaInt(r->Cells[0]), Str::N(Str::Celda(r->Cells[1])),
                    Str::N(Str::Celda(r->Cells[2])), Str::N(Str::Celda(r->Cells[3])),
                    Str::N(Str::Celda(r->Cells[4])), Str::N(Str::Celda(r->Cells[5])));
                gestor->insertarUsuario(u);
            }
            gestor->guardarUsuarios();
        }

        void GuardarInteracciones() {
            gestor->limpiarInteracciones();
            for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                if (r->IsNewRow || Str::Celda(r->Cells[1]) == "") continue;
                Interaccion i(Str::CeldaInt(r->Cells[0]), Str::N(Str::Celda(r->Cells[1])),
                    Str::N(Str::Celda(r->Cells[2])), Str::N(Str::Celda(r->Cells[3])),
                    Str::CeldaInt(r->Cells[4]), Str::CeldaInt(r->Cells[5]));
                gestor->insertarInteraccion(i);
            }
            gestor->guardarInteracciones();
        }

        //====== HashTable ======
        void buscarHash_Click(Object^, EventArgs^) {
            std::string nombre = Str::N(txtBusqueda->Text->Trim());
            Cuenta* res = gestor->buscarCuentaHash(nombre);
            if (res) {
                lblResultado->ForeColor = Color::FromArgb(0, 100, 0);
                lblResultado->Text = "Encontrado:\r\n"
                    "  ID: " + res->getId() + "     "
                    "Nombre: " + Str::M(res->getNombre()) + "\r\n"
                    "  Industria: " + Str::M(res->getIndustria()) + "     "
                    "Email: " + Str::M(res->getEmail());
            }
            else {
                lblResultado->ForeColor = Color::FromArgb(160, 0, 0);
                lblResultado->Text = "No encontrado: \"" + txtBusqueda->Text + "\"";
            }
            ActualizarEstadisticasHash();
        }

        // ====== METODOS PRIVADOS =======
        // ─── Alta de Cuenta ─────────────────────────────────────────────
        void AgregarCuenta() {
            auto campos = gcnew List<CampoFormulario^>();
            campos->Add(CampoFormulario::Texto("Nombre", "nombre", true));
            campos->Add(CampoFormulario::Texto("Industria", "industria", true));
            campos->Add(CampoFormulario::Texto("Telefono", "telefono", true));
            campos->Add(CampoFormulario::Texto("Email", "email", true));
            campos->Add(CampoFormulario::Texto("Pais", "pais", true, "Direccion", nullptr));
            campos->Add(CampoFormulario::Texto("Ciudad", "ciudad", true, "Direccion", nullptr));
            campos->Add(CampoFormulario::Texto("Distrito", "distrito", true, "Direccion", nullptr));
            campos->Add(CampoFormulario::Texto("Calle", "calle", true, "Direccion", nullptr));

            FrmAgregarGenerico^ dlg = gcnew FrmAgregarGenerico("Nueva Cuenta", campos);
            if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) return;

            Cuenta c(gestor->getProximoIdCuenta(), Str::N(dlg->Texto("nombre")), Str::N(dlg->Texto("industria")),
                Str::N(dlg->Texto("telefono")), Str::N(dlg->Texto("email")),
                Direccion(Str::N(dlg->Texto("pais")), Str::N(dlg->Texto("ciudad")),
                    Str::N(dlg->Texto("distrito")), Str::N(dlg->Texto("calle"))));
            gestor->insertarCuenta(c);
            gestor->guardarCuentas();
            ConfigurarCuentas();
        }

        // ─── Alta de Contacto ───────────────────────────────────────────
        void AgregarContacto() {
            List<String^>^ idsCuentas = gcnew List<String^>();
            NodoD<Cuenta>* nc = gestor->getCuentas()->getCabeza();
            while (nc) { idsCuentas->Add(nc->dato.getId() + " - " + Str::M(nc->dato.getNombre())); nc = nc->siguiente; }
            if (idsCuentas->Count == 0) {
                MessageBox::Show("Primero debes registrar una Cuenta antes de agregar un Contacto.",
                    "Sin cuentas disponibles", MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }

            auto campos = gcnew List<CampoFormulario^>();
            campos->Add(CampoFormulario::Texto("Nombre", "nombre", true));
            campos->Add(CampoFormulario::Texto("Apellido", "apellido", true));
            campos->Add(CampoFormulario::Texto("Cargo", "cargo", true));
            campos->Add(CampoFormulario::Texto("Telefono", "telefono", true));
            campos->Add(CampoFormulario::Texto("Email", "email", true));
            campos->Add(CampoFormulario::ComboDinamico("Cuenta", "cuenta", idsCuentas));

            FrmAgregarGenerico^ dlg = gcnew FrmAgregarGenerico("Nuevo Contacto", campos);
            if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) return;

            Contacto c(gestor->getProximoIdContacto(), Str::N(dlg->Texto("nombre")), Str::N(dlg->Texto("apellido")),
                Str::N(dlg->Texto("cargo")), Str::N(dlg->Texto("telefono")), Str::N(dlg->Texto("email")),
                dlg->IdSeleccionado("cuenta"), Direccion("", "", "", ""));
            gestor->insertarContacto(c);
            gestor->guardarContactos();
            ConfigurarContactos();
        }

        // ─── Alta de Usuario ────────────────────────────────────────────
        void AgregarUsuario() {
            auto campos = gcnew List<CampoFormulario^>();
            campos->Add(CampoFormulario::Texto("Nombre", "nombre", true));
            campos->Add(CampoFormulario::Texto("Apellido", "apellido", true));
            campos->Add(CampoFormulario::Combo("Rol", "rol", gcnew cli::array<String^>{"Admin", "Vendedor", "Soporte"}));
            campos->Add(CampoFormulario::Texto("Username", "username", true));
            campos->Add(CampoFormulario::Texto("Password", "password", true));

            FrmAgregarGenerico^ dlg = gcnew FrmAgregarGenerico("Nuevo Usuario CRM", campos);
            if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) return;

            UsuarioCRM u(gestor->getProximoIdUsuario(), Str::N(dlg->Texto("nombre")), Str::N(dlg->Texto("apellido")),
                Str::N(dlg->Texto("rol")), Str::N(dlg->Texto("username")), Str::N(dlg->Texto("password")));
            gestor->insertarUsuario(u);
            gestor->guardarUsuarios();
            ConfigurarUsuarios();
        }

        // ─── Alta de Interaccion ────────────────────────────────────────
        void AgregarInteraccion() {
            List<String^>^ idsContactos = gcnew List<String^>();
            NodoD<Contacto>* ncon = gestor->getContactos()->getCabeza();
            while (ncon) {
                idsContactos->Add(ncon->dato.getId() + " - " + Str::M(ncon->dato.getNombre()) + " " + Str::M(ncon->dato.getApellido()));
                ncon = ncon->siguiente;
            }
            List<String^>^ idsUsuarios = gcnew List<String^>();
            NodoD<UsuarioCRM>* nu = gestor->getUsuarios()->getCabeza();
            while (nu) {
                idsUsuarios->Add(nu->dato.getId() + " - " + Str::M(nu->dato.getNombre()) + " " + Str::M(nu->dato.getApellido()));
                nu = nu->siguiente;
            }
            if (idsContactos->Count == 0 || idsUsuarios->Count == 0) {
                MessageBox::Show("Se necesita al menos un Contacto y un Usuario CRM antes de registrar una Interaccion.",
                    "Datos insuficientes", MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }

            auto campos = gcnew List<CampoFormulario^>();
            campos->Add(CampoFormulario::Combo("Tipo", "tipo", gcnew cli::array<String^>{"Llamada", "Email", "Reunion"}));
            campos->Add(CampoFormulario::Texto("Descripcion", "descripcion", false));
            campos->Add(CampoFormulario::Fecha("Fecha", "fecha"));
            campos->Add(CampoFormulario::ComboDinamico("Contacto", "contacto", idsContactos));
            campos->Add(CampoFormulario::ComboDinamico("Usuario", "usuario", idsUsuarios));

            FrmAgregarGenerico^ dlg = gcnew FrmAgregarGenerico("Nueva Interaccion", campos);
            if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) return;

            Interaccion i(gestor->getProximoIdInteraccion(), Str::N(dlg->Texto("tipo")), Str::N(dlg->Texto("descripcion")),
                Str::N(dlg->FechaFormateada("fecha", "dd/MM/yyyy")), dlg->IdSeleccionado("contacto"), dlg->IdSeleccionado("usuario"));
            gestor->insertarInteraccion(i);
            gestor->guardarInteracciones();
            ConfigurarInteracciones();
        }

        // ====== Navegación mini-sidebar ======
        void navCuentas_Click(Object^, EventArgs^) { btnNavActivo = btnNavCuentas;       CambiarVista(Vista::Cuentas, btnNavCuentas); }
        void navContactos_Click(Object^, EventArgs^) { btnNavActivo = btnNavContactos;     CambiarVista(Vista::Contactos, btnNavContactos); }
        void navUsuarios_Click(Object^, EventArgs^) { btnNavActivo = btnNavUsuarios;      CambiarVista(Vista::Usuarios, btnNavUsuarios); }
        void navInteracciones_Click(Object^, EventArgs^) { btnNavActivo = btnNavInteracciones; CambiarVista(Vista::Interacciones, btnNavInteracciones); }
        void navHash_Click(Object^, EventArgs^) { btnNavActivo = btnNavHash;          CambiarVista(Vista::HashBusqueda, btnNavHash); }
    private: System::Void InitializeComponent() {
        this->SuspendLayout();
        // 
        // FrmClientes
        // 
        this->ClientSize = System::Drawing::Size(284, 261);
        this->Name = L"FrmClientes";
        this->ResumeLayout(false);
    }
};
}