//---------------------------------------------------------------------------
#ifndef TPontoH
#define TPontoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPonto.h"

//---------------------------------------------------------------------------
class TPonto : public VTPonto
   {
   public:  //funções públicas
                   __fastcall  TPonto(void);
      virtual      __fastcall ~TPonto(void);
              bool __fastcall  ContemHM(strHM &hm);
              bool __fastcall  ContemPeriodo(VTPonto *ponto);
              void __fastcall  Reinicia(int len);

	protected: //métodos acessados via property
		double __fastcall PM_GetNumHoras(void);
      double __fastcall PM_GetValor(int index);
      bool   __fastcall PM_SetValor(int index, double val);
   };

//---------------------------------------------------------------------------
VTPonto* __fastcall NewObjPonto1(void);
VTPonto* __fastcall NewObjPonto2(void);
VTPonto* __fastcall NewObjPonto4(void);
VTPonto* __fastcall NewObjPonto8(void);
VTPonto* __fastcall NewObjPonto16(void);
VTPonto* __fastcall NewObjPontoG(int num_val);
//---------------------------------------------------------------------------
VTPonto* __fastcall NewVetObjPonto1(int num_pto);
VTPonto* __fastcall NewVetObjPonto2(int num_pto);
VTPonto* __fastcall NewVetObjPonto4(int num_pto);
VTPonto* __fastcall NewVetObjPonto8(int num_pto);
VTPonto* __fastcall NewVetObjPonto16(int num_pto);

//---------------------------------------------------------------------------
#endif
//eof
