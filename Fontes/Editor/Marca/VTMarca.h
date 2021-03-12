//---------------------------------------------------------------------------
#ifndef VTMarcaH
#define VTMarcaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTEqbar;
class VTLigacao;

//---------------------------------------------------------------------------
class VTMarca : public TObject
   {
   public:
                       __fastcall  VTMarca(void) {};
                       __fastcall ~VTMarca(void) {};
      virtual VTBarra* __fastcall  BarraDaMarca(void) = 0;
      virtual bool     __fastcall  DeBarra(void) {return(false);};
      virtual void     __fastcall  CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2) = 0;
      virtual bool     __fastcall  CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2) = 0;
      virtual int      __fastcall  Dimensao(double escala) = 0;
      virtual void     __fastcall  Move(int coord_x, int coord_y, bool tecla_shift, VTBarra *pbar_nova, bool moving) = 0;
      virtual void     __fastcall  StartMove(void) {};
      virtual void     __fastcall  StopMove(void) {};

   public: //dados
      #define DIM_MARCA  8
      //última Barra que foi alcançada pela Marca durante operação de move
      VTBarra *barra_alvo;
   };

//---------------------------------------------------------------------------
//funções globais p/ criar objetos da classe VTmarca
//---------------------------------------------------------------------------
VTMarca* __fastcall NewObjMarcaBar(VTApl *apl, VTBarra *pbar, bool ref_x1y1);
VTMarca* __fastcall NewObjMarcaEqbar(VTApl *apl, VTEqbar *eqbar, VTBarra *pbar);
VTMarca* __fastcall NewObjMarcaLiga(VTApl *apl, VTLigacao *pliga, VTBarra *pbar);

#endif
//---------------------------------------------------------------------------
//eof

