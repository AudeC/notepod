
#include "Notes.h"
#include <fstream>
using namespace TIME;

namespace NOTES {
	void Note::setTitle(const string& t) {
		title = t;
	}

	void Note::archiver()
	{
		actif = false;
	}

	//NotesManager* NotesManager::instanceUnique= nullptr;
	NotesManager::Handler NotesManager::handler = NotesManager::Handler();

	NotesManager& NotesManager::getInstance() {
		if (handler.instance == nullptr)
			handler.instance = new NotesManager;
		return *handler.instance;
	}

	void NotesManager::freeInstance() {
		delete handler.instance;
		handler.instance = nullptr;

	}



	void NotesManager::addNote(Note* a) {
		for (Note i : notes) {
			if (i.getId() == a->getId()) throw NotesException("error, creation of an already existent note");
		}
		notes.push_back(*a); 
		
	}

	void NotesManager::addRelation(Relation* r)
	{
		relations.push_back(*r); 
	}

	Note& NotesManager::getNote(const string& id) {
		int j = 0;
		for (Note i : notes) {
			if (i.getId() == id) return notes[j]; 
			j++;
		}
		throw NotesException("error, nonexistent note");

	}

	Relation& NotesManager::getNewRelation(const string& n, const string& d, bool o)
	{
		Relation *r = new Relation(n, d, o); 
		addRelation(r); 
		return *r; 
	}

	Relation& NotesManager::getRelation(const string& n)
	{
		int j = 0;
		for(Relation i: relations)
		{
			if (i.getTitre() == n) return relations[j];
			j++;
		}
		throw new NotesException("La relation n'existe pas");
	}
	Note& NotesManager::getNewNote(const string& id) {
		Note* a = new Note(id, "");
		addNote(a);
		return *a;
	}
	Article& NotesManager::getNewArticle(const string& id) {
		Article* a = new Article(id, "", "");
		addNote(a);
		return *a;
	}
	Tache& NotesManager::getNewTache(const string& id) {
		Tache* a = new Tache(id, "", "");
		addNote(a);
		return *a;
	}
	Media& NotesManager::getNewMedia(const string& id, enum Mediatype m) {
		Media* a = new Media(id, "", m, "");
		addNote(a);
		return *a;
	}




	void NotesManager::save() const {
		ofstream fout(filename);
		for (Note i : notes) {
			fout << i;
		}
		fout.close();
	}



	void NotesManager::load(const string& f) {
		if (filename != f) save();
		filename = f;
		ifstream fin(filename); // open file
		if (!fin) throw NotesException("error, file does not exist");
		while (!fin.eof() && fin.good()) {
			char tmp[1000];
			fin.getline(tmp, 1000); // get id on the first line
			if (fin.bad()) throw NotesException("error reading note id on file");
			string id = tmp;
			fin.getline(tmp, 1000); // get title on the next line
			if (fin.bad()) throw NotesException("error reading note title on file");
			string title = tmp;
			Note* a = new Note(id, title);
			addNote(a);
			if (fin.peek() == '\r') fin.ignore();
			if (fin.peek() == '\n') fin.ignore();
		}
		fin.close(); // close file
	}

}

ostream& operator<<(ostream& f, const NOTES::Note& a) {
	f << a.getId() << "\n";
	f << a.getTitle() << "\n";
	return f;
}

ostream& operator<<(ostream& f, const NOTES::Relation::Couple& a) {
	f << "(" << a.getMb1() << ",";
	f << a.getMb2() << ")";
	return f;
}
ostream& operator<<(ostream& f, const NOTES::Relation& a) {
	f << a.getTitre() << " : relation impliquant " << a.getCouples().size() << " couples\n"; 
	for(NOTES::Relation::Couple c : a.getCouples() )
	{
		f << c << " / "; 
	}
	f << "\n";
	return f;
}