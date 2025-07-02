#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "cadastro_local.h"

SCENARIO("Testando a classe Pedido", "[pedido]") {
    GIVEN("Um pedido com parâmetros válidos") {
        int id = 1;
        string origem = "São Paulo";
        string destino = "Rio de Janeiro";
        float peso = 5.0f;

        Pedido pedido(id, origem, destino, peso);

        WHEN("Obtendo os valores do pedido") {
            THEN("Os valores devem ser consistentes com os fornecidos") {
                REQUIRE(pedido.get_id() == id);
                REQUIRE(pedido.get_origem() == origem);
                REQUIRE(pedido.get_destino() == destino);
                REQUIRE(pedido.get_peso() == Approx(peso));
            }
        }

        WHEN("Alterando os valores do pedido") {
            string novaOrigem = "Curitiba";
            string novoDestino = "Porto Alegre";
            float novoPeso = 10.0f;

            pedido.set_origem(novaOrigem);
            pedido.set_destino(novoDestino);
            pedido.set_peso(novoPeso);

            THEN("Os novos valores devem ser consistentes") {
                REQUIRE(pedido.get_origem() == novaOrigem);
                REQUIRE(pedido.get_destino() == novoDestino);
                REQUIRE(pedido.get_peso() == Approx(novoPeso));
            }
        }
    }
}

SCENARIO("Testando a classe GerenciadorPedidos", "[gerenciador]") {
    vector<string> locais = {"São Paulo", "Rio de Janeiro", "Curitiba", "Porto Alegre"};
    vector<Pedido> pedidos;

    GerenciadorPedidos gerenciador;

    GIVEN("Um gerenciador de pedidos vazio") {
        WHEN("Cadastrando um pedido válido") {
            Pedido pedido(1, "São Paulo", "Rio de Janeiro", 5.0f);
            pedidos.push_back(pedido);

            THEN("O pedido deve ser adicionado corretamente") {
                REQUIRE(pedidos.size() == 1);
                REQUIRE(pedidos[0].get_id() == 1);
                REQUIRE(pedidos[0].get_origem() == "São Paulo");
                REQUIRE(pedidos[0].get_destino() == "Rio de Janeiro");
                REQUIRE(pedidos[0].get_peso() == Approx(5.0f));
            }
        }

        WHEN("Tentando cadastrar um pedido com local inválido") {
            bool exceptionThrown = false;

            try {
                Pedido pedido_invalido(2, "Local Inválido", "Rio de Janeiro", 5.0f);
                pedidos.push_back(pedido_invalido);
            } catch (...) {
                exceptionThrown = true;
            }

            THEN("Uma exceção deve ser lançada") {
                REQUIRE(exceptionThrown);
                REQUIRE(pedidos.size() == 0);
            }
        }

        WHEN("Listando pedidos") {
            Pedido pedido1(1, "São Paulo", "Rio de Janeiro", 5.0f);
            Pedido pedido2(2, "Curitiba", "Porto Alegre", 10.0f);
            pedidos.push_back(pedido1);
            pedidos.push_back(pedido2);

            THEN("Os pedidos devem ser listados corretamente") {
                gerenciador.listarPedidos(pedidos);
                REQUIRE(pedidos.size() == 2);
                REQUIRE(pedidos[0].get_id() == 1);
                REQUIRE(pedidos[1].get_id() == 2);
            }
        }

        WHEN("Excluindo um pedido existente") {
            Pedido pedido1(1, "São Paulo", "Rio de Janeiro", 5.0f);
            Pedido pedido2(2, "Curitiba", "Porto Alegre", 10.0f);
            pedidos.push_back(pedido1);
            pedidos.push_back(pedido2);

            gerenciador.excluirPedido(pedidos);

            THEN("O pedido deve ser removido corretamente") {
                REQUIRE(pedidos.size() == 1);
                REQUIRE(pedidos[0].get_id() == 2);
            }
        }

        WHEN("Editando um pedido existente") {
            Pedido pedido1(1, "São Paulo", "Rio de Janeiro", 5.0f);
            Pedido pedido2(2, "Curitiba", "Porto Alegre", 10.0f);
            pedidos.push_back(pedido1);
            pedidos.push_back(pedido2);

            gerenciador.editarPedido(pedidos, locais);

            THEN("O pedido deve ser atualizado corretamente") {
                REQUIRE(pedidos.size() == 2);
                REQUIRE(pedidos[0].get_id() == 1);
                REQUIRE(pedidos[1].get_id() == 2);
            }
        }
    }
}