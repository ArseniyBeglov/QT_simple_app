#ifndef COURSE_H
#define COURSE_H
#include <QString>
#include <QList>
#include <QMap>

struct Grade {
    QString category;
    double value;
};

class Course {
public:
    Course(QString name);
    QString name;
    QList<Grade> grades;
    QMap<QString, double> gradingFormula;

    void addGrade(QString category, double value);
};

#endif // COURSE_H
