#include <iostream>
#include <string>
#include <vector>
#include <limits>   // Necessário para numeric_limits
#include <algorithm> // Necessário para std::transform
#include <locale.h>  // Necessário para setlocale
#include <windows.h> // Necessário para corrigir a acentuação no console Windows

// Inclui a definição das classes Local e GerenciadorLocais
#include "cadastro_local.h"

using namespace std;

// --- CLASSE VEÍCULO ---
// Representa um veículo com suas propriedades básicas.
class Veiculo
{
private:
    string placa;
    string modelo;
    string status;
    string localAtual;

public:
    // Construtor
    Veiculo(string placa, string modelo, string status, string localAtual) {
        this->placa = placa;
        this->modelo = modelo;
        this->status = status;
        this->localAtual = localAtual;
    }

    // Getters (métodos de acesso)
    string get_placa() const { return this->placa; }
    string get_modelo() const { return this->modelo; }
    string get_status() const { return this->status; }
    string get_localAtual() const { return this->localAtual; }

    // Setters (métodos de modificação)
    void change_modelo(string novo_modelo) { this->modelo = novo_modelo; }
    void change_status(string novo_status) { this->status = novo_status; }
    void change_localAtual(string novo_local) { this->localAtual = novo_local; }
};


// --- CLASSE GERENCIADOR DE VEÍCULOS ---
// Centraliza todas as operações relacionadas a veículos.
class GerenciadorVeiculos
{
private:
    vector<Veiculo> veiculos;

public:
    GerenciadorVeiculos() {} // Construtor

    // Cadastra um novo veículo, validando o local contra a lista de locais existentes.
    void cadastrar_veiculo(vector<Local>& locais_disponiveis) {
        cout << "\n--- Cadastro de Novo Veiculo ---" << endl;
        if (locais_disponiveis.empty()) {
            cout << "ERRO: E necessario cadastrar um local antes de um veiculo." << endl;
            return;
        }

        string placa, modelo, status, localAtual;

        cout << "Digite a placa: ";
        getline(cin, placa);

        // Verifica se a placa já existe
        for (int i = 0; i < veiculos.size(); i++) {
            if (veiculos[i].get_placa() == placa) {
                cout << "ERRO: Veiculo com esta placa ja existe." << endl;
                return;
            }
        }

        cout << "Digite o modelo: ";
        getline(cin, modelo);
        
        // Valida o status
        while (true) {
            cout << "Digite o status (Disponivel/Ocupado): ";
            getline(cin, status);
            string status_lower = status;
            transform(status_lower.begin(), status_lower.end(), status_lower.begin(), ::tolower);

            if (status_lower == "disponivel") {
                status = "Disponivel";
                break;
            } else if (status_lower == "ocupado") {
                status = "Ocupado";
                break;
            } else {
                cout << "ERRO: Status invalido. Tente novamente." << endl;
            }
        }

        cout << "\nLocais disponiveis:" << endl;
        for(int i = 0; i < locais_disponiveis.size(); i++) {
            cout << "- " << locais_disponiveis[i].get_nome() << endl;
        }

        // Valida o local
        bool localValido = false;
        while (!localValido) {
            cout << "Digite o local atual (deve ser um da lista): ";
            getline(cin, localAtual);
            for (int i = 0; i < locais_disponiveis.size(); i++) {
                if (locais_disponiveis[i].get_nome() == localAtual) {
                    localValido = true;
                    break;
                }
            }
            if (!localValido) {
                cout << "ERRO: Local nao encontrado. Tente novamente." << endl;
            }
        }

        Veiculo novoVeiculo(placa, modelo, status, localAtual);
        veiculos.push_back(novoVeiculo);
        cout << "\nVeiculo cadastrado com sucesso!" << endl;
    }

    // Lista todos os veículos cadastrados.
    void listar_veiculos() const {
        cout << "\n--- Lista de Veiculos Cadastrados ---" << endl;
        if (veiculos.empty()) {
            cout << "Nenhum veiculo cadastrado no sistema." << endl;
            return;
        }
        for (int i = 0; i < veiculos.size(); i++) {
            cout << "Placa: " << veiculos[i].get_placa()
                 << " | Modelo: " << veiculos[i].get_modelo()
                 << " | Status: " << veiculos[i].get_status()
                 << " | Local: " << veiculos[i].get_localAtual() << endl;
        }
    }

    // Atualiza os dados de um veículo existente.
    void atualizar_veiculo(string placa, vector<Local>& locais_disponiveis) {
        int indice_veiculo = -1;
        for (int i = 0; i < veiculos.size(); i++) {
            if (veiculos[i].get_placa() == placa) {
                indice_veiculo = i;
                break;
            }
        }

        if (indice_veiculo == -1) {
            cout << "ERRO: Veiculo com a placa '" << placa << "' nao foi encontrado." << endl;
            return;
        }
        
        string novo_modelo, novo_status, novo_local;

        cout << "Digite o novo modelo: ";
        getline(cin, novo_modelo);

        while (true) {
            cout << "Digite o novo status (Disponivel/Ocupado): ";
            getline(cin, novo_status);
            string status_lower = novo_status;
            transform(status_lower.begin(), status_lower.end(), status_lower.begin(), ::tolower);
            if (status_lower == "disponivel") { novo_status = "Disponivel"; break; }
            else if (status_lower == "ocupado") { novo_status = "Ocupado"; break; }
            else { cout << "ERRO: Status invalido." << endl; }
        }

        cout << "\nLocais disponiveis:" << endl;
        for(int i = 0; i < locais_disponiveis.size(); i++) {
            cout << "- " << locais_disponiveis[i].get_nome() << endl;
        }

        bool localValido = false;
        while (!localValido) {
            cout << "Digite o novo local (deve ser um da lista): ";
            getline(cin, novo_local);
            for (int i = 0; i < locais_disponiveis.size(); i++) {
                if (locais_disponiveis[i].get_nome() == novo_local) {
                    localValido = true;
                    break;
                }
            }
            if (!localValido) {
                cout << "ERRO: Local nao encontrado. Tente novamente." << endl;
            }
        }

        veiculos[indice_veiculo].change_modelo(novo_modelo);
        veiculos[indice_veiculo].change_status(novo_status);
        veiculos[indice_veiculo].change_localAtual(novo_local);
        cout << "Veiculo atualizado com sucesso!" << endl;
    }

    // Deleta um veículo do sistema.
    void deletar_veiculo(string placa) {
        for (int i = 0; i < veiculos.size(); i++) {
            if (veiculos[i].get_placa() == placa) {
                veiculos.erase(veiculos.begin() + i);
                cout << "Veiculo deletado com sucesso!" << endl;
                return;
            }
        }
        cout << "ERRO: Veiculo com a placa '" << placa << "' nao foi encontrado." << endl;
    }
};


// --- FUNÇÃO PARA EXIBIR O MENU ---
void exibirMenu() {
    cout << "\n\n===== MENU DE GESTAO DE FROTA =====\n";
    cout << "--- VEICULOS ---\n";
    cout << "1. Cadastrar Veiculo\n";
    cout << "2. Listar Veiculos\n";
    cout << "3. Atualizar Veiculo\n";
    cout << "4. Deletar Veiculo\n";
    cout << "--- LOCAIS ---\n";
    cout << "5. Criar Local\n";
    cout << "6. Listar Locais\n";
    cout << "7. Atualizar Local\n";
    cout << "8. Deletar Local\n";
    cout << "-----------------\n";
    cout << "0. Sair\n";
    cout << "Escolha uma opcao: ";
}


// --- FUNÇÃO PRINCIPAL ---
int main() {
    // Comandos para corrigir a acentuação no console do Windows
    setlocale(LC_ALL, "Portuguese");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Instancia os gerenciadores
    GerenciadorLocais gerenciador_local;
    GerenciadorVeiculos gerenciador_veiculos;

    int opcao = -1;
    while (opcao != 0) {
        exibirMenu();
        cin >> opcao;

        // Limpa o buffer de entrada para evitar problemas com getline
        if (cin.fail()) {
            cout << "Opcao invalida! Por favor, digite um numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (opcao) {
            case 1: {
                // Acessamos o vetor público 'locais' do gerenciador_local
                gerenciador_veiculos.cadastrar_veiculo(gerenciador_local.locais);
                break;
            }
            case 2: {
                gerenciador_veiculos.listar_veiculos();
                break;
            }
            case 3: {
                string placa;
                cout << "Digite a placa do veiculo para atualizar: ";
                getline(cin, placa);
                gerenciador_veiculos.atualizar_veiculo(placa, gerenciador_local.locais);
                break;
            }
            case 4: {
                string placa_para_deletar;
                cout << "Digite a placa do veiculo para deletar: ";
                getline(cin, placa_para_deletar);
                gerenciador_veiculos.deletar_veiculo(placa_para_deletar);
                break;
            }
            case 5: {
                string nome;
                float x, y;
                cout << "Digite o nome do novo local: ";
                getline(cin, nome);
                cout << "Digite a coordenada X: ";
                cin >> x;
                cout << "Digite a coordenada Y: ";
                cin >> y;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                gerenciador_local.criar_local(nome, x, y);
                break;
            }
            case 6: {
                gerenciador_local.listar_locais();
                break;
            }
            case 7: {
                string nome_atual, novo_nome;
                float x, y;
                cout << "Digite o nome do local que deseja atualizar: ";
                getline(cin, nome_atual);
                cout << "Digite o NOVO nome para o local: ";
                getline(cin, novo_nome);
                cout << "Digite a NOVA coordenada X: ";
                cin >> x;
                cout << "Digite a NOVA coordenada Y: ";
                cin >> y;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                gerenciador_local.atualizar(nome_atual, novo_nome, x, y);
                break;
            }
            case 8: {
                string local_para_deletar;
                cout << "Digite o nome do local para deletar: ";
                getline(cin, local_para_deletar);
                gerenciador_local.deletar_local(local_para_deletar);
                break;
            }
            case 0: {
                cout << "Saindo do sistema..." << endl;
                break;
            }
            default: {
                cout << "Opcao invalida! Tente novamente." << endl;
                break;
            }
        }
    }

    return 0;
}