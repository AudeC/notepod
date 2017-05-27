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
    ah.setText("Ça veut dire que cette fonctionalité n'est pas encore implémentée...?");
    ah.setStandardButtons(QMessageBox::Ok);
    ah.setDefaultButton(QMessageBox::Ok);
    QPixmap icon("C:/Users/SilverEye/notepod/denis.brogniart.ah.png");
    ah.setIconPixmap(icon);
    ah.setWindowIcon(QIcon("C:/Users/SilverEye/notepod/1495391974_cancel_16.png"));
    ah.show();
    ah.exec();
}

void FenetrePrincipale::creerDock()
{
    dock = new QDockWidget(tr("Notes actives"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    dock2 = new QDockWidget(tr("Relations"), this);
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
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    addDockWidget(Qt::RightDockWidgetArea, dock2);

    menuAffichage->addAction(dock2->toggleViewAction());
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

void FenetrePrincipale::insererNote()
{
    manager.addNote(new Note(id->text().toStdString(),titre->text().toStdString()));
    listeNotes->addItem(titre->text());
    foreach(QLineEdit *line, this->findChildren<QLineEdit*>())
    {
        line->clear();
    }
    //ajouter une erreur au cas où l'identificateur existerait déjà
}

void FenetrePrincipale::unhide()
{
   boutonValiderNote->setDisabled(false);
}

void FenetrePrincipale::cacherSousFenetre()
{
    sousFenetre1->hide();
}


