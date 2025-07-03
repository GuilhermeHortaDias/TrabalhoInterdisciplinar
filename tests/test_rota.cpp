#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../headers/cadastro_rota.h"
#include "../headers/cadastro_local.h"
#include "../headers/cadastro_pedido.h"
#include "../headers/cadastro_veiculos.h"
#include <fstream>
#include <cstdio> // Para a função remove()

// --- Funções Auxiliares para o Teste ---

// Limpa os arquivos de dados para garantir um ambiente de teste limpo
void limpar_arquivos_dat() {
    remove("locais.dat");
    remove("veiculos.dat");
    remove("pedidos.dat");
    remove("rotas.dat");
}

// --- Testes para GerenciadorRotas ---

TEST_CASE("Cálculo de Distância", "[GerenciadorRotas]") {
    GerenciadorLocais gl_temp; // Apenas para ter acesso ao gerenciador
    GerenciadorPedidos gp_temp;
    GerenciadorVeiculos gv_temp;
    GerenciadorRotas gr(gl_temp, gp_temp, gv_temp);

    Local l1("Ponto A", 0, 3);
    Local l2("Ponto B", 4, 0);

    // A distância euclidiana entre (0,3) e (4,0) é sqrt(4^2 + (-3)^2) = sqrt(16+9) = sqrt(25) = 5
    REQUIRE(gr.calcular_distancia(l1, l2) == Approx(5.0f));
}

TEST_CASE("Geração de Rota Otimizada Simples", "[GerenciadorRotas]") {
    limpar_arquivos_dat();

    // 1. Setup: Criar os gerenciadores e os dados de teste
    GerenciadorLocais gl;
    GerenciadorVeiculos gv;
    GerenciadorPedidos gp;
    
    // O construtor de GerenciadorLocais já cria a "Garagem" em (0,0)
    gl.criar_local("Cliente A", 10, 0);
    gl.criar_local("Deposito", 5, 5);
    
    gv.criar_veiculo("Caminhao", "ABC-1234", 100.0f);
    
    gp.criar_pedido("Deposito", "Cliente A", 30.0f);

    // 2. Ação: Gerar as rotas
    GerenciadorRotas gr(gl, gp, gv);
    gr.gerar_rotas_otimizadas();

    // 3. Verificação: Checar se a rota foi criada corretamente
    vector<Rota> rotas = gr.get_todas_rotas();
    REQUIRE(rotas.size() == 1);

    if (!rotas.empty()) {
        REQUIRE(rotas[0].get_placa_veiculo() == "ABC-1234");
        REQUIRE(rotas[0].get_num_pedidos() == 1);
        
        // O único pedido deve estar na rota
        const int* ids_pedidos = rotas[0].get_ids_pedidos();
        Pedido p = gp.get_pedido_by_id(ids_pedidos[0]);
        REQUIRE(p.get_destino() == "Cliente A");
    }
    
    limpar_arquivos_dat(); // Limpa depois do teste
}

TEST_CASE("Geração de Rota com Múltiplos Pedidos", "[GerenciadorRotas]") {
    limpar_arquivos_dat();

    GerenciadorLocais gl;
    GerenciadorVeiculos gv;
    GerenciadorPedidos gp;

    gl.criar_local("Cliente A", 10, 0);
    gl.criar_local("Cliente B", -10, 0);
    gl.criar_local("Deposito", 0, 1);

    gv.criar_veiculo("Van", "XYZ-5678", 80.0f);

    // Pedido 1: Deposito -> Cliente A (mais perto da garagem)
    // Pedido 2: Deposito -> Cliente B
    gp.criar_pedido("Deposito", "Cliente A", 40.0f);
    gp.criar_pedido("Deposito", "Cliente B", 30.0f);

    GerenciadorRotas gr(gl, gp, gv);
    gr.gerar_rotas_otimizadas();

    vector<Rota> rotas = gr.get_todas_rotas();
    REQUIRE(rotas.size() == 1);

    if (!rotas.empty()) {
        REQUIRE(rotas[0].get_placa_veiculo() == "XYZ-5678");
        REQUIRE(rotas[0].get_num_pedidos() == 2);

        // O algoritmo deve pegar primeiro o pedido para o Cliente A, pois a distância
        // Garagem -> Deposito -> Cliente A é menor que Garagem -> Deposito -> Cliente B
        const int* ids = rotas[0].get_ids_pedidos();
        Pedido p1 = gp.get_pedido_by_id(ids[0]);
        Pedido p2 = gp.get_pedido_by_id(ids[1]);

        REQUIRE(p1.get_destino() == "Cliente A");
        REQUIRE(p2.get_destino() == "Cliente B");
    }

    limpar_arquivos_dat();
}
