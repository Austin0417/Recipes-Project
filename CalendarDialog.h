#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H
#include "mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QCalendarWidget>
#include <QPushButton>
#include <QInputDialog>
#include <QLabel>
#include "EventDialog.h"
class CalendarDialog : public QDialog {
    Q_OBJECT
public:
    CalendarDialog(QWidget* parent = nullptr) : QDialog(parent) {
        layout = new QVBoxLayout(this);
        initializeCalendar();
        connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
        connect(resetBtn, &QPushButton::clicked, this, &CalendarDialog::onResetBtnClick);
        connect(this, SIGNAL(rejected()), this, SLOT(handleExit()));
        connect(viewEventsBtn, &QPushButton::clicked, this, &CalendarDialog::onViewButtonClick);
    }
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
            if (ok and !event.isEmpty()) {

                QTextCharFormat format;
                format.setBackground(Qt::red);
                calendar->setDateTextFormat(date, format);

                QString textToDisplay = date.toString("yyyy-MM-dd") + ": " + event;
                eventDayMapping[date].append(textToDisplay);
                QLabel* details = new QLabel(date.toString("yyyy-MM-dd") + ": " + QString::number(eventDayMapping[date].size()) + " events");

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
    QCalendarWidget* calendar;
    QPushButton* resetBtn;
    QVBoxLayout* layout;
    QMap<QDate, QList<QString>> eventDayMapping;
    QMap<QDate, QVector<QLabel*>> labelMapping;
    QPushButton* viewEventsBtn;
    EventDialog* events;
};


#endif // CALENDARDIALOG_H
