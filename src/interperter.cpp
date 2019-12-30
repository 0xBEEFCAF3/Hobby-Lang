#include "interperter.h"
#include "token.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "lexer.h"

Interperter::Interperter(Lexer lexer)
{
    _current_token = lexer.get_next_token();
    _lexer = lexer;
}
Token Interperter::getToken()
{
    return _current_token;
}
void Interperter::setToken(Token token)
{
    _current_token = token;
}

void Interperter::eat(Type type)
{
    if (_current_token.getType() == type)
    {
        _current_token = _lexer.get_next_token(); /* Sets the current token */
    }
    else
    {
        std::cout << "Failed at eat";
        _lexer.error();
    }
}

int Interperter::expr()
{
    /*
        expr   : term ((PLUS | MINUS) term)*
        term   : factor ((MUL | DIV) factor)*
        factor : INTEGER | LParen expr  RParen
     */
    int result = term();
    while (_current_token.getType() == Type::SUBTRACT || _current_token.getType() == Type::PLUS)
    {
        Token token = _current_token;
        eat(token.getType());
        switch (token.getType())
        {
        case Type::SUBTRACT:
            result = result - term();
            break;
        case Type::PLUS:
            result = result + term();
            break;
        default:
            break;
        }
    }
    if (_lexer.getError())
    {
        std::cout << "Syntax error.";
        return -1;
    }
    return result;
}

int Interperter::term()
{
    int result = factor();
    while (_current_token.getType() == Type::DIV || _current_token.getType() == Type::MUL)
    {
        Token token = _current_token;
        eat(token.getType());
        switch (token.getType())
        {
        case Type::DIV:
            result = result / factor();
            break;
        case Type::MUL:
            result = result * factor();
            break;
        default:
            break;
        }
    }
    if (_lexer.getError())
    {
        std::cout << "Syntax error.";
        return -1;
    }
    return result;
}

int Interperter::factor()
{
    if(_lexer.getError()) return -1;
    
    Token token = _current_token;
    if (token.getType() == Type::INTERGER)
    {
        eat(Type::INTERGER);
        return token.getValue();
    }
    else if(token.getType() == Type::LPAREN)
    {
        eat(Type::LPAREN);
        int result = expr();
        eat(Type::RPAREN);
        return result;
    }


    return -1;
}
