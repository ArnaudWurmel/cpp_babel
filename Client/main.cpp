#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <cstring>
#include <thread>
#include "DataManager/DataManager.h"
#include "src/Window.h"

static void setHost(std::string& host, std::string&, char *value) {
    host = value;
}

static void setUsername(std::string&, std::string& username, char *value) {
    username = value;
}

static bool parsingParameters(int ac, char **av, std::string& host, unsigned short& port, std::string& username) {
    std::map<std::string, void (*)(std::string&, std::string&, char *)>   setter;

    setter.insert(std::make_pair("-h", &setHost));
    setter.insert(std::make_pair("-u", &setUsername));
    for (int i = 1; i < ac; i++) {
        if (setter.find(av[i]) != setter.end() && i + 1 < ac) {
            (setter[av[i]])(host, username, av[i + 1]);
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

int main(int ac, char **av) {
    QApplication    app(ac, av);
    std::string host = "127.0.0.1";
    std::string username = "";
    unsigned short port = 8080;
    std::unique_ptr<babel::DataManager>  dataManager;

    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::vector<std::string> >("std::vector<std::string>");
    if (!parsingParameters(ac, av, host, port, username)) {
        std::cerr << "Can't validate parameters" << std::endl;
        return 1;
    }
    try {
        Window  window;
        dataManager = std::unique_ptr<babel::DataManager>(new babel::DataManager(window, host, port));

        dataManager->startData();
        window.show();
        window.start(username);
        int status =  app.exec();
        return status;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}