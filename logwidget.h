#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>

class LogWidget : public QDialog
{
    Q_OBJECT

public:
    enum State {StateLogin, StateRegister};

    LogWidget(QDialog *parent = nullptr);
    ~LogWidget();

    State getState()const;
    void setInfoLog(QString const& log);

    QString getUsername()const;
    QString getPassword()const;

private:
    State currentState_;

    void setState(State state);

    QLabel* infoLog_;
    QLineEdit* textLogin_;
    QLineEdit* textPassword_;
    QLineEdit* textConfPassword_;
    QPushButton* ok_;
    QPushButton* cancel_;
    QPushButton* register_;
    QLabel* login_;
    QLabel* password_;
    QLabel* confPassword_;
};

#endif // LOGWIDGET_H
