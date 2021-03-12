//---------------------------------------------------------------------------
#ifndef TValFasesMalhaH
#define TValFasesMalhaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "TValFases.h"

//---------------------------------------------------------------------------
class VTArvore;
class VTEqpto;
class VTEqbar;
class VTMalha;
class VTMalhas;
class VTNode;
class VTRede;
class VTRegulador;
//---------------------------------------------------------------------------
class TValFasesMalha : public TValFases
{
	public:
				__fastcall  TValFasesMalha(VTApl *apl);
				__fastcall ~TValFasesMalha(void);
		bool    __fastcall  Executa(void);


	private: //métodos
		void		__fastcall AlteraFasesRegulador(VTRegulador *reg, int fasesNovas, VTBarra *ref);
		void 		__fastcall AnalisaMalha(VTLigacao *ligacaoPai, VTMalha *malha, TList *lisAnalisarPai);
		VTBarra*  	__fastcall BarraComum(VTNode *nodePai, VTNode *nodeFilho);
		VTBarra*  	__fastcall BarraMenorIndexOrdenado(TList *lisBarras);
		VTBarra*  	__fastcall BarraPai(VTLigacao *ligacao);
		VTBarra*  	__fastcall BarraPai(VTNode *node);
		void 		__fastcall ConfiguraLog(void);
		void		__fastcall CorrigeFasesEqpto(VTEqpto *eqpto, int fasesDisponiveis, VTBarra *ref);
		VTMalha*  	__fastcall ExisteMalha(VTLigacao *ligacao);
		int    		__fastcall FasesDisponibilizadas(VTLigacao *ligacao, VTBarra* filha);
		int       	__fastcall FasesDisponiveis(VTBarra* barraOrd);
		int       	__fastcall FasesDisponiveis(VTNode *nodeLig, VTBarra *filha);
		int    		__fastcall FasesNecessarias(VTLigacao *ligacao);
		int    		__fastcall FasesNecessarias(VTNode *nodeLig);
		int			__fastcall GetFaseDisponivel(int fasesDisponiveis, int numFasesDesejada);
        int			__fastcall GetFaseFaltante(int fasesDisponiveis, int fasesNecessaria);
		void 		__fastcall LisBarraLigacoes(TList *lisLig, TList *lisEXT_BAR);
		void 		__fastcall LisEqbarPassivos(TList *lisEXT);
		void      	__fastcall LogaEqpto(VTEqpto *eqpto, AnsiString msg);
		void      	__fastcall MarcaFasesDisponiveis(VTBarra* barraOrdenada);
		AnsiString	__fastcall MsgFasesAlteradas(int fasesOrig, int fasesAlt);
		AnsiString	__fastcall MsgFasesIncoerentes(int fasesDisponiveis, int fasesNecessarias);
		AnsiString	__fastcall MsgFasesTrafoAlteradas(int fasesOrig, int fasesAlt, int enrol);
		VTBarra*  	__fastcall ObtemBarraInicial(VTRede *rede);
		bool 		__fastcall OrdenaLigacoes(TList *lisBARRAINI);
		bool		__fastcall PreparaOrdenacao(void);
		bool		__fastcall PreparaOrdenacao(VTRede *rede, TList *lisMalhas);
		void 		__fastcall ProcuraFilhas(VTLigacao *pai, TList *lisLIG, TList *lisEXT);
		bool 		__fastcall RotacionaCarga(VTCarga *carga, int faseNova);
		bool 		__fastcall RotacionaEqpto(VTEqpto *eqpto, int fasesDisponiveis, VTBarra *ref);
		bool 		__fastcall RotacionaTrafo(VTTrafo *trafo, int faseNova,	VTBarra *ref);
		void		__fastcall VerificaCargas(VTCarga *carga);
		void      	__fastcall VerificaLigacao(VTLigacao *ligacao);

	private:
		VTArvore *arvore;
		VTMalhas *malhas;
		TList *lisMalhasRede;
		TList *lisLigacoesRede;
		TList *lisBarrasRede;
		TList *lisBARRAORD, *lisLIGACAOORD;
		TList *lisMALHAFASES;
		enum tipoTAG
		{
			barraFRONTEIRAPAI = 0x20,
			barraFRONTEIRAFILHO = 0x40,
			barraPRECISANEUTRO = 0x80
		};
		bool logCriado;
};

#endif
//-----------------------------------------------------------------------------
// eof
