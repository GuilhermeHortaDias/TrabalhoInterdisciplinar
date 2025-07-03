#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../headers/cadastro_local.h"

//cenario para o primeiro teste
SCENARIO("Testando a classe Pedido", "[pedido]") {
   //condições iniciais do teste
    GIVEN("Um pedido com parâmetros válidos") {
        int id = 1;
        string origem = "São Paulo";
        string destino = "Rio de Janeiro";
        float peso = 5.0f;

        Pedido pedido(id, origem, destino, peso);

        //define a ação que a pessoa vai fazer
        WHEN("Obtendo os valores do pedido") {
           //define a ação esperada pra isso 
            THEN("Os valores devem ser consistentes com os fornecidos") {
                REQUIRE(pedido.get_id() == id);
                REQUIRE(pedido.get_origem() == origem);
                REQUIRE(pedido.get_destino() == destino);
                REQUIRE(pedido.get_peso() == Approx(peso));
            }
        }
        
        //testa a alteraçãp feita no pedido
        WHEN("Alterando os valores do pedido") {
            string novaOrigem = "Curitiba";
            string novoDestino = "Porto Alegre";
            float novoPeso = 10.0f;

            pedido.set_origem(novaOrigem);
            pedido.set_destino(novoDestino);
            pedido.set_peso(novoPeso);

            THEN("Os novos valores devem ser consistentes") {
               //verifica se deu certo os novos valores
                REQUIRE(pedido.get_origem() == novaOrigem);
                REQUIRE(pedido.get_destino() == novoDestino);
                REQUIRE(pedido.get_peso() == Approx(novoPeso));
            }
        }
    }
}

//cenario para o segundo teste
SCENARIO("Testando a classe GerenciadorPedidos", "[gerenciador]") {
    vector<string> locais = {"São Paulo", "Rio de Janeiro", "Curitiba", "Porto Alegre"};
    vector<Pedido> pedidos;

    GerenciadorPedidos gerenciador;

    //as condições iniciais do teste são definidas
    GIVEN("Um gerenciador de pedidos vazio") {
       //testa se novo pedido é valido
        WHEN("Cadastrando um pedido válido") {
            Pedido pedido(1, "São Paulo", "Rio de Janeiro", 5.0f);
            pedidos.push_back(pedido);

            THEN("O pedido deve ser adicionado corretamente") {
               //define se foi adicionado direito o pedido
                REQUIRE(pedidos.size() == 1);
                REQUIRE(pedidos[0].get_id() == 1);
                REQUIRE(pedidos[0].get_origem() == "São Paulo");
                REQUIRE(pedidos[0].get_destino() == "Rio de Janeiro");
                REQUIRE(pedidos[0].get_peso() == Approx(5.0f));
            }
        }

        //testa um cadastro de um pedido com um local invalido
        WHEN("Tentando cadastrar um pedido com local inválido") {
            bool exceptionThrown = false;

            try {
                Pedido pedido_invalido(2, "Local Inválido", "Rio de Janeiro", 5.0f);
                pedidos.push_back(pedido_invalido);
            } catch (...) { //pega as exceções
                exceptionThrown = true;
            }

            //olha se o vetor ficou vazio depois que a exceção foi lançada
            THEN("Uma exceção deve ser lançada") {
                REQUIRE(exceptionThrown);
                REQUIRE(pedidos.size() == 0);
            }
        }

        //testa a listagem de pediddos
        WHEN("Listando pedidos") {
            Pedido pedido1(1, "São Paulo", "Rio de Janeiro", 5.0f);
            Pedido pedido2(2, "Curitiba", "Porto Alegre", 10.0f);
            pedidos.push_back(pedido1);
            pedidos.push_back(pedido2);

            //vê se deu certo ao adicionar os pedidos
            THEN("Os pedidos devem ser listados corretamente") {
                gerenciador.listarPedidos(pedidos);
                REQUIRE(pedidos.size() == 2);
                REQUIRE(pedidos[0].get_id() == 1);
                REQUIRE(pedidos[1].get_id() == 2);
            }
        }

        //testa depois de excluir o pedido
        WHEN("Excluindo um pedido existente") {
            Pedido pedido1(1, "São Paulo", "Rio de Janeiro", 5.0f);
            Pedido pedido2(2, "Curitiba", "Porto Alegre", 10.0f);
            pedidos.push_back(pedido1);
            pedidos.push_back(pedido2);

            gerenciador.excluirPedido(pedidos);

            //testa se deu certo excluir
            THEN("O pedido deve ser removido corretamente") {
                REQUIRE(pedidos.size() == 1);
                REQUIRE(pedidos[0].get_id() == 2);
            }
        }

        //testa a edição do pedido
        WHEN("Editando um pedido existente") {
            Pedido pedido1(1, "São Paulo", "Rio de Janeiro", 5.0f);
            Pedido pedido2(2, "Curitiba", "Porto Alegre", 10.0f);
            pedidos.push_back(pedido1);
            pedidos.push_back(pedido2);

            gerenciador.editarPedido(pedidos, locais);

            //olha se deu certo ao atualizar
            THEN("O pedido deve ser atualizado corretamente") {
                REQUIRE(pedidos.size() == 2);
                REQUIRE(pedidos[0].get_id() == 1);
                REQUIRE(pedidos[1].get_id() == 2);
            }
        }
    }
}