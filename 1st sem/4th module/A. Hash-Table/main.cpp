#include <iostream>
#include <iostream>
#include <vector>

using namespace std;

struct Node{
    string value;
    bool deleted;
};

class HashTable{
public:
    HashTable(int sz):
        data(sz, nullptr),
        numberOfElements(0)
    {}

    bool Add(const string&);
    bool Check(const string&);
    bool Delete(const string&);
private:
    vector<Node*> data;
    static const int a1=17;
    static const int a2=11;
    static constexpr const double occupancy=0.75;
    int numberOfElements;
    void Reset();
    int GetHashKeys1(const string&);
    int GetHashKeys2(const string&);
    void HashFunction(const string&, int&, int&);
    void ReHash();
};

int HashTable::GetHashKeys1(const string& value){
    int key=0;
    for(char ch: value){
        key=(key*a1+(int)ch)%data.size();
    }
    return key;
}

int HashTable::GetHashKeys2(const string& value){
    int key=0;
    for(char ch: value){
        key=(key*a2+(int)ch)%data.size();
    }
    return key;
}

void HashTable::HashFunction(const string& value, int& key1, int& key2){
    key1=GetHashKeys1(value);
    key2=GetHashKeys2(value);
    if (key2%2==0){
        ++key2;
    }
}

void HashTable::ReHash(){
    vector<Node*> buffer=data;
    data.assign(data.size()*2, nullptr);
    numberOfElements=0;
    for (const Node* st: buffer){
        if (st!=nullptr && st->deleted==false){
            Add(st->value);
            delete st;
        }
    }
}

bool HashTable::Add(const string& value){
    if ((double)numberOfElements/(double)data.size()>=occupancy){
       ReHash();
    }
    int key1, key2;
    HashFunction(value, key1, key2);
    int index;
    for (size_t i=0; i<data.size(); ++i){
        int key=(key1+i*key2)%data.size();
        if (data[key]!=nullptr && data[key]->value==value && data[key]->deleted==false){
            return false;
        }
        if(data[key]==nullptr || data[key]->deleted==true){
            index=key;
            if (data[key]==nullptr){
                break;
            }
        }
    }
    if (data[index]==nullptr){
        data[index]=new Node;
    }
    data[index]->value=value;
    data[index]->deleted=false;
    numberOfElements++;
    return true;
}

bool HashTable::Check(const string& value){
    int key1, key2;
    HashFunction(value, key1, key2);
    for (size_t i=0; i<data.size(); ++i){
        int key=(key1+i*key2)%data.size();
        if (data[key]==nullptr){
            return false;
        }
        if (data[key]!=nullptr && data[key]->value==value && data[key]->deleted==false){
            return true;
        }
    }
    return false;
}

bool HashTable::Delete(const string& value){
    int key1, key2;
    HashFunction(value, key1, key2);
    for (size_t i=0; i<data.size(); ++i){
        int key=(key1+i*key2)%data.size();
        if (data[key]==nullptr){
            return false;
        }
        if (data[key]!=nullptr && data[key]->value==value && data[key]->deleted==false){
            data[key]->deleted=true;
            numberOfElements--;
            return true;
        }
    }
    return false;
}

int main()
{
    char command;
    string value;
    HashTable Hash(8);
    while (!cin.eof()){
    	cin>>command>>value;
        if (command=='+'){
            if(Hash.Add(value)){
                cout<<"OK";
            } else {
                cout<<"FAIL";
            }
        }
        if (command=='?'){
            if(Hash.Check(value)){
                cout<<"OK";
            } else {
                cout<<"FAIL";
            }
        }
        if (command=='-'){
            if(Hash.Delete(value)){
                cout<<"OK";
            } else {
                cout<<"FAIL";
            }
        }
    }
    return 0;
}
