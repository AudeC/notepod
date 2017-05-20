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
    connect(actionNouveau, SIGNAL(triggered()), this, SLOT(ouvrirDialogue()));
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
    ah.setText("Vous voulez dire que...?");
    ah.setStandardButtons(QMessageBox::Ok);
    ah.setDefaultButton(QMessageBox::Ok);
    QPixmap icon("C:/Users/SilverEye/notepod/notepod/denis.brogniart.ah.png");
    ah.setIconPixmap(icon);
    ah.show();
    ah.exec();
}

void FenetrePrincipale::creerDock()
{
    QDockWidget *dock = new QDockWidget(tr("Notes actives"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    listeNotes = new QListWidget(dock);
    manager.addNote(new Note("id1","montest"));
    manager.addNote(new Note("id2","montest2"));
    for(Note i : manager.getNotes())
    {
        listeNotes->addItem(QString::fromStdString(i.getTitle()));
    }
    dock->setWidget(listeNotes);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    menuAffichage->addAction(dock->toggleViewAction());
    //connect(listeNotes, SIGNAL(currentTextChanged(QString)),this, SLOT(insererNotes(QString)));
}

/*void FenetrePrincipale::insererNote(const QString& note)
 {
     if (note.isEmpty())
         return;
     listeNotes = note.split(", ");
 }*/

