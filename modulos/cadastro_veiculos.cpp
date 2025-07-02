#include <iostream>
#include <limits>
#include <algorithm>
#include <locale.h>
#include <windows.h>
#include "../headers/cadastro_veiculos.h"
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

Veiculo::Veiculo(string placa, string modelo, string status, string localAtual) {
    this->placa = placa;
    this->modelo = modelo;
    this->status = status;
    this->localAtual = localAtual;
}

string Veiculo::get_placa() const { return this->placa; }
string Veiculo::get_modelo() const { return this->modelo; }
string Veiculo::get_status() const { return this->status; }
string Veiculo::get_localAtual() const { return this->localAtual; }

void Veiculo::change_modelo(string novo_modelo) { this->modelo = novo_modelo; }
void Veiculo::change_status(string novo_status) { this->status = novo_status; }
void Veiculo::change_localAtual(string novo_local) { this->localAtual = novo_local; }

GerenciadorVeiculos::GerenciadorVeiculos() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        ofstream novoArquivo(nomeArquivo, ios::binary);
        novoArquivo.close();
    }
}

int GerenciadorVeiculos::encontrar_pos_veiculo(const string& placa) {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) return -1;

    Veiculo veiculo_lido;
    int pos = 0;
    while (arquivo.read(reinterpret_cast<char*>(&veiculo_lido), sizeof(Veiculo))) {
        if (veiculo_lido.is_ativo() && veiculo_lido.get_placa() == placa) {
            return pos;
        }
        pos++;
    }
    return -1;
}

bool GerenciadorVeiculos::criar_veiculo(const string& tipo, const string& placa, float capacidade) {
    if (encontrar_pos_veiculo(placa) != -1) {
        cout << "Erro: Já existe um veículo com esta placa." << endl;
        return false;
    }

    Veiculo novo_veiculo(tipo, placa, capacidade);
    ofstream arquivo(nomeArquivo, ios::binary | ios::app);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo para escrita." << endl;
        return false;
    }

    arquivo.write(reinterpret_cast<const char*>(&novo_veiculo), sizeof(Veiculo));
    cout << "Veículo criado com sucesso!" << endl;
    return true;
}

bool GerenciadorVeiculos::deletar_veiculo(const string& placa) {
    int pos = encontrar_pos_veiculo(placa);
    if (pos == -1) {
        cout << "Erro: Veículo não encontrado." << endl;
        return false;
    }

    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    arquivo.seekg(pos * sizeof(Veiculo));
    Veiculo veiculo_a_deletar;
    arquivo.read(reinterpret_cast<char*>(&veiculo_a_deletar), sizeof(Veiculo));

    veiculo_a_deletar.desativar();

    arquivo.seekp(pos * sizeof(Veiculo));
    arquivo.write(reinterpret_cast<const char*>(&veiculo_a_deletar), sizeof(Veiculo));

    cout << "Veículo deletado com sucesso!" << endl;
    return true;
}

void GerenciadorVeiculos::listar_veiculos() {
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        cout << "Nenhum veículo cadastrado." << endl;
        return;
    }

    Veiculo veiculo_lido;
    bool encontrou = false;
    cout << "\n--- Lista de Veículos ---" << endl;
    while (arquivo.read(reinterpret_cast<char*>(&veiculo_lido), sizeof(Veiculo))) {
        if (veiculo_lido.is_ativo()) {
            cout << "Tipo: " << veiculo_lido.get_tipo()
                 << ", Placa: " << veiculo_lido.get_placa()
                 << ", Capacidade: " << veiculo_lido.get_capacidade() << endl;
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhum veículo ativo cadastrado." << endl;
    }
    cout << "-------------------------" << endl;
}

bool GerenciadorVeiculos::atualizar_veiculo(const string& placa_atual, const string& novo_tipo, const string& nova_placa, float nova_capacidade) {
    int pos = encontrar_pos_veiculo(placa_atual);
    if (pos == -1) {
        cout << "Erro: Veículo a ser atualizado não encontrado." << endl;
        return false;
    }

    int pos_novo = encontrar_pos_veiculo(nova_placa);
    if (pos_novo != -1 && pos_novo != pos) {
        cout << "Erro: Já existe outro veículo com a nova placa." << endl;
        return false;
    }

    fstream arquivo(nomeArquivo, ios::binary | ios::in | ios::out);
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    arquivo.seekg(pos * sizeof(Veiculo));
    Veiculo veiculo_a_atualizar;
    arquivo.read(reinterpret_cast<char*>(&veiculo_a_atualizar), sizeof(Veiculo));

    veiculo_a_atualizar.change_tipo(novo_tipo);
    veiculo_a_atualizar.change_placa(nova_placa);
    veiculo_a_atualizar.change_capacidade(nova_capacidade);

    arquivo.seekp(pos * sizeof(Veiculo));
    arquivo.write(reinterpret_cast<const char*>(&veiculo_a_atualizar), sizeof(Veiculo));

    cout << "Veículo atualizado com sucesso!" << endl;
    return true;
}

vector<Veiculo> GerenciadorVeiculos::get_todos_veiculos() {
    vector<Veiculo> veiculos_ativos;
    ifstream arquivo(nomeArquivo, ios::binary);
    if (!arquivo) {
        return veiculos_ativos;
    }

    Veiculo veiculo_lido;
    while (arquivo.read(reinterpret_cast<char*>(&veiculo_lido), sizeof(Veiculo))) {
        if (veiculo_lido.is_ativo()) {
            veiculos_ativos.push_back(veiculo_lido);
        }
    }

    return veiculos_ativos;
}

Veiculo GerenciadorVeiculos::get_veiculo_by_placa(const string& placa) {
    ifstream arquivo(nomeArquivo, ios::binary);
    Veiculo veiculo_lido;
    if (arquivo) {
        while (arquivo.read(reinterpret_cast<char*>(&veiculo_lido), sizeof(Veiculo))) {
            if (veiculo_lido.is_ativo() && veiculo_lido.get_placa() == placa) {
                return veiculo_lido;
            }
        }
    }
    return Veiculo(); // Retorna veículo inativo se não encontrado
}

void GerenciadorVeiculos::cadastrar_veiculo(vector<Local>& locais_disponiveis) {
    cout << "\n--- Cadastro de Novo Veiculo ---" << endl;
    if (locais_disponiveis.empty()) {
        cout << "ERRO: E necessario cadastrar um local antes de um veiculo." << endl;
        return;
    }

    string placa, modelo, status, localAtual;

    cout << "Digite a placa: ";
    getline(cin, placa);

    for (int i = 0; i < veiculos.size(); i++) {
        if (veiculos[i].get_placa() == placa) {
            cout << "ERRO: Veiculo com esta placa ja existe." << endl;
            return;
        }
    }

    cout << "Digite o modelo: ";
    getline(cin, modelo);
    
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

void GerenciadorVeiculos::listar_veiculos() const {
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

void GerenciadorVeiculos::atualizar_veiculo(string placa, vector<Local>& locais_disponiveis) {
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

void GerenciadorVeiculos::deletar_veiculo(string placa) {
    for (int i = 0; i < veiculos.size(); i++) {
        if (veiculos[i].get_placa() == placa) {
            veiculos.erase(veiculos.begin() + i);
            cout << "Veiculo deletado com sucesso!" << endl;
            return;
        }
    }
    cout << "ERRO: Veiculo com a placa '" << placa << "' nao foi encontrado." << endl;
}

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
/*
int main() {  
    setlocale(LC_ALL, "Portuguese");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    GerenciadorLocais gerenciador_local;
    GerenciadorVeiculos gerenciador_veiculos;

    int opcao = -1;
    while (opcao != 0) {
        exibirMenu();
        cin >> opcao;

        if (cin.fail()) {
            cout << "Opcao invalida! Por favor, digite um numero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (opcao) {
            case 1: {
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
} */