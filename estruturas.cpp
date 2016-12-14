/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "estruturas.h"

using namespace std;


std::ostream& operator<<(std::ostream& out, const Valor& valor){
    if(valor.valor == NULL){
        out << "NULO";
    }else if(valor.tipo == 'I'){
        out << *((int*)valor.valor);
    }else if(valor.tipo == 'C'){
        out << "'" << *((string*)valor.valor) << "'";
    }
    return out;
}

Tupla::Tupla(string texto, Tabela* tab){
    
    vector<string> valores;
    separar_operandos(texto, valores);
    
    N = valores.size();
    if (N == 0) {
        N = tab->N;
    }
    atributos = new Valor*[N];
    int i = 0;
    for (; i < N; i++){
        atributos[i] = NULL;
    }
    for (i = 0; i < valores.size() && i < tab->N; i++){
        atributos[i] = new Valor(valores[i], tab->atributos[i]->tipo);
    }
}
Tupla::~Tupla(){
    int i = 0;
    for(;i < N; i++){
        delete(atributos[i]);
        atributos[i] = NULL;
    }
    delete[] atributos;
    atributos = NULL;
}

Tupla::Tupla(const Tupla &tupla){
    N = tupla.N;
    atributos = new Valor*[N];
    int i = 0;
    for(;i < N; i++){
        atributos[i] = new Valor(*tupla.atributos[i]);
    }
}

Tupla::Tupla(int n){
    N = n;
    atributos = new Valor*[N];
    int i = 0;
    for(;i < N; i++){
        atributos[i] = NULL;
    }
}



std::ostream& operator<<(std::ostream& out, const Tupla& tupla){
    int i = 0;
    for(;i < tupla.N; i++){
        out << *(tupla.atributos[i]);
        if(i != tupla.N-1){
            out << ",";
        }
    }
    return out;
}

Atributo::Atributo(string texto){
    nome = NULL;
    notNull = 0;
    chave = 0;
    ord = 0;
    int inicial = 0, i = 0, quant = 0;
    vector<string> valores;
    separar_operandos(texto, valores);
    for (quant = 0; quant < valores.size(); quant++){
        string qualquer = valores[quant];
        if(quant == 0){
            nome = new string(qualquer);
        }
        if(quant == 1){
            tipo = qualquer[0];
        }
        if(quant > 1){
            if (qualquer == "nn") {
                notNull = 1;
            }
            if (qualquer == "chv") {
                chave = 1;
            }
            if (qualquer == "ord") {
                ord = 1;
            }
        }
    }
}
Atributo::Atributo(std::string n, std::string tp) {
    nome = new string(n);
    if (tp == "string") {
        tipo = 'C';
    } else if (tp == "integer") {
        tipo = 'I';
    }
    notNull = 0;
    chave = 0;
    ord = 0;
}

Atributo::Atributo(const Atributo &atri){
    nome = new string(*(atri.nome));
    tipo = atri.tipo;
    notNull = atri.notNull;
    ord = atri.ord;
    chave = atri.chave;
}

Atributo::~Atributo(){
    delete nome;
    nome = NULL;
}

std::ostream& operator<<(std::ostream& out, const Atributo& atributo){
    out << *(atributo.nome) << "," << atributo.tipo;
    if(atributo.notNull){
        out << ",nn";
    }
    if(atributo.ord){
        out << ",ord";
    }
    if(atributo.chave){
        out << ",chv";
    }
    return out;
}

Tabela::Tabela(int n, int m, string nm): N(n), M(m), nome(nm) {
    atributos = new Atributo*[n];
    tuplas = new Tupla*[m];
    int i = 0;
    for (; i < n; i++){
        atributos[i] = NULL;
    }
    for (i = 0; i < m; i++){
        tuplas[i] = NULL;
    }
}
Tabela::~Tabela() {
    int i = 0;
    for(;i < N; i++) {
        delete atributos[i];
        atributos[i] = NULL;
    }
    for(i=0;i < M; i++) {
        delete tuplas[i];
        tuplas[i] = NULL;
    }
    delete[] atributos;
    delete[] tuplas;
    atributos = NULL;
    tuplas = NULL;
}
std::ostream& Tabela::imprime_ctl(std::ostream& out){
    out << N << "," << M << endl;
    int i = 0;
    for(;i < N; i++){
        out<<*(atributos[i]);
        out << "" << endl;
    }
    return out;
}

bool Tabela::ler_dad(){
    ifstream in ((nome+".dad").c_str());
    string linha;
    int i = 0;
    if(in.is_open()){
        while(!in.eof()){
            ler_linha(in, linha);
            if (linha.size()) {
                tuplas[i] = new Tupla(linha, this);
                i++;
            }
        }
        in.close();
    }
    else {
        cout<<"ERRO: "<<nome<<".ctl não existe";
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const Tabela& tabela){
    int i = 0;
    for(;i < tabela.M; i++){
        out<<*(tabela.tuplas[i]);
        out << "" << endl;
    }
    return out;
}

bool Tabela::valida(ostream& out){
    int valido = true;
    int i = 0;
    int j = 0;
    int k = 0;
    string* key = NULL;
    string* ord = NULL;
    for(;i < N; i++){
        if(atributos[i]->notNull){
            for(j = 0; j<M; j++){
                if(tuplas[j]->atributos[i]->valor == NULL){
                    valido = false;
                    out << "ERRO: tupla(" << j << ") possui campo "; 
                    out << *(atributos[i]->nome) << "NULO" << endl;
                }
            }
        }
        if(atributos[i]->chave){
            if(key){
                valido = false;
                out << "ERRO: mais de uma chave encontrada: ";
                out << *(atributos[i]->nome) << " (";
                out << *key << " havia sido encontrada anteriormente)" << endl;
            }else{
                key = atributos[i]->nome;
            }
            if(!atributos[i]->notNull){
                valido = false;
                out << "ERRO: " << *(atributos[i]->nome);
                out << " eh chave, mas aceita NULO" << endl;
            }
            for(j = 0; j < M; j++){
                for (k = j + 1; k < M; k++) {
                    if(*(tuplas[j]->atributos[i]) == *(tuplas[k]->atributos[i])){
                        valido = false;
                        out << "ERRO: " << *(atributos[i]->nome);
                        out << " eh chave, mas possui valores iguais" << endl;
                    }
                }
            }
        }
        if(atributos[i]->ord){
            if(ord){
                valido = false;
                out << "ERRO: mais de uma ordenacao encontrada: ";
                out << *(atributos[i]->nome) << " (";
                out << *ord << " havia sido encontrada anteriormente)" << endl;
            }else{
                ord = atributos[i]->nome;
            }
            if(!atributos[i]->notNull){
                valido = false;
                out << "ERRO: " << *(atributos[i]->nome);
                out << " eh ordenacao, mas aceita NULO" << endl;
            }
            for(j = 1; j<M; j++){
                if(*(tuplas[j]->atributos[i])<*(tuplas[j-1]->atributos[i])){
                    valido = false;
                    out << "ERRO: Campo " << *(atributos[i]->nome);
                    out << " nao ordenado" << endl;
                }
            }
        }
    }
    return valido;
}

void ler_linha(std::istream &in, std::string &linha){
    getline(in, linha);
    if (!linha.size()) {
        return;
    }
    if (*linha.rbegin() == '\n'){
        linha.erase(linha.length() - 1);
    }
    if (!linha.size()) {
        return;
    }
    if (*linha.rbegin() == '\r'){
        linha.erase(linha.length() - 1);
    }
    
}

void separar_operandos(string linha, vector<string> &operandos, char delim){
    stringstream os;
    os.str(linha);
    while(getline(os, linha,'\'')){
        stringstream ss(linha);
        while(getline(ss, linha, delim)){
            if (linha.size()) {
                operandos.push_back(linha);
            }
        }
        if(getline(os, linha,'\'')){
            operandos.push_back("'" + linha + "'");
        }
    }
}
