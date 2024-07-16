#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack> 

extern int errorcount;
extern int yylineno;
extern char *build_file_name;

using namespace std;

class Node {
protected:
    vector<Node*> children; 
    int lineno;

public:
    Node() {
        lineno = yylineno;
    }
    int getLineNo() {
        return lineno;
    }
    virtual string toStr() {
        return "stmts";
    }
    virtual string toDebug(){
        return toStr();
    }
    void append(Node *n) {
        children.push_back(n);
    }
    vector<Node*>& getChildren() {
        return children;
    }
};

class Program : public Node {
public:
    virtual string toStr() override {
        return "Program";
    }
};

class TypeDec : public Node {
protected:
    string type; 

public:
    TypeDec(string t){
        type = t;
    }
    virtual string toStr() override{
        if (type == "int") {
            return "int";
        } else if (type == "float") {
            return "float";
        } else if (type == "string") {
            return "string";
        } else if (type == "bool") {
            return "bool";
        } 
        else return "sem tipo";
    }
};

class Ident : public Node{
protected:
    string name;

public:
    Ident(const string n){
        name = n;
    }

    const string getName(){
        return name;
    }

    virtual string toStr() override{
        return name;
    }
};

class Integer : public Node {
protected:
    int value; 

public:
    Integer(const int v) {
        value = v;
    }

    virtual string toStr() override {
        return to_string(value);
    }
};

class Float : public Node {
protected:
    float value; 

public:
    Float(const float v) {
        value = v;
    }

    virtual string toStr() override {
        return to_string(value);
    }
};

class Boolean : public Node {
protected:
    bool value; 

public:
    Boolean(bool v) : value(v) {}

    virtual string toStr() override {
        return value ? "true" : "false";
    }
};

class String : public Node {
protected:
    string value; 

public:
    String(const string v){
        value = v;
    }

    const string getValue(){
        return value;
    }

    virtual string toStr() override{
        return value;
    }
};

class True: public Node{
protected:

public:
    True(){}

    virtual string toStr() override{
        return "true";
    }

    virtual string toDebug() override{
        return toStr();
    }
};

class False: public Node{
protected:

public:
    False(){}

    virtual string toStr() override{
        return "false";
    }

    virtual string toDebug() override{
        return toStr();
    }
};

class Variable : public Node
{
protected:
    TypeDec *type;
    string name;
    Node *value;

public:
    Variable(TypeDec *t,const string n, Node *v)
    {
        type = t;
        name = n;
        value = v;
        children.push_back(t);
        children.push_back(v);
    }

    const string getName(){
        return name;
    }

    virtual string toDebug() override{
        return type->toStr() + name + "=" + value->toDebug();
    }

    virtual string toStr() override
    {
        return  name + "=";
    }
  
};

class Attribution : public Node
{
protected:
    string name;
    Node *value;

public:
    Attribution(const string n, Node *v)
    {
        name = n;
        value = v;
        children.push_back(v);
    }

    const string getName(){
        return name;
    }

    virtual string toStr() override
    {
        return  name + "=";
    }

    virtual string toDebug() override{
        return name + "=" + value->toDebug();
    }
};

class Unary : public Node {
protected:
    Node *value; 
    char operation; 

public:
    Unary(Node *v, char op) {
       value = v;
       operation = op;
       children.push_back(v);
    }

    virtual string toStr() override {
        string aux;
        aux.push_back(operation);
        return aux;
    }
};

class BinaryOp : public Node {
protected:
    Node *value1;
    Node *value2;
    char operation;

public:
    BinaryOp(Node *v1, Node *v2, char op) {
       value1 = v1;
       value2 = v2;
       operation = op;
       children.push_back(v1);
       children.push_back(v2);
    }

    virtual string toStr() override {
        string aux;
        aux.push_back(operation);
        return aux;
    }
};

class Condition : public Node {
protected:
    Node *value1; 
    Node *value2; 
    string operation; 

public:
    Condition(Node *v1, Node *v2, string op){
        value1 = v1;
        value2 = v2;
        operation = op;
        children.push_back(v1);
        children.push_back(v2);
    }

    virtual string toStr() override{   
        return operation ;
    }

    virtual string toDebug() override{
        return value1->toDebug() + operation + value2->toDebug(); 
    }
};


class Pass : public Node {
protected:
    string ident; 
    string operation; 

public:
    Pass(string id, string op) : ident(id), operation(op) {}

    virtual string toStr() override {
        return ident + operation;
    }
};

class If : public Node {
private:
    Node *condition; 
    Node *body; 

public:
    If(Node *cond, Node *b) : condition(cond), body(b) {
        children.push_back(cond);
        children.push_back(b);
    }

    virtual string toStr() override {
        return "if";
    }
};

class IfElse : public Node {
private:
    Node *condition; 
    Node *if_body; 
    Node *else_body; 

public:
    IfElse(Node *cond, Node *ifb, Node *elseb) : condition(cond), if_body(ifb), else_body(elseb) {
        children.push_back(cond);
        children.push_back(ifb);
        children.push_back(elseb);
    }

    virtual string toStr() override {
        return "if-else";
    }
};

class Loop : public Node {
private:
    Node *declaration; 
    Node *condition; 
    Node *postLoop; 
    Node *body; 

public:
    Loop(Node *decl, Node *cond, Node *pass, Node *globals) : declaration(decl), condition(cond), postLoop(pass), body(globals) {
        children.push_back(decl);
        children.push_back(cond);
        children.push_back(pass);
        children.push_back(globals);
    }

    virtual string toStr() override {
        return "loop";
    }
};

class Scan : public Node {
protected:
    Node *typeNode = NULL; 
public:
    Scan() {}
    Scan(Node *t) : typeNode(t) {
        children.push_back(t);
    }

    Node* getTypeNode() {
        return typeNode;
    }

    virtual string toStr() override {
        if (typeNode != NULL)
            return "scan(" + typeNode->toStr() + ")";
        else
            return "sem tipo";
    }

    virtual string toDebug() override {
        return toStr();
    }
};


class Print : public Node {
protected:
    Node *value; 

public:
    Print(Node *v) : value(v) {
        children.push_back(v);
    }

    virtual string toStr() override {
        return "print";
    }

    virtual string toDebug() override {
        return "print(" + value->toDebug() + ")";
    }
};


class CheckVarDecl {
private:
    set<string> symbols;
public:
    CheckVarDecl() {}

    void check(Node *noh) {
        for(Node *c : noh->getChildren()) {
            check(c);
        }

        Ident *id = dynamic_cast<Ident*>(noh);
        if (id) {
            if (symbols.count(id->getName()) <= 0) {
                cout << build_file_name
                     << ":"
                     << id->getLineNo()
                     << ":0: semantic error: " 
                     << id->getName()
                     << " undefined."
                     << endl;
                errorcount++;
            }
        }

        Variable *var = dynamic_cast<Variable*>(noh);
        if (var) {
            symbols.insert(var->getName());
        }
    }
};

class CheckVarMix {
private:
public:
    CheckVarMix() {}

    void check(Node *noh) {
        for(Node *c : noh->getChildren()) {
            check(c);
        }

        BinaryOp *bo = dynamic_cast<BinaryOp*>(noh);
        if (bo) {
           Integer *i0 = dynamic_cast<Integer*>(noh->getChildren()[0]);
           Integer *i1 = dynamic_cast<Integer*>(noh->getChildren()[1]);
           if((i0 == NULL && i1 != NULL) || (i0 !=  NULL && i1 == NULL)){
            cout << build_file_name
                << ":"
                << bo->getLineNo()
                << ":0: semantic error: " 
                << " tipo mesclado proibido."
                << endl;
                errorcount++;
           } 
        }
    }
};

void printf_tree_recursive(Node *noh) {
    if (noh == nullptr) {
        cout << "Node is null" << endl;
        return;
    }
    for (Node *c : noh->getChildren()) {
        printf_tree_recursive(c);
    }
    cout << "N" << (long int)noh << "[label=\"" <<
        noh->toStr() << "\"];" << endl;

    for (Node *c : noh->getChildren()) {
        if (c != nullptr) {
            cout << "N" << (long int)noh << "--" <<
                "N" << (long int)c << ";" << endl;
        }
    }
}

void printf_tree(Node *root) {
    if (root == nullptr) {
        cout << "Root is null" << endl;
        return;
    }
    cout << "graph {" << endl;
    printf_tree_recursive(root);
    cout << "}" << endl;
}

