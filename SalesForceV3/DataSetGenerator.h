#pragma once
#pragma managed(push, off)

#include "GestorCliente.h"
#include "GestorVenta.h"
#include "GestorSoporte.h"

class DatasetGenerator {
public:
    static void cargar(GestorCliente* gc, GestorVenta* gv, GestorSoporte* gs) {
        cargarClientes(gc);
        cargarVentas(gv);
        cargarSoporte(gs);
    }

private:
    static void cargarClientes(GestorCliente* gc) {
        gc->insertarCuenta({ 1, "Claro Peru",       "Telecomunicaciones", "01-630-3000", "info@claro.com.pe",     Direccion("Peru","Lima","Miraflores","Av. La Marina 600") });
        gc->insertarCuenta({ 2, "BCP",               "Banca",             "01-311-9898", "info@viabcp.com",        Direccion("Peru","Lima","San Isidro", "Av. Centenario 156") });
        gc->insertarCuenta({ 3, "Interbank",         "Banca",             "01-219-2000", "contacto@interbank.pe",  Direccion("Peru","Lima","La Molina",  "Av. Benavides 200") });
        gc->insertarCuenta({ 4, "Alicorp",           "Consumo Masivo",    "01-315-0800", "info@alicorp.pe",        Direccion("Peru","Lima","Callao",     "Jr. Lima 100") });
        gc->insertarCuenta({ 5, "Falabella Peru",    "Retail",            "01-610-0000", "clientes@falabella.pe",  Direccion("Peru","Lima","San Borja",  "Av. Angamos 888") });

        gc->insertarContacto({ 1, "Juan",   "Perez",    "Gerente TI",   "+51 999 001 001", "jperez@claro.com.pe",   1, Direccion("Peru","Lima","Miraflores","") });
        gc->insertarContacto({ 2, "Maria",  "Lopez",    "Dir. Comercial","+51 999 002 002","mlopez@bcp.com.pe",     2, Direccion("Peru","Lima","San Isidro","") });
        gc->insertarContacto({ 3, "Carlos", "Ramirez",  "CTO",           "+51 999 003 003","cramirez@interbank.pe", 3, Direccion("Peru","Lima","La Molina","") });
        gc->insertarContacto({ 4, "Ana",    "Torres",   "Jefa Marketing","+51 999 004 004","atorres@alicorp.pe",    4, Direccion("Peru","Lima","Callao","") });
        gc->insertarContacto({ 5, "Luis",   "Fernandez","Gcia. Ventas",  "+51 999 005 005","lfernandez@falabella.pe",5,Direccion("Peru","Lima","San Borja","") });

        gc->insertarUsuario({ 1, "Administrador", "Sistema",   "Admin",    "admin",     "admin123" });
        gc->insertarUsuario({ 2, "Vendedor",       "Principal", "Vendedor", "vendedor1", "pass123" });
        gc->insertarUsuario({ 3, "Soporte",        "Tecnico",   "Soporte",  "soporte1",  "pass456" });

        gc->insertarInteraccion({ 1, "Llamada",  "Presentacion del CRM",     "01/06/2025", 1, 2 });
        gc->insertarInteraccion({ 2, "Email",    "Cotizacion enviada",       "03/06/2025", 2, 2 });
        gc->insertarInteraccion({ 3, "Reunion",  "Demo en oficinas cliente", "05/06/2025", 3, 2 });
        gc->insertarInteraccion({ 4, "Llamada",  "Seguimiento propuesta",    "07/06/2025", 4, 2 });
        gc->insertarInteraccion({ 5, "Email",    "Contrato enviado",         "10/06/2025", 5, 2 });
    }

    static void cargarVentas(GestorVenta* gv) {
        gv->encolarCliente({ 1, "Pablo Huaman",  "phuaman@startup.pe",   "CRM Software" });
        gv->encolarCliente({ 2, "Rosa Quispe",   "rquispe@empresa.pe",   "Cloud Solutions" });
        gv->encolarCliente({ 3, "David Castro",  "dcastro@corp.com",     "ERP Integration" });
        gv->encolarCliente({ 4, "Elena Vargas",  "evargas@tech.io",      "API Services" });
        gv->encolarCliente({ 5, "Miguel Cano",   "mcano@pyme.pe",        "Marketing Automation" });

        gv->insertarOportunidad({ 1, "Renovacion contrato Claro",  50000.0,  "Negociacion" });
        gv->insertarOportunidad({ 2, "Implementacion CRM BCP",     150000.0, "Prospeccion" });
        gv->insertarOportunidad({ 3, "Actualizacion Interbank",     80000.0,  "Negociacion" });
        gv->insertarOportunidad({ 4, "Campana digital Alicorp",    35000.0,  "Cerrado" });
        gv->insertarOportunidad({ 5, "E-commerce Falabella",       200000.0, "Prospeccion" });
        gv->insertarOportunidad({ 6, "Soporte anual Claro",        24000.0,  "Negociacion" });

        gv->insertarProducto({ 1, "CRM Basic",       1500.0,  "Software" });
        gv->insertarProducto({ 2, "CRM Enterprise",  5000.0,  "Software" });
        gv->insertarProducto({ 3, "Consultoria",      200.0,  "Servicio" });
        gv->insertarProducto({ 4, "Soporte Premium",  800.0,  "Servicio" });
        gv->insertarProducto({ 5, "Integracion API", 3000.0,  "Desarrollo" });
        gv->insertarProducto({ 6, "Capacitacion",     500.0,  "Formacion" });
        gv->insertarProducto({ 7, "Cloud Hosting",    350.0,  "Infraestructura" });
        gv->insertarProducto({ 8, "Analytics Plus",  2000.0,  "Software" });

        gv->insertarCotizacion({ 1,  7500.0, "30/06/2025", "Pendiente" });
        gv->insertarCotizacion({ 2, 15000.0, "15/07/2025", "Aceptada" });
        gv->insertarCotizacion({ 3,  3200.0, "20/06/2025", "Rechazada" });

        gv->insertarContrato({ 1, "01/04/2025", "Contrato anual CRM Enterprise",    60000.0 });
        gv->insertarContrato({ 2, "15/03/2025", "Soporte y mantenimiento 12 meses",  9600.0 });

        gv->generarDataSet();
    }

    static void cargarSoporte(GestorSoporte* gs) {
        gs->encolarCaso({ 1, "Error en modulo de pagos",          "Abierto",     "Alta" });
        gs->encolarCaso({ 2, "Integracion con ERP fallida",       "En proceso",  "Media" });
        gs->encolarCaso({ 3, "Dashboard no carga datos",          "Cerrado",     "Baja" });
        gs->encolarCaso({ 4, "Solicitud nueva funcionalidad",     "Abierto",     "Media" });
        gs->encolarCaso({ 5, "Problemas de rendimiento en reportes","En proceso", "Alta" });

        gs->apilarSolucion({ 1, 1, "Parche de actualizacion v2.1 aplicado",      "05/04/2025" });
        gs->apilarSolucion({ 2, 2, "Reconfiguracion de endpoints API",           "10/04/2025" });

        gs->encolarTarea({ 1, "Llamar a contacto BCP",            "Pendiente",  "15/06/2025" });
        gs->encolarTarea({ 2, "Preparar propuesta Alicorp",       "Pendiente",  "20/06/2025" });
        gs->encolarTarea({ 3, "Demo para equipo Claro",           "Completada", "01/06/2025" });
        gs->encolarTarea({ 4, "Enviar cotizacion Falabella",      "Pendiente",  "25/06/2025" });

        gs->insertarEvento({ 1, "Reunion de ventas mensual",    "15/06 09:00", "Sala A" });
        gs->insertarEvento({ 2, "Demo CRM para BCP",           "18/06 14:00", "Virtual - Zoom" });
        gs->insertarEvento({ 3, "Capacitacion equipo soporte", "22/06 10:00", "Sala B" });

        gs->apilarHistorial({ 1, "Clientes", "Alta de cuenta Claro Peru",      "01/04/2025" });
        gs->apilarHistorial({ 2, "Ventas",   "Cierre de contrato con BCP",     "15/03/2025" });
        gs->apilarHistorial({ 3, "Soporte",  "Resolucion caso #1 (pagos)",     "05/04/2025" });
        gs->apilarHistorial({ 4, "Ventas",   "Nueva oportunidad: E-commerce",  "20/05/2025" });
    }
};

#pragma managed(pop)