#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QMenu>
#include<QMenuBar>

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QPushButton* sendToAll_;
    QPushButton* sendPrivate_;
    QTextEdit* fromMe_;
    QTextEdit* toMe_;
    QLineEdit* textMessage_;
    QLabel* label_;
    QMenuBar* usermenu_;
    QMenu* mainMenu_;
    QMenu* openNewDoc_;

};
#endif // WIDGET_H
