#include "adminwidget.h"

#include <QGridLayout>

AdminWidget::AdminWidget(QWidget *parent,
                         QString const& host,
                         QString const& database,
                         QString const& username,
                         QString const& password):
    QWidget(parent)
{
    if(!sqlUsers_.open(host, database, username, password) ||
            !sqlMessages_.open(host, database, username, password))
        throw std::exception();

    usersCbs_ = new QComboBox(this);
    QStringList users;
    sqlUsers_.list(users);
    usersCbs_->insertItems(0, users);

    blockBut_ = new QPushButton("Заблокировать", this);
    connect(blockBut_, &QPushButton::clicked, this, &AdminWidget::block);

    auto const toolbar = new QWidget(this);
    auto const hBox = new QHBoxLayout(toolbar);
    hBox->addWidget(usersCbs_);
    hBox->addWidget(blockBut_);
    toolbar->setLayout(hBox);
    toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    textEdit_ = new QTextEdit(this);
    //textEdit_->setFont(font_but);

    auto const layout = new QGridLayout(this);
    layout->addWidget(toolbar, 0, 0);
    layout->addWidget(textEdit_, 1, 0);
    //layout->setMargin(0);
    //layout->setSpacing(5);

    setLayout(layout);
    update();
}

void AdminWidget::block()
{
    if(!usersCbs_->currentText().isEmpty())
        sqlUsers_.block(usersCbs_->currentText());
}

void AdminWidget::update()
{
    QStringList users;
    sqlUsers_.list(users);

    QString text;

    for(auto const& user: users)
    {
        QVector<Message> messages;
        sqlMessages_.recv(user, messages);

        for(auto const& message: messages)
            text += QString("<i>&lt;from %1 to %2&gt;</i>: %3<br>")
                    .arg(message.from)
                    .arg(message.to)
                    .arg(message.text);
    }

    textEdit_->setHtml(text);
}
