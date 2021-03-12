//---------------------------------------------------------------------------
#ifndef TLinkH
#define TLinkH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTLink.h"

//---------------------------------------------------------------------------
class TLink : public VTLink
   {
   public:
           __fastcall  TLink(VTApl *apl_owner);
		   __fastcall ~TLink(void);
      bool __fastcall  AplicaEstilo(VTEstilo *estilo);
      bool __fastcall  Coordenadas_esq(double escala, int &x1, int &y1, int &x2, int &y2);
      bool __fastcall  Coordenadas_utm(double escala, int &x1, int &y1, int &x2, int &y2);
      void __fastcall  Move_esq(int dx, int dy);
	  void __fastcall  Move_utm(int dx, int dy);
      void __fastcall  SalvaInfoNoEstilo(void);

   protected:  //métodos acessados via property
      int  __fastcall PM_Tipo(void) {return(elemvFIGURA);};
      int __fastcall PM_TipoFigura(void) {return(figLINK);};

   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
   };

#endif
//---------------------------------------------------------------------------
//eof

