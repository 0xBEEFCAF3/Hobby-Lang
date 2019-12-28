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

int Interperter::factor()
{
    Token token = _current_token;
    eat(Type::INTERGER);
    return token.getValue();
}

int Interperter::expr()
{
    /* Arithmetic expression parser / interpreter.

        expr   : factor ((MUL | DIV) factor)*
        factor : INTEGER
    */

   int result = factor();
    while(_current_token.getType() == Type::DIV ||_current_token.getType() == Type::MUL
    ||_current_token.getType() == Type::SUBTRACT ||_current_token.getType() == Type::PLUS){
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
        case Type::SUBTRACT:
            result = result - factor();
            break;
        case Type::PLUS:
            result = result + factor();
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
