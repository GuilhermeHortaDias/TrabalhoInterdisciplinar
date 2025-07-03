#pragma once

#include <string>
#include <vector>
#include "cadastro_local.h" // Para validação de locais

using namespace std;

const int MAX_NOME_LOCAL_PEDIDO = 50;

class Pedido {
private:
    int id;
    char origem[MAX_NOME_LOCAL_PEDIDO];
    char destino[MAX_NOME_LOCAL_PEDIDO];
    float peso;
    bool ativo;

public:
    Pedido();
    Pedido(int id, const string& origem, const string& destino, float peso);

    int get_id() const;
    string get_origem() const;
    string get_destino() const;
    float get_peso() const;
    bool is_ativo() const;

    void set_origem(const string& nova_origem);
    void set_destino(const string& novo_destino);
    void set_peso(float novo_peso);
    void desativar();
};

class GerenciadorPedidos {
private:
    const string nomeArquivo = "pedidos.dat";
    int get_next_id();
    int encontrar_pos_pedido(int id);

public:
    GerenciadorPedidos();
    bool criar_pedido(const string& origem, const string& destino, float peso);
    bool deletar_pedido(int id);
    void listar_pedidos();
    bool atualizar_pedido(int id, const string& nova_origem, const string& novo_destino, float novo_peso);
    vector<Pedido> get_todos_pedidos();
    Pedido get_pedido_by_id(int id);
};
