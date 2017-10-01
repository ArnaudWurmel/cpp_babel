#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include "Socket/QtSocket.h"

int main(int ac, char **av) {
    QApplication    app(ac, av);
    std::mutex  mutex;
    std::condition_variable cv;
    babel::QtSocket socket(mutex, cv);
    babel::Message  message(babel::Message::Connect);

    message.setBody("Arnaud", 6);
    socket.startSession();
    std::cout << socket.connectSocket("127.0.0.1", 8080) << std::endl;
    socket.write(message);
    return app.exec();
}