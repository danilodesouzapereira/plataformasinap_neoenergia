//---------------------------------------------------------------------------
#ifndef TNixAlH
#define TNixAlH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"
#include "VTNixAl.h"

//---------------------------------------------------------------------------
class VTPrimario;

//---------------------------------------------------------------------------
class TNixAl : public VTNixAl
   {
   public:
                 __fastcall  TNixAl(VTPrimario *primario, strIMP &z0,
                                                         strIMP &z1, strIMP &z2,
                                                         bool al_com_corrente,
														 int   patamar);
   				 __fastcall  TNixAl(void){};
                 __fastcall ~TNixAl(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   private: //métodos
      double __fastcall CorrenteCarga(int patamar);

   private: //dados externos
      VTPrimario *primario;

   private: //dados locais
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

//---------------------------------------------------------------------------
#endif
//eof

