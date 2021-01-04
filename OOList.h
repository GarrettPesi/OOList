#pragma once
#include <iostream>

//Creation of record class
class rec
{
    friend class list;
    private:
        char* id;
        char firstname[15];
        char lastname[15];
        rec* previous;
        rec* next;
    public:
        void SetData(char* id_in, char* fn, char* ln);
        rec(char* i, char* fn, char* ln);
        rec(void);
        void operator=(const rec& r);
        rec(const rec& r);
        void Print();
        ~rec();
};


//Creation of list class
class list {
    private:
        rec* first;
        rec* last;
        rec* current;
        int size;
    public:
        int AddItem(rec r);
        int DeleteItem(char* delid);
        void PrintList(int order);
        int Write(rec r);
        void Read(int order);
        void operator=(list l);
        list(void);
};