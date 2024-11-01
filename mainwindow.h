#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include <QSpinBox>
#include <QFontComboBox>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel statusCursorLabel;
    QLabel statusLabel;
    QLabel * flabCurFile;
    QProgressBar * progressBar;
    QSpinBox * spinFontSize;
    QFontComboBox * comboFont;

    QString filePath;   //记录文件

    void iniUI();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_MainWindow_iconSizeChanged(const QSize &iconSize);

    void on_actb_triggered(bool checked);//粗体的槽函数

    void on_actunder_triggered(bool checked);//下划线的槽函数

    void on_action_5_triggered(bool checked);//斜体的槽函数

    void on_actAbout_triggered();//关于的槽函数

    void on_textEdit_selectionChanged();

    void on_MainWindow_toolButtonStyleChanged(const Qt::ToolButtonStyle &toolButtonStyle);

    void on_actionfind_triggered();

    void on_action_R_triggered();

    void on_actionNew_triggered();

    void on_actopen_triggered();

    void on_actionSave_triggered();

    void on_actionAsave_triggered();

    void on_actioncopy_triggered();

    void on_actv_triggered();

    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_redoAvailable(bool b);

    void on_textEdit_undoAvailable(bool b);

    void on_actionRedo_triggered();

    void on_actioncut_triggered();

    void on_actionUndo_triggered();

    void on_action_A_triggered();

    void on_actspace_triggered();

    void on_action_F_triggered();

    void on_actionIconBackColor_triggered();

    void on_actionIconColor_triggered();

    void on_actionBackColor_triggered();

    void on_action_W_triggered();

    void on_action_State_triggered();

    void on_action_T_triggered();

    void updateStatusBar(); //更新状态栏信息

    void on_textEdit_cursorPositionChanged();   //更新移动光标的实时位置

    void on_action_L_triggered(bool checked);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
