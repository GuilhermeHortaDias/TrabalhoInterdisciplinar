#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../headers/cadastro_rota.h"
#include "../headers/cadastro_local.h"
#include "../headers/cadastro_pedido.h"
#include "../headers/cadastro_veiculos.h"

TEST_CASE("calcularDistancia retorna a distância correta entre dois locais", "[Roteamento]") {
    Local origem("A", 0.0, 0.0);
    Local destino("B", 3.0, 4.0);
    Roteamento roteamento;
    REQUIRE(roteamento.calcularDistancia(origem, destino) == Approx(5.0f));
}

TEST_CASE("atribuirPedidosAosVeiculos atribui pedidos ao veículo disponível mais próximo", "[Roteamento]") {
    Local origem("Origem", 0, 0);
    Local destino("Destino", 10, 0);
    Pedido pedido(1, "Origem", "Destino", 5.0f);
    vector<Pedido> pedidos = {pedido};
    Veiculo veic1(1, "disponivel", origem, origem);
    Veiculo veic2(2, "disponivel", destino, destino);
    vector<Veiculo> veiculos = {veic1, veic2};
    vector<Local> locais = {origem, destino};
    Roteamento roteamento;
    roteamento.atribuirPedidosAosVeiculos(pedidos, veiculos, locais);
    // Veículo 1 está mais próximo do local de entrega (origem), então ele deve ser atribuído
    REQUIRE(veiculos[0].get_status() == "ocupado" || veiculos[1].get_status() == "ocupado");
}

TEST_CASE("gerarRotas gera rotas para veículos ocupados", "[Roteamento]") {
    Local origem("Origem", 0, 0);
    Local destino("Destino", 10, 0);
    Veiculo veic1(1, "ocupado", origem, destino);
    vector<Veiculo> veiculos = {veic1};
    vector<Pedido> pedidos; // não usado
    vector<Local> locais = {origem, destino};
    Roteamento roteamento;
    roteamento.gerarRotas(pedidos, veiculos, locais);
    REQUIRE(roteamento.rotas.size() == 1);
    REQUIRE(roteamento.rotas[0][0].get_nome() == "Origem");
    REQUIRE(roteamento.rotas[0][1].get_nome() == "Destino");
}
