//---------------------------------------------------------------------------
#ifndef VTPerfilReguladorH
#define VTPerfilReguladorH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>
//---------------------------------------------------------------------------
class VTRegulador;
class VTMedicao;

//---------------------------------------------------------------------------
class VTPerfilRegulador : public TObject
   {
   public:  //property
		__property VTRegulador* Reg                   	  = {read=PD.reg};
		__property bool     		FaseEnabled[int ind_fase] = {read=PM_GetFaseEnabled        , write=PM_SetFaseEnabled};
	public:
							__fastcall  VTPerfilRegulador(void) {};
							__fastcall ~VTPerfilRegulador(void) {};
		virtual bool 	__fastcall 	Carregamento(int ind_dia, int ind_pat, double* cargto) = 0;			
      virtual bool 	__fastcall  Corrente(int ind_dia, int ind_pat, int ind_fase, double* i_antes, double* i_depois) = 0;
		virtual int    __fastcall  NumeroAmostragem(void) = 0;
      virtual bool   __fastcall  SalvaCarreg(TDateTime data, VTMedicao *medicao) = 0;
	
	protected:  //dados acessados vias property
		virtual bool __fastcall  PM_GetFaseEnabled(int ind_fase) = 0;
		virtual void __fastcall  PM_SetFaseEnabled(int ind_fase, bool enabled) = 0;

      struct   {
					VTRegulador *reg;
					bool    		fase_enabled[MAX_FASE];
					int     		num_amostra;
					}PD;
   };

//---------------------------------------------------------------------------
VTPerfilRegulador* __fastcall NewObjPerfilRegulador(VTRegulador *reg);

//---------------------------------------------------------------------------
#endif
//eof


