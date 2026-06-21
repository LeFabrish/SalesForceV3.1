#pragma once

#pragma managed(push, off)
#include "GestorSoporte.h"
#pragma managed(pop)

#include "MashallHelper.h"

namespace SalesForceV3 {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace cli;

    public ref class FrmSoporte : public Form {
    private:
        GestorSoporte* gestor;

        Panel^ pnlMiniSidebar;
        Panel^ pnlContenido;
        Label^ lblTitulo;
        Panel^ pnlGrid;
        DataGridView^ dgvDatos;
        Panel^ pnlBotones;
        Button^ btnAccion1, ^ btnAccion2, ^ btnAccion3;
        Panel^ pnlHito2;

        Button^ btnNavCasos, ^ btnNavSoluciones, ^ btnNavTareas,
            ^ btnNavEventos, ^ btnNavHistorial, ^ btnNavGrafo;
        Button^ btnNavActivo;

        enum class Vista { Casos, Soluciones, Tareas, Eventos, Historial, Grafo };
        Vista vistaActual;

    public:
        FrmSoporte(GestorSoporte* g) : gestor(g) {
            CrearUI();
            CambiarVista(Vista::Casos, btnNavCasos);
        }

    protected:
        ~FrmSoporte() {}

    private:
        void CrearUI() {
            this->Text = "Operaciones y Soporte";
            this->BackColor = EstiloCRM::GrisClaro();
            CrearAreaContenido();
            CrearMiniSidebar();
        }

        void CrearMiniSidebar() {
            pnlMiniSidebar = gcnew Panel();
            pnlMiniSidebar->Dock = DockStyle::Left; pnlMiniSidebar->Width = 175;
            pnlMiniSidebar->BackColor = Color::FromArgb(0, 85, 170);

            Label^ lbl = gcnew Label();
            lbl->Text = "SOPORTE"; lbl->Font = gcnew Drawing::Font("Segoe UI Semibold", 9.5f);
            lbl->ForeColor = Color::FromArgb(180, 215, 255);
            lbl->Dock = DockStyle::Top; lbl->Height = 38;
            lbl->TextAlign = ContentAlignment::MiddleCenter;

            // Orden inverso
            btnNavGrafo = CrearNavBtn("  Grafo [Hito 2]");
            btnNavHistorial = CrearNavBtn("  Historial  [Pila]");
            btnNavEventos = CrearNavBtn("  Eventos");
            btnNavTareas = CrearNavBtn("  Tareas  [Cola]");
            btnNavSoluciones = CrearNavBtn("  Soluciones  [Pila]");
            btnNavCasos = CrearNavBtn("  Casos  [Cola]");

            btnNavGrafo->Enabled = false;
            btnNavGrafo->ForeColor = Color::FromArgb(130, 165, 200);
            btnNavGrafo->Font = gcnew Drawing::Font("Segoe UI", 8.5f, FontStyle::Italic);

            btnNavCasos->Click += gcnew EventHandler(this, &FrmSoporte::navCasos_Click);
            btnNavSoluciones->Click += gcnew EventHandler(this, &FrmSoporte::navSoluciones_Click);
            btnNavTareas->Click += gcnew EventHandler(this, &FrmSoporte::navTareas_Click);
            btnNavEventos->Click += gcnew EventHandler(this, &FrmSoporte::navEventos_Click);
            btnNavHistorial->Click += gcnew EventHandler(this, &FrmSoporte::navHistorial_Click);

            pnlMiniSidebar->Controls->Add(btnNavGrafo);
            pnlMiniSidebar->Controls->Add(btnNavHistorial);
            pnlMiniSidebar->Controls->Add(btnNavEventos);
            pnlMiniSidebar->Controls->Add(btnNavTareas);
            pnlMiniSidebar->Controls->Add(btnNavSoluciones);
            pnlMiniSidebar->Controls->Add(btnNavCasos);
            pnlMiniSidebar->Controls->Add(lbl);
            this->Controls->Add(pnlMiniSidebar);
        }

        Button^ CrearNavBtn(String^ texto) {
            Button^ b = gcnew Button();
            b->Text = texto; b->Dock = DockStyle::Top; b->Height = 42;
            b->FlatStyle = FlatStyle::Flat; b->FlatAppearance->BorderSize = 0;
            b->FlatAppearance->MouseOverBackColor = Color::FromArgb(0, 65, 140);
            b->BackColor = Color::FromArgb(0, 85, 170); b->ForeColor = Color::White;
            b->Font = gcnew Drawing::Font("Segoe UI", 9.5f);
            b->TextAlign = ContentAlignment::MiddleLeft;
            b->Padding = System::Windows::Forms::Padding(10, 0, 0, 0); b->Cursor = Cursors::Hand;    
            return b;
        }

        void CrearAreaContenido() {
            pnlContenido = gcnew Panel();
            pnlContenido->Dock = DockStyle::Fill;
            pnlContenido->BackColor = EstiloCRM::GrisClaro();
            pnlContenido->Padding = System::Windows::Forms::Padding(12, 10, 12, 10);

            lblTitulo = gcnew Label();
            lblTitulo->Dock = DockStyle::Top; lblTitulo->Height = 38;
            lblTitulo->Font = gcnew Drawing::Font("Segoe UI Semibold", 12.0f);
            lblTitulo->ForeColor = Color::FromArgb(20, 50, 90);
            lblTitulo->BackColor = Color::Transparent;

            CrearPanelGrid();
            CrearPanelHito2();

            pnlContenido->Controls->Add(pnlGrid);
            pnlContenido->Controls->Add(pnlHito2);
            pnlContenido->Controls->Add(lblTitulo);
            this->Controls->Add(pnlContenido);
        }

        void CrearPanelGrid() {
            pnlGrid = gcnew Panel(); pnlGrid->Dock = DockStyle::Fill;
            dgvDatos = gcnew DataGridView(); dgvDatos->Dock = DockStyle::Fill;
            EstiloCRM::AplicarDGV(dgvDatos);

            pnlBotones = gcnew Panel();
            pnlBotones->Dock = DockStyle::Bottom; pnlBotones->Height = 50;
            pnlBotones->BackColor = Color::White; pnlBotones->Padding = System::Windows::Forms::Padding(8, 8, 8, 8);

            btnAccion1 = EstiloCRM::CrearBotonPrimario("  + Agregar");
            btnAccion2 = EstiloCRM::CrearBotonPeligro("  Eliminar");
            btnAccion3 = EstiloCRM::CrearBotonSecundario("  Actualizar");
            btnAccion1->Width = 130; btnAccion2->Width = 130; btnAccion3->Width = 120;
            btnAccion1->Location = Point(8, 8);
            btnAccion2->Location = Point(148, 8);
            btnAccion3->Location = Point(288, 8);

            btnAccion1->Click += gcnew EventHandler(this, &FrmSoporte::accion1_Click);
            btnAccion2->Click += gcnew EventHandler(this, &FrmSoporte::accion2_Click);
            btnAccion3->Click += gcnew EventHandler(this, &FrmSoporte::accion3_Click);

            pnlBotones->Controls->Add(btnAccion1);
            pnlBotones->Controls->Add(btnAccion2);
            pnlBotones->Controls->Add(btnAccion3);
            pnlGrid->Controls->Add(dgvDatos);
            pnlGrid->Controls->Add(pnlBotones);
        }

        void CrearPanelHito2() {
            pnlHito2 = gcnew Panel(); pnlHito2->Dock = DockStyle::Fill;
            pnlHito2->BackColor = Color::White; pnlHito2->Visible = false;

            Label^ lbl = gcnew Label();
            lbl->Text = "Grafo de Relaciones entre Cuentas\r\n\r\n"
                "Esta funcionalidad estara disponible en el Hito 2.\r\n\r\n"
                "Se implementara:\r\n"
                "  - Grafo<string> con lista de adyacencia\r\n"
                "  - BFS (Breadth-First Search)\r\n"
                "  - DFS (Depth-First Search)\r\n"
                "  - Visualizacion de red de relaciones entre cuentas";
            lbl->Font = gcnew Drawing::Font("Segoe UI", 11.0f);
            lbl->ForeColor = Color::FromArgb(80, 110, 150);
            lbl->AutoSize = false;
            lbl->Size = Drawing::Size(550, 250);
            lbl->Location = Point(40, 60);
            pnlHito2->Controls->Add(lbl);
        }

        // ═══════════════════════════════════════════════════════════
        //  LÓGICA DE VISTAS
        // ═══════════════════════════════════════════════════════════
        void CambiarVista(Vista v, Button^ boton) {
            vistaActual = v;
            HighlightNav(boton);
            pnlGrid->Visible = (v != Vista::Grafo);
            pnlHito2->Visible = (v == Vista::Grafo);

            switch (v) {
            case Vista::Casos:      ConfigurarCasos();      break;
            case Vista::Soluciones: ConfigurarSoluciones(); break;
            case Vista::Tareas:     ConfigurarTareas();     break;
            case Vista::Eventos:    ConfigurarEventos();    break;
            case Vista::Historial:  ConfigurarHistorial();  break;
            }
        }

        void HighlightNav(Button^ b) {
            cli::array<Button^>^ navs = gcnew cli::array<Button^>{ btnNavCasos, btnNavSoluciones, btnNavTareas,
                                                                     btnNavEventos, btnNavHistorial };
            for each (Button ^ n in navs) n->BackColor = Color::FromArgb(0, 85, 170);
            if (b->Enabled) b->BackColor = Color::FromArgb(0, 55, 120);
        }

        void ConfigurarColumnas(cli::array<String^>^ cols) {
            dgvDatos->Columns->Clear();
            for each (String ^ c in cols) dgvDatos->Columns->Add(c, c);
            dgvDatos->Columns[0]->ReadOnly = true;
            dgvDatos->Columns[0]->Width = 45;
        }

        void ConfigurarCasos() {
            lblTitulo->Text = "Casos  (Cola FIFO — " + gestor->getColaCasos()->getTamanio() + " pendientes)";
            ConfigurarColumnas(gcnew cli::array<String^>{"ID", "Asunto", "Estado", "Prioridad"});
            btnAccion1->Text = "  + Encolar caso";
            btnAccion2->Text = "  Atender (Dequeue)";
            btnAccion3->Text = "  Actualizar";
            dgvDatos->Rows->Clear();
            NodoS<Caso>* n = gestor->getColaCasos()->getFrenteNodo();
            while (n) {
                Caso& c = n->dato;
                dgvDatos->Rows->Add(c.getId(), Str::M(c.getAsunto()),
                    Str::M(c.getEstado()), Str::M(c.getPrioridad()));
                n = n->siguiente;
            }
        }

        void ConfigurarSoluciones() {
            lblTitulo->Text = "Soluciones  (Pila LIFO — " + gestor->getPilaSoluciones()->getTamanio() + " registros)";
            ConfigurarColumnas(gcnew cli::array<String^>{"ID", "ID Caso", "Descripcion", "Fecha Resolucion"});
            btnAccion1->Text = "  + Apilar solucion";
            btnAccion2->Text = "  Desapilar (Pop)";
            btnAccion3->Text = "  Actualizar";
            dgvDatos->Rows->Clear();
            std::vector<Solucion> v = gestor->getPilaSoluciones()->toVector();
            for (Solucion& s : v)
                dgvDatos->Rows->Add(s.getId(), s.getIdCasoAsociado(),
                    Str::M(s.getDescripcion()), Str::M(s.getFechaResolucion()));
        }

        void ConfigurarTareas() {
            lblTitulo->Text = "Tareas  (Cola FIFO — " + gestor->getColaTareas()->getTamanio() + " pendientes)";
            ConfigurarColumnas(gcnew cli::array<String^>{"ID", "Descripcion", "Estado", "Fecha Limite"});
            btnAccion1->Text = "  + Encolar tarea";
            btnAccion2->Text = "  Completar (Dequeue)";
            btnAccion3->Text = "  Actualizar";
            dgvDatos->Rows->Clear();
            NodoS<Tarea>* n = gestor->getColaTareas()->getFrenteNodo();
            while (n) {
                Tarea& t = n->dato;
                dgvDatos->Rows->Add(t.getId(), Str::M(t.getDescripcion()),
                    Str::M(t.getEstado()), Str::M(t.getFechaLimite()));
                n = n->siguiente;
            }
        }

        void ConfigurarEventos() {
            lblTitulo->Text = "Eventos  (" + gestor->getListaEventos()->getTamanio() + " registros)";
            ConfigurarColumnas(gcnew cli::array<String^>{"ID", "Titulo", "Fecha/Hora", "Ubicacion"});
            btnAccion1->Text = "  + Agregar";
            btnAccion2->Text = "  Eliminar";
            btnAccion3->Text = "  Guardar";
            dgvDatos->Rows->Clear();
            NodoS<Evento>* n = gestor->getListaEventos()->getCabeza();
            while (n) {
                Evento& e = n->dato;
                dgvDatos->Rows->Add(e.getId(), Str::M(e.getTitulo()),
                    Str::M(e.getFechaHora()), Str::M(e.getUbicacion()));
                n = n->siguiente;
            }
        }

        void ConfigurarHistorial() {
            lblTitulo->Text = "Historial  (Pila LIFO — " + gestor->getPilaHistorial()->getTamanio() + " registros)";
            ConfigurarColumnas(gcnew cli::array<String^>{"ID", "Modulo", "Accion", "Fecha"});
            btnAccion1->Text = "  + Registrar";
            btnAccion2->Text = "  Desapilar";
            btnAccion3->Text = "  Actualizar";
            dgvDatos->Rows->Clear();
            std::vector<Historial> v = gestor->getPilaHistorial()->toVector();
            for (Historial& h : v)
                dgvDatos->Rows->Add(h.getId(), Str::M(h.getModuloAsociado()),
                    Str::M(h.getAccion()), Str::M(h.getFecha()));
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
        //  EVENTOS
        // ═══════════════════════════════════════════════════════════
        void accion1_Click(Object^, EventArgs^) {
            switch (vistaActual) {
            case Vista::Casos:
                gestor->encolarCaso({ 0, "Nuevo caso", "Abierto", "Media" });
                ConfigurarCasos(); break;
            case Vista::Soluciones:
                gestor->apilarSolucion({ 0, 0, "Nueva solucion", "01/01/2025" });
                ConfigurarSoluciones(); break;
            case Vista::Tareas:
                gestor->encolarTarea({ 0, "Nueva tarea", "Pendiente", "30/12/2025" });
                ConfigurarTareas(); break;
            case Vista::Eventos:
                dgvDatos->Rows->Add(SiguienteId());
                dgvDatos->CurrentCell = dgvDatos->Rows[dgvDatos->Rows->Count - 1]->Cells[1];
                break;
            case Vista::Historial:
                gestor->apilarHistorial({ 0, "Sistema", "Accion manual", "01/01/2025" });
                ConfigurarHistorial(); break;
            }
        }

        void accion2_Click(Object^, EventArgs^) {
            switch (vistaActual) {
            case Vista::Casos:
                if (gestor->getColaCasos()->estaVacia()) {
                    MessageBox::Show("No hay casos en cola.", "Info"); return;
                }
                MessageBox::Show("Atendiendo: " + Str::M(gestor->getColaCasos()->getFrente().getAsunto()),
                    "Caso atendido", MessageBoxButtons::OK, MessageBoxIcon::Information);
                gestor->atenderCaso();
                ConfigurarCasos(); break;
            case Vista::Soluciones:
                if (gestor->getPilaSoluciones()->estaVacia()) return;
                gestor->desapilarSolucion();
                ConfigurarSoluciones(); break;
            case Vista::Tareas:
                if (gestor->getColaTareas()->estaVacia()) return;
                MessageBox::Show("Completando: " + Str::M(gestor->getColaTareas()->getFrente().getDescripcion()),
                    "Tarea completada", MessageBoxButtons::OK, MessageBoxIcon::Information);
                gestor->atenderTarea();
                ConfigurarTareas(); break;
            case Vista::Eventos:
                if (dgvDatos->SelectedRows->Count == 0) return;
                dgvDatos->Rows->Remove(dgvDatos->SelectedRows[0]); break;
            case Vista::Historial:
                if (gestor->getPilaHistorial()->estaVacia()) return;
                gestor->desapilarHistorial();
                ConfigurarHistorial(); break;
            }
        }

        void accion3_Click(Object^, EventArgs^) {
            if (vistaActual == Vista::Eventos) {
                gestor->limpiarEventos();
                for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                    if (r->IsNewRow || Str::Celda(r->Cells[1]) == "") continue;
                    Evento e(Str::CeldaInt(r->Cells[0]), Str::N(Str::Celda(r->Cells[1])),
                        Str::N(Str::Celda(r->Cells[2])), Str::N(Str::Celda(r->Cells[3])));
                    gestor->insertarEvento(e);
                }
                MessageBox::Show("Eventos guardados.", "OK", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            CambiarVista(vistaActual, btnNavActivo != nullptr ? btnNavActivo : btnNavCasos);
        }

        // ─── Navegación ───────────────────────────────────────────
        void navCasos_Click(Object^, EventArgs^) { btnNavActivo = btnNavCasos;      CambiarVista(Vista::Casos, btnNavCasos); }
        void navSoluciones_Click(Object^, EventArgs^) { btnNavActivo = btnNavSoluciones; CambiarVista(Vista::Soluciones, btnNavSoluciones); }
        void navTareas_Click(Object^, EventArgs^) { btnNavActivo = btnNavTareas;     CambiarVista(Vista::Tareas, btnNavTareas); }
        void navEventos_Click(Object^, EventArgs^) { btnNavActivo = btnNavEventos;    CambiarVista(Vista::Eventos, btnNavEventos); }
        void navHistorial_Click(Object^, EventArgs^) { btnNavActivo = btnNavHistorial;  CambiarVista(Vista::Historial, btnNavHistorial); }
    };
}