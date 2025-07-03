#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include "../headers/cadastro_rota.h"
#include "../headers/cadastro_local.h"
#include "../headers/cadastro_pedido.h"
#include "../headers/cadastro_veiculos.h"

using namespace std;

// Implementação dos métodos da classe Rota
Rota::Rota() : num_pedidos(0), ativa(false) {
    placa_veiculo[0] = '\0';
}

Rota::Rota(const string& placa) : num_pedidos(0), ativa(true) {
    strncpy(placa_veiculo, placa.c_str(), TAM_PLACA_ROTA - 1);
    placa_veiculo[TAM_PLACA_ROTA - 1] = '\0';
}

void Rota::adicionar_pedido(int id_pedido) {
    if (num_pedidos < MAX_PONTOS_ROTA) {
        ids_pedidos[num_pedidos++] = id_pedido;
    }
}

string Rota::get_placa_veiculo() const { return string(placa_veiculo); }
const int* Rota::get_ids_pedidos() const { return ids_pedidos; }
int Rota::get_num_pedidos() const { return num_pedidos; }
bool Rota::is_ativa() const { return ativa; }
void Rota::desativar() { ativa = false; }

// Implementação dos métodos da classe GerenciadorRotas
GerenciadorRotas::GerenciadorRotas(GerenciadorLocais& gl, GerenciadorPedidos& gp, GerenciadorVeiculos& gv)
    : gerenciadorLocais(gl), gerenciadorPedidos(gp), gerenciadorVeiculos(gv) {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        ofstream novoArquivo(nomeArquivo, ios::binary);
        novoArquivo.close();
    }
}

void GerenciadorRotas::limpar_rotas_antigas() {
    ofstream arquivo(nomeArquivo, ios::trunc | ios::binary);
    if (arquivo) {
        cout << "Arquivo de rotas limpo." << endl;
    }
}

float GerenciadorRotas::calcular_distancia(const Local& l1, const Local& l2) {
    if (!l1.is_ativo() || !l2.is_ativo()) return -1.0f; // Indica erro
    float dx = l1.get_x() - l2.get_x();
    float dy = l1.get_y() - l2.get_y();
    return sqrt(dx * dx + dy * dy);
}

Local GerenciadorRotas::encontrar_local(const string& nome_local, const vector<Local>& locais) {
    for (const auto& local : locais) {
        if (local.get_nome() == nome_local) {
            return local;
        }
    }
    return Local(); // Retorna local inativo se não encontrar
}

void GerenciadorRotas::gerar_rotas_otimizadas() {
    limpar_rotas_antigas();

    vector<Veiculo> veiculos = gerenciadorVeiculos.get_todos_veiculos();
    vector<Pedido> pedidos = gerenciadorPedidos.get_todos_pedidos();
    vector<Local> locais = gerenciadorLocais.get_todos_locais();

    if (veiculos.empty() || pedidos.empty() || locais.empty()) {
        cout << "Dados insuficientes para gerar rotas (veículos, pedidos ou locais)." << endl;
        return;
    }

    map<int, bool> pedido_atribuido;
    for(const auto& p : pedidos) {
        pedido_atribuido[p.get_id()] = false;
    }

    vector<Rota> rotas_geradas;

    for (auto& veiculo : veiculos) {
        Rota nova_rota(veiculo.get_placa());
        float capacidade_restante = veiculo.get_capacidade();
        Local local_atual = encontrar_local("Garagem", locais);
        if (!local_atual.is_ativo()) {
            cout << "Erro: Local 'Garagem' não encontrado. Verifique o cadastro de locais." << endl;
            continue; // Pula para o próximo veículo
        }

        while (true) {
            int melhor_pedido_id = -1;
            float menor_distancia = -1.0f;

            for (const auto& pedido : pedidos) {
                if (!pedido_atribuido[pedido.get_id()] && pedido.get_peso() <= capacidade_restante) {
                    Local destino_pedido = encontrar_local(pedido.get_destino(), locais);
                    if (destino_pedido.is_ativo()) {
                        float d = calcular_distancia(local_atual, destino_pedido);
                        if (d != -1.0f && (melhor_pedido_id == -1 || d < menor_distancia)) {
                            menor_distancia = d;
                            melhor_pedido_id = pedido.get_id();
                        }
                    }
                }
            }

            if (melhor_pedido_id != -1) {
                Pedido pedido_escolhido = gerenciadorPedidos.get_pedido_by_id(melhor_pedido_id);
                nova_rota.adicionar_pedido(pedido_escolhido.get_id());
                pedido_atribuido[pedido_escolhido.get_id()] = true;
                capacidade_restante -= pedido_escolhido.get_peso();
                local_atual = encontrar_local(pedido_escolhido.get_destino(), locais);
            } else {
                break; // Nenhum pedido mais pode ser atribuído a este veículo
            }
        }

        if (nova_rota.get_num_pedidos() > 0) {
            rotas_geradas.push_back(nova_rota);
        }
    }

    ofstream arquivo(nomeArquivo, ios::binary | ios::app);
    for (const auto& rota : rotas_geradas) {
        arquivo.write(reinterpret_cast<const char*>(&rota), sizeof(Rota));
    }

    cout << "Rotas geradas e salvas com sucesso!" << endl;
}

void GerenciadorRotas::listar_rotas_com_detalhes() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        cout << "Arquivo de rotas não encontrado ou vazio." << endl;
        return;
    }

    cout << "\n--- Rotas Calculadas ---" << endl;
    Rota rota_lida;
    bool encontrou = false;
    while (arquivo.read(reinterpret_cast<char*>(&rota_lida), sizeof(Rota))) {
        if (rota_lida.is_ativa()) {
            encontrou = true;
            Veiculo v = gerenciadorVeiculos.get_veiculo_by_placa(rota_lida.get_placa_veiculo());
            cout << "\nVeículo: " << v.get_placa() << " (" << v.get_tipo() << ")" << endl;
            cout << "  Rota: Garagem";

            const int* ids = rota_lida.get_ids_pedidos();
            int num_pedidos = rota_lida.get_num_pedidos();
            float peso_total = 0;

            for (int i = 0; i < num_pedidos; ++i) {
                Pedido p = gerenciadorPedidos.get_pedido_by_id(ids[i]);
                if (p.is_ativo()) {
                    cout << " -> " << p.get_destino();
                    peso_total += p.get_peso();
                }
            }
            cout << " -> Garagem (Retorno)" << endl;
            cout << "  Carga Total: " << peso_total << " kg (Capacidade: " << v.get_capacidade() << " kg)" << endl;
        }
    }

    if (!encontrou) {
        cout << "Nenhuma rota ativa encontrada." << endl;
    }
    cout << "------------------------" << endl;
}

vector<Rota> GerenciadorRotas::get_todas_rotas() {
    ifstream arquivo(nomeArquivo, ios::binary);
    vector<Rota> rotas;
    if (!arquivo) {
        return rotas;
    }

    Rota rota_lida;
    while (arquivo.read(reinterpret_cast<char*>(&rota_lida), sizeof(Rota))) {
        if (rota_lida.is_ativa()) {
            rotas.push_back(rota_lida);
        }
    }
    return rotas;
}
