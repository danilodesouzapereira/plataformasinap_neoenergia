// --------------------------------------------------------------------------------------------- bof
#ifndef TRetornoLiberacaoLicencaH
#define TRetornoLiberacaoLicencaH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
/*
// --------------------------------------------------------------------------------------- namespace
namespace sinapsis
{
    namespace sinap
    {
		namespace gestaoLicenca
        {
            class TRetornoLiberacaoLicenca;
        }
	}
}

using namespace std;
using sinapsis::sinap::gestaoLicenca::TRetornoLiberacaoLicenca;
*/
// ------------------------------------------------------------------------------------------- class

/**
 * Encapsula o retorno do WebService de liberacao de licenca.
 */
class TRetornoLiberacaoLicenca : public TObject
{

    public:

        //
        //Construtor e Destrutor
        //
        __fastcall TRetornoLiberacaoLicenca(void);
        __fastcall ~TRetornoLiberacaoLicenca(void);

        //
        // Propriedades
        //
        __property bool Sucesso = {read=GetSucesso, write=SetSucesso};
        __property AnsiString Mensagem = {read=GetMensagem, write=SetMensagem};

    private:

        //
        // Getters & Setters
        //
        bool GetSucesso();
        void SetSucesso(bool sucesso);
        AnsiString GetMensagem();
        void SetMensagem(AnsiString mensagem);

        //
		// Getters & Setters - atributos
		//
		bool sucesso;
		AnsiString mensagem;

};

#endif
// --------------------------------------------------------------------------------------------- eof
