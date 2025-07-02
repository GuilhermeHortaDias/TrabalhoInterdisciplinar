#include "catch.hpp"
#include "../headers/cadastro_local.h"
#define CATCH_CONFIG_MAIN

// testando cadastro novo
TEST_CASE("Cadastro com nome único") {
    GerenciadorLocais g;
    REQUIRE(g.criar_local("Loja A", 10.5, 5.2));
    REQUIRE(g.locais.size() == 1);
    REQUIRE(g.locais[0].get_nome() == "Loja A");
}

TEST_CASE("Cadastro com nome duplicado") {
    GerenciadorLocais g;
    REQUIRE(g.criar_local("Loja A", 10.5, 5.2));
    REQUIRE_FALSE(g.criar_local("Loja A", 12.0, 6.0));
    REQUIRE(g.locais.size() == 1);
}
// testando listagem
TEST_CASE("Listagem com locais cadastrados") {
    GerenciadorLocais g;
    g.criar_local("Loja A", 10.5, 5.2);
    g.criar_local("Centro", 1.0, 2.0);
    REQUIRE(g.locais.size() == 2);
    REQUIRE(g.locais[0].get_nome() == "Loja A");
    REQUIRE(g.locais[1].get_nome() == "Centro");
}

TEST_CASE("Listagem sem locais") {
    GerenciadorLocais g;
    REQUIRE(g.locais.empty());
}

// testando atualização de locais
TEST_CASE("Atualizar local existente") {
    GerenciadorLocais g;
    g.criar_local("Loja A", 10.5, 5.2);
    REQUIRE(g.atualizar("Loja A", "Loja B", 15.0, 8.0));
    REQUIRE(g.locais[0].get_nome() == "Loja B");
    REQUIRE(g.locais[0].get_x() == Approx(15.0));
    REQUIRE(g.locais[0].get_y() == Approx(8.0));
}

TEST_CASE("Atualizar local inexistente") {
    GerenciadorLocais g;
    REQUIRE_FALSE(g.atualizar("Loja X", "Loja Y", 1.0, 2.0));
}

TEST_CASE("Atualizar com nome já existente") {
    GerenciadorLocais g;
    g.criar_local("Loja A", 10.5, 5.2);
    g.criar_local("Centro", 1.0, 2.0);
    REQUIRE_FALSE(g.atualizar("Loja A", "Centro", 3.0, 4.0));
}


// testando remoção de locais
TEST_CASE("Remover local existente") {
    GerenciadorLocais g;
    g.criar_local("Loja A", 10.5, 5.2);
    REQUIRE(g.deletar_local("Loja A"));
    REQUIRE(g.locais.empty());
}

TEST_CASE("Remover local inexistente") {
    GerenciadorLocais g;
    REQUIRE_FALSE(g.deletar_local("Inexistente"));
}