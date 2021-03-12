//---------------------------------------------------------------------------
#ifndef TLineH
#define TLineH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TLine
   {
   public:
      __property char Separador    = {read=PD.separador,   write=PD.separador};

   public:
                 __fastcall  TLine(void);
                 __fastcall ~TLine(void);
      void       __fastcall  Clear(void);
      void       __fastcall  InsereCampo(AnsiString valor);
      void       __fastcall  InsereCampo(int        valor);
      void       __fastcall  InsereCampo(double     valor, AnsiString formato="%6.5f");
      void       __fastcall  InsereCampo(TObject    *objet);
      AnsiString __fastcall  Texto(void);

   private: //dados
      AnsiString texto;
      struct{
            char separador;
            } PD;
   };

#endif
//---------------------------------------------------------------------------
//eof

