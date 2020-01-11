#include <stdlib.h>
#include <iostream>
#include <string>
#include "parser.h"

Parser::Parser()
{
}

Parser ::Parser(Lexer lexer)
{
    _lexer = lexer;
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
        _lexer.error();
}

AST* Parser ::factor()
{
    if (_current_token.getType() == Type::INTERGER)
    {
        eat(Type::INTERGER);
        return &AST(_current_token);
    }
    else
    {
        eat(Type::LPAREN);
        AST* node = expr();
        eat(Type::RPAREN);
        return node;
    }
}

AST* Parser ::term()
{
    AST* node = factor();

    while (_current_token.getType() == Type::MUL || _current_token.getType() == Type::DIV)
    {
        _current_token.getType() == Type::MUL ? eat(Type::MUL) : eat(Type::DIV);
        node = &AST(node, _current_token, factor());
    }

    return node;
}

AST* Parser ::expr()
{
    /**
     * expr   : term ((PLUS | MINUS) term)*
        term   : factor ((MUL | DIV) factor)*
        factor : INTEGER | LPAREN expr RPAREN
     */

    AST* node = term();

    while (_current_token.getType() == Type::PLUS || _current_token.getType() == Type::SUBTRACT)
    {
        _current_token.getType() == Type::PLUS ? eat(Type::PLUS) : eat(Type::SUBTRACT);
        node = &AST(node, _current_token, term());
    }

    return node;
}


AST* Parser :: parse(){
    return expr();
}
