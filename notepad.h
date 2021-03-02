#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QTextEdit>
#include <vector>
#include <QFontDialog>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionExit_triggered();
    void on_actionCopy_triggered();
    void on_actionFont_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionBold_toggled(bool button);
    void on_actionItalic_toggled(bool button);
    void on_actionUnderline_toggled(bool button);
    void on_actionFont_Color_triggered();

    void on_actionNew_Tab_triggered();

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::Notepad *ui;
    QString currentFile;
    QIcon appIcon;
    std::vector <QTextEdit *> textEditVector;
    bool underlined;
    bool italic;
    bool bold;
    QFont currentFont;
    QColor currentTextColor;
    void update();
};
#endif // NOTEPAD_H
