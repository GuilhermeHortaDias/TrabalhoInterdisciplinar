#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Pedido {
public:
    int id;
    string origem;
    string destino;
    float peso;
    Pedido(int id, const string& origem, const string& destino, float peso)
        : id(id), origem(origem), destino(destino), peso(peso) {}
};

bool localExiste(const vector<string>& locais, const string& local) {
    return find(locais.begin(), locais.end(), local) != locais.end();
}

void cadastrarPedido(vector<Pedido>& pedidos, const vector<string>& locais) {
    static int contadorId = 1;

    string origem, destino;
    float peso;

    cout << "\nDigite a origem: ";
    getline(cin >> ws, origem);

    cout << "Digite o destino: ";
    getline(cin >> ws, destino);

    cout << "Digite o peso (em kg): ";
    cin >> peso;

    if (!localExiste(locais, origem)) {
        cout << "Erro: A origem \"" << origem << "\" nao existe." << endl;
        return;
    }
    if (!localExiste(locais, destino)) {
        cout << "Erro: O destino \"" << destino << "\" nao existe." << endl;
        return;
    }

    Pedido novoPedido(contadorId++, origem, destino, peso);
    pedidos.push_back(novoPedido);

    cout << "\nPedido cadastrado com sucesso! ID: " << novoPedido.id << endl;
}

void listarPedidos(const vector<Pedido>& pedidos) {
    if (pedidos.empty()) {
        cout << "Nenhum pedido cadastrado." << endl;
        return;
    }

    cout << "\nListagem de Pedidos:\n" << endl;
    for (const auto& pedido : pedidos) {
        cout << "\nID: " << pedido.id << "\n Origem: " << pedido.origem
             << "\n Destino: " << pedido.destino << "\n Peso: " << pedido.peso << " kg" << endl;
    }
}

int main() {
    vector<string> locais = {"Sao Paulo", "Rio de Janeiro", "Belo Horizonte", "Curitiba", "Rio Branco", "Maceio", "Macapa", "Manaus", "Salvador", "Fortaleza", "Vitoria", "Goiania", "Sao Luis", "Cuiaba", "Campo Grande", "Belem", "Joao Pessoa", "Recife", "Teresina", "Natal", "Porto Alegre", "Porto Velho", "Boa Vista", "Florianopolis", "Aracaju", "Palmas", "Brasilia"};
    vector<Pedido> pedidos;

    int opcao;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Cadastrar Pedido\n";
        cout << "2. Listar Pedidos\n";
        cout << "3. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                cadastrarPedido(pedidos, locais);
                break;
            case 2:
                listarPedidos(pedidos);
                break;
            case 3:
                cout << "Encerrando o sistema... " << endl;
                return 0;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    }

    return 0;
}
