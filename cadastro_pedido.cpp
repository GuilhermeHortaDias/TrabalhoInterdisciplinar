#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Pedido
{
private:
    int id;
    string origem;
    string destino;
    float peso;

public:
    Pedido(int id, const string &origem, const string &destino, float peso)
        : id(id), origem(origem), destino(destino), peso(peso) {}

    int get_id() const { return id; }
    string get_origem() const { return origem; }
    string get_destino() const { return destino; }
    float get_peso() const { return peso; }

    void set_origem(const string &novaOrigem) { origem = novaOrigem; }
    void set_destino(const string &novoDestino) { destino = novoDestino; }
    void set_peso(float novoPeso) { peso = novoPeso; }
};

class GerenciadorPedidos
{
private:
    bool localExiste(const vector<string> &locais, const string &local) const
    {
        return find(locais.begin(), locais.end(), local) != locais.end();
    }

public:
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

        cout << "\nPedido cadastrado com sucesso! ID: " << novoPedido.get_id() << endl;
    }

    void listarPedidos(const vector<Pedido> &pedidos) const
    {
        if (pedidos.empty())
        {
            cout << "Nenhum pedido cadastrado." << endl;
            return;
        }

        cout << "\nListagem de Pedidos:\n" << endl;
        for (const auto &pedido : pedidos)
        {
            cout << "\nID: " << pedido.get_id()
                 << "\n Origem: " << pedido.get_origem()
                 << "\n Destino: " << pedido.get_destino()
                 << "\n Peso: " << pedido.get_peso() << " kg" << endl;
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
            return p.get_id() == id;
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
            if (pedido.get_id() == id)
            {
                string novaOrigem, novoDestino;
                float novoPeso;

                cout << "\nNova origem (atual: " << pedido.get_origem() << "): ";
                getline(cin >> ws, novaOrigem);

                cout << "Novo destino (atual: " << pedido.get_destino() << "): ";
                getline(cin >> ws, novoDestino);

                cout << "Novo peso (atual: " << pedido.get_peso() << " kg): ";
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

                pedido.set_origem(novaOrigem.empty() ? pedido.get_origem() : novaOrigem);
                pedido.set_destino(novoDestino.empty() ? pedido.get_destino() : novoDestino);
                pedido.set_peso(novoPeso > 0 ? novoPeso : pedido.get_peso());

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