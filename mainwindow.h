#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include "Recipe.h"
#include "RecipeListModel.h"
#include "RecipeInfoDialog.h"
#include "RecipeDialog.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QInputDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QListView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QCalendarWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class CalendarDialog;
class ListViewDelegate;
class LoginDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onSearchClicked();
    void onAddClicked();
    void onRemoveClicked();
    void onCalendarBtnClicked();
    void onExitClick();
    void onLoginClick();

    void onListItemClicked(const QModelIndex& index);
    int findRecipe(QString name);
    QList<Recipe*> getRecipeList();
    void setSavedCalendar(CalendarDialog* calendar);
    QVector<QString>* getRecipeInformation();

    void viewRecipeRightClick();
    void favoriteRecipeRightClick();
    void removeRecipeRightClick();

    void updateListView(QModelIndex* index);
    RecipeListModel* getModel();

    int getCurrentUserId();
    void setCurrentUserId(int id);
    void setLoginStatus(bool status);
    void setRecipeList(QList<Recipe*> recipeList);
    bool getLoginStatus();

public slots:
    void onListItemRightClicked(const QPoint& pos);
private:
    Ui::MainWindow* ui;

    QPushButton* addBtn;
    QPushButton* searchBtn;
    QPushButton* removeBtn;
    QPushButton* calendarBtn;
    QPushButton* exitBtn;
    QPushButton* loginBtn;

    QList <Recipe*> recipeList;
    QListView* listView;
    RecipeListModel* model;
    CalendarDialog* savedCalendar = nullptr;
    QVector<QString> recipeInformation;
    ListViewDelegate* delegate = nullptr;

    bool isLoggedIn = false;
    int currentUserId = -1;
};

#endif // MAINWINDOW_H
