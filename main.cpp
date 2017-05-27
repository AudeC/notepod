#include <QApplication>
#include <iostream>
#include "fenetre.h"
#include "notes.h"


int main(int argc, char *argv[])
{
    // Création de l'appli et de la fenêtre
    QApplication app(argc, argv);
    FenetrePrincipale fenetre;
    fenetre.show();

    return app.exec();
}

