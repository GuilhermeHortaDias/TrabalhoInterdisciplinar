#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <algorithm>
#include "headers/cadastro_local.h"
#include "headers/cadastro_veiculos.h"
#include "headers/cadastro_pedido.h"
#include "headers/cadastro_rota.h"

using namespace std;

// Função para limpar o buffer de entrada
void limparBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- Menu de Gerenciamento de Locais ---
void menuLocais(GerenciadorLocais& gerenciador) {
    int opcao;
    do {
        cout << "\n--- Menu de Locais ---\n";
        cout << "1. Cadastrar Local\n";
        cout << "2. Listar Locais\n";
        cout << "3. Atualizar Local\n";
        cout << "4. Excluir Local\n";
        cout << "5. Voltar ao Menu Principal\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        if (cin.fail()) {
            cin.clear();
            limparBuffer();
            cout << "Entrada inválida. Por favor, insira um número.\n";
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: {
                string nome;
                float x, y;
                cout << "Nome do Local: ";
                getline(cin, nome);
                cout << "Coordenada X: ";
                cin >> x;
                cout << "Coordenada Y: ";
                cin >> y;
                limparBuffer();
                gerenciador.criar_local(nome, x, y);
                break;
            }
            case 2:
                gerenciador.listar_locais();
                break;
            case 3: {
                string nome_atual, novo_nome;
                float x, y;
                cout << "Nome do local a ser atualizado: ";
                getline(cin, nome_atual);
                cout << "Novo Nome: ";
                getline(cin, novo_nome);
                cout << "Nova Coordenada X: ";
                cin >> x;
                cout << "Nova Coordenada Y: ";
                cin >> y;
                limparBuffer();
                gerenciador.atualizar_local(nome_atual, novo_nome, x, y);
                break;
            }
            case 4: {
                string nome;
                cout << "Nome do local a ser excluído: ";
                getline(cin, nome);
                gerenciador.deletar_local(nome);
                break;
            }
            case 5:
                break;
            default:
                cout << "Opção inválida!\n";
        }
    } while (opcao != 5);
}

// --- Menu de Gerenciamento de Veículos ---
void menuVeiculos(GerenciadorVeiculos& gerenciador) {
    int opcao;
    do {
        cout << "\n--- Menu de Veículos ---\n";
        cout << "1. Cadastrar Veículo\n";
        cout << "2. Listar Veículos\n";
        cout << "3. Atualizar Veículo\n";
        cout << "4. Excluir Veículo\n";
        cout << "5. Voltar ao Menu Principal\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        if (cin.fail()) {
            cin.clear();
            limparBuffer();
            cout << "Entrada inválida. Por favor, insira um número.\n";
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: {
                string tipo, placa;
                float capacidade;
                cout << "Tipo do Veículo: ";
                getline(cin, tipo);
                cout << "Placa: ";
                getline(cin, placa);
                cout << "Capacidade (kg): ";
                cin >> capacidade;
                limparBuffer();
                gerenciador.criar_veiculo(tipo, placa, capacidade);
                break;
            }
            case 2:
                gerenciador.listar_veiculos();
                break;
            case 3: {
                string placa_atual, novo_tipo, nova_placa;
                float nova_capacidade;
                cout << "Placa do veículo a ser atualizado: ";
                getline(cin, placa_atual);
                cout << "Novo Tipo: ";
                getline(cin, novo_tipo);
                cout << "Nova Placa: ";
                getline(cin, nova_placa);
                cout << "Nova Capacidade (kg): ";
                cin >> nova_capacidade;
                limparBuffer();
                gerenciador.atualizar_veiculo(placa_atual, novo_tipo, nova_placa, nova_capacidade);
                break;
            }
            case 4: {
                string placa;
                cout << "Placa do veículo a ser excluído: ";
                getline(cin, placa);
                gerenciador.deletar_veiculo(placa);
                break;
            }
            case 5:
                break;
            default:
                cout << "Opção inválida!\n";
        }
    } while (opcao != 5);
}

// --- Menu de Gerenciamento de Pedidos ---
void menuPedidos(GerenciadorPedidos& gerenciadorPedidos, GerenciadorLocais& gerenciadorLocais) {
    int opcao;
    do {
        cout << "\n--- Menu de Pedidos ---\n";
        cout << "1. Cadastrar Pedido\n";
        cout << "2. Listar Pedidos\n";
        cout << "3. Atualizar Pedido\n";
        cout << "4. Excluir Pedido\n";
        cout << "5. Voltar ao Menu Principal\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        if (cin.fail()) {
            cin.clear();
            limparBuffer();
            cout << "Entrada inválida. Por favor, insira um número.\n";
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: {
                string origem, destino;
                float peso;
                cout << "--- Locais Disponíveis ---" << endl;
                gerenciadorLocais.listar_locais();
                cout << "--------------------------" << endl;
                cout << "Local de Origem: ";
                getline(cin, origem);
                cout << "Local de Destino: ";
                getline(cin, destino);
                cout << "Peso (kg): ";
                cin >> peso;
                limparBuffer();
                // Validação simples se os locais existem
                if (gerenciadorLocais.get_local_by_nome(origem).is_ativo() && gerenciadorLocais.get_local_by_nome(destino).is_ativo()) {
                    gerenciadorPedidos.criar_pedido(origem, destino, peso);
                } else {
                    cout << "Erro: Origem ou Destino inválido. Verifique os locais cadastrados." << endl;
                }
                break;
            }
            case 2:
                gerenciadorPedidos.listar_pedidos();
                break;
            case 3: {
                int id;
                string nova_origem, novo_destino;
                float novo_peso;
                cout << "ID do pedido a ser atualizado: ";
                cin >> id;
                limparBuffer();
                cout << "--- Locais Disponíveis ---" << endl;
                gerenciadorLocais.listar_locais();
                cout << "--------------------------" << endl;
                cout << "Nova Origem: ";
                getline(cin, nova_origem);
                cout << "Novo Destino: ";
                getline(cin, novo_destino);
                cout << "Novo Peso (kg): ";
                cin >> novo_peso;
                limparBuffer();
                if (gerenciadorLocais.get_local_by_nome(nova_origem).is_ativo() && gerenciadorLocais.get_local_by_nome(novo_destino).is_ativo()) {
                    gerenciadorPedidos.atualizar_pedido(id, nova_origem, novo_destino, novo_peso);
                } else {
                    cout << "Erro: Origem ou Destino inválido. Verifique os locais cadastrados." << endl;
                }
                break;
            }
            case 4: {
                int id;
                cout << "ID do pedido a ser excluído: ";
                cin >> id;
                limparBuffer();
                gerenciadorPedidos.deletar_pedido(id);
                break;
            }
            case 5:
                break;
            default:
                cout << "Opção inválida!\n";
        }
    } while (opcao != 5);
}

// --- Menu de Gerenciamento de Rotas ---
void menuRotas(GerenciadorRotas& gerenciador) {
    int opcao;
    do {
        cout << "\n--- Menu de Rotas ---\n";
        cout << "1. Gerar Novas Rotas Otimizadas (apaga as antigas)\n";
        cout << "2. Listar Rotas Atuais com Detalhes\n";
        cout << "3. Voltar ao Menu Principal\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        if (cin.fail()) {
            cin.clear();
            limparBuffer();
            cout << "Entrada inválida. Por favor, insira um número.\n";
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                gerenciador.gerar_rotas_otimizadas();
                break;
            case 2:
                gerenciador.listar_rotas_com_detalhes();
                break;
            case 3:
                break;
            default:
                cout << "Opção inválida!\n";
        }
    } while (opcao != 3);
}

// --- Função Principal ---
int main() {
    // Instancia todos os gerenciadores uma vez
    GerenciadorLocais gerenciadorLocais;
    GerenciadorVeiculos gerenciadorVeiculos;
    GerenciadorPedidos gerenciadorPedidos;
    GerenciadorRotas gerenciadorRotas(gerenciadorLocais, gerenciadorPedidos, gerenciadorVeiculos);

    int opcao;

    do {
        cout << "\n--- Sistema de Gerenciamento de Entregas ---\n";
        cout << "1. Gerenciar Locais\n";
        cout << "2. Gerenciar Veículos\n";
        cout << "3. Gerenciar Pedidos\n";
        cout << "4. Gerenciar Rotas\n";
        cout << "5. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        if (cin.fail()) {
            cin.clear();
            limparBuffer();
            cout << "Entrada inválida. Por favor, insira um número.\n";
            opcao = 0; 
        } else {
            limparBuffer(); 
        }

        switch (opcao) {
            case 1:
                menuLocais(gerenciadorLocais);
                break;
            case 2:
                menuVeiculos(gerenciadorVeiculos);
                break;
            case 3:
                menuPedidos(gerenciadorPedidos, gerenciadorLocais);
                break;
            case 4:
                menuRotas(gerenciadorRotas);
                break;
            case 5:
                cout << "Saindo do sistema...\n";
                break;
            default:
                cout << "Opção inválida! Tente novamente.\n";
        }
    } while (opcao != 5);

    return 0;
}
