//---------------------------------------------------------------------------
#ifndef TResumoTecH
#define TResumoTecH

//----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResumoTec.h"

//----------------------------------------------------------------------------
//class

//----------------------------------------------------------------------------
class TResumoTec : public VTResumoTec
   {
   public:
				__fastcall  TResumoTec(int ano_ini, int ano_fim);
				__fastcall ~TResumoTec(void);
   VTResumoTec* __fastcall ExisteResumoRede(AnsiString alimentador, AnsiString subestacao);
	  TList*    __fastcall LisResumoTecRede(void);

   protected: //métodos acessados via property
      int    __fastcall PM_GetAnoInicial(void);
      int    __fastcall PM_GetAnoFinal(void);
      bool   __fastcall PM_GetCalculo_OK(int ano);
      double __fastcall PM_GetCompensacao_RS(int ano);
	  int    __fastcall PM_GetDiagBarra(int ano, int idiag);
	  int    __fastcall PM_GetDiagCarga(int ano, int idiag);
	  int    __fastcall PM_GetDiagTrafo(int ano, int idiag);
	  double __fastcall PM_GetDiagTrechoKm(int ano, int idiag);
	  double __fastcall PM_GetDEC(int ano);
	  double __fastcall PM_GetEND(int ano);
	  double __fastcall PM_GetFEC(int ano);
      double __fastcall PM_GetEntrada(int ano);
      double __fastcall PM_GetEntregue(int ano);
	  double __fastcall PM_GetGerada(int ano);
	  double __fastcall PM_GetNota(void);
	  double __fastcall PM_GetPercDiagBarra(int ano, int idiag);
	  double __fastcall PM_GetPercDiagCarga(int ano, int idiag);
	  double __fastcall PM_GetPercDiagTrafo(int ano, int idiag);
      double __fastcall PM_GetPerda_mwhm(int ano);
      double __fastcall PM_GetPerda_perc(int ano);
      //
      void   __fastcall PM_SetCalculo_OK(int ano, bool calculo_ok);
	  void   __fastcall PM_SetCompensacao_RS(int ano, double valor);
	  void   __fastcall PM_SetDiagBarra(int ano, int idiag, int neq);
	  void   __fastcall PM_SetDiagCarga(int ano, int idiag, int neq);
	  void   __fastcall PM_SetDiagTrafo(int ano, int idiag, int neq);
	  void   __fastcall PM_SetDiagTrechoKm(int ano, int idiag, double comp_km);
	  void   __fastcall PM_SetDEC(int ano, double valor);
	  void   __fastcall PM_SetEND(int ano, double valor);
	  void   __fastcall PM_SetFEC(int ano, double valor);
      void   __fastcall PM_SetEntrada(int ano, double valor);
      void   __fastcall PM_SetEntregue(int ano, double valor);
	  void   __fastcall PM_SetGerada(int ano, double valor);
	  void   __fastcall PM_SetPercDiagBarra(int ano, int idiag, double neq);   //DVK 2013.12.20
	  void   __fastcall PM_SetPercDiagCarga(int ano, int idiag, double neq);
	  void   __fastcall PM_SetPercDiagTrafo(int ano, int idiag, double neq);  //DVK 2013.12.20
      void   __fastcall PM_SetPerda_mwhm(int ano, double valor);
      void   __fastcall PM_SetPerda_perc(int ano, double valor);

   private: //métodos
	  int        __fastcall IndiceAno(int ano);

   private: //obj locais
	  TList* lisRESUMO_REDE;

   };

//---------------------------------------------------------------------------
#endif
//eof
