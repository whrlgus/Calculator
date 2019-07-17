#include "dialog.h"
#include<QDebug>
#include<QString>

#include<QKeySequence>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    createGridLayout();
    this->setLayout(mainLayout);
    slotClearAll();
}

Dialog::~Dialog()
{

}

void Dialog::createGridLayout(){
    mainLayout = new QGridLayout;

    // edit창 세팅
    for(int i=0;i<3;++i){
        lineEdit[i] = new QLineEdit;
        lineEdit[i]->setAlignment(Qt::AlignRight);
        lineEdit[i]->setReadOnly(true);
    }
    mainLayout->addWidget(lineEdit[0],0,0,1,6);
    mainLayout->addWidget(lineEdit[1],1,0,1,6);

    // 상위 세개 버튼 세팅
    QPushButton *upperBtns[3];
    const char* upperBtnsEle[3][3]={
        {"BackSpace","Clear","Clear All"}
        ,{SLOT(slotBackSpace()),SLOT(slotClear()),SLOT(slotClearAll())}};
    const QKeySequence qKeySeq[3]={
        QKeySequence(Qt::Key_Backspace),NULL,QKeySequence(Qt::Key_Escape)};
    for(int i=0;i<3;++i){
        upperBtns[i] = new QPushButton(upperBtnsEle[0][i],this);
        upperBtns[i]->setShortcut(qKeySeq[i]);
        connect(upperBtns[i],SIGNAL(clicked()),this,upperBtnsEle[1][i]);
        mainLayout->addWidget(upperBtns[i],2,i*2,1,2);
    }

    // 나머지 버튼 세팅
    QPushButton *mainBtns[24];
    const char* mainBtnsEle[2][24]={
        {"MC","7","8","9","÷","Sqrt"
         ,"MR","4","5","6","x","x^2"
         ,"MS","1","2","3","-","1/x"
         ,"M+","0",".","±","+","="}

        ,{SLOT(slotMC()),SLOT(slotDigits()),SLOT(slotDigits()),SLOT(slotDigits()),SLOT(slotMainOp()),SLOT(slotOp())
          ,SLOT(slotMR()),SLOT(slotDigits()),SLOT(slotDigits()),SLOT(slotDigits()),SLOT(slotMainOp()),SLOT(slotOp())
          ,SLOT(slotMS()),SLOT(slotDigits()),SLOT(slotDigits()),SLOT(slotDigits()),SLOT(slotMainOp()),SLOT(slotOp())
          ,SLOT(slotMPlus()),SLOT(slotDigits()),SLOT(slotDot()),SLOT(slotPlusMinus()),SLOT(slotMainOp()),SLOT(slotEqual())}
    };
    const QKeySequence qKeySeqMain[24]={
        NULL,QKeySequence(Qt::Key_7),QKeySequence(Qt::Key_8),QKeySequence(Qt::Key_9),QKeySequence(Qt::Key_Slash),NULL
        ,NULL,QKeySequence(Qt::Key_4),QKeySequence(Qt::Key_5),QKeySequence(Qt::Key_6),QKeySequence(Qt::Key_Asterisk),NULL
        ,NULL,QKeySequence(Qt::Key_1),QKeySequence(Qt::Key_2),QKeySequence(Qt::Key_3),QKeySequence(Qt::Key_Minus),NULL
        ,NULL,QKeySequence(Qt::Key_0),QKeySequence(Qt::Key_Comma),NULL,QKeySequence(Qt::Key_Plus),QKeySequence(Qt::Key_Enter)
    };
    for(int i=0;i<24;++i){
        mainBtns[i] = new QPushButton(mainBtnsEle[0][i],this);
        mainBtns[i]->setShortcut(qKeySeqMain[i]);
        connect(mainBtns[i],SIGNAL(clicked()),this,mainBtnsEle[1][i]);
        mainLayout->addWidget(mainBtns[i]);
    }


    mainLayout->addWidget(lineEdit[2],7,0,1,6);
}

void Dialog::slotBackSpace(){
    QString tmp = lineEdit[0]->text();
    double num = tmp.toDouble();
    if(tmp=="0")return;
    else if(tmp.length()==1){
        lineEdit[0]->setText("0");
        appendable=false;
    }
    else{
        QString str = lineEdit[0]->text();
        str.chop(1);
        lineEdit[0]->setText(str);
    }

}
void Dialog::slotClear(){}
void Dialog::slotClearAll(){
    lineEdit[0]->setText("0");
    lineEdit[1]->clear();
    lineEdit[2]->clear();
    appendable = false;
    operationEnd = true;
    result=0;
    currOperand=0;
    prevOperator="";
    memoryUsable=false;
}

void Dialog::slotMC(){
    memoryUsable=false;
}
void Dialog::slotMR(){
    if(!memoryUsable) return;
    lineEdit[0]->setText(QString::number(memoryValue));
    appendable=true;
}
void Dialog::slotMS(){
    QString tmp = lineEdit[0]->text();
    memoryValue=tmp.toDouble();
    memoryUsable=true;

}
void Dialog::slotMPlus(){}
void Dialog::slotDigits(){
    QString tmp = lineEdit[0]->text();
    QString inputNum = static_cast<QPushButton*>(sender())->text();
    if(tmp=="0"&&inputNum=="0") return;
    if(!appendable){
        lineEdit[0]->clear();
        appendable=true;
    }
    lineEdit[0]->setText(lineEdit[0]->text()+inputNum);
    calculable=true;
}
void Dialog::slotDot(){
    QString tmp = lineEdit[0]->text();
    for(int i=0;i<tmp.length();++i) if(tmp.at(i)=='.') return;
    lineEdit[0]->setText(tmp+'.');
}
void Dialog::slotPlusMinus(){
    QString tmp = lineEdit[0]->text();
    if(tmp=="0") return;
    if(tmp.at(0)=='-') tmp.remove(0,1);
    else tmp="-"+tmp;
    lineEdit[0]->setText(tmp);
}
void Dialog::slotMainOp(){
    QString currOperator = static_cast<QPushButton*>(sender())->text();
    currOperand = lineEdit[0]->text().toDouble();

    if(!calculable){
        QString tmp = lineEdit[1]->text();
        tmp.chop(1);
        lineEdit[1]->setText(tmp+currOperator);
        prevOperator = currOperator;
        return;
    }
    calculable = false;
    lineEdit[1]->setText(lineEdit[1]->text() +" "+ QString::number(currOperand)+" "+currOperator);

    if(operationEnd){
        result=currOperand;
        operationEnd=false;
    }
    else calculate(currOperand);


    prevOperator = currOperator;
    appendable=false;
    currOperand=0;

}
void Dialog::calculate(double value){
    if(prevOperator=="+")
        result += value;
    else if(prevOperator=="-")
        result -= value;
    else if(prevOperator=="x")
        result *= value;
    else
        result /= value;
    lineEdit[0]->setText(QString::number(result));
}

void Dialog::slotOp(){
    QString currOperator = static_cast<QPushButton*>(sender())->text();
    QString tmp = lineEdit[0]->text();
    if(currOperator=="Sqrt")
        result = sqrt(tmp.toDouble());
    else if(currOperator=="x^2")
        result*=result;
    else result = 1/result;
    lineEdit[0]->setText(QString::number(result));

    prevOperator = currOperator;
    appendable=false;
    currOperand=0;
}
void Dialog::slotEqual(){
    lineEdit[1]->clear();

    if(currOperand==0) currOperand = lineEdit[0]->text().toDouble();
    QString tmp = QString::number(result)+" "+prevOperator+" "+QString::number(currOperand)+" = ";
    calculate(currOperand);
    lineEdit[2]->setText(tmp+QString::number(result));
    appendable=false;
    operationEnd=true;
}
