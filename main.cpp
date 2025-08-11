#include <QApplication>
#include "view/ui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}


/*
# از مسیر Qt
copy "D:\Qt\6.7.3\mingw_64\bin\Qt6*.dll" "C:\study\term 2\project\oop-project-Ehsan\oop-project-Ehsan\cmake-build-debug"
D:\Qt\6.7.3\mingw_64\bin\windeployqt.exe --dir "C:\study\term 2\project\oop-project-Ehsan\oop-project-Ehsan\cmake-build-debug" "C:\study\term 2\project\oop-project-Ehsan\oop-project-Ehsan\cmake-build-debug\Phase2.exe"
# از مسیر MinGW
copy "D:\Qt\Tools\mingw1310_64\bin\*.dll" "C:\study\term 2\project\oop-project-Ehsan\oop-project-Ehsan\cmake-build-debug"
*/