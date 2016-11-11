#include "exportmapdialog.h"
#include "ui_exportmapdialog.h"

#include <QFileDialog>
#include <QDebug>

ExportMapDialog::ExportMapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportMapDialog)
{
    ui->setupUi(this);

    setWindowTitle("Goomatlas - Export Map");
    settings = new QSettings("Blarg City", "Goombatlas");

    QStringList worlds;
    worlds << "World 1"
           << "World 2"
           << "World 3"
           << "World 4"
           << "World 5"
           << "World 6"
           << "World 7"
           << "World 8";
    ui->worldNumberBox->addItems(worlds);

    QRegExp rx("^[a-zA-Z0-9_]*$");
    QRegExpValidator* valid = new QRegExpValidator(rx,this);
    ui->customPrefixBox->setValidator(valid);

    ui->worldNumberBox->setCurrentIndex(settings->value("lastWorld", "0").toInt());
    ui->cFileBox->setText(settings->value("lastCPath", "").toString());
    ui->rFileBox->setText(settings->value("lastRPath", "").toString());

    checkOk();
}

ExportMapDialog::~ExportMapDialog()
{
    delete ui;
    delete settings;
}

void ExportMapDialog::checkOk()
{
    ui->okBtn->setEnabled((ui->cFileBox->text() != "" && ui->rFileBox->text() != ""));
}

void ExportMapDialog::on_okBtn_clicked()
{
    settings->setValue("lastWorld", ui->worldNumberBox->currentIndex());
    settings->setValue("lastCPath", ui->cFileBox->text());
    settings->setValue("lastRPath", ui->rFileBox->text());
    accept();
}

void ExportMapDialog::on_cancelBtn_clicked()
{
    reject();
}

void ExportMapDialog::on_selectCBtn_clicked()
{
    QString dir = ui->cFileBox->text();
    if (dir == "")
        dir = QDir::currentPath();

    QString path = QFileDialog::getSaveFileName(this, "Save C File", dir, "C Files (*.c)");

    if (path == "")
        return;

    ui->cFileBox->setText(path);

    QStringList list = path.split("/");

    if (list.length() < 2)
        return;

    QString replPath = path;
    replPath.chop(list.at(list.length()-1).length() + list.at(list.length()-2).length() + 1);
    replPath.append("replaces.x");

    QFileInfo check(replPath);
    if (check.exists())
        ui->rFileBox->setText(replPath);

    checkOk();
}

void ExportMapDialog::on_selectRBtn_clicked()
{
    QString dir = ui->rFileBox->text();
    if (dir == "")
        dir = QDir::currentPath();

    QString path = QFileDialog::getSaveFileName(this, "Save Replaces File", dir, "Replaces Files (*.x)");

    if (path == "")
        return;

    ui->rFileBox->setText(path);

    checkOk();
}

QString ExportMapDialog::getCPath()
{
    return ui->cFileBox->text();
}

QString ExportMapDialog::getReplacesPath()
{
    return ui->rFileBox->text();
}

QString ExportMapDialog::getPrefix()
{
    if (ui->customPrefixBox->text() != "")
        return ui->customPrefixBox->text();

    return QString("w%1").arg(ui->worldNumberBox->currentIndex()+1);
}

int ExportMapDialog::getWorldId()
{
    return ui->worldNumberBox->currentIndex()+1;
}
