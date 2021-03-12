// --------------------------------------------------------------------------------------------- bof
#ifndef TMessageBrokerH
#define TMessageBrokerH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
#include <System.JSON.hpp>

class TRetornoAlocacaoLicenca;
class TRetornoLiberacaoLicenca;
/*
// --------------------------------------------------------------------------------------- namespace
namespace sinapsis
{
    namespace sinap
    {
        namespace gestaoLicenca
        {
			class TMessageBroker;
        }
    }
}

using sinapsis::sinap::gestaoLicenca::TMessageBroker;
 */
// ------------------------------------------------------------------------------------------- class

/**
 * Classe responsavel por interpretar as strings JSON
 */
class TMessageBroker : public TObject
{

    public:

        //
        //Construtor e Destrutor
        //
        __fastcall TMessageBroker(void);
        __fastcall ~TMessageBroker(void);

        //
		// Methods
        //
		TRetornoAlocacaoLicenca* ToRetornoAlocacaoLicenca(TJSONValue *jValue);
		TRetornoLiberacaoLicenca* ToRetornoLiberacaoLicenca(TJSONValue *jValue);

	private: //objetos locais
		TRetornoAlocacaoLicenca *retornoAloca;
		TRetornoLiberacaoLicenca *retornoLibera;

	private: //metodos
		TDate __fastcall ConverteStrToDate(AnsiString data);
};

#endif
// --------------------------------------------------------------------------------------------- eof
