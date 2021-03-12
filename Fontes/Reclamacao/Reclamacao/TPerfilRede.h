//---------------------------------------------------------------------------
#ifndef TPerfilRedeH
#define TPerfilRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>
#include "VTPerfilRede.h"

//---------------------------------------------------------------------------
class TPerfilRede  : public VTPerfilRede
   {
   public:
								 __fastcall  TPerfilRede(VTApl *apl);
								 __fastcall ~TPerfilRede(void);
//		void           	 __fastcall  CalculaDRA_DRP_DRC(int limite_amostra);
//		void           	 __fastcall  CalculaIndicesQualidadeUmDia(int ind_dia);
//		VTPerfilBarra*     __fastcall  ExistePerfilBarra(VTBarra *barra);
		VTPerfilCarga*     __fastcall  ExistePerfilCarga(VTCarga *carga);
//		VTPerfilRegulador* __fastcall  ExistePerfilReg(VTRegulador *reg);
      VTPerfilSup*       __fastcall  ExistePerfilSup(VTSuprimento *sup);
//		TList*         	 __fastcall  LisPerfilBarra(void);
		void           	 __fastcall  Reinicia(void);
      bool               __fastcall  RestauraPerfilCarga(TDateTime data);
//      bool               __fastcall  SalvaPerfilBarra(TDateTime data);
		bool               __fastcall  SalvaPerfilCarga(TDateTime data);
//		bool               __fastcall  SalvaPerfilRegulador(TDateTime data);
		bool               __fastcall  SalvaPerfilSuprimento(TDateTime data);

   protected:  //métodos acessados via property
//      bool           	__fastcall  PM_GetEnabled(void);

   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList *lisEQP;
//		TList *lisPERFIL_BAR;
      TList *lisPERFIL_CAR;
//		TList *lisPERFIL_REG;
		TList *lisPERFIL_SUP;
   };

//---------------------------------------------------------------------------
#endif
//eof


