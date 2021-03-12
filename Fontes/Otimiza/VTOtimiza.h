//---------------------------------------------------------------------------
#ifndef VTOtimizaH
#define VTOtimizaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTProgresso;
class VTAvalia;
class VTIndividuo;
//---------------------------------------------------------------------------
struct strParAg {
                int ng;         //número de gerações
					 int npais;      //número de pais
					 int nfilhos;    //número de filhos da estrategia evolutiva
					 double pmuta_ev;//probabilidade mutação estrategia evolutiva
					 double pmuta_ag;//probabilidade mutação algoritmo genetico
					 double pcruz_ev;//probabilidade cruzamento estrategia evolutiva
					 double pcruz_ag;//probabilidade cruzamento algoritmo genetico
					 bool   flagRelDet;//se gera chamamda para relatório detalhado
                };
//---------------------------------------------------------------------------
class VTOtimiza: public TObject
   {
	public:  //property
	  __property VTAvalia*     Avalia      = {read=PM_GetAvalia, write=PM_SetAvalia};
	  __property TList*        LisIndiv    = {read=PM_GetLisIndiv};
	  __property VTIndividuo*  MelhorIndiv = {read=PM_GetMelhorIndiv};
	  __property strParAg      ParOt       = {read=PM_GetParam, write=PM_SetParam};

	public:
            __fastcall  VTOtimiza(void) {};
            __fastcall ~VTOtimiza(void) {};
      virtual bool         __fastcall Executa(VTProgresso* prog) = 0;
		virtual void         __fastcall Inicia(void) = 0;

	protected:  //métodos acessados via property
	  virtual TList*        __fastcall PM_GetLisIndiv(void) = 0;
	  virtual VTIndividuo*  __fastcall PM_GetMelhorIndiv(void) = 0;
	  virtual strParAg      __fastcall PM_GetParam(void) = 0;
	  virtual VTAvalia*     __fastcall PM_GetAvalia(void) = 0;
	  virtual void          __fastcall PM_SetAvalia(VTAvalia* avalia) = 0;
	  virtual void          __fastcall PM_SetParam(strParAg par) = 0;
	 };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTConfiab
//---------------------------------------------------------------------------
VTOtimiza* __fastcall NewObjAlgEv(void);

//---------------------------------------------------------------------------
#endif
//eof

