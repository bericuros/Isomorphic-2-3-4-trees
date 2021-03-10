#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<queue>
#include<stack>
#include<Windows.h>

using namespace std;

class Data {
public:
	Data(string, int);

	friend bool operator==(const Data&, const Data &);
	friend bool operator!=(const Data&, const Data &);
	friend bool operator<(const Data &, const Data &);
	friend bool operator>(const Data &, const Data &);
	friend ostream & operator<<(ostream &, const Data &);

	string name; // action
	int key;  // priority
};

Data::Data(string aName = "", int aKey = 0) : name(aName), key(aKey) {}

bool operator==(const Data &d1, const Data &d2) {
	return d1.name == d2.name && d1.key == d2.key;
}

bool operator!=(const Data &d1, const Data &d2) {
	return !(d1 == d2);
}

bool operator<(const Data &d1, const Data &d2) {
	return d1.key < d2.key || d1.key == d2.key && d1.name < d2.name;
}

bool operator>(const Data &d1, const Data &d2) {
	return d1.key > d2.key || d1.key == d2.key && d1.name > d2.name;
}

ostream & operator<<(ostream &os, const Data &d) {
	return os << d.key << " : " << d.name;
}

class Tree {
public:
	Tree();

	friend ostream & operator<<(ostream &, const Tree &);

	Data data[3];
	Tree *son[4], *par;
};

Tree::Tree() {
	for (int i = 0; i < 3; i++) data[i] = Data();
	for (int i = 0; i < 4; i++) son[i] = nullptr;
	par = nullptr;
}

void Print234Tree(Tree *&, ostream &);

void PrintRedBlackTree(Tree *&, ostream &);

void PrintNames(Tree *&, ostream &);

ostream & operator<<(ostream &cout, Tree *&root) {
	Print234Tree(root, cout);
	PrintRedBlackTree(root, cout);
	PrintNames(root, cout);
	return cout;
}

void clear() {
	system("cls");
}

int power(int x, int n) {
	int res = 1;
	while (n) {
		if (n & 1) res *= x;
		x *= x;
		n >>= 1;
	}
	return res;
}

int GetIdx(Tree *&node, string name, int key) {
	if (!node) return -1;
	Data data = { name, key };
	for (int i = 0; i < 3; i++) {
		if (node->data[i] == data) {
			return i;
		}
	}
}

int MainMenu(Tree *&root) {

	int item = 0;
	while (true) {
		cout << "Unesite redni broj zeljene stavke:\n";
		cout << "1. Stvaranje stabla.\n";
		cout << "2. Ispisivanje stabla.\n";
		cout << "3. Umetanje nove akcije u stablo.\n";
		cout << "4. Brisanje akcije.\n";
		cout << "5. Izmena prioriteta akcije.\n";
		cout << "6. Pronalazenje broja akcija zadatog prioriteta.\n";
		cout << "7. Dealociraj stablo.\n";
		cout << "8. Izlaz.\n\n";
		cin >> item;
		if (item < 1 || item > 8) {
			clear();
			cout << "Uneli ste nevalidni broj, pokusajte opet.\n\n";
		}
		else if (!root && item != 1 && item != 3 && item != 8) {
			clear();
			cout << "Treba prvo da se napravi stablo kako bi se izvrsila odabrana naredba.\n\n";
			item = 0;
		}
		else if (root && item == 1) {
			clear();
			cout << "Treba prvo da se dealocira postojece stablo kako bi se kreiralo novo.\n\n";
			item = 0;
		}
		return item;
	}

}

bool SearchTree(Tree *&root, string name, int key, Tree *&parNode, int &idx) {

	parNode = nullptr;
	Tree *node = root;
	Data data = { name, key };
	while (node) {
		parNode = node;
		idx = -1; // nepotrebno ?
		for (int i = 0; i < 3; i++) {
			if (node->data[i].name == "") continue;
			idx = i;
			if (node->data[i] == data) return true;
			else if (node->data[i] > data) break;
			idx++;
		}
		node = node->son[idx];
	}
	return false;

}

bool InsertNode(Tree *&root, string name, int key) {
	Tree *parNode;
	int idx;
	if (SearchTree(root, name, key, parNode, idx)) return false;

	Data data = { name, key };
	Tree *left = nullptr, *right = nullptr;
	while (data.name != "") {

		if (parNode == nullptr) {
			root = new Tree;
			root->data[1] = data;
			root->son[1] = left;
			root->son[2] = right;
			if (left) left->par = root;
			if (right) right->par = root;
			break;
		}

		int type = 0; // (0 - samo sredisnji kljuc postoji, 1 - samo desni kljuc prazan, 2 - samo levi kljuc prazan, 3 - popunjen cvor)
		if (parNode->data[0].name != "") type += 1;
		if (parNode->data[2].name != "") type += 2;
		
		if (type == 0) {
			if (parNode->data[1] > data) {
				parNode->data[0] = data;
				parNode->son[0] = left;
				parNode->son[1] = right;
			}
			else {
				parNode->data[2] = data;
				parNode->son[2] = left;
				parNode->son[3] = right;
			}
			if (left) left->par = parNode;
			if (right) right->par = parNode;
			data.name = "";
		}
		else if (type == 1) {
			if (parNode->data[1] < data) {
				parNode->data[2] = data;
				parNode->son[2] = left;
				parNode->son[3] = right;
			}
			else if (parNode->data[0] > data) {
				parNode->data[2] = parNode->data[1];
				parNode->data[1] = parNode->data[0];
				parNode->data[0] = data;
				parNode->son[3] = parNode->son[2];
				parNode->son[2] = parNode->son[1];
				parNode->son[1] = right;
				parNode->son[0] = left;
			}
			else {
				parNode->data[2] = parNode->data[1];
				parNode->data[1] = data;
				parNode->son[3] = parNode->son[2];
				parNode->son[2] = right;
				parNode->son[1] = left;
			}
			if (left) left->par = parNode;
			if (right) right->par = parNode;
			data.name = "";
		}
		else if (type == 2) {
			if (parNode->data[1] > data) {
				parNode->data[0] = data;
				parNode->son[0] = left;
				parNode->son[1] = right;
			}
			else if (parNode->data[2] < data) {
				parNode->data[0] = parNode->data[1];
				parNode->data[1] = parNode->data[2];
				parNode->data[2] = data;
				parNode->son[0] = parNode->son[1];
				parNode->son[1] = parNode->son[2];
				parNode->son[2] = left;
				parNode->son[3] = right;
			}
			else {
				parNode->data[0] = parNode->data[1];
				parNode->data[1] = data;
				parNode->son[0] = parNode->son[1];
				parNode->son[1] = left;
				parNode->son[2] = right;
			}
			if (left) left->par = parNode;
			if (right) right->par = parNode;
			data.name = "";
		}
		else {
			Tree *newNode = new Tree;
			Data newData = parNode->data[1];
			newNode->data[1] = parNode->data[2];
			parNode->data[1] = parNode->data[0];
			parNode->data[0] = Data(); // postaje prazan
			parNode->data[2] = Data();

			newNode->son[2] = parNode->son[3];
			if (parNode->son[3]) parNode->son[3]->par = newNode;
			newNode->son[1] = parNode->son[2];
			if (parNode->son[2]) parNode->son[2]->par = newNode;

			newNode->par = parNode->par;
			parNode->son[2] = parNode->son[1];
			parNode->son[1] = parNode->son[0];

			parNode->son[3] = nullptr;
			parNode->son[0] = nullptr;
			newNode->son[3] = nullptr;
			newNode->son[0] = nullptr;
			
			if (newNode->data[1] < data) {
				newNode->data[2] = data;
				newNode->son[2] = left;
				newNode->son[3] = right;
				if (left) left->par = newNode;
				if (right) right->par = newNode;
			}
			else if (newNode->data[1] > data && newData < data) {
				newNode->data[0] = data;
				newNode->son[0] = left;
				newNode->son[1] = right;
				if (left) left->par = newNode;
				if (right) right->par = newNode;
			}
			else if (newData > data && parNode->data[1] < data) {
				parNode->data[2] = data;
				parNode->son[2] = left;
				parNode->son[3] = right;
				if (left) left->par = parNode;
				if (right) right->par = parNode;
			}
			else {
				parNode->data[0] = data;
				parNode->son[0] = left;
				parNode->son[1] = right;
				if (left) left->par = parNode;
				if (right) right->par = parNode;
			}

			data = newData;
			left = parNode;
			right = newNode;
		}

		parNode = parNode->par;
	}

	return true;

}

void BuildTree(Tree *&root) {
	clear();
	cout << "Unesite ime datoteke iz koje se cita stablo: ";
	string fileName;
	cin >> fileName;

	ifstream file;
	file.open(fileName.c_str());
	if (!file.is_open()) {
		clear();
		cout << "Datoteka sa datim imenom ne postoji.\n\n";
		return;
	}
	
	string name;
	int key;
	while (file.peek() != EOF) {
		file >> name >> key;
		InsertNode(root, name, key);
	}
	file.close();
	cout << "\n";
}

void InsertAction(Tree *&root) {
	clear();
	cout << "Unesite ime akcije i njen prioritet: ";
	string name;
	int key;
	cin >> name >> key;
	if (InsertNode(root, name, key)) cout << "Akcija je supesno uneta.\n\n";
	else cout << "Navedena akcija vec postoji u stablu.\n\n";
}

void SwitchSucc(Tree *&node, string name, int key, int idx,  Tree *&fNode, int &fIdx) {
	Data data = { name, key };
	Tree *sNode;
	int sIdx;
	
	fNode = node;
	if (idx != 2 && node->data[idx + 1].name != "") fIdx = idx + 1;
	else fIdx = idx;
	sNode = node->son[idx + 1];
	sIdx = 1;

	while (sNode) {
		fNode = sNode;
		fIdx = sIdx;
		if (sIdx == 0) {
			sNode = sNode->son[0];
			sIdx = 1;
		}
		else if (sIdx == 1 && sNode->data[0].name != "") sIdx = 0;
		else {
			sNode = sNode->son[1];
			sIdx = 1;
		}
	}
}

int CalcType(Tree *&node) { // -1 - nullptr, 0 - samo sredisnji kljuc, 1 - levi i sredisnji kljuc, 2 - sredisnji i desni kljuc, 3 - popunjen cvor
	int cnt = 0;
	if (!node) return -1;
	if (node->data[0].name != "") cnt += 1;
	if (node->data[2].name != "") cnt += 2;
	return cnt;
}

void FindBrother(Tree *&node, int &idIdx, Tree *&brNode, int &brIdx) {

	Tree *parNode = node->par;
	int type = CalcType(parNode);

	if (type == -1) idIdx = -1; // cvor je ujedno i koren i list (i ima samo sredsnji kljuc koji se brise)
	else if (type == 0 && parNode->son[1] == node) {
		idIdx = 1;
		brNode = parNode->son[2];
		brIdx = 2;
	}
	else if (type == 0 && parNode->son[2] == node) {
		idIdx = 2;
		brNode = parNode->son[1];
		brIdx = 1;
	}
	else if (parNode->son[0] == node) {
		idIdx = 0;
		brNode = parNode->son[1];
		brIdx = 1;
	}
	else if (parNode->son[1] == node) {
		idIdx = 1;
		brNode = parNode->son[0];
		brIdx = 0;
	}
	else if (parNode->son[2] == node) {
		idIdx = 2;
		brNode = parNode->son[3];
		brIdx = 3;
	}
	else {
		idIdx = 3;
		brNode = parNode->son[2];
		brIdx = 2;
	}
}

int GetSonIdx(Tree *&node, Tree *&son) {
	if (!node) return -1;
	for (int i = 0; i < 4; i++) {
		if (node->son[i] == son) {
			return i;
		}
	}
}

bool DeleteNode(Tree *&root, string name, int key) {
	Tree *node;
	int idx;
	if (!SearchTree(root, name, key, node, idx)) {
		clear();
		cout << "Akcija sa zadatim imenom i prioritetom ne postoji u stablu.\n\n";
		return false;
	}

	idx = GetIdx(node, name, key);

	if (node->son[idx]) { // ako nije list
		Tree *succNode;
		int succIdx;
		SwitchSucc(node, name, key, idx, succNode, succIdx); // idx od kljuca u node-u

		Data temp = node->data[idx];
		node->data[idx] = succNode->data[succIdx];
		succNode->data[succIdx] = temp;
		node = succNode;
		idx = succIdx;
	}

	bool changeIdx = false;

	while (idx != -3) {
		if (idx == -1) { // smanjuje se visina, kljuc iz levog potstabla ostaje crn
			Tree *parNode = node;
			node = parNode->son[1];

			delete parNode;

			node->par = nullptr;
			root = node;
		}
		else if (idx == -2) { // smanjuje se visina, kljuc iz desnog potstabla ostaje crn
			Tree *parNode = node;
			node = parNode->son[2];

			delete parNode;

			node->par = nullptr;
			root = node;
		}
		else if (idx == 0 || idx == 2) node->data[idx] = Data();
		else if (node->data[2].name != "") {  // odavde pa na dalje je idx == 1
			node->data[1] = node->data[2];
			node->data[2] = Data();
		}
		else if (node->data[0].name != "" && node->data[2].name == "") {
			node->data[1] = node->data[0];
			node->data[0] = Data();
		}
		else {
			Tree *parNode = node->par;
			Tree *brNode; // stvarni brat
			int brIdx, idIdx; // idIdx indeks node-a kod svog oca
			FindBrother(node, idIdx, brNode, brIdx);

			if (idIdx == -1) { // brise se poslednji cvor u stablu
				delete node;
				root = nullptr;
				return true;
			}

			int brType = CalcType(brNode);

			if (brType == 1 && brIdx > idIdx) {
				node->data[1] = parNode->data[idIdx];
				parNode->data[idIdx] = brNode->data[0];
				brNode->data[0] = Data();

				if (node->son[2]) node->son[1] = node->son[2];  // ovo se radi zbog propagiranja (kada se visina stabla smanjuje za jedan)
				
				node->son[2] = brNode->son[0];
				if (brNode->son[0]) brNode->son[0]->par = node;
				
				brNode->son[0] = nullptr;
			}
			else if (brType == 2 && brIdx > idIdx) {
				node->data[1] = parNode->data[idIdx];
				parNode->data[idIdx] = brNode->data[1];
				brNode->data[1] = brNode->data[2];
				brNode->data[2] = Data();

				if (node->son[2]) node->son[1] = node->son[2];
				
				node->son[2] = brNode->son[1];
				if (brNode->son[1]) brNode->son[1]->par = node;
				
				brNode->son[1] = brNode->son[2];
				brNode->son[2] = brNode->son[3];
				brNode->son[3] = nullptr;
			}
			else if (brType == 1 && brIdx < idIdx) {
				node->data[1] = parNode->data[brIdx];   // parNode->data[idIdx - 1]
				parNode->data[brIdx] = brNode->data[1];
				brNode->data[1] = brNode->data[0];
				brNode->data[0] = Data();

				if (node->son[1]) node->son[2] = node->son[1];
				
				node->son[1] = brNode->son[2];
				if (brNode->son[2]) brNode->son[2]->par = node;

				brNode->son[2] = brNode->son[1];
				brNode->son[1] = brNode->son[0];
				brNode->son[0] = nullptr;
			}
			else if (brType == 2 && brIdx < idIdx) {
				node->data[1] = parNode->data[brIdx];
				parNode->data[brIdx] = brNode->data[2];
				brNode->data[2] = Data();

				if (node->son[1]) node->son[2] = node->son[1];
				
				node->son[1] = brNode->son[3];
				if (brNode->son[3]) brNode->son[3]->par = node;

				brNode->son[3] = nullptr;
			}
			else if (brType == 3 && brIdx > idIdx) {
				node->data[1] = parNode->data[idIdx];
				parNode->data[idIdx] = brNode->data[1];
				brNode->data[1] = brNode->data[2];
				node->data[2] = brNode->data[0];
				brNode->data[0] = Data();
				brNode->data[2] = Data();

				if (node->son[2]) node->son[1] = node->son[2];
				
				node->son[2] = brNode->son[0];
				if (brNode->son[0]) brNode->son[0]->par = node;

				node->son[3] = brNode->son[1];
				if (brNode->son[1]) brNode->son[1]->par = node;

				brNode->son[0] = nullptr;
				brNode->son[1] = brNode->son[2];
				brNode->son[2] = brNode->son[3];
				brNode->son[3] = nullptr;
			}
			else if (brType == 3 && brIdx < idIdx) {
				node->data[1] = parNode->data[brIdx];
				parNode->data[brIdx] = brNode->data[1];
				brNode->data[1] = brNode->data[0];
				node->data[0] = brNode->data[2];
				brNode->data[0] = Data();
				brNode->data[2] = Data();

				if (node->son[1]) node->son[2] = node->son[1];
				
				node->son[1] = brNode->son[3];
				if (brNode->son[3]) brNode->son[3]->par = node;

				node->son[0] = brNode->son[2];
				if (brNode->son[2]) brNode->son[2]->par = node;
				
				brNode->son[3] = nullptr;
				brNode->son[2] = brNode->son[1];
				brNode->son[1] = brNode->son[0];
				brNode->son[0] = nullptr;
			}
			else if (brType == 0 && idIdx == 0 && brIdx == 1) {
				brNode->data[2] = brNode->data[1];
				brNode->data[1] = parNode->data[0];
				parNode->data[0] = Data();
				parNode->son[0] = nullptr;

				brNode->son[3] = brNode->son[2];
				brNode->son[2] = brNode->son[1];
				if (node->son[2]) {
					brNode->son[1] = node->son[2];
					node->son[2]->par = brNode;
				}
				else if (node->son[1]) {
					brNode->son[1] = node->son[1];
					node->son[1]->par = brNode;
				}

				delete node;
			}
			else if (brType == 0 && idIdx == 1 && brIdx == 0) {
				brNode->data[0] = brNode->data[1];
				brNode->data[1] = parNode->data[0];
				parNode->data[0] = Data();
				parNode->son[1] = parNode->son[0];
				parNode->son[0] = nullptr;

				brNode->son[0] = brNode->son[1];
				brNode->son[1] = brNode->son[2];
				if (node->son[1]) {
					brNode->son[2] = node->son[1];
					node->son[1]->par = brNode;
				}
				else if (node->son[2]) {
					brNode->son[2] = node->son[2];
					node->son[2]->par = brNode;
				}

				delete node;
			}
			else if (brType == 0 && idIdx == 2 && brIdx == 3) {
				brNode->data[2] = brNode->data[1];
				brNode->data[1] = parNode->data[2];
				parNode->data[2] = Data();
				parNode->son[2] = parNode->son[3];
				parNode->son[3] = nullptr;

				brNode->son[3] = brNode->son[2];
				brNode->son[2] = brNode->son[1];
				if (node->son[2]) {
					brNode->son[1] = node->son[2];
					node->son[2]->par = brNode;
				}
				else if (node->son[1]) {
					brNode->son[1] = node->son[1];
					node->son[1]->par = brNode;
				}

				delete node;
			}
			else if (brType == 0 && idIdx == 3 && brIdx == 2) {
				brNode->data[0] = brNode->data[1];
				brNode->data[1] = parNode->data[2];
				parNode->data[2] = Data();
				parNode->son[3] = nullptr;

				brNode->son[0] = brNode->son[1];
				brNode->son[1] = brNode->son[2];
				if (node->son[1]) {
					brNode->son[2] = node->son[1];
					node->son[1]->par = brNode;
				}
				else if (node->son[2]) {
					brNode->son[2] = node->son[2];
					node->son[2]->par = brNode;
				}

				delete node;
			}
			else if (brType == 0 && idIdx == 1 && brIdx == 2) {
				node->data[1] = parNode->data[1];
				node->data[2] = brNode->data[1];
				parNode->data[1] = Data();
				parNode->son[2] = nullptr;

				if (node->son[2]) node->son[1] = node->son[2];

				node->son[2] = brNode->son[1];
				if (brNode->son[1]) brNode->son[1]->par = node;

				node->son[3] = brNode->son[2];
				if (brNode->son[2]) brNode->son[2]->par = node;

				delete brNode;

				node = parNode;
				if (node->par) idx = 1;
				else idx = -1;
				changeIdx = true;
			}
			else if (brType == 0 && idIdx == 2 && brIdx == 1) {
				node->data[1] = parNode->data[1];
				node->data[0] = brNode->data[1];
				parNode->data[1] = Data();
				parNode->son[1] = nullptr;

				if (node->son[1]) node->son[2] = node->son[1];

				node->son[1] = brNode->son[2];
				if (brNode->son[2]) brNode->son[2]->par = node;

				node->son[0] = brNode->son[1];
				if (brNode->son[1]) brNode->son[1]->par = node;

				delete brNode;
				
				node = parNode;
				if (node->par) idx = 1;
				else idx = -2;
				changeIdx = true;
			}
			else if (brType == -1 && idIdx == 1) {
				brNode = node->par->son[2];
				brType = CalcType(brNode);
				if (brType == 1) {
					node->data[1] = parNode->data[1];
					parNode->data[1] = parNode->data[2];
					parNode->data[2] = Data();
					parNode->data[0] = brNode->data[0];
					brNode->data[0] = Data();

					parNode->son[0] = parNode->son[1];
					parNode->son[1] = parNode->son[2];
					parNode->son[2] = parNode->son[3];
					parNode->son[3] = nullptr;

					if (node->son[2]) node->son[1] = node->son[2];
					
					node->son[2] = brNode->son[0];
					if (brNode->son[0]) brNode->son[0]->par = node;
					brNode->son[0] = nullptr;
				}
				else if (brType == 2) {
					node->data[1] = parNode->data[1];
					parNode->data[1] = parNode->data[2];
					parNode->data[2] = Data();
					parNode->data[0] = brNode->data[1];
					brNode->data[1] = brNode->data[2];
					brNode->data[2] = Data();

					parNode->son[0] = parNode->son[1];
					parNode->son[1] = parNode->son[2];
					parNode->son[2] = parNode->son[3];
					parNode->son[3] = nullptr;

					if (node->son[2]) node->son[1] = node->son[2];
					
					node->son[2] = brNode->son[1];
					if (brNode->son[1]) brNode->son[1]->par = node;

					brNode->son[1] = brNode->son[2];
					brNode->son[2] = brNode->son[3];
					brNode->son[3] = nullptr;
				}
				else if (brType == 3) {
					node->data[1] = parNode->data[1];
					parNode->data[1] = parNode->data[2];
					parNode->data[2] = Data();
					parNode->data[0] = brNode->data[1];
					brNode->data[1] = brNode->data[2];
					brNode->data[2] = Data();
					node->data[2] = brNode->data[0];
					brNode->data[0] = Data();

					parNode->son[0] = parNode->son[1];
					parNode->son[1] = parNode->son[2];
					parNode->son[2] = parNode->son[3];
					parNode->son[3] = nullptr;

					if (node->son[2]) node->son[1] = node->son[2];
					
					node->son[2] = brNode->son[0];
					if (brNode->son[0]) brNode->son[0]->par = node;

					node->son[3] = brNode->son[1];
					if (brNode->son[1]) brNode->son[1]->par = node;
					
					brNode->son[0] = nullptr;
					brNode->son[1] = brNode->son[2];
					brNode->son[2] = brNode->son[3];
					brNode->son[3] = nullptr;
				}
				else {
					brNode->data[2] = brNode->data[1];
					brNode->data[1] = parNode->data[1];
					parNode->data[1] = parNode->data[2];
					parNode->data[2] = Data();

					parNode->son[1] = parNode->son[2];
					parNode->son[2] = parNode->son[3];
					parNode->son[3] = nullptr;

					brNode->son[3] = brNode->son[2];
					brNode->son[2] = brNode->son[1];
					if (node->son[2]) {
						brNode->son[1] = node->son[2];
						node->son[2]->par = brNode;
					}
					else if (node->son[1]) {
						brNode->son[1] = node->son[1];
						node->son[1]->par = brNode;
					}

					delete node;
				}
			}
			else if (brType == -1 && idIdx == 2) {
				brNode = node->par->son[1];
				brType = CalcType(brNode);
				if (brType == 1) {
					node->data[1] = parNode->data[1];
					parNode->data[1] = parNode->data[0];
					parNode->data[0] = Data();
					parNode->data[2] = brNode->data[1];
					brNode->data[1] = brNode->data[0];
					brNode->data[0] = Data();

					parNode->son[3] = parNode->son[2];
					parNode->son[2] = parNode->son[1];
					parNode->son[1] = parNode->son[0];
					parNode->son[0] = nullptr;

					if (node->son[1]) node->son[2] = node->son[1];
					
					node->son[1] = brNode->son[2];
					if (brNode->son[2]) brNode->son[2]->par = node;

					brNode->son[2] = brNode->son[1];
					brNode->son[1] = brNode->son[0];
					brNode->son[0] = nullptr;
				}
				else if (brType == 2) {
					node->data[1] = parNode->data[1];
					parNode->data[1] = parNode->data[0];
					parNode->data[0] = Data();
					parNode->data[2] = brNode->data[2];
					brNode->data[2] = Data();

					parNode->son[3] = parNode->son[2];
					parNode->son[2] = parNode->son[1];
					parNode->son[1] = parNode->son[0];
					parNode->son[0] = nullptr;

					if (node->son[1]) node->son[2] = node->son[1];
					
					node->son[1] = brNode->son[3];
					if (brNode->son[3]) brNode->son[3]->par = node;
					brNode->son[3] = nullptr;
				}
				else if (brType == 3) {
					node->data[1] = parNode->data[1];
					parNode->data[1] = parNode->data[0];
					parNode->data[0] = Data();
					parNode->data[2] = brNode->data[1];
					brNode->data[1] = brNode->data[0];
					brNode->data[0] = Data();
					node->data[0] = brNode->data[2];
					brNode->data[2] = Data();

					parNode->son[3] = parNode->son[2];
					parNode->son[2] = parNode->son[1];
					parNode->son[1] = parNode->son[0];
					parNode->son[0] = nullptr;

					if (node->son[1]) node->son[2] = node->son[1];
					
					node->son[1] = brNode->son[3];
					if (brNode->son[3]) brNode->son[3]->par = node;

					node->son[0] = brNode->son[2];
					if (brNode->son[2]) brNode->son[2]->par = node;

					brNode->son[3] = nullptr;
					brNode->son[2] = brNode->son[1];
					brNode->son[1] = brNode->son[0];
					brNode->son[0] = nullptr;
				}
				else {
					brNode->data[0] = brNode->data[1];
					brNode->data[1] = parNode->data[1];
					parNode->data[1] = parNode->data[0];
					parNode->data[0] = Data();

					parNode->son[2] = parNode->son[1];
					parNode->son[1] = parNode->son[0];
					parNode->son[0] = nullptr;

					brNode->son[0] = brNode->son[1];
					brNode->son[1] = brNode->son[2];
					if (node->son[1]) {
						brNode->son[2] = node->son[1];
						node->son[1]->par = brNode;
					}
					else if (node->son[2]) {
						brNode->son[2] = node->son[2];
						node->son[2]->par = brNode;
					}
					
					delete node;
				}
			}

		}

		if (changeIdx) changeIdx = false;
		else idx = -3;
	}

	return true;
}

void DeleteHighestPriority(Tree *&root) { // onaj najdesniji je najviseg prioriteta
	Tree *node = root, *prevNode = nullptr;
	int idx;
	while (node) {
		for (int i = 0; i < 3; i++) {
			if (node->data[i].name != "") {
				idx = i + 1;
			}
		}
		prevNode = node;
		node = node->son[idx];
	}
	string name = prevNode->data[idx - 1].name;
	int key = prevNode->data[idx - 1].key;
	DeleteNode(root, name, key);
	cout << "Akcija " << name << " sa prioritetom " << key << " je izbacena iz stabla.\n\n";
}

void DeleteAction(Tree *&root) {
	clear();
	cout << "Odaberite jednu od sledece dve stavke:\n";
	cout << "1. Brisanje akcije najviseg prioriteta.\n";
	cout << "2. Brisanje proizvoljne akcije.\n\n";
	int item;
	cin >> item;
	if (item < 1 || item > 2) {
		clear();
		cout << "Uneli ste nevalidnu vrednost.\n\n";
		return;
	}

	clear();
	if (item == 1) DeleteHighestPriority(root);
	else {
		cout << "Unesite ime i prioritet akcije koju zelite da obrisete: ";
		string name;
		int key;
		cin >> name >> key;
		if (DeleteNode(root, name, key)) cout << "Akcija je uspesno izbrisana.\n\n";
	}
}

int CalculateHeight(Tree *&root) {

	int height = -1;
	Tree *node = root;
	while (node) {
		height++;
		node = node->son[1]; // ako node nije list sigurno ima son[1] (i son[2]) jer sigurno ima data[1]
	}
	return height;

}

void PrintNames(Tree *&root, ostream &cout) {

	cout << "Imena kljuceva su data po level-order poretku crveno-crnog stabla:\n\n";

	Tree *node;
	int idx;
	queue<Tree *> qNode;
	queue<int> qIdx;

	int currCnt = 1, nextCnt = 0;
	qNode.push(root);
	qIdx.push(1);
	while (!qNode.empty()) {
		while (currCnt--) {
			node = qNode.front();
			qNode.pop();
			idx = qIdx.front();
			qIdx.pop();

			cout << node->data[idx] << endl;

			if (idx == 1) {
				if (node->data[0].name != "") {
					qNode.push(node);
					qIdx.push(0);
					nextCnt++;
				}
				else if (node->son[1]) {
					qNode.push(node->son[1]);
					qIdx.push(1);
					nextCnt++;
				}

				if (node->data[2].name != "") {
					qNode.push(node);
					qIdx.push(2);
					nextCnt++;
				}
				else if (node->son[2]) {
					qNode.push(node->son[2]);
					qIdx.push(1);
					nextCnt++;
				}
			}
			else if (idx == 0 || idx == 2) {
				if (node->son[idx]) {
					qNode.push(node->son[idx]);
					qIdx.push(1);
					nextCnt++;
				}

				if (node->son[idx + 1]) {
					qNode.push(node->son[idx + 1]);
					qIdx.push(1);
					nextCnt++;
				}
			}
		}

		currCnt = nextCnt;
		nextCnt = 0;
		cout << endl;
	}
}

void PrintSpace(int blank, ostream &cout) {
	cout << setfill(' ') << setw(blank) << "";
}

void PrintKey(Tree *&node, ostream &cout) {

	cout << "[";
	if (node && node->data[0].name != "") cout << setfill(' ') << setw(4) << node->data[0].key << ",";
	else cout << setfill(' ') << setw(4) << "" << ",";
	if (node && node->data[1].name != "") cout << setfill(' ') << setw(4) << node->data[1].key << ",";
	else cout << setfill(' ') << setw(4) << "" << ",";
	if (node && node->data[2].name != "") cout << setfill(' ') << setw(4) << node->data[2].key << "]";
	else cout << setfill(' ') << setw(4) << "" << "]";

}

void Print234Tree(Tree *&root, ostream &cout) {

	// IMPROVE
	cout << "2-3-4 stablo izgleda na sledeci nacin:\n\n";

	int height = CalculateHeight(root);
	int cellWidth = 16; // [....|....|....]
	int width = (cellWidth + 4) * power(4, height);

	int level = 0, blankNum, blank, nodesNum = 1;
	Tree *node;
	queue<Tree *> q;
	q.push(root);
	while (level <= height) {
		blankNum = nodesNum + 1; // number of blank fields
		blank = (width - nodesNum * cellWidth) / blankNum;
		for (int i = 0; i < nodesNum; i++) {
			node = q.front();
			q.pop();
			PrintSpace(blank, cout);
			PrintKey(node, cout);
			if (node) for (int j = 0; j < 4; j++) q.push(node->son[j]);
		}
		PrintSpace(blank, cout);
		cout << "\n";
		level++;
		nodesNum *= 4;
	}

	while (!q.empty()) q.pop();
}

void PrintRedBlackTree(Tree *&root, ostream &cout) {
	
	cout << "Crveno-crno stablo izgleda na sledeci nacin:\n\n";

	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	
	int height = (CalculateHeight(root) + 1) * 2;
	Tree *node = root;
	int keyWidth = 4;
	int idx, pos, currCnt, nextCnt;

	int *branches = new int[height];
	int *distance = new int[height];
	int *children;
	branches[0] = 0;
	for (int i = 1; i < height; i++) branches[i] = 2 * (branches[i - 1] + 1);
	distance[0] = 4;
	for (int i = 1; i < height; i++) distance[i] = 2 * distance[i - 1];

	queue<Tree *> qNode;
	queue<int> qIdx;
	qNode.push(node);
	qIdx.push(1);
	children = new int;
	pos = nextCnt = 0;
	currCnt = 1;
	height--;

	while (!qNode.empty()) {
		node = qNode.front();
		qNode.pop();
		idx = qIdx.front();
		qIdx.pop();
		currCnt--;
		nextCnt += 2;
		children[pos] = 0; // 0 - nema sinova, 1 - samo levi sin, 2 - samo desni sin, 3 - oba sina

		if (nextCnt == 2) cout << setfill(' ') << setw(branches[height]) << "";

		if (!node) {
			if (height > 0) {
				for (int i = 0; i < 2; i++) {
					qNode.push(nullptr);
					qIdx.push(-1);
				}
			}
			cout << setfill(' ') << setw(2 * branches[height] + keyWidth) << "";
		}
		else if (idx == 1) {
			if (node->data[0].name != "") {
				cout << setfill('_') << setw(branches[height]) << "";
				qNode.push(node);
				qIdx.push(0);
				children[pos] += 1;
			}
			else if (node->son[1]) {
				cout << setfill('_') << setw(branches[height]) << "";
				qNode.push(node->son[1]);
				qIdx.push(1);
				children[pos] += 1;
			}
			else {
				cout << setfill(' ') << setw(branches[height]) << "";
				qNode.push(nullptr);
				qIdx.push(-1);
			}

			SetConsoleTextAttribute(hCon, 1);
			cout << setfill(' ') << setw(keyWidth) << node->data[idx].key;
			SetConsoleTextAttribute(hCon, 15);

			if (node->data[2].name != "") {
				cout << setfill('_') << setw(branches[height]) << "";
				qNode.push(node);
				qIdx.push(2);
				children[pos] += 2;
			}
			else if (node->son[2]) {
				cout << setfill('_') << setw(branches[height]) << "";
				qNode.push(node->son[2]);
				qIdx.push(1);
				children[pos] += 2;
			}
			else {
				cout << setfill(' ') << setw(branches[height]) << "";
				qNode.push(nullptr);
				qIdx.push(-1);
			}
		}
		else if (idx == 0) {
			if (node->son[0]) {
				cout << setfill('_') << setw(branches[height]) << "";
				qNode.push(node->son[0]);
				qIdx.push(1);
				children[pos] += 1;
			}
			else {
				cout << setfill(' ') << setw(branches[height]) << "";
				qNode.push(nullptr);
				qIdx.push(-1);
			}

			SetConsoleTextAttribute(hCon, 4);
			cout << setfill(' ') << setw(keyWidth) << node->data[idx].key;
			SetConsoleTextAttribute(hCon, 15);

			if (node->son[1]) {
				cout << setfill('_') << setw(branches[height]) << "";
				qNode.push(node->son[1]);
				qIdx.push(1);
				children[pos] += 2;
			}
			else {
				cout << setfill(' ') << setw(branches[height]) << "";
				qNode.push(nullptr);
				qIdx.push(-1);
			}
		}
		else {
			if (node->son[2]) {
				cout << setfill('_') << setw(branches[height]) << "";
				qNode.push(node->son[2]);
				qIdx.push(1);
				children[pos] += 1;
			}
			else {
				cout << setfill(' ') << setw(branches[height]) << "";
				qNode.push(nullptr);
				qIdx.push(-1);
			}

			SetConsoleTextAttribute(hCon, 4);
			cout << setfill(' ') << setw(keyWidth) << node->data[idx].key;
			SetConsoleTextAttribute(hCon, 15);

			if (node->son[3]) {
				cout << setfill('_') << setw(branches[height]) << "";
				qNode.push(node->son[3]);
				qIdx.push(1);
				children[pos] += 2;
			}
			else {
				cout << setfill(' ') << setw(branches[height]) << "";
				qNode.push(nullptr);
				qIdx.push(-1);
			}
		}

		if (currCnt > 0) {
			cout << setfill(' ') << setw(distance[height]) << "";
			pos++;
		}
		else {
			cout << setfill(' ') << setw(branches[height]) << "" << "\n";

			if (!qNode.empty()) {
				for (int i = 0; i <= pos; i++) {
					if (i == 0) cout << setfill(' ') << setw(branches[height]) << "";

					if (children[i] == 1 || children[i] == 3) cout << "|" << setfill(' ') << setw(branches[height] - 1) << "";
					else cout << setfill(' ') << setw(branches[height]) << "";

					cout << setfill(' ') << setw(keyWidth) << "";

					if (children[i] == 2 || children[i] == 3) cout << setfill(' ') << setw(branches[height] - 1) << "" << "|";
					else cout << setfill(' ') << setw(branches[height]) << "";

					if (i < pos) cout << setfill(' ') << setw(distance[height]) << "";
					else cout << setfill(' ') << setw(branches[height]) << "";
				}
				cout << "\n";
			}

			currCnt = nextCnt;
			nextCnt = 0;
			height--;
			pos = 0;
			delete[]children;
			children = new int[currCnt];
		}
	}

	delete[]branches;
	delete[]distance;
}

void PrintTree(Tree *&root) {
	clear();
	cout << root;
}

void ChangePriority(Tree *&root) {
	clear();
	cout << "Unesite ime akcije, njen trenutni prioritet i potom novi zeljeni prioritet: ";
	string name;
	int oldKey, key;
	cin >> name >> oldKey >> key;
	
	Tree *noTree;
	int noIdx;
	if (!SearchTree(root, name, oldKey, noTree, noIdx)) cout << "Akcija ciji prioritet zelite da promenite ne postoji.\n\n";
	else if (SearchTree(root, name, key, noTree, noIdx)) cout << "Promena nije moguca jer vec postoji akcija istog imena sa zadatim novim prioritetom.\n\n";
	else {
		DeleteNode(root, name, oldKey);
		InsertNode(root, name, key);
		cout << "Akcija je uspesno promenila svoj prioritet.\n\n";
	}
}

int TraverseKey(Tree *&root, int key) {

	int cnt = 0;
	if (!root) return cnt;
	
	stack<Tree *> st;
	st.push(root);
	while (!st.empty()) {
		Tree *node = st.top();
		st.pop();

		int idx;
		for (int i = 0;i < 3;i++) {
			if (node->data[i].name == "") continue;
			
			idx = i + 1;			
			cnt += node->data[i].key == key;
			if (node->data[i].key >= key && (i == 0 || node->data[i-1].key <= key) && node->son[i] != nullptr) st.push(node->son[i]);
		}

		if (node->data[idx - 1].key <= key && node->son[idx] != nullptr) st.push(node->son[idx]);
	}

	return cnt;

}

void FindNumber(Tree *&root) {
	clear();
	cout << "Unesite prioritet: ";
	int key;
	cin >> key;
	cout << "Broj akcija u stablu sa vrednoscu kljuca " << key << " ima: " << TraverseKey(root, key) << "\n\n";
}

void DealocateTree(Tree *&root) {
	if (!root) return;
	stack<Tree *> st;
	st.push(root);
	while (!st.empty()) {
		Tree *node = st.top();
		st.pop();
		for (int i = 0; i < 4; i++) {
			if (node->son[i]) {
				st.push(node->son[i]);
			}
		}
		delete node;
	}
	root = nullptr;
	clear();
	cout << "Stablo je uspesno dealocriano.\n\n";
}

int main() {

	Tree *root = nullptr;

	cout << "Dobrodosli!\n\n";
	while (true) {
		int item = MainMenu(root);

		if (item == 1) BuildTree(root);
		else if (item == 2) PrintTree(root);
		else if (item == 3) InsertAction(root);
		else if (item == 4) DeleteAction(root);
		else if (item == 5) ChangePriority(root);
		else if (item == 6) FindNumber(root);
		else if (item == 7) DealocateTree(root);
		else if (item == 8) {
			DealocateTree(root);
			break;
		}
	}
	clear();
	cout << "Dovidjenja!\n";

	return 0;

}
