//---------------------------------------------------------------------------
#ifndef TCorteH
#define TCorteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TCorte : public TObject
   {
   public:
              __fastcall TCorte(void);
              __fastcall ~TCorte(void);
   AnsiString __fastcall Codigo(void);

   private: //métodos

   private: //dados externos

   private: //dados locais
   public:
       TList* lisEqpto;  //Lista de equipamentos pertencentes ao corte
       TList* lisChvSoc; //Lista de possíveis socorros
       int    icorte;//0: total 1:parcial
       struct {
              double dic, fic;
              double tx_ff, r_ff;
              double tx_fm, r_fm;
              }res_conf;
   };

#endif
//---------------------------------------------------------------------------
//eof

