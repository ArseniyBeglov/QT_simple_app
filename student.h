#ifndef STUDENT_H
#define STUDENT_H
#include "course.h"
#include <QMessageBox>

struct CourseResult {
    QString courseName;
    double finalGrade;
};

class Student {
public:
    Student(QString name);

    QString name;
    QMap<QString, QList<Grade>> courseGrades;
    QList<CourseResult> courseResults;
    void enrollCourse(const Course &course) {
        if (!courseGrades.contains(course.name)) {
            courseGrades.insert(course.name, QList<Grade>());
        }
    }


    void setGrade(const Course &course, const QString &category, double value) {
        // Проверяем, записан ли студент на указанный курс
        if (courseGrades.contains(course.name)) {
            Grade grade;
            grade.category = category;
            grade.value = value;

            courseGrades[course.name].append(grade);
        } else {
            QMessageBox::warning(nullptr, "Ошибка", QString("Студент не записан на курс %1").arg(course.name));
        }
    }

    void calculateFinalGrades(const QList<Course> &courses) {
        courseResults.clear();

        for (const QString &courseName : courseGrades.keys()) {
            const Course *coursePtr = nullptr;
            for (const Course &course : courses) {
                if (course.name == courseName) {
                    coursePtr = &course;
                    break;
                }
            }
            if (!coursePtr) {
                QMessageBox::warning(nullptr, "Ошибка", QString("Студент не записан на курс %1").arg(courseName));
                continue;
            }

            double finalGrade = 0.0;
            const QList<Grade> &grades = courseGrades.value(courseName);

            for (const Grade &grade : grades) {
                finalGrade += grade.value * coursePtr->gradingFormula.value(grade.category, 0);
            }

            CourseResult result;
            result.courseName = courseName;
            result.finalGrade = finalGrade;
            courseResults.append(result);
        }
    }

};


#endif // STUDENT_H
