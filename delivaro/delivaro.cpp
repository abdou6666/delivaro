#include "delivaro.h"
#include "ui_delivaro.h"
#include "statistique.h"
delivaro::delivaro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::delivaro)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    ui->tableViewRec->setModel(rtmp.afficher()); //affichage table reclamation
    ui->tableViewPub->setModel(ptmp.afficher()); //affichage table publicite

    ui->tableViewRec->setSelectionBehavior(QAbstractItemView::SelectRows);
     ui->tableViewRec->setSelectionMode(QAbstractItemView::SingleSelection);


    ui->tableViewPub->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewPub->setSelectionMode(QAbstractItemView::SingleSelection);
    son = new QSound("sound.wav");
    click = new QSound("click.wav");
}

delivaro::~delivaro()
{
    delete ui;
}

void delivaro::on_pushButton_ajouterP_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

        bool nom_verif = nom_regex.exactMatch(ui->lineEdit_nom->text());
        bool id_verif=identifiant_regex.exactMatch(ui->lineEdit_ID->text());
        bool nbC_verif=nombre_commandes_regex.exactMatch(ui->lineEdit_nbC->text());
        //bool produit_verif=type_produit_regex.exactMatch(ui->lineEdit_produit->text());
       if(ui->lineEdit_ID->text()!= "" &&
               ui->lineEdit_nbC->text()!="" &&
               ui->lineEdit_nom->text() != "") {

           if (nom_verif && id_verif && nbC_verif ) {
               QString nom=ui->lineEdit_nom->text();
               QString identifiant=ui->lineEdit_ID->text();
               int nombre_commandes=ui->lineEdit_nbC->text().toInt();
               QString type_produit=ui->comboBox_produit->currentText();

               partenaire partenaire(nom,identifiant,nombre_commandes,type_produit);

              bool test=partenaire.ajouter();
               if(test)
                  { ui->tableView_part->setModel(tmp_partenaires.afficher());

               }
               else
                       {

                           if (!nom_verif)
                           {
                               //QMessageBox::warning(this,"Erreur lors de l'ajout","E-mail invalid");
                               ui->lineEdit_nom->setStyleSheet("color: blue");
                           }
                           if (!id_verif)
                           {
                               //QMessageBox::warning(this,"Erreur lors de l'ajout","CIN invalid");
                               ui->lineEdit_ID->setStyleSheet("color: blue");
                           }
                           if (!nbC_verif )
                           {
                               //QMessageBox::warning(this,"Erreur lors de l'ajout","Téléphone invalid");
                               ui->lineEdit_nbC->setStyleSheet("color: blue");
                           }


                       }
           }
       }



     else {
         QMessageBox::warning(this,"Erreur lors de l ajout du partenaire","Veuillez remplir tous les champs à nouveau");
     }


}

void delivaro::on_pushButton_ajouterColis_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    int num_colis=ui->lineEdit_numcolis->text().toInt();
     QString adresse=ui->lineEdit_adresseliv->text();
     QString nom_exp=ui->lineEdit_expediteur->text();
     QString nom_dest=ui->lineEdit_destinataire->text();
     QDate date_livraison=ui->dateEdit_livraison->date();
     Colis colis(num_colis,adresse,nom_exp,nom_dest,date_livraison);

    bool test=colis.ajouter();
     if(test)
        { ui->tableView_colis->setModel(tmpcolis.afficher());

     }
     else {
         QMessageBox::warning(this,"Erreur lors de l ajout du colis","Veuillez remplir tous les champs à nouveau");
     }
}

void delivaro::on_pushButton_modifier_Partenaires_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    if (ui->pushButton_modifier_Partenaires->isChecked())
        {

            ui->pushButton_modifier_Partenaires->setText("Modification en cours");
            QSqlTableModel *tableModel= new QSqlTableModel();
            tableModel->setTable("PARTENAIRES");
            tableModel->select();
            ui->tableView_part->setModel(tableModel);
        }
        else
        {
            ui->pushButton_modifier_Partenaires->setText("Modifier");
            ui->tableView_part->setModel(tmp_partenaires.afficher());

        }
}

void delivaro::on_pushButton_modifier_colis_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    if (ui->pushButton_modifier_colis->isChecked())
        {
            ui->pushButton_modifier_colis->setText("Modification en cours");
            QSqlTableModel *tableModel= new QSqlTableModel();
            tableModel->setTable("COLIS");
            tableModel->select();
            ui->tableView_colis->setModel(tableModel);
        }
        else
        {
            ui->pushButton_modifier_colis->setText("Modifier");
            ui->tableView_colis->setModel(tmpcolis.afficher());

        }
}

void delivaro::on_pushButton_supprimer_colis_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    QItemSelectionModel *select = ui->tableView_colis->selectionModel();

        int num_colis =select->selectedRows(0).value(0).data().toInt();

       if(tmpcolis.supprimer(num_colis))
        {
            ui->tableView_colis->setModel(tmpcolis.afficher());
            ui->statusbar->showMessage("colis supprimé");
         }
}

void delivaro::on_pushButton_recherche_colis_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    int NUM_COLIS = ui->lineEdit_recherche_colis->text().toInt();
    QSqlQueryModel * test=tmpcolis.chercher_colis(NUM_COLIS);
    if (ui->pushButton_recherche_colis->isChecked()){
        ui->pushButton_recherche_colis->setText("En cours");

        if(test != nullptr)
    {
        QMessageBox::information(nullptr, QObject::tr("chercher un colis"),
                    QObject::tr("colis trouvé.\n"
                                "Click Cancel to get informations."), QMessageBox::Cancel);
            ui->tableView_colis->setModel(test);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("chercher un colis"),
                    QObject::tr("Erreur , colis introuvable !.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        ui->tableView_colis->setModel(test);
}}
        else
        {
            ui->pushButton_recherche_colis->setText("Recherche");
            ui->tableView_colis->setModel(tmpcolis.afficher());

        }
}

void delivaro::on_pushButton_export_colis_clicked()
{       QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    tmpcolis.exporter(ui->tableView_colis);
    ui->statusbar->showMessage("EXPORT TABLE ",5000);
}

void delivaro::on_pushButton_tabcolis_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(0);
    ui->statusbar->showMessage("0");
}

void delivaro::on_pushButton_tabpartenaires_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->statusbar->showMessage("1");
}

/*void delivaro::on_pushButton_StatistiquesColis_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->statcolis->setBackground(QBrush(gradient));
    QCPBars *test= new QCPBars(ui->statcolis->xAxis, ui->statcolis->yAxis);
    test->setAntialiased(false);
    test->setStackingGap(1);
    test->setName("Nombre de commandes");
    test->setPen(QPen(QColor(0,168,140).lighter(130)));
    test->setBrush(QColor(0,168,140));
    //axes des x
    QVector<double> nb;
    QVector<QString> nom;
    nb <<1<<2 ;
    nom <<"alimentaire"<<"divers";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
       textTicker->addTicks(nb,nom);
       ui->statcolis->xAxis->setTicker(textTicker);
       ui->statcolis->xAxis->setTickLabelRotation(60);
       ui->statcolis->xAxis->setSubTicks(false);
       ui->statcolis->xAxis->setTickLength(0, 4);
       ui->statcolis->xAxis->setRange(0, 8);
       ui->statcolis->xAxis->setBasePen(QPen(Qt::white));
       ui->statcolis->xAxis->setTickPen(QPen(Qt::white));
       ui->statcolis->xAxis->grid()->setVisible(true);
       ui->statcolis->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
       ui->statcolis->xAxis->setTickLabelColor(Qt::white);
       ui->statcolis->xAxis->setLabelColor(Qt::white);
    //axe des y
           ui->statcolis->yAxis->setRange(0,30);
           ui->statcolis->yAxis->setPadding(5); // a bit more space to the left border
           ui->statcolis->yAxis->setBasePen(QPen(Qt::white));
           ui->statcolis->yAxis->setTickPen(QPen(Qt::white));
           ui->statcolis->yAxis->setSubTickPen(QPen(Qt::white));
           ui->statcolis->yAxis->grid()->setSubGridVisible(true);
           ui->statcolis->yAxis->setTickLabelColor(Qt::white);
           ui->statcolis->yAxis->setLabelColor(Qt::white);
           ui->statcolis->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
           ui->statcolis->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
//njibou el data
           QVector<double> testData=tmpcolis.getData();

           test->setData(nb,testData);
           ui->statcolis->legend->setVisible(true);
               ui->statcolis->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
               ui->statcolis->legend->setBrush(QColor(255, 255, 255, 100));
               ui->statcolis->legend->setBorderPen(Qt::NoPen);

               QFont legendFont = font();
               legendFont.setPointSize(10);
               ui->statcolis->legend->setFont(legendFont);
               ui->statcolis->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

}*/

void delivaro::on_pushButton_triColis_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    if(ui->radioButton_numcolis->isChecked())
    {
        ui->tableView_colis->setModel(tmpcolis.trier("NUM_COLIS",ui->AD->currentText()));
    }
    else if(ui->radioButton_adresse->isChecked())
    {
        ui->tableView_colis->setModel(tmpcolis.trier("ADRESSE",ui->AD->currentText()));
    }
    else if(ui->radioButton_destinataire->isChecked())
    {
        ui->tableView_colis->setModel(tmpcolis.trier("ID_PARTENAIRE",ui->AD->currentText()));
    }
    else if(ui->radioButton_expediteur->isChecked())
    {
        ui->tableView_colis->setModel(tmpcolis.trier("ID_CLIENT",ui->AD->currentText()));
    }
    else if(ui->radioButton_dateL->isChecked())
    {
        ui->tableView_colis->setModel(tmpcolis.trier("DATE_LIVRAISON",ui->AD->currentText()));
    }
}

void delivaro::on_lineEdit_recherche_colis_textChanged(const QString &arg1)
{
   ui->tableView_colis->setModel(tmpcolis.recherche_avancee(arg1));
}

void delivaro::on_pushButton_export_partenaires_clicked()
{
        QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

        tmp_partenaires.imprimerpdf(ui->tableView_part);

}

void delivaro::on_pushButton_supp_Partenaires_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    QItemSelectionModel *select = ui->tableView_part->selectionModel();

    QString n =select->selectedRows(0).value(0).data().toString();
       if(tmp_partenaires.supprimer(n))
        {
            ui->tableView_part->setModel(tmp_partenaires.afficher());
            ui->statusbar->showMessage("partenaire supprimé");
         }
}

void delivaro::on_pushButton_Statistiques_partenaires_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->statPartenaires->setBackground(QBrush(gradient));
    QCPBars *test= new QCPBars(ui->statPartenaires->xAxis, ui->statPartenaires->yAxis);
    test->setAntialiased(false);
    test->setStackingGap(1);
    //test->setName("Nombre de commandes");
    test->setPen(QPen(QColor(255,67,27)));
   test->setBrush(QColor(255,67,27));
    //axes des x
    QVector<double> nb;
    QVector<QString> nom;
    nb <<1<<2<<3<<4 ;
    nom <<"alimentaire"<<"esthétique"<<"vestimentaire"<<"divers";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
       textTicker->addTicks(nb,nom);
       ui->statPartenaires->xAxis->setTicker(textTicker);
       ui->statPartenaires->xAxis->setTickLabelRotation(60);
       ui->statPartenaires->xAxis->setSubTicks(false);
       ui->statPartenaires->xAxis->setTickLength(0, 4);
       ui->statPartenaires->xAxis->setRange(0, 8);
       ui->statPartenaires->xAxis->setBasePen(QPen(Qt::white));
       ui->statPartenaires->xAxis->setTickPen(QPen(Qt::white));
       ui->statPartenaires->xAxis->grid()->setVisible(true);
       ui->statPartenaires->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
       ui->statPartenaires->xAxis->setTickLabelColor(Qt::white);
       ui->statPartenaires->xAxis->setLabelColor(Qt::white);
    //axe des y
           ui->statPartenaires->yAxis->setRange(0,30);
           ui->statPartenaires->yAxis->setPadding(5); // a bit more space to the left border
           ui->statPartenaires->yAxis->setBasePen(QPen(Qt::white));
           ui->statPartenaires->yAxis->setTickPen(QPen(Qt::white));
           ui->statPartenaires->yAxis->setSubTickPen(QPen(Qt::white));
           ui->statPartenaires->yAxis->grid()->setSubGridVisible(true);
           ui->statPartenaires->yAxis->setTickLabelColor(Qt::white);
           ui->statPartenaires->yAxis->setLabelColor(Qt::white);
           ui->statPartenaires->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
           ui->statPartenaires->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
//njibou el data
           QVector<double> testData=tmp_partenaires.getData();

           test->setData(nb,testData);
            ui->statPartenaires->legend->setVisible(false);
               ui->statPartenaires->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
               ui->statPartenaires->legend->setBrush(QColor(255, 255, 255, 100));
               ui->statPartenaires->legend->setBorderPen(Qt::NoPen);

               QFont legendFont = font();
               legendFont.setPointSize(20);
               ui->statPartenaires->legend->setFont(legendFont);
               ui->statPartenaires->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void delivaro::on_contract_colis_clicked()
{
    static int turn=1;
        if (turn==1)
        {
            contract_animation->start();
            ui->contract_colis->setText("Expand");
        }
        else
        {
            expand_animation->start();
            ui->contract_colis->setText("Contract");
        }
        turn*=-1;
        QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");
}

void delivaro::on_pushButton_recherche_partenaires_clicked()
{
    QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");

    QString NOM = ui->lineEdit_recherche__partenaires->text();
    QSqlQueryModel *test=tmp_partenaires.chercher_partenaire(NOM);
    if (ui->pushButton_recherche_partenaires->isChecked()){
        ui->pushButton_recherche_partenaires->setText("En cours");

        if(test != nullptr)
    {
        QMessageBox::information(nullptr, QObject::tr("chercher un colis"),
                    QObject::tr("colis trouvé.\n"
                                "Click Cancel to get informations."), QMessageBox::Cancel);
            ui->tableView_colis->setModel(test);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("chercher un partenaire"),
                    QObject::tr("Erreur , partenaire introuvable !.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        ui->tableView_part->setModel(test);
}}
        else
        {
            ui->pushButton_recherche_partenaires->setText("Recherche");
            ui->tableView_part->setModel(tmp_partenaires.afficher());

        }
}



void delivaro::on_deconnexion_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



void delivaro::on_RecherchePub_clicked()
{
              click->play();
            ui->tableViewPub->setModel(ptmp.afficher(  ));
           int id= ui->RecherchePub_2->text().toInt();
           ui->tableViewPub->setModel(ptmp.recherche(id));

}

void delivaro::on_SupprimerPub_clicked()
{
    click->play();
        QItemSelectionModel *select = ui->tableViewPub->selectionModel();

                int id =select->selectedRows(0).value(0).data().toInt();

               if(ptmp.supprimer(id))
                {
                    ui->tableViewPub->setModel(ptmp.afficher());
                    ui->statusbar->showMessage("publicite supprimé");
                 }
}

void delivaro::on_TriRec_clicked()
{
    click->play();
    ui->tableViewRec->setModel(rtmp.tri());
}

void delivaro::on_ModifierRec_clicked()
{
    click->play();
        if (ui->ModifierRec->isChecked())
                {
                   // ui->pushButton_modifier->setDisabled(true);
                    ui->ModifierRec->setText("Modifiable");
                    QSqlTableModel *tableModel= new QSqlTableModel();
                    tableModel->setTable("RECLAMATION");
                    tableModel->select();
                    ui->tableViewRec->setModel(tableModel);
                }
                else
                {
                    ui->ModifierRec->setText("Modifier");
                    ui->tableViewRec->setModel(rtmp.afficher());

                }
}

void delivaro::on_SupprimerRec_clicked()
{
    click->play();
      QItemSelectionModel *select = ui->tableViewRec->selectionModel();

              int id =select->selectedRows(0).value(0).data().toInt();

             if(rtmp.supprimer(id))
              {
                  ui->tableViewRec->setModel(rtmp.afficher());
                  ui->statusbar->showMessage("Reclamation supprimé");
               }
}

void delivaro::on_Exporter_clicked()
{
    click->play();
        QMessageBox msgBox;

        int y=0;
        QPdfWriter pdf("try.pdf");
        QPainter painter;
        if (!painter.begin(&pdf))
        {
            msgBox.setText("Couldn't write in the file.");
            msgBox.exec();
        }

      QSqlQuery query("SELECT * FROM RECLAMATION");


      while (query.next())
      {
          painter.setPen(Qt::red);
          painter.drawText(4500,2000,"Fiche Reclamation");
          painter.setPen(Qt::black);
          painter.drawText(3000,3500+y,"Identifiant de reclamation:");
          painter.drawText(3000,4000+y,"produit id:");
          painter.drawText(3000,4500+y,"description:");
          painter.drawText(3000,5000+y,"Date d'ajout de reclamation:");

          QString id = query.value(0).toString();
          QString produit_id = query.value(1).toString();
          QString description = query.value(2).toString();
          QString date_a = query.value(3).toString();


          painter.drawText(5200,3500+y,id);
          painter.drawText(5200,4000+y,produit_id);
          painter.drawText(5200,4500+y,description);
          painter.drawText(5200,5000+y,date_a);
          y=y+3000;
          query.next();
      }
      msgBox.setText("The document has been saved.");
      msgBox.exec();

}

void delivaro::on_StatistiqueRec_clicked()
{
    click->play();
       statistique stat;
     double dag = stat.traite0();
     double dir = stat.traite1();


       // set dark background gradient:
       QLinearGradient gradient(0, 0, 0, 400);
       gradient.setColorAt(0, QColor(90, 90, 90));
       gradient.setColorAt(0.38, QColor(105, 105, 105));
       gradient.setColorAt(1, QColor(70, 70, 70));
       ui->widget->setBackground(QBrush(gradient));
       // create empty bar chart objects:

       QCPBars *fossil = new QCPBars(ui->widget->xAxis, ui->widget->yAxis);
       fossil->setAntialiased(false);


       fossil->setStackingGap(1);



       // prepare x axis with country labels:
       QVector<double> ticks;
       QVector<QString> labels;
       ticks << 1 << 2 ;
       labels << "les reclamation traite" << "les reclamation non traite" ;
       QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
       textTicker->addTicks(ticks, labels);
       ui->widget->xAxis->setTicker(textTicker);
       ui->widget->xAxis->setTickLabelRotation(60);
       ui->widget->xAxis->setSubTicks(false);
       ui->widget->xAxis->setTickLength(0, 4);
       ui->widget->xAxis->setRange(0, 13);
       ui->widget->xAxis->setBasePen(QPen(Qt::white));
       ui->widget->xAxis->setTickPen(QPen(Qt::white));
       ui->widget->xAxis->grid()->setVisible(true);
       ui->widget->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
       ui->widget->xAxis->setTickLabelColor(Qt::white);
       ui->widget->xAxis->setLabelColor(Qt::white);

       // prepare y axis:
       ui->widget->yAxis->setRange(0, 10);
       ui->widget->yAxis->setPadding(5); // a bit more space to the left border
       ui->widget->yAxis->setLabel("Nombres des reclamations traite");
       ui->widget->yAxis->setBasePen(QPen(Qt::white));
       ui->widget->yAxis->setTickPen(QPen(Qt::white));
       ui->widget->yAxis->setSubTickPen(QPen(Qt::white));
       ui->widget->yAxis->grid()->setSubGridVisible(true);
       ui->widget->yAxis->setTickLabelColor(Qt::white);
       ui->widget->yAxis->setLabelColor(Qt::white);
       ui->widget->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
       ui->widget->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

       // Add data:
       QVector<double> fossilData;

       fossilData  << dag << dir ;
       fossil->setData(ticks, fossilData);

       ui->widget->legend->setVisible(true);
       ui->widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
       ui->widget->legend->setBrush(QColor(255, 255, 255, 100));
       ui->widget->legend->setBorderPen(Qt::NoPen);
       QFont legendFont = font();
       legendFont.setPointSize(10);
       ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
       ui->widget->legend->setFont(legendFont);
}

void delivaro::on_Play_clicked()
{
    click->play();
     son->play();
     son->setLoops(QSound::Infinite);
}

void delivaro::on_Mute_clicked()
{
    click->play();
    son->stop();
}

void delivaro::on_AjouterRec_clicked()
{
    click->play();
        int id=ui->IdRec->text().toInt();
        int produitID=ui->ProduitIdRec->text().toInt();
        QString descrption=ui->DescriptionRec->text();
        QString date = ui->DateRec->text();
        int traite = ui->TraiteRec->text().toInt();
     if(id != 0 && produitID != 0 && descrption !="" && date != "")
      {

        reclamation r(id,produitID,descrption,date,traite);

        qDebug() << "id:" << r.getID();
        qDebug() << "prod id:" << r.getProdID();
        qDebug() << "descrption :" << r.getDescription();
        qDebug() << "Date:" << r.getDate();
        qDebug() << traite;
        if(traite==1 || traite==0)
        {
            bool test=r.ajouter();
               qDebug() << "test :" << test;
               qDebug() << traite;
            if (test)
            {
                ui->tableViewRec->setModel(rtmp.afficher());
                QMessageBox::information(nullptr,QObject::tr("")
                                         ,QObject::tr("Ajout effectué\n"
                                                      "Click Cancel to exit."),QMessageBox::Cancel);
                ui->tableViewRec->setModel(rtmp.afficher());
            }
                  }else
                            {
                            QMessageBox::critical(nullptr,QObject::tr("erreur")
                                                     ,QObject::tr("Ajout non effectué , Il faut remplir tous les champs\n"
                                                                  "Click Cancel to exit."),QMessageBox::Cancel);
                             }

        }
        else
            {
                QMessageBox::critical(nullptr,QObject::tr("erreur")
                                         ,QObject::tr("Ajout non effectué , Il faut remplir tous les champs\n"
                                                      "Click Cancel to exit."),QMessageBox::Cancel);
            }
}

void delivaro::on_AjouterPub_clicked()
{
    click->play();
        int id=ui->IdPub->text().toInt();
        int duree=ui->DureePub->text().toInt();
        int prix=ui->PrixPub->text().toInt();
        QString date = ui->DatePub->text();
       if(id != 0 && duree !=0 && prix !=0 && date != "")
       {

        publicite p(id,duree,prix,date);

        qDebug() << "id:" << p.getID();
        qDebug() << "duree:" << p.getDuree();
        qDebug() << "prix:" << p.getPrix();
        qDebug() << "Date:" << p.getDate();

        bool test=p.ajouter();
           qDebug() << "test :" << test;
        if (test)
        {
            ui->tableViewPub->setModel(ptmp.afficher());
            QMessageBox::information(nullptr,QObject::tr("")
                                     ,QObject::tr("Ajout effectué\n"
                                                  "Click Cancel to exit."),QMessageBox::Cancel);
        }
       }


  else

            {
                QMessageBox::warning(nullptr,QObject::tr("Erreur de saisie")
                                         ,QObject::tr("Erreur de saisie ,Il faut remplir tout les champs\n"
                                                      "Click Cancel to exit."),QMessageBox::Cancel);
            }



}


void delivaro::on_ModifierPub_clicked()
{
    click->play();
        if (ui->ModifierPub->isChecked())
                {
                   // ui->pushButton_modifier->setDisabled(true);
                    ui->ModifierPub->setText("Modifiable");
                    QSqlTableModel *tableModel= new QSqlTableModel();
                    tableModel->setTable("PUBLICITE");
                    tableModel->select();
                    ui->tableViewPub->setModel(tableModel);
                }
                else
                {
                    ui->ModifierPub->setText("Modifier");
                    ui->tableViewPub->setModel(ptmp.afficher());

                }
}

void delivaro::on_StatPub_clicked()
{
    click->play();
      statistique stat;
    double dag = stat.statestique_partie1();
    double dir = stat.statestique_partie2();


      // set dark background gradient:
      QLinearGradient gradient(0, 0, 0, 400);
      gradient.setColorAt(0, QColor(90, 90, 90));
      gradient.setColorAt(0.38, QColor(105, 105, 105));
      gradient.setColorAt(1, QColor(70, 70, 70));
      ui->widget_2->setBackground(QBrush(gradient));
      // create empty bar chart objects:

      QCPBars *fossil = new QCPBars(ui->widget_2->xAxis, ui->widget_2->yAxis);
      fossil->setAntialiased(false);


      fossil->setStackingGap(1);


      // prepare x axis with country labels:
      QVector<double> ticks;
      QVector<QString> labels;
      ticks << 1 << 2 ;
      labels << "prix entre 1 et 50" << "prix entre 50 et 100 " ;
      QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
      textTicker->addTicks(ticks, labels);
      ui->widget_2->xAxis->setTicker(textTicker);
      ui->widget_2->xAxis->setTickLabelRotation(60);
      ui->widget_2->xAxis->setSubTicks(false);
      ui->widget_2->xAxis->setTickLength(0, 4);
      ui->widget_2->xAxis->setRange(0, 13);
      ui->widget_2->xAxis->setBasePen(QPen(Qt::white));
      ui->widget_2->xAxis->setTickPen(QPen(Qt::white));
      ui->widget_2->xAxis->grid()->setVisible(true);
      ui->widget_2->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
      ui->widget_2->xAxis->setTickLabelColor(Qt::white);
      ui->widget_2->xAxis->setLabelColor(Qt::white);

      // prepare y axis:
      ui->widget_2->yAxis->setRange(0, 10);
      ui->widget_2->yAxis->setPadding(5); // a bit more space to the left border
      ui->widget_2->yAxis->setLabel("Les prix du publicite");
      ui->widget_2->yAxis->setBasePen(QPen(Qt::white));
      ui->widget_2->yAxis->setTickPen(QPen(Qt::white));
      ui->widget_2->yAxis->setSubTickPen(QPen(Qt::white));
      ui->widget_2->yAxis->grid()->setSubGridVisible(true);
      ui->widget_2->yAxis->setTickLabelColor(Qt::white);
      ui->widget_2->yAxis->setLabelColor(Qt::white);
      ui->widget_2->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
      ui->widget_2->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

      // Add data:
      QVector<double> fossilData;

      fossilData  << dag << dir ;
      fossil->setData(ticks, fossilData);

      ui->widget_2->legend->setVisible(true);
      ui->widget_2->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
      ui->widget_2->legend->setBrush(QColor(255, 255, 255, 100));
      ui->widget_2->legend->setBorderPen(Qt::NoPen);
      QFont legendFont = font();
      legendFont.setPointSize(10);
      ui->widget_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
      ui->widget_2->legend->setFont(legendFont);
}

void delivaro::on_RechercheRec_clicked()
{
    click->play();
        ui->tableViewRec->setModel(rtmp.afficher(  ));
       int id= ui->RechercheRec_2->text().toInt();
       ui->tableViewRec->setModel(rtmp.recherche(id));

}

void delivaro::on_login_clicked()
{
    Connection C;
    QSqlQuery dbQuery;
    Load_DBV();
    qDebug()<<"VOITURES DB LOADED";
    Load_DBT();


    QString identifiant = ui->lineEdit_login->text();
        QString mdp = ui->lineEdit_password->text();
        QSound::play("C:/Users/legion/Documents/GitHub/Smart_Home_Delivery_2A1/Salma Aidli/InterfaceGraphique/click.wav");
        if (identifiant == "salma" && mdp== "salma1"){

            ui->stackedWidget->setCurrentIndex(1);
            QMessageBox::information(this,"Bienvenue","Mot de passe et identifiant correctes");

        }
        else if (identifiant == "abdou" && mdp== "abdou1"){

            ui->stackedWidget->setCurrentIndex(1);
            QMessageBox::information(this,"Bienvenue","Mot de passe et identifiant correctes");

        }
        else if (identifiant == "yessine" && mdp== "yessine1"){
            //BEGIN TEST
            playsound();

            ui->stackedWidget->setCurrentIndex(1);
            QMessageBox::information(this,"Bienvenue","Mot de passe et identifiant correctes");



        }
        else{

            QMessageBox::information(this,"Warning","Mot de passe ou identifiant incorrectes");
        }
}


void delivaro::on_gestionpartenaires_clicked()
{   ui->stackedWidget->setCurrentIndex(2);

    ui->stackedWidget_2->setCurrentIndex(1);

}

void delivaro::on_gestioncolis_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

        ui->stackedWidget_2->setCurrentIndex(0);

}

void delivaro::on_gestionpubreclamation_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void delivaro::on_retour_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void delivaro::on_retour2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}



void delivaro::on_retourmenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}
void delivaro::on_passwordcheck_toggled(bool checked)
{

    if(checked){
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    }
    else
         ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}
//yyyyyyyyyyy
void delivaro::on_vt_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void delivaro::on_y1_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
}
void delivaro::on_y2_clicked()
{
    //V1.Supprimer(ui);
    QMessageBox msgbox;
    msgbox.addButton(msgbox.Yes);
    msgbox.addButton(msgbox.No);
    QMessageBox msgbox2;
    if (ui->table_modif->currentRow()>=0)
    {
        msgbox.setText("Etes vous sur de vouloir supprimer cette voiture ?");
        if (msgbox.exec()==msgbox.Yes) //gonna run it here
        {
            int i=0;
            while (i<V.size() && V[i].get_matricule()!=ui->table_modif->item(ui->table_modif->currentRow(),0)->text())
            {
                i++;
            }
            if (i<V.size())//tjr verifié
            {
                QSqlQuery qry;
                qry.prepare("Delete from VOITURES where MATRICULE='"+V[i].get_matricule()+"'");
                if (qry.exec())
                {
                }
                V.remove(i);
            }
            ui->table_modif->removeRow(ui->table_modif->currentRow());
        }
    }
    else
    {
        msgbox2.setText("Veuillez selectionner une voiture!");
        msgbox2.exec();
    }
   // ui->table_modif->clearSelection(); not working
}
void delivaro::on_y3_clicked()
{
    //V1.Statistiques();
    /*QBarSet *set0 = new QBarSet("Jane");
    QBarSet *set1 = new QBarSet("John");
    QBarSet *set2 = new QBarSet("Axel");
    QBarSet *set3 = new QBarSet("Mary");
    QBarSet *set4 = new QBarSet("Samantha");*/
    int j;
    int k;
    int x;
    //TESting
    QBarSeries *series = new QBarSeries();
    for(int i=0;i<V.size();i++)
    {
        j=0;
        while (j<i && V[j].get_type()!=V[i].get_type())
            j++;
        if (j==i)//New Type
        {
            QBarSet *set_i=new QBarSet(V[i].get_type());
            x=0;
            for (k=i;k<V.size();k++)//Occurences
                if(V[k].get_type()==V[i].get_type())
                    x++;
            *set_i<<x;
            series->append(set_i);


        }

    }

    /**set0 << 1 << 2 << 3 << 4 << 5 << 6;
        *set1 << 5 << 0 << 0 << 4 << 0 << 7;
        *set2 << 3 << 5 << 8 << 13 << 8 << 5;
        *set3 << 5 << 6 << 7 << 3 << 4 << 5;
        *set4 << 9 << 7 << 5 << 3 << 1 << 2;*/

    /*QBarSeries *series = new QBarSeries();
        series->append(set0);
        series->append(set1);
        series->append(set2);
        series->append(set3);
        series->append(set4);*/

     QChart *chart = new QChart();
           chart->addSeries(series);
           chart->setTitle("Statistiques");
           chart->setAnimationOptions(QChart::SeriesAnimations);

     QStringList categories;
             categories << "Type Voitures"; //<< "Feb" << "Mar" << "Apr" << "May" << "Jun";
     QBarCategoryAxis *axis = new QBarCategoryAxis();
              axis->append(categories);
              chart->createDefaultAxes();
              chart->setAxisX(axis, series);


              chart->legend()->setVisible(true);
              chart->legend()->setAlignment(Qt::AlignBottom);

      QChartView *chartView = new QChartView(chart);
                 chartView->setRenderHint(QPainter::Antialiasing);
                 chartView->resize(620,325);
                 chartView->show();
}
void delivaro::on_y5_clicked()
{
    //V1.Dispo(ui);
    {
        ui->stackedWidget_2->setCurrentIndex(1);
        /////////////////////////////////////////////
        ui->tabledispo->setRowCount(0);
        ui->tabledispo->setSortingEnabled(false);
        int d;
        for (int i=0;i<V.size();i++)
        if (V[i].get_statut()=="Disponible")
        {
            ui->tabledispo->insertRow(ui->tabledispo->rowCount());
            ui->tabledispo->setItem(ui->tabledispo->rowCount()-1,0,new QTableWidgetItem(V[i].get_matricule()));
            ui->tabledispo->setItem(ui->tabledispo->rowCount()-1,1,new QTableWidgetItem(V[i].get_type()));
            ui->tabledispo->setItem(ui->tabledispo->rowCount()-1,2,new QTableWidgetItem);
            ui->tabledispo->item(ui->tabledispo->rowCount()-1,2)->setData(Qt::DisplayRole,V[i].get_capacite());//int sorting
            ui->tabledispo->setItem(ui->tabledispo->rowCount()-1,3,new QTableWidgetItem);
            ui->tabledispo->item(ui->tabledispo->rowCount()-1,3)->setData(Qt::DisplayRole,V[i].get_kilometrage());//int sorting

            for (d=0;d<=3;d++)
                ui->tabledispo->item(ui->tabledispo->rowCount()-1,d)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        }
        ui->tabledispo->setSortingEnabled(true);
    }
    ui->stackedWidget_4->setCurrentIndex(1);

}
void delivaro::on_y6_clicked()
{
    //T1.Supprimer(ui);
    QMessageBox msgbox;
    msgbox.addButton(msgbox.Yes);
    msgbox.addButton(msgbox.No);
    QMessageBox msgbox2;

    if (ui->table_trajet->currentRow()>=0)
    {
        msgbox.setText("Etes vous sur de vouloir supprimer ce trajet ?");
        if (msgbox.exec()==msgbox.Yes) //gonna run it here
        {
            int i=0;
            while (i<T.size() && T[i].get_numero()!=ui->table_trajet->item(ui->table_trajet->currentRow(),0)->text())
            {
                i++;
            }
            if (i<T.size())//tjr verifié
            {
                QSqlQuery qry;
                qry.prepare("Delete from TRAJETS where NUMERO='"+T[i].get_numero()+"'");
                if (qry.exec())
                {
                }
                T.remove(i);
            }

            ui->table_trajet->removeRow(ui->table_trajet->currentRow());
        }
    }
    else
    {
        msgbox2.setText("Veuillez selectionner un trajet !");
        msgbox2.exec();
    }
}
//SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
void delivaro::on_pb3_clicked()
{
    playsound();
    ui->stackedWidget_3->setCurrentIndex(0);
}


void delivaro::on_pb2_clicked()
{
    playsound();
    //V1.Ajouter(ui);
    Carac_voiture e;
    e.set_matricule(ui->matricule->text());
    e.set_type(ui->comboBox->currentText());
    e.set_capacite(ui->spinBox->value());
    e.set_kilometrage(ui->spinBox_2->value());
    e.set_statut(ui->comboBox_2->currentText());

    QMessageBox msgbox;
    int i=0;
    while (i<V.size() && V[i].get_matricule()!=e.get_matricule())
    {
        i++;
    }
    if (i==V.size())
    {

        ui->table_modif->setSortingEnabled(false);
        msgbox.setText("Voiture Ajoutée");
        V.push_back(e);
        ui->table_modif->insertRow(ui->table_modif->rowCount());
        ui->table_modif->setItem(ui->table_modif->rowCount()-1,0,new QTableWidgetItem(e.get_matricule()));
        ui->table_modif->item(ui->table_modif->rowCount()-1,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->table_modif->setItem(ui->table_modif->rowCount()-1,1,new QTableWidgetItem(e.get_type()));
        ui->table_modif->item(ui->table_modif->rowCount()-1,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->table_modif->setItem(ui->table_modif->rowCount()-1,2,new QTableWidgetItem); // INT SORTING
        ui->table_modif->item(ui->table_modif->rowCount()-1,2)->setData(Qt::DisplayRole,e.get_capacite());
        //ui->table_modif->setItem(ui->table_modif->rowCount()-1,3,new QTableWidgetItem(QString::number(e.get_kilometrage()))); //Sorting Problem
        ui->table_modif->setItem(ui->table_modif->rowCount()-1,3,new QTableWidgetItem);
        ui->table_modif->item(ui->table_modif->rowCount()-1,3)->setData(Qt::DisplayRole,e.get_kilometrage());//int sorting


        ui->table_modif->setItem(ui->table_modif->rowCount()-1,4,new QTableWidgetItem(e.get_statut()));
        ui->table_modif->setSortingEnabled(true);
//test DB

        QSqlQuery qry;
        qry.prepare("insert into VOITURES (MATRICULE,TYPE,CAPACITE,KILOMETRAGE,STATUT) values('"+e.get_matricule()+"','"+e.get_type()+"','"+ QString::number(e.get_capacite())+"','"+QString::number(e.get_kilometrage())+"','"+e.get_statut()+"')");
        //can't insert INT :(
        if (qry.exec())
        {
        }

//test DB
        msgbox.exec();
        ui->stackedWidget_3->setCurrentIndex(0);
    }
    else
    {
        msgbox.setText("Matricule Existe Deja !");
        msgbox.exec();
    }
}


void delivaro::on_matricule_textChanged(const QString &arg1)
{
    if (arg1.size()>=3)
        ui->pb2->setEnabled(true);
    else
        ui->pb2->setEnabled(false);
}


void delivaro::on_ln1_textChanged(const QString &arg1)
{
    QStringList wordlist;

    for (int i=0;i<V.size();i++)
        wordlist.push_back(V[i].get_matricule());
    QCompleter *completer = new QCompleter(wordlist,this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->ln1->setCompleter(completer);


}


/*void delivaro::on_actionSupprimer_triggered()
{
    if (ui->tabWidget_2->currentIndex()==0)//TAB VOITURES
    {
        V1.Supprimer(ui);
    }
    else if (ui->tabWidget_2->currentIndex()==1)//TAB TRAJETS
    {
        T1.Supprimer(ui);
    }
}*/


/*void delivaro::on_actionNouveau_triggered()
{
    if(ui->tabWidget_2->currentIndex()==0)
        ui->stackedWidget->setCurrentIndex(1);
    else if (ui->tabWidget_2->currentIndex()==1)
    V1.Dispo(ui);
}*/


void delivaro::on_ln1_editingFinished()
{
    //V1.Rechercher(ui);
    int i=0;
       while(i<V.size() && ui->ln1->text()!=ui->table_modif->item(i,0)->text())
           i++;
    if(i<V.size())
        ui->table_modif->selectRow(i);
}


void delivaro::on_pb6_clicked()
{
    playsound();
    ui->stackedWidget_4->setCurrentIndex(0);
}


void delivaro::on_stackedWidget_3_currentChanged(int arg1)
{
    if (arg1 == 0  )
    {
        ui->tabWidget_2->setTabEnabled(1,true);
        //ui->toolBar->show();
    }
    else
    {
        //ui->toolBar->hide();
        ui->tabWidget_2->setTabEnabled(1,false);
    }
    if (arg1==1)
        ui->matricule->setText("");
}


void delivaro::on_dateEdit_userDateChanged(const QDate &date)
{
    ui->dateEdit_2->setMinimumDate(date);
}


void delivaro::on_pb5_clicked()
{
    playsound();
    //T1.Ajouter(ui,V1);
    carac_trajet e;
    e.set_numero(ui->lineEdit_2->text());
    e.set_pays_chargement(ui->lineEdit_3->text());
    e.set_ville_chargement(ui->lineEdit_4->text());
    e.set_cdp_chargement(ui->lineEdit_5->text());
    e.set_date_chargement(QString::number(ui->dateEdit->date().day()) + "/" + QString::number(ui->dateEdit->date().month()) +
                          "/" + QString::number(ui->dateEdit->date().year()));

    e.set_pays_livaison(ui->lineEdit_7->text());
    e.set_ville_livraison(ui->lineEdit_8->text());
    e.set_cdp_livraison(ui->lineEdit_9->text());
    e.set_date_livraison(QString::number(ui->dateEdit_2->date().day()) + "/" + QString::number(ui->dateEdit_2->date().month()) +
                          "/" + QString::number(ui->dateEdit_2->date().year()));

    QMessageBox msgbox;
    int i=0;
    while (i<T.size() && T[i].get_numero()!=e.get_numero())
    {
        i++;
    }
    if (i==T.size())
    {
        ui->table_trajet->setSortingEnabled(false);

        msgbox.setText("Trajet Ajoutée");
        T.push_back(e);

        ui->table_trajet->insertRow(ui->table_trajet->rowCount());
        ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,0,new QTableWidgetItem(e.get_numero()));
        ui->table_trajet->item(ui->table_trajet->rowCount()-1,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,1,new QTableWidgetItem(e.get_pays_chargement()));
        ui->table_trajet->item(ui->table_trajet->rowCount()-1,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,2,new QTableWidgetItem(e.get_pays_livaison()));
        ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,3,new QTableWidgetItem(e.get_ville_chargement()));
        ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,4,new QTableWidgetItem(e.get_ville_livraison()));
        ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,5,new QTableWidgetItem(e.get_cdp_chargement()));
        ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,6,new QTableWidgetItem(e.get_cdp_livraison()));
        ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,7,new QTableWidgetItem(e.get_date_chargement()));
        ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,8,new QTableWidgetItem(e.get_ddate_livraison() ));
        ui->table_trajet->setSortingEnabled(true);


        QSqlQuery qry;
        qry.prepare("insert into TRAJETS (NUMERO,PAYS_CHARGEMENT,PAYS_LIVRAISON,VILLE_CHARGEMENT,VILLE_LIVRAISON,CDP_CHARGEMENT,CDP_LIVRAISON,DATE_CHARGEMENT,DATE_LIVRAISON) values('"+e.get_numero()+"','"+e.get_pays_chargement()+"','"+ e.get_pays_livaison()+"','"+e.get_ville_chargement()+"','"+e.get_ville_livraison()+"','"+e.get_cdp_chargement()+"','"+e.get_cdp_livraison()+"','"+e.get_date_chargement()+"','"+e.get_ddate_livraison()+"')");
        //can't insert INT :(
        if (qry.exec())
        {
        }
        ///////// Debut test Affectation trajet ->voitures
        ///


        //V1.Affecter(ui,e);
        //
        foreach(QModelIndex index, ui->tabledispo->selectionModel()->selectedRows())
            {
                for (int i=0;i<V.size();i++)
                    if(V[i].get_matricule()==ui->tabledispo->item(index.row(),0)->text())//Check V with SELECETED MATRICULES
                    {
                        V[i].get_num_traj().push_back(e.get_numero());//V[i].T.push_brack(NUM) //TRAJ ajouté à V[i]
                        V[i].set_statut("Indisponible");
                        for(int j=0;j<ui->table_modif->rowCount();j++)
                            if(ui->table_modif->item(j,0)->text()==V[i].get_matricule())
                            {
                                ui->table_modif->item(j,4)->setText("Indisponible");
                                // UPDATE DB
                                QSqlQuery qry;
                                qry.prepare("update VOITURES set STATUT='""Indisponible""' where MATRICULE='"+V[i].get_matricule()+"' ");
                                //can't insert INT :(
                                if (qry.exec())
                                {
                                }
                            }
                        //msgbox.setText(msgbox.text() + QString::number(V[i].get_num_traj().size())); nbr d'affectation
                    }
            }
        ///////// Fin test Affecation

        msgbox.setText("Trajet Ajouté");
        msgbox.exec();
        ui->stackedWidget_4->setCurrentIndex(0);
    }
    else
    {
        msgbox.setText("Numero deja existant");
        msgbox.exec();
    }
}


void delivaro::on_lineEdit_6_textChanged(const QString &arg1)
{
    QStringList wordlist;

    for (int i=0;i<V.size();i++)
        if(V[i].get_statut()=="Disponible")
        wordlist.push_back(V[i].get_matricule());
    QCompleter *completer = new QCompleter(wordlist,this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_6->setCompleter(completer);
}


/*void delivaro::on_actionaffecter_triggered()
{
    V1.Trajets_Associes(ui,T);
}*/


void delivaro::on_pb4_clicked()
{
    playsound();
    ui->stackedWidget_3->setCurrentIndex(0);
}


void delivaro::on_table_modif_cellChanged(int row, int column)
{
    //V1.Modifier(ui,row,column);
    int i=0;
    while(i<V.size()&& V[i].get_matricule()!=ui->table_modif->item(row,0)->text())
        i++;
    if(i<V.size())
    {
        switch (column)
        {
        case 2:
        {
            V[i].set_capacite(ui->table_modif->item(row,2)->text().toInt());
            QSqlQuery qry;
            qry.prepare("update VOITURES set CAPACITE='"+ui->table_modif->item(row,2)->text()+"' where MATRICULE='"+V[i].get_matricule()+"' ");
            //can't insert INT :(
            if (qry.exec())
            {
            }
        }break;
        case 3:
        {

            V[i].set_kilometrage( ui->table_modif->item(row,3)->text().toInt());
            QSqlQuery qry;
            qry.prepare("update VOITURES set KILOMETRAGE='"+ui->table_modif->item(row,3)->text()+"' where MATRICULE='"+V[i].get_matricule()+"' ");
            //can't insert INT :(
            if (qry.exec())
            {
            }

        }break;
        case 4:
        {
            if ((ui->table_modif->item(row,4)->text()!="Disponible") && (ui->table_modif->item(row,4)->text()!="Indisponible"))
            ui->table_modif->item(row,4)->setText(V[i].get_statut());
            else
            V[i].set_statut(ui->table_modif->item(row,4)->text());
        }break;
        }
    }
}


/*void delivaro::on_actionEnrigistrer_triggered()
{
    V1.Exporter(ui);
}*/


void delivaro::on_stackedWidget_2_currentChanged(int arg1)
{
    if (arg1 == 0  )
    {
        ui->tabWidget_2->setTabEnabled(0,true);
        //ui->toolBar->show();
    }
    else
    {
        ui->tabWidget_2->setTabEnabled(0,false);
        //ui->toolBar->hide();
    }
}


/*void delivaro::on_tabWidget_2_currentChanged(int index)
{
    if (index==1)
    {
        ui->actionNouveau->setText("Nouveau Trajet");
        ui->actionSupprimer->setText("Supprimer");
        ui->actionaffecter->setVisible(false);
        ui->actionStatistiques->setVisible(false);
        ui->table_trajet->setSortingEnabled(true);
    }
    else if(index==0)
    {
        ui->actionNouveau->setText("Nouvelle Voiture");
        ui->actionSupprimer->setText("Supprimer");
        ui->actionaffecter->setVisible(true);
        ui->actionStatistiques->setVisible(true);
        ui->table_modif->setSortingEnabled(true);
    }
    ui->toolBar->show();
}*/


void delivaro::on_lineEdit_10_textChanged(const QString &arg1)
{
    QStringList wordlist;

    for (int i=0;i<T.size();i++)
        wordlist.push_back(T[i].get_numero());
    QCompleter *completer = new QCompleter(wordlist,this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_10->setCompleter(completer);
}





/*void delivaro::on_actionStatistiques_triggered()
{
    V1.Statistiques();
}*/

void delivaro::playsound()
{
    QMediaPlayer *music = new QMediaPlayer();
    music->setMedia(QUrl("C:/Users/Yessine/Desktop/Projet/testinggg/Sounds/sound.mp3"));
    music->setVolume(80);
    qDebug()<<"played";
    music->play();
}
void delivaro::on_pb90_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void delivaro::on_pb91_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void delivaro::on_pb92_clicked()
{
    QString filters("CSV files (*.csv);;All files (*.*)");
    QString defaultFilter("CSV files (*.csv)");
    QString fileName = QFileDialog::getSaveFileName(0, "Save file", QCoreApplication::applicationDirPath(),
                                                    filters, &defaultFilter);
    //V1.Exporter(ui);
    QString textData;
    int rows = ui->table_modif->rowCount();
    int columns = ui->table_modif->columnCount();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

                textData += ui->table_modif->item(i,j)->text();
                if (j<columns-1)
                textData += ";" ;
                // for .csv file format
        }
        textData += "\n";             // (optional: for new line segmentation)
    }

    // [Save to file] (header file <QFile> needed)
    // .csv
    QFile csvFile(fileName);
    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {

        QTextStream out(&csvFile);

        out << textData;

        //csvFile.open(QIODevice::ReadOnly);
        csvFile.close();
        QMessageBox msgbox;
        msgbox.setText("Fichier Exporté Avec Succès");
        msgbox.exec();
    }
}
void delivaro::Load_DBV()
{
    Connection C;
    QSqlQuery dbQuery;

//LOAD VOITURES DB
    if(dbQuery.exec("select * from VOITURES"))
        while (dbQuery.next())
        {
            Carac_voiture t;

            t.set_matricule(dbQuery.value(0).toString());
            t.set_type(dbQuery.value(1).toString());
            t.set_capacite(dbQuery.value(2).toInt());
            t.set_kilometrage(dbQuery.value(3).toInt());
            t.set_statut(dbQuery.value(4).toString());

            V.push_back(t);
            //ui->table_modif->setSortingEnabled(false);
            ui->table_modif->insertRow(ui->table_modif->rowCount());
            ui->table_modif->setItem(ui->table_modif->rowCount()-1,0,new QTableWidgetItem(t.get_matricule()));
            ui->table_modif->item(ui->table_modif->rowCount()-1,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->table_modif->setItem(ui->table_modif->rowCount()-1,1,new QTableWidgetItem(t.get_type()));
            ui->table_modif->item(ui->table_modif->rowCount()-1,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->table_modif->setItem(ui->table_modif->rowCount()-1,2,new QTableWidgetItem);
            ui->table_modif->item(ui->table_modif->rowCount()-1,2)->setData(Qt::DisplayRole,t.get_capacite());//int sorting
            ui->table_modif->setItem(ui->table_modif->rowCount()-1,3,new QTableWidgetItem);
            ui->table_modif->item(ui->table_modif->rowCount()-1,3)->setData(Qt::DisplayRole,t.get_kilometrage());//int sorting
            ui->table_modif->setItem(ui->table_modif->rowCount()-1,4,new QTableWidgetItem(t.get_statut()));

            qDebug()<<t.get_matricule();
        }
    ui->table_modif->setSortingEnabled(true);
}
void delivaro::Load_DBT()
{
    qDebug()<<"looks good\n";
    Connection C;
    QSqlQuery dbQuery;

    if(dbQuery.exec("select * from TRAJETS"))
        while (dbQuery.next())
        {
            carac_trajet f;
            f.set_numero(dbQuery.value(0).toString());
            f.set_pays_chargement(dbQuery.value(1).toString());
            f.set_pays_livaison(dbQuery.value(2).toString());
            f.set_ville_chargement(dbQuery.value(3).toString());
            f.set_ville_livraison(dbQuery.value(4).toString());
            f.set_cdp_chargement(dbQuery.value(5).toString());
            f.set_cdp_livraison(dbQuery.value(6).toString());
            f.set_date_chargement(dbQuery.value(7).toString());
            f.set_date_livraison(dbQuery.value(8).toString());

            T.push_back(f);

            ui->table_trajet->insertRow(ui->table_trajet->rowCount());
            ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,0,new QTableWidgetItem(f.get_numero()));
            ui->table_trajet->item(ui->table_trajet->rowCount()-1,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,1,new QTableWidgetItem(f.get_pays_chargement()));
            ui->table_trajet->item(ui->table_trajet->rowCount()-1,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,2,new QTableWidgetItem(f.get_pays_livaison()));
            ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,3,new QTableWidgetItem(f.get_ville_chargement()));
            ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,4,new QTableWidgetItem(f.get_ville_livraison()));
            ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,5,new QTableWidgetItem(f.get_cdp_chargement()));
            ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,6,new QTableWidgetItem(f.get_cdp_livraison()));
            ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,7,new QTableWidgetItem(f.get_date_chargement()));
            ui->table_trajet->setItem(ui->table_trajet->rowCount()-1,8,new QTableWidgetItem(f.get_ddate_livraison() ));

            qDebug()<<f.get_numero();
        }
    ui->table_trajet->setSortingEnabled(true);
}

void delivaro::on_pushButton355_clicked()
{
    if (ui->table_modif->currentRow()>=0)
     ui->table_trajeff->setRowCount(0);
    int i=0;
    while(i<V.size() && V[i].get_matricule()!=ui->table_modif->item(ui->table_modif->currentRow(),0)->text()) //Matricule
        i++;
    if (i<V.size())
    {
        int k=0;
        int d;

        ui->table_trajeff->setSortingEnabled(false);

        for(int j=0; j<V[i].get_num_traj().size(); j++) //Matricule.NUM
        {
            while (k<T.size() && T[k].get_numero()!=V[i].get_num_traj()[j]) //faute possible -> getter(numero)
                k++;
            if (k<T.size())
            {
                ui->table_trajeff->insertRow(ui->table_trajeff->rowCount());
                ui->table_trajeff->setItem(ui->table_trajeff->rowCount()-1,0,new QTableWidgetItem(T[k].get_numero()));
                ui->table_trajeff->setItem(ui->table_trajeff->rowCount()-1,1,new QTableWidgetItem(T[k].get_pays_chargement()));
                ui->table_trajeff->setItem(ui->table_trajeff->rowCount()-1,2,new QTableWidgetItem(T[k].get_pays_livaison()));
                ui->table_trajeff->setItem(ui->table_trajeff->rowCount()-1,3,new QTableWidgetItem(T[k].get_ville_chargement()));
                ui->table_trajeff->setItem(ui->table_trajeff->rowCount()-1,4,new QTableWidgetItem(T[k].get_ville_livraison()));
                ui->table_trajeff->setItem(ui->table_trajeff->rowCount()-1,5,new QTableWidgetItem(T[k].get_cdp_chargement()));
                ui->table_trajeff->setItem(ui->table_trajeff->rowCount()-1,6,new QTableWidgetItem(T[k].get_cdp_livraison()));
                ui->table_trajeff->setItem(ui->table_trajeff->rowCount()-1,7,new QTableWidgetItem(T[k].get_date_chargement()));
                ui->table_trajeff->setItem(ui->table_trajeff->rowCount()-1,8,new QTableWidgetItem(T[k].get_ddate_livraison() ));
                for (d=0;d<=8;d++)
                    ui->table_trajeff->item(ui->table_trajeff->rowCount()-1,d)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            }
        }
        ui->table_trajeff->setSortingEnabled(true);
        ui->stackedWidget_3->setCurrentIndex(2);
    }
}
