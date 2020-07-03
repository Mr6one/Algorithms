#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <list>
#include <map>
#include "Huffman.h"

using namespace std;

typedef bool bit;

class ByteEncode{
public:
    ByteEncode(IOutputStream& compressed):
        stream(compressed), BitsWritten(0)
    {}
    void write(bit value);
private:
    IOutputStream& stream;
    byte CurrentByte;
    int BitsWritten;
};

class ByteDecode{
public:
    ByteDecode(IInputStream& compressed):
        stream(compressed), BitsRead(0)
    {}
    void read(bit& value);
private:
    IInputStream& stream;
    byte CurrentByte;
    int BitsRead;
};

void ByteDecode::read(bit & value){
    if(BitsRead==0){
        byte tmp=0;
        if (!stream.Read(tmp))
            return;
        else {
            CurrentByte=0;
            for (int i=0; i<8; i++){
                CurrentByte <<=1;
                CurrentByte|=(1&tmp);
                tmp>>=1;
            }
        }
    }
    value=CurrentByte&1;
    CurrentByte>>=1;
    BitsRead=(BitsRead + 1)%8;
}

void ByteEncode::write(bit value){
    CurrentByte<<=1;
    CurrentByte|=value;
    BitsWritten++;
    if(BitsWritten==8){
        stream.Write(CurrentByte);
        BitsWritten=0;
    }
}

struct Node{
    byte value;
    int frequency;
    Node* left;
    Node* right;
    Node(byte data, int freq, Node* L, Node* R){
        value=data;
        frequency=freq;
        left=L;
        right=R;
    }
};

bool NodeComparator(Node* L, Node* R){
    return L->frequency<R->frequency;
}

Node* BuildHuffmanTree(map <byte, int> freq){
    list <Node*> list;
    for(map<byte, int>::iterator i=freq.begin(); i!= freq.end(); i++){
        if(i->second!=0){
            Node* node=new Node(i->first, i->second, nullptr, nullptr);
            list.push_back(node);
        }
    }

    while(list.size()>1){
        list.sort(NodeComparator);
        Node* left=list.front();
        list.pop_front();
        Node* right=list.front();
        list.pop_front();
        Node* NewNode=new Node(0, left->frequency+right->frequency, left, right);
        list.push_back(NewNode);
    }
    Node* root= list.front();
    return root;
}

void SetCodes(Node* node, int code, map <byte, int>& codes){
    if(node->left==nullptr){
        int result=1;
        while(code!=1){
            result<<=1;
            result|=(code& 1);
            code>>=1;
        }
        codes[node->value]=result;
        return;
    }
    SetCodes(node->left, code <<1, codes);
    SetCodes(node->right, (code<<1)+1, codes);
}

void DeleteTree(Node* node){
    if(node==nullptr){
        return;
    }
    DeleteTree(node->right);
    DeleteTree(node->left);
    delete node;
}

char SetCodeMaxLength(map <byte, char>& codeLengths){
    char MaxLength=0;
    for(map<byte, char>::iterator i=codeLengths.begin(); i!=codeLengths.end(); i++){
        if(i->second>MaxLength){
            MaxLength=i->second;
        }
    }
    return MaxLength;
}

void CountCodeLengths(const map <byte, int>& codes, map <byte, char>& codeLengths){
    for(map<byte, int>::const_iterator i=codes.begin(); i!=codes.end(); i++){
        codeLengths[i->first]=0;
        if(i->second!=0){
            int code=i->second;
            while(code != 1){
                codeLengths[i->first]++;
                code>>=1;
            }
        }
    }
}

void EncodingTableCreation(map <byte, int>& codes, map <byte, int> frequency, const byte RestBits, const byte BytesPerCode, const byte AlphabetSize, IOutputStream& compressed, ByteEncode& bout){
    compressed.Write(RestBits);
    compressed.Write(BytesPerCode);
    compressed.Write(AlphabetSize);
    for(map<byte, int>::iterator i=codes.begin(); i!=codes.end(); i++){
        if(frequency[i->first]!=0){
            int code=i->second;
            int j=0;
            compressed.Write((byte)i->first);
            while(code!=1){
                bout.write(code& 1);
                code>>=1;
                j++;
            }
            for(; j<BytesPerCode*8; j++)
                bout.write(0);
        }
    }
}

void Encoding(map <byte, int>& codes, byte RestBits, string& file, ByteEncode& bout){
    for(size_t i=0; i<file.length(); i++){
        long code=codes[file[i]];
        while(code!=1){
            bout.write(code& 1);
            code>>=1;
        }
    }
    for(int i=0; i<RestBits; i++){
        bout.write(0);
    }
}

void FixTree(Node* node){
    if(node==nullptr){
        return;
    }
    if(node->left!=nullptr && node->right==nullptr){
        DeleteTree(node->left);
        node->left = nullptr;
        return;
    }
    FixTree(node->left);
    FixTree(node->right);
}

void CreationDecodingHuffmanTree(Node* root, unsigned char AlphabetSize, byte BytesPerCode, IInputStream& compressed){
    ByteDecode bin(compressed);
    for(int i=0; i<AlphabetSize; i++){
        byte symbol;
        Node* node=root;
        compressed.Read(symbol);
        for(int j=0; j<8*BytesPerCode; j++){
            bit bit1;
            bin.read(bit1);
            if(bit1==0){
                if(node->left==nullptr){
                    node->left=new Node(symbol, 0, nullptr, nullptr);
                }
                node=node->left;
            }
            else{
                if(node->right==nullptr){
                    node->right=new Node(symbol, 0, nullptr, nullptr);
                }
                node=node->right;
            }
        }
    }
    FixTree(root);
}

void Decoding(Node* root, string& file, byte RestBits, IOutputStream& decoded){
    Node* node=root;
    for(size_t i=0; i<file.length()-1; i++){
        for(int j=7; j>=0; j--){
            if(((file[i]>>j)& 1)==0){
                node=node->left;
            }
            else {
                node=node->right;
            }
            if(node->left==nullptr){
                decoded.Write(node->value);
                node=root;
            }
        }
    }
    for(int j=7; j>=RestBits; j--){
        if(((file[file.length()-1]>>j)& 1)==0){
            node=node->left;
        }
        else {
            node=node->right;
        }
        if(node->left==nullptr){
            decoded.Write(node->value);
            node=root;
        }
    }
}

void Encode(IInputStream& original, IOutputStream& compressed){
    map <byte, int> frequency;
    byte value;
    string file;
    while (original.Read(value)){
        frequency[value]++;
        file+=value;
    }

    Node* root=BuildHuffmanTree(frequency);

    map <byte, int> codes;
    SetCodes(root, 1, codes);

    map <byte, char> codeLengths;
    CountCodeLengths(codes, codeLengths);

    char MaxLength=SetCodeMaxLength(codeLengths);

    int EncodedRestBits=0;
    byte AlphabetSize=0;
    for(map<byte, int>::iterator i=frequency.begin(); i!=frequency.end(); i++){
        if(i->second != 0){
            AlphabetSize++;
        }
        EncodedRestBits=(EncodedRestBits+i->second*codeLengths[i->first])%8;
    }
    byte RestBits=8-EncodedRestBits;
    byte BytesPerCode=(MaxLength%8==0)?(MaxLength/8):(MaxLength/8+1);
    ByteEncode bout(compressed);

    EncodingTableCreation(codes, frequency, RestBits, BytesPerCode, AlphabetSize, compressed, bout);

    Encoding(codes, RestBits, file, bout);
}

void Decode(IInputStream& compressed, IOutputStream& decoded){
    byte RestBits, BytesPerCode, tmp;
    compressed.Read(RestBits);
    compressed.Read(BytesPerCode);
    compressed.Read(tmp);
    unsigned char AlphabetSize=(unsigned char)tmp;

    Node* root=new Node(0, 0, nullptr, nullptr);
    CreationDecodingHuffmanTree(root, AlphabetSize, BytesPerCode, compressed);

    string file;
    byte symbol;
    while(compressed.Read(symbol)){
        file+=symbol;
    }

    Decoding(root, file, RestBits, decoded);

    DeleteTree(root);
}
