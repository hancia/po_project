#include <iostream>
#include <list>
#include <fstream>
using namespace std;

class Object{
public:
    string name, content;
    Object(string a){name=a,content="";}
    Object(string a, string b): name(a),content(b){}
};

class Leaf{
public:
    list<Object*> objects;
    void mo(string o_name,string o_content) //funkcja dodajaca nowy element do biezacego liscia
    {
        int flag=0;
        for(auto it=objects.begin();it!=objects.end(); it++) //przeszukuje lisc w poszukiwaniu juz istniejacego obiektu o tej nazwie
            if((*it)->name==o_name)
            {
                flag=1;
                break;
            }
        if(flag==0) //jesli nie znalazl obiektu, dodaje go
        {
            Object *neww= new Object(o_name,o_content);
            this->objects.push_back(neww);
        }
        else cout<<"Obiekt o podanej nazwie juz instnieje"<<endl;
    }
    void DO(string o_name)//funkcja usuwajaca obiekt z biezacego liscia
    {
        for(auto it=objects.begin(); it!=objects.end();it++)
        {
            if((*it)->name==o_name) //jezeli znajdzie obiekt o tej nazwie, usuwa go
            {
                objects.remove((*it));
                return;
            }
        }
        cout<<"Nie znaleziono takiego elementu";
    }
    void mdo(string o_name)//funkcja modyfikujaca obiekt o podanej nazwie
    {
        string new_cont,new_name;
        for (auto &object : objects)
            if(object->name==o_name)//jesli znajdzie ten obiekt, prosi o podanie nowej nazwy i zawartosci i zamienia je
            {
                cout<<endl<<"Nowa nazwa: "; cin>>new_name;
                cout<<endl<<"Nowa zawartosc: "; cin>>new_cont;
                object->name=new_name;
                object->content=new_cont;
                return;
            }
        cout<<"Nie znaleziono takiego elementu";
    }
    void dir()//wyswietla wszystkie obiekty dla danego liscia
    {
        for (auto &object : objects) {
            cout<<(object)->name<<endl;
        }
    }
    Leaf(){};
};
class A{
private:
    int id;
protected:
    string name;
public:
    list<A*> parents;
    list<A*> children;
    A* find_node(A* T[],string node_name) //szuka wskaznika do podanego liscia w tablicy wszystkich wskaznikow
    {
        for(int i=0; i<12; i++)
            if(T[i]->name==node_name) return T[i];
        cout<<"nie ma takiego wezla"<<endl;
        return NULL;
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
    void show_parents()//przeglada i wyswietla wszystkich rodzicow
    {
        for(auto &it:this->parents)
        {
            if(this->children.empty()) cout<<this->name<<",";
            cout<<it->name<<",";
            it->show_parents();
            cout<<endl;
        }
    }
    void show(string o_name, Leaf* nowy,A* root)//funkcja pokazujaca obiekt o podanej nazwie
    {
        string parent;
        if(this->children.empty())//jezeli jestesmy w lisciu
        {
            nowy = dynamic_cast<Leaf*>(this);
            for (auto &object : nowy->objects) {//przeglada obiekty liscia
                if(object->name==o_name)//jezeli znajdzie obiekt na liscie, wyswietla go
                {
                    cout<<"Nazwa obiektu: "<< object->name<<endl;
                    cout<<"Zawartosc: "<< object->content<<endl;
                    this->show_parents();
                    cout<<endl;
                }
            }
        }
        else
        {
            for (auto &it : this->children) {//jezeli nie jestesmy w lisciu, przegladamy cala liste dzieci
                if(it->children.empty()) {//jezeli trafimy na lisc, przeszukuje liste obiektow
                    nowy = dynamic_cast<Leaf *>(it);
                    for (auto &object : nowy->objects) {
                        if (object->name == o_name) {
                            cout << "Nazwa obiektu: " << object->name << endl;
                            cout << "Zawartosc: " << object->content << endl;
                            it->show_parents();
                            cout<<endl;
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
                    plik<< object->name<<" ";
                    plik<<object->content<<endl;
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
                nowy = dynamic_cast<Leaf*>(curr);
                if(s2!="")
                nowy->mo(s1,s2);
            }
        }
        plik.close();
    }
    A(){name="a";}
    virtual ~A() {}
};
class B:public A{
private:
    int id;
public:
    B(){name="b";};
    B(A *a){name="b";parents.push_front(a);};
};

class C:public A{
private:
    int id;
public:
    C(){name="c";};
    C(A *a){name="c";parents.push_front(a);};
};

class D:public A{
private:
    int id;
public:
    D(){name="d";};
    D(A *a){name="d";parents.push_front(a);};
};

class E:public B, public Leaf{
private:
    int id;
public:
    E(){name="e";};
    E(A *a){name="e";parents.push_front(a);};
};
class F:public B, public Leaf{
private:
    int id;
public:
    F(){name="f";};
    F(A *a){name="f";parents.push_front(a);};
};
class G:public C, public Leaf{
private:
    int id;
public:
    G(){name="g";};
    G(A *a){name="g";parents.push_front(a);};
};
class H:public C, public Leaf{
private:
    int id;
public:
    H(){name="h";};
    H(A *a){name="h";parents.push_front(a);};
};
class I:public virtual D{
private:
    int id;
public:
    I(){name="i";};
    I(A *a){name="i";parents.push_front(a);};
};
class J:public virtual D{
private:
    int id;
public:
    J(){name="j";};
    J(A *a){name="j";parents.push_front(a);};
};
class K:public I, public Leaf{
private:
    int id;
public:
    K(){name="k";};
    K(A *a){name="k";parents.push_front(a);};
};
class L:public I, public J, public Leaf{
private:
    int id;
public:
    L(){name="l";};
    L(A *a, A *b){name="l";parents.push_front(a);parents.push_front(b);};

};
void input(string commands[])
{
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

    string commands[2];
    do
    {
        cout<<"wprowadz komende: ";
        input(commands);
        if(commands[0]=="cd") {
            temp = curr->cd(commands[1], &a, curr);
            if(temp!=curr) curr=temp;
            else
                cout<<"Nieprawidlowa nazwa"<<endl;
        }
        if(commands[0]=="tree")
            (&a)->tree(0);
        if(commands[0]=="mo")
            if(curr->children.empty())
            {
                string o_cont;
                cout<<"Podaj zawartosc obiektu: ";
                cin>>o_cont;
                nowy = dynamic_cast<Leaf*>(curr);
                nowy->mo(commands[1],o_cont);
            }
        if(commands[0]=="do")
            if(curr->children.empty())
            {
                nowy = dynamic_cast<Leaf*>(curr);
                nowy->DO(commands[1]);
            }
        if(commands[0]=="mdo")
            if(curr->children.empty())
            {
                nowy = dynamic_cast<Leaf*>(curr);
                nowy->mdo(commands[1]);
            }
        if(commands[0]=="dir")
        {
            curr->print(nowy);
        }
        if(commands[0]=="show")
        {
            curr->show(commands[1],nowy,&a);
        }
        if(commands[0]=="save")
        {
            curr->save(T,nowy);
        }
        if(commands[0]=="read")
        {
            curr->read(T,nowy);
        }
    }while(commands[0]!="exit");
    return 0;
}
