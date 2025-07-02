#pragma once

#include <string>
#include "cadastro_local.h" // Para usar GerenciadorLocais

using namespace std;

const int MAX_NOME_LOCAL_PEDIDO = 50;

class Pedido
{
private:
    int id;
    char origem[MAX_NOME_LOCAL_PEDIDO];
    char destino[MAX_NOME_LOCAL_PEDIDO];
    float peso;
    bool ativo;

public:
    Pedido();
    Pedido(int id, const char* origem, const char* destino, float peso);

    int get_id() const;
    const char* get_origem() const;
    const char* get_destino() const;
    float get_peso() const;
    bool is_ativo() const;

    void set_origem(const char* novaOrigem);
    void set_destino(const char* novoDestino);
    void set_peso(float novoPeso);
    void desativar();
};

class GerenciadorPedidos
{
private:
    const string nomeArquivo = "pedidos.bin";
    int obterProximoId();

public:
    GerenciadorPedidos();
    void cadastrarPedido(GerenciadorLocais& gerenciadorLocais);
    void listarPedidos() const;
    void excluirPedido();
    void editarPedido(GerenciadorLocais& gerenciadorLocais);
};
