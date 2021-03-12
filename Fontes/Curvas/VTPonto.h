//---------------------------------------------------------------------------
#ifndef VTPontoH
#define VTPontoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCurva.h"

//---------------------------------------------------------------------------
class VTPonto
   {
   public:  //property
      __property strHM  HM_ini           = {read=PD.hm_ini,      write=PD.hm_ini};
      __property strHM  HM_fim           = {read=PD.hm_fim,      write=PD.hm_fim};
      __property double Valor[int index] = {read=PM_GetValor,    write=PM_SetValor};
		__property double NumHoras         = {read=PM_GetNumHoras                   };

   public:  //funções públicas
                       __fastcall  VTPonto(void) {};
      virtual          __fastcall ~VTPonto(void) {};
      virtual VTPonto* __fastcall  Clone(int num_val) = 0;
      virtual bool     __fastcall  ContemHM(strHM &hm) = 0;
      virtual bool     __fastcall  ContemPeriodo(VTPonto *ponto) = 0;
      virtual void     __fastcall  Reinicia(int len) = 0;

	protected: //métodos acessados via property
		virtual double __fastcall PM_GetNumHoras(void) = 0;
      virtual double __fastcall PM_GetValor(int index) = 0;
      virtual bool   __fastcall PM_SetValor(int index, double val) = 0;

   protected: //dados acessados via property
      struct   {
					strHM   hm_ini, hm_fim;
					float  *valor;
					} PD;

   };

//---------------------------------------------------------------------------
VTPonto* __fastcall NewObjPonto(int num_val);
//VTPonto* __fastcall NewVetObjPonto(int num_pto, int num_val);

//---------------------------------------------------------------------------
#endif
//eof
