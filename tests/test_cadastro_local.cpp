#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../headers/cadastro_local.h"
#include <fstream>
#include <cstdio> // Para remove()

// Função auxiliar para limpar o arquivo de dados antes de cada teste
void limpar_arquivo_locais() {
    remove("locais.dat");
}

TEST_CASE("Testes do Gerenciador de Locais com Arquivos Binarios") {
    GerenciadorLocais gl;

    SECTION("Criar e Listar Locais") {
        limpar_arquivo_locais();
        REQUIRE(gl.criar_local("Garagem", 0, 0));
        REQUIRE(gl.criar_local("Cliente A", 10, 20));

        // Verifica se a criação de local duplicado falha
        REQUIRE_FALSE(gl.criar_local("Garagem", 1, 1));

        vector<Local> locais = gl.get_todos_locais();
        REQUIRE(locais.size() == 2);
        REQUIRE(locais[0].get_nome() == "Garagem");
        REQUIRE(locais[1].get_nome() == "Cliente A");
    }

    SECTION("Atualizar Local") {
        limpar_arquivo_locais();
        gl.criar_local("Ponto Antigo", 5, 5);

        // Atualiza com sucesso
        REQUIRE(gl.atualizar("Ponto Antigo", "Ponto Novo", 15, 15));

        vector<Local> locais = gl.get_todos_locais();
        REQUIRE(locais.size() == 1);
        REQUIRE(locais[0].get_nome() == "Ponto Novo");
        REQUIRE(locais[0].get_x() == 15);

        // Tenta atualizar local que não existe
        REQUIRE_FALSE(gl.atualizar("Ponto Inexistente", "Qualquer", 0, 0));
    }

    SECTION("Deletar Local") {
        limpar_arquivo_locais();
        gl.criar_local("Para Deletar", 1, 1);
        gl.criar_local("Para Manter", 2, 2);

        REQUIRE(gl.deletar_local("Para Deletar"));

        vector<Local> locais = gl.get_todos_locais();
        REQUIRE(locais.size() == 1);
        REQUIRE(locais[0].get_nome() == "Para Manter");

        // Tenta deletar local que não existe
        REQUIRE_FALSE(gl.deletar_local("Para Deletar"));
    }
}