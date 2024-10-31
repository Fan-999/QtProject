#include "mainwindow.h"
#include "ui_mainwindow.h"
#include    <QFile>
#include    <QCoreApplication>
#include    <QLabel>
#include    <QTextCharFormat>
#include    <QMessageBox>
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>
#include <QTextBlock>

void MainWindow::iniUI()
{
    // flabCurFile=new QLabel;
    // flabCurFile->setMidLineWidth(150);
    // flabCurFile->setText("当前文件：");
    // ui->statusbar->addWidget(flabCurFile);

    // progressBar=new QProgressBar;
    // progressBar->setMinimum(5);
    // progressBar->setMaximum(50);
    // progressBar->setValue(ui->textEdit->font().pointSize());
    // ui->statusbar->addWidget(progressBar);

    // spinFontSize=new QSpinBox;
    // spinFontSize->setMinimum(5);
    // spinFontSize->setMaximum(50);
    // spinFontSize->setValue(ui->textEdit->font().pointSize());//初始值
    // spinFontSize->setMinimumWidth(50);//设置组件最小宽度
    // ui->toolBar->addWidget(new QLabel("字体大小 ")); //不引用的Label无需定义变量
    // ui->toolBar->addWidget(spinFontSize); //SpinBox添加到工具栏
    // ui->toolBar->addSeparator(); //工具栏上增加分隔条

    // comboFont=new QFontComboBox;
    // ui->toolBar->addWidget(new QLabel("字体"));
    // ui->toolBar->addWidget(comboFont);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iniUI();
    //设置状态栏内容
    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length: "+QString::number(0)+"   lines: "+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: "+QString::number(0)+"   col:  "+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *autor=new QLabel(ui->statusbar);
    autor->setText("xyx");
    ui->statusbar->addPermanentWidget(autor);

    //将撤销、恢复、复制、剪切、粘贴的按钮显示均置为假
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actioncopy->setEnabled(false);
    ui->actioncut->setEnabled(false);
    ui->actv->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode=ui->textEdit->lineWrapMode();

    //判断是否会自动换行
    if(mode==QTextEdit::NoWrap){
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->action_W->setChecked(true);
    }
    else{
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->action_W->setChecked(false);
    }

    //状态栏与工具栏的初始化
    ui->action_State->setChecked(true);
    ui->action_T->setChecked(true);

    // 连接"更新状态栏信息"的信号到槽函数
    connect(ui->textEdit, &QPlainTextEdit::textChanged, this, &MainWindow::updateStatusBar);

    // 连接“关于”按钮的信号到槽函数
    // connect(ui->actAbout, &QAction::triggered, this, &MainWindow::on_actAbout_triggered);
    // setCentralWidget(ui->textEdit);
}
MainWindow::~MainWindow()
{
    delete ui;
}
//粗体按下去的状态
void MainWindow::on_actb_triggered(bool checked)
{
    QTextCharFormat fmt; //格式
    fmt=ui->textEdit->currentCharFormat();//获取当前选择文字的格式
    if (checked) // 相当于调用ui->actFontBold->isChecked();读取Action的check状态
        fmt.setFontWeight(QFont::Bold);
    else
        fmt.setFontWeight(QFont::Normal);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}
void MainWindow::on_action_5_triggered(bool checked)
{
    QTextCharFormat fmt;
    fmt=ui->textEdit->currentCharFormat();
    fmt.setFontItalic(checked);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::on_actunder_triggered(bool checked)
{
    QTextCharFormat fmt;
    fmt=ui->textEdit->currentCharFormat();
    fmt.setFontUnderline(checked);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::on_textEdit_selectionChanged()
{
    QTextCharFormat fmt;
    fmt=ui->textEdit->currentCharFormat(); //获取文字的格式

    ui->action_5->setChecked(fmt.fontItalic()); //是否斜体
    ui->actb->setChecked(fmt.font().bold()); //是否粗体
    ui->actunder->setChecked(fmt.fontUnderline()); //是否有下划线

}

// void MainWindow::on_actAbout_triggered()
// {
//     QMessageBox aboutBox;
//     aboutBox.setWindowTitle(tr("About"));
//     aboutBox.setText(tr("姓名：Xyx\n学号：2022414040234\n班级：22软件工程2班"));
//     aboutBox.exec();
// }

//关于窗口
void MainWindow::on_actAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}

//查找窗口
void MainWindow::on_actionfind_triggered()
{
    SearchDialog dlg(this,ui->textEdit);
    dlg.exec();
}

//替换窗口
void MainWindow::on_action_R_triggered()
{
    ReplaceDialog dlg(this,ui->textEdit);
    dlg.exec();
}

//新建文件
void MainWindow::on_actionNew_triggered()
{
    ui->textEdit->clear();  //清空编辑器内容
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));   //修改编辑器名字
}

//打开文件
void MainWindow::on_actopen_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text files (*.txt) ;; All(*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly|QFile::Text)) //以文本方式打开
    {
        QMessageBox::warning(this,"..","打开文件失败");   //无法打开，输出提示
        return;
    }

    filePath=filename;

    QTextStream in(&file);  //打开的文件
    QString text=in.readAll();  //一次性读进
    ui->textEdit->insertPlainText(text);    //显示在编辑器上
    file.close();   //关闭文件
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());   //将编辑器的名字改为对应文件路径
}

//保存文件
void MainWindow::on_actionSave_triggered()
{

    // 如果filePath为空，表示这是第一次保存文件，需要让用户选择路径
    if (filePath.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files (*.txt)"));
        if (filePath.isEmpty()) {
            // 用户取消选择，不进行保存操作
            return;
        }
    }

    QFile file(filePath);
    if(!file.open(QFile::WriteOnly|QFile::Text)) //以文本方式保存
    {
        QMessageBox::warning(this,"..","保存文件失败");   //无法打开，输出提示
        return;
    }

    QTextStream out(&file);
    QString text=ui->textEdit->toPlainText();   //将文件内容读入text
    out<<text;
    file.close();
}

//另存文件
void MainWindow::on_actionAsave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "另存为", ".", tr("Text files (*.txt)"));
    if (filename.isEmpty()) {
        // 用户取消选择，不进行保存操作
        return;
    }

    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) { // 以文本方式保存
        QMessageBox::warning(this, "..", "保存文件失败"); // 无法打开，输出提示
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();   // 将编辑器的内容读入text
    out << text;
    file.close();

    filePath = filename;    // 更新当前文件路径为新保存的路径
}

//撤销
void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

//恢复
void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

//剪切
void MainWindow::on_actioncut_triggered()
{
    ui->textEdit->cut();
    ui->actv->setEnabled(true);
}

//复制
void MainWindow::on_actioncopy_triggered()
{
    ui->textEdit->copy();
    ui->actv->setEnabled(true);
}

//粘贴
void MainWindow::on_actv_triggered()
{
    ui->textEdit->paste();

}

//剪切、复制的选中
void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actioncopy->setEnabled(b);
    ui->actioncut->setEnabled(b);
}

//恢复的选中
void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}

//撤销选中
void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}

//全选
void MainWindow::on_action_A_triggered()
{
    ui->textEdit->selectAll(); // 选中文本编辑器中的所有文本
    ui->actioncut->setEnabled(true); // 启用剪切操作
    ui->actioncopy->setEnabled(true); // 启用复制操作
}

//清空
void MainWindow::on_actspace_triggered()
{
    ui->textEdit->clear();
}

//字体
void MainWindow::on_action_F_triggered()
{
    bool ok=false;
    QFont font=QFontDialog::getFont(&ok,this);
    if(ok)
        ui->textEdit->setFont(font);

}

//字体背景颜色
void MainWindow::on_actionIconBackColor_triggered()
{

}

//字体颜色
void MainWindow::on_actionIconColor_triggered()
{
    QColor color=QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {color: %1}").arg(color.name()));
    }
}

//编辑器颜色
void MainWindow::on_actionBackColor_triggered()
{
    QColor color=QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {background-color: %1}").arg(color.name()));
    }
}

//自动换行(根据界面窗口)
void MainWindow::on_action_W_triggered()
{
    QPlainTextEdit::LineWrapMode mode=ui->textEdit->lineWrapMode();

    //判断是否会自动换行
    if(mode==QTextEdit::NoWrap){
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->action_W->setChecked(true);
    }
    else{
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->action_W->setChecked(false);
    }
}

//状态栏
void MainWindow::on_action_State_triggered()
{
    bool visible=ui->statusbar->isVisible();  //获取当前状态
    ui->statusbar->setVisible(!visible);  //根据当前状态切换
    ui->action_State->setChecked(!visible); //状态切换
}

//工具栏
void MainWindow::on_action_T_triggered()
{
    bool visible=ui->toolBar->isVisible();  //获取当前状态
    ui->toolBar->setVisible(!visible);  //根据当前状态切换
    ui->action_T->setChecked(!visible); //状态切换
}

// 更新状态栏信息
void MainWindow::updateStatusBar()
{
    // 获取当前光标位置
    int cursorLine = ui->textEdit->textCursor().blockNumber() + 1; // 行号（从1开始）
    int cursorColumn = ui->textEdit->textCursor().columnNumber() + 1; // 列号（从1开始）

    // 获取文本长度和行数
    int textLength = ui->textEdit->toPlainText().length(); // 文本长度
    int lineCount = ui->textEdit->document()->blockCount(); // 行数

    // 更新状态栏中的光标位置标签
    statusCursorLabel.setText(QString("Ln: %1   Col: %2").arg(cursorLine).arg(cursorColumn));
    // 更新状态栏中的文本长度和行数标签
    statusLabel.setText(QString("Length: %1   Lines: %2").arg(textLength).arg(lineCount));
}
