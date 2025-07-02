# TrabalhoInterdisciplinar
Trabalho interdisciplinar de Fundamentos e AEDES

# Sistema de Gerenciamento de Entregas

Este é um projeto de sistema de gerenciamento de entregas desenvolvido em C++. Ele foi projetado para gerenciar locais, veículos, pedidos e rotas de entrega, com todos os dados sendo salvos diretamente em arquivos.

## Arquitetura do Projeto

O sistema foi construído com uma arquitetura modular e com persistência de dados em arquivos, o que significa que todas as informações não se perdem quando o programa é fechado.

### 1. Persistência de Dados em Arquivos Binários

- **Chega de Vetores em Memória:** Diferente de abordagens mais simples, este projeto não usa vetores em memória para guardar os dados. Em vez disso, todas as informações de **Locais, Veículos, Pedidos e Rotas** são gravadas e lidas diretamente de arquivos binários (`.dat`).
- **Dados Persistentes:** Isso garante que, uma vez que um dado é cadastrado, ele permanece salvo mesmo que o programa seja fechado e reaberto.
- **Exclusão Lógica:** Para não precisar reescrever o arquivo inteiro a cada exclusão, usamos um sistema de "exclusão lógica". Em vez de apagar um registro, nós apenas o marcamos como inativo (`ativo = false`). Ele continua no arquivo, mas o sistema o ignora nas operações do dia a dia.

### 2. Estrutura dos Módulos

O projeto é dividido em módulos para manter o código organizado:

- **`main.cpp`**: É o coração do programa. Ele contém o menu principal, onde o usuário interage com o sistema para escolher as opções (cadastrar, listar, etc.). Toda a lógica de interface com o usuário fica aqui.
- **`headers/`**: Esta pasta contém os arquivos de cabeçalho (`.h`). Eles definem como as classes (como `Local`, `Veiculo`) e os gerenciadores (como `GerenciadorLocais`) são estruturados.
- **`modulos/`**: Aqui fica a implementação (`.cpp`) da lógica de negócio. Cada arquivo (`cadastro_local.cpp`, `cadastro_veiculos.cpp`, etc.) contém o código que de fato salva, lê, atualiza e busca os dados nos arquivos binários.
- **`tests/`**: Contém os testes automatizados para garantir que cada parte do sistema funcione como esperado. Os testes verificam se os dados estão sendo gravados e lidos corretamente dos arquivos.

## Como Compilar e Executar

Para compilar o projeto, você pode usar o compilador g++. Certifique-se de incluir todos os arquivos `.cpp` da pasta `modulos` na compilação.

**Exemplo de comando para compilar:**
```bash
g++ main.cpp modulos/*.cpp -o programa.exe
```

**Para executar o programa:**
```bash
./programa.exe
```

**Para rodar os testes:**
Os testes usam o framework Catch2. Cada arquivo de teste pode ser compilado de forma independente para verificar um módulo específico.

**Exemplo para compilar e rodar o teste de locais:**
```bash
g++ tests/test_cadastro_local.cpp modulos/cadastro_local.cpp -o teste_local.exe
./teste_local.exe
```
