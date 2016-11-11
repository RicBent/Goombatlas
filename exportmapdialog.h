#ifndef EXPORTMAPDIALOG_H
#define EXPORTMAPDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class ExportMapDialog;
}

class ExportMapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportMapDialog(QWidget *parent = 0);
    ~ExportMapDialog();

    QString getCPath();
    QString getReplacesPath();
    QString getPrefix();
    int getWorldId();

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

    void on_selectCBtn_clicked();

    void on_selectRBtn_clicked();

private:
    Ui::ExportMapDialog *ui;

    QSettings* settings;

    void checkOk();
};

#endif // EXPORTMAPDIALOG_H
