#include "FrmLogin.h"
#include "FrmPrincipalh.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    SalesForceV3::FrmLogin^ login = gcnew SalesForceV3::FrmLogin();
    if (login->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        Application::Run(gcnew SalesForceV3::FrmPrincipal());
    }

    return 0;
}