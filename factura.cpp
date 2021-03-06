#include "factura.h"
#include "ui_factura.h"

Factura::Factura(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Factura)
{
    ui->setupUi(this);
    this->m_fecha = QDateTime::currentDateTime();
    this->m_ci = 0;
    this->m_nombre = "";
    this->m_telef = "";
    this->m_email = "";
    this->m_dir = "";
    QString dirHome = QDir::home().absolutePath();
    ui->outDir->setText(tr("Destino de impresión: ") + dirHome);
}

Factura::~Factura()
{
    delete ui;
}

void Factura::on_cmdCancelar_clicked()
{
    this->close();
}

void Factura::on_cmdImprimir_clicked()
{
    QString date = QString::number(m_fecha.date().day()) +
            QString::number(m_fecha.date().month()) +
            QString::number(m_fecha.date().year());
    QString time = QString::number(m_fecha.time().hour()) + QString::number(m_fecha.time().minute());
    QString nuevo = date + time + ".txt";
    QString dirHome = QDir::home().absolutePath();
    QString nombreArchivo = dirHome + "/" + nuevo;
    QFile archivo(nombreArchivo);
    if(archivo.open(QFile::WriteOnly | QIODevice::Append)){
        QTextStream salida(&archivo);
        salida << ui->outFactura->toPlainText();
        salida << "\n";
    }else{
        QMessageBox::warning(this,
                             tr("Guardar datos"),
                             tr("No se pudo guardar el archivo"));
    }
    archivo.close();
    this->close();
}

void Factura::setSubtotal(const QString &newSubtotal)
{
    m_subtotal = newSubtotal;
}

void Factura::setIva(const QString &newIva)
{
    m_iva = newIva;
}

void Factura::setTotal(const QString &newTotal)
{
    m_total = newTotal;
}

void Factura::facturar()
{
    int sizeMaxCant = 0;
    int sizeMaxProd = 0;
    int sizeMaxPUni = 0;
    int sizeMaxST = 0;
    int nRow = m_tabla->rowCount()-1;
    int cc = 0;
    int maxSize = 0;
    // Determinamos los tamaños máximos de los strings (items)
    do{
        if (cc == 0){
            sizeMaxCant = m_tabla->item(nRow, 0)->text().size();
            sizeMaxProd = m_tabla->item(nRow, 1)->text().size();
            sizeMaxPUni = m_tabla->item(nRow, 2)->text().size();
            sizeMaxST = m_tabla->item(nRow, 3)->text().size();
            cc++;
        }else{
            if (m_tabla->item(nRow, 0)->text().size() > sizeMaxCant)
                sizeMaxCant = m_tabla->item(nRow, 0)->text().size();
            if (m_tabla->item(nRow, 1)->text().size() > sizeMaxProd)
                sizeMaxProd = m_tabla->item(nRow, 1)->text().size();
            if (m_tabla->item(nRow, 2)->text().size() > sizeMaxPUni)
                sizeMaxPUni = m_tabla->item(nRow, 2)->text().size();
            if (m_tabla->item(nRow, 3)->text().size() > sizeMaxST)
                sizeMaxST = m_tabla->item(nRow, 3)->text().size();
        }
        nRow--;
    }while(nRow >= 0);
    // Establecemos un tamaño de string máximo
    maxSize = sizeMaxCant + sizeMaxProd + sizeMaxPUni + sizeMaxST + 35;
    // Creamos una línea divisora que se utilizará en la impresión
    QString lineas = "";
    for (int i = 0; i <= maxSize; i++){
        lineas += "- ";
    }
    // Agregamos al widget de salida los datos junto con espacios para moverlos del borde
    ui->outFactura->appendPlainText(lineas);
    QString espacios = "";
    QString texto = "MI TIENDA";
    int cant = maxSize-texto.size();
    for (int i = 0; i < cant/2; i++){
        espacios += "   ";
    }
    ui->outFactura->appendPlainText(espacios+texto);
    texto = "RUC: 172130733001";
    espacios = "";
    cant = maxSize-texto.size();
    for (int i = 0; i < cant/2+2; i++){
        espacios += "   ";
    }
    ui->outFactura->appendPlainText(espacios+texto);
    texto = "QUITUMBE";
    espacios = "";
    cant = maxSize-texto.size();
    for (int i = 0; i < cant/2-1; i++){
        espacios += "   ";
    }
    ui->outFactura->appendPlainText(espacios+texto);
    texto = tr("OBLIGADO A LLEVAR CONTABILIDAD: SI");
    espacios = "";
    cant = maxSize-texto.size();
    for (int i = 0; i < cant/2+7; i++){
        espacios += "   ";
    }
    // Obtenemos la fecha del sistema
    ui->outFactura->appendPlainText(espacios+texto);
    ui->outFactura->appendPlainText(lineas);
    QString date = QString::number(m_fecha.date().day()) + "/" +
            QString::number(m_fecha.date().month()) + "/" +
            QString::number(m_fecha.date().year());
    QString time = m_fecha.time().toString();
    // Agregamos al widget de salida los datos del cliente
    ui->outFactura->appendPlainText(tr("FECHA: ") + date + " " + time);
    ui->outFactura->appendPlainText(tr("CLIENTE: ") + m_nombre);
    ui->outFactura->appendPlainText(tr("CI: ") + QString::number(m_ci));
    ui->outFactura->appendPlainText(tr("TEL: ") + m_telef);
    ui->outFactura->appendPlainText(tr("DIR: ") + m_dir);
    ui->outFactura->appendPlainText(tr("E-MAIL: ") + m_email);
    ui->outFactura->appendPlainText(lineas);
    // Agregamos al widget la cabecera de la tabla
    QString cabecera = "";
    QString cantidad = tr("CANT.");
    espacios = "";
    for (int i = 0; i <= cantidad.size()+5; i++){
        espacios += " ";
    }
    cabecera += cantidad + espacios;
    cantidad += espacios;
    sizeMaxCant = cantidad.size();
    QString producto = tr("PRODUCTO");
    espacios = "";
    for (int i = 0; i <= producto.size()+25; i++){
        espacios += " ";
    }
    cabecera += producto + espacios;
    producto += espacios;
    sizeMaxProd = producto.size();
    QString pUnitario = tr("P.UNITARIO");
    espacios = "";
    for (int i = 0; i <= pUnitario.size(); i++){
        espacios += " ";
    }
    cabecera += pUnitario + espacios;
    pUnitario += espacios;
    sizeMaxPUni = pUnitario.size();
    QString subTotal = tr("SUBTOTAL");
    cabecera += subTotal;
    ui->outFactura->appendPlainText(cabecera);
    ui->outFactura->appendPlainText(lineas);
    nRow = m_tabla->rowCount();
    cc = 0;
    // Agregamos al widget la tabla del detalle de compra
    QString row = "";
    do{
        row = "";
        espacios = "";
        cant = sizeMaxCant - m_tabla->item(cc, 0)->text().size();
        for (int i = 0; i < cant; i++){
            espacios += " ";
        }
        row += m_tabla->item(cc, 0)->text() + espacios;
        espacios = "";
        cant = sizeMaxProd - m_tabla->item(cc, 1)->text().size();
        for (int i = 0; i < cant; i++){
            espacios += " ";
        }
        row += m_tabla->item(cc, 1)->text() + espacios;
        espacios = "";
        cant = sizeMaxPUni - m_tabla->item(cc, 2)->text().size();
        for (int i = 0; i < cant; i++){
            espacios += " ";
        }
        row += m_tabla->item(cc, 2)->text() + espacios;
        row += m_tabla->item(cc, 3)->text();
        ui->outFactura->appendPlainText(row);
        cc++;
    }while(cc < nRow);
    // Agregamos al widget los datos finales de la factura
    ui->outFactura->appendPlainText(lineas);
    espacios = "";
    texto = tr("Subtotal: ") + m_subtotal;
    cant = maxSize-texto.size();
    for (int i = 0; i < cant+8; i++){
        espacios += "  ";
    }
    ui->outFactura->appendPlainText(espacios + texto);
    espacios = "";
    texto = tr("IVA (12%): ") + m_iva;
    cant = maxSize-texto.size();
    for (int i = 0; i < cant+8; i++){
        espacios += "  ";
    }
    ui->outFactura->appendPlainText(espacios + texto);
    espacios = "";
    texto = tr("TOTAL: ") + m_total;
    cant = maxSize-texto.size();
    for (int i = 0; i < cant+6; i++){
        espacios += "  ";
    }
    ui->outFactura->appendPlainText(espacios + texto);
}

void Factura::setTabla(QTableWidget *newTabla)
{
    m_tabla = newTabla;
}

void Factura::setCi(long long newCi)
{
    m_ci = newCi;
}

void Factura::setNombre(const QString &newNombre)
{
    m_nombre = newNombre;
}

void Factura::setTelef(const QString &newTelef)
{
    m_telef = newTelef;
}

void Factura::setEmail(const QString &newEmail)
{
    m_email = newEmail;
}

void Factura::setDir(const QString &newDir)
{
    m_dir = newDir;
}
