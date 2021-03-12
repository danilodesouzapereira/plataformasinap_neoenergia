//---------------------------------------------------------------------------
#ifndef VTTema_SinalH
#define VTTema_SinalH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class strImpCelPlanner : public TObject
   {
   public:
      __fastcall  strImpCelPlanner(void) {};
      __fastcall ~strImpCelPlanner(void) {};

   public:
       AnsiString titulo;
       struct
         {
         int grau;
         int minuto;
         double segundo;
         AnsiString hemisferio;
         }lat, lon;
       struct
         {
         double      x;
         double      y;
         int         zona;
         AnsiString zona_letra;
         }utm;
       AnsiString valor_str;
       double     valor;
   };
//---------------------------------------------------------------------------
class VTTema_Sinal : public TObject
   {
   public:  //métodos
                     __fastcall  VTTema_Sinal(void) {};
					 __fastcall ~VTTema_Sinal(void) {};
	  virtual void   __fastcall  ConverteLatLonGMStoSINAP(TList *lisSTR) = 0;
      virtual void   __fastcall  Executa(TList *lisImpCelPlan) = 0;
      //virtual void   __fastcall  Executa(TList *lisREDE, TList *lisCLASSE, int ind_pat) = 0;
      virtual void   __fastcall  FaixaValoresCelPlanner(void) = 0;
      virtual void   __fastcall  FaixaValoresDefault(void) = 0;
      //virtual void   __fastcall  FaixaValoresSinal(void) = 0;
      //virtual void   __fastcall  FaixaValoresQtdCelula(void) = 0;
      virtual void   __fastcall  FaixaValoresUniforme(void) = 0;
      virtual TList* __fastcall  LisFaixa(void) = 0;
      virtual void   __fastcall  MontaFaixaSinal(void) = 0;
      //virtual void   __fastcall  MontaGraficoDegrade(TColor color) = 0;
      virtual bool   __fastcall  MontaGraficoDegradeFaixa(TColor color1, TColor color2, bool linearDem = false) = 0;
      //virtual void   __fastcall  MontaGraficoSinalMax(void) = 0;
      virtual void   __fastcall  MontaGraficoPadrao(void) = 0;
      virtual int    __fastcall  TotalCelula(void) = 0;
      //virtual double __fastcall  TotalDemanda_mva(void) = 0;
   };

//---------------------------------------------------------------------------
VTTema_Sinal* __fastcall NewObjTema_Sinal(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
