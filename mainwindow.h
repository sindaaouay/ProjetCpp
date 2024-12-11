#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "chatbot.h"


#include <QMainWindow>
#include "partnership.h"  // Include the header file
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>
#include <QGraphicsScene>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QFile>
#include <QTextStream>


#include <QSystemTrayIcon>

#include <QDateTime>
struct Change {
    QString action;  // Example: "Added", "Modified", "Deleted"
    QString details; // Details of the change
    QDateTime timestamp;  // Time of the change
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


    void on_checkContractsButton_clicked();


private slots:
    void on_modbutton_clicked();

    void on_addbutton_clicked();

    void on_viewbutton_clicked();

    void on_ajouterButton_clicked();


    void on_reloadbutton_clicked();

    void on_delete_prt_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_modifier_btn_clicked();

    void on_lineEdit_Search_textChanged(const QString &arg1);

    void on_pushButton_pdf_clicked();

    void on_comboBox_tri_activated();
    void on_openChatbotButton_clicked();// Slot for navigating to chatbot page

    void on_chatbotSendButton_clicked();  // Slot for sending chatbot messages




    void on_statsButton_clicked(); // Slot for the statistics button
    void on_backButton_clicked();





private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon; // System tray icon for notifications
     QList<Change> recentChanges; // To store recent changes

    QTableView *tableView;
    partnership prt;
    ChatBot chatbot;
    QGraphicsScene *graphicsScene;
    void displayStatisticsByAddress();



};


#endif // MAINWINDOW_H

