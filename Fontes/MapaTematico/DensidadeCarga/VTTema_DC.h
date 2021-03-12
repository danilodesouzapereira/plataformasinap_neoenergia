//---------------------------------------------------------------------------
#ifndef VTTema_DCH
#define VTTema_DCH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTTema_DC : public TObject
   {
   public:  //métodos
                     __fastcall  VTTema_DC(void) {};
                     __fastcall ~VTTema_DC(void) {};
      virtual void   __fastcall  Executa(TList *lisREDE, TList *lisCLASSE) = 0;
      virtual void   __fastcall  Executa(TList *lisREDE, TList *lisCLASSE, int ind_pat) = 0;
      virtual void   __fastcall  FaixaValoresDefault(void) = 0;
      virtual void   __fastcall  FaixaValoresDemanda(void) = 0;
      virtual void   __fastcall  FaixaValoresQtdCelula(void) = 0;
      virtual void   __fastcall  FaixaValoresUniforme(void) = 0;
      virtual TList* __fastcall  LisFaixa(void) = 0;
      virtual void   __fastcall  MontaFaixaDemanda(void) = 0;
      virtual void   __fastcall  MontaGraficoDegrade(TColor color) = 0;
      virtual bool   __fastcall  MontaGraficoDegradeFaixa(TColor color1, TColor color2, bool linearDem = false) = 0;
      virtual void   __fastcall  MontaGraficoDemandaMax(void) = 0;
      virtual void   __fastcall  MontaGraficoPadrao(void) = 0;
      virtual int    __fastcall  TotalCelula(void) = 0;
      virtual double __fastcall  TotalDemanda_mva(void) = 0;
   };

//---------------------------------------------------------------------------
VTTema_DC* __fastcall NewObjTema_DC(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
