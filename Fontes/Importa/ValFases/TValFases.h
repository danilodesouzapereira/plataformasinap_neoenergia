// ---------------------------------------------------------------------------
#ifndef TValFasesH
#define TValFasesH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTValFases.h"

// ---------------------------------------------------------------------------
class VTArranjo;
class VTCabo;
class VTSuporte;
class VTPrimario;
class VTSecundario;
class VTBarra;
class VTCarga;
class VTLigacao;
class VTRegulador;
class VTTrafo;
class VTTrecho;
class VTFases;
class VTLog;

// ---------------------------------------------------------------------------
class TValFases : public VTValFases
{
public:
	__fastcall TValFases(VTApl *apl);
	__fastcall ~TValFases(void);
	bool __fastcall Executa(void);
	int __fastcall ExisteEqptoComFaseInvalida(void);
	TList* __fastcall LisEqptoComFaseInvalida(void);
	bool __fastcall SubstituiTrechosFasesInvalidas(int faseNOVA); // FKM 2017.05.26

protected: // métodos
	VTBarra* __fastcall BarraComum(VTLigacao *pai, VTLigacao *filha);
	void __fastcall CorrigeFasesCarga(VTCarga *carga, int fases_ref);
	void __fastcall CorrigeFasesRegulador(VTTrecho *trecho_pai, VTRegulador *regulador);
	void __fastcall CorrigeFasesTrafo(VTTrecho *trecho_pai, VTTrafo *trafo);
	void __fastcall CorrigeFasesTrafoMonoEmDelta(VTTrecho *trecho_pai, TList *lisTRF);
	void __fastcall CorrigeFasesTrafoMTBT(VTPrimario *primario);
	void __fastcall CorrigeFasesTrafoSecundario(VTSecundario *secundario);
	void __fastcall CorrigeFasesTrechoFilho(VTTrecho *pai, VTTrecho *filho);
	void __fastcall CorrigeFasesTrechoPai(VTTrecho *pai, VTRegulador *regulador);
	void __fastcall CorrigeFasesTrechoPai(VTTrecho *pai, VTTrafo *trafo);
	void __fastcall CorrigeFasesTrechoPai(VTTrecho *pai, VTTrecho *filho);
	void __fastcall CorrigeTrechoFaseINV(VTTrecho *trecho);
	void __fastcall CorrigeTrechoMono(VTTrecho *pai, VTTrecho *trecho);
	void __fastcall CorrigeTrechoBi(VTTrecho *pai, VTTrecho *trecho);
	void __fastcall CorrigeTrechoTri(VTTrecho *pai, VTTrecho *trecho);
	VTTrecho* __fastcall ExisteTrechoPai(VTLigacao *filha, bool aceita_faseINV = true);
	VTLigacao* __fastcall ExistePaiValido(VTLigacao *filha);
	int __fastcall FasesDaLigacao(VTLigacao *ligacao, VTBarra *barra);
	int __fastcall FasesDisponiveis(VTLigacao *ligacao, VTBarra *barraSecTerc = NULL);
	int __fastcall FasesDisponiveis(VTBarra *barra);
	void __fastcall LisTrafoConectaBarra(TList *lisLIG, VTBarra *barra, TList *lisEXT);
	void __fastcall RedefineArranjo(VTTrecho *trecho, int faseNOVA); // FKM 2017.05.26
	void __fastcall RedefineArranjoFilhoIgualArranjoPai(VTTrecho *pai, VTTrecho *filho);
	void __fastcall RedefineArranjoPaiFasesFilho(VTTrecho *pai, int fases_filho);
	void __fastcall RedefineArranjoPaiIgualArranjoFilho(VTTrecho *pai, VTTrecho *filho);
	void __fastcall RedefineArranjoComNeutro(VTTrecho *trecho);
	void __fastcall RedefineArranjoSemNeutro(VTTrecho *trecho);
	bool __fastcall SubstituiTrechosFasesInvalidas(TList *lisLIG, int faseNOVA);
	void __fastcall VerificaFases(VTPrimario * primario);
	void __fastcall VerificaFases(VTSecundario * secundario);
	void __fastcall VerificaFases(void);
	void __fastcall VerificaFasesCarga(VTBarra *barra, int fases_tag);
	bool __fastcall VerificaTrafosMonoEmDelta(TList *lisTRF);

	// objetos externos
	VTApl *apl;
	VTFases *fases;
	VTLog *plog;

	// dados locais
	TList *lisBAR;
	TList *lisEQP;
	TList *lisLIG;
	TList *lisTRF;
};

#endif
// -----------------------------------------------------------------------------
// eof
