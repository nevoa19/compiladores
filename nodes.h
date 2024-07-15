#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack> 

extern int errorcount;
extern int yylineno;
extern char *build_file_name;

using namespace std;

// Base class for all nodes
class Node {
protected:
    vector<Node*> children; // Child nodes
    int lineno; // Line number where the node is found

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

// Represents the entire program
class Program : public Node {
public:
    virtual string toStr() override {
        return "Program";
    }
};

// Represents a type declaration
class TypeDec : public Node {
protected:
    int type; // Type identifier

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
        } 
        else return "sem tipo";
    }
};

// Represents an identifier
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

// Represents an integer value
class Integer : public Node {
protected:
    int value; // Integer value

public:
    Integer(const int v) {
        value = v;
    }

    virtual string toStr() override {
        return to_string(value);
    }
};

// Represents a float value
class Float : public Node {
protected:
    float value; // Float value

public:
    Float(const float v) {
        value = v;
    }

    virtual string toStr() override {
        return to_string(value);
    }
};

// Represents a boolean value
class Boolean : public Node {
protected:
    bool value; // Boolean value

public:
    Boolean(bool v) : value(v) {}

    virtual string toStr() override {
        return value ? "true" : "false";
    }
};

// Represents a string value
class String : public Node {
protected:
    string value; // String value

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

// Represents a variable
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

// Represents a unary operation
class Unary : public Node {
protected:
    Node *value; // Operand
    char operation; // Unary operator

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

// Represents a binary operation
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

// Represents a conditional expression
class Condition : public Node {
protected:
    Node *value1; // First operand
    Node *value2; // Second operand
    string operation; // Condition operator

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

// Represents a variable declaration
class Declaration : public Node {
protected:
    string name; // Variable name
    Node *value; // Variable value

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

// Represents a pass operation (no-op)
class Pass : public Node {
protected:
    string ident; // Identifier
    string operation; // Operation

public:
    Pass(string id, string op) : ident(id), operation(op) {}

    virtual string toStr() override {
        return ident + operation;
    }
};

// Represents an if statement
class If : public Node {
private:
    Node *condition; // Condition
    Node *body; // If body

public:
    If(Node *cond, Node *b) : condition(cond), body(b) {
        children.push_back(cond);
        children.push_back(b);
    }

    virtual string toStr() override {
        return "if";
    }
};

// Represents an if-else statement
class IfElse : public Node {
private:
    Node *condition; // Condition
    Node *if_body; // If body
    Node *else_body; // Else body

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

// Represents a loop statement
class Loop : public Node {
private:
    Node *declaration; // Initialization
    Node *condition; // Loop condition
    Node *postLoop; // Post-loop operation
    Node *body; // Loop body

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

// Represents a scan operation (input)
class Scan: public Node {
protected:
    string value; // Input value

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

// Represents a print operation (output)
class Print : public Node {
protected:
    Node *value; // Value to print

public:
    Print(Node *v) {
        value = v;
        children.push_back(v);
    }

    virtual string toStr() {
        return "print";
    }
};

// Semantic checker for variable declarations and operations
class CheckVarDecl {
private:
    stack<set<string>> symbolTableStack;

public:
    CheckVarDecl() {
        symbolTableStack.push(set<string>()); // Global scope
    }

    void enterScope() {
        symbolTableStack.push(set<string>());
    }

    void exitScope() {
        if (!symbolTableStack.empty()) {
            symbolTableStack.pop();
        }
    }

    bool isDeclared(const string& name) {
        vector<set<string>> temp; // Vector temporário para armazenar os elementos da pilha
        bool found = false;

        // Transferir elementos da pilha para o vetor temporário
        while (!symbolTableStack.empty()) {
            temp.push_back(symbolTableStack.top());
            symbolTableStack.pop();
        }

        // Procurar pelo nome no vetor temporário
        for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
            if (it->count(name) > 0) {
                found = true;
                break;
            }
        }

        // Transferir de volta os elementos para a pilha original
        for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
            symbolTableStack.push(*it);
        }

        return found;
    }

    void declare(const string& name) {
        if (!symbolTableStack.empty()) {
            symbolTableStack.top().insert(name);
        }
    }

    void check(Node *node) {
        if (!node) return;

        for (Node *child : node->getChildren()) {
            check(child);
        }

        // Implementação de verificação semântica aqui...
    }
};

// Functions to print the syntax tree
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

