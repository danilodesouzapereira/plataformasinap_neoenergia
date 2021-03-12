//---------------------------------------------------------------------------
#ifndef TPerfilReguladorH
#define TPerfilReguladorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>
#include "VTPerfilRegulador.h"

//---------------------------------------------------------------------------
class VTCarreg;

//---------------------------------------------------------------------------
class TPerfilRegulador  : public VTPerfilRegulador
	{
	public:
				 __fastcall  TPerfilRegulador(VTRegulador *reg);
				 __fastcall ~TPerfilRegulador(void);
		bool 	 __fastcall  Carregamento(int ind_dia, int ind_pat, double* cargto);
		bool 	 __fastcall  Corrente(int ind_dia, int ind_pat, int ind_fase, double* i_antes, double* i_depois);
		int    __fastcall  NumeroAmostragem(void);
		bool   __fastcall  SalvaCarreg(TDateTime data, VTMedicao *medicao);

	protected:  //métodos acessados vias property
      bool __fastcall  PM_GetFaseEnabled(int ind_fase);
		void __fastcall  PM_SetFaseEnabled(int ind_fase, bool enabled);

   private: //métodos
		VTCarreg* __fastcall ExistePerfilReg(TDateTime data);

   private: //objetos externos

   private: //dados
		TList *lisCARREG;
   };

//---------------------------------------------------------------------------
#endif
//eof


