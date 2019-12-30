#include "token.h"
#include "lexer.h"
#include <stdlib.h>
#include <string>


Lexer::Lexer(){}
Lexer::Lexer(std::string text){
    _text = text;
    _pos = 0;
    _has_error = false;
    _current_char = text.c_str()[0];
}

bool Lexer::getError()
{
    return _has_error;
}

int Lexer::getPos()
{
    return _pos;
}

void Lexer::setPos(int pos)
{
    _pos = pos;
}

std::string Lexer::getText()
{
    return _text;
}

void Lexer::error()
{
    _has_error = true;
}

void Lexer::advance()
{
    //Advance the 'pos' pointer and set the 'current_char' variable.
    _pos++;
    if (size_t(_pos) > _text.size())
        _current_char = '\0';
    else
        _current_char = _text.c_str()[_pos];
}

void Lexer::skipWhiteSpace()
{
    while (_current_char != '\0' && _current_char == ' ')
        advance();
}

int Lexer::interger()
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

Token Lexer::get_next_token()
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
        if (_current_char == '*')
        {
            advance();
            return Token(Type::MUL);
        }
        if (_current_char == '/')
        {
            advance();
            return Token(Type::DIV);
        }
        if (_current_char == ')')
        {
            advance();
            return Token(Type::RPAREN);
        }
        if (_current_char == '(')
        {
            advance();
            return Token(Type::LPAREN);
        }
        //Error state
        error();
        return Token(Type::EMPTY);
    }
    return Token(Type::ENDOFFILE);
}
