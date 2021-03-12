//---------------------------------------------------------------------------
#ifndef TCarregH
#define TCarregH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>
#include "VTCarreg.h"

//---------------------------------------------------------------------------
class TCarreg  : public VTCarreg
   {
   public:
			  __fastcall  TCarreg(int num_pat);
			  __fastcall ~TCarreg(void);
		bool __fastcall  InsereCarreg(int ind_pat, double Carreg);
		bool __fastcall  InsereCorrenteAntes(int ind_pat, double I_A[MAX_FASE]);
		bool __fastcall  InsereCorrenteDepois(int ind_pat, double I_A[MAX_FASE]);

	protected:  //métodos acessados vias property
      TDateTime       __fastcall PM_GetData(void);
      AnsiString      __fastcall PM_GetDataAsString(void);
		bool            __fastcall PM_GetEnabled(int ind_pat);
		double			 __fastcall PM_GetCarreg(int ind_pat);
		double          __fastcall PM_GetModI_Antes(int ind_pat, int ind_fase);
      double          __fastcall PM_GetModI_Depois(int ind_pat, int ind_fase);
	private: //métodos

	private: //dados
		struct strCARREG  {
                        bool            enabled;
								double			 Carreg;
								double			 i_antes[MAX_FASE];
								double			 i_depois[MAX_FASE];
								}*carreg;

   };

//---------------------------------------------------------------------------
#endif
//eof


