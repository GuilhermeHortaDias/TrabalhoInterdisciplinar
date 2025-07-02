// Garanta que esta linha (#define) venha ANTES do #include "catch.hpp"
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../headers/cadastro_veiculos.h"
#include <fstream>
#include <cstdio>

// Função auxiliar para limpar o arquivo de dados antes de cada teste
void limpar_arquivo_veiculos() {
    remove("veiculos.dat");
}

TEST_CASE("Testes do Gerenciador de Veiculos com Arquivos Binarios") {
    GerenciadorVeiculos gv;

    SECTION("Criar e Listar Veiculos") {
        limpar_arquivo_veiculos();
        REQUIRE(gv.criar_veiculo("Caminhao", "ABC-1234", 5000.0));
        REQUIRE(gv.criar_veiculo("Van", "DEF-5678", 1500.0));

        // Verifica se a criação de veiculo com placa duplicada falha
        REQUIRE_FALSE(gv.criar_veiculo("Carro", "ABC-1234", 800.0));

        vector<Veiculo> veiculos = gv.get_todos_veiculos();
        REQUIRE(veiculos.size() == 2);
        REQUIRE(veiculos[0].get_placa() == "ABC-1234");
        REQUIRE(veiculos[1].get_placa() == "DEF-5678");
    }

    SECTION("Atualizar Veiculo") {
        limpar_arquivo_veiculos();
        gv.criar_veiculo("Moto", "GHI-9012", 150.0);

        // Atualiza com sucesso
        REQUIRE(gv.atualizar_veiculo("GHI-9012", "Motocicleta", "GHI-9012", 180.0));

        Veiculo v = gv.get_veiculo_by_placa("GHI-9012");
        REQUIRE(v.is_ativo());
        REQUIRE(v.get_tipo() == "Motocicleta");
        REQUIRE(v.get_capacidade() == 180.0f);

        // Tenta atualizar veiculo que não existe
        REQUIRE_FALSE(gv.atualizar_veiculo("XYZ-0000", "Inexistente", "XYZ-0000", 100));
    }

    SECTION("Deletar Veiculo") {
        limpar_arquivo_veiculos();
        gv.criar_veiculo("Para Deletar", "DEL-0001", 100);
        gv.criar_veiculo("Para Manter", "MAN-0002", 200);

        REQUIRE(gv.deletar_veiculo("DEL-0001"));

        vector<Veiculo> veiculos = gv.get_todos_veiculos();
        REQUIRE(veiculos.size() == 1);
        REQUIRE(veiculos[0].get_placa() == "MAN-0002");

        // Tenta deletar veiculo que não existe
        REQUIRE_FALSE(gv.deletar_veiculo("DEL-0001"));
    }
}