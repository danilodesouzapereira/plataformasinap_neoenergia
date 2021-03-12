//---------------------------------------------------------------------------
#ifndef TRecondutorarH
#define TRecondutorarH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum opRECOND {opJUSANTE = 0, opT_ARRANJO, opT_RADIAL, opT_TOTAL};

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTArvore;
class VTBarra;
class VTEqpto;
class VTLigacao;
class VTPatamar;
class VTRadial;
class VTRede;
class VTTrecho;
class VTTronco;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14217067
class TRecondutorar : public TObject
{
	public:  //property
	  __property VTEqpto *Eqpto  = {read=PM_GetEqpto,    write=PM_SetEqpto};
	  __property VTPatamar *Patamar = {read=PM_GetPatamar,    write=PM_SetPatamar};

	public:
				   __fastcall  TRecondutorar(VTApl *apl, TList *lisEQP);
				   __fastcall  ~TRecondutorar(void);
		AnsiString __fastcall  ComprimentoSel(void);
		bool 	   __fastcall  Executa(VTArranjo *arranjo);
		bool 	   __fastcall  Seleciona(void);

	public:
		int opcao;

	private:
		void __fastcall CopiaTrechos(TList *lisLIG);
		VTEqpto* __fastcall PM_GetEqpto(void);
		void __fastcall PM_SetEqpto(VTEqpto *eqpto);
		VTPatamar* __fastcall PM_GetPatamar(void);
		void __fastcall PM_SetPatamar(VTPatamar *patamar);
		bool __fastcall SelecionaJusante(void);
		bool __fastcall SelecionaTroncoArranjo(void);
		bool __fastcall SelecionaTroncoFluxo(bool limitado);
		VTTrecho* __fastcall TrechoInicial(VTBarra *barra, VTRede *rede, VTLigacao *liga_ini);
		bool __fastcall TrechosMesmoArranjoInicial(TList *lisNODE);
		bool __fastcall TrechosMesmoArranjoNode(TList *lisNODE, VTArranjo *arranjo);

	private: //obj externo
		TList *lisEQP;
		VTApl *apl;
		VTEqpto *eqpto;
		VTPatamar *patamar;

	private: //obj local
		TList *lisBAR, *lisLIG;//, *lisNODE_JUS;
		VTArvore *arvore;
		VTRadial *radial;
		VTTronco *tronco;
};

//---------------------------------------------------------------------------
#endif
