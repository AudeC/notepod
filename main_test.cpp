// projet_td5.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include "Notes.h"

using namespace NOTES;

int main() {
	NotesManager app = NotesManager::getInstance();
	
	Note& test = app.getNewNote("montest");
	Article& test2 = app.getNewArticle("montest_meilleur");
	cout << test; 
	Relation& rel = app.getNewRelation("Meilleur");
	rel.ajouterCouple(&test, &test2, "test"); 
	cout << rel; 
	system("pause");
	return 0;
}