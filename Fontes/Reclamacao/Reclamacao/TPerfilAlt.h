//---------------------------------------------------------------------------

#ifndef TPerfilAltH
#define TPerfilAltH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>
#include "VTPerfilAlt.h"

//---------------------------------------------------------------------------
class TPerfilAlt  : public VTPerfilAlt
   {
   public:
	                     __fastcall  TPerfilAlt(VTApl *apl);
                        __fastcall ~TPerfilAlt(void);
   void           	   __fastcall  CalculaDRA_DRP_DRC(int limite_amostra);
   void           	   __fastcall  CalculaIndicesQualidadeUmDia(int ind_dia);
   VTPerfilBarra*       __fastcall  ExistePerfilBarra(VTBarra *barra);
   VTPerfilRegulador*   __fastcall  ExistePerfilReg(VTRegulador *reg);
   TList*         	   __fastcall  LisPerfilBarra(void);
   void           	   __fastcall  Reinicia(void);
   bool                 __fastcall  SalvaPerfilBarra(TDateTime data);
   bool                 __fastcall  SalvaPerfilRegulador(TDateTime data);
   
   protected:  //métodos acessados via property
      bool           	__fastcall  PM_GetEnabled(void);

   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList *lisEQP;
		TList *lisPERFIL_BAR;
		TList *lisPERFIL_REG;
   };
//---------------------------------------------------------------------------
#endif
