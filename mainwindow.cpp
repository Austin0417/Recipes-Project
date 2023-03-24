#include "mainwindow.h"
#include "CalendarDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    listView->setModel(model);
    listView->show();
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(calendarBtn, &QPushButton::clicked, this, &MainWindow::onCalendarBtnClicked);
    connect(listView, &QAbstractItemView::clicked, this, &MainWindow::onListItemClicked);
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

void MainWindow::onRemoveClicked() {
    bool ok;
    QString nameToRemove = QInputDialog::getText(this, tr("Name of Recipe to Remove: "), tr("Recipe Name: "), QLineEdit::Normal, "", &ok);
    auto it = findRecipe(nameToRemove);
    if (ok && !nameToRemove.isEmpty() && it >= 0) {
        QMessageBox::information(this, "Remove", "Removing recipe...");
        recipeList.remove(it);
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

    } else {
        QMessageBox::critical(this, "Error", "Recipe to remove does not exist!");
    }
}

void MainWindow::onAddClicked() {
    bool ok;
    QString name;
    QString addRecipe = QInputDialog::getText(this, tr("Name of Recipe:"), tr("Recipe Name: "), QLineEdit::Normal, "", &ok);
    if (ok and !addRecipe.isEmpty()) {
        name = addRecipe;
    } else if (!ok) {
        return;
    } else {
        QMessageBox::critical(this, "Error", "Invalid or empty name!");
        return;
    }
    ok = false;
    QString addIngredients = QInputDialog::getText(this, tr("Recipe Ingredients"), tr("Ingredients: "), QLineEdit::Normal, "", &ok);
    QString ingredients;
    if (ok and !addIngredients.isEmpty()) {
        ingredients = addIngredients;
    } else {
        QMessageBox::critical(this, "Error", "Invalid or empty ingredients!");
        return;
    }

    ok = false;
    QString addInstructions = QInputDialog::getText(this, tr("Recipe Instructions"), tr("Instructions: "), QLineEdit::Normal, "", &ok);
    QString instructions;
    if (ok and !addInstructions.isEmpty()) {
        instructions = addInstructions;
    } else {
        QMessageBox::critical(this, "Error", "Invalid or empty instructions!");
        return;
    }

    Recipe* newRecipe = new Recipe(name, ingredients, instructions);

    recipeList.append(newRecipe);
    delete model;
    model = new RecipeListModel(recipeList);
    listView->setModel(model);
    listView->show();

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
    } else {
        QMessageBox::critical(this, "Error", "Recipe does not exist!");
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

void MainWindow::setSavedCalendar(CalendarDialog* calendar) {
    savedCalendar = calendar;
}
