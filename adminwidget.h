#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <sqlmessages.h>
#include <sqlusers.h>

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

private:
    SqlUsers sqlUsers_;
    SqlMessages sqlMessages_;

    QComboBox* usersCbs_;
    QPushButton* blockBut_;
    QTextEdit* textEdit_;

};

#endif // ADMINWIDGET_H
