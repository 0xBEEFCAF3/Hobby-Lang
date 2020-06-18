#pragma once
    
#include "token.h"
#include <stdlib.h>
#include <string>
#include <map>


#ifndef lexer_h
#define lexer_h

class Lexer
{
private:
    
    std::string _text;
    int _pos; //pos into text
    bool _has_error;
    char _current_char;
    std::map<std::string, Token> _reserved_key_words =
     {
        {"BEGIN",   {Token( Type::BEGIN, "BEGIN")}},
        {"END",     {Token(Type::END, "END")}},
        {"DIV",     {Token(Type::DIV, "DIV")}},
        {"PROGRAM", {Token(Type::PROGRAM, "PROGRAM")}}, 
        {"VAR", {Token(Type::VAR, "VAR")}}, 
        {"INTEGER", {Token(Type::INTEGER, "INTEGER")}}, 
        {"REAL", {Token(Type::REAL, "REAL")}},
    };
public:
    Lexer();
    Lexer(std::string text);
    Token get_next_token();
    Token _id();
    void error();
    
    int getPos();
    void setPos(int pos);
    std::string getText();
    bool getError();
    void advance();
    void skipWhiteSpace();
    void skipComment();
    Token number();
    char peek();
    bool isReservedKeyword(std::string);
    

};
#endif
