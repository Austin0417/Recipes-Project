#include "mainwindow.h"
#include "CalendarDialog.h"
#include "ListViewDelegate.h"
#include "LoginDialog.h"
#include "RecipeDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  MainWindow::setWindowTitle("Recipes Project");
  addBtn = ui->addButton;
  searchBtn = ui->searchButton;
  removeBtn = ui->removeBtn;
  calendarBtn = ui->calendarBtn;
  exitBtn = ui->exitBtn;
  loginBtn = ui->loginBtn;
  listView = ui->listView;
  removeBtn->setEnabled(false);
  searchBtn->setEnabled(false);
  addBtn->setIcon(QIcon(":/button_icons/F:/Pictures/plus_icon.png"));
  searchBtn->setIcon(
      QIcon(":/button_icons/F:/Pictures/magnifying_glass_icon.png"));
  removeBtn->setIcon(QIcon(":/button_icons/F:/Pictures/remove_icon.png"));


  listView->setStyleSheet("background-color: #a75a00; color: white;");

  model = new RecipeListModel(recipeList);
  listView->setContextMenuPolicy(Qt::CustomContextMenu);
  delegate = new ListViewDelegate(this);
  listView->setItemDelegate(delegate);
  listView->setModel(model);
  listView->show();


  this->setStyleSheet("MainWindow {"
                      "background-image: url(:/button_icons/F:/Pictures/Recipes Background Image.PNG);"
                      " background-repeat: repeat;"
                      "background-color: #F0F0F0;"
                      "}"
                      "QLabel {"
                      "color: #333333;"
                      "}"
                      "QPushButton {"
                      "background-color: #502412;"
                      "color: #FFFFFF;"
                      "padding: 8px 16px;"
                      "border: none;"
                      "border-radius: 4px;"
                      "}");

  QPalette addPalette = addBtn->palette();
  QPalette removePalette = removeBtn->palette();
  QPalette searchPalette = searchBtn->palette();

  addPalette.setColor(QPalette::Button, QColor(0, 170, 0));
  addBtn->setAutoFillBackground(true);
  addBtn->setFlat(true);
  addBtn->setPalette(addPalette);
  addBtn->update();

  removePalette.setColor(QPalette::Button, QColor(165, 2, 35));
  removeBtn->setAutoFillBackground(true);
  removeBtn->setFlat(true);
  removeBtn->setPalette(removePalette);
  removeBtn->update();

  searchPalette.setColor(QPalette::Button, QColor(0, 183, 200));
  searchBtn->setAutoFillBackground(true);
  searchBtn->setFlat(true);
  searchBtn->setPalette(searchPalette);
  searchBtn->update();

  addBtn->setIconSize(QSize(30, 30));
  removeBtn->setIconSize(QSize(30, 30));
  searchBtn->setIconSize(QSize(30, 30));

  db = QSqlDatabase::addDatabase("QPSQL");
  db.setHostName("localhost");
  db.setDatabaseName("users");
  db.setUserName("postgres");
  db.setPassword("I<3games");

  if (db.open()) {
      qDebug() << "Opened database!";
  } else {
      qDebug() << "Failed to open database!";
  }

  connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddClicked);
  connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
  connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
  connect(exitBtn, &QPushButton::clicked, this, &MainWindow::onExitClick);
  connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLoginClick);
  connect(calendarBtn, &QPushButton::clicked, this,
          &MainWindow::onCalendarBtnClicked);
  connect(listView, &QAbstractItemView::doubleClicked, this,
          &MainWindow::onListItemClicked);
  connect(listView, SIGNAL(customContextMenuRequested(QPoint)), this,
          SLOT(onListItemRightClicked(QPoint)));
}

MainWindow::~MainWindow() {
    db.close();
    qDebug() << "Closing database connection...";
    delete ui;

}

int MainWindow::findRecipe(QString name) {
  for (auto it = recipeList.begin(); it != recipeList.end(); ++it) {
    if ((*it)->getName() == name) {
      return it - recipeList.begin();
    }
  }
  return -1;
}

RecipeListModel *MainWindow::getModel() { return model; }

QVector<QString> *MainWindow::getRecipeInformation() {
  return &recipeInformation;
}

void MainWindow::updateListView(QModelIndex *index) {
  delete model;
  model = new RecipeListModel(recipeList);
  if (index) {
    model->setData(*index, QBrush(Qt::yellow), Qt::ForegroundRole);
  }
  listView->setModel(model);
  listView->show();
  if (recipeList.isEmpty()) {
    removeBtn->setEnabled(false);
    searchBtn->setEnabled(false);
  } else {
    removeBtn->setEnabled(true);
    searchBtn->setEnabled(true);
  }
}

void MainWindow::onRemoveClicked() {
  bool ok;
  QString nameToRemove =
      QInputDialog::getText(this, tr("Remove Recipe "), tr("Recipe Name: "),
                            QLineEdit::Normal, "", &ok);
  auto it = findRecipe(nameToRemove);
  if (ok && !nameToRemove.isEmpty() && it >= 0) {
    QMessageBox::information(this, "Remove", "Removing recipe...");
    Recipe* recipe = *(recipeList.begin() + it);
    recipeList.remove(it);
    updateListView(nullptr);
    if (getLoginStatus()) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM UserSavedRecipes WHERE name = ? AND ingredients = ? AND instructions = ? AND id = ?");
        query.addBindValue(nameToRemove);
        query.addBindValue(recipe->getIngredients());
        query.addBindValue(recipe->getInstructions());
        query.addBindValue(getCurrentUserId());
        if (query.exec()) {
            qDebug() << "Removing " << nameToRemove << " from the database...";
        }
    }

  } else if (!ok) {
    return;
  } else if (it < 0) {
    QMessageBox::critical(this, "Error", "Recipe to remove does not exist!");
    return;
  } else {
    return;
  }
}

void MainWindow::onExitClick() {
  QMessageBox::StandardButton message;
  message =
      QMessageBox::question(this, "Exit", "Are you sure you want to exit?",
                            QMessageBox::Yes | QMessageBox::No);
  if (message == QMessageBox::Yes) {
    if (close()) {
      return;
    }
  } else {
    return;
  }
}

void MainWindow::onLoginClick() {
  if (getLoginStatus()) {
    QMessageBox::StandardButton message;
    message = QMessageBox::question(this, "Log out",
                                    "Are you sure you want to log out?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (message == QMessageBox::Yes) {
      setLoginStatus(false);
      setCurrentUserId(-1);
      loginBtn->setText("Login");
      recipeList.clear();
      updateListView(nullptr);
      savedCalendar = nullptr;
      return;
    } else {
      return;
    }
  }
  LoginDialog *login = new LoginDialog(&db, this);
  login->exec();
  delete login;
  if (getLoginStatus()) {
    loginBtn->setText("Logout");
  }
}

void MainWindow::onAddClicked() {
  RecipeInfoDialog *recipeInfoDialog = new RecipeInfoDialog(this);
  recipeInfoDialog->exec();

  if (recipeInformation.size() < 3) {
    return;
  }
  qDebug() << "Recipe information size is: " << recipeInformation.size();
  Recipe *newRecipe = new Recipe(recipeInformation[0], recipeInformation[1],
                                 recipeInformation[2]);

  recipeList.append(newRecipe);
  updateListView(nullptr);

  if (getLoginStatus()) {
      QSqlQuery query(db);
      query.prepare("INSERT INTO UserSavedRecipes (id, name, ingredients, instructions, favorited) VALUES (?, ?, ?, ?, ?)");
      query.addBindValue(getCurrentUserId());
      query.addBindValue(recipeInformation[0]);
      query.addBindValue(recipeInformation[1]);
      query.addBindValue(recipeInformation[2]);
      query.addBindValue(false);
      if (query.exec()) {
          qDebug() << "Successfully inserted new recipe into user's account!";
      }
  }
  recipeInformation.clear();
  qDebug() << "Ingredients: " << newRecipe->getIngredients();
  qDebug() << "Instructions: " << newRecipe->getInstructions();
  qDebug() << "List size is now: " << recipeList.size();
  removeBtn->setEnabled(true);
  searchBtn->setEnabled(true);
}

void MainWindow::onSearchClicked() {
  bool ok;
  QString searchedRecipe =
      QInputDialog::getText(this, tr("Search Recipe"), tr("Recipe Name: "),
                            QLineEdit::Normal, "", &ok);
  int iteratorPos = findRecipe(searchedRecipe);
  if (ok and !searchedRecipe.isEmpty() && iteratorPos >= 0) {
    // Recipe found
    auto it = recipeList.begin();
    Recipe* recipe = *(recipeList.begin() + iteratorPos);
    it += iteratorPos;
    QMessageBox::information(this, "Recipe found",
                             "Found a recipe with matching name!");
    RecipeDialog *dialog = new RecipeDialog(recipe, &db, this);
    dialog->setWidgetText(*it);
    dialog->exec();
    return;
    // Recipe does not exist
  } else if (!ok) {
    return;
  } else if (iteratorPos < 0) {
    QMessageBox::critical(this, "Error", "Recipe to remove does not exist!");
    return;
  } else {
    return;
  }
}

QList<Recipe *> MainWindow::getRecipeList() { return recipeList; }

void MainWindow::onCalendarBtnClicked() {
  CalendarDialog *calendarDialog;
  if (getLoginStatus()) {
      calendarDialog = new CalendarDialog(&db, userSavedEvents, this);
  }
  else if (!savedCalendar) {
    calendarDialog = new CalendarDialog(&db, nullptr, this);
  } else {
    calendarDialog = savedCalendar;
  }
  calendarDialog->exec();
}

void MainWindow::onListItemClicked(const QModelIndex &index) {
  if (index.isValid()) {
    qDebug() << "Clicked index: " << index.row();
    Recipe *recipe = model->data(index, Qt::UserRole).value<Recipe *>();
    RecipeDialog *dialog = new RecipeDialog(recipe, &db, this);
    qDebug() << "Name of selected recipe: " << recipe->getName();
    qDebug() << "Ingredients of selected recipe: " << recipe->getIngredients();
    qDebug() << "Instructions of selected recipe: "
             << recipe->getInstructions();
    dialog->setWidgetText(recipe);
    dialog->exec();
  }
}

void MainWindow::onListItemRightClicked(const QPoint &pos) {
  QModelIndex index = listView->indexAt(pos);
  qDebug() << "User right clicked at index: " << index.row();
  if (index.isValid()) {
    Recipe *recipe = model->data(index, Qt::UserRole).value<Recipe *>();
    QMenu *menu = new QMenu(this);
    QAction *actionView = new QAction("View recipe", this);
    QAction *favoriteView;
    if (recipe->getFavoritedStatus()) {
      favoriteView = new QAction("Remove from favorites", this);
    } else {
      favoriteView = new QAction("Add to favorites", this);
    }
    QAction *removeView = new QAction("Remove", this);
    connect(actionView, &QAction::triggered, this,
            &MainWindow::viewRecipeRightClick);
    connect(favoriteView, &QAction::triggered, this,
            &MainWindow::favoriteRecipeRightClick);
    connect(removeView, &QAction::triggered, this,
            &MainWindow::removeRecipeRightClick);
    menu->addAction(actionView);
    menu->addAction(favoriteView);
    menu->addAction(removeView);
    menu->exec(listView->viewport()->mapToGlobal(pos));
  }
}

void MainWindow::viewRecipeRightClick() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    QModelIndex index = listView->currentIndex();
    if (index.isValid()) {
      Recipe *recipe = model->data(index, Qt::UserRole).value<Recipe *>();
      RecipeDialog *dialog = new RecipeDialog(recipe, &db, this);
      dialog->setWidgetText(recipe);
      dialog->exec();
    }
  }
}

void MainWindow::favoriteRecipeRightClick() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    QModelIndex index = listView->currentIndex();
    if (index.isValid()) {
      Recipe *recipe = model->data(index, Qt::UserRole).value<Recipe *>();
      int it = findRecipe(recipe->getName());
      QSqlQuery query(db);
      if (it >= 0) {
        if (!recipe->getFavoritedStatus()) {
          recipe->setFavorited(true);
          QIcon icon = model->data(index, Qt::DecorationRole).value<QIcon>();
          recipeList.remove(it);
          recipeList.prepend(recipe);
          updateListView(&index);
          qDebug() << recipe->getName() << " favorites status is now "
                   << recipe->getFavoritedStatus();
          if (getLoginStatus()) {
              query.prepare("UPDATE UserSavedRecipes SET favorited = ? WHERE id = ? AND name = ? AND ingredients = ? AND instructions = ?");
              query.addBindValue(true);
              query.addBindValue(getCurrentUserId());
              query.addBindValue(recipe->getName());
              query.addBindValue(recipe->getIngredients());
              query.addBindValue(recipe->getInstructions());
              if (query.exec()) {
                  qDebug() << "Updated favorites status for " << recipe->getName() << ". Status is now " << recipe->getFavoritedStatus() << "for user id " << getCurrentUserId();
              }
          }

        } else {
          recipe->setFavorited(false);
          QStringList originalNameList = recipe->getName().split(
              " ", Qt::KeepEmptyParts, Qt::CaseSensitive);
          recipeList.remove(it);
          recipeList.append(recipe);
          updateListView(&index);
          if (getLoginStatus()) {
              query.prepare("UPDATE UserSavedRecipes SET favorited = ? WHERE name = ? AND ingredients = ? AND instructions = ? AND id = ?");
              query.addBindValue(false);
              query.addBindValue(recipe->getName());
              query.addBindValue(recipe->getIngredients());
              query.addBindValue(recipe->getInstructions());
              query.addBindValue(getCurrentUserId());
              if (query.exec()) {
                  qDebug() << "Updated favorites status for " << recipe->getName() << ". Status is now " << recipe->getFavoritedStatus();
              }
          }
          qDebug() << recipe->getName() << " favorites status is now "
                   << recipe->getFavoritedStatus();
        }
      }
    }
  }
}

void MainWindow::removeRecipeRightClick() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    QModelIndex index = listView->currentIndex();
    if (index.isValid()) {
      Recipe *recipe = model->data(index, Qt::UserRole).value<Recipe *>();
      int it = findRecipe(recipe->getName());
      if (it >= 0) {
        recipeList.remove(it);
        updateListView(nullptr);
        if (getLoginStatus()) {
            QSqlQuery query(db);
            query.prepare("DELETE FROM UserSavedRecipes WHERE id = ? AND name = ? AND ingredients = ? AND instructions = ?");
            query.addBindValue(getCurrentUserId());
            query.addBindValue(recipe->getName());
            query.addBindValue(recipe->getIngredients());
            query.addBindValue(recipe->getInstructions());
            if (query.exec()) {
                qDebug() << "Removing " << recipe->getName() << " from the database...";
            }
        }
      }
    }
  }
}

void MainWindow::setSavedCalendar(CalendarDialog *calendar) {
  savedCalendar = calendar;
}

void MainWindow::setCurrentUserId(int id) { currentUserId = id; }

void MainWindow::setLoginStatus(bool status) { isLoggedIn = status; }

int MainWindow::getCurrentUserId() { return currentUserId; }

bool MainWindow::getLoginStatus() { return isLoggedIn; }

void MainWindow::setRecipeList(QList<Recipe *> recipeList_) {
  recipeList = recipeList_;
  updateListView(nullptr);
}

void MainWindow::setEvents(QMap<QDate, QList<QString>>* events) {
    userSavedEvents = events;
}


