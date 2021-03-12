//---------------------------------------------------------------------------
#ifndef TNixBcH
#define TNixBcH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTCapacitor;

//---------------------------------------------------------------------------
class TNixBc
   {
   public:
                 __fastcall  TNixBc(VTCapacitor *capacitor);
                 __fastcall ~TNixBc(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   private: //métodos

   private: //dados externos
      //VTCapacitor *capacitor;

   private: //dados locais
      struct{
            AnsiString bc;
            AnsiString codigo;
            int        num_no;
            double     potencia;
            int        elementos;
            int        ligacao;
            } strBC;
   };

//---------------------------------------------------------------------------
#endif
//eof

