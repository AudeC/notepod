#include "fenetre.h"

/*
 * Constructeur de la fenêtre principale
 */
FenetrePrincipale::FenetrePrincipale() : QMainWindow()
{
    // Initialisation du NotesManager
    manager = NotesManager::getInstance();

    // Paramètrage de la fenêtre
    setWindowTitle("Notepod-- v0.0");
    setMinimumHeight(600);
    setMinimumWidth(800);
    zoneCentrale = new QMdiArea;
    setCentralWidget(zoneCentrale);

    // Barre d'outils
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

/*
 * Easter Egg - Denis Brogniart
 */
void FenetrePrincipale::ouvrirDialogue()
{
    QMessageBox ah;

    ah.setWindowTitle("Ah!");
    ah.setText("Ça veut dire que cette fonctionalité n'est pas encore implémentée...?");
    ah.setStandardButtons(QMessageBox::Ok);
    ah.setDefaultButton(QMessageBox::Ok);
    QPixmap icon("C:/Users/SilverEye/notepod/denis.brogniart.ah.png");
    ah.setIconPixmap(icon);
    ah.setWindowIcon(QIcon("C:/Users/SilverEye/notepod/1495391974_cancel_16.png"));
    ah.show();
    ah.exec();
}

/*
 * Dock (panneau latéral pour afficher les notes et les relations)
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
    connect(listeNotes, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(visualiserNote(QListWidgetItem*)));
    dockLayout->addWidget(listeNotes);

    // Bouton
    boutonAjouter = new QPushButton("Nouvelle note", this);
    connect(boutonAjouter, SIGNAL(clicked()),this, SLOT(sousFenetreAjoutNote()));
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
 * Ajouter une note
 */
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

    boutonValiderNote = new QPushButton("Ajouter");
    connect(boutonValiderNote, SIGNAL(clicked()), this, SLOT(insererNote()));
    boutonValiderNote->setShortcut(QKeySequence(Qt::Key_Return));
    layoutSousFenetre->addWidget(boutonValiderNote);
    boutonValiderNote->setDisabled(true);
    connect(id, SIGNAL(textChanged(QString)), this, SLOT(unhide()));
    connect(titre, SIGNAL(textChanged(QString)), this, SLOT(unhide()));

    fenetre->setLayout(layoutSousFenetre);
    sousFenetre1 = zoneCentrale->addSubWindow(fenetre);
    sousFenetre1->setWindowTitle("Nouvelle note");
    sousFenetre1->setWindowIcon(QIcon("C:/Users/SilverEye/notepod/edit-set-5-256.png"));
    sousFenetre1->show();
    connect(boutonValiderNote, SIGNAL(clicked()), this, SLOT(cacherSousFenetre()));
}

/*
 * Voir une note
 */
void FenetrePrincipale::visualiserNote(QListWidgetItem * i)
{
    QWidget* fenetre = new QWidget(this);

    Note& n = manager.getNote(i->text().toLocal8Bit().constData());

    fenVisualisation = zoneCentrale->addSubWindow(fenetre);
    fenVisualisation->setWindowTitle("Voir une note");
    fenVisualisation->setWindowIcon(QIcon("C:/Users/SilverEye/notepod/edit-set-5-256.png"));
    fenVisualisation->setBaseSize(200, 200);

    visTitre = new QLabel(QString::fromStdString(n.getTitle()), fenVisualisation);

    fenVisualisation->show();

}

/*
 *  Insertion de note
 */
void FenetrePrincipale::insererNote()
{
    manager.addNote(new Note(id->text().toStdString(),titre->text().toStdString()));
    listeNotes->addItem(id->text());

    // Nettoyer les lignes
    foreach(QLineEdit *line, this->findChildren<QLineEdit*>())
    {
        line->clear();
    }
    // TODO: ajouter une erreur au cas où l'identificateur existerait déjà
}

void FenetrePrincipale::unhide()
{
   boutonValiderNote->setDisabled(false);
}

void FenetrePrincipale::cacherSousFenetre()
{
    sousFenetre1->hide();
}


