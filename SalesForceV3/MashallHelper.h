#pragma once

#include <string>

namespace SalesForceV3 {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Runtime::InteropServices;

    // Conversiones entre mundo nativo C++ y managed C++/CLI
    ref class Str sealed {
    public:
        static String^ M(const std::string& s) {
            return gcnew String(s.c_str());
        }

        static std::string N(String^ s) {
            IntPtr ptr = Marshal::StringToHGlobalAnsi(s);
            std::string result(static_cast<const char*>(ptr.ToPointer()));
            Marshal::FreeHGlobal(ptr);
            return result;
        }

        // Lectura segura de celda (devuelve "" si es null)
        static String^ Celda(DataGridViewCell^ c) {
            return (c->Value == nullptr) ? "" : c->Value->ToString()->Trim();
        }

        static int CeldaInt(DataGridViewCell^ c) {
            String^ v = Celda(c);
            return (v == "") ? 0 : Int32::Parse(v);
        }

        static double CeldaDbl(DataGridViewCell^ c) {
            String^ v = Celda(c);
            return (v == "") ? 0.0 : Double::Parse(v);
        }
    };

    // Estilos visuales del sistema Salesforce aplicados a un DataGridView
    ref class EstiloCRM sealed {
    public:
        static Color Azul() { return Color::FromArgb(21, 137, 238); }
        static Color AzulOscuro() { return Color::FromArgb(0, 112, 210); }
        static Color GrisClaro() { return Color::FromArgb(243, 245, 249); }
        static Color FilaAlterna() { return Color::FromArgb(237, 245, 255); }

        static void AplicarDGV(DataGridView^ dgv) {
            dgv->EnableHeadersVisualStyles = false;
            dgv->ColumnHeadersDefaultCellStyle->BackColor = AzulOscuro();
            dgv->ColumnHeadersDefaultCellStyle->ForeColor = Color::White;
            dgv->ColumnHeadersDefaultCellStyle->Font = gcnew Drawing::Font("Segoe UI Semibold", 9.0f);
            dgv->ColumnHeadersHeight = 34;
            dgv->RowHeadersVisible = false;
            dgv->AlternatingRowsDefaultCellStyle->BackColor = FilaAlterna();
            dgv->DefaultCellStyle->Font = gcnew Drawing::Font("Segoe UI", 9.0f);
            dgv->DefaultCellStyle->SelectionBackColor = Azul();
            dgv->DefaultCellStyle->SelectionForeColor = Color::White;
            dgv->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgv->MultiSelect = false;
            dgv->AllowUserToAddRows = false;
            dgv->AllowUserToDeleteRows = false;
            dgv->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgv->BackgroundColor = Color::White;
            dgv->BorderStyle = BorderStyle::None;
            dgv->GridColor = Color::FromArgb(210, 225, 240);
        }

        static Button^ CrearBotonPrimario(String^ texto) {
            Button^ btn = gcnew Button();
            btn->Text = texto;
            btn->BackColor = AzulOscuro();
            btn->ForeColor = Color::White;
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderSize = 0;
            btn->Font = gcnew Drawing::Font("Segoe UI Semibold", 9.0f);
            btn->Height = 34;
            btn->Cursor = Cursors::Hand;
            return btn;
        }

        static Button^ CrearBotonPeligro(String^ texto) {
            Button^ btn = CrearBotonPrimario(texto);
            btn->BackColor = Color::FromArgb(178, 0, 0);
            return btn;
        }

        static Button^ CrearBotonSecundario(String^ texto) {
            Button^ btn = CrearBotonPrimario(texto);
            btn->BackColor = Color::FromArgb(90, 115, 140);
            return btn;
        }
    };
}