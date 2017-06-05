#ifndef FENETRE_H
#define FENETRE_H

#include "notes.h"
#include <QApplication>
#include <QtWidgets>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>


using namespace NOTES;

/*
 * Easter Egg - Denis Brogniart
 */
class Denis : public QMessageBox
{
    Q_OBJECT
public:
    Denis()
    {
        setWindowTitle("Ah!");
        setText("Ça veut dire que cette fonctionalité n'est pas encore implémentée...?");
        setStandardButtons(QMessageBox::Ok);
        setDefaultButton(QMessageBox::Ok);
        QPixmap icon("C:/Users/SilverEye/notepod/denis.brogniart.ah.png");
        setIconPixmap(icon);
        setWindowIcon(QIcon("C:/Users/SilverEye/notepod/1495391974_cancel_16.png"));
    }
    void ouvrir()
    {
        show();
        exec();
    }
};

class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT
protected:
    NotesManager manager;

    QMdiArea* zoneCentrale;
    QDockWidget* dock;

    QMenu* menuFichier;
    QMenu* menuEdition;
    QMenu* menuAffichage;

    QListWidget* listeNotes;
    QPushButton* boutonAjouter;
    QVBoxLayout* dockLayout;
    QWidget* widget;

    QDockWidget* dock2;
    void creerDock();

    QMdiSubWindow* sousFenetre;
public:
    FenetrePrincipale();
signals:

public slots:
    void creerSFNote();
    void receptionNote(QString, QString);
    void visualiserNote(QListWidgetItem*);
    void ouvrirDenis(){Denis ah; ah.ouvrir();}
    void fermerSousFenetre()
    {
        sousFenetre->hide();
        delete sousFenetre;
    }
    void redimensionnerFenetre();
};

class ContenuSousFenetre : public QWidget
{
    Q_OBJECT
protected:
    QGridLayout* layoutFenetre;
    QFormLayout* formLayout;
    QLineEdit* titre;

public:
    ContenuSousFenetre()
    {
        layoutFenetre = new QGridLayout;
        formLayout = new QFormLayout;
        titre = new QLineEdit;
        setWindowIcon(QIcon("C:/Users/SilverEye/notepod/edit-set-5-256.png"));
    }
public slots:
    void ouvrirDenis(){Denis ah; ah.ouvrir();}
};

class ContenuFenetreAjoutNote : public ContenuSousFenetre
{
    Q_OBJECT
protected:
    QLineEdit* id;
    QPushButton* boutonValiderNote;
    QVBoxLayout* layoutBoutons;
    QHBoxLayout* layoutAjout;
    QPushButton* boutonTexte;
    QPushButton* boutonFichier;
    QLineEdit* adresseFichier;
    QPushButton* boutonTache;

public:
    ContenuFenetreAjoutNote(): ContenuSousFenetre()
    {
        id = new QLineEdit;
        formLayout->addRow("Identificateur", id);
        formLayout->addRow("Titre", titre);
        layoutFenetre->addLayout(formLayout, 0, 0);

        layoutBoutons = new QVBoxLayout;
        boutonTexte = new QPushButton("Ajouter un texte");
        connect(boutonTexte, SIGNAL(clicked()), this, SLOT(widgetAjouterTexte()));
        layoutBoutons->addWidget(boutonTexte);
        boutonTache = new QPushButton("Ajouter une tâche");
        connect(boutonTache, SIGNAL(clicked()), this, SLOT(ouvrirDenis()));
        layoutBoutons->addWidget(boutonTache);
        boutonFichier = new QPushButton("Ajouter un fichier");
        connect(boutonFichier, SIGNAL(clicked()), this, SLOT(widgetAjouterFichier()));
        layoutBoutons->addWidget(boutonFichier);
        layoutFenetre->addLayout(layoutBoutons, 0, 1);

        boutonValiderNote = new QPushButton("Créer la nouvelle note");
        connect(boutonValiderNote, SIGNAL(clicked()), this, SLOT(insererNote()));
        boutonValiderNote->setShortcut(QKeySequence(Qt::Key_Return));
        layoutFenetre->addWidget(boutonValiderNote, 3, 0, 1, 2);
        boutonValiderNote->setDisabled(true);
        connect(id, SIGNAL(textChanged(QString)), this, SLOT(montrerBouton()));
        connect(titre, SIGNAL(textChanged(QString)), this, SLOT(montrerBouton()));
        /*if (&&) emit formulaireRempli();
        connect(this, SIGNAL(formulaireRempli()), this, SLOT(montrerBouton()));*/
        //amélioration: faire en sorte que le slot ne soit appelé que si les deux signaux ont été émis
        setLayout(layoutFenetre);
        setWindowTitle("Nouvelle note");
    }

    QPushButton* getBoutonValiderNote() {return boutonValiderNote;}

signals:
    void nouvelleNote(QString id, QString titre);
    //void formulaireRempli();
    void demandeRedimensionnement();

public slots:
    void insererNote()
    {
        emit nouvelleNote(id->text(), titre->text());

        //s'occuper aussi de l'ajout de texte, fichier et tâche

        // Nettoyer les lignes
        /*foreach(QLineEdit* line, this->findChildren<QLineEdit*>())
        {
            line->clear();
        }*/
        // TODO: ajouter une erreur au cas où l'identificateur existerait déjà
    }

    void montrerBouton() {boutonValiderNote->setDisabled(false);}

    void widgetAjouterTexte()
    {
        emit demandeRedimensionnement();
        QTextEdit* texte = new QTextEdit;
        layoutFenetre->addWidget(texte, 2, 0, 1, 2);
        boutonTexte->hide();

    }

    void widgetAjouterFichier()
    {
        adresseFichier = new QLineEdit;

        QPushButton* button = new QPushButton("...");
        connect(button, SIGNAL(clicked()), SLOT(chercherFichier()));
        QHBoxLayout* layout = new QHBoxLayout;
        layout->addWidget(adresseFichier);
        layout->addWidget(button);
        layoutFenetre->addLayout(layout, 1, 0, 1, 2);
        boutonFichier->hide();
    }

    void chercherFichier()
    {

        QString fichier = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"),
                                                        QDir::toNativeSeparators(QDir::currentPath()),
                                                        tr("Images (*.png *.gif *.jpg);;"
                                                           "Fichiers audio (*.mp3 *.wav *.wma);;"
                                                           "Fichiers vidéo (*.mp4 *.avi *.wmv)"));

        if (!fichier.isNull())
            adresseFichier->setText(QDir::toNativeSeparators(fichier));
    }
};

class ContenuFenetreVisualisationNote : public ContenuSousFenetre
{
    Q_OBJECT
protected:
    NotesManager manager;
public:
    ContenuFenetreVisualisationNote(QListWidgetItem* i): ContenuSousFenetre()
    {
        //Récupération de la note dans le manager
        manager = NotesManager::getInstance();
        try{
        Note& n = manager.getNote(i->text().toLocal8Bit().constData());
        //bug de l'instruction ci-dessus

        //Création du layout de la sous fenetre
        titre->setPlaceholderText(QString::fromStdString(n.getTitle()));
        formLayout->addRow("Titre", titre);
        layoutFenetre->addLayout(formLayout, 0, 0);
        titre->setDisabled(true);

        setLayout(layoutFenetre);
        setWindowTitle(QString::fromStdString(n.getId()));
        setWindowIcon(QIcon("C:/Users/SilverEye/notepod/edit-set-5-256.png"));
        }catch(NotesException e) {std::cout<<e.getInfo();}
    }
};



#endif // FENETRE_H
