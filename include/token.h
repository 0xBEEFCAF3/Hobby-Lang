#include <string>

#pragma once

#ifndef token_h
#define token_h

enum class Type{
    INTERGER,
    PLUS,
    SUBTRACT,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    ENDOFFILE,
    EMPTY,
    BEGIN,
    END,
    DOT,
    ASSIGN,
    SEMI,
    ID
};

class Token
{
private:
    Type _type;
    int _value;
    std::string _value_s;

public:
    Token();
    Token(Type type);
    Token(Type type, int value);
    Token(Type type, std::string value);

    void printToken();
    Type getType();
    int getValue();
    std::string getStringValue();
    
};
#endif
