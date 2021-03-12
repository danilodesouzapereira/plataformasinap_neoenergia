//---------------------------------------------------------------------------
#ifndef TNixRgH
#define TNixRgH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTRegulador;

//---------------------------------------------------------------------------
class TNixRg
   {
   public:
                 __fastcall  TNixRg(VTRegulador *regulador);
                 __fastcall ~TNixRg(void);
      AnsiString __fastcall  LinhaArqCsv(void);

   private: //m�todos

   private: //dados externos
      //VTArranjo *arranjo;

   private: //dados locais
      struct{
            AnsiString rg;
            AnsiString codigo;
            int        no_inicio;
            int        no_fim;
            double     potencia;    //kVA
            int        reversivel;  //{0=n�0, 1=sim}
            int        controle;    //{0=controle �nico, 1=controle individual}
            } strRG;
   };

//---------------------------------------------------------------------------
#endif
//eof

