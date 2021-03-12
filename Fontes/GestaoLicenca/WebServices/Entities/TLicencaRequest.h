// --------------------------------------------------------------------------------------------- bof
#ifndef TLicencaRequestH
#define TLicencaRequestH

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
            class TLicencaRequest;
        }
    }
}

using sinapsis::sinap::gestaoLicenca::TLicencaRequest;
*/
// ------------------------------------------------------------------------------------------- class

/**
 * Encapsula a requisicao que serah feita ao WebService de alocacao de licenca.
 */
class TLicencaRequest : public TObject
{

public:

    //
    //Construtor e Destrutor
    //
    __fastcall TLicencaRequest(void);
    __fastcall ~TLicencaRequest(void);

    //
    // Propriedades
    //
    __property int Dominio = {read=GetDominio, write=SetDominio};
    __property AnsiString Login = {read=GetLogin, write=SetLogin};
    __property AnsiString Senha = {read=GetSenha, write=SetSenha};
    __property AnsiString Produto = {read=GetProduto, write=SetProduto};
    __property AnsiString CPUid = {read=GetCPUid, write=SetCPUid};


	//
	// Metodos
	//
	AnsiString ToJSON();

private:

	//
    // Getters & Setters
    //
    int GetDominio();
    void SetDominio(int dominio);
    AnsiString GetLogin();
    void SetLogin(AnsiString login);
    AnsiString GetSenha();
    void SetSenha(AnsiString senha);
    AnsiString GetProduto();
    void SetProduto(AnsiString produto);
    AnsiString GetCPUid();
    void SetCPUid(AnsiString cpuId);

    //
    // Getters & Setters - atributos
    //
    int dominio;
    AnsiString login;
    AnsiString senha;
    AnsiString produto;
    AnsiString cpuId;

};

#endif
// --------------------------------------------------------------------------------------------- eof
