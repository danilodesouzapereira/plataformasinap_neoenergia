//---------------------------------------------------------------------------
#ifndef TNixAlEDPSPH
#define TNixAlEDPSPH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"
#include "TNixAl.h"
//---------------------------------------------------------------------------
class VTPrimario;
class VTApl;
//---------------------------------------------------------------------------
class TNixAlEDPSP : public TNixAl
   {
   public:
				 __fastcall  TNixAlEDPSP(VTApl *apl, VTPrimario *primario, strIMP &z0,
														 strIMP &z1, strIMP &z2,
                                                         bool al_com_corrente,
                                                         int   patamar);
                 __fastcall ~TNixAlEDPSP(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   private: //métodos
	  double __fastcall CorrenteCarga(int patamar);
	  bool   __fastcall LeZccDoArquivo(void);

   private: //dados externos
      VTPrimario *primario;
	  VTApl	*apl;

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

