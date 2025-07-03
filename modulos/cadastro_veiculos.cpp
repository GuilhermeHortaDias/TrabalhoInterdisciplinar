#include "../headers/cadastro_veiculos.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// Implementação dos métodos da classe Veiculo
Veiculo::Veiculo() : capacidade(0), ativo(false) {
    tipo[0] = '\0';
    placa[0] = '\0';
}

Veiculo::Veiculo(const string& tipo_str, const string& placa_str, float capacidade) {
    strncpy(this->tipo, tipo_str.c_str(), TAM_TIPO_VEICULO - 1);
    this->tipo[TAM_TIPO_VEICULO - 1] = '\0';
    strncpy(this->placa, placa_str.c_str(), TAM_PLACA_VEICULO - 1);
    this->placa[TAM_PLACA_VEICULO - 1] = '\0';
    this->capacidade = capacidade;
    this->ativo = true;
}

string Veiculo::get_tipo() const { return string(tipo); }
string Veiculo::get_placa() const { return string(placa); }
float Veiculo::get_capacidade() const { return capacidade; }
bool Veiculo::is_ativo() const { return ativo; }

void Veiculo::change_tipo(const string& novo_tipo) {
    strncpy(this->tipo, novo_tipo.c_str(), TAM_TIPO_VEICULO - 1);
    this->tipo[TAM_TIPO_VEICULO - 1] = '\0';
}

void Veiculo::change_placa(const string& nova_placa) {
    strncpy(this->placa, nova_placa.c_str(), TAM_PLACA_VEICULO - 1);
    this->placa[TAM_PLACA_VEICULO - 1] = '\0';
}

void Veiculo::change_capacidade(float nova_capacidade) { this->capacidade = nova_capacidade; }
void Veiculo::desativar() { this->ativo = false; }

// Implementação dos métodos da classe GerenciadorVeiculos
GerenciadorVeiculos::GerenciadorVeiculos() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        ofstream novoArquivo(nomeArquivo, ios::binary);
        novoArquivo.close();
    }
}

int GerenciadorVeiculos::encontrar_pos_veiculo(const string& placa_procurada) {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) return -1;

    Veiculo veiculo_lido;
    int pos = 0;
    while (arquivo.read(reinterpret_cast<char*>(&veiculo_lido), sizeof(Veiculo))) {
        if (veiculo_lido.is_ativo() && veiculo_lido.get_placa() == placa_procurada) {
            return pos;
        }
        pos++;
    }
    return -1;
}

bool GerenciadorVeiculos::criar_veiculo(const string& tipo, const string& placa, float capacidade) {
    if (encontrar_pos_veiculo(placa) != -1) {
        cout << "Erro: Já existe um veículo com esta placa." << endl;
        return false;
    }

    Veiculo novo_veiculo(tipo, placa, capacidade);
    ofstream arquivo(nomeArquivo, ios::binary | ios::app);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo para escrita." << endl;
        return false;
    }

    arquivo.write(reinterpret_cast<const char*>(&novo_veiculo), sizeof(Veiculo));
    cout << "Veículo criado com sucesso!" << endl;
    return true;
}

bool GerenciadorVeiculos::deletar_veiculo(const string& placa) {
    int pos = encontrar_pos_veiculo(placa);
    if (pos == -1) {
        cout << "Erro: Veículo não encontrado." << endl;
        return false;
    }

    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    arquivo.seekg(pos * sizeof(Veiculo));
    Veiculo veiculo_a_deletar;
    arquivo.read(reinterpret_cast<char*>(&veiculo_a_deletar), sizeof(Veiculo));

    veiculo_a_deletar.desativar();

    arquivo.seekp(pos * sizeof(Veiculo));
    arquivo.write(reinterpret_cast<const char*>(&veiculo_a_deletar), sizeof(Veiculo));

    cout << "Veículo deletado com sucesso!" << endl;
    return true;
}

void GerenciadorVeiculos::listar_veiculos() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        cout << "Nenhum veículo cadastrado." << endl;
        return;
    }

    Veiculo veiculo_lido;
    bool encontrou_ativo = false;
    cout << "--- Lista de Veículos ---" << endl;
    while (arquivo.read(reinterpret_cast<char*>(&veiculo_lido), sizeof(Veiculo))) {
        if (veiculo_lido.is_ativo()) {
            cout << "Placa: " << veiculo_lido.get_placa()
                 << ", Tipo: " << veiculo_lido.get_tipo()
                 << ", Capacidade: " << veiculo_lido.get_capacidade() << " kg" << endl;
            encontrou_ativo = true;
        }
    }

    if (!encontrou_ativo) {
        cout << "Nenhum veículo ativo encontrado." << endl;
    }
    cout << "-------------------------" << endl;
}

bool GerenciadorVeiculos::atualizar_veiculo(const string& placa_atual, const string& novo_tipo, const string& nova_placa, float nova_capacidade) {
    int pos = encontrar_pos_veiculo(placa_atual);
    if (pos == -1) {
        cout << "Erro: Veículo com a placa " << placa_atual << " não encontrado." << endl;
        return false;
    }

    // Se a placa for alterada, verificar se a nova já existe
    if (placa_atual != nova_placa && encontrar_pos_veiculo(nova_placa) != -1) {
        cout << "Erro: Já existe um veículo com a nova placa " << nova_placa << "." << endl;
        return false;
    }

    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    arquivo.seekg(pos * sizeof(Veiculo));
    Veiculo veiculo_a_atualizar;
    arquivo.read(reinterpret_cast<char*>(&veiculo_a_atualizar), sizeof(Veiculo));

    veiculo_a_atualizar.change_tipo(novo_tipo);
    veiculo_a_atualizar.change_placa(nova_placa);
    veiculo_a_atualizar.change_capacidade(nova_capacidade);

    arquivo.seekp(pos * sizeof(Veiculo));
    arquivo.write(reinterpret_cast<const char*>(&veiculo_a_atualizar), sizeof(Veiculo));

    cout << "Veículo atualizado com sucesso!" << endl;
    return true;
}

vector<Veiculo> GerenciadorVeiculos::get_todos_veiculos() {
    ifstream arquivo(nomeArquivo, ios::binary);
    vector<Veiculo> veiculos;
    if (!arquivo) {
        return veiculos;
    }

    Veiculo veiculo_lido;
    while (arquivo.read(reinterpret_cast<char*>(&veiculo_lido), sizeof(Veiculo))) {
        if (veiculo_lido.is_ativo()) {
            veiculos.push_back(veiculo_lido);
        }
    }
    return veiculos;
}

Veiculo GerenciadorVeiculos::get_veiculo_by_placa(const string& placa) {
    ifstream arquivo(nomeArquivo, ios::binary);
    Veiculo veiculo_lido;
    if (arquivo) {
        while (arquivo.read(reinterpret_cast<char*>(&veiculo_lido), sizeof(Veiculo))) {
            if (veiculo_lido.is_ativo() && veiculo_lido.get_placa() == placa) {
                return veiculo_lido;
            }
        }
    }
    // Retorna um veículo "vazio" se não encontrar
    return Veiculo();
}