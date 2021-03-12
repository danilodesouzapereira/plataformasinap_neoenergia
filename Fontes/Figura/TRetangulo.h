//---------------------------------------------------------------------------
#ifndef TRetanguloH
#define TRetanguloH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRetangulo.h"

//---------------------------------------------------------------------------
class TRetangulo : public VTRetangulo
   {
   public:
           __fastcall  TRetangulo(VTApl *apl_owner);
		   __fastcall ~TRetangulo(void);
      bool __fastcall  AplicaEstilo(VTEstilo *estilo);
      bool __fastcall  Coordenadas_esq(double escala, int &x1, int &y1, int &x2, int &y2);
      bool __fastcall  Coordenadas_utm(double escala, int &x1, int &y1, int &x2, int &y2);
      void __fastcall  GetCentro(strUTM &utm, strCOORD &esq);
      void __fastcall  GetDimensao(int &dx, int &dy);
      void __fastcall  Move_esq(int dx, int dy);
      void __fastcall  Move_utm(int dx, int dy);
      bool __fastcall  Ponto_esq(int pos_index, double escala, int &x, int &y);
	  bool __fastcall  Ponto_utm(int pos_index, double escala, int &x, int &y);
	  void __fastcall  SalvaInfoNoEstilo(void);
	  bool __fastcall  SalvaValorFormaAtributo(VTEstilos *estilos);
      void __fastcall  SetCentro(strUTM &utm, strCOORD &esq);
      void __fastcall  SetDimensao(int dx, int dy);

   protected:  //métodos acessados via property
	  int  __fastcall PM_Tipo(void) {return(elemvFIGURA);};
      int __fastcall PM_TipoFigura(void) {return(figRETANGULO);};

   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      int   width, heigth;
      struct{
            strUTM   utm;
            strCOORD esq;
            }centro;
      struct{//dx,dy definem distância do centro às bordas
            int dx;
            int dy;
            }dim;
   };

#endif
//---------------------------------------------------------------------------
//eof

