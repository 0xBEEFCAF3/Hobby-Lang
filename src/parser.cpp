#include <stdlib.h>
#include <iostream>
#include <string>
#include "interperter.h"
#include "lexer.h"

Parser::Parser()
{
}

Parser ::Parser(Lexer lexer)
{
    _lexer = lexer;
    _current_token = _lexer.get_next_token();
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
        std::cout << "PARSER ERROR";
        _lexer.error();
    }
}

ASTNode Parser ::factor()
{
    if (_current_token.getType() == Type::INTERGER)
    {
        ASTNode tempNode; 
        tempNode._token = Token(Type::INTERGER, _current_token.getValue());
        eat(Type::INTERGER);
        return tempNode;
    }
    else
    {
        eat(Type::LPAREN);
        ASTNode node = expr();
        eat(Type::RPAREN);
        return node;
    }
}

ASTNode Parser ::term()
{
    ASTNode node;
    node = factor();

    while (_current_token.getType() == Type::MUL || _current_token.getType() == Type::DIV)
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

            leftNode->_left = node._left;
            leftNode->_right = node._right;
            leftNode->_token = node._token;

            node = {leftNode, Token(Type::MUL), rightNode};


        }
        else
        {
            eat(Type::DIV);
            ASTNode t = term();

            ASTNode *rightNode = new ASTNode;
            ASTNode *leftNode = new ASTNode;
            
            rightNode->_left = t._left;
            rightNode->_right = t._right;
            rightNode->_token = t._token;

            leftNode->_left = node._left;
            leftNode->_right = node._right;
            leftNode->_token = node._token;

            node = {leftNode, Token(Type::DIV), rightNode};
        }
    }

    return node;
}

ASTNode Parser :: expr()
{
    /**
     *  expr   : term ((PLUS | MINUS) term)*
        term   : factor ((MUL | DIV) factor)*
        factor : (PLUS | MINUS)factor | INTEGER | LPAREN expr RPAREN
     */

    ASTNode node;
    node = term();
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

            leftNode->_left = node._left;
            leftNode->_right = node._right;
            leftNode->_token = node._token;

            node = {leftNode, Token(Type::PLUS), rightNode};
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

            leftNode->_left = node._left;
            leftNode->_right = node._right;
            leftNode->_token = node._token;

            node = {leftNode, Token(Type::SUBTRACT), rightNode};
        }
    }

    return node;
}

ASTNode Parser :: parse()
{
    ASTNode tempNode;
    tempNode = expr();
    return tempNode;
}
