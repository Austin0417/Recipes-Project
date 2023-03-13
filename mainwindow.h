#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include "Recipe.h"
#include "RecipeListModel.h"
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



class MyDate : public QDate {
public:
    MyDate() : QDate() {}
    void setEvent(QString event_) {
        event = event_;
    }

private:
    QString event;
};

class CalendarDialog;

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
    void onListItemClicked(const QModelIndex& index);
    int findRecipe(QString name);
    QList<Recipe*> getRecipeList();
    void setSavedCalendar(CalendarDialog* calendar);
private:
    Ui::MainWindow *ui;
    QPushButton* addBtn;
    QPushButton* searchBtn;
    QList <Recipe*> recipeList;
    QListView* listView;
    RecipeListModel* model;
    QPushButton* removeBtn;
    QPushButton* calendarBtn;
    CalendarDialog* savedCalendar = nullptr;
};

#endif // MAINWINDOW_H
