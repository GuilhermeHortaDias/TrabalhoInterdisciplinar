#include <string>
#include <vector>
#pragma once
using namespace std;

class Local
{
    private:
        string nome;
        float x, y;
    public:
        Local(string nome, float x, float y);
        string get_nome();
        float get_x();
        float get_y();
        void change_nome(string nome);
        void change_x(float x);
        void change_y(float y);
};


class GerenciadorLocais
{
    public:
        vector<Local> locais;
        GerenciadorLocais();
        bool deletar_local(string local);
        bool criar_local(string nome, float x, float y);
        bool listar_locais(void);
        bool atualizar(string nome_atual, string novo_nome, float x, float y);
};