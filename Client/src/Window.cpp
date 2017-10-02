#include <QGridLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QListView>


#include "./Window.h"

Window::Window()
{
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(createChannelsList(), 0, 0);
    grid->addWidget(createUsersList(), 0, 1);
    setLayout(grid);

    setWindowTitle(tr("cpp_babel"));
    resize(480, 320);
}

QGroupBox *Window::createChannelsList()
{

    QGroupBox *groupBox = new QGroupBox(tr("Channels list"));

    QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
    QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
    QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));


    radio1->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

QGroupBox *Window::createUsersList()
{
    QGroupBox *groupBox = new QGroupBox(tr("Users list"));

    QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
    QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
    QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));


    radio1->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}
