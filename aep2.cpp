#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CLIENTES 100
#define MAX_USUARIO 50
#define MAX_SENHA 10
#define ARQUIVO "clientes.dat"

typedef struct {
    char usuario[MAX_USUARIO];
    char senha[MAX_SENHA];
} Cliente;

void cadastrarLoginSenha(Cliente clientes[], int *numClientes);
void editarLoginSenha(Cliente clientes[], int numClientes);
void excluirLoginSenha(Cliente clientes[], int *numClientes);
void listarClientes(Cliente clientes[], int numClientes);
void criptografarSenha(char *senha);
int validarSenha(const char *senha);
int usuarioJaExiste(Cliente clientes[], int numClientes, const char *usuario);
void salvarDados(Cliente clientes[], int numClientes);
void carregarDados(Cliente clientes[], int *numClientes);

int main() {
    Cliente clientes[MAX_CLIENTES];
    int numClientes = 0;
    
    carregarDados(clientes, &numClientes);

    int opcao;

    do {
        system("color F8");
        printf(" -----------------------------------------\n");
        printf("|            Menu de Clientes             |\n");
        printf(" -----------------------------------------\n");
        printf("| 1. Cadastrar cliente.                   |\n");
        printf("| 2. Editar dados do cliente.             |\n");
        printf("| 3. Excluir conta.                       |\n");
        printf("| 4. Listar clientes cadastrados.         |\n");
        printf("| 5. Sair.                                |\n");
        printf(" -----------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        system("cls");
        getchar();

        switch(opcao) {
            case 1:
                cadastrarLoginSenha(clientes, &numClientes);
                system("cls");
                break;
                
            case 2:
                editarLoginSenha(clientes, numClientes);
                system("cls");
                break;
            case 3:
                excluirLoginSenha(clientes, &numClientes);
                system("cls");
                break;
            case 4:
                listarClientes(clientes, numClientes);
                system("pause");
                system("cls");
                break;
                
            case 5:
                salvarDados(clientes, numClientes);
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while(opcao != 5);

    return 0;
}

void criptografarSenha(char *senha) {
    for (int i = 0; i < strlen(senha); i++) {
        senha[i] = senha[i] + 1; // Soma 1 ao valor ASCII de cada caractere
    }
}

int validarSenha(const char *senha) {
    int temMaiuscula = 0, temSimbolo = 0;

    if (strlen(senha) < 4) {
        printf("Erro: A senha deve ter no minimo 4 caracteres, conter pelo menos uma letra maiuscula e um simbolo.\n");
        return 0;
    }

    for (int i = 0; i < strlen(senha); i++) {
        if (isupper(senha[i])) {
            temMaiuscula = 1;
        }
        if (ispunct(senha[i])) {
            temSimbolo = 1;
        }
    }

    if (!temMaiuscula || !temSimbolo) {
        printf("Erro: A senha deve conter pelo menos uma letra maiuscula e um simbolo.\n");
        return 0;
    }

    return 1;
}

int usuarioJaExiste(Cliente clientes[], int numClientes, const char *usuario) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].usuario, usuario) == 0) { 
            return 1;                 // se retornar 1 o usuário já esta cadastrado 
        } 
    }
    return 0;                         // retorna 0 e pode adicionar mais 1 usuário
}

void cadastrarLoginSenha(Cliente clientes[], int *numClientes) {
	system("color 3F");
    if (*numClientes >= MAX_CLIENTES) {
        printf("Limite de clientes atingido!\n");
        return;
    }

    Cliente novoCliente;
    int usuarioValido = 0;

    while (!usuarioValido) {
        printf("Digite o login: ");
        fgets(novoCliente.usuario, MAX_USUARIO, stdin);
        novoCliente.usuario[strcspn(novoCliente.usuario, "\n")] = 0;

        if (usuarioJaExiste(clientes, *numClientes, novoCliente.usuario)) {
            printf("Erro: O usuario '%s' ja esta cadastrado. Tente um nome diferente.\n", novoCliente.usuario);
        } else {
            usuarioValido = 1;
        }
    }

    char senha1[MAX_SENHA], senha2[MAX_SENHA];
    int senhasCorretas = 0;

    while (!senhasCorretas) {
        printf("Digite a senha: ");
        fgets(senha1, MAX_SENHA, stdin);
        senha1[strcspn(senha1, "\n")] = 0;

        if (!validarSenha(senha1)) {
            continue;                  // Pede novamente se a senha não for válida
        }

        printf("Digite a senha novamente para confirmar: ");
        fgets(senha2, MAX_SENHA, stdin);
        senha2[strcspn(senha2, "\n")] = 0;

        if (strcmp(senha1, senha2) == 0) {
            senhasCorretas = 1;
        } else {
            printf("Senha invalida. As senhas nao coincidem.\n");
        }
    }

    criptografarSenha(senha1); 
    strcpy(novoCliente.senha, senha1);
    clientes[*numClientes] = novoCliente;
    (*numClientes)++;
    printf("Login e senha cadastrados com sucesso!\n");

    salvarDados(clientes, *numClientes); 
}

void editarLoginSenha(Cliente clientes[], int numClientes) {
    char nomeUsuario[MAX_USUARIO];
    printf("Digite o nome do usuario que deseja editar: ");
    fgets(nomeUsuario, MAX_USUARIO, stdin);
    nomeUsuario[strcspn(nomeUsuario, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].usuario, nomeUsuario) == 0) {
            encontrado = 1;
            printf("Digite o novo login para o cliente (atual: %s): ", clientes[i].usuario);
            fgets(clientes[i].usuario, MAX_USUARIO, stdin);
            clientes[i].usuario[strcspn(clientes[i].usuario, "\n")] = 0;

            char novaSenha1[MAX_SENHA], novaSenha2[MAX_SENHA];
            int senhasCorretas = 0;

            while (!senhasCorretas) {
                printf("Digite a nova senha para o cliente: ");
                fgets(novaSenha1, MAX_SENHA, stdin);
                novaSenha1[strcspn(novaSenha1, "\n")] = 0;

                if (!validarSenha(novaSenha1)) {
                    continue;                             // vai pedir a senha novamente ao usuário
                }

                printf("Digite a nova senha novamente para confirmar: ");
                fgets(novaSenha2, MAX_SENHA, stdin);
                novaSenha2[strcspn(novaSenha2, "\n")] = 0;

                if (strcmp(novaSenha1, novaSenha2) == 0) {
                    senhasCorretas = 1;
                } else {
                    printf("As senhas nao coincidem. Tente novamente.\n");
                }
            }

            criptografarSenha(novaSenha1); 
            strcpy(clientes[i].senha, novaSenha1);
            printf("Login e senha atualizados com sucesso!\n");
            salvarDados(clientes, numClientes);       
            break;
        }
    }
    if (!encontrado) {
        printf("Usuario nao encontrado.\n");
    }
}

void excluirLoginSenha(Cliente clientes[], int *numClientes) {
	system("color 74");
    char nomeUsuario[MAX_USUARIO];
    printf("Digite o nome do usuario que deseja excluir: ");
    fgets(nomeUsuario, MAX_USUARIO, stdin);
    nomeUsuario[strcspn(nomeUsuario, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < *numClientes; i++) {
        if (strcmp(clientes[i].usuario, nomeUsuario) == 0) {
            char senha[MAX_SENHA];
            printf("Digite a senha para confirmar exclusao: ");
            fgets(senha, MAX_SENHA, stdin);
            senha[strcspn(senha, "\n")] = 0;

            criptografarSenha(senha); 
            if (strcmp(clientes[i].senha, senha) == 0) {
                encontrado = 1;
                for (int j = i; j < *numClientes - 1; j++) {
                    clientes[j] = clientes[j + 1];
                }
                (*numClientes)--; 
                printf("Usuario excluido com sucesso!\n");
                salvarDados(clientes, *numClientes); 
                break;
            } else {
                printf("Senha incorreta. Nao foi possivel excluir o usuario.\n");
                break;
            }
        }
    }
    if (!encontrado) {
        printf("Usuario nao encontrado.\n");
    }
}

void listarClientes(Cliente clientes[], int numClientes) {
	system("color 02");
    printf("Logins cadastrados:\n");
    printf(" ----------------------------------------------\n");
    printf("| ID |    Login    |   Senha (criptografada)   |\n");
    printf("|----|-------------|---------------------------|\n");

    for (int i = 0; i < numClientes; i++) {
        printf("| %2d | %-11s | %-25s |\n", i + 1, clientes[i].usuario, clientes[i].senha);
    }
    printf(" ----------------------------------------------\n");
}


void salvarDados(Cliente clientes[], int numClientes) {
    FILE *arquivo = fopen(ARQUIVO, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados.\n");
        return;
    }
    fwrite(&numClientes, sizeof(int), 1, arquivo);
    fwrite(clientes, sizeof(Cliente), numClientes, arquivo);
    fclose(arquivo);
}

void carregarDados(Cliente clientes[], int *numClientes) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        return; 
    }
    fread(numClientes, sizeof(int), 1, arquivo);
    fread(clientes, sizeof(Cliente), *numClientes, arquivo);
    fclose(arquivo);
}
