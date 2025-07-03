#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

const int TAM_NOME_LOCAL = 50;

class Local
{
private:
    char nome[TAM_NOME_LOCAL];
    float x, y;
    bool ativo;

public:
    Local();
    Local(const string& nome_str, float x, float y);

    string get_nome() const;
    float get_x() const;
    float get_y() const;
    bool is_ativo() const;

    void change_nome(const string& novo_nome);
    void change_x(float novo_x);
    void change_y(float novo_y);
    void desativar();
};

class GerenciadorLocais
{
private:
    const string nomeArquivo = "locais.dat";
    int encontrar_pos_local(const string& nome);

public:
    GerenciadorLocais();
    bool deletar_local(const string& nome);
    bool criar_local(const string& nome, float x, float y);
    void listar_locais();
    bool atualizar_local(const string& nome_atual, const string& novo_nome, float x, float y);
    vector<Local> get_todos_locais();
    Local get_local_by_nome(const string& nome);
};