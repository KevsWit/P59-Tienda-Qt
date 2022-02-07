#include "tienda.h"
#include "ui_tienda.h"

#include <QDebug>

Tienda::Tienda(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Tienda)
{
    ui->setupUi(this);
    // Lista de productos
    cargarProductos();
    // Mostrar los productos en el combo
    foreach (Producto *p, m_productos){
        ui->inProducto->addItem(p->nombre());
    }
    // Configurar cabecera de la tabla
    QStringList cabecera = {"Cantidad", "Producto", "P. unitario", "Subtotal"};
    ui->outDetalle->setColumnCount(4);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);
    // Establecer el subtotal a 0
    m_subtotal = 0;
}

Tienda::~Tienda()
{
    delete ui;
}
/**
 * @brief Tienda::cargarProductos Carga la lista de productos de la tienda
 */
void Tienda::cargarProductos()
{
    // Crear productos "quemados" en el código
    m_productos.append(new Producto(1, "Leche", 0.80));
    m_productos.append(new Producto(2, "Pan", 0.15));
    m_productos.append(new Producto(3, "Queso", 2.50));
    // Podría leerse de una base de datos, de un archivo o incluso de Internet
}

void Tienda::calcular(float stProducto)
{
    // Calcular valores
    m_subtotal += stProducto;
    float iva = m_subtotal * IVA / 100;
    float total = m_subtotal + iva;
    // Mostrar valores en GUI
    ui->outSubtotal->setText("$ " + QString::number(m_subtotal, 'f', 2));
    ui->outIva->setText("$ " + QString::number(iva, 'f', 2));
    ui->outTotal->setText("$ " + QString::number(total, 'f', 2));
}

void Tienda::restablecer()
{
    ui->inCedula->setText("");
    QPalette palette;
    palette.setColor(QPalette::Base,Qt::white);
    ui->inCedula->setPalette(palette);
    ui->inNombre->setText("");
    ui->inTelefono->setText("");
    ui->inEmail->setText("");
    ui->inDireccion->clear();
}

bool Tienda::verificarCedula()
{
    long long int ced = ui->inCedula->text().toLongLong();
    if (ced < 100000000 || ced > 3099999999){
        return false;
    }
    int cednum[10];
    long long int resto;
    int cociente;
    int posicion = 0;
    cociente = ced/10;
    resto = ced % 10;
    cednum[0] = resto;
    posicion++;
    while (cociente > 0){
        resto = cociente % 10;
        cednum[posicion] = resto;
        cociente /= 10;
        posicion++;
    }
    long long int divi1p = ced/100000000;
    long long int divi2p = ced%1000;
    if (((divi1p >= 1 && divi1p <= 24) || (divi1p == 30)) &&
            (cednum[7] >= 0 && cednum[7] <= 6 && divi2p > 0)){
        int i1 = cednum[1];
        int i2 = cednum[3];
        int i3 = cednum[5];
        int i4 = cednum[7];
        int i5 = cednum[9];
        if (posicion == 9)
            i5 = 0;
        int ni[]={i5, i4, i3, i2, i1};
        int suma1 = 0;
        for (int i = 0; i < 5; i++){
            if (2*ni[i] > 9){
                suma1 += (2*ni[i] - 9);
            }else{
                suma1 += (2*ni[i]);
            }
        }
        int j1 = cednum[2];
        int j2 = cednum[4];
        int j3 = cednum[6];
        int j4 = cednum[8];
        int suma2 = j1+ j2+ j3+ j4;
        int suma = suma1 + suma2;
        int verificacion1 = 0;
        if (suma % 10 == 0){
            verificacion1 = 0;
        }else{
            verificacion1 = 10-(suma % 10);
        }
        if (verificacion1 == cednum[0]){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool Tienda::verificar()
{
    if (ui->inCedula->text() == "9999999999")
        return true;
    bool ok;
    ui->inCedula->text().toLongLong(&ok);
    if (ok){
        if (verificarCedula()){
            QPalette palette;
            palette.setColor(QPalette::Base,Qt::green);
            ui->inCedula->setPalette(palette);
            return true;
        }
    }
    QPalette palette;
    palette.setColor(QPalette::Base,Qt::red);
    ui->inCedula->setPalette(palette);
    QMessageBox::warning(this,"Advertencia","La cédula es inadmisible.");
    return false;
}



void Tienda::on_inProducto_currentIndexChanged(int index)
{
    // Obtener el precio del producto actual seleccionado
    float precio = m_productos.at(index)->precio();
    // Mostrar el precio del product en la etiqueta
    ui->outPrecio->setText("$ " + QString::number(precio,'f',2));
    // Resetear el spinbox de cantidad
    ui->inCantidad->setValue(0);
}

void Tienda::on_btnAgregar_released()
{
    // Validar que no se agregen productos con 0 cantidad
    int cantidad = ui->inCantidad->value();
    if (cantidad == 0){
        QMessageBox::warning(this,"Advertencia","La cantidad es cero.");
        return;
    }
    // Obtener los datos de la GUI
    int i = ui->inProducto->currentIndex();
    Producto *p = m_productos.at(i);

    // Calcular el subrotal del producto
    float subtotal = p->precio() * cantidad;

    // Agregar los datos a la tabla
    int fila = ui->outDetalle->rowCount();
    int fila2 = fila-1;
    int bandera = 0;
    while(fila2 >= 0 && bandera == 0){
        if (p->nombre() == ui->outDetalle->item(fila2, 1)->text())
            bandera = 1;
        if (bandera == 0)
            fila2--;
    }
    if (bandera == 1){
        int newCant = cantidad + ui->outDetalle->item(fila2, 0)->text().toInt();
        float newSubTotal = newCant * p->precio();
        ui->outDetalle->setItem(fila2, 0, new QTableWidgetItem(QString::number(newCant)));
        ui->outDetalle->item(fila2, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->outDetalle->setItem(fila2, 3, new QTableWidgetItem(QString::number(newSubTotal,'f',2)));
        ui->outDetalle->item(fila2, 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    }else{
        ui->outDetalle->insertRow(fila);
        ui->outDetalle->setItem(fila, 0, new QTableWidgetItem(QString::number(cantidad)));
        ui->outDetalle->item(fila, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->outDetalle->setItem(fila, 1, new QTableWidgetItem(p->nombre()));
        ui->outDetalle->item(fila, 1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->outDetalle->setItem(fila, 2, new QTableWidgetItem(QString::number(p->precio(),'f',2)));
        ui->outDetalle->item(fila, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->outDetalle->setItem(fila, 3, new QTableWidgetItem(QString::number(subtotal,'f',2)));
        ui->outDetalle->item(fila, 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    }

    // Limpiar datos
    ui->inCantidad->setValue(0);
    ui->inProducto->setFocus();

    // Actualizar subtotales
    calcular(subtotal);

}

void Tienda::on_cmdCfinal_toggled(bool checked)
{
    if (checked){
        ui->inCedula->setText("9999999999");
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::green);
        ui->inCedula->setPalette(palette);
        ui->inNombre->setText("Consumidor Final");
    }else{
        ui->inCedula->setText("");
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::white);
        ui->inCedula->setPalette(palette);
        ui->inNombre->setText("");
    }
}

void Tienda::on_cmdFacturar_clicked()
{
    if (verificar() && ui->outDetalle->rowCount() != 0){
        Factura *factura = new Factura(this);
        factura->setCi(ui->inCedula->text().toLongLong());
        factura->setNombre(ui->inNombre->text());
        factura->setTelef(ui->inTelefono->text());
        factura->setEmail(ui->inEmail->text());
        factura->setDir(ui->inDireccion->toPlainText());
        factura->setTabla(ui->outDetalle);
        factura->setSubtotal(ui->outSubtotal->text());
        factura->setIva(ui->outIva->text());
        factura->setTotal(ui->outTotal->text());
        factura->facturar();
        factura->exec();
        restablecer();
        int nRow = ui->outDetalle->rowCount()-1;
        do{
            ui->outDetalle->removeRow(nRow);
            nRow--;
        }while(nRow >= 0);
        ui->outSubtotal->setText("0.0");
        ui->outIva->setText("0.0");
        ui->outTotal->setText("0.0");
        if (ui->cmdCfinal->isChecked()){
            ui->cmdCfinal->setCheckState(Qt::Unchecked);
        }
    }else{
        QMessageBox::warning(this,"Advertencia","No hay productos comprados.");
        return;
    }
}


void Tienda::on_action_Nuevo_triggered()
{
    restablecer();
    int nRow = ui->outDetalle->rowCount()-1;
    do{
        ui->outDetalle->removeRow(nRow);
        nRow--;
    }while(nRow >= 0);
    ui->outSubtotal->setText("0.0");
    ui->outIva->setText("0.0");
    ui->outTotal->setText("0.0");
    if (ui->cmdCfinal->isChecked()){
        ui->cmdCfinal->setCheckState(Qt::Unchecked);
    }
}


void Tienda::on_action_Salir_triggered()
{
    this->close();
}


void Tienda::on_actionA_cerca_triggered()
{
    Acerca *acerca = new Acerca(this);
    acerca->setVersion(VERSION);
    acerca->exec();
}

