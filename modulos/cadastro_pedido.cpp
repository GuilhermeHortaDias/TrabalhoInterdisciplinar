#include "../headers/cadastro_pedido.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <limits>

using namespace std;

// Implementação dos métodos da classe Pedido
Pedido::Pedido() : id(0), peso(0.0f), ativo(false) {
    origem[0] = '\0';
    destino[0] = '\0';
}

Pedido::Pedido(int id, const string& origem_str, const string& destino_str, float peso) 
    : id(id), peso(peso), ativo(true) {
    strncpy(this->origem, origem_str.c_str(), MAX_NOME_LOCAL_PEDIDO - 1);
    this->origem[MAX_NOME_LOCAL_PEDIDO - 1] = '\0';
    strncpy(this->destino, destino_str.c_str(), MAX_NOME_LOCAL_PEDIDO - 1);
    this->destino[MAX_NOME_LOCAL_PEDIDO - 1] = '\0';
}

int Pedido::get_id() const { return id; }
string Pedido::get_origem() const { return string(origem); }
string Pedido::get_destino() const { return string(destino); }
float Pedido::get_peso() const { return peso; }
bool Pedido::is_ativo() const { return ativo; }

void Pedido::set_origem(const string& nova_origem) {
    strncpy(this->origem, nova_origem.c_str(), MAX_NOME_LOCAL_PEDIDO - 1);
    this->origem[MAX_NOME_LOCAL_PEDIDO - 1] = '\0';
}

void Pedido::set_destino(const string& novo_destino) {
    strncpy(this->destino, novo_destino.c_str(), MAX_NOME_LOCAL_PEDIDO - 1);
    this->destino[MAX_NOME_LOCAL_PEDIDO - 1] = '\0';
}

void Pedido::set_peso(float novo_peso) { this->peso = novo_peso; }
void Pedido::desativar() { this->ativo = false; }

// Implementação dos métodos da classe GerenciadorPedidos
GerenciadorPedidos::GerenciadorPedidos() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        ofstream novoArquivo(nomeArquivo, ios::binary);
        novoArquivo.close();
    }
}

int GerenciadorPedidos::get_next_id() {
    ifstream arquivo(nomeArquivo, ios::binary | ios::ate);
    if (!arquivo || arquivo.tellg() == 0) {
        return 1; // Começa do 1 se o arquivo estiver vazio
    }
    arquivo.seekg(-static_cast<long>(sizeof(Pedido)), ios::end);
    Pedido ultimo_pedido;
    arquivo.read(reinterpret_cast<char*>(&ultimo_pedido), sizeof(Pedido));
    return ultimo_pedido.get_id() + 1;
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

bool GerenciadorPedidos::criar_pedido(const string& origem, const string& destino, float peso) {
    int novo_id = get_next_id();
    Pedido novo_pedido(novo_id, origem, destino, peso);

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
        cout << "Erro: Pedido com ID " << id << " não encontrado." << endl;
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
    bool encontrou_ativo = false;
    cout << "--- Lista de Pedidos ---" << endl;
    while (arquivo.read(reinterpret_cast<char*>(&pedido_lido), sizeof(Pedido))) {
        if (pedido_lido.is_ativo()) {
            cout << "ID: " << pedido_lido.get_id()
                 << ", Origem: " << pedido_lido.get_origem()
                 << ", Destino: " << pedido_lido.get_destino()
                 << ", Peso: " << pedido_lido.get_peso() << " kg" << endl;
            encontrou_ativo = true;
        }
    }

    if (!encontrou_ativo) {
        cout << "Nenhum pedido ativo encontrado." << endl;
    }
    cout << "------------------------" << endl;
}

bool GerenciadorPedidos::atualizar_pedido(int id, const string& nova_origem, const string& novo_destino, float novo_peso) {
    int pos = encontrar_pos_pedido(id);
    if (pos == -1) {
        cout << "Erro: Pedido com ID " << id << " não encontrado." << endl;
        return false;
    }

    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    arquivo.seekg(pos * sizeof(Pedido));
    Pedido pedido_a_atualizar;
    arquivo.read(reinterpret_cast<char*>(&pedido_a_atualizar), sizeof(Pedido));

    pedido_a_atualizar.set_origem(nova_origem);
    pedido_a_atualizar.set_destino(novo_destino);
    pedido_a_atualizar.set_peso(novo_peso);

    arquivo.seekp(pos * sizeof(Pedido));
    arquivo.write(reinterpret_cast<const char*>(&pedido_a_atualizar), sizeof(Pedido));

    cout << "Pedido ID " << id << " atualizado com sucesso!" << endl;
    return true;
}

vector<Pedido> GerenciadorPedidos::get_todos_pedidos() {
    ifstream arquivo(nomeArquivo, ios::binary);
    vector<Pedido> pedidos;
    if (!arquivo) {
        return pedidos;
    }

    Pedido pedido_lido;
    while (arquivo.read(reinterpret_cast<char*>(&pedido_lido), sizeof(Pedido))) {
        if (pedido_lido.is_ativo()) {
            pedidos.push_back(pedido_lido);
        }
    }
    return pedidos;
}

Pedido GerenciadorPedidos::get_pedido_by_id(int id) {
    ifstream arquivo(nomeArquivo, ios::binary);
    Pedido pedido_lido;
    if (arquivo) {
        while (arquivo.read(reinterpret_cast<char*>(&pedido_lido), sizeof(Pedido))) {
            if (pedido_lido.is_ativo() && pedido_lido.get_id() == id) {
                return pedido_lido;
            }
        }
    }
    return Pedido(); // Retorna um pedido "vazio" se não encontrar
}