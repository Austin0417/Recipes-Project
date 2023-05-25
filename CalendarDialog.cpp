#include "CalendarDialog.h"


void MyCalendarWidget::addDate(QDate* date) {
    datesWithEvents.append(date);
}

bool CalendarEventFilter::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::MouseMove) {
        if (watched->inherits("QCalendarWidget")) {
            MyCalendarWidget* calendar = qobject_cast<MyCalendarWidget*>(watched);
            QTableView* table = calendar->findChild<QTableView*>();
            QMouseEvent* mEvent = static_cast<QMouseEvent*>(event);
            QPersistentModelIndex index = table->indexAt(mEvent->pos());
            QDate date = index.data().toDate();


            emit onDateHovered(date, index.row(), index.column());
        }
    }
    return QObject::eventFilter(watched, event);
}



void CalendarDialog::onResetBtnClick() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to clear the calendar?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        clearCalendar();
        initializeCalendar();
        connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
        connect(resetBtn, &QPushButton::clicked, this, &CalendarDialog::onResetBtnClick);
        connect(viewEventsBtn, &QPushButton::clicked, this, &CalendarDialog::onViewButtonClick);
        eventDayMapping.clear();
        labelMapping.clear();
        if (window->getLoginStatus()) {
            QSqlQuery query(*db);
            query.prepare("DELETE FROM SavedCalendar WHERE id = ?");
            query.addBindValue(window->getCurrentUserId());
            if (query.exec()) {
                qDebug() << "Successfully deleted all events for user " << window->getCurrentUserId();
            }
        }
    } else {
        qDebug() << "Confirmation to clear was declined.";
        return;
    }
}

void CalendarDialog::onViewButtonClick() {
    events = new EventDialog(this);
    events->exec();
}

QMap<QDate, QList<QString>> CalendarDialog::getEventMapping() {
    return eventDayMapping;
}

void CalendarDialog::fetchUserCalendar() {
        QSqlQuery query(*db);
        query.prepare("SELECT date, event FROM SavedCalendar WHERE id = ?");
        query.addBindValue(window->getCurrentUserId());
        if (query.exec()) {
            qDebug() << "Successfully fetched user calendar!";
            while (query.next()) {
                QDate date = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
                qDebug() << query.value(0).toString();
                QString event = query.value(1).toString();
                eventDayMapping[date].append(event);
                QTextCharFormat format;
                format.setBackground(Qt::red);
                calendar->setDateTextFormat(date, format);
            }
        }

}

void CalendarDialog::initializeCalendar() {

    calendar = new MyCalendarWidget(this);
    resetBtn = new QPushButton("Clear", this);
    viewEventsBtn = new QPushButton("View Event Details", this);
    layout->addWidget(new QLabel("Click on a date to add an event"));
    layout->addWidget(calendar);
    layout->addWidget(viewEventsBtn);
    layout->addWidget(resetBtn);

    if (window->getLoginStatus()) {
        //fetchUserCalendar();
        QTextCharFormat format;
        format.setBackground(Qt::red);
        for (auto it = eventDayMapping.begin(); it != eventDayMapping.end(); ++it) {
            calendar->setDateTextFormat(it.key(), format);
            layout->addWidget(new QLabel(it.key().toString("yyyy-MM-dd") + ": " + QString::number(it.value().size()) + " events"));
        }
    }


    filter = new CalendarEventFilter(this);
    calendar->installEventFilter(filter);

}

void CalendarDialog::clearCalendar() {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}


