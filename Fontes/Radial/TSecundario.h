//---------------------------------------------------------------------------
#ifndef TSecundarioH
#define TSecundarioH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSecundario.h"

//---------------------------------------------------------------------------
class TSecundario : public VTSecundario
   {
   private: //dados acessados via property
      struct   {
               VTBarra    *barra_inicial;
               VTRede     *rede;
               VTPrimario *primario;
               } PD;

   public:  //métodos
                 __fastcall  TSecundario(VTApl *apl_owner);
                 __fastcall ~TSecundario(void);
      bool       __fastcall  Ordena(TList *lisEXT_BAR, TList *lisEXT_LIG);
      VTPatamar* __fastcall  PatamarDemandaMaxima(complex<double>&s_mva);
      VTPatamar* __fastcall  PatamarDemandaMinima(complex<double>&s_mva);

   protected:  //métodos acessados via property
      VTBarra*    __fastcall PM_GetBarraInicial(void);
      VTPrimario* __fastcall PM_GetPrimario(void);
      VTRede*     __fastcall PM_GetRede(void);
      void        __fastcall PM_SetBarraInicial(VTBarra *barra);
      void        __fastcall PM_SetPrimario(VTPrimario *primario);
      void        __fastcall PM_SetRede(VTRede *rede);

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList  *lisEQP;
   };

//---------------------------------------------------------------------------
#endif
//eof
