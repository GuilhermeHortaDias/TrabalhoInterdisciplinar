#pragma once
#include <vector>
#include <string>
#include "cadastro_local.h"
#include "cadastro_pedido.h"
#include "cadastro_veiculos.h"

using namespace std;

const int MAX_PONTOS_ROTA = 20;
const int TAM_PLACA_ROTA = 10;

class Rota {
private:
    char placa_veiculo[TAM_PLACA_ROTA];
    int ids_pedidos[MAX_PONTOS_ROTA];
    int num_pedidos;
    bool ativa;

public:
    Rota();
    Rota(const string& placa);

    void adicionar_pedido(int id_pedido);
    string get_placa_veiculo() const;
    const int* get_ids_pedidos() const;
    int get_num_pedidos() const;
    bool is_ativa() const;
    void desativar();
};

class GerenciadorRotas {
private:
    const string nomeArquivo = "rotas.dat";
    GerenciadorLocais& gerenciadorLocais;
    GerenciadorPedidos& gerenciadorPedidos;
    GerenciadorVeiculos& gerenciadorVeiculos;

    float calcular_distancia(const Local& l1, const Local& l2);
    Local encontrar_local(const string& nome_local, const vector<Local>& locais);

public:
    GerenciadorRotas(GerenciadorLocais& gl, GerenciadorPedidos& gp, GerenciadorVeiculos& gv);
    void gerar_rotas_otimizadas();
    void listar_rotas_com_detalhes();
    void limpar_rotas_antigas();
    vector<Rota> get_todas_rotas();
};
