//---------------------------------------------------------------------------
#ifndef TCalcIndicesH
#define TCalcIndicesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTCalcIndCont.h"
//---------------------------------------------------------------------------
class VTReducao;
class TCorteConf;
//---------------------------------------------------------------------------
class TCalcIndices : public VTCalcIndCont
	{
	public:
					__fastcall TCalcIndices(VTApl* apl);
					__fastcall ~TCalcIndices(void);
	 bool       __fastcall Executa(TList *lisCortes, TList *lisArvs, VTProgresso *prog);
	 bool       __fastcall AgregaRes(RES_CONF &res_conf, TList* lisArv);
	 //bool       __fastcall Executa(VTArvores* arv);

	private: //métodos
VTReducao*     __fastcall BuscaReducao(VTEqpto* eqpto);
		bool     __fastcall CalcInd1(PAR_CONF p1, TList* lisFontes, TList* lisFontesP, RES_MC &res);
		bool     __fastcall CalcInd2(PAR_CONF p1, PAR_CONF p2, TList* lisFontes, RES_MC &res);
		bool     __fastcall CalcInd3(PAR_CONF p1, PAR_CONF p2, PAR_CONF p3, TList* lisFontes, RES_MC &res);
	 double     __fastcall CompEqpto(VTEqpto* eqpto);
	 int        __fastcall NCons(VTEqpto* eqpto);
	 double     __fastcall PotMedia(TList* lisEqpto);
	 double     __fastcall PotMedia(VTEqpto* eqpto);
	 double     __fastcall PotMediaCarga(VTEqpto* eqpto);
	 double     __fastcall PotMediaReducao(VTEqpto* eqpto);
	private: //dados externos
    VTApl* apl;

   private: //dados locais
   };
#endif
//---------------------------------------------------------------------------
//eof

