#pragma once
#include <vector>
#include "cadastro_local.h"
#include "cadastro_pedido.h"
#include "cadastro_veiculos.h"
using namespace std;

class Roteamento {
public:
    // Cada rota é um vetor de locais (origem, pontos de entrega, destino)
    vector<vector<Local>> rotas;

    // Calcula a distância entre dois locais
    float calcularDistancia(const Local& origem, const Local& destino);

    // Atribui pedidos aos veículos disponíveis conforme proximidade
    void atribuirPedidosAosVeiculos(const vector<Pedido>& pedidos, vector<Veiculo>& veiculos, const vector<Local>& locais);

    // Gera as rotas para cada veículo
    void gerarRotas(const vector<Pedido>& pedidos, vector<Veiculo>& veiculos, const vector<Local>& locais);

    // Exibe as rotas calculadas
    void exibirRotas() const;
};
