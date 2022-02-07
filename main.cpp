#include "tienda.h"

#include <QApplication>
#include <QTranslator>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        QTranslator traducion;
        QString idioma = QLocale::system().name();
        int pos = idioma.indexOf('_');
        idioma = idioma.mid(0, pos);
        if (idioma == "fr"){
            traducion.load(":/tienda_fr.qm");
        }else if (idioma == "en"){
            traducion.load(":/tienda_en.qm");
        }else if(idioma == "it"){
            traducion.load(":/tienda_it.qm");
        }else if(idioma == "de"){
            traducion.load(":/tienda_de.qm");
        }
        if (idioma != "es"){
            a.installTranslator(&traducion);
        }
    Tienda w;
    w.show();
    return a.exec();
}
