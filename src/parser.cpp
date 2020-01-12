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
    {
        _current_token = _lexer.get_next_token();
        _current_token.printToken();
    }
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
        // std::cout << "on of the integers" << _current_token.getValue() << std::endl;
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
    ASTNode node = factor();

    while (_current_token.getType() == Type::MUL || _current_token.getType() == Type::DIV)
    {
         if (_current_token.getType() == Type::MUL)
        {
            eat(Type::MUL);
            ASTNode rightNode = term();
            ASTNode tempNode = {&node, Token(Type::MUL), &rightNode};
            node = tempNode;
        }
        else
        {
            eat(Type::DIV);
            ASTNode rightNode = term();
            ASTNode tempNode = {&node, Token(Type::DIV), &rightNode};
            node = tempNode;
        }
    }

    return node;
}

ASTNode Parser :: expr()
{
    /**
     * expr   : term ((PLUS | MINUS) term)*
        term   : factor ((MUL | DIV) factor)*
        factor : INTEGER | LPAREN expr RPAREN
     */

    ASTNode node;
    node = term();

    while (_current_token.getType() == Type::PLUS || _current_token.getType() == Type::SUBTRACT)
    {
        if (_current_token.getType() == Type::PLUS)
        {
            eat(Type::PLUS);
            ASTNode *tempNode = new ASTNode;
            ASTNode rightNode = term();

            tempNode->_right = &rightNode;
            tempNode->_token = Token(Type::PLUS);
            tempNode->_left = &node;    
            node = *tempNode;
        }
        else
        {
            eat(Type::SUBTRACT);
            ASTNode rightNode = term();
            ASTNode tempNode = {&node, Token(Type::SUBTRACT), &rightNode};
            node = tempNode;
        }
    }

    return node;
}

ASTNode Parser :: parse()
{
    ASTNode* tempNode = new ASTNode;
    ASTNode t = expr();
    tempNode = &t;
    ASTNode leftNode = *(tempNode->_left);
    std::cout << leftNode._token.getValue() << "    +    " << tempNode->_right->_token.getValue() << std::endl;


    return *tempNode;
}
