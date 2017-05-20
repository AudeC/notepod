#ifndef FENETRE_H
#define FENETRE_H

#include <QApplication>
#include <QtWidgets>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>

class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT
private:
    QMdiArea *zoneCentrale;
    QMenu *menuFichier;
    QMenu *menuEdition;
    QMenu *menuAffichage;
    QListWidget *listeNotes;
    void creerDock();
public:
    FenetrePrincipale();
public slots:
    void ouvrirDialogue();
    void ouvrirNote();
};

#endif // FENETRE_H
