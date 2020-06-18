#include <stdlib.h>
#include <iostream>
#include <string>
#include "interperter.h"
#include "lexer.h"
#include <vector>

Parser::Parser()
{
}

Parser ::Parser(Lexer lexer)
{
    _lexer = lexer;
    // _current_token = _lexer.get_next_token();
    _root = *(new ASTNode);
}

Token Parser ::getToken()
{
    return _current_token;
}

void Parser ::setToken(Token token)
{
    _current_token = token;
}

void Parser ::eat(Type type)
{
    if (_current_token.getType() == type)
        _current_token = _lexer.get_next_token();
    else
    {
        std::cout << "PARSER ERROR :: ";
        printf(" got type: %d\tExpected:%d\n", _current_token.getType(), type);
        exit(0);
        // _lexer.error();
    }
}

/** Decent recursive lexers */
ASTProgramNode Parser::program()
{
    // PROGRAM variable SEMI block DOT
    std::cout << "[PARSER] in program" << std::endl;
    _current_token = _lexer.get_next_token();  //taken from ctor 
    eat(Type::PROGRAM);
    eat(Type::SEMI);
    // ASTNode programVariableNode = variable();
    // eat(Type::SEMI);
    declarations();
    ASTNode blockNode = block();
    ASTProgramNode programNode = {"Program name", blockNode};

    eat(Type::DOT);
    return programNode;
}

ASTNode Parser::block()
{
    ASTNode compoundStatments = compoundStatment();
    ASTNode block;
    block._children = compoundStatments._children;
    block._declarations = declarations();
    block._type = ASTNodeType::BLOCK;
    return block;
}

std::vector<ASTNode> Parser::declarations()
{
    /* declarations : VAR (variable_declaration SEMI)+
                    | empty
    */
    std::vector<ASTNode> decs = {};
    if (_current_token.getType() == Type::VAR)
    {
        eat(Type::VAR);
        eat(Type::SEMI);
        while (_current_token.getType() == Type::ID)
        {
            std::vector<ASTNode> ds = variableDeclarations();
            /*Extend a declaration of a variable of a certain type */
            for (std::vector<ASTNode>::iterator it = ds.begin(); it != ds.end(); it++)
            {
                decs.push_back(*it);
            }
            eat(Type::SEMI);
        }
    }
    return decs;
}

std::vector<ASTNode> Parser::variableDeclarations()
{
    /*variable_declaration : ID (COMMA ID)* COLON type_spec */
    std::vector<ASTNode> varIds = {variable()}; //first id
    while (_current_token.getType() == Type::COMMA)
    {
        eat(Type::COMMA);
        varIds.push_back(variable());
    }

    eat(Type::COLON);
    ASTNode varTypeNode = typeSpec();
    for (std::vector<ASTNode>::iterator it = varIds.begin(); it != varIds.end(); it++)
    {
        it->_typeNode = &varTypeNode;
    }
    return varIds;
}

ASTNode Parser::typeSpec()
{
    /*
    type_spec : INTEGER
                 | REAL
                  */

    if (_current_token.getType() == Type::INTEGER)
    {
        eat(Type::INTEGER);
    }
    else
        eat(Type::REAL);

    ASTNode node;
    node._token = _current_token;
    node._type = ASTNodeType::Type;

    return node;
}

ASTNode Parser::compoundStatment()
{
    //    compound_statement: BEGIN statement_list END
    ASTNode node;
    eat(Type::BEGIN);
    node._children = statmentList();
    node._type = ASTNodeType::COMPOUND;
    eat(Type::END);

    return node;
}

std::vector<ASTNode> Parser::statmentList()
{
    /*
    statement_list : statement
                   | statement SEMI statement_list
     */
    std::vector<ASTNode> children;
    ASTNode node = statment();
    children.push_back(node);

    while (_current_token.getType() == Type::SEMI)
    {
        eat(Type::SEMI);
        children.push_back(statment());
    }

    if (_current_token.getType() == Type::ID)
        __error();

    return children;
}

ASTNode Parser::statment()
{
    /**
     * statement : compound_statement
              | assignment_statement
              | empty
     **/

    printf("[PARSER] in statment %d\n" , _current_token.getType());

    ASTNode node;
    if (_current_token.getType() == Type::BEGIN)
        node = compoundStatment();
    else if (_current_token.getType() == Type::ID)
        node = assignmentStatment();
    else
        node = empty();

    return node;
}

ASTNode Parser::assignmentStatment()
{
    ASTNode left = variable();
    Token token = _current_token;
    eat(Type::ASSIGN);

    ASTNode right = expr();
    // printf("[PARSER] assigning %s = %f \n", left._token.getStringValue().c_str(), static_cast<double>(right._token.getFloatValue()));

    /** Set up the current next node */
    ASTNode *rightNode = new ASTNode;
    ASTNode *leftNode = new ASTNode;

    rightNode->_left = right._left;
    rightNode->_right = right._right;
    rightNode->_token = right._token;
    rightNode->_type = right._type;

    leftNode->_left = left._left;
    leftNode->_right = left._right;
    leftNode->_token = left._token;
    leftNode->_type = left._type;

    std::vector<ASTNode> _emptyChildren;
    ASTNode node = {leftNode, token, rightNode, _emptyChildren, ASTNodeType::ASSIGN, {}, nullptr, nullptr};

    return node;
}

ASTNode Parser::variable()
{
    //    variable : ID
    ASTNode node;
    node._left = nullptr;
    node._token = _current_token;
    node._right = nullptr;
    node._type = ASTNodeType::VAR;

    eat(Type::ID);
    return node;
}

ASTNode Parser::empty()
{
    ASTNode node;
    node._type = ASTNodeType::NOOP;
    return node;
}

ASTNode Parser ::factor()
{
   
    if (_current_token.getType() == Type::SUBTRACT)
    {
        ASTNode unaryOp;
        ASTNode *rightNode = new ASTNode;

        unaryOp._token = Token(Type::SUBTRACT);
        eat(Type::SUBTRACT);
        /* Recurse to get factor */
        ASTNode t = factor();
        rightNode->_left = t._left;
        rightNode->_right = t._right;
        rightNode->_token = t._token;
        rightNode->_type = t._type;
        /* Always assign unary op expr to right node */
        unaryOp._right = rightNode;
        /* Init left branch */
        unaryOp._left = nullptr;
        unaryOp._type = ASTNodeType::UNARY;

        return unaryOp;
    }
    else if (_current_token.getType() == Type::PLUS)
    {
        ASTNode unaryOp;
        ASTNode *rightNode = new ASTNode;

        unaryOp._token = Token(Type::PLUS);
        eat(Type::PLUS);
        /* Recurse to get factor */
        ASTNode t = factor();
        rightNode->_left = t._left;
        rightNode->_right = t._right;
        rightNode->_token = t._token;
        rightNode->_type = t._type;
        /* Always assign unary op expr to right node */
        unaryOp._right = rightNode;
        /* Init left branch */
        unaryOp._left = nullptr;
        unaryOp._type = ASTNodeType::UNARY;

        return unaryOp;
    }
    else if (_current_token.getType() == Type::INTERGER)
    {
        ASTNode *tempNode = new ASTNode;
        tempNode->_token = Token(Type::INTERGER, _current_token.getValue());
        tempNode->_type = ASTNodeType::VALUE;

        eat(Type::INTERGER);
        return *tempNode;
    }
    else if (_current_token.getType() == Type::LPAREN)
    {
        eat(Type::LPAREN);
        ASTNode node = expr();
        eat(Type::RPAREN);
        return node;
    }
    else if (_current_token.getType() == Type::INTEGER_CONST)
    {
        
        ASTNode node;
        node._token = _current_token;
        node._type = ASTNodeType::VALUE;
        eat(Type::INTEGER_CONST);
        return node;
    }else if (_current_token.getType() == Type::REAL_CONST)
    {
       printf("[PARSER] :: found real const  current token type:: %d\n", _current_token.getType()); 
        ASTNode node;
        node._token = _current_token;
        node._type = ASTNodeType::VALUE;
        eat(Type::REAL_CONST);
        return node;
    }
    else
    {
        std::cout << "[PARSER] making variable in factor  " << std::endl;
        ASTNode node = variable();
        return node;
    }
}

ASTNode Parser ::term()
{
    ASTNode node;
    node = factor();

    while (_current_token.getType() == Type::MUL || _current_token.getType() == Type::INTEGER_DIV || _current_token.getType() == Type::FLOAT_DIV)
    {
        if (_current_token.getType() == Type::MUL)
        {
            eat(Type::MUL);
            ASTNode t = term();

            ASTNode *rightNode = new ASTNode;
            ASTNode *leftNode = new ASTNode;

            rightNode->_left = t._left;
            rightNode->_right = t._right;
            rightNode->_token = t._token;
            rightNode->_type = t._type;

            leftNode->_left = node._left;
            leftNode->_right = node._right;
            leftNode->_token = node._token;
            leftNode->_type = node._type;

            printf("[PARSER] in MUL r: %d --- l: %d\n", node._token.getValue(), t._token.getValue());

            std::vector<ASTNode> _emptyChildren;
            node = {leftNode, Token(Type::MUL), rightNode, _emptyChildren, ASTNodeType::BINARY, {}, nullptr, nullptr};
        }
        else
        {
            if (_current_token.getType() == Type::INTEGER_DIV)
                eat(Type::INTEGER_DIV);
            else
                eat(Type::FLOAT_DIV);

            ASTNode t = term();

            ASTNode *rightNode = new ASTNode;
            ASTNode *leftNode = new ASTNode;

            rightNode->_left = t._left;
            rightNode->_right = t._right;
            rightNode->_token = t._token;
            rightNode->_type = t._type;

            leftNode->_left = node._left;
            leftNode->_right = node._right;
            leftNode->_token = node._token;
            leftNode->_type = node._type;

            std::vector<ASTNode> _emptyChildren;
            node = {leftNode, Token(Type::DIV), rightNode, _emptyChildren, ASTNodeType::BINARY, {}, nullptr, nullptr};
        }
    }

    return node;
}

ASTNode Parser ::expr()
{
    /**
     *  expr   : term ((PLUS | MINUS) term)*
        term   : factor ((MUL | DIV) factor)*
        factor : (PLUS | MINUS)factor | INTEGER | LPAREN expr RPAREN
     */

    ASTNode node;
    node = term();
    printf("[PARSER] : node ast type in EXPR %d\n", node._type);
    while (_current_token.getType() == Type::PLUS || _current_token.getType() == Type::SUBTRACT)
    {
        if (_current_token.getType() == Type::PLUS)
        {
            eat(Type::PLUS);
            ASTNode t = term();

            ASTNode *rightNode = new ASTNode;
            ASTNode *leftNode = new ASTNode;

            rightNode->_left = t._left;
            rightNode->_right = t._right;
            rightNode->_token = t._token;
            rightNode->_type = t._type;

            leftNode->_left = node._left;
            leftNode->_right = node._right;
            leftNode->_token = node._token;
            leftNode->_type = node._type;

            std::vector<ASTNode> _emptyChildren;
            node = {leftNode, Token(Type::PLUS), rightNode, _emptyChildren, ASTNodeType::BINARY, {}, nullptr, nullptr};
        }
        else
        {
            eat(Type::SUBTRACT);
            ASTNode t = term();

            ASTNode *rightNode = new ASTNode;
            ASTNode *leftNode = new ASTNode;

            rightNode->_left = t._left;
            rightNode->_right = t._right;
            rightNode->_token = t._token;
            rightNode->_type = t._type;

            leftNode->_left = node._left;
            leftNode->_right = node._right;
            leftNode->_token = node._token;
            leftNode->_type = node._type;

            std::vector<ASTNode> _emptyChildren;
            node = {leftNode, Token(Type::SUBTRACT), rightNode, _emptyChildren, ASTNodeType::BINARY, {}, nullptr, nullptr};
        }
    }

    return node;
}

ASTProgramNode Parser ::parse()
{
    std::cout << "[PARSER] Starting to parse" << std::endl;
    ASTProgramNode tempNode;
    tempNode = program();
    //TODO check -- current token should be EOF
    return tempNode;
}
