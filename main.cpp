#include "notepad.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Notepad window;
    window.show();
    return app.exec();
}
