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

class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT
private:
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
    QMdiSubWindow* sousFenetre1;

    QWidget* sousFenetre;
    QLineEdit* id;
    QLineEdit* titre;
    QPushButton* boutonValiderNote;

    // Visualiser note
    QMdiSubWindow* fenVisualisation;
    QLabel* visTitre;

    QDockWidget* dock2;
    void creerDock();
public:
    FenetrePrincipale();
    NotesManager getManager() const {return manager;}
public slots:
    void ouvrirDialogue();
    void sousFenetreAjoutNote();
    void insererNote();
    void unhide();
    void cacherSousFenetre();
    void visualiserNote(QListWidgetItem * i);
};

#endif // FENETRE_H
