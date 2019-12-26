#include "interperter.h"
#include "token.h"
#include <stdlib.h>
#include <iostream>
#include <string>

Interperter::Interperter(std::string str)
{
    _text = str;
    _pos = 0;
    _current_token = Token(Type::EMPTY);
    _current_char = _text.c_str()[0];
    _has_error = false;
}

bool Interperter::getError()
{
    return _has_error;
}

Token Interperter::getToken()
{
    return _current_token;
}
void Interperter::setToken(Token token)
{
    _current_token = token;
}

int Interperter::getPos()
{
    return _pos;
}

void Interperter::setPos(int pos)
{
    _pos = pos;
}

std::string Interperter::getText()
{
    return _text;
}

void Interperter::error()
{
    _has_error = true;
}

void Interperter::advance()
{
    //Advance the 'pos' pointer and set the 'current_char' variable.
    _pos++;
    if (size_t(_pos) > _text.size())
        _current_char = '\0';
    else
        _current_char = _text.c_str()[_pos];
}

void Interperter::eat(Type type)
{
    if (_current_token.getType() == type)
    {
        _current_token = get_next_token(); /* Sets the current token */
    }else{
        std::cout << "Failed at eat";
        error();
    }
    
}

void Interperter::skipWhiteSpace()
{
    while (_current_char != '\0' && _current_char == ' ')
        advance();
}

int Interperter::interger()
{
    //Return a (multidigit) integer consumed from the input
    int i = 0;
    char tempStr[10];
    while (_current_char != '\0' && std::isdigit(_current_char))
    {
        tempStr[i] = _current_char;
        i++;
        advance();
    }
    return std::atoi(tempStr);
}

Token Interperter::get_next_token()
{
    std::string text = _text;

    while (_current_char != '\0')
    {
        if (_current_char == ' ')
        {
            skipWhiteSpace();
            continue;
        }
        if (std::isdigit(_current_char))
        {
            return Token(Type::INTERGER, interger());
        }
        if (_current_char == '+')
        {
            advance();
            return Token(Type::PLUS);
        }
        if (_current_char == '-')
        {
            advance();
            return Token(Type::SUBTRACT);
        }
        //Error state
        error();
        return Token(Type::EMPTY);
    }
    return Token(Type::ENDOFFILE);
}

int Interperter::expr()
{
    //set current token to the first token taken from the input
    _current_token = get_next_token();
    //we expect the current token to be a single - digit integer
    Token left = _current_token;
    eat(Type::INTERGER);

    Token op = _current_token;
    if (op.getType() == Type::PLUS)
        eat(Type::PLUS);
    else
        eat(Type::SUBTRACT);

    Token right = _current_token;
    eat(Type::INTERGER);

    if (_has_error)
    {
        std::cout << "Syntax error.";
        return -1;
    }
    return op.getType() == Type::PLUS ? left.getValue() + right.getValue() : left.getValue() - right.getValue();
}
