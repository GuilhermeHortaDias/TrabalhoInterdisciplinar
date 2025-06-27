#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Pedido
{
public:
    int id;
    string origem;
    string destino;
    float peso;
    Pedido(int id, const string &origem, const string &destino, float peso)
        : id(id), origem(origem), destino(destino), peso(peso) {}
};

    class GerenciadorPedidos
    {
    public:

        bool localExiste(const vector<string> &locais, const string &local)
                {
                    return find(locais.begin(), locais.end(), local) != locais.end();
                }

        void cadastrarPedido(vector<Pedido> &pedidos, const vector<string> &locais)
        {
                static int contadorId = 1;

                string origem, destino;
                float peso;

                cout << "\nDigite a origem: ";
                getline(cin >> ws, origem);

                cout << "Digite o destino: ";
                getline(cin >> ws, destino);

               cout << "Digite o peso (em kg): ";
               cin >> peso;

                if (!localExiste(locais, origem))
                {
                    cout << "Erro: A origem \"" << origem << "\" nao existe." << endl;
                    return;
                }
                if (!localExiste(locais, destino))
                {
                    cout << "Erro: O destino \"" << destino << "\" nao existe." << endl;
                    return;
                }

                Pedido novoPedido(contadorId++, origem, destino, peso);
                pedidos.push_back(novoPedido);

                cout << "\nPedido cadastrado com sucesso! ID: " << novoPedido.id << endl;
        }

        void listarPedidos(const vector<Pedido> &pedidos)
        {
            if (pedidos.empty())
            {
                cout << "Nenhum pedido cadastrado." << endl;
                return;
            }

            cout << "\nListagem de Pedidos:\n"
                << endl;
            for (const auto &pedido : pedidos)
            {
                cout << "\nID: " << pedido.id << "\n Origem: " << pedido.origem
                    << "\n Destino: " << pedido.destino << "\n Peso: " << pedido.peso << " kg" << endl;
            }
        }

        void excluirPedido(vector<Pedido> &pedidos)
        {
        if (pedidos.empty())
        {
            cout << "Nenhum pedido cadastrado para excluir." << endl;
            return;
        }

        int id;
        cout << "Digite o ID do pedido que deseja excluir: ";
        cin >> id;

        auto it = remove_if(pedidos.begin(), pedidos.end(), [id](const Pedido &p) {
            return p.id == id;
        });

        if (it == pedidos.end())
        {
            cout << "Pedido com ID " << id << " nao encontrado." << endl;
        }
        else
        {
            pedidos.erase(it, pedidos.end());
            cout << "Pedido com ID " << id << " excluido com sucesso!" << endl;
        }
    }
        void editarPedido(vector<Pedido> &pedidos, const vector<string> &locais)
        {
            if (pedidos.empty())
            {
                cout << "Nenhum pedido cadastrado para editar." << endl;
                return;
            }

            int id;
            cout << "Digite o ID do pedido que deseja editar: ";
            cin >> id;

            for (auto &pedido : pedidos)
            {
                if (pedido.id == id)
                {
                    string novaOrigem, novoDestino;
                    float novoPeso;

                    cout << "\nNova origem (atual: " << pedido.origem << "): ";
                    getline(cin >> ws, novaOrigem);

                    cout << "Novo destino (atual: " << pedido.destino << "): ";
                    getline(cin >> ws, novoDestino);

                    cout << "Novo peso (atual: " << pedido.peso << " kg): ";
                    cin >> novoPeso;

                    if (!localExiste(locais, novaOrigem))
                    {
                        cout << "Erro: A nova origem \"" << novaOrigem << "\" nao existe." << endl;
                        return;
                    }
                    if (!localExiste(locais, novoDestino))
                    {
                        cout << "Erro: O novo destino \"" << novoDestino << "\" nao existe." << endl;
                        return;
                    }

                    pedido.origem = novaOrigem.empty() ? pedido.origem : novaOrigem;
                    pedido.destino = novoDestino.empty() ? pedido.destino : novoDestino;
                    pedido.peso = novoPeso > 0 ? novoPeso : pedido.peso;

                    cout << "\nPedido com ID " << id << " editado com sucesso!" << endl;
                    return;
                }
            }

        cout << "Pedido com ID " << id << " nao encontrado." << endl;
    }

    };





int main()
{
    vector<string> locais = {"Sao Paulo", "Rio de Janeiro", "Belo Horizonte", "Curitiba", "Rio Branco", "Maceio", "Macapa", "Manaus", "Salvador", "Fortaleza", "Vitoria", "Goiania", "Sao Luis", "Cuiaba", "Campo Grande", "Belem", "Joao Pessoa", "Recife", "Teresina", "Natal", "Porto Alegre", "Porto Velho", "Boa Vista", "Florianopolis", "Aracaju", "Palmas", "Brasilia"};
    vector<Pedido> pedidos;

    GerenciadorPedidos gerenciador;

    int opcao;

    while (true)
    {
        cout << "\nMenu:\n";
        cout << "1. Cadastrar Pedido" << endl;
        cout << "2. Listar Pedidos" << endl;
        cout << "3. Editar Pedido" << endl;
        cout << "4. Excluir Pedido" << endl;
        cout << "5. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao)
        {
        case 1:
            gerenciador.cadastrarPedido(pedidos, locais);
            break;
        case 2:
            gerenciador.listarPedidos(pedidos);
            break;
        case 3:
            gerenciador.editarPedido(pedidos, locais);
            break;
        case 4:
            gerenciador.excluirPedido(pedidos);
            break;
        case 5:
            cout << "Encerrando o sistema... " << endl;
            return 0;
        default:
            cout << "Opcao invalida. Tente novamente." << endl;
        }
    }

    return 0;
}
