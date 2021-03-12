//---------------------------------------------------------------------------
#ifndef TNixPrH
#define TNixPrH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
#include "VTNixPr.h"

//---------------------------------------------------------------------------
class VTChave;

//---------------------------------------------------------------------------
class TNixPr : public VTNixPr
   {
   public:
				 __fastcall  TNixPr(VTChave *chave, int index, bool pr_com_corrente);
				 __fastcall  TNixPr(void){};
                 __fastcall ~TNixPr(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   protected: //métodos
      complex<double> __fastcall CorrenteMaxima(VTChave *chave);
      int             __fastcall SupoeTipo(VTChave *chave);
      AnsiString      __fastcall TraduzCodigo(VTChave *chave, int index);


   private: //dados externos
      //VTArranjo *arranjo;

   protected: //dados locais
      struct{
            AnsiString pr;
            AnsiString codigo;
            int        no_inicio;
            int        no_fim;
            double     i_mod;       //obsoleto, definir com valor zero
            double     i_ang;       //obsoleto, definir com valor zero
            AnsiString elo;
            int        cons;        //obsoleto, definir com valor zero
            int        tipo;
            } strPR;
   };

//---------------------------------------------------------------------------
#endif
//eof

