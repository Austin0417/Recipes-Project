#include "mainwindow.h"
#include "CalendarDialog.h"
#include "ListViewDelegate.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::setWindowTitle("Recipes Project");
    addBtn = ui->addButton;
    searchBtn = ui->searchButton;
    removeBtn = ui->removeBtn;
    calendarBtn = ui->calendarBtn;
    listView = ui->listView;
    removeBtn->setEnabled(false);
    searchBtn->setEnabled(false);
    addBtn->setIcon(QIcon(":/button_icons/F:/Pictures/plus_icon.png"));
    searchBtn->setIcon(QIcon(":/button_icons/F:/Pictures/magnifying_glass_icon.png"));
    removeBtn->setIcon(QIcon(":/button_icons/F:/Pictures/remove_icon.png"));

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

    model = new RecipeListModel(recipeList);
    listView->setContextMenuPolicy(Qt::CustomContextMenu);
    delegate = new ListViewDelegate(this);
    listView->setItemDelegate(delegate);
    listView->setModel(model);
    listView->show();

    QLabel* background = new QLabel(this);
    QPixmap backgroundImage(":/button_icons/F:/Pictures/Recipes Background Image.PNG");
    background->setScaledContents(true);
    QPixmap scaledBackground = backgroundImage.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    background->setPixmap(scaledBackground);
    background->setAlignment(Qt::AlignCenter);
    background->lower();
    //setStyleSheet("QMainWindow {background-image: url(:/button_icons/F:/Pictures/Recipes Background Image.PNG); background-repeat: no-repeat; background-size: cover;}");

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(calendarBtn, &QPushButton::clicked, this, &MainWindow::onCalendarBtnClicked);
    connect(listView, &QAbstractItemView::doubleClicked, this, &MainWindow::onListItemClicked);
    connect(listView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onListItemRightClicked(QPoint)));
}

MainWindow::~MainWindow()
{
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

RecipeListModel* MainWindow::getModel() {
    return model;
}

QVector<QString>* MainWindow::getRecipeInformation() {
    return &recipeInformation;
}

void MainWindow::updateListView() {
    delete model;
    model = new RecipeListModel(recipeList);
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
    QString nameToRemove = QInputDialog::getText(this, tr("Name of Recipe to Remove: "), tr("Recipe Name: "), QLineEdit::Normal, "", &ok);
    auto it = findRecipe(nameToRemove);
    if (ok && !nameToRemove.isEmpty() && it >= 0) {
        QMessageBox::information(this, "Remove", "Removing recipe...");
        recipeList.remove(it);
        updateListView();

    } else if (!ok) {
        return;
    }
    else if (it < 0) {
        QMessageBox::critical(this, "Error", "Recipe to remove does not exist!");
        return;
    }
    else {
        return;
    }
}

void MainWindow::onAddClicked() {
      RecipeInfoDialog* recipeInfoDialog = new RecipeInfoDialog(this);
      recipeInfoDialog->exec();

    if (recipeInformation.size() < 3) {
        return;
    }
    qDebug() << "Recipe information size is: " << recipeInformation.size();
    Recipe* newRecipe = new Recipe(recipeInformation[0], recipeInformation[1], recipeInformation[2]);
    recipeInformation.clear();
    recipeList.append(newRecipe);
    updateListView();

    qDebug() << "Ingredients: " << newRecipe->getIngredients();
    qDebug() << "Instructions: " << newRecipe->getInstructions();
    qDebug() << "List size is now: " << recipeList.size();
    removeBtn->setEnabled(true);
    searchBtn->setEnabled(true);
}


void MainWindow::onSearchClicked() {
    bool ok;
    QString searchedRecipe = QInputDialog::getText(this, tr("Enter the recipe name that you are looking for"), tr("Recipe Name: "), QLineEdit::Normal, "", &ok);
    int iteratorPos = findRecipe(searchedRecipe);
    if (ok and !searchedRecipe.isEmpty() && iteratorPos >= 0) {
                // Recipe found
                auto it = recipeList.begin();
                it += iteratorPos;
                QMessageBox::information(this, "Recipe found", "Found a recipe with matching name!");
                RecipeDialog* dialog = new RecipeDialog(this);
                dialog->setWidgetText(*it);
                dialog->exec();
                return;
        // Recipe does not exist
    }
    else if (!ok) {
        return;
    }
    else if (iteratorPos < 0) {
        QMessageBox::critical(this, "Error", "Recipe to remove does not exist!");
        return;
    }
    else {
        return;
    }
}

QList<Recipe*> MainWindow::getRecipeList() {
    return recipeList;
}

void MainWindow::onCalendarBtnClicked() {
    CalendarDialog* calendarDialog;
    if (!savedCalendar) {
        calendarDialog = new CalendarDialog(this);
    } else {
        calendarDialog = savedCalendar;
    }
    calendarDialog->exec();
}

void MainWindow::onListItemClicked(const QModelIndex &index) {
    if (index.isValid()) {
        qDebug() << "Clicked index: " << index.row();
        Recipe* recipe = model->data(index, Qt::UserRole).value<Recipe*>();
        RecipeDialog* dialog = new RecipeDialog(this);
        qDebug() << "Name of selected recipe: " << recipe->getName();
        qDebug() << "Ingredients of selected recipe: " << recipe->getIngredients();
        qDebug() << "Instructions of selected recipe: " << recipe->getInstructions();
        dialog->setWidgetText(recipe);
        dialog->exec();

    }
}


void MainWindow::onListItemRightClicked(const QPoint& pos) {
    QModelIndex index = listView->indexAt(pos);
    qDebug() << "User right clicked at index: " << index.row();
    if (index.isValid()) {
        Recipe* recipe = model->data(index, Qt::UserRole).value<Recipe*>();
        QMenu* menu = new QMenu(this);
        QAction* actionView = new QAction("View recipe", this);
        QAction* favoriteView;
        if (recipe->getFavoritedStatus()) {
            favoriteView = new QAction("Remove from favorites", this);
        } else {
            favoriteView = new QAction("Add to favorites", this);
        }
        QAction* removeView = new QAction("Remove", this);
        connect(actionView, &QAction::triggered, this, &MainWindow::viewRecipeRightClick);
        connect(favoriteView, &QAction::triggered, this, &MainWindow::favoriteRecipeRightClick);
        connect(removeView, &QAction::triggered, this, &MainWindow::removeRecipeRightClick);
        menu->addAction(actionView);
        menu->addAction(favoriteView);
        menu->addAction(removeView);
        menu->exec(listView->viewport()->mapToGlobal(pos));
    }

}

void MainWindow::viewRecipeRightClick() {
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QModelIndex index = listView->currentIndex();
        if (index.isValid()) {
            Recipe* recipe = model->data(index, Qt::UserRole).value<Recipe*>();
            RecipeDialog* dialog = new RecipeDialog(this);
            dialog->setWidgetText(recipe);
            dialog->exec();
        }
    }
}

void MainWindow::favoriteRecipeRightClick() {
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QModelIndex index = listView->currentIndex();
        if (index.isValid()) {
            Recipe* recipe = model->data(index, Qt::UserRole).value<Recipe*>();
            int it = findRecipe(recipe->getName());
            if (it >= 0) {
                if (!recipe->getFavoritedStatus()) {
                    recipe->setFavorited(true);
                    recipeList.remove(it);
                    recipeList.prepend(recipe);
                    updateListView();
                    qDebug() << recipe->getName() << " favorites status is now " << recipe->getFavoritedStatus();
                    model->dataChanged(index, index);
                } else {
                    recipe->setFavorited(false);
                    recipeList.remove(it);
                    recipeList.append(recipe);
                    updateListView();
                    model->dataChanged(index, index);
                    qDebug() << recipe->getName() << " favorites status is now " << recipe->getFavoritedStatus();
                }
            }
        }
    }
}

void MainWindow::removeRecipeRightClick() {
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        QModelIndex index = listView->currentIndex();
        if (index.isValid()) {
            Recipe* recipe = model->data(index, Qt::UserRole).value<Recipe*>();
            int it = findRecipe(recipe->getName());
            if (it >= 0) {
                recipeList.remove(it);
                updateListView();
            }
        }
    }
}


void MainWindow::setSavedCalendar(CalendarDialog* calendar) {
    savedCalendar = calendar;
}
