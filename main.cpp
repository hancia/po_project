#include <iostream>
#include <list>
#include <fstream>
using namespace std;

class A;

class Object{
public:
    string o_name, content;
    list <A> o_parents;
    Object(){o_name="";content="";};
    Object(string a){o_name=a;content="";}
    Object(string a, string b): o_name(a),content(b){}
};

class Leaf{
public:
    bool visited;
    list<Object*> objects;
    Object* mo(string o_name,string o_content) //funkcja dodajaca nowy element do biezacego liscia
    {
        int flag=0;
        for(auto it=objects.begin();it!=objects.end(); it++) //przeszukuje lisc w poszukiwaniu juz istniejacego obiektu o tej nazwie
            if((*it)->o_name==o_name)
            {
                flag=1;
                break;
            }
        if(flag==0) //jesli nie znalazl obiektu, dodaje go
        {
            Object *neww= new Object(o_name,o_content);
            this->objects.push_back(neww);
            return neww;
        }
        else cout<<"Obiekt o podanej nazwie juz instnieje";
    }
    void DO(string o_name)//funkcja usuwajaca obiekt z biezacego liscia
    {
        for(auto it=objects.begin(); it!=objects.end();it++)
        {
            if((*it)->o_name==o_name) //jezeli znajdzie obiekt o tej nazwie, usuwa go
            {
                objects.remove((*it));
                return;
            }
        }
        cout<<"Nie znaleziono takiego elementu";
    }
    Object* mdo(string o_name)//funkcja modyfikujaca obiekt o podanej nazwie
    {
        string new_cont,new_name;
        int temp;
        for (auto &object : objects)
            if(object->o_name==o_name)//jesli znajdzie ten obiekt, prosi o podanie nowej nazwy i zawartosci i zamienia je
            {
                cout<<"Nowa nazwa: "; cin>>new_name;
                cout<<"Nowa zawartosc: "; cin>>new_cont;
                cout<<"Podaj nowe parametry obiektu: "<<endl;
                object->o_name=new_name;
                object->content=new_cont;
                return object;
            }
        cout<<"Nie znaleziono takiego elementu";
    }
    void dir()//wyswietla wszystkie obiekty dla danego liscia
    {
        if(!this->visited){
            for (auto &object : objects) {
                cout<<(object)->o_name<<endl;
            }
            this->visited=1;
        }
    }
    Leaf(){};
};
class A{
protected:
    string name;
    string id_number;
    A* find_node(A* T[],string node_name) //szuka wskaznika do podanego liscia w tablicy wszystkich wskaznikow
    {
        for(int i=0; i<12; i++)
            if(T[i]->name==node_name) return T[i];
        cout<<"nie ma takiego wezla"<<endl;
        return NULL;
    }
public:
    list<A*> parents;
    list<A*> children;
    void push_parents(Object* O)//dodaje rodzicow do obiektu
    {
        if(this->children.empty()) {
            O->o_parents.push_back(*this);
        }
        for(auto it:this->parents)
        {
            O->o_parents.push_back(*it);
        }
        if(!this->parents.empty())
            this->parents.front()->push_parents(O);
    }
    void edit_parents(Object *O) //funkcja edytujaca rodzicow obiektu
    {
        string temp;
        for(auto &it:O->o_parents) {
            cout << it.name << " " << it.id_number << " Wprowadz wartosc: ";
            cin>>temp;
            it.id_number=temp;
        }
    }
    A* cd(string node_name, A *root, A* curr) //sluzy do zmiany biezacego wezla
    {
        A* temp=NULL;
        if (node_name == this->name) return this; //jesli jestes w podanym wezle
        if (curr->name > node_name) {//jesli szukasz wezla wyzej w drzewie
            for (auto &parent : this->parents)//przeglada liste rodzicow
                if(parent->name==node_name) {return parent;} //jesli znajdzie na liscie rodzicow
            for (auto &parent : this->parents) {//jesli nie znajdzie, wywoluje funkcje rekurencyjnie od kazdego rodzica
                temp=parent->cd(node_name,root,curr);
                if (temp->name == node_name) { return temp; }
            }
        } else {//jezeli szukasz wezla nizej w drzewie
            for (auto &it : this->children)//przeglada liste dzieci
                if(it->name==node_name) {return it;}//jezeli znajdzie na liscie dzieci
            for (auto &it : this->children) {//jezeli nie, wywoluje funkcje rekurencyjnie od kazdego dziecka
                temp=it->cd(node_name,root,curr);
                if (temp->name == node_name) { return temp; }
            }
        }
        return this;//jesli nigdzie wczesniej nie zwroci wyniku, zwraca wezel, w ktorym bylismy na poczatku
    }
    void show(string o_name, Leaf* nowy,A* root)//funkcja pokazujaca obiekt o podanej nazwie
    {
        string parent;
        if(this->children.empty())//jezeli jestesmy w lisciu
        {
            nowy = dynamic_cast<Leaf*>(this);
            for (auto &object : nowy->objects) {//przeglada obiekty liscia
                if(object->o_name==o_name)//jezeli znajdzie obiekt na liscie, wyswietla go
                {
                    if(nowy->visited!=1){
                        cout<<"Nazwa obiektu: "<< object->o_name<<endl;
                        cout<<"Zawartosc: "<< object->content<<endl;
                        for(auto p:object->o_parents)
                        {
                            cout<<p.name<<" "<<p.id_number<<endl;
                        }
                        nowy->visited=1;
                    }
                }
            }
        }
        else
        {
            for (auto &it : this->children) {//jezeli nie jestesmy w lisciu, przegladamy cala liste dzieci
                if(it->children.empty()) {//jezeli trafimy na lisc, przeszukuje liste obiektow
                    nowy = dynamic_cast<Leaf *>(it);
                    for (auto &object : nowy->objects) {//przeglada obiekty liscia
                        if(object->o_name==o_name)//jezeli znajdzie obiekt na liscie, wyswietla go
                        {
                            if(nowy->visited!=1){
                                cout<<"Nazwa obiektu: "<< object->o_name<<endl;
                                cout<<"Zawartosc: "<< object->content<<endl;
                                for(auto p:object->o_parents)
                                {
                                    cout<<p.name<<" "<<p.id_number<<endl;
                                }
                                nowy->visited=1;
                            }
                        }
                    }
                }
                else {//jezeli nie, wywoluje funkcje od wezla
                    it->show(o_name,nowy,root);
                }
            }
        }
    }
    void print(Leaf* nowy)//wyswietla obiekty widoczne z danego wezla
    {
        if(this->children.empty())//jezeli jestesmy w lisciu, wywoluje funkcje wyswietlajaca wszystkie obiekty w lisciu
        {
            nowy = dynamic_cast<Leaf*>(this);
            nowy->dir();
        }
        else//jezeli nie, przeglada dzieci
        {
            for (auto &it : this->children) {//jezeli trafi na lisc, wywoluje funkcje dir, jezeli nie, wywoluje print od wezla
                if(it->children.empty())
                {
                    nowy = dynamic_cast<Leaf*>(it);
                    nowy->dir();
                }
                else it->print(nowy);
            }
        }
    }
    void tree(int t)//wyswietla strukture
    {
        for(int i=0;i<t;i++)//wyswietla wymagana liczbe spacji
            cout<<" ";
        cout<<this->name<<endl;
        for (auto &it : this->children) {//wywoluje sie od wszystkich dzieci, powielajac liczbe spacji o 1, im glebiej w rekurencji
            it->tree(t+1);
        }
    }
    void save(A* T[], Leaf* nowy)//zapisywanie do pliku
    {
        ofstream plik;
        plik.open("zbior.txt");
        for(int i=0; i<12; i++)//dla wszystkich wezlow drzewa
        {
            if(T[i]->children.empty())//jezeli jestesmy w lisciu
            {
                plik<<"Lisc "<<T[i]->name<<endl;//wypisuje nazwe liscia
                nowy = dynamic_cast<Leaf*>(T[i]);
                for (auto &object : nowy->objects)//wypisuje wszystkie obiekty liscia
                {
                    plik<< object->o_name<<" "<<object->content<<endl;
                    for(auto it:object->o_parents)
                        plik<<it.name<<" "<<it.id_number<<endl;
                }
            }
        }
        plik.close();
    }
    void read(A* T[],Leaf* nowy)//odczytuje z pliku
    {
        ifstream plik;
        string s1, s2;
        A* curr;
        Object* temp;
        plik.open("zbior.txt");
        while(!plik.eof())//pobiera po linii pliku, dzieli na dwa slowa, dopoki plik sie nie skonczy
        {
            string s;
            s1="",s2="";
            getline(plik,s);
            unsigned int i=0;
            for(i=0; i<s.length(); i++)
                if(s[i]!=' ') s1+=s[i];
                else break;
            while(i<s.length())
            {
                if(s[i]!=' ')
                    s2+=s[i];
                i++;
            }
            if(s1=="Lisc") {//jesli pierwszym slowem jest lisc, szuka wezla o podanej nazwie
                curr = find_node(T, s2);
            }
            else//jezeli nie, dodaje nowy obiekt do znalezionego wczesniej liscia
            {
                if(s1=="a"||s1=="b"||s1=="c"||s1=="d"||s1=="e"||s1=="f"||s1=="g"||s1=="h"||s1=="i"||s1=="j"||s1=="k"||s1=="l")
                    for(auto &it:temp->o_parents) {
                        if(it.name==s1) it.id_number=s2;
                    }
                else
                    {
                        nowy = dynamic_cast<Leaf*>(curr);
                        if(s2!="") {
                            temp=nowy->mo(s1, s2);
                            curr->push_parents(temp);
                        }
                    }
            }
        }
        plik.close();
    }
    void reset(Leaf* nowy)//funkcja sluzaca do resetowania flagi visited dla przechodzenia przez strukture
    {
        if(this->children.empty())
        {
            nowy = dynamic_cast<Leaf*>(this);
            nowy->visited=0;
        }
        else
        {
            for (auto &it : this->children) {
                if(it->children.empty())
                {
                    nowy = dynamic_cast<Leaf*>(it);
                    nowy->visited=0;
                }
                else it->reset(nowy);
            }
        }
    }
    A(){name="a";id_number="1";}
    virtual ~A() {}
};
class B:public A{
public:
    B(){name="b";id_number="2";};
    B(A *a){name="b";parents.push_front(a);id_number="2";};
};

class C:public A{
public:
    C(){name="c";id_number="3";};
    C(A *a){name="c";parents.push_front(a);id_number="3";};
};

class D:public A{
public:
    D(){name="d";id_number="4";};
    D(A *a){name="d";parents.push_front(a);id_number="4";};
};

class E:public B, public Leaf{
public:
    E(){name="e";id_number="5";};
    E(A *a){name="e";parents.push_front(a);id_number="5";};
};
class F:public B, public Leaf{
public:
    F(){name="f";id_number="6";};
    F(A *a){name="f";parents.push_front(a);id_number="6";};
};
class G:public C, public Leaf{
public:
    G(){name="g";id_number="7";};
    G(A *a){name="g";parents.push_front(a);id_number="7";};
};
class H:public C, public Leaf{
public:
    H(){name="h";id_number="8";};
    H(A *a){name="h";parents.push_front(a);id_number="8";};
};
class I:public virtual D{
public:
    I(){name="i";id_number="9";};
    I(A *a){name="i";parents.push_front(a);id_number="9";};
};
class J:public virtual D{
public:
    J(){name="j";id_number="10";};
    J(A *a){name="j";parents.push_front(a);id_number="10";};
};
class K:public I, public Leaf{
public:
    K(){name="k";id_number="11";};
    K(A *a){name="k";parents.push_front(a);id_number="11";};
};
class L:public I, public J, public Leaf{
public:
    L(){name="l";id_number="12";};
    L(A *a, A *b){name="l";parents.push_front(a);parents.push_front(b);id_number="12";};

};
void input(string commands[],bool flag)
{
    if(flag==1)
    cout<<"wprowadz komende: ";
    string s1,s2,s;
    getline(cin,s);
    unsigned int i=0;
    for(i=0; i<s.length(); i++)
        if(s[i]!=' ') s1+=s[i];
        else break;
    while(i<s.length())
    {
        if(s[i]!=' ')
            s2+=s[i];
        i++;
    }
    commands[0]=s1;
    commands[1]=s2;
}

int main()
{
    A* T[12];
    A a;
    B b(&a);
    C c(&a);
    D d(&a);
    E e(&b);
    F f(&b);
    G g(&c);
    H h(&c);
    I i(&d);
    J j(&d);
    K k(&i);
    L l(&i,&j);
    T[0]=&a;
    T[1]=&b;
    T[2]=&c;
    T[3]=&d;
    T[4]=&e;
    T[5]=&f;
    T[6]=&g;
    T[7]=&h;
    T[8]=&i;
    T[9]=&j;
    T[10]=&k;
    T[11]=&l;
    a.children.push_front(&b);
    a.children.push_front(&c);
    a.children.push_front(&d);
    b.children.push_front(&e);
    b.children.push_front(&f);
    c.children.push_front(&g);
    c.children.push_front(&h);
    d.children.push_front(&i);
    d.children.push_front(&j);
    i.children.push_front(&k);
    i.children.push_front(&l);
    j.children.push_front(&l);
    A* curr;
    A* temp;
    curr = new A;
    temp=NULL;
    curr = &a;
    Leaf* nowy = new Leaf;
    Object *pom;
    int flag=1;
    curr->reset(nowy);

    string commands[2];
    do
    {
        input(commands,flag);
        if(commands[0]!=""){
            if(commands[0]=="cd") {
                temp = curr->cd(commands[1], &a, curr);
                if(temp!=curr) curr=temp;
                else
                    cout<<"Nieprawidlowa nazwa"<<endl;
            }
            else{
                if(commands[0]=="tree")
                    (&a)->tree(0);
                else{
                    if(commands[0]=="mo")
                        if(curr->children.empty())
                        {
                            string o_cont;
                            cout<<"Podaj zawartosc obiektu: ";
                            cin>>o_cont;
                            nowy = dynamic_cast<Leaf*>(curr);
                            pom=nowy->mo(commands[1],o_cont);
                            if(pom) {
                                curr->push_parents(pom);
                                curr->edit_parents(pom);
                            }

                        }
                        else cout<<"Nie jestes w lisciu"<<endl;
                    else{
                        if(commands[0]=="do")
                            if(curr->children.empty())
                            {
                                nowy = dynamic_cast<Leaf*>(curr);
                                nowy->DO(commands[1]);
                            }
                            else cout<<"Nie jestes w lisciu"<<endl;
                        else{
                            if(commands[0]=="mdo")
                                if(curr->children.empty())
                                {
                                    nowy = dynamic_cast<Leaf*>(curr);
                                    pom=nowy->mdo(commands[1]);
                                    if(pom) curr->edit_parents(pom);
                                }
                                else cout<<"Nie jestes w lisciu"<<endl;
                            else{
                                if(commands[0]=="dir")
                                {
                                    a.reset(nowy);
                                    curr->print(nowy);
                                }
                                else{
                                    if(commands[0]=="show")
                                    {
                                        a.reset(nowy);
                                        a.show(commands[1],nowy,&a);
                                    }
                                    else{
                                        if(commands[0]=="save")
                                        {
                                            curr->save(T,nowy);
                                        }
                                        else{
                                        if(commands[0]=="read")
                                        {
                                            curr->read(T,nowy);
                                        }
                                        else if(commands[0]!="exit")cout<<"Nieprawidlowa komenda"<<endl;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            flag=1;
        }
        else flag=0;
    }while(commands[0]!="exit");
    return 0;
}
