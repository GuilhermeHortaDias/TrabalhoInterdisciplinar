#include <iostream>
#include <limits>
#include "cadastro_local.h"
using namespace std;

//Cria os metodos da classe local
Local::Local(string nome, float x, float y)
    {
        this->nome = nome;
        this->x = x;
        this->y = y;
    }

    // getters
    string Local::get_nome()
    {
        return nome;
    }
    float Local::get_x()
    {
        return x;
    }
    float Local::get_y()
    {
        return y;
    }


    void Local::change_nome(string nome)
    {
        this->nome = nome;
    }
    void Local::change_x(float x)
    {
        this->x = x;
    }
    void Local::change_y(float y)
    {
        this->y = y;
    }



//Cria os metodos da classe gerenciadora
GerenciadorLocais::GerenciadorLocais()
{

}


        bool GerenciadorLocais::deletar_local(string local)
        {
            try
                {
                    for (int i = 0; i < locais.size(); i++)
                    {
                        if (local == locais[i].get_nome())
                        {
                            locais.erase(locais.begin() + i);
                            cout << "Removido com sucesso" << endl;
                            return true;
                        }
                    }
                    throw 404;
                }
            catch (int error)
            {
                cout << "Nao foi possivel encontrar a conta para remover" << endl;
                return false;
            }
        }


        bool GerenciadorLocais::criar_local(string nome, float x, float y)
        {
            for (int i = 0; i < locais.size(); i++)
            {
                if (nome == locais[i].get_nome())
                {
                    cout << "Ja existe um lugar com esse nome" << endl;
                    return false;
                }
            }
          Local local(nome, x, y);
          locais.push_back(local);
          cout << "Local criado com sucesso" << endl;
          return true;
        }


        bool GerenciadorLocais::listar_locais(void)
        {
            for (int i = 0; i < locais.size(); i++) // itera por todos os locais no vetor local e printa todos para o usuario.
            {
                cout << "Local: " << locais[i].get_nome() << endl;
                cout << "Coordenadas: " << locais[i].get_x() << " , " << locais[i].get_y() << endl;
            }
            return true;
        }

        bool GerenciadorLocais::atualizar(string nome_atual, string novo_nome, float x, float y)
        {
            // Verifica se já existe outro local com o novo nome
            for (int i = 0; i < locais.size(); i++) {
                if (locais[i].get_nome() == novo_nome && nome_atual != novo_nome) {
                    cout << "Esse local ja existe" << endl;
                    return false;
                }
            }
            // Procura o local a ser atualizado
            for (int i = 0; i < locais.size(); i++) {
                if (locais[i].get_nome() == nome_atual) {
                    locais[i].change_nome(novo_nome);
                    locais[i].change_x(x);
                    locais[i].change_y(y);
                    return true;
                }
            }
            cout << "Nao foi possivel encontrar o local" << endl;
            return false;
        }
