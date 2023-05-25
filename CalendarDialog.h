#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H
#include "mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QCalendarWidget>
#include <QPushButton>
#include <QInputDialog>
#include <QTableView>
#include <QLabel>
#include "EventDialog.h"


class CalendarEventFilter : public QObject {
    Q_OBJECT
public:
    CalendarEventFilter(QWidget* parent = nullptr) : QObject(parent) {}
    bool eventFilter(QObject* watched, QEvent* event) override;

signals:
    void onDateHovered(QDate date, int row, int column);
};


class MyCalendarWidget : public QCalendarWidget {
    Q_OBJECT
public:
    MyCalendarWidget(QWidget* parent = nullptr) : QCalendarWidget(parent) {}
    void addDate(QDate* date);
private:
    QVector<QDate*> datesWithEvents;

};

class CalendarDialog : public QDialog {
    Q_OBJECT
public:
    CalendarDialog(QSqlDatabase* db_, QMap<QDate, QList<QString>>* events, QWidget* parent_ = nullptr) : db(db_), QDialog(parent_) {
        this->setWindowTitle("Calendar");
        window = qobject_cast<MainWindow*>(parent());


        if (events) {
            eventDayMapping = *events;
        }

        layout = new QVBoxLayout(this);
        initializeCalendar();
        connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
        connect(resetBtn, &QPushButton::clicked, this, &CalendarDialog::onResetBtnClick);
        connect(this, SIGNAL(rejected()), this, SLOT(handleExit()));
        connect(viewEventsBtn, &QPushButton::clicked, this, &CalendarDialog::onViewButtonClick);
        connect(filter, &CalendarEventFilter::onDateHovered, this, [=](const QDate& date, int row, int column) {
           qDebug() << date.toString("yyyy-MM-dd");
           qDebug() << row << ", " << column;
        });
    }

    void fetchUserCalendar();
    void initializeCalendar();
    void onResetBtnClick();
    void onViewButtonClick();
    void clearLabels();
    void clearCalendar();
    QMap<QDate, QList<QString>> getEventMapping();


public slots:
    void handleExit() {
        qDebug() << "Exiting calendar...";
        MainWindow* window = qobject_cast<MainWindow*>(parent());
        if (window) {
           window->setSavedCalendar(this);
        }
    }
    void onDateClicked(QDate date) {
        if (calendar) {
            bool ok;
            QString event = QInputDialog::getText(this, tr("Event details"), tr("Enter event details: "), QLineEdit::Normal, "", &ok);
            if (ok && !event.isEmpty()) {

                QTextCharFormat format;
                format.setBackground(Qt::red);
                calendar->setDateTextFormat(date, format);

                QString textToDisplay = date.toString("yyyy-MM-dd") + ": " + event;
                eventDayMapping[date].append(textToDisplay);
                QLabel* details = new QLabel(date.toString("yyyy-MM-dd") + ": " + QString::number(eventDayMapping[date].size()) + " events");

                if (window->getLoginStatus()) {
                    QSqlQuery query(*db);
                    query.prepare("INSERT INTO SavedCalendar (id, date, event) VALUES (?, ?, ?)");
                    query.addBindValue(window->getCurrentUserId());
                    query.addBindValue(date.toString("yyyy-MM-dd"));
                    query.addBindValue(event);
                    if (query.exec()) {
                        qDebug() << "Added event to database";
                    }
                }

                if (labelMapping[date].size() >= 1) {
                    for (auto it = labelMapping[date].begin(); it != labelMapping[date].end(); ++it) {
                        layout->removeWidget(*it);
                        delete *it;
                    }
                    labelMapping[date].clear();
                }

                labelMapping[date].append(details);
                layout->addWidget(details);
            }
        }
    }
private:
    MyCalendarWidget* calendar;
    QPushButton* resetBtn;
    QVBoxLayout* layout;
    QMap<QDate, QList<QString>> eventDayMapping;
    QMap<QDate, QVector<QLabel*>> labelMapping;
    QPushButton* viewEventsBtn;
    EventDialog* events;
    CalendarEventFilter* filter;

    QSqlDatabase* db;
    MainWindow* window;
};


#endif // CALENDARDIALOG_H
