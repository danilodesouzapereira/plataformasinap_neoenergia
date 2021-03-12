//---------------------------------------------------------------------------
#ifndef TNixNoH
#define TNixNoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTScg;
//---------------------------------------------------------------------------
class TNixNo
   {
   public:
                 __fastcall  TNixNo(VTBarra *barra);
                 __fastcall ~TNixNo(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   private: //métodos

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais
      struct{
            AnsiString no;
            int        numero;
            AnsiString nome;
            double     tensao;
            int        coord_x;
            int        coord_y;
            } strNO;
   };

//---------------------------------------------------------------------------
#endif
//eof

