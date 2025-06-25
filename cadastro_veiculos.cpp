#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <locale.h>

using namespace std;

struct Veiculo
{
    string placa;
    string modelo;
    string status;
    string localAtual;
};

struct Local
{
    string nome;
};

void exibirMenu();
void limparBufferEntrada();
void cadastrarVeiculo(vector<Veiculo> &veiculos, const vector<Local> &locais);
void cadastrarLocal(vector<Local> &locais);
void listarVeiculos(const vector<Veiculo> &veiculos);
void listarLocais(const vector<Local> &locais);

int main()
{
    setlocale(LC_ALL, "Portuguese");

    vector<Veiculo> veiculos;
    vector<Local> locais;
    int escolha;

    cout << "--- Sistema de Gerenciamento de Frota ---" << endl;

    do
    {
        exibirMenu();
        cin >> escolha;
        limparBufferEntrada();

        switch (escolha)
        {
        case 1:
            cadastrarLocal(locais);
            break;
        case 2:
            cadastrarVeiculo(veiculos, locais);
            break;
        case 3:
            listarLocais(locais);
            break;
        case 4:
            listarVeiculos(veiculos);
            break;
        case 5:
            cout << "\nSaindo do sistema..." << endl;
            break;
        default:
            cerr << "\n❌ Opção inválida! Por favor, tente novamente." << endl;
            break;
        }
        if (escolha != 5)
        {
            cout << "\nPressione Enter para continuar...";
            cin.get();
        }

    } while (escolha != 5);

    return 0;
}

void exibirMenu()
{
    cout << "\n--- Menu Principal ---" << endl;
    cout << "1. Cadastrar Novo Local" << endl;
    cout << "2. Cadastrar Novo Veículo" << endl;
    cout << "3. Listar Locais Cadastrados" << endl;
    cout << "4. Listar Veículos Cadastrados" << endl;
    cout << "5. Sair" << endl;
    cout << "----------------------" << endl;
    cout << "Digite sua escolha: ";
}

void limparBufferEntrada()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void cadastrarLocal(vector<Local> &locais)
{
    cout << "\n--- Cadastro de Novo Local ---" << endl;
    Local novoLocal;

    cout << "Digite o nome do novo local: ";
    getline(cin, novoLocal.nome);

    for (const auto &local : locais)
    {
        if (local.nome == novoLocal.nome)
        {
            cerr << "❌ Erro: O local '" << novoLocal.nome << "' já existe. Cadastro cancelado." << endl;
            return;
        }
    }

    locais.push_back(novoLocal);
    cout << "\n✅ Local '" << novoLocal.nome << "' cadastrado com sucesso!" << endl;
}

void cadastrarVeiculo(vector<Veiculo> &veiculos, const vector<Local> &locais)
{
    cout << "\n--- Cadastro de Novo Veículo ---" << endl;

    if (locais.empty())
    {
        cerr << "❌ Erro: Nenhum local cadastrado. Cadastre um local antes de adicionar um veículo." << endl;
        return;
    }

    Veiculo novoVeiculo;

    cout << "Digite a placa (ex: ABC-1234): ";
    getline(cin, novoVeiculo.placa);

    for (const auto &veiculo : veiculos)
    {
        if (veiculo.placa == novoVeiculo.placa)
        {
            cerr << "❌ Erro: A placa '" << novoVeiculo.placa << "' já existe. Cadastro cancelado." << endl;
            return;
        }
    }

    cout << "Digite o modelo do veículo (ex: Van, Moto, Caminhão): ";
    getline(cin, novoVeiculo.modelo);

    int statusEscolhido = 0;
    while (statusEscolhido < 1 || statusEscolhido > 2)
    {
        cout << "Selecione o status do veículo:" << endl;
        cout << "  1. Disponível" << endl;
        cout << "  2. Ocupado" << endl;
        cout << "Opção: ";
        cin >> statusEscolhido;
        limparBufferEntrada();
        if (statusEscolhido < 1 || statusEscolhido > 2)
        {
            cerr << "❌ Opção de status inválida. Tente novamente." << endl;
        }
    }

    if (statusEscolhido == 1)
    {
        novoVeiculo.status = "Disponível";
    }
    else
    {
        novoVeiculo.status = "Ocupado";
    }

    cout << "Locais disponíveis para definir a posição atual:" << endl;
    for (const auto &local : locais)
    {
        cout << "- " << local.nome << endl;
    }

    bool localValido = false;
    while (!localValido)
    {
        cout << "Digite o local atual (deve ser um local da lista acima): ";
        getline(cin, novoVeiculo.localAtual);

        for (const auto &local : locais)
        {
            if (local.nome == novoVeiculo.localAtual)
            {
                localValido = true;
                break;
            }
        }

        if (!localValido)
        {
            cerr << "❌ Erro: O local '" << novoVeiculo.localAtual << "' não é válido. Tente novamente." << endl;
        }
    }

    veiculos.push_back(novoVeiculo);
    cout << "\n✅ Veículo cadastrado com sucesso!" << endl;
}

void listarVeiculos(const vector<Veiculo> &veiculos)
{
    cout << "\n--- Lista de Veículos Cadastrados ---" << endl;
    if (veiculos.empty())
    {
        cout << "Nenhum veículo cadastrado no sistema." << endl;
    }
    else
    {
        for (const auto &veiculo : veiculos)
        {
            cout << "Placa: " << veiculo.placa
                 << " | Modelo: " << veiculo.modelo
                 << " | Status: " << veiculo.status
                 << " | Local Atual: " << veiculo.localAtual << endl;
        }
    }
    cout << "------------------------------------" << endl;
}

void listarLocais(const vector<Local> &locais)
{
    cout << "\n--- Lista de Locais Cadastrados ---" << endl;
    if (locais.empty())
    {
        cout << "Nenhum local cadastrado no sistema." << endl;
    }
    else
    {
        for (const auto &local : locais)
        {
            cout << "- " << local.nome << endl;
        }
    }
    cout << "-----------------------------------" << endl;
}