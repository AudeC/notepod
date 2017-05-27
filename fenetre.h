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

    // Visualiser note
    QMdiSubWindow* fenVisualisation;
    QLabel* visTitre;

    QDockWidget* dock2;
    void creerDock();
public:
    FenetrePrincipale();
    NotesManager getManager() const {return manager;}
signals:

public slots:
    void unhide(QWidget* widget) {widget->setDisabled(false);}
    void creerSFNote();
    void receptionNote(QString, QString);
    void visualiserNote(QListWidgetItem*);
    void ouvrirDenis(){Denis ah; ah.ouvrir();}
};

class SousFenetre : public QWidget
{
    Q_OBJECT
protected:
    NotesManager manager;
    QMdiSubWindow* fenetre;
    QWidget* contenuFenetre;
    QVBoxLayout* layoutFenetre;
    QFormLayout* formLayout;
    QLineEdit* titre;
public:
    SousFenetre()
    {
        manager = NotesManager::getInstance();
        fenetre->setWindowIcon(QIcon("C:/Users/SilverEye/notepod/edit-set-5-256.png"));
    }
public slots:
    void cacherSousFenetre() {fenetre->hide();}
};

class FenetreAjoutNote : public SousFenetre
{
    Q_OBJECT
protected:
    QLineEdit* id;
    QPushButton* boutonValiderNote;
public:
    FenetreAjoutNote(QMdiArea * zoneCentrale): SousFenetre()
    {
        id = new QLineEdit;
        titre = new QLineEdit;
        QFormLayout* ajoutNoteLayout = new QFormLayout;
        ajoutNoteLayout->addRow("Identificateur", id);
        ajoutNoteLayout->addRow("Titre", titre);
        layoutFenetre->addLayout(ajoutNoteLayout);

        boutonValiderNote = new QPushButton("Ajouter");
        connect(boutonValiderNote, SIGNAL(clicked()), this, SLOT(insererNote()));
        boutonValiderNote->setShortcut(QKeySequence(Qt::Key_Return));
        layoutFenetre->addWidget(boutonValiderNote);
        boutonValiderNote->setDisabled(true);
        connect(id, SIGNAL(textChanged(QString)), this, SLOT(unhide()));
        connect(titre, SIGNAL(textChanged(QString)), this, SLOT(unhide()));

        contenuFenetre->setLayout(layoutFenetre);
        fenetre = zoneCentrale->addSubWindow(contenuFenetre);
        fenetre->setWindowTitle("Nouvelle note");
    }
signals:
    void nouvelleNote(QString id, QString titre);

public slots:
    void insererNote()
    {
        emit nouvelleNote(id->text(), titre->text());
        // Nettoyer les lignes
        foreach(QLineEdit* line, this->findChildren<QLineEdit*>())
        {
            line->clear();
        }
        // TODO: ajouter une erreur au cas où l'identificateur existerait déjà
    }


};







#endif // FENETRE_H
