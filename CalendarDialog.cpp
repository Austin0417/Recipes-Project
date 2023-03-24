#include "CalendarDialog.h"

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

void CalendarDialog::initializeCalendar() {
    calendar = new QCalendarWidget(this);
    resetBtn = new QPushButton("Clear", this);
    viewEventsBtn = new QPushButton("View Event Details", this);
    layout->addWidget(new QLabel("Click on a date to add an event"));
    layout->addWidget(calendar);
    layout->addWidget(viewEventsBtn);
    layout->addWidget(resetBtn);

}

void CalendarDialog::clearCalendar() {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}
