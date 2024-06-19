#ifndef ADDCOURSEDIALOG_H
#define ADDCOURSEDIALOG_H
#include <QDoubleSpinBox>
#include<QLabel>
#include<QCheckBox>
#include <QDialog>
#include<QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
class AddCourseDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddCourseDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Добавить курс");

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *nameLabel = new QLabel("Название курса:", this);
        layout->addWidget(nameLabel);

        nameLineEdit = new QLineEdit(this);
        layout->addWidget(nameLineEdit);

        QLabel *categoriesLabel = new QLabel("Категории оценивания:", this);
        layout->addWidget(categoriesLabel);

        for (const QString &category : categories) {
            QCheckBox *checkBox = new QCheckBox(category, this);
            layout->addWidget(checkBox);
            categoryCheckBoxes.append(checkBox);

            QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
            spinBox->setMinimum(0.0);
            spinBox->setMaximum(1.0);
            spinBox->setSingleStep(0.1);
            categorySpinBoxes.append(spinBox);

            layout->addWidget(spinBox);
        }

        QHBoxLayout *buttonLayout = new QHBoxLayout;

        QPushButton *okButton = new QPushButton("OK", this);
        connect(okButton, &QPushButton::clicked, this, &AddCourseDialog::accept);
        buttonLayout->addWidget(okButton);

        QPushButton *cancelButton = new QPushButton("Отмена", this);
        connect(cancelButton, &QPushButton::clicked, this, &AddCourseDialog::reject);
        buttonLayout->addWidget(cancelButton);

        layout->addLayout(buttonLayout);
    }

    QString courseName() const {
        return nameLineEdit->text();
    }

    QMap<QString, double> gradingFormula() const {
        QMap<QString, double> formula;
        for (int i = 0; i < categories.size(); ++i) {
            if (categoryCheckBoxes.at(i)->isChecked()) {
                formula[categories.at(i)] = categorySpinBoxes.at(i)->value();
            }
        }
        return formula;
    }
private slots:
    void accept() override {
        double sum = 0.0;
        for (const double value : gradingFormula().values()) {
            sum += value;
        }

        if (qFuzzyCompare(sum, 1.0)) {
            QDialog::accept(); // Принимаем диалоговое окно
        } else {
            QMessageBox::warning(this, "Ошибка", "Сумма всех коэффициентов должна быть равна 1.");

            for (QDoubleSpinBox *spinBox : categorySpinBoxes) {
                spinBox->setValue(0.0);
            }
        }
    }
private:
    QLineEdit *nameLineEdit;
    QList<QCheckBox*> categoryCheckBoxes;
    QList<QDoubleSpinBox*> categorySpinBoxes;

    QStringList categories = {"Практические работы", "Семинары", "Самостоятельные работы", "Домашние задания", "Экзамен"};
};
#endif // ADDCOURSEDIALOG_H
