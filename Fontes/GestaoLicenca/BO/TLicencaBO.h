// --------------------------------------------------------------------------------------------- bof
#ifndef TLicencaBOH
#define TLicencaBOH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

#include <VTLicencaBO.h>

class TWebServiceConnection;
class TRetornoAlocacaoLicenca;
class TRetornoLiberacaoLicenca;
class VTGestaoLicenca;
/*
// --------------------------------------------------------------------------------------- namespace
namespace sinapsis
{
	namespace sinap
	{
		namespace gestaoLicenca
		{
			class TLicencaBO;
		}
	}
}

using namespace std;
using sinapsis::sinap::gestaoLicenca::TLicencaBO;
*/
// ------------------------------------------------------------------------------------------- class

/**
 * Encapsula o retorno do WebService de liberacao de licenca.
 */
class TLicencaBO : public VTLicencaBO
{
	public:

		//
		//Construtor e Destrutor
		//
		__fastcall TLicencaBO(VTApl *apl_owner);
		__fastcall ~TLicencaBO(void);

		//
		// Methods
		//
		bool  Alocar(TLicencaRequest* request, UnicodeString &diagnostico)throw(ELicencaConnection);
		void  ConfiguraProxy(AnsiString server="", int port = -1,AnsiString user="",AnsiString password="");
		bool  Liberar(UnicodeString &diagnostico)throw(ELicencaConnection);
        bool  LicencaSinapViewer(UnicodeString login, UnicodeString senha, UnicodeString dominio, UnicodeString &diagnostico);
		TLicencaRequest* MontaRequest(UnicodeString login, UnicodeString senha, UnicodeString dominio);

	private:
		bool  ValidaRetorno(TRetornoAlocacaoLicenca *retorno);
		bool  ValidaRetorno(TRetornoLiberacaoLicenca *retorno);

	private: //objetos externos
		VTApl *apl;

	private: //objetos locais
		TWebServiceConnection *wsConnection;
		VTGestaoLicenca *gestaoLicenca;

};

#endif
// --------------------------------------------------------------------------------------------- eof
