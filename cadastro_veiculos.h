#pragma once

#include <string>
#include <vector>
#include "cadastro_local.h"

using namespace std;

class Veiculo
{
private:
    string placa;
    string modelo;
    string status;
    string localAtual;

public:
    Veiculo(string placa, string modelo, string status, string localAtual);

    string get_placa() const;
    string get_modelo() const;
    string get_status() const;
    string get_localAtual() const;

    void change_modelo(string novo_modelo);
    void change_status(string novo_status);
    void change_localAtual(string novo_local);
};

class GerenciadorVeiculos
{
private:
    vector<Veiculo> veiculos;

public:
    GerenciadorVeiculos();

    void cadastrar_veiculo(vector<Local>& locais_disponiveis);
    void listar_veiculos() const;
    void atualizar_veiculo(string placa, vector<Local>& locais_disponiveis);
    void deletar_veiculo(string placa);
};