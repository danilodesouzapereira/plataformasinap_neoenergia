//---------------------------------------------------------------------------
#ifndef TTema_DCH
#define TTema_DCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTema_DC.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCelula;
class VTRede;

//---------------------------------------------------------------------------
class TTema_DC : public VTTema_DC
   {
   public:  //métodos
             __fastcall  TTema_DC(VTApl *apl);
             __fastcall ~TTema_DC(void);

      void   __fastcall  Executa(TList *lisREDE, TList *lisCLASSE);
      void   __fastcall  Executa(TList *lisREDE, TList *lisCLASSE, int ind_pat);
      void   __fastcall  FaixaValoresDefault(void);
      void   __fastcall  FaixaValoresDemanda(void);
      void   __fastcall  FaixaValoresQtdCelula(void);
      void   __fastcall  FaixaValoresUniforme(void);
      TList* __fastcall  LisFaixa(void);
      void   __fastcall  MontaFaixaDemanda(void);
      void   __fastcall  MontaGraficoDegrade(TColor color);
      bool   __fastcall  MontaGraficoDegradeFaixa(TColor color1, TColor color2, bool linearDem = false);
      //bool   __fastcall  MontaGraficoDegradeFaixaLinear(TColor color1, TColor color2);
      void   __fastcall  MontaGraficoPadrao(void);
      void   __fastcall  MontaGraficoDemandaMax(void);
      int    __fastcall  TotalCelula(void);
      double __fastcall  TotalDemanda_mva(void);

   private: //métodos locais
      void   __fastcall CalculaValorProprioCelula(void);
      void   __fastcall CalculaValorTotalCelula(void);
      TColor __fastcall Degrade(TColor color, double valor);
      TColor __fastcall Degrade(TColor beginColor, TColor endColor, double valor);
      TColor __fastcall DegradeCinza(double valor);
      void   __fastcall IniciaLisCarga(VTRede *rede, TList *lisCARGA);
      void   __fastcall IniciaLisCelula(TList *lisREDE, TList *lisCLASSE, int ind_pat = -1);
      void   __fastcall IniciaLisFaixa(void);
      void   __fastcall IniciaLisFaixaEnabled(void);
      //void   __fastcall MontaFaixaDemanda(void);
      void   __fastcall NormalizaValorTotalCelula(void);
      void   __fastcall RemoveCelulaNula(TList *lisORDENADA);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisEQP;
      TList *lisFAIXA;
      TList *lisFAIXA_ENABLED;
      bool   degrade_ok;
      struct{
            int    qtde_cel;
            double s_mva;
            }total;
      bool   lisCelula_iniciado;
   };

//---------------------------------------------------------------------------
#endif
//eof
