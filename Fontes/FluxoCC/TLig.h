//---------------------------------------------------------------------------
#ifndef TLigH
#define TLigH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TBar;

//---------------------------------------------------------------------------
class TLig : public TObject
   {
   public:
           __fastcall  TLig(void *eqpto,
                            TBar *bar1,
                            TBar *bar2);
           __fastcall ~TLig(void);
      void __fastcall  InsereLigacao(double bser);

   public:  //dados
      void   *eqpto;
      TBar   *bar1;
      TBar   *bar2;
      double bser;      //suscept�ncia em pu
      double pflux;     //fluxo de pot�ncia ativa em pu
      struct   {
               double fluxo12_pu;
               } res;
   };

//---------------------------------------------------------------------------
#endif
//eof

