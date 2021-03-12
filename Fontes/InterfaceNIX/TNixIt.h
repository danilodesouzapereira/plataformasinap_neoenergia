//---------------------------------------------------------------------------
#ifndef TNixItH
#define TNixItH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTChave;

//---------------------------------------------------------------------------
class TNixIt
   {
   public:
                 __fastcall  TNixIt(VTChave *chave, AnsiString data, int tipo_def, double icc_amp);
                 __fastcall ~TNixIt(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   private: //métodos

   private: //dados externos
      //VTArranjo *arranjo;

   private: //dados locais
      struct{
            AnsiString it;
            AnsiString codigo;
            AnsiString data;
            double     duracao;  //seg;
            AnsiString causa;
            int        tipo;
            double     valor;
            } strIT;
   };

//---------------------------------------------------------------------------
#endif
//eof

