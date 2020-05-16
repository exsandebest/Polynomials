#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>


struct pair{
    double val;
    int i;
};


struct Polynom{
    double k;
    int n;
    Polynom * next;
};
struct polTrip{
    Polynom * start;
    Polynom * finish;
    int len;
};

struct polPair{
    Polynom * a;
    Polynom * b;
};


struct MainList{
    Polynom * polynom;
    MainList * next;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateDb();
    QString getPolynomString(Polynom * pol);
    QString getDegree(int n);
    pair getK(QString & s, int i);
    pair getN(QString & s, int i);
    bool parseInput(Polynom * pol,QString & s, int i);
    void addPolynomToDb(Polynom * pol);
    void normalize(Polynom * pol);
    polTrip sort(Polynom * pol, int len);
    void cutZero(Polynom * pol);
    Polynom * Plus (Polynom * pol1, Polynom * pol2);
    Polynom * Minus (Polynom * pol1, Polynom * pol2);
    Polynom * Multiply (Polynom * pol1, Polynom * pol2);
    Polynom * Derivative (Polynom * pol);
    bool Equal(Polynom * pol1, Polynom * pol2);
    void updatePlaceholders();
    polPair Division(Polynom * pol1, Polynom * pol2);
    double Point(Polynom * pol, double x);
    double pow(double x, int n);
    void setSavabilityPolynom(bool t);
    Polynom * complex(Polynom * pol);
    bool checkNull(Polynom * pol);

private slots:
    void on_btnAddPolynom_clicked();

    void on_btnGetResult_clicked();

    void on_bntClearInputField1_clicked();

    void on_bntClearInputField2_clicked();

    void on_bntClearOutputResult_clicked();

    void on_btnSave_clicked();

    void on_bntClearInputPolymon_clicked();

    void onDblClicked(QListWidgetItem * item);
    void on_rbtnPlus_clicked();

    void on_rbtnMinus_clicked();

    void on_rbtnMultiply_clicked();

    void on_rbtnDivision_clicked();

    void on_rbtnEqual_clicked();

    void on_rbtnPoint_clicked();

    void on_rbtnDerivative_clicked();

    void on_bntClearInputField3_clicked();

    void on_btnFindRoots_clicked();

    void on_bntClearOutputField2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
