#include <iostream>
#include <vector>
#include <map>
#include <set>

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
    int type;

public:
    TypeDec(int t){
        type = t;
    }
    virtual string toStr() override{
        if (type == 1) {
            return "giez";
        } else if (type == 2) {
            return "bevumbagon";
        } else if (type == 3) {
            return "iderennon";
        } else if (type == 4) {
            return "engos";
        } else {
            return "sem tipo";
        }
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

class Ident : public Node {
protected:
    string name;
public:
    Ident(const string n) {
        name = n;
    }

    const string getName() {
        return name;
    }

    virtual string toStr() override {
        return name;
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

class Variable : public Node {
protected:
    string name;
    Node *value;
public:
    Variable(const string n, Node *v) {
        name = n;
        value = v;
        children.push_back(v);
    }

    const string getName() {
        return name;
    }

    virtual string toStr() override {
        return name + "=";
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


class Condition : public Node
{
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

class String : public Node{
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

class Scan: public Node{
protected:
    string value;

public:
    Scan(const string v){
        value = v;
    }

    const string getValue(){
       return value;
    }

    virtual string toStr() override{
        value = "scan";
        return value;
    }
};


class Print : public Node {
protected:
    Node *value;

public:
    Print(Node *v) {
        value = v;
        children.push_back(v);
    }

    virtual string toStr() {
        return "print";
    }
};

class Declaration : public Node {
protected:
    string name;
    Node *value;

public:
    Declaration(const string n, Node *v) {
        name = n;
        value = v;
        children.push_back(v);
    }

    const string getName() {
        return name;
    }

    virtual string toStr() override {
        return "declaration";
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

class CheckVarDecl {
private:
    set<string> symbols;

public:
    CheckVarDecl() {}

    void check(Node *node) {
        for (Node *child : node->getChildren()) {
            check(child);
        }

        Ident *id = dynamic_cast<Ident*>(node);
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

        Variable *var = dynamic_cast<Variable*>(node);
        if (var) {
            symbols.insert(var->getName());
        }

        BinaryOp *binOp = dynamic_cast<BinaryOp*>(node);
        if (binOp) {
            if (!typeCheck(binOp)) {
                cout << build_file_name
                     << ":"
                     << binOp->getLineNo()
                     << ":0: semantic error: Invalid operand types for operator "
                     << binOp->toStr()
                     << endl;
                errorcount++;
            }
        }
        
        Unary *unOp = dynamic_cast<Unary*>(node);
        if (unOp) {
            if (!typeCheck(unOp)) {
                cout << build_file_name
                     << ":"
                     << unOp->getLineNo()
                     << ":0: semantic error: Invalid operand type for operator "
                     << unOp->toStr()
                     << endl;
                errorcount++;
            }
        }
    }

   bool typeCheck(BinaryOp *op) {
    Node *left = op->getChildren()[0];
    Node *right = op->getChildren()[1];

    Integer *intLeft = dynamic_cast<Integer*>(left);
    Integer *intRight = dynamic_cast<Integer*>(right);
    if (intLeft && intRight) {
        return true; 
    }
    return false;
}

bool typeCheck(Unary *op) {
    Node *operand = op->getChildren()[0];

    Boolean *boolOperand = dynamic_cast<Boolean*>(operand);
    if (boolOperand) {
        return true; 
    }
    return false;
}

};

void printf_tree_recursive(Node *noh) {
    for(Node *c : noh->getChildren()) {
        printf_tree_recursive(c);
    }
    cout << "N" << (long int)noh << "[label=\"" <<
        noh->toStr() << "\"];" << endl;
    
    for(Node *c : noh->getChildren()) {
        cout << "N" << (long int)noh << "--" <<
            "N" << (long int)c << ";" << endl;
    }

}

void printf_tree(Node *root) {
    cout << "graph {" << endl;
    printf_tree_recursive(root);
    cout << "}" << endl;
}

