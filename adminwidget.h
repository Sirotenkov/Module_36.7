#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <sqlmessages.h>
#include <sqlusers.h>
#include <QLabel>

class AdminWidget : public QWidget
{
    Q_OBJECT

public:
    AdminWidget(QWidget *parent = nullptr,
                QString const& host = "",
                QString const& database = "",
                QString const& username = "",
                QString const& password = "'");

    void update();

private slots:
    void block();
    void unblock();

private:
    SqlUsers sqlUsers_;
    SqlMessages sqlMessages_;

    QComboBox* usersCbs_;
    QPushButton* blockBut_;
    QPushButton* unblockBut_;
    QTextEdit* messagesEdit_;
    QTextEdit* usersBlackList_;
    QLabel* labelTextEdit_;
    QLabel* labelUsersBlackList_;


};

#endif // ADMINWIDGET_H
