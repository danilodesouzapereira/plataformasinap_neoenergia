//---------------------------------------------------------------------------
#ifndef VTNixAlH
#define VTNixAlH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTPrimario;
class VTApl;
//---------------------------------------------------------------------------
class VTNixAl
   {
   public:
				 __fastcall  VTNixAl(void){};
	  			 __fastcall ~VTNixAl(void){};
	  virtual AnsiString __fastcall  LinhaArqCsv(void)=0;

   protected: //métodos
	  virtual double __fastcall CorrenteCarga(int patamar)=0;

   protected: //dados externos
      VTPrimario *primario;

   protected: //dados locais
      struct{
            AnsiString al;
            AnsiString cod_alim;
            AnsiString nome_alim;
            AnsiString nome_se;
            double     r1;
            double     x1;
            double     r2;
            double     x2;
            double     r0;
            double     x0;
            double     rf;
            double     ta;
            int        no_num;
            int        unidade;
            double     carga;
            } strAL;
   };
//função global p/ criar objeto da classe
VTNixAl* __fastcall NewObjNixAlDefault(VTPrimario *primario, strIMP &z0,
														 strIMP &z1, strIMP &z2,
														 bool al_com_corrente,
														 int   patamar);
VTNixAl* __fastcall NewObjNixAlEDPSP(VTApl * apl, VTPrimario *primario, strIMP &z0,
														 strIMP &z1, strIMP &z2,
														 bool al_com_corrente,
														 int   patamar);
//---------------------------------------------------------------------------
#endif
//eof

