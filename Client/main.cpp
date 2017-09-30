#include <iostream>
#include <QApplication>
#include <QMainWindow>

int main(int ac, char **av) {
    QApplication    app(ac, av);
    std::cout << "Hello, World!" << std::endl;
    return app.exec();
}