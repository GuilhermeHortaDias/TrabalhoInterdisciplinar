#pragma once
#include <string>
#include <vector>

using namespace std;

const int TAM_TIPO_VEICULO = 50;
const int TAM_PLACA_VEICULO = 10;

class Veiculo {
private:
    char tipo[TAM_TIPO_VEICULO];
    char placa[TAM_PLACA_VEICULO];
    float capacidade;
    bool ativo;

public:
    Veiculo();
    Veiculo(const string& tipo_str, const string& placa_str, float capacidade);

    string get_tipo() const;
    string get_placa() const;
    float get_capacidade() const;
    bool is_ativo() const;

    void change_tipo(const string& novo_tipo);
    void change_placa(const string& nova_placa);
    void change_capacidade(float nova_capacidade);
    void desativar();
};

class GerenciadorVeiculos {
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