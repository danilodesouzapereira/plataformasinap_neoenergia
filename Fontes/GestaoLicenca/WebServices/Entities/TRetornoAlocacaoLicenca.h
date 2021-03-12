// --------------------------------------------------------------------------------------------- bof
#ifndef TRetornoAlocacaoLicencaH
#define TRetornoAlocacaoLicencaH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

class TLicencaAlocada;
/*
// --------------------------------------------------------------------------------------- namespace
namespace sinapsis
{
    namespace sinap
    {
		namespace gestaoLicenca
        {
            class TRetornoAlocacaoLicenca;
        }
    }
}

using sinapsis::sinap::gestaoLicenca::TRetornoAlocacaoLicenca;
*/
// ------------------------------------------------------------------------------------------- class

/**
 * Encapsula o retorno do WebService de alocacao de licenca.
 */
class TRetornoAlocacaoLicenca : public TObject
{

    public:

        //
        //Construtor e Destrutor
        //
        __fastcall TRetornoAlocacaoLicenca(void);
        __fastcall ~TRetornoAlocacaoLicenca(void);

        //
        // Propriedades
        //
        __property bool Sucesso = {read=GetSucesso, write=SetSucesso};
        __property AnsiString Mensagem = {read=GetMensagem, write=SetMensagem};
		__property TLicencaAlocada *LicencaAlocada = {read=GetLicencaAlocada, write=SetLicencaAlocada};

    private:

        //
        // Getters & Setters
        //
		bool GetSucesso();
        void SetSucesso(bool sucesso);
        AnsiString GetMensagem();
        void SetMensagem(AnsiString mensagem);
		TLicencaAlocada* GetLicencaAlocada();
		void SetLicencaAlocada(TLicencaAlocada* licencaAlocada);
		//
		// Getters & Setters - atributos
		//
		bool sucesso;
		AnsiString mensagem;
		TLicencaAlocada* licencaAlocada;

};

#endif
// --------------------------------------------------------------------------------------------- eof
