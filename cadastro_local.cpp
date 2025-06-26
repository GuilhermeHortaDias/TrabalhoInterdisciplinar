#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

class Local
{
public:
    string nome;
    float x, y;

    Local(string nome, float x, float y)
    {
        this->nome = nome;
        this->x = x;
        this->y = y;
    }

    void atualizar()
    {
        
        cout << "Corrija o nome: ";
        cin >> this->nome;
        cout << "Corrija a coordenada X: ";
        cin >> this->x;
        cout << "Corrija a coordenada Y: ";
        cin >> this->y;
        cout << "Correção Realizda com sucesso." << endl;

    }
};

vector<Local> locais;
void criar_local(void)
{
    int quantidade = 0;

    while (true)
    {
        string nome;
        float x, y;
        bool existe = false;

        cout << "Digite o nome do local: ";
        getline(cin, nome);
        if (nome == "")
        {
            break;
        }

        if (quantidade > 0)
        {
            for (int i = 0; i < quantidade; i++)
            {
                if (locais[i].nome == nome)
                {
                    cout << "Ja existe esse local" << endl;
                    existe = true;
                    break; // sai do loop que checa se existe nome igual
                }
            }
        }

        if (existe == true)
        {
            continue; // se ja exisir um lugar com o nome, vai ir para a proxima iteração
        }

        cout << "Digite o valor x da coordenada: ";
        cin >> x;
        cout << "Digite o valor y da coordenada: ";
        cin >> y;
        Local novo_local(nome, x, y);
        locais.push_back(novo_local);
        quantidade++;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void listar_locais()
{
    for (int i = 0; i < locais.size(); i++) // itera por todos os locais no vetor local e printa todos para o usuario.
    {
        cout << "Local: " << locais[i]. nome << endl;
        cout << "Coordenadas: " << locais[i].x << " , " << locais[i].y << endl;
    }
}

void deletar_local(string local)
{
    try
        {
            for (int i = 0; i < locais.size(); i++)
            {
                if (local == locais[i].nome)
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


int main(void)
{
    criar_local();
    listar_locais();
    locais[0].atualizar();
    deletar_local("teste");
}