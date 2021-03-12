// --------------------------------------------------------------------------------------------- bof
#ifndef TExportaArquivoCurvaTipicaModeloH
#define TExportaArquivoCurvaTipicaModeloH

// ----------------------------------------------------------------------------------------- include
#include <Classes.hpp>
#include<stdexcept>

class VTApl;
class VTCurva;
class VTCurvas;
/*
// --------------------------------------------------------------------------------------- namespace
namespace sinapsis
{
   namespace sinap
   {
      namespace configuracao
      {
         class TExportaArquivoCurvaTipicaModelo;
      }

   }
}

using namespace std;
using sinapsis::sinap::configuracao::TExportaArquivoCurvaTipicaModelo;
 */
// ------------------------------------------------------------------------------------------- class
/**
 * Responsavel por exportar o arquivo modelo contendo as Curvas Tipica de Demanda (Consumidores e
 * Iluminação pública).
 * @see TObject
 */
class TExportaArquivoCurvaTipicaModelo : public TObject
{

public:

   /**
    * Construtor
    *@param apl um objeto descendente de  VTApl.
    *@see VTApl
    */
   __fastcall TExportaArquivoCurvaTipicaModelo(TAplCfg *apl);

   /**
    * Destrutor
    */
   __fastcall ~TExportaArquivoCurvaTipicaModelo(void);

   /**
    * Exporta o arquivo.
	* @param fileName O path completo até o arquivo. Ex.: D:\arquivo.csv
	* @return retorna <code>true</code> se o arquivo foi gerado com sucesso ou lança uma exceção
	* @throws runtime_error lança uma exceção caso tenha ocorrido algum problema durante a
	* exportação das curvas
	* @see AnsiString
	* @see <a href="http://en.cppreference.com/w/cpp/error/runtime_error">runtime_error</a>
    */
   bool __fastcall Exportar(AnsiString fileName) throw(runtime_error);

private:
   TAplCfg *apl;

   /**
	* Retorna o texto fixo de instrucao para preenchimento da secao: <code>[Curvas Tipica de Demanda
	* dos Consumidores]</code>.
    * @return as instrucoes para preenchimento e a tag que indica o inicio da secao.
    */
   AnsiString __fastcall GetTextoFixoCurvaConsumidor();

   /**
    * Retorna as curvas da secao <code>[Curvas Tipica de Demanda dos Consumidores]</code>.
    *
    * @return as curvas jah formatadas conforme o padrao do arquivo, exemplo:<br>
    *<code>{"TIPO_CURVA": 1, "NOME":"Residencial", "CLASSE":1, "MEDICAO":1, "PONTOS":24,
    * "FAIXA_CONSUMO_INFERIOR":0.0, "FAIXA_CONSUMO_SUPERIOR":0.0}<br>
    *INICIO;FIM;POTENCIA_ATIVA;FATOR_POTENCIA <br>
    *00:00;01:00;129.452;0.920<br>
    *...<br>
    *23:00;24:00;114.364;0.920</code>
    *
    */
   AnsiString __fastcall GetCurvaConsumidor(TList *curvaConsumidorList);

   /**
    * Retorna o texto fixo de instrucao para preenchimento da secao: <code>[Curvas de Iluminacao
    * Publica]</code>.
    * @return as instrucoes para preenchimento e a tag que indica o inicio da secao.
    */
   AnsiString __fastcall GetTextoFixoCurvaIluminacaoPublica();

   /**
    *  Retorna as curvas da secao <code>[Curvas de Iluminacao Publica]</code>.
    *
    * @return as curvas jah formatadas conforme o padrao do arquivo, exemplo:</br>
    * <code>{"TIPO_CURVA": 2, "NOME":"IP", "PONTOS":24}
    * INICIO;FIM;LIGADO
    * 00:00;01:00;1.000
    * ...
    * 23:00;24:00;1.000
    */
   AnsiString __fastcall GetCurvaIluminacaoPublica(TList *curvaIluminacaoPublicaList);

};

#endif
// --------------------------------------------------------------------------------------------- eof
