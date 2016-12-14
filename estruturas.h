/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   estruturas.h
 * Author: Marcelo
 *
 * Created on 12 de Dezembro de 2016, 16:48
 */

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <string>
#include <iostream>
#include <vector>


struct Tabela;

struct Valor{
    char tipo; //tipo do valor
    void* valor; //valor
    Valor(int val) {init(val);};
    Valor(std::string val) {init(val);};
    Valor(char tp) {init(tp);};

    Valor(std::string val, char tp) {
        int tam = val.size();
        if(val[0] == '\''){
            std::string qualquer = val.substr(1, tam - 2); 
            init(qualquer);
        }else if(tam == 4 && val[0] == 'N'){
            init(tp);
        }else{
            init(atoi(val.c_str()));
        }
    }
    
    Valor(const Valor &val){
        tipo = val.tipo;
        if(val.valor == NULL){
            valor = NULL;
        }else if(val.tipo == 'I'){
            init(*((int*)val.valor));
        }else if(val.tipo == 'C'){
            init(*((std::string*)val.valor));
        }
    }

    void init(int val){
        tipo = 'I';
        valor = (void*)new int(val);
    }
    void init(std::string val){
        tipo = 'C';
        valor = (void*)new std::string(val);
    }
    void init(char tp){
        tipo = tp;
        valor = NULL;
    }
    
    ~Valor(){
        if (tipo == 'C') {
            delete ((std::string*)valor);
        } else {
            delete ((int*)valor);
        }
        valor = NULL;
    }
    friend std::ostream& operator<<(std::ostream& os, const Valor& val);
    
    bool operator==(const Valor& dois){
        if(tipo != dois.tipo){
            return false;
        }
        if(valor == NULL && dois.valor == NULL){
            return true;
        }
        if(valor == NULL || dois.valor == NULL){
            return false;
        }
        if(tipo == 'I'){
            return *((int*)valor) == *((int*)dois.valor);
        }
        
        if(tipo == 'C'){
            return *((std::string*)valor) == *((std::string*)dois.valor);
        }
    }


    bool operator<(const Valor& dois){
        if(tipo != dois.tipo){
            return false;
        }
        if(valor == NULL || dois.valor == NULL){
            return false;
        }
        if(tipo == 'I'){
            return *((int*)valor) < *((int*)dois.valor);
        }
        if(tipo == 'C'){
            return *((std::string*)valor) < *((std::string*)dois.valor);
        }
    }
    
    bool operator<=(const Valor& dois){
        return *this<dois || *this==dois;
    }
    bool operator>=(const Valor& dois){
        return *this>dois || *this==dois;
    }
    bool operator>(const Valor& dois){
        if(valor == NULL || dois.valor == NULL){
            return false;
        }
        return !(*this<=dois);
    }
    bool operator!=(const Valor& dois){
        return !(*this==dois);
    }
    
    bool compara(const Valor& dois, const std::string& operacao){
        if(operacao == "="){
            return *this==(dois);
        }
        if(operacao == "<"){
            return *this<(dois);
        }
        if(operacao == "<="){
            return *this<=(dois);
        }
        if(operacao == ">"){
            return *this>(dois);
        }
        if(operacao == ">="){
            return *this>=(dois);
        }
        if(operacao == "<>"){
            return *this!=(dois);
        }
    }

};

struct Tupla{
    int N; //grau
    Valor** atributos; //valores de uma linha
    Tupla(std::string texto, Tabela* tab);
    Tupla(const Tupla &tupla);
    Tupla(int n);
    ~Tupla();
    friend std::ostream& operator<<(std::ostream& os, const Tupla& tup);
};

struct Atributo{
    std::string* nome; //nome do atributo
    char tipo;//tipo do atributo
    int notNull;//não pode ser nulo
    int ord;//está ordenado
    int chave;//é chave
    Atributo(std::string texto);
    Atributo(std::string n, std::string tp);
    Atributo(const Atributo &atri);
    ~Atributo();
    friend std::ostream& operator<<(std::ostream& os, const Atributo& atrib);
};

struct Tabela{
    int N;//grau
    int M;//cardinalidade
    std::string nome;//nome da tabela
    Atributo** atributos;//atributo da tabela(metadados)
    Tupla** tuplas;//dados da tabela
    Tabela(int n, int m, std::string nm);
    ~Tabela();
    std::ostream& imprime_ctl(std::ostream& out);
    friend std::ostream& operator<<(std::ostream& os, const Tabela& tab);
    bool valida(std::ostream& out);
    bool ler_dad();
};

void ler_linha(std::istream &in, std::string &linha);

void separar_operandos(std::string linha, std::vector<std::string> &operandos, char delim=',');

#endif /* ESTRUTURAS_H */

