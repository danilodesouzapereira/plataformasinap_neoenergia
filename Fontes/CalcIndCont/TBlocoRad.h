// ---------------------------------------------------------------------------

#ifndef TBlocoRadH
#define TBlocoRadH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBlocoRad.h"

// ---------------------------------------------------------------------------
class TBlocoRad : public VTBlocoRad
{
public:
	__fastcall TBlocoRad(VTBloco *bloco, VTApl *apl);
	__fastcall ~TBlocoRad(void);
	void __fastcall IniciaCarga(int numPat);
	void __fastcall LisBlRadJus(TList *lisEXT);

	// métodos

		private :
	VTRede* __fastcall RedeBloco(VTBloco *bloco); // hkv201610: definição de rede de bloco

protected: // métodos acessados via property

	VTBloco* __fastcall PM_GetBloco(void)
	{
		return (bloco);
	};

	VTBlocoRad* __fastcall PM_GetBlocoRadMon(void)
	{
		return (blocoRadMon);
	};

	void __fastcall PM_SetBlocoRadMon(VTBlocoRad* blocoRadMon)
	{
		this->blocoRadMon = blocoRadMon;
	};

	VTChave* __fastcall PM_GetChave(void)
	{
		return (pchave);
	};

	void __fastcall PM_SetChave(VTChave* pchave)
	{
		this->pchave = pchave;
	};

	VTBarra* __fastcall PM_GetBarraChave(void)
	{
		return (barraChave);
	};

	void __fastcall PM_SetBarraChave(VTBarra* barraChave)
	{
		this->barraChave = barraChave; //hk201802a
	};

	VTLisBlocoChave* __fastcall PM_GetLisBlocoChave(void)
	{
		return (plisBlocoChave);
	}; // hk201611

	void __fastcall PM_SetLisBlocoChave(VTLisBlocoChave* plisBlocoChave)
	{
		this->plisBlocoChave = plisBlocoChave;
	}; // hk201611

	TList* __fastcall PM_GetLisBlocoRad(void)
	{
		return (lisBlocoRad);
	};

	TList* __fastcall PM_GetLisBlocoRadJus(void)
	{
		return (lisBlocoRadJus);
	};

	TList* __fastcall PM_GetLisBlocoJus(void)
	{
		return (lisBlocoJus);
	};

	TList* __fastcall PM_GetLisBlocoJusProt(void)
	{
		return (lisBlocoJusProt);
	};

	TList* __fastcall PM_GetLisBlocoMon(void)
	{
		return (lisBlocoMon);
	};

	int __fastcall PM_GetNumPat(void)
	{
		return (numPat);
	};
	double __fastcall PM_GetCargaBlRad(int iPat);
	void __fastcall PM_SetCargaBlRad(int iPat, double valor);
	double __fastcall PM_GetCargaBloco(int iPat);
	void __fastcall PM_SetCargaBloco(int iPat, double valor);

	int __fastcall PM_GetNCons(void) {return (ncons);};
	void __fastcall PM_SetNCons(int ncons) {this->ncons = ncons;};
	//hk201902
	int __fastcall PM_GetNConsJus(void) {return (nconsJus);};
	void __fastcall PM_SetNConsJus(int nconsJus) {this->nconsJus = nconsJus;};

	bool __fastcall PM_GetFlagSup(void)
	{
		return (flagSup);
	};

	void __fastcall PM_SetFlagSup(bool flagSup)
	{
		this->flagSup = flagSup;
	};
	VTRede* __fastcall PM_GetRede(void);

	void __fastcall PM_SetRede(VTRede *rede)
	{
		this->rede = rede;
	};

private: // dados externos

		private : // dados locais

	VTApl *apl; // hkv201610: definição de rede de bloco: inclusao de apl
	VTBloco *bloco; // bloco
	VTRede *rede;
	// hkv201610: rede de blocoRad  //hkv201610: definição de rede de bloco : VTRede     *rede
	VTBlocoRad *blocoRadMon; // blocoRad a montante
	VTChave *pchave; // chave a montante
   VTBarra *barraChave; //chave de referencia para determinação do parametro associado a bloco
	VTLisBlocoChave *plisBlocoChave; // hk201611: lisBlocoChave a montante correspondente a pchave
	TList *lisBlocoRad; // lista de blocoRads a jusante

	TList *lisBlocoRadJus; // lista de blocosRad a jusante
	TList *lisBlocoJus; // lista de blocos a jusante
	TList *lisBlocoJusProt; // lista de blocos a jusante até as chaves de protecao
	TList *lisBlocoMon; // lista de blocos a montante

	double *cargaBlRad; // Carga do bloco e dos blocos a jusante por patamar
	double *cargaBloco; // Carga do bloco this->bloco
	int numPat; // número de patamares de cargaBlRad
	int ncons; // número de consumidores do bloco
	int nconsJus; // número de consumidores do bloco e a jusante hk201902
	bool flagSup; // se aponta para bloco ligado a ponto de suprimento

public:
};

// ---------------------------------------------------------------------------
#endif
