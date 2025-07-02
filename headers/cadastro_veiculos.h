#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#pragma once

using namespace std;

const int TAM_TIPO_VEICULO = 50;
const int TAM_PLACA_VEICULO = 10;

class Veiculo
{
private:
    char tipo[TAM_TIPO_VEICULO];
    char placa[TAM_PLACA_VEICULO];
    float capacidade;
    bool ativo;

public:
    Veiculo() : capacidade(0), ativo(false) {
        tipo[0] = '\0';
        placa[0] = '\0';
    }

    Veiculo(const string& tipo_str, const string& placa_str, float capacidade) {
        strncpy(this->tipo, tipo_str.c_str(), TAM_TIPO_VEICULO - 1);
        this->tipo[TAM_TIPO_VEICULO - 1] = '\0';
        strncpy(this->placa, placa_str.c_str(), TAM_PLACA_VEICULO - 1);
        this->placa[TAM_PLACA_VEICULO - 1] = '\0';
        this->capacidade = capacidade;
        this->ativo = true;
    }

    string get_tipo() const { return string(tipo); }
    string get_placa() const { return string(placa); }
    float get_capacidade() const { return capacidade; }
    bool is_ativo() const { return ativo; }

    void change_tipo(const string& novo_tipo) {
        strncpy(this->tipo, novo_tipo.c_str(), TAM_TIPO_VEICULO - 1);
        this->tipo[TAM_TIPO_VEICULO - 1] = '\0';
    }
    void change_placa(const string& nova_placa) {
        strncpy(this->placa, nova_placa.c_str(), TAM_PLACA_VEICULO - 1);
        this->placa[TAM_PLACA_VEICULO - 1] = '\0';
    }
    void change_capacidade(float nova_capacidade) { this->capacidade = nova_capacidade; }
    void desativar() { this->ativo = false; }
};

class GerenciadorVeiculos
{
private:
    const string nomeArquivo = "veiculos.dat";
    int encontrar_pos_veiculo(const string& placa);

public:
    GerenciadorVeiculos();
    bool criar_veiculo(const string& tipo, const string& placa, float capacidade);
    bool deletar_veiculo(const string& placa);
    void listar_veiculos();
    bool atualizar_veiculo(const string& placa_atual, const string& novo_tipo, const string& nova_placa, float nova_capacidade);
    vector<Veiculo> get_todos_veiculos();
    Veiculo get_veiculo_by_placa(const string& placa);
};