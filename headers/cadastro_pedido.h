#pragma once

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Pedido
{
private:
    int id;
    string origem;
    string destino;
    float peso;

public:
    Pedido(int id, const string &origem, const string &destino, float peso);
    int get_id() const;
    string get_origem() const;
    string get_destino() const;
    float get_peso() const;

    void set_origem(const string &novaOrigem);
    void set_destino(const string &novoDestino);
    void set_peso(float novoPeso);
};

class GerenciadorPedidos
{
private:
    bool localExiste(const vector<string> &locais, const string &local) const;

public:
    void cadastrarPedido(vector<Pedido> &pedidos, const vector<string> &locais);
    void listarPedidos(const vector<Pedido> &pedidos) const;
    void excluirPedido(vector<Pedido> &pedidos);
    void editarPedido(vector<Pedido> &pedidos, const vector<string> &locais);
};
