//---------------------------------------------------------------------------
#ifndef TDefeitosH
#define TDefeitosH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;

//---------------------------------------------------------------------------
class TDefeito : public TObject
   {
   public:
      __fastcall  TDefeito(void) {};
      __fastcall ~TDefeito(void) {};

   public: //dados
      VTBarra    *barra;
      int        tipodefeito;
      int        fases;
   };

//---------------------------------------------------------------------------
class TDefeitos : public TObject
   {
   public:
                 __fastcall  TDefeitos(void);
                 __fastcall ~TDefeitos(void);
       void      __fastcall  Clear(void);
       TDefeito* __fastcall  ExisteDefeito(VTBarra *barra);
       TDefeito* __fastcall  ExisteDefeito(VTBarra *barra, int tipodefeito, int fases);
       TDefeito* __fastcall  FirstDefeito(void);
       TDefeito* __fastcall  InsereDefeito(VTBarra *barra, int tipodefeito, int fases);
       TList*    __fastcall  LisDefeito(void);
       bool      __fastcall  RetiraDefeito(TDefeito *defeito);
       int       __fastcall  NumeroDeDefeitos(void);

   private: //dados
      TList      *lisDEF;
   };

//---------------------------------------------------------------------------
TDefeitos* __fastcall NewObjDefeitos(void);

#endif
//---------------------------------------------------------------------------
//eof