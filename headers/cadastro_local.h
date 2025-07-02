#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#pragma once
using namespace std;

const int TAM_NOME_LOCAL = 50;

class Local
{
private:
    char nome[TAM_NOME_LOCAL];
    float x, y;
    bool ativo;

public:
    Local() : x(0), y(0), ativo(false) {
        nome[0] = '\0';
    }

    Local(const string& nome_str, float x, float y) {
        strncpy(this->nome, nome_str.c_str(), TAM_NOME_LOCAL - 1);
        this->nome[TAM_NOME_LOCAL - 1] = '\0';
        this->x = x;
        this->y = y;
        this->ativo = true;
    }

    string get_nome() const { return string(nome); }
    float get_x() const { return x; }
    float get_y() const { return y; }
    bool is_ativo() const { return ativo; }

    void change_nome(const string& novo_nome) {
        strncpy(this->nome, novo_nome.c_str(), TAM_NOME_LOCAL - 1);
        this->nome[TAM_NOME_LOCAL - 1] = '\0';
    }
    void change_x(float novo_x) { this->x = novo_x; }
    void change_y(float novo_y) { this->y = novo_y; }
    void desativar() { this->ativo = false; }
};


class GerenciadorLocais
{
private:
    const string nomeArquivo = "locais.dat";
    int encontrar_pos_local(const string& nome);

public:
    vector<Local> locais; // Temporário para compatibilidade
    GerenciadorLocais();
    bool deletar_local(const string& local);
    bool criar_local(const string& nome, float x, float y);
    void listar_locais();
    bool atualizar(const string& nome_atual, const string& novo_nome, float x, float y);
    vector<Local> get_todos_locais();
};