#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "../headers/cadastro_local.h"

using namespace std;

// Implementações da classe Local
Local::Local() : x(0), y(0), ativo(false) {
    nome[0] = '\0';
}

Local::Local(const string& nome_str, float x, float y) {
    strncpy(this->nome, nome_str.c_str(), TAM_NOME_LOCAL - 1);
    this->nome[TAM_NOME_LOCAL - 1] = '\0';
    this->x = x;
    this->y = y;
    this->ativo = true;
}

string Local::get_nome() const { return string(nome); }
float Local::get_x() const { return x; }
float Local::get_y() const { return y; }
bool Local::is_ativo() const { return ativo; }

void Local::change_nome(const string& novo_nome) {
    strncpy(this->nome, novo_nome.c_str(), TAM_NOME_LOCAL - 1);
    this->nome[TAM_NOME_LOCAL - 1] = '\0';
}
void Local::change_x(float novo_x) { this->x = novo_x; }
void Local::change_y(float novo_y) { this->y = novo_y; }
void Local::desativar() { this->ativo = false; }

// Implementações da classe GerenciadorLocais
GerenciadorLocais::GerenciadorLocais() {
    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out | ios::app);
    if (!arquivo.is_open()) {
        // Se o arquivo não existir, ele será criado vazio.
        ofstream novoArquivo(nomeArquivo, ios::binary);
        novoArquivo.close();
    }
}

int GerenciadorLocais::encontrar_pos_local(const string& nome) {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) return -1;

    Local temp;
    int pos = 0;
    while (arquivo.read(reinterpret_cast<char*>(&temp), sizeof(Local))) {
        if (temp.is_ativo() && temp.get_nome() == nome) {
            return pos;
        }
        pos++;
    }
    return -1;
}

bool GerenciadorLocais::criar_local(const string& nome, float x, float y) {
    if (encontrar_pos_local(nome) != -1) {
        cout << "Erro: Local com nome '" << nome << "' já existe." << endl;
        return false;
    }

    Local novo(nome, x, y);
    ofstream arquivo(nomeArquivo, ios::binary | ios::app);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo para escrita." << endl;
        return false;
    }
    arquivo.write(reinterpret_cast<const char*>(&novo), sizeof(Local));
    cout << "Local '" << nome << "' cadastrado com sucesso." << endl;
    return true;
}

void GerenciadorLocais::listar_locais() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        cout << "Nenhum local cadastrado." << endl;
        return;
    }

    cout << "\n--- Lista de Locais ---" << endl;
    Local temp;
    bool encontrou = false;
    while (arquivo.read(reinterpret_cast<char*>(&temp), sizeof(Local))) {
        if (temp.is_ativo()) {
            cout << "Nome: " << temp.get_nome() << ", Coordenadas: (" << temp.get_x() << ", " << temp.get_y() << ")" << endl;
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhum local ativo encontrado." << endl;
    }
}


bool GerenciadorLocais::atualizar_local(const string& nome_atual, const string& novo_nome, float x, float y) {
    int pos = encontrar_pos_local(nome_atual);
    if (pos == -1) {
        cout << "Erro: Local '" << nome_atual << "' não encontrado." << endl;
        return false;
    }

    if (nome_atual != novo_nome && encontrar_pos_local(novo_nome) != -1) {
        cout << "Erro: Já existe um local com o nome '" << novo_nome << "'." << endl;
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
    ifstream arquivo(nomeArquivo, ios::binary);
    vector<Local> locais;
    if (!arquivo) {
        return locais;
    }

    Local temp;
    while (arquivo.read(reinterpret_cast<char*>(&temp), sizeof(Local))) {
        if (temp.is_ativo()) {
            locais.push_back(temp);
        }
    }
    return locais;
}

Local GerenciadorLocais::get_local_by_nome(const string& nome) {
    ifstream arquivo(nomeArquivo, ios::binary);
    Local temp;
    if (arquivo) {
        while (arquivo.read(reinterpret_cast<char*>(&temp), sizeof(Local))) {
            if (temp.is_ativo() && temp.get_nome() == nome) {
                return temp;
            }
        }
    }
    return Local(); // Retorna um local inativo se não encontrar
}

bool GerenciadorLocais::deletar_local(const string& nome) {
    int pos = encontrar_pos_local(nome);
    if (pos == -1) {
        cout << "Erro: Local '" << nome << "' não encontrado." << endl;
        return false;
    }

    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    Local local_a_deletar;
    arquivo.seekg(pos * sizeof(Local));
    arquivo.read(reinterpret_cast<char*>(&local_a_deletar), sizeof(Local));

    local_a_deletar.desativar(); // Marca como inativo

    arquivo.seekp(pos * sizeof(Local));
    arquivo.write(reinterpret_cast<const char*>(&local_a_deletar), sizeof(Local));

    cout << "Local '" << nome << "' deletado com sucesso." << endl;
    return true;
}
