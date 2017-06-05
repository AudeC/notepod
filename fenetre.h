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

/*
 * Classe fenêtre principale
 */
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

    QDockWidget* dockRelations;
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

/*
 * Classe gérant le contenu des sous-fenêtres
 */
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

/*
 * Classe gérant le contenu des sous-fenêtres d'ajout de note
 */
class ContenuFenetreAjoutNote : public ContenuSousFenetre
{
    Q_OBJECT
protected:
    QLineEdit* id;
    QPushButton* boutonValiderNote;
    QComboBox* options;
    QAction* ajouterTexte;
    QAction* ajouterTache;
    QAction* ajouterFichier;
    QLineEdit* adresseFichier;

public:
    ContenuFenetreAjoutNote(): ContenuSousFenetre()
    {
        id = new QLineEdit;
        formLayout->addRow("Identificateur", id);
        formLayout->addRow("Titre", titre);
        layoutFenetre->addLayout(formLayout, 0, 0);

        options = new QComboBox;
        options->addItem(QString("<Choisissez un type de contenu>"));

        ajouterTexte = new QAction("Texte", this);
        options->addItem(ajouterTexte->text(), QVariant::fromValue(ajouterTexte));
        connect(ajouterTexte, SIGNAL(triggered()), this, SLOT(widgetAjouterTexte()));

        ajouterTache = new QAction("Tâche", this);
        options->addItem(ajouterTache->text(), QVariant::fromValue(ajouterTache));
        connect(ajouterTache, SIGNAL(triggered()), this, SLOT(ouvrirDenis()));

        ajouterFichier = new QAction("Fichier média", this);
        options->addItem(ajouterFichier->text(), QVariant::fromValue(ajouterFichier));
        connect(ajouterFichier, SIGNAL(triggered()), this, SLOT(widgetAjouterFichier()));

        layoutFenetre->addWidget(options, 1, 0);
        connect(options, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionAction(int)));

        boutonValiderNote = new QPushButton("Créer la nouvelle note");
        connect(boutonValiderNote, SIGNAL(clicked()), this, SLOT(insererNote()));
        boutonValiderNote->setShortcut(QKeySequence(Qt::Key_Return));
        layoutFenetre->addWidget(boutonValiderNote, 3, 0);
        boutonValiderNote->setDisabled(true);

        setLayout(layoutFenetre);
        setWindowTitle("Nouvelle note");
    }

    QPushButton* getBoutonValiderNote() {return boutonValiderNote;}

signals:
    void nouvelleNote(QString id, QString titre);
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

    void montrerBouton()
    {
        if (id->text()!=QString()&&titre->text()!=QString())boutonValiderNote->setDisabled(false);
        else boutonValiderNote->setDisabled(true);
    }

    void selectionAction(int index)
    {
        QAction* action = options->itemData(index, Qt::UserRole).value<QAction*>();
        if (action)
        {
            action->trigger();
        }
    }

    void widgetAjouterTexte()
    {
        emit demandeRedimensionnement();
        QLayoutItem* item = layoutFenetre->itemAtPosition(2, 0);
        if (item && item->widget()) item->widget()->deleteLater();
        //item->deleteLater();
        QTextEdit* texte = new QTextEdit;
        layoutFenetre->addWidget(texte, 2, 0);
        connect(id, SIGNAL(textChanged(QString)), this, SLOT(montrerBouton()));
        connect(titre, SIGNAL(textChanged(QString)), this, SLOT(montrerBouton()));
    }

    void widgetAjouterFichier()
    {
        adresseFichier = new QLineEdit;
        QLayoutItem* item = layoutFenetre->itemAtPosition(2, 0);
        if (item && item->widget()) item->widget()->deleteLater();
        //item->deleteLater();
        QPushButton* button = new QPushButton("Parcourir");
        connect(button, SIGNAL(clicked()), SLOT(chercherFichier()));
        QHBoxLayout* layout = new QHBoxLayout;
        layout->addWidget(adresseFichier);
        layout->addWidget(button);
        layoutFenetre->addLayout(layout, 2, 0);
        connect(id, SIGNAL(textChanged(QString)), this, SLOT(montrerBouton()));
        connect(titre, SIGNAL(textChanged(QString)), this, SLOT(montrerBouton()));
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

/*
 * Classe gérant le contenu des sous-fenêtres de visualisation de notes
 */
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
