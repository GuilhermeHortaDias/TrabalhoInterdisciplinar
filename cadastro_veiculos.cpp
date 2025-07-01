#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <locale.h>

using namespace std;

class Local
{
private:
    string nome;

public:
    Local(string nome)
    {
        this->nome = nome;
    }

    string get_nome() const
    {
        return this->nome;
    }
};

class Veiculo
{
private:
    string placa;
    string modelo;
    string status;
    string localAtual;

public:
    Veiculo(string placa, string modelo, string status, string localAtual)
    {
        this->placa = placa;
        this->modelo = modelo;
        this->status = status;
        this->localAtual = localAtual;
    }

    string get_placa() { return this->placa; }
    string get_modelo() { return this->modelo; }
    string get_status() { return this->status; }
    string get_localAtual() { return this->localAtual; }

    void change_modelo(string novo_modelo) { this->modelo = novo_modelo; }
    void change_status(string novo_status) { this->status = novo_status; }
    void change_localAtual(string novo_local) { this->localAtual = novo_local; }
};

class GerenciadorVeiculos
{
private:
    vector<Veiculo> veiculos;

public:
    GerenciadorVeiculos() {}

    void cadastrar_veiculo(const vector<Local>& locais)
    {
        cout << "\n--- Cadastro de Novo Veículo ---" << endl;
        if (locais.empty())
        {
            cout << "ERRO: É necessário cadastrar um local antes de um veículo." << endl;
            return;
        }

        string placa, modelo, status, localAtual;

        cout << "Digite a placa: ";
        getline(cin, placa);

        for (int i = 0; i < veiculos.size(); i++)
        {
            if (veiculos[i].get_placa() == placa)
            {
                cout << "ERRO: Veículo com esta placa já existe." << endl;
                return;
            }
        }

        cout << "Digite o modelo: ";
        getline(cin, modelo);
        
        cout << "Digite o status (Disponivel/Ocupado): ";
        getline(cin, status);
        
        cout << "Digite o local atual: ";
        getline(cin, localAtual);

        Veiculo novoVeiculo(placa, modelo, status, localAtual);
        veiculos.push_back(novoVeiculo);
        cout << "\nVeículo cadastrado com sucesso!" << endl;
    }

    void listar_veiculos()
    {
        cout << "\n--- Lista de Veículos Cadastrados ---" << endl;
        if (veiculos.empty())
        {
            cout << "Nenhum veículo cadastrado no sistema." << endl;
            return;
        }

        for (int i = 0; i < veiculos.size(); i++)
        {
            cout << "Placa: " << veiculos[i].get_placa()
                 << " | Modelo: " << veiculos[i].get_modelo()
                 << " | Status: " << veiculos[i].get_status()
                 << " | Local: " << veiculos[i].get_localAtual() << endl;
        }
    }

    void atualizar_veiculo(string placa, string novo_modelo, string novo_status, string novo_local)
    {
        bool encontrado = false;
        for (int i = 0; i < veiculos.size(); i++)
        {
            if (veiculos[i].get_placa() == placa)
            {
                veiculos[i].change_modelo(novo_modelo);
                veiculos[i].change_status(novo_status);
                veiculos[i].change_localAtual(novo_local);
                encontrado = true;
                break;
            }
        }

        if (encontrado)
        {
            cout << "Veículo atualizado com sucesso!" << endl;
        }
        else
        {
            cout << "ERRO: Veículo com a placa '" << placa << "' não foi encontrado." << endl;
        }
    }

    void deletar_veiculo(string placa)
    {
        bool encontrado = false;
        for (int i = 0; i < veiculos.size(); i++)
        {
            if (veiculos[i].get_placa() == placa)
            {
                veiculos.erase(veiculos.begin() + i);
                encontrado = true;
                break;
            }
        }

        if (encontrado)
        {
            cout << "Veículo deletado com sucesso!" << endl;
        }
        else
        {
            cout << "ERRO: Veículo com a placa '" << placa << "' não foi encontrado." << endl;
        }
    }
};

int main()
{
    setlocale(LC_ALL, "Portuguese");

    vector<Local> locais_cadastrados;
    locais_cadastrados.push_back(Local("Garagem-A"));
    locais_cadastrados.push_back(Local("Patio-B"));
    locais_cadastrados.push_back(Local("Oficina-C"));

    GerenciadorVeiculos gerenciador;

    int opcao = -1;
    while (opcao != 0)
    {
        cout << "\n===== MENU DE GESTÃO DE FROTA =====\n";
        cout << "1. Cadastrar Veículo\n";
        cout << "2. Listar Veículos\n";
        cout << "3. Atualizar Veículo\n";
        cout << "4. Deletar Veículo\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcao)
        {
        case 1:
        {
            gerenciador.cadastrar_veiculo(locais_cadastrados);
            break;
        }
        case 2:
        {
            gerenciador.listar_veiculos();
            break;
        }
        case 3:
        {
            string placa, novo_modelo, novo_status, novo_local;
            cout << "Digite a placa do veículo para atualizar: ";
            getline(cin, placa);
            cout << "Digite o novo modelo: ";
            getline(cin, novo_modelo);
            cout << "Digite o novo status: ";
            getline(cin, novo_status);
            cout << "Digite o novo local: ";
            getline(cin, novo_local);
            gerenciador.atualizar_veiculo(placa, novo_modelo, novo_status, novo_local);
            break;
        }
        case 4:
        {
            string placa_para_deletar;
            cout << "Digite a placa do veículo para deletar: ";
            getline(cin, placa_para_deletar);
            gerenciador.deletar_veiculo(placa_para_deletar);
            break;
        }
        case 0:
            cout << "Saindo do sistema..." << endl;
            break;
        default:
            cout << "Opção inválida!" << endl;
            break;
        }
    }

    return 0;
}