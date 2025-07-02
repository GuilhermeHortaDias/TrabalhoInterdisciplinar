#include <iostream>
#include <limits>
#include <fstream>
#include <vector>
#include <cstring>
#include "../headers/cadastro_local.h"
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

    bool Local::is_ativo() const {
        return ativo;
    }

    void Local::desativar() {
        ativo = false;
    }


//Cria os metodos da classe gerenciadora
GerenciadorLocais::GerenciadorLocais() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        ofstream novoArquivo(nomeArquivo, ios::binary);
        novoArquivo.close();
    }
}

int GerenciadorLocais::encontrar_pos_local(const string& nome) {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) return -1;

    Local local_lido;
    int pos = 0;
    while (arquivo.read(reinterpret_cast<char*>(&local_lido), sizeof(Local))) {
        if (local_lido.is_ativo() && local_lido.get_nome() == nome) {
            return pos;
        }
        pos++;
    }
    return -1;
}

bool GerenciadorLocais::criar_local(const string& nome, float x, float y) {
    if (encontrar_pos_local(nome) != -1) {
        cout << "Erro: Já existe um local com este nome." << endl;
        return false;
    }

    Local novo_local(nome, x, y);
    ofstream arquivo(nomeArquivo, ios::binary | ios::app);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo para escrita." << endl;
        return false;
    }

    arquivo.write(reinterpret_cast<const char*>(&novo_local), sizeof(Local));
    cout << "Local criado com sucesso!" << endl;
    return true;
}


bool GerenciadorLocais::deletar_local(const string& nome) {
    int pos = encontrar_pos_local(nome);
    if (pos == -1) {
        cout << "Erro: Local não encontrado." << endl;
        return false;
    }

    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    arquivo.seekg(pos * sizeof(Local));
    Local local_a_deletar;
    arquivo.read(reinterpret_cast<char*>(&local_a_deletar), sizeof(Local));

    local_a_deletar.desativar();

    arquivo.seekp(pos * sizeof(Local));
    arquivo.write(reinterpret_cast<const char*>(&local_a_deletar), sizeof(Local));

    cout << "Local deletado com sucesso!" << endl;
    return true;
}

void GerenciadorLocais::listar_locais() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        cout << "Nenhum local cadastrado." << endl;
        return;
    }

    Local local_lido;
    bool encontrou = false;
    cout << "\n--- Lista de Locais ---" << endl;
    while (arquivo.read(reinterpret_cast<char*>(&local_lido), sizeof(Local))) {
        if (local_lido.is_ativo()) {
            cout << "Nome: " << local_lido.get_nome()
                 << ", X: " << local_lido.get_x()
                 << ", Y: " << local_lido.get_y() << endl;
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhum local ativo cadastrado." << endl;
    }
    cout << "----------------------" << endl;
}


bool GerenciadorLocais::atualizar(const string& nome_atual, const string& novo_nome, float x, float y) {
    int pos = encontrar_pos_local(nome_atual);
    if (pos == -1) {
        cout << "Erro: Local a ser atualizado não encontrado." << endl;
        return false;
    }

    // Verifica se o novo nome já existe (e não é o mesmo local)
    int pos_novo = encontrar_pos_local(novo_nome);
    if (pos_novo != -1 && pos_novo != pos) {
        cout << "Erro: Já existe outro local com o novo nome." << endl;
        return false;
    }

    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    arquivo.seekg(pos * sizeof(Local));
    Local local_a_atualizar;
    arquivo.read(reinterpret_cast<char*>(&local_a_atualizar), sizeof(Local));

    local_a_atualizar.change_nome(novo_nome);
    local_a_atualizar.change_x(x);
    local_a_atualizar.change_y(y);

    arquivo.seekp(pos * sizeof(Local));
    arquivo.write(reinterpret_cast<const char*>(&local_a_atualizar), sizeof(Local));

    cout << "Local atualizado com sucesso!" << endl;
    return true;
}

vector<Local> GerenciadorLocais::get_todos_locais() {
    vector<Local> locais_ativos;
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        return locais_ativos; // Retorna vetor vazio se o arquivo não existir
    }

    Local local_lido;
    while (arquivo.read(reinterpret_cast<char*>(&local_lido), sizeof(Local))) {
        if (local_lido.is_ativo()) {
            locais_ativos.push_back(local_lido);
        }
    }

    return locais_ativos;
}
