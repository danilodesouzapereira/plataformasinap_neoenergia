//---------------------------------------------------------------------------
#ifndef TNixTrH
#define TNixTrH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTTrecho;

//---------------------------------------------------------------------------
class TNixTr
   {
   public:
                 __fastcall  TNixTr(VTTrecho *trecho);
                 __fastcall ~TNixTr(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   private: //métodos
      int __fastcall TraduzFases(int fases);

   private: //dados externos
      //VTBarra *barra;

   private: //dados locais
      struct{
            AnsiString tr;
            int        no_inicio;
            int        no_fim;
            AnsiString cabo;
            double     distancia;
            int        fases;
            int        estado;
            } strTR;
   };

//---------------------------------------------------------------------------
#endif
//eof

