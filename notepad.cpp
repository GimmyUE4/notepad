#include "notepad.h"
#include "ui_notepad.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);
    appIcon = QIcon(":/img/Resources/pencil.png");
    this->setWindowIcon(appIcon);
    this->setCentralWidget(ui->tabWidget);
    bold = false;
    italic = false;
    underlined = false;
    /* This is terrible but works */
    Notepad::on_actionNew_Tab_triggered();
    /* End of cancer */

}

Notepad::~Notepad()
{
    delete ui;
    textEditVector.clear();
}

void Notepad::on_actionNew_triggered()
{
    if (textEditVector.size() > 0)
    {
        int i = ui->tabWidget->currentIndex();
        textEditVector.at(i)->clear();
    }
}

void Notepad::on_actionOpen_triggered()
{
    if (textEditVector.size() <= 0)
    {
        Notepad::on_actionNew_Tab_triggered();
    }
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    QTextStream input(&file);
    QString text = input.readAll();

    int index = ui->tabWidget->currentIndex();
    textEditVector.at(index)->setText(text);
    QFileInfo fileInfo(file.fileName());
    ui->tabWidget->setTabText(index, fileInfo.fileName());
}

void Notepad::on_actionSave_triggered()
{
    if (textEditVector.size() > 0)
    {
        QString fileName;
        if (currentFile.isEmpty()){
            fileName = QFileDialog::getSaveFileName(this, "Save");
            currentFile = fileName;
        }
        else
            fileName = currentFile;
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
            return;
        }
        setWindowTitle(fileName);
        QTextStream output(&file);
        int i = ui->tabWidget->currentIndex();
        QString text = textEditVector.at(i)->toPlainText();
        output << text;
        file.close();
    }
}

void Notepad::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream output(&file);
    int i = ui->tabWidget->currentIndex();
    QString text = textEditVector.at(i)->toPlainText();
    output << text;
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), fileName);
    file.close();
}

void Notepad::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void Notepad::on_actionCopy_triggered()
{
#if QT_CONFIG(clipboard)
   int i = ui->tabWidget->currentIndex();
   textEditVector.at(i)->copy();
#endif
}


void Notepad::on_actionFont_triggered()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
    {
        int i = ui->tabWidget->currentIndex();
        textEditVector.at(i)->setFont(font);
    }
    currentFont = font;
}

void Notepad::on_actionCut_triggered()
{
#if QT_CONFIG(clipboard)
    int i = ui->tabWidget->currentIndex();
    textEditVector.at(i)->cut();
#endif
}

void Notepad::on_actionPaste_triggered()
{
#if QT_CONFIG(clipboard)
    int i = ui->tabWidget->currentIndex();
    textEditVector.at(i)->paste();
#endif
}

void Notepad::on_actionUndo_triggered()
{
    int i = ui->tabWidget->currentIndex();
    textEditVector.at(i)->undo();
}

void Notepad::on_actionRedo_triggered()
{
    int i = ui->tabWidget->currentIndex();
    textEditVector.at(i)->redo();
}

void Notepad::on_actionBold_toggled(bool button)
{
    if (button == true)
    {
        for (unsigned i = 0; i < textEditVector.size() ; ++i)
        {
            textEditVector.at(i)->setFontWeight(QFont::Bold);
        }
    }
    else
    {
        for (unsigned i = 0; i < textEditVector.size() ; ++i)
        {
            textEditVector.at(i)->setFontWeight(QFont::Normal);
        }
    }
    bold = button;
}

void Notepad::on_actionItalic_toggled(bool button)
{
    for (unsigned i = 0; i < textEditVector.size() ; ++i)
    {
        textEditVector.at(i)->setFontItalic(button);
    }
    italic = button;
}

void Notepad::on_actionUnderline_toggled(bool button)
{
    for (unsigned i = 0; i < textEditVector.size() ; ++i)
    {
        textEditVector.at(i)->setFontUnderline(button);
    }
    underlined = button;
}

void Notepad::on_actionFont_Color_triggered()
{
    QColor color = QColorDialog::getColor();
    for (unsigned i = 0; i < textEditVector.size() ; ++i)
    {
        textEditVector.at(i)->setTextColor(color);
    }
    currentTextColor = color;
}

void Notepad::update()
{
    for (unsigned i = 0; i < textEditVector.size() ; ++i)
    {
        textEditVector.at(i)->setTextColor(currentTextColor);
        textEditVector.at(i)->setFontUnderline(underlined);
        textEditVector.at(i)->setFontItalic(italic);
        if (bold)
        {
            textEditVector.at(i)->setFontWeight(QFont::Bold);
        }
        else
        {
            textEditVector.at(i)->setFontWeight(QFont::Normal);
        }
        textEditVector.at(i)->setFont(currentFont);
    }
}

void Notepad::on_actionNew_Tab_triggered()
{
    int index = ui->tabWidget->count() + 1;
    QString label = "Tab ";
    label.append(QString::number(index));
    QTextEdit * newTextEdit = new QTextEdit;
    ui->tabWidget->addTab(newTextEdit, label);
    textEditVector.push_back(newTextEdit);
    update();
}

void Notepad::on_tabWidget_tabCloseRequested(int index)
{
    delete textEditVector.at(index);
    textEditVector.erase(textEditVector.begin() + index);
    textEditVector.shrink_to_fit();
}
