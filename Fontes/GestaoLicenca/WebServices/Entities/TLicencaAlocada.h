// --------------------------------------------------------------------------------------------- bof
#ifndef TLicencaAlocadaH
#define TLicencaAlocadaH

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
			class TLicencaAlocada;
        }
	}
}

using sinapsis::sinap::gestaoLicenca::TLicencaAlocada;
 */
// ------------------------------------------------------------------------------------------- class

/**
 * Representa a licenca que foi alocada e retornada pelo WebService de alocacao de licenca.
 */
class TLicencaAlocada : public TObject
{

public:

    //
    //Construtor e Destrutor
    //
    __fastcall TLicencaAlocada(void);
	__fastcall ~TLicencaAlocada(void);

    //
    // Propriedades
    //
	__property TDate DataAlocada = {read=GetDataAlocada, write=SetDataAlocada};
    __property TDate DataTermino = {read=GetDataTermino, write=SetDataTermino};
    __property int Validade = {read=GetValidade, write=SetValidade};
	__property int Tipo = {read=GetTipo, write=SetTipo};
	__property DynamicArray<int> Operacoes = {read=GetOperacoes, write=SetOperacoes};

private:

    //
    // Getters & Setters
    //
    TDate GetDataAlocada();
    void SetDataAlocada(TDate dataAlocada);
    TDate GetDataTermino();
    void SetDataTermino(TDate dataTermino);
    int GetValidade();
    void SetValidade(int validade);
	int GetTipo();
    void SetTipo(int tipo);
	DynamicArray<int> GetOperacoes();
	void SetOperacoes(DynamicArray<int> operacoes);

    //
    // Getters & Setters - atributos
    //
    TDate dataAlocada;
    TDate dataTermino;
    int validade;
    int tipo;
	DynamicArray<int> operacoes;

};

#endif
// --------------------------------------------------------------------------------------------- eof
