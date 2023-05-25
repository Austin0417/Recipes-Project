#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include "mainwindow.h"
#include <QDebug>
#include <QMouseEvent>
#include <QDialog>
#include <QEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QStackedLayout>
#include <QtSql>
#include <QSqlQuery>



class AccountLabel : public QLabel {
    Q_OBJECT
public:
    AccountLabel(QWidget* parent_ = nullptr) : QLabel(parent_) {}
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
signals:
    void labelClick();

};


class LoginDialog : public QDialog {
public:
    LoginDialog(QSqlDatabase* db_, QWidget* parent_ = nullptr) : QDialog(parent_), db(db_) {
        setWindowTitle("Login");
        resize(QSize(400, 250));

        window = qobject_cast<MainWindow*>(parent());

        QSqlQuery query(*db);
        QString createAccountsTable("CREATE TABLE IF NOT EXISTS accounts ("
                                    "id SERIAL PRIMARY KEY,"
                                    "email TEXT,"
                                    "username TEXT,"
                                    "password TEXT)");
        if (query.exec(createAccountsTable)) {
            qDebug() << "Successfully created accounts table";
        } else {
            qDebug() << "Failed to create accounts table";
        }

        QString createSavedRecipesTable("CREATE TABLE IF NOT EXISTS UserSavedRecipes ("
                                        "id INT REFERENCES accounts(id),"
                                        "name TEXT,"
                                        "ingredients TEXT,"
                                        "instructions TEXT,"
                                        "favorited BOOLEAN)");
        if (query.exec(createSavedRecipesTable)) {
            qDebug() << "Created UserSavedRecipes table";
        } else {
            qDebug() << "Failed to create UserSavedRecipes table";
        }

        QString createSavedCalendarTable("CREATE TABLE IF NOT EXISTS SavedCalendar ("
                                         "id INT REFERENCES accounts(id),"
                                         "date TEXT,"
                                         "event TEXT)");

        if (query.exec(createSavedCalendarTable)) {
            qDebug() << "Created SavedCalendar table successfully!";
        } else {
            qDebug() << "Failed to create SavedCalendar table";
        }

        mainLayout = new QVBoxLayout(this);

        layout = new QStackedLayout;
        loginWidget = new QWidget();
        accountCreationWidget = new QWidget();

        // Login Layout
        usernameInput = new QLineEdit(this);
        passwordInput = new QLineEdit(this);
        createAccount = new AccountLabel(this);


        createAccount->setStyleSheet("color: blue; font-weight: bold; text-decoration: underline; max-width: 120px;");
        createAccount->setText("Or create an account");
        createAccount->setOpenExternalLinks(false);

        passwordInput->setEchoMode(QLineEdit::Password);

        loginLayout = new QVBoxLayout();

        QLabel* usernameLabel = new QLabel(this);
        usernameLabel->setText("Enter username or email: ");

        QLabel* passwordLabel = new QLabel(this);
        passwordLabel->setText("Enter password: ");

        loginLayout->addWidget(usernameLabel);
        loginLayout->addWidget(usernameInput);
        loginLayout->addWidget(passwordLabel);
        loginLayout->addWidget(passwordInput);
        loginLayout->addWidget(createAccount);

        loginWidget->setLayout(loginLayout);
        layout->addWidget(loginWidget);


        // Account creation layout
        accountCreationLayout = new QVBoxLayout();

        createEmail = new QLineEdit(nullptr);
        createUsername = new QLineEdit(nullptr);
        createPassword = new QLineEdit(nullptr);

        createPassword->setEchoMode(QLineEdit::Password);

        backBtn = new QPushButton(nullptr);
        backBtn->setText("Back");

        QLabel* emailLabel = new QLabel();
        QLabel* createUsernameLabel = new QLabel();
        QLabel* createPasswordLabel = new QLabel();

        emailLabel->setText("Email: ");
        createUsernameLabel->setText("Username: ");
        createPasswordLabel->setText("Password: ");

        accountCreationLayout->addWidget(emailLabel);
        accountCreationLayout->addWidget(createEmail);
        accountCreationLayout->addWidget(createUsernameLabel);
        accountCreationLayout->addWidget(createUsername);
        accountCreationLayout->addWidget(createPasswordLabel);
        accountCreationLayout->addWidget(createPassword);
        accountCreationLayout->addWidget(backBtn);

        accountCreationWidget->setLayout(accountCreationLayout);
        layout->addWidget(accountCreationWidget);

        mainLayout->addLayout(layout);
        setLayout(mainLayout);

        connect(backBtn, &QPushButton::clicked, this, &LoginDialog::onBackClick);
        connect(createAccount, &AccountLabel::labelClick, this, &LoginDialog::onAccountCreateClick);
        connect(usernameInput, &QLineEdit::returnPressed, this, &LoginDialog::onLoginEnterPressed);
        connect(passwordInput, &QLineEdit::returnPressed, this, &LoginDialog::onLoginEnterPressed);
        connect(createEmail, &QLineEdit::returnPressed, this, &LoginDialog::onAccountCreateEnterPress);
        connect(createUsername, &QLineEdit::returnPressed, this, &LoginDialog::onAccountCreateEnterPress);
        connect(createPassword, &QLineEdit::returnPressed, this, &LoginDialog::onAccountCreateEnterPress);


    }
    void onAccountCreateClick();
    void onBackClick();
    void onLoginEnterPressed();
    void onAccountCreateEnterPress();

    ~LoginDialog() {
        qDebug() << "LoginDialog destructor has been called";
        delete mainLayout;
//        db.close();
    }

private:
    QVBoxLayout* mainLayout;

    QWidget* loginWidget;
    QVBoxLayout* loginLayout;
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    AccountLabel* createAccount;

    QWidget* accountCreationWidget;
    QVBoxLayout* accountCreationLayout;
    QLineEdit* createEmail;
    QLineEdit* createUsername;
    QLineEdit* createPassword;
    QPushButton* backBtn;

    QStackedLayout* layout;

    QSqlDatabase* db;
    MainWindow* window;
};


#endif // LOGINDIALOG_H
