#if !defined(_NOTES_H)
#define _NOTES_H

#include <string>
#include <iostream>
#include "timing.h"
#include <vector>
using namespace std;


enum Mediatype { image, son, video };

namespace NOTES {
	class NotesException {
	public:
		NotesException(const string& message) :info(message) {}
		string getInfo() const { return info; }
	private:
		string info;
	};

	class Note {
		friend class NotesManager;
		string id;
		string title;
		TIME::Date creation;
		TIME::Date modification;
		bool actif;
		// éviter la duplication d'articles

		
	protected:
        //Note& operator=(const Note& a);
		//Note(const Note& a);
    public:
		Note(const string& i, const string& ti) : id(i), title(ti), creation(TIME::Date()), modification(TIME::Date()) {};

	public:
	
		string getId() const { return id; }
		string getTitle() const { return title; }
		TIME::Date getCreation() const { return creation; }
		TIME::Date getModification() const { return modification; }
		void setTitle(const string& t);
		void archiver();
	};

	class Article : public Note
	{
		friend class NotesManager;
		string texte;
		Article(const string& i, const string& ti, const string& te) : Note(i, ti), texte(te) {}
	public:
		string getTexte() const { return texte; }
		void setTexte(const string& s) { texte = s;  }

	};
	class Media : public Note
	{
		friend class NotesManager;
		string texte;
		enum Mediatype type;
		Media(const string& i, const string& ti, enum Mediatype m, const string& te) : Note(i, ti), texte(te), type(m) {}
	public:
		string getTexte() const { return texte; }
		enum Mediatype getType() const { return type; }
		void setTexte(const string& t) { texte = t;  }
		void setType(enum Mediatype m) { type = m;  }
	};
	class Tache: public Note
	{
		friend class NotesManager;
		string action;
		int priorite;
		TIME::Date echeance; 
		Tache(const string& i, const string& ti, const string& a, int p = 0, TIME::Date e = 0):
			Note(i ,ti), action(a), priorite(p), echeance(e){}
	public:
		string getAction() const{ return action; }
		int getPriorite() const{ return priorite; }
		TIME::Date getEcheance() const { return echeance; }
		void setAction(const string& s) { action = s;  }
		void setPriorite(int i) { priorite = i; }
		void setEcheance(TIME::Date d) { echeance = d;  }
	};
	class Relation
	{
	public: 
		class Couple
		{
			Note* mb1;
			Note* mb2;
			string label;
		public:
			Couple(Note* m1, Note* m2, const string& la = "") : mb1(m1), mb2(m2), label(la) {}
			Couple(Note m1, Note m2, const string& la = "") : mb1(&m1), mb2(&m2), label(la) {}
			string getLabel() const { return label; }
			void setLabel(const string& la) { label = la; }
			Note& getMb1() const { return *mb1;  }
			Note& getMb2() const { return *mb2;  }
			bool contient(Note* m) const
			{
				if (m == mb1 || m == mb2) return true;
				else return false;
			}
		};

	private: 
		Relation(const string& t, const string& d, bool o = false) : titre(t), description(d), oriente(o) {}
		string titre;
		string description;
		bool oriente;
		vector<Couple> couples;
		friend class NotesManager; 
	public:
		
		string getTitre() const { return titre; }
		string getDescription() const { return description; }
		vector<Couple> getCouples() const { return couples; }
	
		void ajouterCouple(Note* m1, Note* m2, const string& la = "")
		{
			for(Couple c : couples)
			{
				if (c.contient(m1) && c.contient(m2)) throw NotesException("Couple déjà existant"); 
			}
			couples.push_back(Couple(m1, m2, la));
		}
		

		
	};

	class NotesManager {
	public: 
		// handler
		struct Handler {
			NotesManager* instance;
			Handler() :instance(nullptr) {}
			~Handler() {
				delete instance;
				instance = nullptr;
			}
			
		};

	private:
		vector<Note> notes; 
		vector<Relation> relations; 
		string filename;
		static Handler handler;

		//NotesManager& operator=(const NotesManager& m);
        //NotesManager(): filename("notes.json"){}
		//NotesManager(const NotesManager& m);

		friend struct Handler;
    public:
		void addNote(Note* a);
    private:
		void addRelation(Relation* r); 
	
	public:
		static NotesManager& getInstance();
		// donne le seul NotesManager du programme
		static void freeInstance(); // le supprime

		static Handler getHandler() 
		{
			return handler; 
		}
		Note& getNewNote(const string& id);
		Note& getNote(const string& id);
		Article& getNewArticle(const string& id);
		Tache& getNewTache(const string& id);
		Media& getNewMedia(const string& id, enum Mediatype m = image);
		Relation& getNewRelation(const string& n, const string& d = "", bool o = false);
		Relation& getRelation(const string& n);
        vector<Note> getNotes(){ return notes; }
		void load(const string& f);
		void save() const;
		virtual ~NotesManager()
		{
			notes.clear();
			relations.clear();
			filename = "";
		}

		/*
		class iterator {
			int ptr;
			int max;
			Note** notes;
			iterator(Note** a, int m, int c = 0) {
				notes = a;
				ptr = c;
				max = m;
			}
			friend class NotesManager;
		public:
			void operator++() {
				if (ptr < max) ptr++;
				else throw NotesException("iterateur fini");
			}
			Note& operator*() const {
				return *notes[ptr];
			}
			bool operator!=(const iterator a) const {
				return notes != a.notes || ptr != a.ptr;
			}

		};
		NotesManager::iterator begin() const { return NotesManager::iterator(notes, nbNotes); }
		NotesManager::iterator end() const { return NotesManager::iterator(notes, nbNotes, nbNotes); }

		class ConstIterator {
			int ptr;
			int max;
			Note** articles;
			friend class NotesManager;
			ConstIterator(Note** a, int m) {
				articles = a;
				ptr = 0;
				max = m;
			}
		public:
			int isDone() const {
				if (ptr == max)
					return 1;
			}
			void next() {
				if (ptr < max) ptr++;
				else throw NotesException("iterateur fini");
			}
			const Note& current() const {
				return *articles[ptr];
			}
		};


		ConstIterator getIterator() const { return ConstIterator(notes, nbNotes); }
		ConstIterator getConstIterator() const { return ConstIterator(notes, nbNotes); }

		class SearchIterator {
			int ptr;
			int max;
			string search;
			Note** notes;
			friend class NotesManager;
			SearchIterator(Note** a, int m, const string& se = "") {
				notes = a;
				ptr = 0;
				max = m;
				search = se;
				do {
					ptr++;
				} while (ptr < max && notes[ptr]->getTitle().find(search) == string::npos);
			}
		public:
			bool isDone() const {
				return ptr == max;
			}
			void next() {
				if (ptr < max) {
					do {
						ptr++;
					} while (ptr < max && notes[ptr]->getTitle().find(search) == string::npos);
				}
				else throw NotesException("iterateur fini");
			}
			Note& current() const {
				return *notes[ptr];
			}
		};
		SearchIterator getSearchIterator(const string& txt) const { return SearchIterator(notes, nbNotes, txt); }
		*/

	};


}

ostream& operator<<(ostream& f, const NOTES::Note& a);
ostream& operator<<(ostream& f, const NOTES::Relation& a);
ostream& operator<<(ostream& f, const NOTES::Relation::Couple& a);
#endif
