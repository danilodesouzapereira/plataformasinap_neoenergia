// --------------------------------------------------------------------------------------------- bof
#ifndef TImportaArquivoCurvaTipicaH
#define TImportaArquivoCurvaTipicaH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
#include<stdexcept>

class VTApl;
class TAplCfg;
class VTCurva;
class VTCurvas;
class VTClasse;
/*
// --------------------------------------------------------------------------------------- namespace
namespace sinapsis
{
    namespace sinap
    {
        namespace configuracao
        {
            class TImportaArquivoCurvaTipica;
        }

    }
}

using sinapsis::sinap::configuracao::TImportaArquivoCurvaTipica;
*/
using namespace std;
// ------------------------------------------------------------------------------------------- class

/**
 * Importa arquivos com Curvas Tipicas de Demanda (Consumidores e Iluminação pública).
 * @see TObject
 */
class TImportaArquivoCurvaTipica : public TObject
{

public:

    /** Construtor
     *@param apl um ponteiro para um objeto do tipo TAplCfg.
     *@see TAplCfg
     */
    __fastcall TImportaArquivoCurvaTipica(TAplCfg *apl);

    /**
     * Destrutor
     */
    __fastcall ~TImportaArquivoCurvaTipica(void);

    /**
     * Importa o arquivo.
     * @param fileName O path completo até o arquivo. Ex.: D:\\arquivo.csv
     * @return a quantidade de curvas importadas
     * @throws runtime_error lança uma exceção caso tenha ocorrido algum problema durante a
     * importação das curvas
     * @see <a href="http://en.cppreference.com/w/cpp/error/runtime_error">runtime_error</a>
	 */
	int __fastcall Importar(AnsiString filePath);

private:

	TAplCfg *apl;
    VTCurvas *curvas;

	/**
	* Corrige a linha do arquivo .csv que pode ter sido alterada de forma errada pelo Excel.<br>
	* Este método faz as seguintes correções:<br>
	* 1) remove os 3 ';' do final da linha(quando estes estiverem presentes)<br>
	* 2) remove o ';' do final da linha(quando presente)<br>
	* 3) substituí "" por ", comum nas linhas com JSON<br>
	* 4) remove '"' do final e começo das linhas com JSON <br>
	*/
	AnsiString __fastcall CorrigeLinha(AnsiString linha);


	/**
	 * Cria um objeto do tipo <code>TCurva</code> já populado com as informações do arquivo.
	 * @param tipoCurva um valor do enum (<code>tipoCURVA</code>), atualmente o sistema só importa
     * as curvas: curvaPFP_MDP e curvaLD.
     * @param linha a linha do arquivo que contém as informações da curva(<code>TCurva</code>) a ser
     * criada
     * @return um ponteiro para uma implementação de <code>VTCurva</code>
     * @see VTCurva
     * @see TCurva
     */
	VTCurva* __fastcall CriarCurva(tipoCURVA tipoCurva, AnsiString linha);

	/**
	 * Verifica se o header do csv esta correto.
	 * @param tipoCurva um valor do enum (<code>tipoCURVA</code>), atualmente o sistema só importa
     * as curvas: curvaPFP_MDP e curvaLD.
     * @param linha a linha do arquivo que contém o header do csv da curva. Exemplo:<br>
     * <code>INICIO;FIM;POTENCIA_ATIVA;FATOR_POTENCIA</code>
     * @return um bool se o header estiver correto
     * @see tipoCURVA
     */
    bool __fastcall IsHeaderValido(tipoCURVA tipoCurva, AnsiString linha);

    /**
     * Verifica se a linha é de medição(valor da curva).
     * Uma linha é considerada de medição quando:<br>
     * 1. não começa por: '//' (comentário) ou  '{' (linha de definição de curva)<br>
     * 2. começa com um número [0..9]<br>
     * 3. não está em branco <br>
     *<br>
     * Para esta validação será verificado se existe uma string que encaixe na expressão regular:
     * <code>^\d{2}:\d{2}</code>
     * @param linha a linha do arquivo que contém uma medição. Exemplo:<br>
     * <code>00:00;01:00;129.452;0.920</code>
     * @return um <code>bool</code> se a linha conter informações de medição
     */
    bool __fastcall IsLinhaMedicao(AnsiString linha);


    /**
     * Processa a linha de medição e adiciona um ponto da curva.
     * @param tipoCurva um valor do enum (<code>tipoCURVA</code>), atualmente o sistema só importa
     * as curvas: curvaPFP_MDP e curvaLD.
     * @param curva a curva(<code>VTCurva</code>) que o ponto será inserido
     * @param linha a linha do arquivo que contém uma medição. Exemplo:<br>
     * <code>00:00;01:00;129.452;0.920</code>
     * @see tipoCURVA
     */
    void __fastcall AdicionarPonto(tipoCURVA tipoCurva, VTCurva* curva, AnsiString linha);

	/**
	 * Exclui todas as curvas típicas(Demanda de consumidor e Iluminação pública) da base.
	 */
	void __fastcall ExcluirCurvasTipicas();

	/**
	 * Executa importação do arquivo de curvas
	 */
	int __fastcall Executa(AnsiString filePath);

	/**
	 * Valida arquivo inteiro
	 */
	bool __fastcall ValidaArquivo(AnsiString filePath, AnsiString &msg);

};

#endif
// --------------------------------------------------------------------------------------------- eof
