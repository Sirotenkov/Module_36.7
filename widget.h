#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <sqlusers.h>
#include <sqlmessages.h>

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr,
           QString const& login = "",
           QString const& host = "",
           QString const& database = "",
           QString const& username = "",
           QString const& password = "",
           bool isBlocked = false);
    ~Widget();

private slots:
    void sendMessagePrivate();
    void sendMessageAll();
    void update();

private:
    QString login_;

    SqlUsers sqlUsers_;
    SqlMessages sqlMessages_;

    QPushButton* sendToAll_;
    QPushButton* sendPrivate_;
    QComboBox* usersCbs_;
    QTextEdit* privateText_;
    QTextEdit* publicText_;
    QLineEdit* textMessage_;
    QLabel* label_;
    QLabel* labelReceivedAll_;
    QLabel* labelReceivedPrivate_;
    QMenuBar* usermenu_;
    QMenu* mainMenu_;
    QMenu* openNewDoc_;

};
#endif // WIDGET_H
