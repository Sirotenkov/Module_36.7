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

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr,
           QString const& host = "",
           QString const& database = "",
           QString const& username = "",
           QString const& password = "");
    ~Widget();

private:
    SqlUsers sqlUsers_;

    QPushButton* sendToAll_;
    QPushButton* sendPrivate_;
    QComboBox* usersCbs_;
    QTextEdit* fromMe_;
    QTextEdit* toMe_;
    QLineEdit* textMessage_;
    QLabel* label_;
    QMenuBar* usermenu_;
    QMenu* mainMenu_;
    QMenu* openNewDoc_;

};
#endif // WIDGET_H
