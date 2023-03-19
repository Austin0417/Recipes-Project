#include "EventDialog.h"
#include "CalendarDialog.h"

void EventDialog::recordEvents() {
    CalendarDialog* parentWidget = qobject_cast<CalendarDialog*>(parent());
    QMap<QDate, QList<QString>> mapping = parentWidget->getEventMapping();
    for (auto it = mapping.begin(); it != mapping.end(); ++it) {
        if (it.value().size() > 1) {
            eventsList->append(it.key().toString("yyyy-MM-dd") + ": " + QString::number(it.value().size()) + " events total");
            for (int i = 0; i < it.value().size(); i++) {
                eventsList->append("Event #" + QString::number(i + 1) + ": " + it.value()[i]);
            }
            eventsList->append("--------------------------------------------");
        } else {
            eventsList->append(it.value()[0]);
            qDebug() <<"Event info: " + it.value()[0];
        }
    }
}
