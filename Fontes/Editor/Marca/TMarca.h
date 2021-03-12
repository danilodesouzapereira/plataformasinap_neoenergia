//---------------------------------------------------------------------------
#ifndef TMarcaH
#define TMarcaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMarca.h"

//---------------------------------------------------------------------------
class VTEdita;
class VTRedegraf;

//---------------------------------------------------------------------------
class TMarcaBar : public VTMarca
   {
   public:
               __fastcall  TMarcaBar(VTApl *apl, VTBarra *pbar, bool ref_x1y1);
			   __fastcall ~TMarcaBar(void);
      VTBarra* __fastcall  BarraDaMarca(void);
      bool     __fastcall  DeBarra(void);
	  void     __fastcall  CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2);
      bool     __fastcall  CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2);
      int      __fastcall  Dimensao(double escala);
      void     __fastcall  Move(int coord_x, int coord_y, bool tecla_shift, VTBarra *pbar_nova, bool moving);

   private: //dados
	  VTEdita    *edita;
	  VTRedegraf *redegraf;
      VTBarra    *pbar;
	  struct {
			 bool x1y1; //true indica que a Marca está associada às coord(x1,y1) da Barra
			 } ref;
   };

//---------------------------------------------------------------------------
class TMarcaEqbar : public VTMarca
   {
   public:
               __fastcall  TMarcaEqbar(VTApl *apl, VTEqbar *eqbar, VTBarra *pbar);
               __fastcall ~TMarcaEqbar(void);
      VTBarra* __fastcall  BarraDaMarca(void);
      void     __fastcall  CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2);
      bool     __fastcall  CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2);
      int      __fastcall  Dimensao(double escala);
      void     __fastcall  Move(int coord_x, int coord_y, bool tecla_shift, VTBarra *pbar_nova, bool moving);

   private: //dados
      VTEdita    *edita;
      VTRedegraf *redegraf;
      VTEqbar    *eqbar;
      VTBarra    *bar_orig;
      VTBarra    *bar_dest;
      VTBarra    *bar_tmp;
   };

//---------------------------------------------------------------------------
class TMarcaLiga : public VTMarca
   {
   public:
               __fastcall  TMarcaLiga(VTApl *apl, VTLigacao *pliga, VTBarra *pbar);
               __fastcall ~TMarcaLiga(void);
      VTBarra* __fastcall  BarraDaMarca(void);
      void     __fastcall  CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2);
      bool     __fastcall  CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2);
      int      __fastcall  Dimensao(double escala);
      void     __fastcall  Move(int coord_x, int coord_y, bool tecla_shift, VTBarra *pbar_nova, bool moving);
      void     __fastcall  StartMove(void);
      void     __fastcall  StopMove(void);

   private:
	  bool     __fastcall VerificaSeFechaMalhaExterna(VTLigacao *lig);

   private: //dados
      VTApl      *apl;
      VTEdita    *edita;
      VTRedegraf *redegraf;
      VTLigacao  *ligacao;
      VTBarra    *bar_orig;
      VTBarra    *bar_dest;
      VTBarra    *bar_tmp;
   };

#endif
//---------------------------------------------------------------------------
//eof

