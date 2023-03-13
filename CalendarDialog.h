#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H
#include "mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QCalendarWidget>
#include <QPushButton>
#include <QInputDialog>
#include <QLabel>
class CalendarDialog : public QDialog {
    Q_OBJECT
public:
    CalendarDialog(QWidget* parent = nullptr) : QDialog(parent) {
        layout = new QVBoxLayout(this);
        calendar = new QCalendarWidget(this);
        resetBtn = new QPushButton("Clear", this);
        layout->addWidget(calendar);
        layout->addWidget(resetBtn);
        connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
        connect(resetBtn, &QPushButton::clicked, this, &CalendarDialog::onResetBtnClick);
        connect(this, SIGNAL(rejected()), this, SLOT(handleExit()));
    }
    void onResetBtnClick();
public slots:
    void handleExit() {
        qDebug() << "Exiting calendar...";
        MainWindow* window = qobject_cast<MainWindow*>(parent());
        if (window) {
           window->setSavedCalendar(this);
        }
    }
    void onDateClicked(QDate date) {
//        QCalendarWidget* calendar = qobject_cast<QCalendarWidget*>(sender());
        if (calendar) {
            bool ok;
            QString event = QInputDialog::getText(this, tr("Event details"), tr("Enter event details: "), QLineEdit::Normal, "", &ok);
            if (ok and !event.isEmpty()) {
                QLabel* details = new QLabel(event, calendar);
                calendar->setDateTextFormat(date, QTextCharFormat());
                QTextCharFormat format;
                format.setBackground(Qt::red);
                calendar->setDateTextFormat(date, format);
                layout->addWidget(details);
            }
        }
    }
private:
    QCalendarWidget* calendar;
    QPushButton* resetBtn;
    QVBoxLayout* layout;
};


#endif // CALENDARDIALOG_H
