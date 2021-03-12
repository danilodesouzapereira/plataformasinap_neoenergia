//---------------------------------------------------------------------------
#ifndef VTTensaoH
#define VTTensaoH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
#include <Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTTensao : public TObject
   {
	public:  //property
		__property int             Dia                                  = {read=PD.dia            , write=PD.dia};
		__property int             Mes                                  = {read=PD.mes            , write=PD.mes};
		__property int             Ano                                  = {read=PD.ano            , write=PD.ano};
		__property TDateTime       Data                                 = {read=PM_GetData                      };
      __property AnsiString      DataAsString                         = {read=PM_GetDataAsString              };
      __property int             NumeroPatamares                      = {read=PD.num_pat                      };
      __property bool            Enabled  [int ind_pat]               = {read=PM_GetEnabled                   };
      __property double          ModVfn_kv[int ind_pat][int ind_fase] = {read=PM_GetModVfn_kv                 };
      __property complex<double> Vfn_kv   [int ind_pat][int ind_fase] = {read=PM_GetVfn_kv                    };


   public:
                   __fastcall  VTTensao(void) {};
                   __fastcall ~VTTensao(void) {};
      virtual bool __fastcall  InsereTensao(int ind_pat, double vfn_kv[MAX_FASE]) = 0;
      virtual bool __fastcall  InsereTensao(int ind_pat, complex<double>vfn_kv[MAX_FASE]) = 0;

   protected:  //métodos acessados vias property
      virtual TDateTime       __fastcall PM_GetData(void) = 0;
      virtual AnsiString      __fastcall PM_GetDataAsString(void) = 0;
      virtual bool            __fastcall PM_GetEnabled(int ind_pat) = 0;
      virtual double          __fastcall PM_GetModVfn_kv(int ind_pat, int ind_fase) = 0;
      virtual complex<double> __fastcall PM_GetVfn_kv(int ind_pat, int ind_fase) = 0;

   protected:  //dados acessados vias property
      struct   {
               int dia;
               int mes;
					int ano;
               int num_pat;
               }PD;
   };

//---------------------------------------------------------------------------
VTTensao*  __fastcall NewObjTensao(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof


