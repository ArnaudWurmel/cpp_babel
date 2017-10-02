#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QGroupBox;

class Window : public QWidget
{
Q_OBJECT

public:
    Window();

private:
    QGroupBox *createChannelsList();
    QGroupBox *createUsersList();
};

#endif