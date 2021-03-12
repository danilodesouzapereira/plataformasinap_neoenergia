//---------------------------------------------------------------------------
#ifndef VTCarregH
#define VTCarregH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTCarreg : public TObject
   {
   public:  //property
      __property int             Dia                                  	= {read=PD.dia            , write=PD.dia};
		__property int             Mes                                  	= {read=PD.mes            , write=PD.mes};
		__property int             Ano                                  	= {read=PD.ano            , write=PD.ano};
		__property TDateTime       Data                                 	= {read=PM_GetData                      };
		__property AnsiString      DataAsString                         	= {read=PM_GetDataAsString              };
		__property int             NumeroPatamares                      	= {read=PD.num_pat                      };
		__property double          Carregamento [int ind_pat]           	= {read=PM_GetCarreg                    };
      __property bool            Enabled  [int ind_pat]                 = {read=PM_GetEnabled                   };
		__property double          ModI_Antes[int ind_pat][int ind_fase]	= {read=PM_GetModI_Antes            	 };
		__property double          ModI_Depois[int ind_pat][int ind_fase] = {read=PM_GetModI_Depois          		 };

	public:
							__fastcall  VTCarreg(void) {};
							__fastcall ~VTCarreg(void) {};
		virtual bool 	__fastcall  InsereCarreg(int ind_pat, double Carregamento) = 0;
		virtual bool   __fastcall  InsereCorrenteAntes(int ind_pat, double I_A[MAX_FASE]) = 0;
		virtual bool   __fastcall  InsereCorrenteDepois(int ind_pat, double I_A[MAX_FASE]) = 0;

	protected:  //dados acessados vias property
		virtual TDateTime       __fastcall PM_GetData(void) = 0;
      virtual AnsiString      __fastcall PM_GetDataAsString(void) = 0;
      virtual bool            __fastcall PM_GetEnabled(int ind_pat) = 0;
		virtual double          __fastcall PM_GetCarreg(int ind_pat) = 0;
		virtual double          __fastcall PM_GetModI_Antes(int ind_pat, int ind_fase) = 0;
  		virtual double          __fastcall PM_GetModI_Depois(int ind_pat, int ind_fase) = 0;

      struct   {
               int dia;
					int mes;
               int ano;
               int num_pat;
               }PD;
   };

//---------------------------------------------------------------------------
VTCarreg*  __fastcall NewObjCarreg(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof


