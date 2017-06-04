#include "fenetre.h"

/*
 * Constructeur de la fenêtre principale
 */
FenetrePrincipale::FenetrePrincipale() : QMainWindow()
{
    // Initialisation du NotesManager
    manager = NotesManager::getInstance();

    // Paramétrage de la fenêtre
    setWindowTitle("Notepod-- v0.0");
    setMinimumHeight(600);
    setMinimumWidth(800);
    zoneCentrale = new QMdiArea;
    setCentralWidget(zoneCentrale);

    // Barre d'outils
    menuFichier = menuBar()->addMenu("&Fichier");
    QAction *actionNouveau = new QAction("&Nouvelle note", this);
    menuFichier->addAction(actionNouveau);
    connect(actionNouveau, SIGNAL(triggered()), this, SLOT(creerSFNote()));
    QAction *actionQuitter = new QAction("&Quitter", this);
    menuFichier->addAction(actionQuitter);
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    menuEdition = menuBar()->addMenu("&Edition");
    QAction *actionSauver = new QAction("&Sauver", this);
    menuEdition->addAction(actionSauver);
    connect(actionSauver, SIGNAL(triggered()), this, SLOT(ouvrirDenis()));
    menuAffichage = menuBar()->addMenu("&Affichage");


    creerDock();


    /*QStatusBar *m_statusBar = new StatusBar(this);
    m_statusBar->showMessage("Trololo");
    setStatusBar(m_statusBar);*/
}

/*
 * Docks (panneaux latéraux pour afficher les notes et les relations)
 */
void FenetrePrincipale::creerDock()
{
    // Création des docks
    dock = new QDockWidget(tr("Notes actives"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    dock2 = new QDockWidget(tr("Relations"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    dockLayout = new QVBoxLayout;

    // Ajout des notes
    listeNotes = new QListWidget(dock);
    for(Note i : manager.getNotes())
    {
        listeNotes->addItem(QString::fromStdString(i.getId()));
    }
    connect(listeNotes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(visualiserNote(QListWidgetItem*)));
    dockLayout->addWidget(listeNotes);

    // Bouton nouvelle note
    boutonAjouter = new QPushButton("Nouvelle note", this);
    connect(boutonAjouter, SIGNAL(clicked()),this, SLOT(creerSFNote()));
    dockLayout->addWidget(boutonAjouter);


    widget = new QWidget(this);
    widget->setLayout(dockLayout);

    // "Accrocher" les docks
    dock->setWidget(widget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    addDockWidget(Qt::RightDockWidgetArea, dock2);

    menuAffichage->addAction(dock2->toggleViewAction());
}

/*
 * Implémentation des slots de FenetrePrincipale
 */
void FenetrePrincipale::creerSFNote()
{
    sousFenetre = new QMdiSubWindow;
    ContenuFenetreAjoutNote* contenuAjout = new ContenuFenetreAjoutNote;
    sousFenetre = zoneCentrale->addSubWindow(contenuAjout);
    sousFenetre->show();
    connect(contenuAjout, SIGNAL(demandeRedimensionnement()), this, SLOT(redimensionnerFenetre()));
    connect(contenuAjout, SIGNAL(nouvelleNote(QString,QString)),this, SLOT(receptionNote(QString,QString)));
    connect(contenuAjout->getBoutonValiderNote(), SIGNAL(clicked()), this, SLOT(fermerSousFenetre()));
}

void FenetrePrincipale::receptionNote(QString id, QString titre)
{
    manager.addNote(new Note(id.toStdString(),titre.toStdString()));
    listeNotes->addItem(id);
}

void FenetrePrincipale::redimensionnerFenetre()
{
    sousFenetre->resize(500,400);
    sousFenetre->updateGeometry();
}

/*
 * Voir une note
 */
void FenetrePrincipale::visualiserNote(QListWidgetItem* i)
{
    sousFenetre = new QMdiSubWindow;
    ContenuFenetreVisualisationNote* contenuVisualisation = new ContenuFenetreVisualisationNote(i);
    sousFenetre = zoneCentrale->addSubWindow(contenuVisualisation);
    sousFenetre->show();
}








