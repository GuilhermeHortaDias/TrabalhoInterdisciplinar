#include "../headers/cadastro_pedido.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

GerenciadorPedidos::GerenciadorPedidos() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        ofstream novoArquivo(nomeArquivo, ios::binary);
        novoArquivo.close();
    }
}

int GerenciadorPedidos::encontrar_pos_pedido(int id) {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) return -1;

    Pedido pedido_lido;
    int pos = 0;
    while (arquivo.read(reinterpret_cast<char*>(&pedido_lido), sizeof(Pedido))) {
        if (pedido_lido.is_ativo() && pedido_lido.get_id() == id) {
            return pos;
        }
        pos++;
    }
    return -1;
}

int GerenciadorPedidos::get_next_id() {
    ifstream arquivo(nomeArquivo, ios::binary | ios::ate);
    if (!arquivo || arquivo.tellg() == 0) {
        return 1; // Começa do 1 se o arquivo estiver vazio ou não existir
    }
    // Pega a posição do último registro
    arquivo.seekg(long(arquivo.tellg()) - long(sizeof(Pedido)));
    Pedido ultimo_pedido;
    arquivo.read(reinterpret_cast<char*>(&ultimo_pedido), sizeof(Pedido));
    return ultimo_pedido.get_id() + 1;
}

bool GerenciadorPedidos::criar_pedido(const string& item, int quantidade, float valor, const string& local_entrega, const string& local_retirada) {
    int novo_id = get_next_id();

    Pedido novo_pedido(novo_id, item, quantidade, valor, local_entrega, local_retirada);
    ofstream arquivo(nomeArquivo, ios::binary | ios::app);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo para escrita." << endl;
        return false;
    }

    arquivo.write(reinterpret_cast<const char*>(&novo_pedido), sizeof(Pedido));
    cout << "Pedido criado com sucesso! ID: " << novo_id << endl;
    return true;
}

bool GerenciadorPedidos::deletar_pedido(int id) {
    int pos = encontrar_pos_pedido(id);
    if (pos == -1) {
        cout << "Erro: Pedido não encontrado." << endl;
        return false;
    }

    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    arquivo.seekg(pos * sizeof(Pedido));
    Pedido pedido_a_deletar;
    arquivo.read(reinterpret_cast<char*>(&pedido_a_deletar), sizeof(Pedido));

    pedido_a_deletar.desativar();

    arquivo.seekp(pos * sizeof(Pedido));
    arquivo.write(reinterpret_cast<const char*>(&pedido_a_deletar), sizeof(Pedido));

    cout << "Pedido ID " << id << " deletado com sucesso!" << endl;
    return true;
}

void GerenciadorPedidos::listar_pedidos() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        cout << "Nenhum pedido cadastrado." << endl;
        return;
    }

    Pedido pedido_lido;
    bool encontrou = false;
    cout << "\n--- Lista de Pedidos ---" << endl;
    while (arquivo.read(reinterpret_cast<char*>(&pedido_lido), sizeof(Pedido))) {
        if (pedido_lido.is_ativo()) {
            cout << "ID: " << pedido_lido.get_id()
                 << ", Item: " << pedido_lido.get_item()
                 << ", Qtd: " << pedido_lido.get_quantidade()
                 << ", Valor: " << pedido_lido.get_valor()
                 << ", Entrega: " << pedido_lido.get_local_entrega()
                 << ", Retirada: " << pedido_lido.get_local_retirada() << endl;
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhum pedido ativo cadastrado." << endl;
    }
    cout << "------------------------" << endl;
}

vector<Pedido> GerenciadorPedidos::get_todos_pedidos() {
    vector<Pedido> pedidos_ativos;
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        return pedidos_ativos;
    }

    Pedido pedido_lido;
    while (arquivo.read(reinterpret_cast<char*>(&pedido_lido), sizeof(Pedido))) {
        if (pedido_lido.is_ativo()) {
            pedidos_ativos.push_back(pedido_lido);
        }
    }

    return pedidos_ativos;
}

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