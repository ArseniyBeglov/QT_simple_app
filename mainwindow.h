#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "student.h"
#include "course.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addCourse();
    void viewCourses();
    void addStudent();
    void viewStudentInfo();
    void enrollStudent();
    void gradeStudent();

private:
    Ui::MainWindow *ui;
    QList<Course> courses;
    QList<Student> students;
};
#endif // MAINWINDOW_H
