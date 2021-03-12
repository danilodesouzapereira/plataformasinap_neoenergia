//---------------------------------------------------------------------------
#ifndef TNixCbH
#define TNixCbH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArranjo;

//---------------------------------------------------------------------------
class TNixCb
   {
   public:
                 __fastcall  TNixCb(VTArranjo *arranjo);
                 __fastcall ~TNixCb(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   private: //métodos

   private: //dados externos
      //VTArranjo *arranjo;

   private: //dados locais
      struct{
            AnsiString cb;
            AnsiString cabo;
            double     r1;
            double     x1;
            double     r2;
            double     x2;
            double     r0;
            double     x0;
            int        unidade;
            AnsiString descricao;
            } strCB;
   };

//---------------------------------------------------------------------------
#endif
//eof

