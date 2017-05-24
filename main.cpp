#include <QApplication>
#include <iostream>
#include "fenetre.h"
#include "notes.h"

using namespace NOTES;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);



    FenetrePrincipale fenetre;


    /*NotesManager manager=fenetre.getManager();
    Note& test = manager.getNewNote("montest");
    Article& test2 = manager.getNewArticle("montest_meilleur");*/

    /*Relation& rel = manager.getNewRelation("Meilleur");
    rel.ajouterCouple(&test, &test2, "test");
    cout << rel;*/
    fenetre.show();

    return app.exec();
}

