#pragma once
#include "ui_demo.h"
#include <QMainWindow>

class demo : public QMainWindow
{
    Q_OBJECT

public:
    demo(QWidget *parent = nullptr);
    ~demo();

private:
    Ui_demo *ui;

    bool zeroIsLegal = false, // 判断当前数字是否为整数 0
        numIsInit = true,     // 判断当前数字是否为整数
        numTime = true;       // 判断当前为 数字输入环节 或 符号输入环节

    int numLength = 0;   // 记录当前输入数字长度，保证小于6
    char stackOperation; // 记录 输入数字前 的 符号 以用于计算
    QString inputStr,    // 记录 输入的数字
        stackStr;        // 记录 输入符号前的数字 和 计算结果，也是 判断 是否可以 进行一次计算的标志

    void textInit();                    // 初始化
    void digitBtn(QChar digit);         // 输入数字
    void operationBtn(QChar operation); // 输入符号
    void equalBtn();                    // 计算结果

    void setShortCut(); // 链接快捷键

private slots:
    void on_pushButton_clicked();
    void on_btn_0_clicked();
    void on_btn_1_clicked();
    void on_btn_2_clicked();
    void on_btn_3_clicked();
    void on_btn_4_clicked();
    void on_btn_5_clicked();
    void on_btn_6_clicked();
    void on_btn_7_clicked();
    void on_btn_8_clicked();
    void on_btn_9_clicked();
    void on_btn_point_clicked();
    void on_btn_equal_clicked();
    void on_btn_add_clicked();
    void on_btn_subtract_clicked();
    void on_btn_multiply_clicked();
    void on_btn_divide_clicked();
    void on_btn_percent_clicked();
    void on_btn_back_clicked();
    void on_btn_del_clicked();
};