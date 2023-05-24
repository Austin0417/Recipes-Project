#include "LoginDialog.h"
#include <algorithm>


void AccountLabel::enterEvent(QEnterEvent* event) {
    QLabel::enterEvent(event);
    setCursor(Qt::PointingHandCursor);
}

void AccountLabel::leaveEvent(QEvent* event) {
    QLabel::leaveEvent(event);
    unsetCursor();
}

void AccountLabel::mousePressEvent(QMouseEvent *event) {
  QLabel::mousePressEvent(event);
  emit labelClick();
}

void LoginDialog::onAccountCreateClick() {
  qDebug() << "Creating account...";
  layout->setCurrentIndex(1);
  setWindowTitle("Create Account");
  usernameInput->clear();
  passwordInput->clear();
}

void LoginDialog::onBackClick() {
    layout->setCurrentIndex(0);
    setWindowTitle("Login");
    createEmail->clear();
    createUsername->clear();
    createPassword->clear();
}

void LoginDialog::onLoginEnterPressed() {
    if (!usernameInput->text().isEmpty() && !passwordInput->text().isEmpty()) {
        QString username = usernameInput->text();
        QString password = passwordInput->text();
        QSqlQuery query(*db);
        query.prepare("SELECT id FROM accounts WHERE (username = ? OR email = ?) AND password = ?");
        query.addBindValue(username);
        query.addBindValue(username);
        query.addBindValue(password);
        if (query.exec()) {
            if (query.size() > 0) {
                if (query.next()) {
                    int id = query.value("id").toInt();
                    qDebug() << "ID is " << id;
                    window->setCurrentUserId(id);
                    window->setLoginStatus(true);
                    QMessageBox::information(this, "Success", "Login was successful!");
                    query.clear();
                    query.prepare("SELECT name, ingredients, instructions, favorited FROM UserSavedRecipes WHERE id = ?");
                    query.addBindValue(id);
                    if (query.exec()) {
                        QList<Recipe*> recipes;
                        while (query.next()) {
                            QString recipeName = query.value(0).toString();
                            QString recipeIngredients = query.value(1).toString();
                            QString recipeInstructions = query.value(2).toString();
                            bool favoritedStatus = query.value(3).toBool();
                            Recipe* recipe = new Recipe(recipeName, recipeIngredients, recipeInstructions);
                            recipe->setFavorited(favoritedStatus);
                            recipes.append(recipe);
                        }
                        std::reverse(recipes.begin(), recipes.end());
                        window->setRecipeList(recipes);
                    }
                }
            } else {
                QMessageBox::information(this, "Error logging in", "Invalid credentials, please try again.");
            }
            close();
        }
    } else {
        QMessageBox::information(this, "Error logging in", "One or more fields are empty. Please try again.");
    }

}

void LoginDialog::onAccountCreateEnterPress() {
    if (!createEmail->text().isEmpty() && !createUsername->text().isEmpty() && !createPassword->text().isEmpty()) {
        QSqlQuery query(*db);
        query.prepare("INSERT INTO accounts (email, username, password) VALUES (?, ?, ?)");
        query.addBindValue(createEmail->text());
        query.addBindValue(createUsername->text());
        query.addBindValue(createPassword->text());

        if (query.exec()) {
            QMessageBox::information(this, "Account create success", "Sucessfully created account!");
            query.clear();
            query.prepare("SELECT id FROM accounts WHERE (email = ? AND username = ? AND password = ?)");
            query.addBindValue(createEmail->text());
            query.addBindValue(createUsername->text());
            query.addBindValue(createPassword->text());
            int id = -1;
            if (query.exec()) {
                if (query.next()) {
                    qDebug() << "Successfully established user id!";
                    id = query.value("id").toInt();
                    QList<Recipe*> currentRecipeList = window->getRecipeList();
                    query.clear();
                    for (auto it = currentRecipeList.begin(); it != currentRecipeList.end(); ++it) {
                        query.prepare("INSERT INTO UserSavedRecipes (id, name, ingredients, instructions, favorited) VALUES (?, ?, ?, ?, ?)");
                        query.addBindValue(id);
                        query.addBindValue((*it)->getName());
                        query.addBindValue((*it)->getIngredients());
                        query.addBindValue((*it)->getInstructions());
                        query.addBindValue((*it)->getFavoritedStatus());
                        if (query.exec()) {
                            qDebug() << "Added recipe " << (*it)->getName();
                            query.clear();
                        }
                    }
                }

                window->setLoginStatus(true);
                window->setCurrentUserId(id);
            }
            close();
        } else {
            qDebug() << "Couldn't create account!";
        }

    } else {
        QMessageBox::information(this, "Error creating account", "One or more fields are empty. Please try again.");
    }
}
