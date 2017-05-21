#include "fenetre.h"

FenetrePrincipale::FenetrePrincipale() : QMainWindow()
{
    manager = NotesManager::getInstance();

    setWindowTitle("Notepod-- v0.0");
    setMinimumHeight(600);
    setMinimumWidth(800);

    zoneCentrale = new QMdiArea;
    setCentralWidget(zoneCentrale);

    menuFichier = menuBar()->addMenu("&Fichier");
    QAction *actionNouveau = new QAction("&Nouvelle note", this);
    menuFichier->addAction(actionNouveau);
    connect(actionNouveau, SIGNAL(triggered()), this, SLOT(sousFenetreAjoutNote()));
    QAction *actionQuitter = new QAction("&Quitter", this);
    menuFichier->addAction(actionQuitter);
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    menuEdition = menuBar()->addMenu("&Edition");
    QAction *actionSauver = new QAction("&Sauver", this);
    menuEdition->addAction(actionSauver);
    connect(actionSauver, SIGNAL(triggered()), this, SLOT(ouvrirDialogue()));
    menuAffichage = menuBar()->addMenu("&Affichage");

    creerDock();


    /*QStatusBar *m_statusBar = new StatusBar(this);
    m_statusBar->showMessage("Trololo");
    setStatusBar(m_statusBar);*/
}


void FenetrePrincipale::ouvrirDialogue()
{
    QMessageBox ah;

    ah.setWindowTitle("Ah!");
    ah.setText("Ça veut dire que...?");
    ah.setStandardButtons(QMessageBox::Ok);
    ah.setDefaultButton(QMessageBox::Ok);
    QPixmap icon("C:/Users/SilverEye/notepod/denis.brogniart.ah.png");
    ah.setIconPixmap(icon);
    ah.show();
    ah.exec();
}

void FenetrePrincipale::creerDock()
{
    dock = new QDockWidget(tr("Notes actives"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    dockLayout = new QVBoxLayout;

    listeNotes = new QListWidget(dock);
    for(Note i : manager.getNotes())
    {
        listeNotes->addItem(QString::fromStdString(i.getTitle()));
    }
    dockLayout->addWidget(listeNotes);

    boutonAjouter = new QPushButton("Nouvelle note", this);
    connect(boutonAjouter, SIGNAL(clicked()),this, SLOT(sousFenetreAjoutNote()));
    dockLayout->addWidget(boutonAjouter);

    widget = new QWidget(this);
    widget->setLayout(dockLayout);

    dock->setWidget(widget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    menuAffichage->addAction(dock->toggleViewAction());
}

void FenetrePrincipale::sousFenetreAjoutNote()
{
    QWidget* fenetre = new QWidget(this);
    QVBoxLayout* layoutSousFenetre = new QVBoxLayout;

    id = new QLineEdit;
    titre = new QLineEdit;
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Identificateur", id);
    formLayout->addRow("Titre", titre);
    layoutSousFenetre->addLayout(formLayout);

    QPushButton* boutonValiderNote = new QPushButton("Ajouter");
    connect(boutonValiderNote, SIGNAL(clicked()), this, SLOT(insererNote()));
    layoutSousFenetre->addWidget(boutonValiderNote);

    fenetre->setLayout(layoutSousFenetre);
    QMdiSubWindow* sousFenetre1 = zoneCentrale->addSubWindow(fenetre);
    sousFenetre1->setWindowTitle("Nouvelle note");
    sousFenetre1->setWindowIcon(QIcon("C:/Users/SilverEye/notepod/edit-set-5-256.png"));
    sousFenetre1->show();
}

void FenetrePrincipale::insererNote()
{
    manager.addNote(new Note(id->text().toStdString(),titre->text().toStdString()));
    listeNotes->addItem(titre->text());
    foreach(QLineEdit *widget, this->findChildren<QLineEdit*>())
    {
        widget->clear();
    }
    //ajouter une erreur au cas où l'identificateur existerait déjà
}

