#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <iostream>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEdit->setReadOnly(true);//只读模式
    ui->lineEdit->move(QPoint(0,0));//设置lineEdit位置
    ui->lineEdit->setAlignment(Qt::AlignRight);//从右侧显示
    setWindowTitle(QString("IU我老婆"));//设置标题
    initScene();
}
void Widget::initScene()
{
    setFixedSize(261,373); //设置窗口大小
}


Widget::~Widget()
{
    delete ui;
}

void Widget::getChar()
{
    ba=in.toLatin1();
}

void Widget::getSuffix()
{
  for(int i=0;i<ba.size();i++)
  {
      if((ba[i]>='0' && ba[i]<='9')||ba[i]=='.'||ba[i]=='e'||ba[i]=='p')//1,数字直接输出
      {
         QString str;
         QByteArray m;
         for(;(ba[i]>='0' && ba[i]<='9')||ba[i]=='.'||ba[i]=='e'||ba[i]=='p';i++)
         {
             //将一个数字存储到字节数组m中
             char x=ba[i];
             m.append(x);
         }
         //将字节数组m转换为QString，并将字符串输出
         str=QString(m);
         out.append(str);
         i--;
      }
       else if(s1.isEmpty())//2,遇见非数字时, 如果堆栈为空，则直接把该字符放入堆栈
      {
          s1.push(ba[i]);
      }else if(ba[i]=='+' || ba[i]=='-')
          //2.1,如果是+或- 那么遍历堆栈栈顶元素 一直输出优先级>=加减的 即输出栈顶的+-*/^sqrt sin,
          //当遇到其他字符时（如左括号）或堆栈空时break
      {
          while(!s1.empty()){
              if((s1.top()=='+') || (s1.top()=='-')||(s1.top()=='*') || (s1.top()=='/')|| (s1.top()=='^')|| (s1.top()== 'k')|| (s1.top()== 'a')|| (s1.top()== 'b')|| (s1.top()== 'c')|| (s1.top()== 'd'))
              {
                  //把字符转换为字符串输出
                  QString str=QString(s1.pop());
                  out.append(str);
               }else break;
          }
          s1.push(ba[i]);
      }else if((ba[i]=='*') || (ba[i]=='/'))
          //2.2,如果是*或/ 那么遍历堆栈栈顶元素 一直输出优先级>=乘除的 即输出栈顶的*/^sqrt,
          //当遇到其他字符时（如左括号，减号，除号）或堆栈空时break
      {
          while(!s1.empty()){
              if((s1.top()=='*') || (s1.top()=='/')|| (s1.top()=='^')|| (s1.top()=='k')|| (s1.top()== 'a')|| (s1.top()== 'b')|| (s1.top()== 'c')|| (s1.top()== 'd')){
                  //把字符转换为字符串输出
                  QString str=QString(s1.pop());
                  out.append(str);
               }else break;
          }
          s1.push(ba[i]);
      }else if(ba[i]=='^'||ba[i]=='k'||ba[i]=='a'||ba[i]=='b'||ba[i]=='c'||ba[i]=='d')
          //2.2,如果是^,sqrt等等，那么遍历堆栈栈顶元素 一直输出优先级>=乘方开方的 即输出栈顶的^ sqrt,
          //当遇到其他字符时（如左括号，减号，除号）或堆栈空时break
      {
          while(!s1.empty()){
              if (s1.top()=='^'||s1.top()=='k'||s1.top()=='a'||s1.top()=='b'||s1.top()=='c'||s1.top()=='d'){
                  //把字符转换为字符串输出
                  QString str=QString(s1.pop());
                  out.append(str);
               }else break;
          }
          s1.push(ba[i]);
      }else if(ba[i]==')')
          //2.3,如果该字符是右括号时，一直输出栈顶元素，直到遇见左括号为止break:
      {
          while(!s1.empty()){
              if(s1.top()=='('){
                  s1.pop();
                  break;
              }
              //把字符转换为字符串输出
              QString str=QString(s1.pop());
              out.append(str);
          }
      }else if(ba[i]=='(')
          //2.4,如果该字符是左括号时，直接放入堆栈
      {
          s1.push(ba[i]);
      }
  }
  //字符串循环结束后 再输出堆栈中剩余的数据
  while(!s1.empty()){
      //把字符转换为字符串输出
      QString str=QString(s1.pop());
      out.append(str);
  }
  //输出后缀表达式
  qDebug()<<out;
}

void Widget::Calc_Suffix()
{
  for(int i=0;i<out.size();i++)
  {
      bool is_Num =true;
      out[i].toFloat(&is_Num);
      if(is_Num)//如果是数字，直接入栈
      {
          s2.push(out[i].toFloat(&is_Num));
      }else if(out[i]=='e'){
          s2.push(2.71828);
      }else if(out[i]=='p'){
          s2.push(3.1415926);
      }
      else if(out[i]=="+"||out[i]=="-"||out[i]=="*"||out[i]=="/"||out[i]=="^")//如果是运算符，弹栈两次，并进行运算，运算后，结果入栈
      {
          float x=s2.pop();
          float y=s2.pop();
          if(out[i]=="+")
          {
              s2.push(y+x);
          }else if(out[i]=="-"){
              s2.push(y-x);
          }else if(out[i]=="*"){
              s2.push(y*x);
          }else if(out[i]=="/"){
              s2.push(y/x);
          }else if(out[i]=="^"){
              float temp=y;
              if(x==0)
                  s2.push(1);
              else
              {
                for (int j=1;j<x;j++)
                {
                  y=y*temp;
                }s2.push(y);
              }
          }
      }else if(out[i]=="k"||out[i]=="a"||out[i]=="b"||out[i]=="c"){
          float x=s2.pop();
          if(out[i]=="k"){
              s2.push(sqrt(x));
          }else if(out[i]=="a"){
              s2.push(sin(x));
          }else if(out[i]=="b"){
              s2.push(cos(x));
          }else if(out[i]=="c"){
              s2.push(log(x));
          }
      }else if(out[i]=="d"){
          int y=s2.pop();
          int f[1000];
          int j=0,n=1;
          int  m=0;
          while(y){
              f[j]=y%2;
              y/=2;
              j++;
          }
         for(int z=j-1;z>=0;z--){
             for (int r=0;r<z;r++) {
                 n=n*10;

             }m+=f[z]*n;
             n=1;
         } s2.push(m);
      }

  }
  result = s2.pop();
  qDebug()<<result;
}

void Widget::m_init()
{
    ba.clear();
    out.clear();
}

void Widget::on_pushButton_clicked()
{
    in=ui->lineEdit->text();//获取表达式
    getChar();
    getSuffix();
    Calc_Suffix();
    ui->lineEdit->clear();//清空文本编辑框
    ui->lineEdit->setText(QString("%1").arg(result));//显示结果
    m_init();
}

void Widget::on_pushButton_2_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(0));
}

void Widget::on_pushButton_3_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(1));
}

void Widget::on_pushButton_4_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(2));
}

void Widget::on_pushButton_5_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(3));
}

void Widget::on_pushButton_6_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(4));
}

void Widget::on_pushButton_7_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(5));
}

void Widget::on_pushButton_8_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(6));
}

void Widget::on_pushButton_9_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(7));
}

void Widget::on_pushButton_10_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(8));
}

void Widget::on_pushButton_11_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(9));
}

void Widget::on_pushButton_12_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("+"));
}

void Widget::on_pushButton_13_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("-"));
}

void Widget::on_pushButton_14_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("*"));
}

void Widget::on_pushButton_15_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("/"));
}

void Widget::on_pushButton_20_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("."));
}

void Widget::on_pushButton_18_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("("));
}

void Widget::on_pushButton_19_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg(")"));
}

void Widget::on_pushButton_21_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("k"));
}

void Widget::on_pushButton_22_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("^"));
}

void Widget::on_pushButton_23_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("p"));
}

void Widget::on_pushButton_24_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("d"));
}

void Widget::on_pushButton_25_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("e"));
}

void Widget::on_pushButton_26_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("a"));
}

void Widget::on_pushButton_27_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("b"));
}

void Widget::on_pushButton_28_clicked()
{
    QString str=ui->lineEdit->text();
    ui->lineEdit->setText(QString("%1%2").arg(str).arg("c"));
}

void Widget::on_pushButton_16_clicked()
{
    //清空所有内容，并初始化
    ui->lineEdit->clear();
    m_init();
}

void Widget::on_pushButton_17_clicked()
{
    //退格删除一个字符
    ui->lineEdit->backspace();
}
