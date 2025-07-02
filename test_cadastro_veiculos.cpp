// Garanta que esta linha (#define) venha ANTES do #include "catch.hpp"
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "cadastro_veiculos.h"
#include "cadastro_local.h"
#include <sstream> // NECESSÁRIO para simular cin/cout
#include <iostream>

// --- Testes para Cadastro de Veículos ---

TEST_CASE("Cadastro de veiculo") {
    GerenciadorVeiculos gv;
    GerenciadorLocais gl;
    gl.criar_local("Garagem", 1.0, 1.0); // Pré-condição: precisa de um local

    SECTION("Cadastro bem-sucedido com placa unica") {
        // Simula a entrada do usuário via teclado (placa, tipo, status, local)
        std::stringstream input_simulado;
        input_simulado << "ABC-1234\n";
        input_simulado << "Ford Ka\n";
        input_simulado << "Disponivel\n";
        input_simulado << "Garagem\n";

        // Redireciona o 'cin' para ler do nosso texto simulado
        std::streambuf* cin_original = std::cin.rdbuf();
        std::cin.rdbuf(input_simulado.rdbuf());

        // Captura a saída do 'cout' para verificar a mensagem
        std::stringstream output_capturado;
        std::streambuf* cout_original = std::cout.rdbuf();
        std::cout.rdbuf(output_capturado.rdbuf());

        // CHAMA A SUA FUNÇÃO REAL
        gv.cadastrar_veiculo(gl.locais);

        // Restaura o cin e cout originais
        std::cin.rdbuf(cin_original);
        std::cout.rdbuf(cout_original);

        // Verifica se a mensagem de sucesso foi impressa
        REQUIRE(output_capturado.str().find("Veiculo cadastrado com sucesso!") != std::string::npos);
    }

    SECTION("Cadastro com placa duplicada deve falhar") {
        // Primeiro, cadastra um veículo normalmente
        std::stringstream input1;
        input1 << "DEF-5678\nCarro\nDisponivel\nGaragem\n";
        std::streambuf* cin_orig = std::cin.rdbuf();
        std::cin.rdbuf(input1.rdbuf());
        gv.cadastrar_veiculo(gl.locais);
        std::cin.rdbuf(cin_orig); // Restaura

        // Agora, tenta cadastrar de novo com a mesma placa
        std::stringstream input2;
        input2 << "DEF-5678\nMoto\nOcupado\nGaragem\n";
        std::cin.rdbuf(input2.rdbuf());

        std::stringstream output_capturado;
        std::streambuf* cout_orig = std::cout.rdbuf();
        std::cout.rdbuf(output_capturado.rdbuf());

        gv.cadastrar_veiculo(gl.locais);

        std::cin.rdbuf(cin_orig);
        std::cout.rdbuf(cout_orig);

        REQUIRE(output_capturado.str().find("ERRO: Veiculo com esta placa ja existe.") != std::string::npos);
    }
}


// --- Testes para Remoção de Veículos ---

TEST_CASE("Remocao de veiculo") {
    GerenciadorVeiculos gv;
    GerenciadorLocais gl;
    gl.criar_local("Garagem", 1.0, 1.0);

    // Cadastra um veículo para poder remover depois
    std::stringstream input_cadastro;
    input_cadastro << "XYZ-9876\nVan\nOcupado\nGaragem\n";
    std::streambuf* cin_orig = std::cin.rdbuf();
    std::cin.rdbuf(input_cadastro.rdbuf());
    gv.cadastrar_veiculo(gl.locais);
    std::cin.rdbuf(cin_orig);

    SECTION("Remover veiculo existente") {
        std::stringstream output_capturado;
        std::streambuf* cout_orig = std::cout.rdbuf();
        std::cout.rdbuf(output_capturado.rdbuf());

        gv.deletar_veiculo("XYZ-9876");

        std::cout.rdbuf(cout_orig);
        REQUIRE(output_capturado.str().find("Veiculo deletado com sucesso!") != std::string::npos);
    }
}