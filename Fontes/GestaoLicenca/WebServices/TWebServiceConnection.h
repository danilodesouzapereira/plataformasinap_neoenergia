// --------------------------------------------------------------------------------------------- bof
#ifndef TWebServiceConnectionH
#define TWebServiceConnectionH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

#include <REST.Client.hpp>
class VTApl;
class TMessageBroker;
class TLicencaRequest;
class TRetornoAlocacaoLicenca;
class TRetornoLiberacaoLicenca;
class ELicencaConnection;
/*
// --------------------------------------------------------------------------------------- namespace
namespace sinapsis
{
    namespace sinap
    {
		namespace gestaoLicenca
        {
            class TWebServiceConnection;
        }
    }
}

using sinapsis::sinap::gestaoLicenca::TWebServiceConnection;
*/
// ------------------------------------------------------------------------------------------- class


/**
 * Encapsula o retorno do WebService de liberacao de licenca.
 */
class TWebServiceConnection : public TObject
{

    public:

        //
        //Construtor e Destrutor
        //
		__fastcall TWebServiceConnection(VTApl *apl);
        __fastcall ~TWebServiceConnection(void);

        //
        // Methods
		//
		TRetornoAlocacaoLicenca* Alocar(TLicencaRequest* request)throw(ELicencaConnection);
		TRetornoLiberacaoLicenca* Liberar(TLicencaRequest* request)throw(ELicencaConnection);
		AnsiString Diagnostico(void);
		void ConfiguraProxy(AnsiString server="", int port = -1,AnsiString user="",AnsiString password="");

	private: //metodos
		void ConfiguraREST(void);

		void CriaProxyCFGVazio(void);

	private: //objetos locais
		TMessageBroker *message;
		TRESTRequest *RESTRequest;
		TRESTResponse *RESTResponse;
		TRESTClient *RESTClient;
		AnsiString diagnostico;
		VTApl *apl;

		#define FILE_CFG_PROXY "ProxyCFG.txt"

};

#endif
// --------------------------------------------------------------------------------------------- eof
