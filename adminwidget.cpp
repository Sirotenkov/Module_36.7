#include "adminwidget.h"

#include <QGridLayout>

AdminWidget::AdminWidget(QWidget *parent,
                         QString const& host,
                         QString const& database,
                         QString const& username,
                         QString const& password):
    QWidget(parent)
{
    QFont const font_but("Sans serif", 10, QFont::ExtraLight),
            font_text("Courrier New", 10, QFont::ExtraLight, QFont::Style::StyleItalic);

    if(!sqlUsers_.open(host, database, username, password) ||
            !sqlMessages_.open(host, database, username, password))
        throw std::exception();

    usersCbs_ = new QComboBox(this);
    usersCbs_->setFont(font_text);
    QStringList users;
    sqlUsers_.list(users);
    usersCbs_->insertItems(0, users);

    blockBut_ = new QPushButton("Block", this);
    connect(blockBut_, &QPushButton::clicked, this, &AdminWidget::block);
    blockBut_->setFont(font_but);

    unblockBut_ = new QPushButton("Unlock", this);
    connect(unblockBut_, &QPushButton::clicked, this, &AdminWidget::unblock);
    unblockBut_->setFont(font_but);

    auto const toolbar = new QWidget(this);
    auto const hBox = new QHBoxLayout(toolbar);
    hBox->addWidget(usersCbs_);
    hBox->addWidget(blockBut_);
    hBox->addWidget(unblockBut_);
    hBox->setMargin(0);
    hBox->setSpacing(5);
    toolbar->setLayout(hBox);
    toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    labelTextEdit_ = new QLabel("All messages", this);
    labelTextEdit_->setFont(font_text);

    messagesEdit_ = new QTextEdit(this);
    messagesEdit_->setTextInteractionFlags(Qt::NoTextInteraction);

    labelUsersBlackList_ = new QLabel("Black list", this);
    labelUsersBlackList_->setFont(font_text);

    usersBlackList_ = new QTextEdit(this);
    usersBlackList_->setTextInteractionFlags(Qt::NoTextInteraction);

    auto const layout = new QGridLayout(this);
    layout->addWidget(toolbar, 0, 0);

    layout->addWidget(labelTextEdit_, 1, 0);
    layout->addWidget(labelUsersBlackList_, 1, 1);
    layout->addWidget(messagesEdit_, 2, 0);
    layout->addWidget(usersBlackList_, 2, 1);
    //layout->setMargin(0);
    //layout->setSpacing(0);

    setLayout(layout);
    update();
}

void AdminWidget::block()
{
    sqlUsers_.block(usersCbs_->currentText());
    update();
}

void AdminWidget::unblock()
{
    sqlUsers_.unblock(usersCbs_->currentText());
    update();
}

void AdminWidget::update()
{
    QStringList users;
    sqlUsers_.list(users);

    QString messagesText, blockedUsers;

    for(auto const& user: users)
    {
        QVector<Message> messages;
        sqlMessages_.recv(user, messages);

        for(auto const& message: messages)
            messagesText += QString("<i>&lt;from %1 to %2&gt;</i>: %3<br>")
                    .arg(message.from)
                    .arg(message.to)
                    .arg(message.text);

        if (sqlUsers_.isBlocked(user))
            blockedUsers += QString("%1<br>").arg(user);
    }

    messagesEdit_->setHtml(messagesText);
    usersBlackList_->setHtml(blockedUsers);
}
