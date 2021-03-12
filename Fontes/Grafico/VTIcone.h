//---------------------------------------------------------------------------
#ifndef VTIconeH
#define VTIconeH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum tipoFORMA {formaLINHA=1, formaELIPSE=2, formaARCO=3};

//---------------------------------------------------------------------------
class TForma
   {
   public:
      __fastcall  TForma(void) {};
      __fastcall ~TForma(void) {};

   public:  //dados
      int    tipo;
      int    x1, y1, x2, y2;
      int    x3, y3, x4, y4;
      int    width;
      TColor cor;
   };

//---------------------------------------------------------------------------
class VTIcone : public TObject
   {
   public:
                   __fastcall  VTIcone(void) {};
      virtual      __fastcall ~VTIcone(void) {};
      virtual void __fastcall  InsereArco(int x1, int y1, int x2, int y2,
                                          int x3, int y3, int x4, int y4, int pen_width, TColor cor)= 0;
      virtual void __fastcall  InsereElipse(int x1, int y1, int x2, int y2, int pen_width, TColor cor)= 0;
      virtual void __fastcall  InsereLinha(int x1, int y1, int x2, int y2, int pen_width, TColor cor)= 0;

   public: //dados
      AnsiString eqpto_cod;
      int        eqpto_id, index;
      int        height, width;
      TColor     cor_fundo;
      TList      *lisFORMA;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTIcone
//---------------------------------------------------------------------------
VTIcone* __fastcall NewObjIcone(void);

//---------------------------------------------------------------------------
#endif
//eof



 