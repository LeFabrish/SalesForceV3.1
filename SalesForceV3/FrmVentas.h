#pragma once

#pragma managed(push, off)
#include "GestorVenta.h"
#pragma managed(pop)

#include "MashallHelper.h"
#include "Frmagregargenerico.h"

namespace SalesForceV3 {
    using namespace System::Collections::Generic;
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Diagnostics;
    using namespace cli;


    public ref class FrmVentas : public Form {
    private:
        GestorVenta* gestor;

        Panel^ pnlMiniSidebar;
        Panel^ pnlContenido;
        Label^ lblTitulo;
        Panel^ pnlGrid;
        DataGridView^ dgvDatos;
        Panel^ pnlBotones;
        Button^ btnAccion1, ^ btnAccion2, ^ btnAccion3;
        Panel^ pnlDataset;
        DataGridView^ dgvDataset;
        Label^ lblTiempos;

        Button^ btnNavClientes, ^ btnNavOportunidades, ^ btnNavProductos,
            ^ btnNavCotizaciones, ^ btnNavContratos, ^ btnNavDataset, ^ btnNavHito2;
        Button^ btnNavActivo;

        enum class Vista { Clientes, Oportunidades, Productos, Cotizaciones, Contratos, Dataset, Hito2 };
        Vista vistaActual;

    public:
        FrmVentas(GestorVenta* g) : gestor(g) {
            CrearUI();
            CambiarVista(Vista::Oportunidades, btnNavOportunidades);
        }

    protected:
        ~FrmVentas() {}

    private:
        void CrearUI() {
            this->Text = "Embudo de Ventas";
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
            lbl->Text = "VENTAS"; lbl->Font = gcnew Drawing::Font("Segoe UI Semibold", 9.5f);
            lbl->ForeColor = Color::FromArgb(180, 215, 255);
            lbl->Dock = DockStyle::Top; lbl->Height = 38;
            lbl->TextAlign = ContentAlignment::MiddleCenter;

            // Orden inverso
            btnNavHito2 = CrearNavBtn("  Arbol BST/AVL [Hito 2]");
            btnNavDataset = CrearNavBtn("  Dataset y Ordenamiento");
            btnNavContratos = CrearNavBtn("  Contratos");
            btnNavCotizaciones = CrearNavBtn("  Cotizaciones");
            btnNavProductos = CrearNavBtn("  Catalogo de Productos");
            btnNavOportunidades = CrearNavBtn("  Oportunidades");
            btnNavClientes = CrearNavBtn("  Clientes Potenciales");

            btnNavHito2->Enabled = false;
            btnNavHito2->ForeColor = Color::FromArgb(130, 165, 200);
            btnNavHito2->Font = gcnew Drawing::Font("Segoe UI", 8.5f, FontStyle::Italic);

            btnNavClientes->Click += gcnew EventHandler(this, &FrmVentas::navClientes_Click);
            btnNavOportunidades->Click += gcnew EventHandler(this, &FrmVentas::navOportunidades_Click);
            btnNavProductos->Click += gcnew EventHandler(this, &FrmVentas::navProductos_Click);
            btnNavCotizaciones->Click += gcnew EventHandler(this, &FrmVentas::navCotizaciones_Click);
            btnNavContratos->Click += gcnew EventHandler(this, &FrmVentas::navContratos_Click);
            btnNavDataset->Click += gcnew EventHandler(this, &FrmVentas::navDataset_Click);

            pnlMiniSidebar->Controls->Add(btnNavHito2);
            pnlMiniSidebar->Controls->Add(btnNavDataset);
            pnlMiniSidebar->Controls->Add(btnNavContratos);
            pnlMiniSidebar->Controls->Add(btnNavCotizaciones);
            pnlMiniSidebar->Controls->Add(btnNavProductos);
            pnlMiniSidebar->Controls->Add(btnNavOportunidades);
            pnlMiniSidebar->Controls->Add(btnNavClientes);
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
            CrearPanelDataset();

            pnlContenido->Controls->Add(pnlGrid);
            pnlContenido->Controls->Add(pnlDataset);
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
            btnAccion3 = EstiloCRM::CrearBotonSecundario("  Guardar");
            btnAccion1->Width = 120; btnAccion2->Width = 110; btnAccion3->Width = 110;
            btnAccion1->Location = Point(8, 8);
            btnAccion2->Location = Point(138, 8);
            btnAccion3->Location = Point(258, 8);

            btnAccion1->Click += gcnew EventHandler(this, &FrmVentas::accion1_Click);
            btnAccion2->Click += gcnew EventHandler(this, &FrmVentas::accion2_Click);
            btnAccion3->Click += gcnew EventHandler(this, &FrmVentas::accion3_Click);

            pnlBotones->Controls->Add(btnAccion1);
            pnlBotones->Controls->Add(btnAccion2);
            pnlBotones->Controls->Add(btnAccion3);
            pnlGrid->Controls->Add(dgvDatos);
            pnlGrid->Controls->Add(pnlBotones);
        }

        void CrearPanelDataset() {
            pnlDataset = gcnew Panel(); pnlDataset->Dock = DockStyle::Fill;
            pnlDataset->BackColor = Color::White; pnlDataset->Visible = false;

            Panel^ pnlBotonesSort = gcnew Panel();
            pnlBotonesSort->Dock = DockStyle::Top; pnlBotonesSort->Height = 55;
            pnlBotonesSort->BackColor = Color::White; pnlBotonesSort->Padding = System::Windows::Forms::Padding(8);

            Button^ btnQS = EstiloCRM::CrearBotonPrimario("  QuickSort");
            Button^ btnMS = EstiloCRM::CrearBotonPrimario("  MergeSort");
            Button^ btnHS = EstiloCRM::CrearBotonPrimario("  HeapSort");
            Button^ btnGen = EstiloCRM::CrearBotonSecundario("  Regenerar");
            btnQS->Width = 120; btnMS->Width = 120; btnHS->Width = 120; btnGen->Width = 120;
            btnQS->Location = Point(8, 10);
            btnMS->Location = Point(138, 10);
            btnHS->Location = Point(268, 10);
            btnGen->Location = Point(398, 10);

            btnQS->Click += gcnew EventHandler(this, &FrmVentas::quickSort_Click);
            btnMS->Click += gcnew EventHandler(this, &FrmVentas::mergeSort_Click);
            btnHS->Click += gcnew EventHandler(this, &FrmVentas::heapSort_Click);
            btnGen->Click += gcnew EventHandler(this, &FrmVentas::genDataset_Click);

            pnlBotonesSort->Controls->Add(btnQS);
            pnlBotonesSort->Controls->Add(btnMS);
            pnlBotonesSort->Controls->Add(btnHS);
            pnlBotonesSort->Controls->Add(btnGen);

            lblTiempos = gcnew Label();
            lblTiempos->Dock = DockStyle::Bottom; lblTiempos->Height = 60;
            lblTiempos->Font = gcnew Drawing::Font("Segoe UI", 9.5f);
            lblTiempos->ForeColor = Color::FromArgb(30, 70, 120);
            lblTiempos->Padding = System::Windows::Forms::Padding(10, 5, 0, 0);
            lblTiempos->Text = "Presione QuickSort, MergeSort o HeapSort para ordenar los 1000 registros.";

            dgvDataset = gcnew DataGridView(); dgvDataset->Dock = DockStyle::Fill;
            EstiloCRM::AplicarDGV(dgvDataset);
            dgvDataset->Columns->Add("N", "#");
            dgvDataset->Columns->Add("Precio", "Precio (S/.)");
            dgvDataset->Columns["N"]->Width = 60;
            dgvDataset->Columns["Precio"]->FillWeight = 1;

            pnlDataset->Controls->Add(dgvDataset);
            pnlDataset->Controls->Add(lblTiempos);
            pnlDataset->Controls->Add(pnlBotonesSort);
        }

        // ═══════════════════════════════════════════════════════════
        //  LÓGICA DE VISTAS
        // ═══════════════════════════════════════════════════════════
        void CambiarVista(Vista v, Button^ boton) {
            vistaActual = v;
            HighlightNav(boton);
            pnlGrid->Visible = (v != Vista::Dataset);
            pnlDataset->Visible = (v == Vista::Dataset);

            switch (v) {
            case Vista::Clientes:      ConfigurarClientes();      break;
            case Vista::Oportunidades: ConfigurarOportunidades(); break;
            case Vista::Productos:     ConfigurarProductos();     break;
            case Vista::Cotizaciones:  ConfigurarCotizaciones();  break;
            case Vista::Contratos:     ConfigurarContratos();     break;
            case Vista::Dataset:       ConfigurarDataset();       break;
            }
        }

        void HighlightNav(Button^ b) {
            cli::array<Button^>^ navs = gcnew cli::array<Button^>(6);
            navs[0] = btnNavClientes; navs[1] = btnNavOportunidades; navs[2] = btnNavProductos;
            navs[3] = btnNavCotizaciones; navs[4] = btnNavContratos; navs[5] = btnNavDataset;
            for each (Button ^ n in navs) n->BackColor = Color::FromArgb(0, 85, 170);
            if (b->Enabled) b->BackColor = Color::FromArgb(0, 55, 120);
        }

        void ConfigurarClientes() {
            lblTitulo->Text = "Clientes Potenciales  (Cola FIFO — " +
                gestor->getColaClientes()->getTamanio() + " en espera)";
            cli::array<String^>^ cols = gcnew cli::array<String^>(4);
            cols[0] = "ID"; cols[1] = "Nombre"; cols[2] = "Correo"; cols[3] = "Interes";
            ConfigurarColumnas(cols);
            btnAccion1->Text = "  + Encolar";
            btnAccion2->Text = "  Atender";
            btnAccion3->Text = "  Actualizar";
            dgvDatos->Rows->Clear();
            NodoS<Cliente_Potencial>* n = gestor->getColaClientes()->getFrenteNodo();
            while (n) {
                Cliente_Potencial& c = n->dato;
                dgvDatos->Rows->Add(c.getId(), Str::M(c.getNombre()),
                    Str::M(c.getCorreo()), Str::M(c.getInteres()));
                n = n->siguiente;
            }
        }

        void ConfigurarOportunidades() {
            lblTitulo->Text = "Oportunidades  (" + gestor->getOportunidades()->getTamanio()
                + " registros  |  Pipeline: S/. " +
                gestor->totalPipeline().ToString("N0") + ")";
            cli::array<String^>^ cols = gcnew cli::array<String^>(4);
            cols[0] = "ID"; cols[1] = "Titulo"; cols[2] = "Valor Esperado (S/.)"; cols[3] = "Fase";
            ConfigurarColumnas(cols);
            btnAccion1->Text = "  + Agregar"; btnAccion2->Text = "  Eliminar"; btnAccion3->Text = "  Guardar";
            dgvDatos->Rows->Clear();
            NodoD<Oportunidad>* n = gestor->getOportunidades()->getCabeza();
            while (n) {
                Oportunidad& o = n->dato;
                dgvDatos->Rows->Add(o.getId(), Str::M(o.getTitulo()),
                    o.getValorEsperado().ToString("N2"), Str::M(o.getFase()));
                n = n->siguiente;
            }
        }

        void ConfigurarProductos() {
            lblTitulo->Text = "Catalogo de Productos  (" + gestor->getProductos()->getTamanio() + " registros)";
            cli::array<String^>^ cols = gcnew cli::array<String^>(4);
            cols[0] = "ID"; cols[1] = "Nombre"; cols[2] = "Precio (S/.)"; cols[3] = "Categoria";
            ConfigurarColumnas(cols);
            btnAccion1->Text = "  + Agregar"; btnAccion2->Text = "  Eliminar"; btnAccion3->Text = "  Guardar";
            dgvDatos->Rows->Clear();
            NodoS<Producto>* n = gestor->getProductos()->getCabeza();
            while (n) {
                Producto& p = n->dato;
                dgvDatos->Rows->Add(p.getId(), Str::M(p.getNombre()),
                    p.getPrecio().ToString("N2"), Str::M(p.getCategoria()));
                n = n->siguiente;
            }
        }

        void ConfigurarCotizaciones() {
            lblTitulo->Text = "Cotizaciones  (" + gestor->getCotizaciones()->getTamanio() + " registros)";
            cli::array<String^>^ cols = gcnew cli::array<String^>(4);
            cols[0] = "ID"; cols[1] = "Total (S/.)"; cols[2] = "Vencimiento"; cols[3] = "Estado";
            ConfigurarColumnas(cols);
            btnAccion1->Text = "  + Agregar"; btnAccion2->Text = "  Eliminar"; btnAccion3->Text = "  Guardar";
            dgvDatos->Rows->Clear();
            NodoS<Cotizacion>* n = gestor->getCotizaciones()->getCabeza();
            while (n) {
                Cotizacion& c = n->dato;
                dgvDatos->Rows->Add(c.getId(), c.getTotal().ToString("N2"),
                    Str::M(c.getFechaVencimiento()), Str::M(c.getEstado()));
                n = n->siguiente;
            }
        }

        void ConfigurarContratos() {
            lblTitulo->Text = "Contratos  (" + gestor->getContratos()->getTamanio() + " registros)";
            cli::array<String^>^ cols = gcnew cli::array<String^>(4);
            cols[0] = "ID"; cols[1] = "Fecha Firma"; cols[2] = "Terminos"; cols[3] = "Monto (S/.)";
            ConfigurarColumnas(cols);
            btnAccion1->Text = "  + Agregar"; btnAccion2->Text = "  Eliminar"; btnAccion3->Text = "  Guardar";
            dgvDatos->Rows->Clear();
            NodoS<Contrato>* n = gestor->getContratos()->getCabeza();
            while (n) {
                Contrato& c = n->dato;
                dgvDatos->Rows->Add(c.getId(), Str::M(c.getFechaFirma()),
                    Str::M(c.getTerminos()), c.getMontoTotal().ToString("N2"));
                n = n->siguiente;
            }
        }

        void ConfigurarDataset() {
            lblTitulo->Text = "Dataset y Ordenamiento  (1000 precios — QuickSort / MergeSort / HeapSort)";
            CargarDatasetEnDGV(gestor->getDataSet());
        }

        void ConfigurarColumnas(cli::array<String^>^ cols) {
            dgvDatos->Columns->Clear();
            for each (String ^ c in cols) dgvDatos->Columns->Add(c, c);
            dgvDatos->Columns[0]->ReadOnly = true;
            dgvDatos->Columns[0]->Width = 45;
        }

        void CargarDatasetEnDGV(const std::vector<double>& datos) {
            dgvDataset->Rows->Clear();
            int i = 1;
            for (double v : datos)
                dgvDataset->Rows->Add(i++, v.ToString("F2"));
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
        // AHORA
        void accion1_Click(Object^, EventArgs^) {
            switch (vistaActual) {
            case Vista::Clientes:      AgregarClientePotencial(); break;
            case Vista::Oportunidades: AgregarOportunidad();      break;
            case Vista::Productos:     AgregarProducto();         break;
            case Vista::Cotizaciones:  AgregarCotizacion();       break;
            case Vista::Contratos:     AgregarContrato();         break;
            }
        }
        void accion2_Click(Object^, EventArgs^) {
            dgvDatos->EndEdit();
            if (vistaActual == Vista::Clientes) {
                gestor->atenderCliente();
                gestor->guardarClientes();
                ConfigurarClientes();
                return;
            }
            if (dgvDatos->SelectedRows->Count == 0) return;
            if (MessageBox::Show("¿Eliminar la fila seleccionada?", "Confirmar",
                MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes)
                dgvDatos->Rows->Remove(dgvDatos->SelectedRows[0]);
        }

        void accion3_Click(Object^, EventArgs^) {
            GuardarVistaActual();
            MessageBox::Show("Datos guardados.", "OK", MessageBoxButtons::OK, MessageBoxIcon::Information);
            CambiarVista(vistaActual, btnNavActivo != nullptr ? btnNavActivo : btnNavOportunidades);
        }

        void GuardarVistaActual() {
            if (vistaActual == Vista::Oportunidades) {
                gestor->limpiarOportunidades();
                for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                    if (r->IsNewRow || Str::Celda(r->Cells[1]) == "") continue;
                    Oportunidad o(Str::CeldaInt(r->Cells[0]), Str::N(Str::Celda(r->Cells[1])),
                        Str::CeldaDbl(r->Cells[2]), Str::N(Str::Celda(r->Cells[3])));
                    gestor->insertarOportunidad(o);
                }
                gestor->guardarOportunidades();
            }
            else if (vistaActual == Vista::Productos) {
                gestor->limpiarProductos();
                for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                    if (r->IsNewRow || Str::Celda(r->Cells[1]) == "") continue;
                    Producto p(Str::CeldaInt(r->Cells[0]), Str::N(Str::Celda(r->Cells[1])),
                        Str::CeldaDbl(r->Cells[2]), Str::N(Str::Celda(r->Cells[3])));
                    gestor->insertarProducto(p);
                }
                gestor->guardarProductos();
            }
            else if (vistaActual == Vista::Cotizaciones) {
                // Antes esta vista no tenia rama aqui: los cambios se veian en la
                // grilla pero se perdian al volver a cargarla (ni siquiera llegaban
                // a la lista en memoria).
                gestor->limpiarCotizaciones();
                for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                    if (r->IsNewRow || Str::Celda(r->Cells[1]) == "") continue;
                    Cotizacion c(Str::CeldaInt(r->Cells[0]), Str::CeldaDbl(r->Cells[1]),
                        Str::N(Str::Celda(r->Cells[2])), Str::N(Str::Celda(r->Cells[3])));
                    gestor->insertarCotizacion(c);
                }
                gestor->guardarCotizaciones();
            }
            else if (vistaActual == Vista::Contratos) {
                gestor->limpiarContratos();
                for each (DataGridViewRow ^ r in dgvDatos->Rows) {
                    if (r->IsNewRow || Str::Celda(r->Cells[1]) == "") continue;
                    Contrato c(Str::CeldaInt(r->Cells[0]), Str::N(Str::Celda(r->Cells[1])),
                        Str::N(Str::Celda(r->Cells[2])), Str::CeldaDbl(r->Cells[3]));
                    gestor->insertarContrato(c);
                }
                gestor->guardarContratos();
            }
        }

        // ─── Dataset y Ordenamiento ────────────────────────────────
        void quickSort_Click(Object^, EventArgs^) {
            Stopwatch^ sw = gcnew Stopwatch(); sw->Start();
            std::vector<double> sorted = gestor->ordenarConQuickSort();
            sw->Stop();
            CargarDatasetEnDGV(sorted);
            lblTiempos->Text = "QuickSort  →  " + sw->ElapsedMilliseconds + " ms  |  "
                + sw->ElapsedTicks + " ticks  (O(n log n) promedio)";
        }

        void mergeSort_Click(Object^, EventArgs^) {
            Stopwatch^ sw = gcnew Stopwatch(); sw->Start();
            std::vector<double> sorted = gestor->ordenarConMergeSort();
            sw->Stop();
            CargarDatasetEnDGV(sorted);
            lblTiempos->Text = "MergeSort  →  " + sw->ElapsedMilliseconds + " ms  |  "
                + sw->ElapsedTicks + " ticks  (O(n log n) garantizado, estable)";
        }

        void heapSort_Click(Object^, EventArgs^) {
            Stopwatch^ sw = gcnew Stopwatch(); sw->Start();
            std::vector<double> sorted = gestor->ordenarConHeapSort();
            sw->Stop();
            CargarDatasetEnDGV(sorted);
            lblTiempos->Text = "HeapSort   →  " + sw->ElapsedMilliseconds + " ms  |  "
                + sw->ElapsedTicks + " ticks  (O(n log n) garantizado, in-place)";
        }

        void genDataset_Click(Object^, EventArgs^) {
            gestor->generarDataSet();
            CargarDatasetEnDGV(gestor->getDataSet());
            lblTiempos->Text = "Dataset regenerado (1000 valores aleatorios con semilla fija 42).";
        }
        // ====== Metodos Privados =====
        // ─── Alta de Cliente Potencial (Cola FIFO) ─────────────────────
        void AgregarClientePotencial() {
            auto campos = gcnew List<CampoFormulario^>();
            campos->Add(CampoFormulario::Texto("Nombre", "nombre", true));
            campos->Add(CampoFormulario::Texto("Correo", "correo", true));
            campos->Add(CampoFormulario::Texto("Interes", "interes", true));

            FrmAgregarGenerico^ dlg = gcnew FrmAgregarGenerico("Nuevo Cliente Potencial", campos);
            if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) return;

            Cliente_Potencial c(gestor->getProximoIdCliente(), Str::N(dlg->Texto("nombre")),
                Str::N(dlg->Texto("correo")), Str::N(dlg->Texto("interes")));
            gestor->encolarCliente(c);
            gestor->guardarClientes(); // esta cola no tiene boton "Guardar" propio
            ConfigurarClientes();
        }

        // ─── Alta de Oportunidad ────────────────────────────────────────
        void AgregarOportunidad() {
            auto campos = gcnew List<CampoFormulario^>();
            campos->Add(CampoFormulario::Texto("Titulo", "titulo", true));
            campos->Add(CampoFormulario::Numero("Valor Esperado (S/.)", "valorEsperado"));
            campos->Add(CampoFormulario::Combo("Fase", "fase", gcnew cli::array<String^>{"Prospeccion", "Negociacion", "Cerrado"}));

            FrmAgregarGenerico^ dlg = gcnew FrmAgregarGenerico("Nueva Oportunidad", campos);
            if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) return;

            Oportunidad o(gestor->getProximoIdOportunidad(), Str::N(dlg->Texto("titulo")),
                dlg->Numero("valorEsperado"), Str::N(dlg->Texto("fase")));
            gestor->insertarOportunidad(o);
            gestor->guardarOportunidades();
            ConfigurarOportunidades();
        }

        // ─── Alta de Producto ───────────────────────────────────────────
        void AgregarProducto() {
            auto campos = gcnew List<CampoFormulario^>();
            campos->Add(CampoFormulario::Texto("Nombre", "nombre", true));
            campos->Add(CampoFormulario::Numero("Precio (S/.)", "precio"));
            campos->Add(CampoFormulario::Texto("Categoria", "categoria", true));

            FrmAgregarGenerico^ dlg = gcnew FrmAgregarGenerico("Nuevo Producto", campos);
            if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) return;

            Producto p(gestor->getProximoIdProducto(), Str::N(dlg->Texto("nombre")),
                dlg->Numero("precio"), Str::N(dlg->Texto("categoria")));
            gestor->insertarProducto(p);
            gestor->guardarProductos();
            ConfigurarProductos();
        }

        // ─── Alta de Cotizacion ─────────────────────────────────────────
        void AgregarCotizacion() {
            auto campos = gcnew List<CampoFormulario^>();
            campos->Add(CampoFormulario::Numero("Total (S/.)", "total"));
            campos->Add(CampoFormulario::Fecha("Fecha de Vencimiento", "vencimiento"));
            campos->Add(CampoFormulario::Combo("Estado", "estado", gcnew cli::array<String^>{"Pendiente", "Aceptada", "Rechazada"}));

            FrmAgregarGenerico^ dlg = gcnew FrmAgregarGenerico("Nueva Cotizacion", campos);
            if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) return;

            Cotizacion c(gestor->getProximoIdCotizacion(), dlg->Numero("total"),
                Str::N(dlg->FechaFormateada("vencimiento", "dd/MM/yyyy")), Str::N(dlg->Texto("estado")));
            gestor->insertarCotizacion(c);
            gestor->guardarCotizaciones();
            ConfigurarCotizaciones();
        }

        // ─── Alta de Contrato ───────────────────────────────────────────
        void AgregarContrato() {
            auto campos = gcnew List<CampoFormulario^>();
            campos->Add(CampoFormulario::Fecha("Fecha de Firma", "firma"));
            campos->Add(CampoFormulario::Texto("Terminos", "terminos", true));
            campos->Add(CampoFormulario::Numero("Monto Total (S/.)", "monto"));

            FrmAgregarGenerico^ dlg = gcnew FrmAgregarGenerico("Nuevo Contrato", campos);
            if (dlg->ShowDialog(this) != System::Windows::Forms::DialogResult::OK) return;

            Contrato c(gestor->getProximoIdContrato(), Str::N(dlg->FechaFormateada("firma", "dd/MM/yyyy")),
                Str::N(dlg->Texto("terminos")), dlg->Numero("monto"));
            gestor->insertarContrato(c);
            gestor->guardarContratos();
            ConfigurarContratos();
        }

        void navClientes_Click(Object^, EventArgs^) { btnNavActivo = btnNavClientes;      CambiarVista(Vista::Clientes, btnNavClientes); }
        void navOportunidades_Click(Object^, EventArgs^) { btnNavActivo = btnNavOportunidades; CambiarVista(Vista::Oportunidades, btnNavOportunidades); }
        void navProductos_Click(Object^, EventArgs^) { btnNavActivo = btnNavProductos;     CambiarVista(Vista::Productos, btnNavProductos); }
        void navCotizaciones_Click(Object^, EventArgs^) { btnNavActivo = btnNavCotizaciones;  CambiarVista(Vista::Cotizaciones, btnNavCotizaciones); }
        void navContratos_Click(Object^, EventArgs^) { btnNavActivo = btnNavContratos;     CambiarVista(Vista::Contratos, btnNavContratos); }
        void navDataset_Click(Object^, EventArgs^) { btnNavActivo = btnNavDataset;       CambiarVista(Vista::Dataset, btnNavDataset); }
    };
}