#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <cstring>
#include <thread>
#include "DataManager/DataManager.h"

static bool parsingParameters(int ac, char **av, std::string& host, unsigned short& port) {
    for (int i = 1; i < ac; i++) {
        if (std::strcmp(av[i], "-h") == 0 && i + 1 < ac) {
            host = av[i + 1];
            ++i;
        }
        else if (std::strcmp(av[i], "-p") == 0 && i + 1 < ac) {
            port = std::atoi(av[i + 1]);
            ++i;
        }
        else {
            return false;
        }
    }
    return true;
}

void    mainThread(babel::DataManager *dataManager) {

    if (dataManager->connectUser()) {
        std::cout << "Username : <" << dataManager->getUsername() << ">" << std::endl;
    }
    else {
        std::cout << dataManager->getLastError() << std::endl;
    }
}

#include "src/Window.h"

int main(int ac, char **av) {
    QApplication    app(ac, av);
    std::string host = "127.0.0.1";
    unsigned short port = 8080;

    if (!parsingParameters(ac, av, host, port)) {
        std::cerr << "Can't validate parameters" << std::endl;
        return 1;
    }
    try {
        babel::DataManager  dataManager(host, port);
        std::thread thread(&mainThread, &dataManager);

<<<<<<< HEAD
        int status = app.exec();
        thread.join();
        return status;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
=======
    message.setBody("Arnaud", 6);
    socket.startSession();
    std::cout << socket.connectSocket("127.0.0.1", 8080) << std::endl;
    socket.write(message);

    //Instancie class window
    Window window;

    window.show();
    return app.exec();
}
>>>>>>> be2809400ca14373fe8fb161f420349b3da6964a
