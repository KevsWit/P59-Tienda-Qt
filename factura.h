#ifndef FACTURA_H
#define FACTURA_H

#include <QDialog>
#include <QDateTime>
#include <QTableWidget>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

namespace Ui {
class Factura;
}

class Factura : public QDialog
{
    Q_OBJECT

public:
    explicit Factura(QWidget *parent = nullptr);
    ~Factura();

    void setCi(long long newCi);
    void setNombre(const QString &newNombre);
    void setTelef(const QString &newTelef);
    void setEmail(const QString &newEmail);
    void setDir(const QString &newDir);
    void setTabla(QTableWidget *newTabla);

    void facturar();

    void setSubtotal(const QString &newSubtotal);
    void setIva(const QString &newIva);
    void setTotal(const QString &newTotal);

private slots:
    void on_cmdCancelar_clicked();

    void on_cmdImprimir_clicked();



private:
    Ui::Factura *ui;
    QDateTime m_fecha;
    long long int m_ci;
    QString m_nombre;
    QString m_telef;
    QString m_email;
    QString m_dir;
    QTableWidget *m_tabla;
    QString m_subtotal;
    QString m_iva;
    QString m_total;
};

#endif // FACTURA_H
