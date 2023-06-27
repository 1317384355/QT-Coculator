#include "demo.h"

demo::demo(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_demo)
{
    ui->setupUi(this);

    setShortCut(); // 链接快捷键

    textInit(); // 初始化
    // ui->inputEdit->setText("0"); // 设置初始文本
    // ui->inputEdit->setAlignment(Qt::AlignRight);    // 设置居右显示
    // ui->inputEdit->setStyleSheet("font-size:25px"); // 设置字体大小

    ui->statusBar->showMessage("储存数据类型为Double,请保证计算中总数总少于6个");
}

demo::~demo()
{
    delete ui;
}

void demo::setShortCut()
{
    ui->btn_0->setShortcut(Qt::Key_0);
    ui->btn_1->setShortcut(Qt::Key_1);
    ui->btn_2->setShortcut(Qt::Key_2);
    ui->btn_3->setShortcut(Qt::Key_3);
    ui->btn_4->setShortcut(Qt::Key_4);
    ui->btn_5->setShortcut(Qt::Key_5);
    ui->btn_6->setShortcut(Qt::Key_6);
    ui->btn_7->setShortcut(Qt::Key_7);
    ui->btn_8->setShortcut(Qt::Key_8);
    ui->btn_9->setShortcut(Qt::Key_9);
    ui->btn_point->setShortcut(Qt::Key_Period);

    ui->btn_equal->setShortcut(Qt::Key_Enter);

    ui->btn_add->setShortcut(Qt::Key_Plus);
    ui->btn_subtract->setShortcut(Qt::Key_Minus);
    ui->btn_multiply->setShortcut(Qt::Key_Asterisk);
    ui->btn_divide->setShortcut(Qt::Key_Slash);

    ui->btn_back->setShortcut(Qt::Key_Backspace);
    ui->btn_del->setShortcut(Qt::Key_Delete);
}

void demo::textInit() // 初始化
{
    // 不将 inputStr = "0" 是为了使 其他整数可以直接 覆盖 第一个0
    if (!stackStr.isEmpty())
    {
        stackStr.clear();
    }
    if (!inputStr.isEmpty())
    {
        inputStr.clear();
    }
    ui->showEdit->clear();
    ui->inputEdit->setText("0");

    numLength = 0;

    zeroIsLegal = false;
    numIsInit = true;
    numTime = true;
}

void demo::digitBtn(QChar digit) // 输入数字
{
    zeroIsLegal = true; // 放到最上 可以让 下边的处理 覆盖

    // 处理输入符号后再输入的数字
    // 将第一组数字与符号暂存
    if (numTime == false)
    {
        zeroIsLegal = false; // 保障不会出现 无限 0
        numIsInit = true;
        numTime = true;

        if (stackStr.isEmpty()) // 持续计算中不对 stackStr 进行赋值
        {
            stackStr = inputStr;
        }
        inputStr.clear(); // 赋值后清空当前记录的 inputStr

        // 第一个 '0' 不占限制长度
        if (digit == '0')
        {
            numLength = -1;
        }
        else
        {
            numLength = 0;
        }

        // 暂存计算符号
        stackOperation = ui->showEdit->text().at(ui->showEdit->text().size() - 1).toLatin1();
    }

    // 长度限制,放这里为保障inputEdit输满后输入符号后可以再输入数字
    if (numLength > 5)
        return;
    numLength++;

    inputStr.push_back(digit);
    ui->inputEdit->setText(inputStr);
}

void demo::operationBtn(QChar operation) // 输入符号
{
    // numTime 后的 同一轮 operatorTime
    if (numTime) // 第一次输入符号
    {
        // 仅 inputEdit.text() == "0" 时，inputStr.isEmpty() == true
        // 避免将此时 空的inputStr 写入 showEdit
        if (inputStr.isEmpty())
        {
            inputStr = "0";
        }
        ui->showEdit->setText(inputStr);
    }
    else // 重复输入符号，先删除 上一个，在写入输入的
    {

        ui->showEdit->backspace();
    }

    // 如果 有储存的stackStr，即两轮 numTime 后的 第二轮 operatorTime，此时进行一次计算
    if (!stackStr.isEmpty())
    {
        equalBtn();
        ui->showEdit->setText(stackStr);
    }

    zeroIsLegal = true; // 输入符号后 第一个数字 可以是 0
    numTime = false;

    // if (ui->showEdit->text().size()) // 忘了啥用了
    ui->showEdit->insert(operation);
}

void demo::equalBtn() // 计算结果
{
    numTime = true;
    switch (stackOperation)
    {
    case '+':
        stackStr = QString::number(stackStr.toDouble() + inputStr.toDouble());
        break;
    case '-':
        stackStr = QString::number(stackStr.toDouble() - inputStr.toDouble());
        break;
    case '*':
        stackStr = QString::number(stackStr.toDouble() * inputStr.toDouble());
        break;
    default:
        double tmpStr = inputStr.toDouble();
        if (tmpStr != 0.0)
        {
            stackStr = QString::number(stackStr.toDouble() / tmpStr);
        }
        else
        {
            textInit();
        }
        break;
    }
}

void demo::on_pushButton_clicked() // 切换当前数字正负
{
    if (numTime && zeroIsLegal)
    {
        if (inputStr.left(1) == "-")
        {
            inputStr = inputStr.right(inputStr.size() - 1);
        }
        else
        {
            inputStr = "-" + inputStr;
        }
        ui->inputEdit->setText(inputStr);
    }
}

void demo::on_btn_0_clicked()
{
    if (zeroIsLegal)
    {
        digitBtn('0');
    }
}

void demo::on_btn_point_clicked()
{
    // 输入符号后直接输入 '.' 前先补 '0'
    if (numTime == false)
    {
        digitBtn('0');
    }

    // 整数才可以输入 '.'
    if (numIsInit)
    {
        numLength--;                      // '.'不占限制长度
        inputStr = ui->inputEdit->text(); // 初始化后的第一个 0 不写入 inputStr
        digitBtn('.');
        numIsInit = false;
    }
}

void demo::on_btn_equal_clicked()
{
    if (numTime && !stackStr.isEmpty())
    {
        // 在 showEdit 中添加 = ，在 inputEdit 中显示结果
        ui->showEdit->insert(inputStr + "=");
        equalBtn();
        ui->inputEdit->setText(stackStr);

        // 计算结果为 0 时，会出现可以无限0的情况
        if (stackStr != "0")
        {
            inputStr = stackStr;
        }
        else
        {
            zeroIsLegal = false;
            inputStr.clear();
        }

        // 保障计算结果依旧有长度限制
        numLength = inputStr.size();
        if (inputStr.contains('.'))
        {
            numIsInit = false;
            if (inputStr.left(1) == "0")
            {
                numLength--;
            }
            numLength--;
        }

        // 清空储存的数字使 = 后未经过数字 +-*/ 失效
        stackStr.clear();
    }
}

void demo::on_btn_percent_clicked()
{
    // 条件：当前不是符号环节 && 不是 0
    if (numTime && zeroIsLegal)
    {
        // 特殊情况 "0." 与 "0.0" 等，直接退出
        double tmpNum = inputStr.toDouble();
        if (tmpNum == 0.0)
        {
            return;
        }

        // 结尾 0 的情况下，把不必要的 0 都删去
        inputStr = QString::number(tmpNum);

        // 条件：'.' 后的数字小于4位，仅符合条件的进行变换
        int tmpIndex = inputStr.indexOf(QChar('.'), 0);
        if (tmpIndex == -1 || (inputStr.size() - tmpIndex < 6))
        {
            inputStr = QString::number(tmpNum / 100);
            numIsInit = false;
        }

        // 到这一步 inputStr 不可能是整数
        numLength = inputStr.size() - 1;
        if (inputStr.at(0) == '0')
        {
            numLength--;
        }
        ui->inputEdit->setText(inputStr);
    }
}

void demo::on_btn_back_clicked()
{
    // 仅 numTime 和 当前数不为 0 时可删除
    if (numTime && zeroIsLegal)
    {
        if (inputStr.right(1) == ".") // 删除 '.'
        {
            if (numLength == 0) // 仅 "0." 的情况下
                zeroIsLegal = false;
            numIsInit = true;
            ui->inputEdit->backspace();
        }
        else if (numLength == 1 && numIsInit) // 使 一位整数 变成 0
        {
            zeroIsLegal = false;
            ui->inputEdit->setText("0");
            numLength--;
        }
        else // 剩下的情况
        {
            ui->inputEdit->backspace();
            numLength--;
        }

        // 最后删除 因为要先 对要删除的内容 进行判断
        inputStr.chop(1);
    }
}

void demo::on_btn_del_clicked()
{
    textInit();
}

void demo::on_btn_add_clicked()
{
    operationBtn('+');
}

void demo::on_btn_subtract_clicked()
{
    operationBtn('-');
}

void demo::on_btn_multiply_clicked()
{
    operationBtn('*');
}

void demo::on_btn_divide_clicked()
{
    operationBtn('/');
}

void demo::on_btn_1_clicked()
{
    digitBtn('1');
}

void demo::on_btn_2_clicked()
{
    digitBtn('2');
}

void demo::on_btn_3_clicked()
{
    digitBtn('3');
}

void demo::on_btn_4_clicked()
{
    digitBtn('4');
}

void demo::on_btn_5_clicked()
{
    digitBtn('5');
}

void demo::on_btn_6_clicked()
{
    digitBtn('6');
}

void demo::on_btn_7_clicked()
{
    digitBtn('7');
}

void demo::on_btn_8_clicked()
{
    digitBtn('8');
}

void demo::on_btn_9_clicked()
{
    digitBtn('9');
}
