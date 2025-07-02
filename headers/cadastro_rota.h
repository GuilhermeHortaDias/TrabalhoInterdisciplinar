#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include "cadastro_local.h"
#include "cadastro_pedido.h"
#include "cadastro_veiculos.h"

using namespace std;

const int MAX_PONTOS_ROTA = 20;
const int TAM_PLACA_ROTA = 10;

// Classe para representar uma rota individual, projetada para ser serializável.
class Rota {
private:
    char placa_veiculo[TAM_PLACA_ROTA];
    int ids_pedidos[MAX_PONTOS_ROTA];
    int num_pedidos;
    bool ativa;

public:
    Rota() : num_pedidos(0), ativa(false) {
        placa_veiculo[0] = '\0';
    }

    Rota(const string& placa) : num_pedidos(0), ativa(true) {
        strncpy(placa_veiculo, placa.c_str(), TAM_PLACA_ROTA - 1);
        placa_veiculo[TAM_PLACA_ROTA - 1] = '\0';
    }

    void adicionar_pedido(int id_pedido) {
        if (num_pedidos < MAX_PONTOS_ROTA) {
            ids_pedidos[num_pedidos++] = id_pedido;
        }
    }

    string get_placa_veiculo() const { return string(placa_veiculo); }
    const int* get_ids_pedidos() const { return ids_pedidos; }
    int get_num_pedidos() const { return num_pedidos; }
    bool is_ativa() const { return ativa; }
    void desativar() { ativa = false; }
};

// Gerenciador de Rotas: calcula, salva e exibe as rotas.
class GerenciadorRotas {
private:
    const string nomeArquivo = "rotas.dat";
    float calcular_distancia(const Local& l1, const Local& l2);
    Local encontrar_local(const string& nome_local, const vector<Local>& locais);

public:
    GerenciadorRotas();
    void gerar_rotas();
    void listar_rotas();
    void limpar_rotas_antigas();
    vector<Rota> get_todas_rotas(); // Adicionado para testabilidade
};
