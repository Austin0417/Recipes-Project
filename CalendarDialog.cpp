#include "CalendarDialog.h"

void CalendarDialog::onResetBtnClick() {
    delete calendar;
    calendar = new QCalendarWidget(this);
    layout->addWidget(calendar);
    connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
}
