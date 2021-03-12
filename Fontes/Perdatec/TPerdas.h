//---------------------------------------------------------------------------
#ifndef TPerdasH
#define TPerdasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPerdas.h"

//---------------------------------------------------------------------------
class PERDA_TRAFO;
class VTGeral;
class VTPatamares;
class VTRede;
class VTRedes;
class VTTrafo;
class VTTrafo3E;

//---------------------------------------------------------------------------
class TPerdas : public VTPerdas
   {
   public:
               __fastcall TPerdas(VTApl *apl);
               __fastcall ~TPerdas(void);
      bool     __fastcall Executa(void);
      VTPerda* __fastcall ExistePerda(TObject *object);
      TList*   __fastcall LisPerda(void);
      void     __fastcall Reinicia(void);

   private: //métodos
      PERDA_TRAFO* __fastcall ExistePerdaTrafo(VTTrafo *trafo);
      PERDA_TRAFO* __fastcall ExistePerdaTrafo3E(VTTrafo3E *trafo);
      void         __fastcall InserePerda(VTPerda *perda, TList *lisPER);
      bool         __fastcall PerdaPorCluster(void);
      bool         __fastcall PerdaPorRede(void);
      bool         __fastcall PerdaPorTipoRede(void);
      bool         __fastcall PerdaTotal(void);
      void         __fastcall SalvaPerdaCabo(VTRede *rede, VTPerda *perda, TList *lisTRE);
      void         __fastcall SalvaPerdaFluxo(VTRede *rede, VTPerda *perda);
      void         __fastcall SalvaPerdaFora(VTRede *rede, VTPerda *perda);
      void         __fastcall SalvaPerdaMedidorPorDentro(VTRede *rede, VTPerda *perda, TList *lisCAR);
      void         __fastcall SalvaPerdaMedidorPorFora(VTRede *rede, VTPerda *perda, TList *lisCAR);
      void         __fastcall SalvaPerdaOutros(VTRede *rede, VTPerda *perda);
      void         __fastcall SalvaPerdaRamalGis(VTRede *rede, VTPerda *perda);
      void         __fastcall SalvaPerdaRamalPorDentro(VTRede *rede, VTPerda *perda);
      void         __fastcall SalvaPerdaRamalPorFora(VTRede *rede, VTPerda *perda, TList *lisCAR);
      void         __fastcall SalvaPerdaTotal(VTRede *rede, VTPerda *perda);
      void         __fastcall SalvaPerdaTrafoPorDentro(VTRede *rede, VTPerda *perda, TList *lisTRF);
      void         __fastcall SalvaPerdaTrafoPorFora(VTRede *rede, VTPerda *perda, TList *lisTRF);
      void         __fastcall SalvaPerdaTrafo3EPorDentro(VTRede *rede, VTPerda *perda, TList *lisTRF);
      void         __fastcall SalvaPerdaTrafo3EPorFora(VTRede *rede, VTPerda *perda, TList *lisTRF);

   private: //objetos externos
      VTApl       *apl;
      VTGeral     *geral;
      VTPatamares *patamares;
      VTRedes     *redes;

   private: //dados locais
      TList *lisEQP;
      TList *lisPERDA;
   };

#endif
//---------------------------------------------------------------------------
//eof

