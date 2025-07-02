
#include <iostream>
#include <cmath>
#include "../headers/Roteamento.h"
using namespace std;


float Roteamento::calcularDistancia(const Local& origem, const Local& destino)
{
    float dx = origem.get_x() - destino.get_x();
    float dy = origem.get_y() - destino.get_y();
    return sqrt(dx * dx + dy * dy);
}

void Roteamento::atribuirPedidosAosVeiculos(const vector<Pedido>& pedidos, vector<Veiculo>& veiculos, const vector<Local>& locais)
{
    for (int p = 0; p < pedidos.size(); p++)
    {
        float menorDist = 1e9;
        int idxVeic = -1;
        int idxDestino = -1;

        // encontra o indice do local de destino do pedido
        for (int l = 0; l < locais.size(); l++)
        {
            if (locais[l].get_nome() == pedidos[p].get_destino())
            {
                idxDestino = l;
                break;
            }
        }
        if (idxDestino == -1) continue; // local de entrega nao encontrado

        for (int v = 0; v < veiculos.size(); v++)
        {
            if (veiculos[v].get_status() == "disponivel")
            {
                float dist = calcularDistancia(veiculos[v].get_localAtual(), locais[idxDestino]);
                if (dist < menorDist)
                {
                    menorDist = dist;
                    idxVeic = v;
                }
            }
        }
        if (idxVeic != -1)
        {
            veiculos[idxVeic].atribuirPedido(pedidos[p]);
            veiculos[idxVeic].set_status("ocupado");
            veiculos[idxVeic].set_localAtual(locais[idxDestino]);
        }
    }
}

// gera as rotas para cada veiculo
void Roteamento::gerarRotas(const vector<Pedido>& pedidos, vector<Veiculo>& veiculos, const vector<Local>& locais)
{
    rotas.clear();
    for (int v = 0; v < veiculos.size(); v++)
    {
        if (veiculos[v].get_status() == "ocupado")
        {
            vector<Local> rota;
            rota.push_back(veiculos[v].get_localOrigem());
            rota.push_back(veiculos[v].get_localAtual());
            rotas.push_back(rota);
        }
    }
}

// exibe as rotas calculadas
void Roteamento::exibirRotas() const
{
    cout << "Rotas calculadas:" << endl;
    for (int i = 0; i < rotas.size(); i++)
    {
        cout << "Veículo " << i+1 << ": ";
        for (int j = 0; j < rotas[i].size(); j++)
        {
            cout << rotas[i][j].get_nome();
            if (j < rotas[i].size() - 1)
                cout << " -> ";
        }
        cout << " -> FIM" << endl;
    }
}
