#include <string>

using namespace std;

#include "siga.h"

Siga::Siga(string arquivo_dados_estudante)
{
    this->arquivo_nome = arquivo_dados_estudante;

    // TODO: Implementar abertura de arquivo em modo binário, tanto para leitura quanto para escrita
    // se arquivo ja existir, o conteudo pre existente não deve ser apagado.
    // this->file_stream = ...

    //abre o arquivo para leitura e escrita binária:
    this->file_stream.open(this->arquivo_nome, ios::in | ios::out | ios::binary); 
    
    // A função deve imprimir:
    //   SIGA: Inicializado com sucesso
    // caso o arquivo seja aberto corretamente ou caso contrário, 
    //   SIGA: Erro ao abrir arquivo
    if(this->file_stream.is_open())
    {
        cout << "SIGA: Inicializado com sucesso" << endl;
    }
    else
    {
        cout << "SIGA: Erro ao abrir arquivo" << endl;
        return; 
    }

    // TODO: Obter numero de registros de estudante no arquivo
    // 
    // Lembre-se que o arquivo de dados de estudante é binário e 
    // que já pode conter registros de estudantes.
    // this->n_estudantes = 

    //inicializa ponteiro para Estudante:
    Estudante *est = new Estudante();
    //posiciona o cursor no final do arquivo:
    this->file_stream.seekg(0, ios::end);
    //verifica se o arquivo está vazio:
    if(this->file_stream.tellg() == 0){
        //arquivo está vazio
        this->n_estudantes = 0;
    }
    else{
        //arquivo não está vazio
        int tam_arquivo = this->file_stream.tellg();
        this->n_estudantes = tam_arquivo / ESTUDANTE_SIZE;
    }
    //libera o ponteiro:
    delete est;

    cout << this->n_estudantes << " registros de estudantes" << endl;
}


int Siga::PesquisaPorMatricula(int matricula)
{
    // TODO: implementar pesquisa por matrícula
    // Posicione o cursor para o inicio do arquivo:
    // Para i = 0 até n_estudante
    //    Ler estudante na posição corrente no arquivo
    //    Testar se é a matricula procurada, se afirmativo
    //    retorne a posiçao i.
    // Fim-Para
    // Coloque o cursor para o final do arquivo
    // retorne -1
    
    //inicializa ponteiro para Estudante:
    Estudante *est = new Estudante();
    //posiciona o cursor no inicio do arquivo:
    this->file_stream.seekg(0, ios::beg);
    //percorre o arquivo:
    for(int i=0; i<this->n_estudantes; i++){
        //posiciona o cursor:
        this->file_stream.seekg(i*ESTUDANTE_SIZE);
        //lê no arquivo e armazena a leitura na variável est:
        est->LerDoArquivoBinario(this->file_stream); 
        //testa se é a matricula procurada: 
        if(est->ObterMatricula() == matricula){
            //libera o ponteiro:
            delete est;
            return i;
        }
    }
    //posiciona o cursor no final do arquivo:
    this->file_stream.seekg(0, ios::end);
    this->file_stream.seekp(0, ios::end);
    //libera o ponteiro:
    delete est;

    return -1;
}
        
void Siga::CadastraEstudante(Estudante est)
{
    // TODO: Implementar cadastro de estudante
    // Passos:
    // Testar se est já foi cadastrado
    // Se já cadastrado, saia   
    // Caso Contrário, escreva o estudante no final do arquivo 
    // e incremente o numero de estudantes

    //verifica se o estudante já foi cadastrado, pesquisando por sua matricula no arquivo:
    int n = this->PesquisaPorMatricula(est.ObterMatricula());
    if(n != -1){
        //estudante cadastrado
        cout << "Estudante já cadastrado!" << endl;
        return;
    }
    else{
        //estudante sem cadastro 
        //o cursor já está no final do arquivo, pois a função PesquisaPorMatricula foi chamada e 
        //ela já faz isso qdo o estudante não é encontrado, que é o caso
        //escreve o estudante no final do arquivo:

        if(this->file_stream.eof()){
            cout << "final do arquivo" << endl;
        }

        est.EscreverNoArquivoBinario(this->file_stream);
        //incrementa o número de estudantes:
        this->n_estudantes++;
    }
}
        
void Siga::ImprimeEstudantePorMatricula(int matricula)
{
    // TODO: implementar impressão de estudante por matricula.
    // Pesquisa aluno por matricula
    // Se o aluno estiver cadastrado, imprima o aluno.
    // caso contrario: Estudante não cadastrado

    //pesquisa a matrícula informada:
    int n = this->PesquisaPorMatricula(matricula);
    //verifica se o estudante já está cadastrado:
    if(n != -1){
        //estudante cadastrado
        //posiciona o cursor no estudante desejado:
        this->file_stream.seekg(n*ESTUDANTE_SIZE);
        this->file_stream.seekp(n*ESTUDANTE_SIZE);
        //inicializa ponteiro para Estudante:
        Estudante *est = new Estudante();
        //lê os dados do estudante a partir do arquivo e armazena em est:
        est->LerDoArquivoBinario(this->file_stream);
        //imprime os dados do estudante:
        est->Imprimir();
        //libera o ponteiro:
        delete est;
    }
    else{
        //estudante sem cadastro
        cout << "Estudante não cadastrado" << endl;
    }

}
        
void Siga::SalvaListaEstudanteEmTexto(string arquivo_txt)
{
    // TODO: implementar salvar lista de estudantes em texto
    // A saida deve ser a seguinte formatação:
    // Primeira linha conter o cabeçalho:
    // Matricula;Nome;Ano de Ingresso;IRA
    // Demais linhas com as informações dos estudantes.
    // nao use espacos entre os campos!!

    //cria e abre o arquivo de texto para escrita:
    ofstream arq;
    arq.open(arquivo_txt, ios::out);
    //imprime cabeçalho:
    arq << "Matricula;Nome;Ano de Ingresso;IRA" << endl;
    //inicializa ponteiro para Estudante:
    Estudante *est = new Estudante();
    //posiciona o cursor no inicio do arquivo:
    this->file_stream.seekg(0);
    while(!this->file_stream.eof()){
        //lê os dados do estudante e armazena na variavel est:
        est->LerDoArquivoBinario(this->file_stream);
        if(this->file_stream.eof()){
            break;
        }
        //imprime os dados do estudante:
        arq << est->ObterMatricula() << ";";
        arq << est->ObterNome() << ";";
        arq << est->ObterAnoIngresso() << ";";
        arq << est->ObterIRA() << endl;
    }
    //fecha o arquivo de texto:
    arq.close();
    //libera o ponteiro:
    delete est;
}
        
        
void Siga::AlteraCadastroEstudante(Estudante est)
{
    // TODO: implementar alteração de cadastro de estudante
    // Pesquisa se o aluno com est->matricula está cadastrado
    // Se o aluno estiver cadastrado, reescreva a informação do aluno e 
    // escreva  "Dados Alterados"
    // Caso contrário, escreva: "Estudante não cadastrado"

    //verifica se o estudante já foi cadastrado, pesquisando por sua matricula no arquivo:
    int n = this->PesquisaPorMatricula(est.ObterMatricula());
    if(n != -1){
        //estudante cadastrado
        //posiciona o cursor no estudante desejado:
        this->file_stream.seekp(n*ESTUDANTE_SIZE);
        //reescreve as informações do aluno no arquivo:
        est.EscreverNoArquivoBinario(this->file_stream);
        cout << "Dados Alterados" << endl;
    }
    else{
        //estudante sem cadastro
        cout << "Estudante não cadastrado" << endl;
    }
}
        
Siga::~Siga()
{
    //TODO: fecha arquivo

    this->file_stream.close();
}

int Siga::ObterNumeroEstudantesArmazenados()
{
    return this->n_estudantes;
}
