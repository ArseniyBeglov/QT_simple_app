#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include<QLabel>
#include<QCheckBox>
#include "AddCourseDialog.h"
#include "AddCiurseAndStudentDialog.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(ui->centralwidget);

    QPushButton *addCourseButton = new QPushButton("Добавить курс");
    QPushButton *viewCoursesButton = new QPushButton("Посмотреть курсы");
    QPushButton *addStudentButton = new QPushButton("Добавить студента");
    QPushButton *viewStudentInfoButton = new QPushButton("Информация о студенте");
    QPushButton *enrollStudentButton = new QPushButton("Записать студента на курс");
    QPushButton *gradeStudentButton = new QPushButton("Поставить оценку студенту");
    layout->addWidget(addCourseButton);
    layout->addWidget(viewCoursesButton);
    layout->addWidget(addStudentButton);
    layout->addWidget(viewStudentInfoButton);
    layout->addWidget(enrollStudentButton);
    layout->addWidget(gradeStudentButton);
    connect(addCourseButton, &QPushButton::clicked, this, &MainWindow::addCourse);
    connect(viewCoursesButton, &QPushButton::clicked, this, &MainWindow::viewCourses);
    connect(addStudentButton, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(viewStudentInfoButton, &QPushButton::clicked, this, &MainWindow::viewStudentInfo);
    connect(enrollStudentButton, &QPushButton::clicked, this, &MainWindow::enrollStudent);
    connect(gradeStudentButton, &QPushButton::clicked, this, &MainWindow::gradeStudent);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addCourse() {
    AddCourseDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        QString courseName = dialog.courseName();
        Course course(courseName);
        QMap<QString, double> formula = dialog.gradingFormula();
        for (const QString &category : formula.keys()) {
            course.gradingFormula[category] = formula[category];
        }
        courses.append(course);
    }
}

void MainWindow::viewCourses() {
    QString courseListText = "Список курсов:\n";
    for (const Course &course : courses) {
        courseListText += "Название: " + course.name + "\n";
        QString gradingFormulaText = "Формула оценки:\n";
        for (const QString &category : course.gradingFormula.keys()) {
            gradingFormulaText += QString::number(course.gradingFormula[category]) + "*" +category  + "\n";
        }

        courseListText += gradingFormulaText + "\n";
    }
    QMessageBox::information(this, "Список курсов", courseListText);
}

void MainWindow::addStudent() {
    QString studentName = QInputDialog::getText(this, "Добавить студента", "Имя студента:");
    if (!studentName.isEmpty()) {
        Student student(studentName);
        students.append(student);
    }
}

void MainWindow::viewStudentInfo() {
    QString studentName = QInputDialog::getText(this, "Информация о студенте", "Имя студента:");
    if (!studentName.isEmpty()) {
        Student *student = nullptr;
        for (Student &s : students) {
            if (s.name == studentName) {
                student = &s;
                break;
            }
        }

        if (student) {
            student->calculateFinalGrades(courses);
            QString infoText = "Информация о студенте " + student->name + ":\n";
            for (const CourseResult &result : student->courseResults) {
                infoText += "Курс: " + result.courseName + "\n";
                infoText += "Итоговая оценка: " + QString::number(result.finalGrade) + "\n\n";
            }
            QMessageBox::information(this, "Информация о студенте", infoText);
        } else {
            QMessageBox::warning(this, "Ошибка", "Студент не найден.");
        }
    }
}


void MainWindow::enrollStudent() {
    QString studentName = QInputDialog::getText(this, "Записать студента на курс", "Имя студента:");
    if (!studentName.isEmpty()) {
    AddCourseAndStudentDialog dialog(courses, this);
    if (dialog.exec() == QDialog::Accepted) {

            // Найти студента в списке
            Student *student = nullptr;
            for (Student &s : students) {
                if (s.name == studentName) {
                    student = &s;
                    break;
                }
            }
            if (student) {
                QList<QString> selectedCourses = dialog.selectedCourses();
                for (const QString &courseName : selectedCourses) {
                    for (Course &course : courses) {
                        if (course.name == courseName) {
                            student->enrollCourse(course);
                            for (const QString &category : course.gradingFormula.keys()) {
                                student->setGrade(course, category, 0);
                            }
                            break;
                        }
                    }
                }
                QMessageBox::information(this, "Успех", "Студент успешно записан на выбранные курсы.");
            } else {
                QMessageBox::warning(this, "Ошибка", "Студент не найден.");
            }
        }
    }
}


void MainWindow::gradeStudent() {
    QString studentName = QInputDialog::getText(this, "Поставить оценку студенту", "Имя студента:");
    QString courseName = QInputDialog::getText(this, "Поставить оценку студенту", "Название курса:");

    if (!studentName.isEmpty() && !courseName.isEmpty()) {
        Student *student = nullptr;
        for (Student &s : students) {
            if (s.name == studentName) {
                student = &s;
                break;
            }
        }
        Course *course = nullptr;
        for (Course &c : courses) {
            if (c.name == courseName) {
                course = &c;
                break;
            }
        }

        if (student && course) {
            for (const QString &category : course->gradingFormula.keys()) {
                double gradeValue = QInputDialog::getDouble(this, "Поставить оценку студенту",
                                                            "Оценка для категории " + category + ":");
                student->setGrade(*course, category, gradeValue);
            }

            QMessageBox::information(this, "Успех", "Оценки успешно поставлены.");
        } else {
            QMessageBox::warning(this, "Ошибка", "Студент или курс не найдены.");
        }
    }
}

