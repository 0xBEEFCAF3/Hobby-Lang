#include "token.h"
#include "lexer.h"
#include <stdlib.h>
#include <string>
#include <iostream>

Lexer::Lexer() {}
Lexer::Lexer(std::string text)
{
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

    while (_current_char != '\0' && (_current_char == ' '))
        advance();
}

Token Lexer::number()
{
    //Return a (multidigit) integer consumed from the input
    int i = 0;
    char tempStr[256];
    while (_current_char != '\0' && std::isdigit(_current_char))
    {
        tempStr[i] = _current_char;
        i++;
        advance();
    }

    //Are we dealing with a floating point number
    if(_current_char == '.'){
        tempStr[i] = _current_char;
        i++;
        advance();
       while (_current_char != '\0' && std::isdigit(_current_char))
       {
            tempStr[i] = _current_char;
            i++;
            advance(); 
            return Token(Type::REAL_CONST, std::atof(tempStr));
       } 
    }

    return Token(Type::REAL_CONST, std::atoi(tempStr));
}

char Lexer::peek()
{
    size_t peekPos = static_cast<size_t>(_pos + 1);

    if (peekPos > _text.length())
        return '\0';

    return _text.at(peekPos);
}

bool Lexer ::isReservedKeyword(std::string s)
{
    return (_reserved_key_words.find(s) != _reserved_key_words.end());
}

void Lexer :: skipComment(){
    while(_current_char != '}')
        advance();

    advance(); //For the actual curly brace
}

Token Lexer::_id()
{
    /** Handle identifiers and reserved keywords */
    std::string result;
    while (_current_char != '\0' && std::isalnum(_current_char) && _current_char != ';')
    {
        // std::cout << "foo " << _current_char << "\t " << _pos << std::endl;
        result.append(std::string(1, _current_char));
        advance();
    }
    /** check if identifier is in reserved map */
    if (_reserved_key_words.find(result) == _reserved_key_words.end())
        return Token(Type::ID, result);

    return _reserved_key_words[result];
}

Token Lexer::get_next_token()
{
    std::string text = _text;
    while (_current_char != '\0')
    {
        if (_current_char == '\n')
        {
            advance();
            continue;
        }
        if (_current_char == ' ')
        {
            skipWhiteSpace();
            continue;
        }
        if (std::isalpha(_current_char))
        {
            return _id();
        }
        if (std::isdigit(_current_char))
        {
            return number();
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
        if (_current_char == ':' && peek() == '=')
        {
            advance();
            advance();
            return Token(Type::ASSIGN, std::string(":="));
        }
        if (_current_char == ';')
        {
            advance();
            return Token(Type::SEMI, std::string(";"));
        }
        if (_current_char == '.')
        {
            advance();
            return Token(Type::DOT, std::string("."));
        }
        //Error state
        error();
        return Token(Type::EMPTY);
    } //end of while
    return Token(Type::ENDOFFILE);
}
