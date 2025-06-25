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
};

void criar_local(void)
{
    int quantidade = 0;
    vector<Local> locais;

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
