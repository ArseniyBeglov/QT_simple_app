#ifndef ADDCIURSEANDSTUDENTDIALOG_H
#define ADDCIURSEANDSTUDENTDIALOG_H
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include "course.h"

class AddCourseAndStudentDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddCourseAndStudentDialog(const QList<Course> &courses, QWidget *parent = nullptr) : QDialog(parent), m_courses(courses) {
        setWindowTitle("Записать студента на курс");

        QVBoxLayout *layout = new QVBoxLayout(this);
        for (const Course &course : m_courses) {
            QCheckBox *checkBox = new QCheckBox(course.name, this);
            layout->addWidget(checkBox);
            m_checkBoxes.append(checkBox);
        }
        QHBoxLayout *buttonLayout = new QHBoxLayout;

        QPushButton *okButton = new QPushButton("OK", this);
        connect(okButton, &QPushButton::clicked, this, &AddCourseAndStudentDialog::accept);
        buttonLayout->addWidget(okButton);

        QPushButton *cancelButton = new QPushButton("Отмена", this);
        connect(cancelButton, &QPushButton::clicked, this, &AddCourseAndStudentDialog::reject);
        buttonLayout->addWidget(cancelButton);

        layout->addLayout(buttonLayout);
    }

    QList<QString> selectedCourses() const {
        QList<QString> selectedCourses;
        for (int i = 0; i < m_courses.size(); ++i) {
            if (m_checkBoxes.at(i)->isChecked()) {
                selectedCourses.append(m_courses.at(i).name);
            }
        }
        return selectedCourses;
    }

private:
    QList<Course> m_courses;
    QList<QCheckBox*> m_checkBoxes;
};

#endif // ADDCIURSEANDSTUDENTDIALOG_H
