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

class FenetrePrincipale : public QMainWindow, public NotesManager
{
    Q_OBJECT
private:
    NotesManager manager;
    QMdiArea *zoneCentrale;
    QMenu *menuFichier;
    QMenu *menuEdition;
    QMenu *menuAffichage;
    QListWidget *listeNotes;
    void creerDock();
public:
    FenetrePrincipale();
    NotesManager getManager() {return manager;}
public slots:
    void ouvrirDialogue();
    //void insererNote(const QString&);

};

#endif // FENETRE_H
