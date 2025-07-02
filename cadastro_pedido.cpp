#include "cadastro_pedido.h"
#include "cadastro_local.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

Pedido::Pedido(int id, const string &origem, const string &destino, float peso)
    : id(id), origem(origem), destino(destino), peso(peso) {}

int Pedido::get_id() const { return id; }
string Pedido::get_origem() const { return origem; }
string Pedido::get_destino() const { return destino; }
float Pedido::get_peso() const { return peso; }

void Pedido::set_origem(const string &novaOrigem) { origem = novaOrigem; }
void Pedido::set_destino(const string &novoDestino) { destino = novoDestino; }
void Pedido::set_peso(float novoPeso) { peso = novoPeso; }

bool GerenciadorPedidos::localExiste(const vector<string> &locais, const string &local) const
{
    return find(locais.begin(), locais.end(), local) != locais.end();
}

void GerenciadorPedidos::cadastrarPedido(vector<Pedido> &pedidos, GerenciadorLocais &gerenciadorLocais)
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

    vector<string> locaisValidos;
    for (const auto &local : gerenciadorLocais.locais) {
        locaisValidos.push_back(local.get_nome());
    }

    if (!localExiste(locaisValidos, origem)) {
        cout << "Erro: A origem \"" << origem << "\" nao existe." << endl;
        return;
    }
    if (!localExiste(locaisValidos, destino)) {
        cout << "Erro: O destino \"" << destino << "\" nao existe." << endl;
        return;
    }

    Pedido novoPedido(contadorId++, origem, destino, peso);
    pedidos.push_back(novoPedido);

    cout << "\nPedido cadastrado com sucesso! ID: " << novoPedido.get_id() << endl;
}

void GerenciadorPedidos::listarPedidos(const vector<Pedido> &pedidos) const
{
    if (pedidos.empty()) {
        cout << "Nenhum pedido cadastrado." << endl;
        return;
    }

    cout << "\nListagem de Pedidos:\n" << endl;
    for (const auto &pedido : pedidos) {
        cout << "\nID: " << pedido.get_id()
             << "\n Origem: " << pedido.get_origem()
             << "\n Destino: " << pedido.get_destino()
             << "\n Peso: " << pedido.get_peso() << " kg" << endl;
    }
}

void GerenciadorPedidos::excluirPedido(vector<Pedido> &pedidos)
{
    if (pedidos.empty()) {
        cout << "Nenhum pedido cadastrado para excluir." << endl;
        return;
    }

    int id;
    cout << "Digite o ID do pedido que deseja excluir: ";
    cin >> id;

    auto it = remove_if(pedidos.begin(), pedidos.end(), [id](const Pedido &p) {
        return p.get_id() == id;
    });

    if (it == pedidos.end()) {
        cout << "Pedido com ID " << id << " nao encontrado." << endl;
    } else {
        pedidos.erase(it, pedidos.end());
        cout << "Pedido com ID " << id << " excluido com sucesso!" << endl;
    }
}

void GerenciadorPedidos::editarPedido(vector<Pedido> &pedidos, GerenciadorLocais &gerenciadorLocais)
{
    if (pedidos.empty()) {
        cout << "Nenhum pedido cadastrado para editar." << endl;
        return;
    }

    int id;
    cout << "Digite o ID do pedido que deseja editar: ";
    cin >> id;

    for (auto &pedido : pedidos) {
        if (pedido.get_id() == id) {
            string novaOrigem, novoDestino;
            float novoPeso;

            cout << "\nNova origem (atual: " << pedido.get_origem() << "): ";
            getline(cin >> ws, novaOrigem);

            cout << "Novo destino (atual: " << pedido.get_destino() << "): ";
            getline(cin >> ws, novoDestino);

            cout << "Novo peso (atual: " << pedido.get_peso() << " kg): ";
            cin >> novoPeso;

            vector<string> locaisValidos;
            for (const auto &local : gerenciadorLocais.locais) {
                locaisValidos.push_back(local.get_nome());
            }

            if (!localExiste(locaisValidos, novaOrigem)) {
                cout << "Erro: A nova origem \"" << novaOrigem << "\" nao existe." << endl;
                return;
            }
            if (!localExiste(locaisValidos, novoDestino)) {
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

int main()
{
    GerenciadorLocais gerenciadorLocais;
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