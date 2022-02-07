#ifndef ACERCA_H
#define ACERCA_H

#include <QDialog>

namespace Ui {
class Acerca;
}

class Acerca : public QDialog
{
    Q_OBJECT

public:
    explicit Acerca(QWidget *parent = nullptr);
    ~Acerca();
    void setVersion(const QString &newVersion);

private slots:
    void on_cmdCancelar_clicked();

    void on_cmdOk_clicked();

private:
    Ui::Acerca *ui;
    QString m_version;
};

#endif // ACERCA_H
