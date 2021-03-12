// ---------------------------------------------------------------------------
#ifndef TRelatorioJusanteH
#define TRelatorioJusanteH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRelatorioJusante.h"

// ---------------------------------------------------------------------------
class VTChave;
class VTCaminho;
class VTEqpto;
class VTPatamar;
class TArranjoTrecho;

// ---------------------------------------------------------------------------
class TRelatorioJusante : public VTRelatorioJusante
{

public:
			__fastcall TRelatorioJusante(VTApl *apl);
			__fastcall ~TRelatorioJusante(void);
	void   	__fastcall AgregaValores(TStrings *key_value, VTEqpto *eqptoRef,
										int eqptoTIPO, VTPatamar *patamar = NULL, int tipo_relatorio = 0);
	//nova interface
	VTChave* __fastcall ChavePai(VTBloco *bloco);
	void __fastcall DestacaEquipamentosJusante(void);
	void __fastcall DestacaEquipamentosMontante(void);
	void __fastcall ListaBlocosJusante(VTBloco *bloco, TList *lisEXT);
	void __fastcall ListaEqptosJusante(TList *lisEXT, VTEqpto *eqptoRef,
																int eqptoTIPO);
	void __fastcall ListaEqptosMontante(TList *lisEXT, VTEqpto *eqptoRef,
																int eqptoTIPO);
	void __fastcall PreparaExecucao(void);
	void   __fastcall SelecionaPatamar(VTPatamar *patamar);

	public:  //property
	  __property TColor CorDestaque = {read=colorDestaque, write=colorDestaque};

private:
	//void __fastcall AdicionaListaLigacoesJusanteSecundario(VTSecundario *secundario);
	void __fastcall AgregaCarga(VTEqpto *eqptoRef, strpi_CARGA &str);
	void __fastcall AgregaCargaValores(TStrings *key_value, VTEqpto *eqptoRef);
	void __fastcall AgregaConsumidores(VTEqpto *eqptoRef, strpi_CONSUMIDOR &str);
	void __fastcall AgregaConsumidoresValores(TStrings *key_value, VTEqpto *eqptoRef);
	void __fastcall AgregaResumo(VTEqpto *eqptoRef, strpi_RESUMO &str);
	void __fastcall AgregaResumoValores(TStrings *key_value, VTEqpto *eqptoRef);
	void __fastcall AgregaTrafo(VTEqpto *eqptoRef, strpi_TRAFO &str);
	void __fastcall AgregaTrafoValores(TStrings *key_value, VTEqpto *eqptoRef);
	TList* __fastcall AgregaTrecho(VTEqpto *eqptoRef);
	void   __fastcall AgregaTrechoValores(TStrings *key_value, VTEqpto *eqptoRef);
	VTBarra* __fastcall BarraReferencia(VTEqpto *eqptoRef);
	void __fastcall DestacaChavesEntreBlocosJusante(VTBloco *bloco);
	void __fastcall DestroiLista(TList *lista);
	TArranjoTrecho* __fastcall ExisteArranjoTrecho(TList *lisArranjoTrecho, VTTrecho *trecho);
	VTNode* __fastcall ExisteNodeBloco(VTBloco *bloco);
	VTNode* __fastcall ExisteNodeEqpto(VTEqpto *eqpto);
    void __fastcall InsereGD(void);
	VTLigacao* __fastcall LigacaoReferencia(VTEqpto *eqptoRef);
	void __fastcall ListaBlocosJusante(VTNode *node, TList *lisEXT);
	void __fastcall ListaBlocosJusante(TList *lisELOConsiderados, TList *lisEXT , VTBloco *bloco_ref);
	void __fastcall ListaEqbarExistentes(TList *lisBAR, TList *lisEXT, int eqptoTIPO);
	void __fastcall ListaLigacaoExistentes(TList *lisLIG, TList *lisEXT, int eqptoTIPO);
	void __fastcall ListaNodesJusante(VTNode *node, TList *lisEXT);
	int __fastcall IniciaArvoreBlocos(void);
	void __fastcall OrdenaLigBloco(VTBloco *bloco,  TList* lisEXT_BAR, TList *lisEXT_LIG);
	void __fastcall Ordena(VTBarra *barra_ini,TList *lisLIG, TList *lisEXT_BAR, TList *lisEXT_LIG);
	void __fastcall RemoveEqptosNaoJusante(VTEqpto *eqptoRef, TList *lisEXT_BAR, TList *lisEXT_LIG);
    void   __fastcall RemoveGD(void);
	void   __fastcall SelecionaEqpto(VTEqpto *eqpto);
	void   __fastcall TipoEquipamento(int eqptoTIPO);


private: //dados externos
	VTEqpto *eqpto;
	VTPatamar *patamar;
	VTApl *apl;
	VTCaminho *caminho;


private:
	bool    blocos_executado;
	bool    isBarra, isEqbar, isLigacao;
    bool    existe_gd;
	int     index_tipo_rel;
	VTArvore *arvore_lig;
	VTArvore *arvore_bar;
	TList *lisBarra;
	TList *lisLigacao;
	TList *lisPrimario;
	TList *lisLigacaoJusante;
	TList *lisDestaqueJusante;
	TList *lisLigacaoMontante;
	TList *lisDestaqueMontante;

};

#endif
// ---------------------------------------------------------------------------
// eof
