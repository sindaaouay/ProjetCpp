#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QFileDialog>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QDebug>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "partnership.h"
#include <QSystemTrayIcon>
#include <QtCharts/QPieSeries>
 /*/stats*/
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>





/*pdf*/
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QPdfWriter>
#include <QDesktopServices>
#include <QFileDialog>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include <QLabel>
#include <QDebug>
#include <QComboBox>
#include <QTimer>
#include <QThread>


// Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    trayIcon(nullptr),
    graphicsScene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    // Inside the constructor







    connect(ui->openChatbotButton, &QPushButton::clicked, this, &MainWindow::on_openChatbotButton_clicked);
    connect(ui->chatbotSendButton, &QPushButton::clicked, this, &MainWindow::on_chatbotSendButton_clicked);
    ui->graphicsView->setScene(graphicsScene);
    connect(ui->statsButton, &QPushButton::clicked, this, &MainWindow::on_statsButton_clicked);
    connect(ui->checkContractsButton, &QPushButton::clicked, this, &MainWindow::on_checkContractsButton_clicked);
    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::on_backButton_clicked);
}





// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_modbutton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1); //update
}


void MainWindow::on_addbutton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0); //add
}


void MainWindow::on_viewbutton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3); //view
}
void MainWindow::on_statsButton_clicked()
{
    // Switch to the statistics page
    ui->stackedWidget->setCurrentWidget(ui->statisticsPage);

    // Clear any existing graphics in the scene
    graphicsScene->clear();

    // Display the statistics
    displayStatisticsByAddress();
}






// Example: Notification after adding a partner
void MainWindow::on_ajouterButton_clicked()
{
    QString partner_name = ui->partnername_le->text();
    QString business_sector = ui->Sector_le->text();
    QString phone_prt = ui->phoneprt_le->text();
    QString address_prt = ui->adresseprt_le->text();
    QString email_prt = ui->emailprt_le->text();
    QString type_prt = ui->typeprt_le->text();
    QDate start_date = ui->sdate_le->date();
    QDate end_date = ui->edate_le->date();
    QString statut_prt = ui->statutprt_le->text();

    partnership P(partner_name, business_sector, phone_prt, address_prt, email_prt, type_prt, start_date, end_date, statut_prt);

    if (P.ajouter()) {
        ui->tableView->setModel(prt.afficher_partnership());

        // Afficher un QMessageBox pour informer l'utilisateur
        QMessageBox::information(this, "Ajout Réussi",
            QString("Le partenariat a été ajouté avec succès :\nNom : %1\nSecteur : %2\nTéléphone : %3\nAdresse : %4")
            .arg(partner_name)
            .arg(business_sector)
            .arg(phone_prt)
            .arg(address_prt));
    } else {
        QMessageBox::critical(this, "Erreur", "L'ajout du partenariat a échoué.");
    }

}









void MainWindow::on_reloadbutton_clicked()
{
    ui->tableView->setModel(prt.afficher_partnership());
}
void MainWindow::on_delete_prt_clicked()
{
    QString ref=ui->prt_a_supp->text();
    bool test=prt.supprimer(ref);

    if(test)
    {
        //Refresh (Actualiser)
        ui->tableView->setModel(prt.afficher_partnership());

        QMessageBox::information(nullptr,QObject::tr("Smart Printing System"),
                                   QObject::tr("Suppression effectuée\n"
                                                  "Cliquez sur annuler pour quitter."),QMessageBox::Cancel);

        ui->nameprt_update->clear();
        ui->sector_update->clear();
        ui->phoneprt_update->clear();
        ui->adressprt_update->clear();
        ui->emailprt_update->clear();
        ui->typeprt_update->clear();
        ui->sdate_update->clear();
        ui->edate_update->clear();
        ui->statutprt_update->clear();

    }
    else
    {
         QMessageBox::critical(nullptr,QObject::tr("Smart Printing System"),
                                   QObject::tr("Suppression non effectuée.\n"
                                               "La référence n'existe pas."
                                               "Cliquez sur annuler pour quitter."),QMessageBox::Cancel);
    }

}


void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    QString value=ui->tableView->model()->data(index).toString();

    QSqlQuery qry;

        qry.prepare("select * from PARTNERSHIP where NOM_PRT='"+value+"'");
        if(qry.exec())
        {
            while(qry.next())
            {
                ui->nameprt_update->setText(qry.value(0).toString());
                ui->sector_update->setText(qry.value(1).toString());
                ui->phoneprt_update->setText(qry.value(2).toString());
                ui->adressprt_update->setText(qry.value(3).toString());
                ui->emailprt_update->setText(qry.value(4).toString());
                ui->typeprt_update->setText(qry.value(5).toString());
                ui->sdate_update->setDate(qry.value(6).toDate());
                ui->edate_update->setDate(qry.value(7).toDate());
                ui->statutprt_update->setText(qry.value(8).toString());

                ui->prt_a_supp->setText(qry.value(0).toString());
            }
       }
}





void MainWindow::on_modifier_btn_clicked()
{
        partnership P;
        P.setpartner_name(ui->nameprt_update->text());
        P.setbusiness_sector(ui->sector_update->text());
        P.setphone_prt(ui->phoneprt_update->text());
        P.setaddress_prt(ui->adressprt_update->text());
        P.setemail_prt(ui->emailprt_update->text());
        P.settype_prt(ui->typeprt_update->text());
        P.setstart_date(ui->sdate_update->date());
        P.setend_date(ui->edate_update->date());
        P.setStatut_prt(ui->statutprt_update->text());


       bool check=P.modifier();

        if (check)
        {
            //Refresh (Actualiser)
            ui->tableView->setModel(prt.afficher_partnership());

            QMessageBox::information(nullptr, QObject::tr("Smart Printing System"),
                                      QObject::tr("Modification avec succés.\n"
                                                  "Cliquez sur annuler pour quitter."), QMessageBox::Cancel);

            ui->nameprt_update->clear();
            ui->sector_update->clear();
            ui->phoneprt_update->clear();
            ui->adressprt_update->clear();
            ui->emailprt_update->clear();
            ui->typeprt_update->clear();
            ui->sdate_update->clear();
            ui->edate_update->clear();
            ui->statutprt_update->clear();
        }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("Smart Printing System"),
                                      QObject::tr("Modification échouée.\n"
                                                  "Cliquez sur annuler pour quitter."), QMessageBox::Cancel);

        }


}


void MainWindow::on_lineEdit_Search_textChanged(const QString &arg1)
{
    QString rech=ui->lineEdit_Search->text();
    ui->tableView->setModel(prt.Recherchepartnership(rech));
}


void MainWindow::on_pushButton_pdf_clicked() {
    QString ref_pdf = ui->prt_a_supp->text();

    if (ref_pdf.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Exportation de fichiers PDF"),
                              QObject::tr("Pas de référence saisie. \nVeuillez saisir une référence.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home/Desktop", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        qDebug() << dir;
        QPdfWriter pdf(dir + "/Contrat.pdf");
        QPainter painter(&pdf);
        int y = 1200;
        int lineSpacing = 300;

        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        painter.drawText(200, y, "Contrat de partenariat"); // Title
        y += lineSpacing;

        painter.setFont(QFont("Arial", 12));
        QSqlQuery query;
        query.prepare("SELECT * FROM PARTNERSHIP WHERE NOM_PRT='" + ref_pdf + "'");
        if (query.exec() && query.next()) {
            painter.drawText(200, y, "Nom : " + query.value("NOM_PRT").toString());
            y += lineSpacing;
            painter.drawText(200, y, "Domaine : " + query.value("DOMAINE").toString());
            y += lineSpacing;
            painter.drawText(200, y, "Numéro : " + query.value("NUM_PRT").toString());
            y += lineSpacing;
            painter.drawText(200, y, "Adresse : " + query.value("ADRESSE").toString());
            y += lineSpacing;
            painter.drawText(200, y, "Email : " + query.value("EMAIL").toString());
            y += lineSpacing;
            painter.drawText(200, y, "Type : " + query.value("TYPE").toString());
            y += lineSpacing;
            painter.drawText(200, y, "Date début : " + query.value("DATE_DEBUT").toDate().toString("dd/MM/yyyy"));
            y += lineSpacing;
            painter.drawText(200, y, "Date fin : " + query.value("DATE_FIN").toDate().toString("dd/MM/yyyy"));
            y += lineSpacing;
            painter.drawText(200, y, "Statut : " + query.value("STATUT").toString());
        }

        // Add footer with legal terms
        y += 300; // Add some spacing before the footer
        QFont footerFont("Arial", 10);
        footerFont.setItalic(true); // Set the font to italic
        painter.setFont(footerFont);
        painter.drawText(200, y, "Entre les soussignés :");
        y += lineSpacing;
        painter.drawText(200, y, "La société, représentée par M. OO, ci-après désignée comme 'Le Client'.");
        y += lineSpacing;
        painter.drawText(200, y, "Et la société OO, représentée par M. OO, ci-après désignée comme 'Le Prestataire'.");

        // Prompt to view the PDF
        int reponse = QMessageBox::question(this, "Générer PDF", "PDF Enregistré.\nVoulez-vous l'afficher ?", QMessageBox::Yes | QMessageBox::No);
        if (reponse == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(dir + "/Contrat.pdf"));
        }

        painter.end();
        ui->prt_a_supp->clear();
    }
}
void MainWindow::on_comboBox_tri_activated() {
    QString tri = ui->comboBox_tri->currentText();
    QString column;

    // Map user-friendly names to database columns
    if (tri == "name") {
        column = "NOM_PRT";
    } else if (tri == "address") {
        column = "ADRESSE";
    } else if (tri == "statut") {
        column = "STATUT";
    } else {
        qDebug() << "Invalid sorting option selected.";
        return; // Exit if column is not valid
    }

    // Call the sorting method
    QSqlQueryModel* model = prt.trierPar(column);
    if (model) {
        ui->tableView->setModel(model);
    } else {
        qDebug() << "Sorting failed.";
    }
}
void MainWindow::on_openChatbotButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->chatbotPage);  // Switch to chatbot page
}
void MainWindow::on_chatbotSendButton_clicked() {
    QString userQuestion = ui->chatbotLineEdit->text();  // Get user input
    if (userQuestion.isEmpty()) return;  // Do nothing if input is empty

    // Display user's question
    ui->chatbotTextBrowser->append("Vous: " + userQuestion);

    // Get chatbot's response
    QString botResponse = chatbot.respondToUser(userQuestion);

    // Display chatbot's response
    ui->chatbotTextBrowser->append("Chatbot: " + botResponse);

    // Clear input field
    ui->chatbotLineEdit->clear();
}



// Method to display statistics by address
void MainWindow::displayStatisticsByAddress()
{
    QSqlQuery query;
    query.prepare("SELECT ADRESSE, COUNT(*) AS count FROM PARTNERSHIP GROUP BY ADRESSE");
    if (!query.exec()) {
        qDebug() << "Error executing query for Address statistics:" << query.lastError().text();
        return;
    }

    // Variables for drawing
    int barWidth = 50;
    int spacing = 20;
    int xOffset = 50; // Start drawing bars slightly to the right
    int yOffset = 300; // Base of the bars (y-axis origin)
    int maxHeight = 200; // Max height of the bar
    int maxValue = 0;

    // Calculate the maximum value for scaling
    while (query.next()) {
        int count = query.value(1).toInt();
        if (count > maxValue)
            maxValue = count;
    }

    query.first(); // Reset query iterator

    // Draw the bars
    while (query.next()) {
        QString address = query.value(0).toString();
        int count = query.value(1).toInt();

        // Calculate bar height proportionally
        int barHeight = (count * maxHeight) / maxValue;

        // Draw a rectangle for the bar
        graphicsScene->addRect(xOffset, yOffset - barHeight, barWidth, barHeight, QPen(Qt::black), QBrush(Qt::blue));

        // Add text for the count
        graphicsScene->addText(QString::number(count))->setPos(xOffset + barWidth / 4, yOffset - barHeight - 20);

        // Add text for the address
        graphicsScene->addText(address)->setPos(xOffset, yOffset + 10);

        // Move to the next bar position
        xOffset += barWidth + spacing;
    }
}
//arduino

void MainWindow::on_checkContractsButton_clicked() {
      QDate today = QDate::currentDate();
      QDate tomorrow = today.addDays(1);
      QDate dayAfterTomorrow = today.addDays(2);

    QSqlQuery query;
    query.prepare("SELECT NOM_PRT, DATE_FIN FROM PARTNERSHIP WHERE DATE_FIN <= :dayAfterTomorrow");
    query.bindValue(":dayAfterTomorrow", dayAfterTomorrow);

    if (!query.exec()) {
        qDebug() << "Error querying contracts:" << query.lastError().text();
        return;
    }

    QString notificationMessage = "<h2>Contracts nearing end or already ended:</h2>";

    bool contractsFound = false;
    while (query.next()) {
        QDate endDate = query.value("DATE_FIN").toDate();
        QString contractName = query.value("NOM_PRT").toString();

        if (endDate < today) {
            notificationMessage += QString("<p>- <b>%1</b> has already ended on %2.</p>")
                .arg(contractName)
                .arg(endDate.toString("dd/MM/yyyy"));
        } else if (endDate == today) {
            notificationMessage += QString("<p>- <b>%1</b> ends today.</p>").arg(contractName);
        } else if (endDate == tomorrow) {
            notificationMessage += QString("<p>- <b>%1</b> will end tomorrow.</p>").arg(contractName);
        } else if (endDate == dayAfterTomorrow) {
            notificationMessage += QString("<p>- <b>%1</b> will end in two days.</p>").arg(contractName);
        }

        contractsFound = true;
    }

    if (!contractsFound) {
        notificationMessage = "<h2>No contracts ending soon.</h2>";
    }

    // Set the notification message to the QTextBrowser in the notification page
    ui->notificationMessage->setHtml(notificationMessage);

    // Switch to the notification page
    ui->stackedWidget->setCurrentWidget(ui->notificationPage);
}

void MainWindow::on_backButton_clicked() {
    ui->stackedWidget->setCurrentIndex(0);  // Switch back to the main page
}
