#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QTextEdit>
#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent,QPlainTextEdit * textEdit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    pTextEdit=textEdit;
    ui->rdDown->setChecked(true);   //初始选中向下
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

//替换
void ReplaceDialog::on_btreplace_clicked()
{
    QString target=ui->searchText->text();   //待测 获取被替换关键词
    QString to=ui->targetText->text();      //获取替换内容

    if((pTextEdit!=nullptr)&&(target!="")&&to!=""){
        QString selText=pTextEdit->textCursor().selectedText();
        if(selText==target)
            pTextEdit->insertPlainText(to);
        on_btsearch_clicked();
    }
}

//全部替换
void ReplaceDialog::on_btreplaceAll_clicked()
{
    QString target=ui->searchText->text();   //待测 获取被替换关键词
    QString to=ui->targetText->text();      //获取替换内容
    if((pTextEdit!=nullptr)&&(target!="")&&to!=""){
        QString text=pTextEdit->toPlainText();
        text.replace(target,to,ui->caseSensitive->isChecked()?Qt::CaseSensitive : Qt::CaseInsensitive);
        pTextEdit->clear();
        pTextEdit->insertPlainText(text);
    }
}

//取消
void ReplaceDialog::on_btCancel_clicked()
{
    accept();
}

//查找下一个
void ReplaceDialog::on_btsearch_clicked()
{
    QString target=ui->searchText->text();   //待测 获取搜索关键词

    if(target==""||pTextEdit==nullptr){
        return;
    }
    QString text=pTextEdit->toPlainText();      //文本框所有内容获取
    QTextCursor c=pTextEdit->textCursor();

    int index=-1;   //初始为-1，没找到

    if(ui->rdDown->isChecked()) //往下搜索的按钮
    {
        index=text.indexOf(target,c.position(),ui->caseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);  //获取当前光标位置并设置区分大小写按钮
        if(index>=0){
            c.setPosition(index);   //移动光标
            c.setPosition(index+target.length(),QTextCursor::KeepAnchor);    //
            pTextEdit->setTextCursor(c);
        }
    }
    else if(ui->rdUp->isChecked())  //往下搜索的按钮
    {
        index=text.lastIndexOf(target,c.position()-text.length()-1,ui->caseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);    //从字符串对应的位置往前找
        if(index>=0){
            c.setPosition(index + target.length());   //移动光标 +目标字符串长度
            c.setPosition(index,QTextCursor::KeepAnchor);    //
            pTextEdit->setTextCursor(c);
        }
    }

    //若找不到文本，弹出提示窗口
    if(index<0){
        QMessageBox msg(this);
        msg.setWindowTitle("Attention");
        msg.setText("该文本中找不到 “"+target+"” !");
        msg.setWindowFlag(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}

