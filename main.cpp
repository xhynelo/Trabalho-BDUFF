/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Marcelo
 *
 * Created on 12 de Dezembro de 2016, 02:00
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "estruturas.h"

using namespace std;

struct ordenacao{
    bool operator()(Tupla* A, Tupla* B){
        return *A->atributos[i] < *B->atributos[i];
    }
    ordenacao(int k){
        i = k;
    }
    int i;
};

Tupla* combinar_tuplas(Tupla* A, Tupla* B){
    vector<Valor*> valores;
    int u = 0;
    for(u=0;u<A->N;u++){
        Valor* v = new Valor(*A->atributos[u]);
        valores.push_back(v);
    }
    for(u=0;u<B->N;u++){
        Valor* v = new Valor(*B->atributos[u]);
        valores.push_back(v);
    }
    Tupla* resultado= new Tupla(A->N+B->N);
    copy(valores.begin(),valores.end(),resultado->atributos);
    return resultado;
}

std::string lower(const std::string& in) {
  std::string out;

  std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
  return out;
}

// Apaga espacos da esquerda
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// Apaga espacos da direita
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// Apaga espacos de borda
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}


int busca_atributo(string T, Tabela *A){
    int i = 0;
    for(; i < A->N; i++){
        if(lower(*A->atributos[i]->nome) == lower(T)){
            return i;
        }
    }
    return -1;
}

bool S(Tabela* A, int coluna, string O, Valor &v, Tabela* &Z, string nome){
    int i = 0;
    if(coluna == -1){
        cout << "ERRO: coluna nao existe";
        return false;
    }
    if(A->atributos[coluna]->tipo != v.tipo && v.valor != NULL){
        cout << "ERRO: coluna com tipo inválido";
        return false;
    }
    int cardinalidade = 0;
    for(i = 0; i < A->M; i++){
        if(A->tuplas[i]->atributos[coluna]->compara(v, O)){
            cardinalidade++;
        }
    }
    int j = 0;
    Z = new Tabela(A->N, cardinalidade, nome);
    
    for(i = 0; i < A->N; i++){
        Z->atributos[i] = new Atributo(*A->atributos[i]);
    }
    for(i = 0; i < A->M; i++){
        if(A->tuplas[i]->atributos[coluna]->compara(v, O)){
            Z->tuplas[j++] = new Tupla(*A->tuplas[i]);
        }
    }
    return true;
}
    
bool P(Tabela* A, int n, vector<string> &lista_de_atributos, Tabela* &Z, string nome){
    Z = new Tabela(n, A->M, nome);
    int h = 0;
    for(;h < A->M;h++ ){
        Z->tuplas[h] = new Tupla("", Z); 
    }
    bool existe = false;
    int i = 0, j, k = 0, y = 0;
    for(; i < lista_de_atributos.size(); i++){
        existe = false;
        for(j = 0; j < A->N;j++){
            if(lower(lista_de_atributos[i]) == lower(*A->atributos[j]->nome)){
                existe = true;
                Z->atributos[k] = new Atributo(*A->atributos[j]);
                for(y = 0; y < A->M; y++){
                    Z->tuplas[y]->atributos[k] = new Valor(*A->tuplas[y]->atributos[j]);
                }
                k++;
            }
        }
        if(!existe){
            cout<<"ERRO: Atributo "<<lista_de_atributos[i];
            cout<<" não encontrado"<<endl;
            delete Z;
            Z = NULL;
            return false;
        }
    }
    return true;
}
    
bool J(Tabela* A, Tabela* B, int x, int y, Tabela* &Z, string nome){
    int i = 0, j = 0;
    if(x == -1 || y == -1){
        cout << "ERRO: coluna nao existe";
        return false;
    }
    vector<Atributo*> atributos;
    string concatA(A->nome), concatB(B->nome);
    if(A->nome == B->nome){
        concatA+="1";
        concatB+="2";
    }
    int u = 0;
    for(; u < A->N; u++){
        Atributo* at = new Atributo(*A->atributos[u]);
        at->ord = false;
        at->chave = false;
        atributos.push_back(at);
        *(at->nome) = concatA + *(A->atributos[u]->nome); 
    }
    for(u=0; u < B->N; u++){
        Atributo* at = new Atributo(*B->atributos[u]);
        at->ord = false;
        at->chave = false;
        atributos.push_back(at);
        *(at->nome) = concatB + *(B->atributos[u]->nome); 
    }
    vector<Tupla*> R(A->tuplas,A->tuplas+A->M);
    vector<Tupla*> S(B->tuplas,B->tuplas+B->M);
    ordenacao ordR(x);
    ordenacao ordS(y);
    sort(R.begin(), R.end(), ordR);
    sort(S.begin(), S.end(), ordS);
    vector<Tupla*> tuplas;
    while(i<R.size() && j<S.size()){
        if(*R[i]->atributos[x] > *S[j]->atributos[y]){
            j++;
        }else if (*R[i]->atributos[x] < *S[j]->atributos[y]){
            i++;
        }else{
            tuplas.push_back(combinar_tuplas(R[i], S[j]));
            
            int l = j+1;
            while(l < S.size() && *R[i]->atributos[x] == *S[l]->atributos[y]){
                tuplas.push_back(combinar_tuplas(R[i], S[l]));
                l++;
            }
            int k = i+1;
            while(k < S.size() && *R[k]->atributos[x] == *S[j]->atributos[y]){
                tuplas.push_back(combinar_tuplas(R[k], S[j]));
                k++;
            }
            i++;
            j++;
        }
    }
    Z = new Tabela(atributos.size(), tuplas.size(), nome);
    copy(atributos.begin(),atributos.end(),Z->atributos);
    copy(tuplas.begin(),tuplas.end(),Z->tuplas);
    return true;
}
        
/**/

Tabela* ler_arquivo_ctl(string nome_tab){
    ifstream in ((nome_tab+".ctl").c_str());
    string linha;
    int N = 0, M = 0, i = 0;
    char c = 0;
    Tabela* A = NULL;
    if(in.is_open()){
        in>>N>>c>>M;
        A = new Tabela(N, M, nome_tab);
        getline(in, linha);
        while(!in.eof()){
            ler_linha(in, linha);
            if (linha.size()) {
                A->atributos[i] = new Atributo(linha);
                i++;
            }
        }
        in.close();
    }
    else cout<<"ERRO: "<<nome_tab<<".ctl não existe";
    return A;
}

void ler_arquivo_alg(string nome){
    ifstream in ((nome+".alg").c_str());
    string linha;
    string ultima_tabela;
    if(in.is_open()){
        while(!in.eof()){
            ler_linha(in, linha);
            if (!linha.size()) {
                continue;
            }

            char operacao = linha[0];
            linha = linha.substr(2,linha.length()-3);
            vector<string> operandos;
            separar_operandos(linha, operandos);
            Tabela* A = ler_arquivo_ctl(operandos[0]);
            Tabela* Z = NULL;
            if(A){
                A->ler_dad();
            }else{
                delete A;
                return;
            }
            bool sim = false;
            if(operacao == 'S'){
                int coluna = busca_atributo(operandos[1], A);
                Valor v(operandos[3], A->atributos[coluna]->tipo);
                sim = S(A, coluna, operandos[2], v, Z, operandos[4]);
            }else if(operacao == 'P'){
                vector<string> lista;
                for(int i = 2; i < operandos.size() - 1; i++) {
                    lista.push_back(operandos[i]);
                } 
                sim = P(A, lista.size(), lista, Z, operandos[operandos.size()-1]);
            }else if(operacao == 'J'){
                Tabela* B = ler_arquivo_ctl(operandos[1]);
                if(B){
                    B->ler_dad();
                } else {
                    delete A;
                    delete B;
                    delete Z;
                    return;
                }
                int x = busca_atributo(operandos[2], A);
                int y = busca_atributo(operandos[3], B);
                sim = J(A, B, x, y, Z, operandos[operandos.size()-1]);
                delete B;
            }
            if(!sim){
                delete A;
                delete Z;
                return;
            }
            ultima_tabela = Z->nome;
            ofstream ctl ((Z->nome+".ctl").c_str());
            Z->imprime_ctl(ctl);
            ctl.close();
            ofstream dad ((Z->nome+".dad").c_str());
            dad<<*Z;
            dad.close();
            delete A;
            delete Z;
        }
        in.close();
        Tabela* R = ler_arquivo_ctl(ultima_tabela);
        if (R){
            R->ler_dad();
            cout << *R << endl;
        }
        delete R;
    }
}


bool processa_create_table(istream &in, string nome_arq) {
    string atual, tabela;
    in >> atual;
    istreambuf_iterator<char> eos;
    string resto(istreambuf_iterator<char>(in), eos); // le todo texto restante
    trim(resto);
    vector<string> paren;
    separar_operandos(resto, paren, '(');
    if (paren.size() != 2) {
        cout << "ERRO: Sintaxe invalida" << endl;
        return false;
    }
    trim(paren[0]);
    tabela = paren[0];
    if (*paren[1].rbegin() == ';'){
        paren[1].erase(paren[1].length() - 1);
    }
    if (*paren[1].rbegin() == ')'){
        paren[1].erase(paren[1].length() - 1);
    }
    vector<string> atributos;
    separar_operandos(paren[1], atributos, ',');
    
    
    Tabela *A = new Tabela(atributos.size(), 0, tabela);
    for (int i = 0; i < atributos.size(); i++) {
        vector<string> partes;
        separar_operandos(atributos[i], partes, ' ');
        if (partes.size() < 2) {
            cout << "ERRO: Sintaxe invalida" << endl;
            delete A;
            return false;
        }
        trim(partes[0]);
        trim(partes[1]);
        if (lower(partes[1]) != "string" && lower(partes[1]) != "integer") {
            cout << "ERRO: Tipo " << partes[1] << " invalido" << endl;
            delete A;
            return false;
        }
        A->atributos[i] = new Atributo(partes[0], lower(partes[1]));
        for (int j = 2; j < partes.size(); j++) {
            trim(partes[j]);
            if (partes[j] == "NN") {
                A->atributos[i]->notNull = 1;
            }
            if (partes[j] == "KEY") {
                A->atributos[i]->chave = 1;
            }
            if (partes[j] == "ORD") {
                A->atributos[i]->ord = 1;
            }
        }
    }
    int r = A->valida(cout);
    if (r) {
        ofstream ctl ((A->nome+".ctl").c_str());
        A->imprime_ctl(ctl);
        ctl.close();
        ofstream dad ((A->nome+".dad").c_str());
        dad<<*A;
        dad.close();
        cout<<"Tabela " << A->nome << " criada!" << endl;
    }
    
    delete A;
    return true;
}

bool processa_insert(istream &in, string nome_arq) {
    string atual, tabela;
    in >> atual >> tabela;
    istreambuf_iterator<char> eos;
    string resto(istreambuf_iterator<char>(in), eos); // le todo texto restante
    trim(resto);
    cout << resto << endl;
    vector<string> paren;
    separar_operandos(resto, paren, '(');
    if (paren.size() < 2) {
        cout << "ERRO: Sintaxe invalida" << endl;
        return false;
    }
    resto = "";
    for (int i = 1; i < paren.size(); i++) {
        trim(paren[i]);
        resto += paren[i];
    }
    if (*resto.rbegin() == ';'){
        resto.erase(resto.length() - 1);
    }
    if (*resto.rbegin() == ')'){
        resto.erase(resto.length() - 1);
    }
    Tabela *A = ler_arquivo_ctl(tabela);
    if (!A) {
        cout << "ERRO: Tabela " << A->nome << " nao existe" << endl;
        delete A;
        return false;
    }
    A->ler_dad();
    Tupla ** tuplas = new Tupla*[A->M + 1];
    memcpy(tuplas, A->tuplas, A->M*sizeof(Tupla *));
    tuplas[A->M] = new Tupla(resto, A);
    A->M += 1;
    delete [] A->tuplas;
    A->tuplas = tuplas;
    
    int i = 0;
    for(;i < A->N; i++) {
        if (A->atributos[i]->ord) {
            break;
        }
    }
    if (i != A->N) {
        ordenacao ordi(i);
        sort(A->tuplas, A->tuplas + A->M, ordi);
    }
        
    ofstream ctl ((A->nome+".ctl").c_str());
    A->imprime_ctl(ctl);
    ctl.close();
    ofstream dad ((A->nome+".dad").c_str());
    dad<<*A;
    dad.close();
    cout<<"Tupla inserida em " << A->nome << "!" << endl;
    
    delete A;
    return true;
}

int desclassifica(string& atributo, Tabela *A, string concat) {
    vector<string> classificado;
    separar_operandos(atributo, classificado, '.');
    if (classificado.size() > 1) {
        if (classificado[0] != A->nome) {
            return -1;
        }
    }
    string atr = classificado[classificado.size() - 1];
    for (int i = 0; i < A->N; i++) {
        if (lower(atr) == lower(*A->atributos[i]->nome)) {
            atributo = concat + atr;
            return 1;
        }
    }
    return -2;
}

bool processa_select(istream &in, string nome_arq) {
    // Parseia
    string atual, calculado, relacao_A;
    in >> atual;
    while(lower(atual)!="from"){
        calculado+=atual;
        in >> atual;
    }
    vector<string> atributos;
    separar_operandos(calculado, atributos);
    in >> relacao_A;
    if (*relacao_A.rbegin() == ';'){
        relacao_A.erase(relacao_A.length() - 1);
    }
    while(!in.eof() && lower(atual)!="where" && lower(atual)!="join"){
        in >> atual;
        if (lower(atual)!="where" && lower(atual)!="join" && atual != ";") {
            cout << "ERRO: Sintaxe. Esperava WHERE, JOIN ou ;";
            cout << " mas encontrou " << atual << endl;
            return false;
        }
    }
    calculado = "";
    string relacao_B;
    vector<string> cond_JOIN;
    if(lower(atual) == "join"){
        in >> relacao_B >> atual >> atual;
        while(!in.eof() && lower(atual)!="where"){
            calculado+=atual;
            in >> atual;
        }
        if (in.eof()) {
            calculado += atual;
        }
        if (*calculado.rbegin() == ';'){
            calculado.erase(calculado.length() - 1);
        }
        separar_operandos(calculado, cond_JOIN, '=');
        if (cond_JOIN.size() < 2) {
            cout << "ERRO: condicao de juncao invalida" << endl;
            return false;
        }
    }
    string T;
    string op;
    string v;
    if(lower(atual) == "where"){
        calculado = "";
        in >> atual;
        while(!in.eof()){
             calculado+=atual;
             in >> atual;
        }
        calculado += atual;
        if (*calculado.rbegin() == ';'){
            calculado.erase(calculado.length() - 1);
        }
        
        for (int i=0; i<calculado.size(); i++) {
            char c = calculado[i];
            if (c == '=' || c == '<' || c == '>') {
                T = calculado.substr(0, i);
                op += c;
                char c2 = calculado[i + 1];
                i++;
                if (c2 == '=' || c2 == '>') {
                    op += c2;
                    i++;
                }
                v = calculado.substr(i, calculado.size() - i);
                break;
            }
        }
    }

    // valida e transforma em alg
    
    string selecao;
    string projecao;
    string juncao;
    
    Tabela* A = ler_arquivo_ctl(relacao_A);
    Tabela* B;
    if (!A) {
        cout << "ERRO: Tabela " << relacao_A << " nao existe" << endl;
        delete A;
        delete B;
        return false;
    }
    atual = relacao_A;
    if (relacao_B.size() == 0) {
        for (int i = 0; i < atributos.size(); i++) {
            int d = desclassifica(atributos[i], A, "");
            if (d < 0) {
                cout << "ERRO: Atributo " << atributos[i] << " nao pertence a " << relacao_A << endl;
                delete A;
                delete B;
                return false;
            }
        }
        if (T.size()) {
            int dt = desclassifica(T, A, "");
            if (dt < 0) {
                cout << "ERRO: Atributo " << T << " nao pertence a " << relacao_A << endl;
                delete A;
                delete B;
                return false;
            }

            selecao = "S(" + relacao_A + "," + T + "," + op + "," + v + "," + atual + "SELECT)";
            atual = atual + "SELECT";
        }
        juncao = "";
    } else {
        B = ler_arquivo_ctl(relacao_B);
        if (!B) {
            cout << "ERRO: Tabela " << relacao_B << " nao existe" << endl;
            delete A;
            delete B;
            return false;
        }
        string concatA = A->nome;
        string concatB = B->nome;
        if (A->nome == B->nome) {
            concatA += "1";
            concatB += "2";
        }
        for (int i = 0; i < atributos.size(); i++) {
            string testaA = atributos[i];
            string testaB = atributos[i];
            int da = desclassifica(testaA, A, concatA);
            int db = desclassifica(testaB, B, concatB);
            
            if (da < 0 && db < 0) {
                cout << "ERRO: Atributo " << atributos[i] << " nao pertence a ";
                cout << relacao_A << " nem " << relacao_B << endl;
                delete A;
                delete B;
                return false;
            } else if (da >= 0 && db >= 0) {
                cout << "ERRO: Atributo ambiguo " << atributos[i] << " pertence a ";
                cout << relacao_A << " e " << relacao_B << endl;
                delete A;
                delete B;
                return false;
            } else if (da >= 0) {
                atributos[i] = testaA;
            } else {
                atributos[i] = testaB;
            }
        }
        int dca = desclassifica(cond_JOIN[0], A, "");
        if (dca < 0){
            cout << "ERRO: Atributo " << cond_JOIN[0] << " nao pertence a " << relacao_A << endl;
            delete A;
            delete B;
            return false; 
        }
        int dcb = desclassifica(cond_JOIN[1], B, "");
        if (dcb < 0){
            cout << "ERRO: Atributo " << cond_JOIN[1] << " nao pertence a " << relacao_B << endl;
            delete A;
            delete B;
            return false; 
        }
        
        atual = relacao_A + "JOIN" + relacao_B;
        juncao = "J(" + relacao_A + "," + relacao_B + "," + cond_JOIN[0] + "," + cond_JOIN[1] + "," + atual + ")";
                
        if (T.size()) {
            string testaA = T;
            string testaB = T;
            int da = desclassifica(testaA, A, concatA);
            int db = desclassifica(testaB, B, concatB);
            
            if (da < 0 && db < 0) {
                cout << "ERRO: Atributo " << T << " nao pertence a ";
                cout << relacao_A << " nem " << relacao_B << endl;
                delete A;
                delete B;
                return false;
            } else if (da >= 0 && db >= 0) {
                cout << "ERRO: Atributo ambiguo " << T << " pertence a ";
                cout << relacao_A << " e " << relacao_B << endl;
                delete A;
                delete B;
                return false;
            } else if (da >= 0) {
                T = testaA;
            } else {
                T = testaB;
            }
            selecao = "S(" + atual + "," + T + "," + op + "," + v + "," + atual + "SELECT)";
            atual += "SELECT";
        }
        
    }
    stringstream ss;
    ss << "P(" << atual << "," << atributos.size() << ",";
    for (int i = 0; i < atributos.size(); i++) {
        ss << atributos[i] << ",";
    }
    ss << atual + "PROJ)"; 
    projecao = ss.str();
    ofstream alg ((nome_arq + ".alg").c_str());
    if (juncao.size()) alg << juncao << endl;
    if (selecao.size()) alg << selecao << endl;
    if (projecao.size()) alg << projecao << endl;
    alg.close();
    delete A;
    delete B;
    return true;
}

bool processa_sql(istream &in, string nome_arq) {
    string comando;
    in >> comando;
    if (lower(comando) == "create") {
        return processa_create_table(in, nome_arq);
    } else if (lower(comando) == "insert") {
        return processa_insert(in, nome_arq);
    } else if (lower(comando) == "select") {
        if (processa_select(in, nome_arq)) {
            ler_arquivo_alg(nome_arq);
            return true;
        }
        return false;
    }
}


int main(int argc, char** argv) {
    /*
    Valor inteiro("-5",'I');
    Valor texto("'oie'", 'C');
    Valor Nulo("NULO", 'I');
    cout<<inteiro<<texto<<Nulo;
    cout << endl;
    
    Atributo* atributo = new Atributo("A,I,nn,chv");
    cout<<*atributo;
    delete (atributo);
    cout << endl;
    
    Tabela* tabela = new Tabela(4, 4, "tab");
    tabela->atributos[0] = new Atributo("A,I,nn,chv");
    tabela->atributos[1] = new Atributo("BX,I");
    tabela->atributos[2] = new Atributo("CYZ,C,nn,ord");
    tabela->atributos[3] = new Atributo("XZ,C");
    tabela->imprime_ctl(cout);
    cout << endl;
    
    Tupla* tupla = new Tupla("50,33,'Banco de Dados',''", tabela);
    cout<<*tupla;
    delete (tupla);
    cout << endl;
    
    tabela->tuplas[0] = new Tupla("50,33,'Banco de Dados',''", tabela);
    tabela->tuplas[1] = new Tupla("777,NULO,'Calculo I',' '", tabela);
    tabela->tuplas[2] = new Tupla("2,43,'Compiladores',NULO", tabela);
    tabela->tuplas[3] = new Tupla("51,1,'Estrutura de Dados','NULO'", tabela);
    cout << *tabela;
    cout << endl;
    
    int res = tabela->valida(cout);
    cout << res << endl;
    Valor v(33);
    Tabela *Z = NULL;
    bool sel = S(tabela, busca_atributo("BX", tabela), "<>", v, Z, "Z");
    if (sel) {
        Z->imprime_ctl(cout);
        cout<<*Z;
    }
    
    delete Z;
    Z = NULL;
    vector<string> lista;
    lista.push_back("XZ");
    lista.push_back("BX");
    bool pro = P(tabela, 2, lista, Z, "Z");
    if (pro){
        Z->imprime_ctl(cout);
        cout<<*Z;
    }
    delete Z;
    delete (tabela);
    
    Tabela* A = ler_arquivo_ctl("A");
    A->imprime_ctl(cout);
    bool foi = A->ler_dad();
    if(foi){
        cout<<"leu dad";
    }
//    cout<<*A;
    J(A, A, busca_atributo("A", A), busca_atributo("A", A), Z, "Z");
    delete A;
    cout << *Z << endl;
    Z->imprime_ctl(cout);
    delete Z;
    ler_arquivo_alg("consulta1");
    */
    string arquivo;
    if (argc > 1) {
        arquivo = argv[1];
        if (arquivo[arquivo.size() - 4] == '.') {
            arquivo = arquivo.substr(0, arquivo.size() - 4);
        }
    } else {
        arquivo = "comando";
    }
    ifstream arquivo_sql((arquivo + ".sql").c_str());
    if(arquivo_sql.is_open()){
        processa_sql(arquivo_sql, arquivo);
        arquivo_sql.close();
    }
    return (EXIT_SUCCESS);
}