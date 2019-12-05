#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QMap>
#include <QDebug>
#include <QMessageBox>
#include <QObject>

QString degrees = "⁰¹²³⁴⁵⁶⁷⁸⁹";

MainList * db = nullptr;

int dbLen = 0;

Polynom * tmpResultPolynom = nullptr;
Polynom * tmpResultPolynom2 = nullptr;

bool isSavablePolynom = false;


QString MainWindow::getPolynomString(Polynom *pol){
    QString ans ="";
    Polynom * tmp = pol;
    while (tmp!=nullptr){
        if(tmp == pol){
        if (tmp->k == 0){
            tmp = tmp->next;
            continue;
        }
        QString s1 = "";
        if(tmp->k==-1 && tmp->n != 0){
            s1="-";
        } else if(tmp->k ==-1 && tmp->n == 0){
            s1="-1";
        } else if (tmp->k == 1 && tmp->n == 0){
            s1 = "1";
        } else if(tmp->k == 1 && tmp->n != 0){
            s1 = "";
        } else {
            s1.setNum(tmp->k);
        }
        ans+=s1;
        if (tmp->n != 0){
            ans+="x";
        }
        ans+=getDegree(tmp->n);
        tmp = tmp->next;
        } else {
            if (tmp->k == 0){
                tmp = tmp->next;
                continue;
            }
            QString s1 = "";
            if (tmp == pol){
                if(tmp->k == 1){
                    if (tmp->n != 0){
                        s1 = "";
                    } else {
                        s1 = "1";
                    }
                } else if (tmp->k == -1){
                    if (tmp->n != 0){
                        s1 = "-";
                    } else {
                        s1 = "-1";
                    }
                } else {
                    s1.setNum(tmp->k);
                }
            } else {
                if(tmp->k == 1){
                    s1 = "+";
                    if (tmp->n ==0){
                        s1+="1";
                    }
                } else if (tmp->k==-1){

                    s1 = "-";
                    if (tmp->n ==0){
                        s1+="1";
                    }
                } else {
                    s1.setNum(tmp->k);
                    if (tmp->k > 0){
                        s1 = "+"+s1;
                    }
                }
            }
            ans+=s1;
            if (tmp->n != 0){
                ans+="x";
            }
            ans+=getDegree(tmp->n);
            tmp = tmp->next;
        }
    }
    return  ans;
}

double MainWindow::pow(double x, int n){
    if (n == 0) return 1;
    double ans = 1;
    for (int i = 0; i < n; ++i){
        ans*=x;
    }
    return ans;
}



void MainWindow::updateDb(){
    MainList * tmp = db;
    ui->dbListWidget->clear();
    int t = 1;
    while (tmp!=nullptr){
        QString sPolynom = getPolynomString(tmp->polynom);
        QString s1;
        s1.setNum(t++);
        QListWidgetItem * item = new QListWidgetItem();
        item->setText(s1 + ". "+sPolynom);
        item->setToolTip(s1);
        ui->dbListWidget->addItem(item);
        tmp=tmp->next;
    }
}


void MainWindow::normalize(Polynom *pol){
    Polynom * i = pol, *j;
    while (i!=nullptr){
        j=i->next;
        while(j!=nullptr){
            if(i->n == j->n){
                i->k+=j->k;
                j->k = 0;
                j->n = 0;
            }
            j=j->next;
        }
        i=i->next;
    }
    Polynom * tmp = pol;
    while(tmp!=nullptr){
        if(tmp->k==0){
            tmp->n=0;
        }
        tmp = tmp->next;
    }
}


QString MainWindow::getDegree(int n){
    QString ans = "";
    QString tmp;
    tmp.setNum(n);
    if (tmp.length() ==1){
        if (n ==0 || n == 1){
            return "";
        } else {
            return QString(degrees[n]);
        }
    }
    for (int i = 0; i < tmp.length(); ++i){
        int g = QString(tmp[i]).toInt();
            ans+=degrees[g];
    }

    return ans;
}

pair MainWindow::getK(QString &s, int i){
    QString tmp = "";
    int tmpK = 1;
    if (s[i] == '-'){
        tmpK = -1;
        ++i;
    }
    while (s[i]>='0' && s[i] <='9'){
        tmp+=s[i];
        ++i;
    }
    if(tmp == ""){
        pair pr;
        pr.val = tmpK;
        pr.i = i;
        return pr;
    }
    if(tmp[0] == '0' && tmp.length()>1){
        QMessageBox::about(this, "Think about it", "Мы, конечно, не запрещаем вам использовать ведущие нули, но давайте будем обходиться без них :)");
    }
    pair pr;
    pr.val = tmp.toDouble()*tmpK;
    pr.i = i;
    return pr;
}

void MainWindow::onDblClicked(QListWidgetItem * item){
    int n = item->toolTip().toInt();
    if (n==1){
        MainList * cache = db;
        db = db->next;
        delete cache;
        --dbLen;
        if(!dbLen){
            db = nullptr;
        }
        updateDb();
        return;
    }
    MainList * tmpdb = db;
    for (int i = 0; i < n-2; ++i){
        tmpdb = tmpdb->next;
    }
    MainList * cache = tmpdb->next;
    tmpdb->next = tmpdb->next->next;
    delete cache;
    --dbLen;
    if(!dbLen){
        db = nullptr;
    }
    updateDb();
}


pair MainWindow::getN(QString &s, int i){
    QString tmp = "";
    if (QString(s[i])+QString(s[i+1]) == "+-") throw QString("Степень может быть только неотрицательным числом");
    if (i>s.length()-1 || s[i]=='+') throw QString("Введите степень переменной");
    if (s[i] == 'x') throw QString("Степень должна быть неотрицательным числом");
    if (s[i] == '^') throw QString("Введите степень переменной\nТы хотел сделать смайлик ^^ ?)");
    while (s[i]>='0' && s[i] <='9'){
        tmp+=s[i];
        ++i;
    }
    pair pr;
    pr.val = tmp.toInt();
    pr.i = i;
    return pr;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    db = new MainList;
    db->next = nullptr;
    Polynom * pl = new Polynom();
    pl->k = 25;
    pl->n = 3;
    pl->next = nullptr;
    db->polynom = pl;
    ++dbLen;
    ui->setupUi(this);
    this->showMaximized();
    ui->rbtnPlus->setChecked(true);
    ui->inputPolynom->setText("5x^2+4x-10");
    on_btnAddPolynom_clicked();
    ui->inputPolynom->setText("4x^3-2x^2+4x+11");
    on_btnAddPolynom_clicked();
    ui->inputPolynom->setText("x^2-4x+5");
    on_btnAddPolynom_clicked();
    ui->inputPolynom->setText("x-1");
    on_btnAddPolynom_clicked();
    ui->inputPolynom->setText("x^4-3x^3+5x^2+1");
    on_btnAddPolynom_clicked();
    ui->inputPolynom->setText("x^6-x^2+3x");
    on_btnAddPolynom_clicked();
    updateDb();
    QObject::connect(ui->dbListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(onDblClicked(QListWidgetItem *)));
    updatePlaceholders();
    setSavabilityPolynom(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

polTrip MainWindow::sort(Polynom *pol, int len){
    Polynom * big, *small, *middle;
    Polynom * bigStart, * smallStart, *middleStart;
    Polynom * bigEnd, * smallEnd, * middleEnd;
    Polynom * ansStart, * ansEnd, * g = pol;
    int x = pol->n;
    int bd = 0, sd =0, md = 0;
    for(int i = 0; i < len; ++i){
        if (g->n>x){
                  if (bd != 0){
                      big->next = g;
                      big = big->next;
                      ++bd;
                    } else {
                      bigStart = big = g;
                               ++bd;
                    }
        } else if(g->n == x){
            if (md!=0){
                        middle->next = g;
                        middle = middle->next;
                        ++md;
                        } else {
                            middleStart = middle = g;
                            ++md;
                        }
        } else {
            if (sd!=0){
                        small->next = g;
                        small = small->next;
                        ++sd;
                        } else {
                            smallStart = small = g;
                            ++sd;
                        }
        }
        g = g->next;
    }
    smallEnd = small;
    bigEnd = big;
    middleEnd = middle;

    if(sd){
        if (sd>1){
            polTrip tmpTrip = sort(smallStart, sd);
            smallStart = tmpTrip.start;
            smallEnd = tmpTrip.finish;
        }
        middleEnd->next = smallStart;
        ansEnd = smallEnd;
    } else {
        ansEnd = middleEnd;
    }
    if(bd){
        if (bd>1){
            polTrip tmpTrip = sort(bigStart, bd);
            bigStart = tmpTrip.start;
            bigEnd = tmpTrip.finish;
        }
        bigEnd->next = middleStart;
        ansStart = bigStart;
    } else {
        ansStart = middleStart;
    }
    polTrip ans;
    ans.start = ansStart;
    ans.finish = ansEnd;
    ans.len = len;
    return ans;
}

void MainWindow::addPolynomToDb(Polynom *pol){
    if (db == nullptr){
        db = new MainList;
        db->next = nullptr;
        db->polynom = pol;
        ++dbLen;
        return;
    }
    MainList * t = db;
    while (t->next!=nullptr){
        t=t->next;
    }
    t->next = new MainList;
    t = t->next;
    t->polynom = pol;
    t->next = nullptr;
    ++dbLen;
}


bool MainWindow::Equal(Polynom *pol1, Polynom *pol2){
    while (1){
        if (pol1 == nullptr && pol2 == nullptr){
            return true;
        }
        if (pol1 == nullptr && pol2 != nullptr){
            return false;
        }
        if (pol1 != nullptr && pol2 == nullptr){
            return false;
        }
        if (pol1->k != pol2->k){
            return false;
        }
        if (pol1->n != pol2->n){
            return false;
        }
        pol1 = pol1->next;
        pol2 = pol2->next;
    }
}


void MainWindow::cutZero(Polynom *pol){
    Polynom * tmp = pol;
    while (tmp!= nullptr){
        if(tmp->k == 0 && tmp->next != nullptr){
            tmp = tmp->next;
        }
        if (tmp->next != nullptr && tmp->next->k == 0){
            Polynom * p = tmp->next;
            tmp->next = tmp->next->next;
            delete p;
        }
        tmp = tmp->next;
    }
}

Polynom * MainWindow::Multiply(Polynom *pol1, Polynom *pol2){
    Polynom * p1 = pol1, * p2 = pol2;
    Polynom *ans = new Polynom, * ansStart = ans;
    while (p1!=nullptr){
        p2 = pol2;
        while(p2!=nullptr){
            Polynom * tmp = new Polynom;
            tmp->k = p1->k * p2->k;
            tmp->n = p1->n + p2->n;
            tmp->next = nullptr;
            ans->next = tmp;
            ans = ans->next;
            p2 = p2->next;
        }
        p1 = p1->next;
    }
    ansStart = ansStart->next;
    return ansStart;
}

Polynom * MainWindow::Derivative(Polynom *pol){
    Polynom *ans = new Polynom, * ansStart = ans;
    Polynom * tmp = pol;
    while (tmp!=nullptr){
        Polynom * g = new Polynom;
        g->k = tmp->k * tmp->n;
        g->n = tmp->n-1;
        g->next = nullptr;
        ans->next = g;
        ans = ans->next;
        tmp = tmp->next;
    }
    ansStart = ansStart->next;
    return ansStart;
}


bool MainWindow::parseInput(Polynom * pol, QString &s, int i){
    pair tmpPair = getK(s, i);
    int tmpK = tmpPair.val;
    pol->k = tmpK;
    i = tmpPair.i;
    if (i > s.length()) throw QString("Ожидался одночлен");
    if (s[i] == 'x'){
        ++i;
        if(s[i] == '^'){
            tmpPair = getN(s, ++i);
            int tmpN = tmpPair.val;
            pol->n = tmpN;
            i = tmpPair.i;
        } else {
            pol->n = 1;
        }
    } else {
        if (s[i] == '^') throw QString("В степень может быть возведена только переменная");
        pol->n = 0;
    }
    if (s[i] == '+'){
        pol->next = new Polynom;
        parseInput(pol->next,s,i+1);
    } else if(s[i] == "x"){
       throw QString("Некорректный ввод: xxx...");    
    } else if (s[i] == '^'){
        throw QString("Нельзя возводить показатель степени в степень");
    } else if (s[i]>='0' && s[i] <='9'){
        throw QString("Что за цифры после x?\nВы забыли значок степени?");
    }else{
        pol->next = nullptr;
    }
    return true;
}



Polynom * MainWindow::Plus(Polynom *pol1, Polynom *pol2){
    Polynom * p1=pol1, *p2=pol2;
    Polynom *ans = new Polynom, * ansStart = ans, * tmp = pol1;
    while (tmp!=nullptr){
        Polynom * tmptmp = new Polynom;
        tmptmp->k = tmp->k;
        tmptmp->n = tmp->n;
        tmptmp->next = nullptr;
        ans->next = tmptmp;
        ans=ans->next;
        tmp = tmp->next;
    }
    tmp = pol2;
    while (tmp != nullptr){
        Polynom * tmptmp = new Polynom;
        tmptmp->k = tmp->k;
        tmptmp->n = tmp->n;
        tmptmp->next = nullptr;
        ans->next = tmptmp;
        ans = ans->next;
        tmp = tmp->next;
    }
    ansStart = ansStart->next;
    return ansStart;


}


Polynom * MainWindow::Minus(Polynom *pol1, Polynom *pol2){
    Polynom *ans = new Polynom, * ansStart = ans, * tmp = pol1;
    while (tmp!=nullptr){
        Polynom * tmptmp = new Polynom;
        tmptmp->k = tmp->k;
        tmptmp->n = tmp->n;
        tmptmp->next = nullptr;
        ans->next = tmptmp;
        ans=ans->next;
        tmp = tmp->next;
    }
    tmp = pol2;
    while (tmp != nullptr){
        Polynom * tmptmp = new Polynom;
        tmptmp->k = -tmp->k;
        tmptmp->n = tmp->n;
        tmptmp->next = nullptr;
        ans->next = tmptmp;
        ans = ans->next;
        tmp = tmp->next;
    }
    ansStart = ansStart->next;
    return ansStart;
}




void MainWindow::setSavabilityPolynom(bool t){
    isSavablePolynom = t;
    ui->btnSave->setDisabled(!t);
}


void MainWindow::on_btnAddPolynom_clicked(){
     try {
    QString str = ui->inputPolynom->text();
    for (int i = 0; i < str.length(); ++i){
        if(str[i] == " ") throw QString("Пробелы запрещены");
    }
    if (str == "") throw QString("Введите многочлен");
    for (int i = 0; i < str.length()-1; ++i){
        if(QString(str[i]) + QString(str[i+1]) == "+-" ||QString(str[i]) + QString(str[i+1]) == "-+"){
            throw QString("Ожидался одночлен");
        }
    }

    if (str[0] == "+" || str == "-" || QRegExp(".*[-]{2,}.*").exactMatch(str) || QRegExp(".*[+]{2,}.*").exactMatch(str) || str[str.length()-1] == "-" || str[str.length()-1]=="+") throw QString("Ожидался одночлен");
    str = str.replace(QRegExp("-"),"+-");
    if (str[0] == '+') str[0] = ' ';
    str = str.replace(QRegExp(" "),"");

    Polynom * pol = new Polynom;
    QString tmps;
    tmps.setNum(str.toInt());
    if (str == tmps){
        if(str[0] == '0' && str.length()>1 && str.toInt() != 0 ){
            QMessageBox::about(this, "Think about it", "Мы, конечно, не запрещаем вам использовать ведущие нули, но давайте будем обходиться без них :)");
        }
        pol->k = str.toInt();
        if (pol->k == 0){
            throw QString("Ноль в вашей жизни ничего не изменит, так что давайте не будем его добавлять");
        }
        pol->n = 0;
        pol->next = nullptr;
        addPolynomToDb(pol);
        updateDb();
        ui->inputPolynom->clear();
        return;
    }
        if(!QRegExp("[0123456789x+-\\s^]+").exactMatch(str)) throw QString("Обнаружены недопустимые символы");
        parseInput(pol,str,0);
        pol = complex(pol);
        if(pol->k == 0){
            throw QString("Ваш многочлен сократился");
        }
        addPolynomToDb(pol);
        updateDb();
        ui->inputPolynom->setText("");
    } catch (QString str){
        QMessageBox::warning(this, "Warning", str);
    }
}

bool MainWindow::checkNull(Polynom *pol){
    while(pol != nullptr){
        if (pol->n!=0 || pol->k!=0){
            return false;
        }
        pol = pol->next;
    }
    return true;
}


Polynom * MainWindow::complex(Polynom *pol){
    normalize(pol);
    int tmpLen = 0;
    Polynom * tmptmp = pol;
    while (tmptmp != nullptr){
        tmpLen++;
        tmptmp = tmptmp->next;
    }
    polTrip tmpPair = sort(pol, tmpLen);
    pol = tmpPair.start;
    tmpPair.finish->next = nullptr;
    cutZero(pol);
    return pol;
}

polPair MainWindow::Division(Polynom *pol1, Polynom *pol2){
    Polynom * ans = new Polynom, * ansStart = ans, * reminder = nullptr;
    int ln = 0;
    while(1){
        double tmpK = pol1->k/pol2->k;
        int tmpN = pol1->n-pol2->n;
        if (tmpN <0){
            polPair tmpPair;
            if (ln == 0){
            tmpPair.a = nullptr;
            } else {
                tmpPair.a = ansStart->next;
            }
            tmpPair.b = pol1;
            return tmpPair;
        }
        ans->next = new Polynom;
        ans = ans->next;
        ans->n = tmpN;
        ans->k = tmpK;
        ans->next = nullptr;
        Polynom * tmpPol = new Polynom, *tmpPolStart = tmpPol;
        Polynom * p2 = pol2;
        while (p2 != nullptr){
            tmpPol->next = new Polynom;
            tmpPol = tmpPol->next;
            tmpPol->k = tmpK*p2->k;
            tmpPol->n = tmpN + p2->n;
            tmpPol->next = nullptr;
            p2 = p2->next;
        }
        tmpPol = tmpPolStart->next;
        Polynom * nextPol = Minus(pol1,tmpPol);

        nextPol = complex(nextPol);
        pol1 = nextPol;
        if (nextPol->n == 0){
            reminder = nextPol;
            break;
        }
        ln++;
    }
    polPair tmpPair;
    ansStart = ansStart->next;
    tmpPair.a = ansStart;
    reminder = complex(reminder);
    if (checkNull(reminder)){
        reminder = nullptr;
    }
    tmpPair.b = reminder;
    return tmpPair;
}

double MainWindow::Point(Polynom * pol, double x){
    double res = 0;
    while (pol!=nullptr){
        res += pow(x,pol->n) * pol->k;
        pol = pol->next;
    }
    return res;
}

void MainWindow::on_btnGetResult_clicked()
{
    try{
    QString str1 = ui->inputField1->text();
    QString str2 = ui->inputField2->text();
    if (str1 == "" || str2 == ""){
        throw QString("Заполнены не все поля");
    }
    if (!QRegExp("[0123456789]+").exactMatch(str1)){
        throw QString("Недопустимые символы в поле 1");
    }
    if (!QRegExp("[0123456789]+").exactMatch(str2) && !ui->rbtnPoint->isChecked()){
        throw QString("Недопустимые символы в поле 2");
    }
    if (ui->rbtnPoint->isChecked() && !QRegExp("[0123456789.]+").exactMatch(str2)){
        throw QString("Недопустимые символы в поле 2");
    }
    if (ui->rbtnDerivative->isChecked()){
        int t1 = str1.toInt();
        int t2 = str2.toInt();
        if (t1 > dbLen || t1<=0){
            QString s1;
            s1.setNum(t1);
            throw QString("Полинома под номером "+s1+" не существует");
        }
        if (t2 < 0){
            throw QString("При взятии производной n должно быть ≥ 0");
        }
        MainList * tmp = db;
        for (int i = 0; i < t1-1; ++i){
            tmp = tmp->next;
        }
        Polynom * p = tmp->polynom;
        for (int i = 0; i < t2; ++i){
            p = Derivative(p);
        }
        p = complex(p);
        tmpResultPolynom = p;
        if (p->k != 0){
            ui->outputResult->setText(getPolynomString(p));
            setSavabilityPolynom(true);
        } else {
            ui->outputResult->setText("0");
            setSavabilityPolynom(false);
        }
        return;
    } else if (ui->rbtnPoint->isChecked()){
        int t1 = str1.toInt();
        double t2 = str2.toDouble();
        if (t1 > dbLen || t1 <= 0){
            QString s1;
            s1.setNum(t1);
            throw QString("Полинома под номером "+s1+" не существует");
        }
        MainList * tmp = db;
        for (int i = 0; i < t1-1; ++i){
            tmp = tmp->next;
        }
        Polynom * pol = tmp->polynom;
        double res = Point(pol,t2);
        QString s1;
        s1.setNum(res);
        ui->outputResult->setText(s1);
        setSavabilityPolynom(false);
        return;
    }

    int t1 = str1.toInt();
    int t2 = str2.toInt();
    if (t1 > dbLen || t1<=0){
        QString s1;
        s1.setNum(t1);
        throw QString("Полинома под номером "+s1+" не существует");
    } else if (t2 > dbLen || t2 <= 0){
        QString s1;
        s1.setNum(t2);
        throw QString("Полинома под номером "+s1+" не существует");
    }
    Polynom * pol1, * pol2;
    MainList * tmp = db;
    for (int i = 0; i < t1-1; ++i){
        tmp = tmp->next;
    }
    pol1 = tmp->polynom;
    tmp = db;
    for (int i = 0; i < t2-1; ++i){
        tmp = tmp->next;
    }
    pol2 = tmp->polynom;
    Polynom * ans = nullptr;
    if(ui->rbtnPlus->isChecked()){
        ans = Plus(pol1, pol2);
    } else if (ui->rbtnMinus->isChecked()){
        ans = Minus(pol1,pol2);
    } else if (ui->rbtnMultiply->isChecked()){
        ans = Multiply(pol1, pol2);
    } else if(ui->rbtnDivision->isChecked()){
        polPair tmpPair = Division(pol1,pol2);
        tmpResultPolynom = tmpPair.a;
        tmpResultPolynom2 = tmpPair.b;
        QString text =getPolynomString(tmpPair.a);
        if (tmpPair.b != nullptr){
            text+="(ост."+getPolynomString(tmpPair.b)+")";
        }
        ui->outputResult->setText(text);
        setSavabilityPolynom(true);
        return;
    } else if (ui->rbtnEqual->isChecked()){
        bool ansBool = Equal(pol1, pol2);
        if (ansBool){
            ui->outputResult->setText("TRUE");
        } else {
            ui->outputResult->setText("FALSE");
        }
        setSavabilityPolynom(false);
        return;
    }
    ans = complex(ans);
    tmpResultPolynom = ans;

    if(ans->k != 0){
        ui->outputResult->setText(getPolynomString(ans));
        setSavabilityPolynom(true);
    } else {
        ui->outputResult->setText("0");
        setSavabilityPolynom(false);
    }
    } catch(QString str){
         QMessageBox::warning(this, "Warning", str);
    }
}

void MainWindow::updatePlaceholders(){
    if (!ui->rbtnDivision->isChecked()){
        tmpResultPolynom2 = nullptr;
    }
    if(ui->rbtnDerivative->isChecked()){
        ui->inputField2->setPlaceholderText("N");
    } else if (ui->rbtnPoint->isChecked()){
        ui->inputField2->setPlaceholderText("X");
    } else {
        ui->inputField1->setPlaceholderText("№");
        ui->inputField2->setPlaceholderText("№");
    }
}


void MainWindow::on_bntClearInputField1_clicked()
{
    ui->inputField1->setText("");
}

void MainWindow::on_bntClearInputField2_clicked()
{
    ui->inputField2->setText("");
}

void MainWindow::on_bntClearOutputResult_clicked()
{
    ui->outputResult->setText("");
    setSavabilityPolynom(false);
}

void MainWindow::on_btnSave_clicked()
{
    addPolynomToDb(tmpResultPolynom);
    if (tmpResultPolynom2 != nullptr){
        addPolynomToDb(tmpResultPolynom2);
        tmpResultPolynom2 = nullptr;
    }
    updateDb();
}

void MainWindow::on_bntClearInputPolymon_clicked()
{
    ui->inputPolynom->clear();
}



void MainWindow::on_rbtnPlus_clicked()
{
    updatePlaceholders();
}

void MainWindow::on_rbtnMinus_clicked()
{
    updatePlaceholders();
}

void MainWindow::on_rbtnMultiply_clicked()
{
    updatePlaceholders();
}

void MainWindow::on_rbtnDivision_clicked()
{
    updatePlaceholders();
}

void MainWindow::on_rbtnEqual_clicked()
{
    updatePlaceholders();
}

void MainWindow::on_rbtnPoint_clicked()
{
    updatePlaceholders();
}

void MainWindow::on_rbtnDerivative_clicked()
{
    updatePlaceholders();
}

void MainWindow::on_bntClearInputField3_clicked()
{
    ui->inputField3->setText("");
}

void MainWindow::on_btnFindRoots_clicked()
{
    try {
    QString str = ui->inputField3->text().replace(" ","");
    if (str == ""){
        throw QString("Введите номер полинома");
    }
    if (!QRegExp("[0123456789]+").exactMatch(str)){
        throw QString("Введены недопустимые символы");
    }
    int t1 = str.toInt();
    if (t1 > dbLen || t1<=0){
        QString s1;
        s1.setNum(t1);
        throw QString("Полинома под номером "+s1+" не существует");
    }
    MainList * tmpMainList = db;
    for (int i = 0; i < t1-1; ++i){
        tmpMainList = tmpMainList->next;
    }
    Polynom * pol = tmpMainList->polynom;
    QString ans = "";
    for (int i = -1000000; i <1000000; ++i){
        if (Point(pol,i) == 0){
            QString s1;
            s1.setNum(i);
            ans+=s1+"\n";
        }
    }
    if (ans == ""){
        ui->outputResult2->setText("Нет целочисленных корней");
    } else {
       ui->outputResult2->setText(ans);
    }


    } catch(QString str){
         QMessageBox::warning(this, "Warning", str);
    }

}

void MainWindow::on_bntClearOutputField2_clicked()
{
    ui->outputResult2->setText("");
}
