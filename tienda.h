#ifndef TIENDA_H
#define TIENDA_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

#include "producto.h"
#include "factura.h"
#include "acerca.h"

#define IVA 12
#define VERSION "1.0"

QT_BEGIN_NAMESPACE
namespace Ui { class Tienda; }
QT_END_NAMESPACE

class Tienda : public QMainWindow
{
    Q_OBJECT

public:
    Tienda(QWidget *parent = nullptr);
    ~Tienda();

private slots:
    void on_inProducto_currentIndexChanged(int index);

    void on_btnAgregar_released();

    void on_cmdCfinal_toggled(bool checked);

    void on_cmdFacturar_clicked();

    void on_action_Nuevo_triggered();

    void on_action_Salir_triggered();

    void on_actionA_cerca_triggered();

private:
    Ui::Tienda *ui;
    QList<Producto*> m_productos;
    void cargarProductos();
    float m_subtotal;
    void calcular(float stProducto);
    void restablecer();
    bool verificarCedula();
    bool verificar();
};
#endif // TIENDA_H
