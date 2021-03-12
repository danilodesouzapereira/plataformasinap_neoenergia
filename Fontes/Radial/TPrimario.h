//---------------------------------------------------------------------------
#ifndef TPrimarioH
#define TPrimarioH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPrimario.h"

//---------------------------------------------------------------------------
class VTCorte;

//---------------------------------------------------------------------------
class TPrimario : public VTPrimario
   {
   private: //dados acessados via property
      struct   {
               VTBarra      *bar_ini;
               VTRede       *rede;
               VTSubestacao *subestacao;
               } PD;

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList  *lisSEC;
      TList  *lisBAR;
      TList  *lisLIG;
      TList  *lisEQP;

   public:  //métodos
                    __fastcall  TPrimario(VTApl *apl_owner);
                    __fastcall ~TPrimario(void);
      VTSecundario* __fastcall  ExisteSecundario(VTRede *rede);
      void          __fastcall  InsereSecundario(VTSecundario* secundario);
      void          __fastcall  LisEqptoPriSec(TList *lisEXT, int tipo_eqpto, bool barra_isolada=true);
      void          __fastcall  LisRede(TList *lisEXT, int segmento);
      TList*        __fastcall  LisSecundario(void);
      bool          __fastcall  Ordena(TList *lisEXT_BAR, TList *lisEXT_LIG);
      VTPatamar*    __fastcall  PatamarDemandaMaxima(complex<double>&s_mva);
      VTPatamar*    __fastcall  PatamarDemandaMinima(complex<double>&s_mva);

   protected:  //métodos acessados via property
      VTBarra*      __fastcall PM_GetBarraInicial(void);
      VTLigacao*    __fastcall PM_GetLigacaoInicial(void);
      VTRede*       __fastcall PM_GetRede(void);
      VTSubestacao* __fastcall PM_GetSubestacao (void);
      void          __fastcall PM_SetRede(VTRede *rede);
      void          __fastcall PM_SetSubestacao(VTSubestacao* subestacao);

   private: //métodos
      void          __fastcall DeterminaBarraInicial(void);
      VTCorte*      __fastcall ExisteCorteComRedeComGeracao(void);
      VTCorte*      __fastcall ExisteCorteComSubestacao(void);
   };

//---------------------------------------------------------------------------
#endif
//eof
