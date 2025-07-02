#include <iostream>
#include <cmath>
#include "../headers/Roteamento.h"
#include "../headers/cadastro_rota.h"
#include "../headers/cadastro_local.h"
#include "../headers/cadastro_pedido.h"
#include "../headers/cadastro_veiculos.h"
#include <fstream>
#include <vector>
#include <algorithm>

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

// Construtor: Garante que o arquivo de rotas exista.
GerenciadorRotas::GerenciadorRotas() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        ofstream novoArquivo(nomeArquivo, ios::binary);
        novoArquivo.close();
    }
}

// Limpa o arquivo de rotas para uma nova geração.
void GerenciadorRotas::limpar_rotas_antigas() {
    ofstream arquivo(nomeArquivo, ios::trunc | ios::binary);
    arquivo.close();
}

// Helper para calcular a distância euclidiana entre dois locais.
float GerenciadorRotas::calcular_distancia(const Local& l1, const Local& l2) {
    float dx = l1.get_x() - l2.get_x();
    float dy = l1.get_y() - l2.get_y();
    return sqrt(dx * dx + dy * dy);
}

// Helper para encontrar um objeto Local pelo nome.
Local GerenciadorRotas::encontrar_local(const string& nome_local, const vector<Local>& locais) {
    for (const auto& local : locais) {
        if (local.get_nome() == nome_local) {
            return local;
        }
    }
    return Local(); // Retorna um local inválido/inativo se não encontrar.
}

// Lógica principal para gerar e salvar as rotas.
void GerenciadorRotas::gerar_rotas() {
    limpar_rotas_antigas();

    GerenciadorVeiculos gv;
    GerenciadorPedidos gp;
    GerenciadorLocais gl;

    vector<Veiculo> veiculos = gv.get_todos_veiculos();
    vector<Pedido> pedidos = gp.get_todos_pedidos();
    vector<Local> locais = gl.get_todos_locais();

    if (veiculos.empty() || pedidos.empty() || locais.empty()) {
        cout << "Dados insuficientes para gerar rotas (veiculos, pedidos ou locais)." << endl;
        return;
    }

    vector<Rota> rotas_geradas;
    vector<bool> pedido_atribuido(pedidos.size(), false);

    for (auto& veiculo : veiculos) {
        Rota nova_rota(veiculo.get_placa());
        Local local_partida = encontrar_local("Garagem", locais); // Assumindo que todos saem da Garagem

        while (true) {
            float menor_distancia = -1;
            int melhor_pedido_idx = -1;

            for (int i = 0; i < pedidos.size(); ++i) {
                if (!pedido_atribuido[i]) {
                    Local destino_pedido = encontrar_local(pedidos[i].get_local_entrega(), locais);
                    if (destino_pedido.is_ativo()) {
                        float d = calcular_distancia(local_partida, destino_pedido);
                        if (melhor_pedido_idx == -1 || d < menor_distancia) {
                            menor_distancia = d;
                            melhor_pedido_idx = i;
                        }
                    }
                }
            }

            if (melhor_pedido_idx != -1) {
                nova_rota.adicionar_pedido(pedidos[melhor_pedido_idx].get_id());
                pedido_atribuido[melhor_pedido_idx] = true;
                local_partida = encontrar_local(pedidos[melhor_pedido_idx].get_local_entrega(), locais);
            } else {
                break; // Nenhum pedido restante para atribuir
            }
        }

        if (nova_rota.get_num_pedidos() > 0) {
            rotas_geradas.push_back(nova_rota);
        }
    }

    // Salva as rotas geradas no arquivo binário
    ofstream arquivo(nomeArquivo, ios::binary | ios::app);
    for (const auto& rota : rotas_geradas) {
        arquivo.write(reinterpret_cast<const char*>(&rota), sizeof(Rota));
    }

    cout << "Rotas geradas e salvas com sucesso!" << endl;
}

// Lista as rotas salvas no arquivo binário.
void GerenciadorRotas::listar_rotas() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        cout << "Arquivo de rotas nao encontrado." << endl;
        return;
    }

    cout << "\n--- Rotas Calculadas ---" << endl;
    Rota rota_lida;
    bool encontrou = false;
    while (arquivo.read(reinterpret_cast<char*>(&rota_lida), sizeof(Rota))) {
        if (rota_lida.is_ativa()) {
            encontrou = true;
            cout << "Veiculo Placa: " << rota_lida.get_placa_veiculo() << endl;
            cout << "  Sequencia de Pedidos (IDs): ";
            const int* ids = rota_lida.get_ids_pedidos();
            for (int i = 0; i < rota_lida.get_num_pedidos(); ++i) {
                cout << ids[i] << (i == rota_lida.get_num_pedidos() - 1 ? "" : " -> ");
            }
            cout << "\n------------------------" << endl;
        }
    }

    if (!encontrou) {
        cout << "Nenhuma rota gerada ou salva." << endl;
    }
}
