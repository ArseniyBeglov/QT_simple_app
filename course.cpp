#include "course.h"

Course::Course(QString name) : name(name) {}

void Course::addGrade(QString category, double value) {
    grades.append({category, value});
}
