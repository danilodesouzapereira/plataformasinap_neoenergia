//---------------------------------------------------------------------------
#ifndef VTCalcEstratSocH
#define VTCalcEstratSocH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTDefeito;
//---------------------------------------------------------------------------
class VTCalcEstratSoc: public TObject
	{
	public:  //property
		 __property TList* LisChavesAuto        = {read=PM_GetLisChavesAuto, write=PM_SetLisChavesAuto};
		 __property double PuLim                = {read=PM_GetPuLim, write=PM_SetPuLim};
		 __property TList* LisRelSaida          = {read=PM_GetLisRelSaida, write=PM_SetLisRelSaida};

	public:
							__fastcall VTCalcEstratSoc(void) {};
							__fastcall ~VTCalcEstratSoc(void) {};
	  virtual  bool   __fastcall CalculaEstrategiaSoc(VTDefeito *pdef, int iPat, double pu_lim, TList *lisChavesAuto, TList *lisChavesAtivas = NULL) = 0;
	  virtual  bool   __fastcall IniciaEstrategiaSoc(VTDefeito *pdef, int iPat) = 0;

	protected: //métodos acessados via property
	  virtual  TList* __fastcall PM_GetLisChavesAuto(void) = 0;
	  virtual  void   __fastcall PM_SetLisChavesAuto(TList *lisChavesAuto) = 0;
	  virtual  double __fastcall PM_GetPuLim(void) = 0;
	  virtual  void   __fastcall PM_SetPuLim(double pu_lim) = 0;
	  virtual  TList* __fastcall  PM_GetLisRelSaida(void) = 0;
	  virtual  void   __fastcall  PM_SetLisRelSaida(TList *lisRelSaida) = 0;

	public:
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTCalcEstratSoc* __fastcall NewObjCalcEstratSoc(void);

//---------------------------------------------------------------------------
#endif

