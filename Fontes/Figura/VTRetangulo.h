//---------------------------------------------------------------------------
#ifndef VTRetanguloH
#define VTRetanguloH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFigura.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEstilos;
//---------------------------------------------------------------------------
class VTRetangulo : public VTFigura
   {
   public:  //property
      __property int    BordaEstilo     = {read=PD.borda_estilo,    write=PD.borda_estilo};
      __property int    BordaEspessura  = {read=PD.borda_espessura, write=PD.borda_espessura};
      __property TColor BordaCor        = {read=PD.borda_cor,       write=PD.borda_cor};
      __property TColor FundoCor        = {read=PD.fundo_cor,       write=PD.fundo_cor};
      __property bool   Transparente    = {read=PD.transparente,    write=PD.transparente};

   public:
				   __fastcall  VTRetangulo(void){};
				   __fastcall ~VTRetangulo(void){};
      virtual bool __fastcall  Coordenadas_esq(double escala, int &x1, int &y1, int &x2, int &y2) = 0;
      virtual bool __fastcall  Coordenadas_utm(double escala, int &x1, int &y1, int &x2, int &y2) = 0;
      virtual void __fastcall  GetCentro(strUTM &utm, strCOORD &esq) = 0;
      virtual void __fastcall  GetDimensao(int &dx, int &dy) = 0;
      virtual void __fastcall  Move_esq(int dx, int dy) = 0;
      virtual void __fastcall  Move_utm(int dx, int dy) = 0;
      virtual bool __fastcall  Ponto_esq(int pos_index, double escala, int &x, int &y) = 0;
	  virtual bool __fastcall  Ponto_utm(int pos_index, double escala, int &x, int &y) = 0;
	  virtual bool __fastcall  SalvaValorFormaAtributo(VTEstilos *estilos)=0;
      virtual void __fastcall  SetCentro(strUTM &utm, strCOORD &esq) = 0;
      virtual void __fastcall  SetDimensao(int dx, int dy) = 0;


   protected:
      struct{
            bool   transparente;
            int    borda_estilo;
            int    borda_espessura;
            TColor borda_cor;
            TColor fundo_cor;
            }PD;
   };

//---------------------------------------------------------------------------
VTRetangulo* __fastcall NewObjRetangulo(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof

