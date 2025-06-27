#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

class Local
{
private:
    string nome;
    float x, y;

public:
    Local(string nome, float x, float y)
    {
        this->nome = nome;
        this->x = x;
        this->y = y;
    }


    // getters
    string get_nome()
    {
        return nome;
    }
    float get_x()
    {
        return x;
    }
        float get_y()
    {
        return y;
    }


    void change_nome(string nome)
    {
        this->nome = nome;
    }
    void change_x(float x)
    {
        this->x = x;
    }
    void change_y(float y)
    {
        this->y = y;
    }
};



class GerenciadorLocais
{
    public:
    vector<Local> locais;
    
    GerenciadorLocais()
    {
        vector<Local> locais;
    }


        void deletar_local(string local)
        {
            try
                {
                    for (int i = 0; i < locais.size(); i++)
                    {
                        if (local == locais[i].get_nome())
                        {
                            locais.erase(locais.begin() + i);
                            cout << "Removido com sucesso" << endl;
                            return;
                        }
                    }
                    throw 404;
                }
            catch (int error)
            {
                cout << "Nao foi possivel encontrar a conta para remover" << endl;
            }
        }


        void criar_local(string nome, float x, float y)
        {
          Local local(nome, x, y);
          locais.push_back(local);
          cout << "Local criado com sucesso" << endl;
        }


        void listar_locais(void)
        {
            for (int i = 0; i < locais.size(); i++) // itera por todos os locais no vetor local e printa todos para o usuario.
            {
                cout << "Local: " << locais[i].get_nome() << endl;
                cout << "Coordenadas: " << locais[i].get_x() << " , " << locais[i].get_y() << endl;
            }
        }

        void atualizar(string nome_atual, string novo_nome, float x, float y)
        {
            try
                {
                    for (int i = 0; i < locais.size(); i++)
                    {
                        if (nome_atual == locais[i].get_nome() && nome_atual != novo_nome)
                        {
                            locais[i].change_nome(novo_nome);
                            locais[i].change_x(x);
                            locais[i].change_y(y);
                            return;
                        }
                        else if (nome_atual != novo_nome)
                        {
                            throw 505;
                        }
                    }
                    throw 404;
                }
            catch (int num)
            {
                if (num == 404)
                    {
                        cout << "Nao foi possivel encontrar o local" << endl;
                    }
                else
                {
                    cout << "Esse local ja existe" << endl;
                }
            }
        

        }
};



int main(void)
{
    GerenciadorLocais gerenciador;
    gerenciador.criar_local("teste", 10.3, 9.0);
    gerenciador.listar_locais();

}