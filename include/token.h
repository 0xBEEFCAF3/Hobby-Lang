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
    EMPTY
};

class Token
{
private:
    Type _type;
    int _value;
public:
    Token();
    Token(Type type);
    Token(Type type, int value);
    void printToken();
    Type getType();
    int getValue();
};
#endif
