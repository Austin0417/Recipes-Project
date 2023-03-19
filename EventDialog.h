#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>


class EventDialog : public QDialog {
public:
    EventDialog(QWidget* parent = nullptr) : QDialog(parent) {
        layout = new QVBoxLayout(this);
        eventsList = new QTextEdit(this);
        eventsList->setReadOnly(true);
        layout->addWidget(eventsList);
        recordEvents();
    }
    void recordEvents();

private:
    QVBoxLayout* layout;
    QTextEdit* eventsList;
};

#endif // EVENTDIALOG_H
