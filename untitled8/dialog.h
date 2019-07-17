#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QGroupBox>
#include<QGridLayout>
#include<QPushButton>
#include<QLineEdit>
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

    QGridLayout * mainLayout;
    QLineEdit *lineEdit[3];
    //QGroupBox * grpBox;
    void createGridLayout();
private:
    bool memoryUsable;
    double memoryValue;
    bool appendable;
    bool operationEnd;
    bool calculable;
    double result;
    double currOperand;
    QString prevOperator;
    void calculate(double);

public slots:
    void slotBackSpace();
    void slotClear();
    void slotClearAll();

    void slotMC();
    void slotMR();
    void slotMS();
    void slotMPlus();
    void slotDigits();
    void slotDot();
    void slotPlusMinus();
    void slotMainOp();
    void slotOp();
    void slotEqual();
};

#endif // DIALOG_H
