#include <QApplication>

#include "StudentExamWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StudentExamWindow window;
    window.show();

    return app.exec();
}