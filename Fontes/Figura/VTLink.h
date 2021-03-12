//---------------------------------------------------------------------------
#ifndef VTLinkH
#define VTLinkH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFigura.h"

//---------------------------------------------------------------------------
class VTApl;
class VTFiguraComentario;
class VTLink;
class VTEqpto;

//---------------------------------------------------------------------------
class VTLink : public VTFigura
   {
   public:  //property
      __property VTEqpto      *Eqpto       = {read=PD.eqpto,      write=PD.eqpto      };
	  __property VTFiguraComentario *Comentario  = {read=PD.comentario, write=PD.comentario };
      __property int          PosEqp       = {read=PD.pos_eqp,    write=PD.pos_eqp    };
      __property int          PosCom       = {read=PD.pos_com,    write=PD.pos_com    };
	  __property int          BordaTipo    = {read=PD.bordaTipo,  write=PD.bordaTipo     };
      __property int          Espessura    = {read=PD.espessura,  write=PD.espessura  };
      __property TColor       Color        = {read=PD.color,      write=PD.color      };

   public:
				   __fastcall  VTLink(void){};
                   __fastcall ~VTLink(void){};
      virtual bool __fastcall  Coordenadas_esq(double escala, int &x1, int &y1, int &x2, int &y2) = 0;
      virtual bool __fastcall  Coordenadas_utm(double escala, int &x1, int &y1, int &x2, int &y2) = 0;

   protected:
      struct{
            VTEqpto      *eqpto;
            VTFiguraComentario *comentario;
            int           pos_eqp;
            int           pos_com;
            int           bordaTipo;
            int           espessura;
            TColor        color;
            }PD;
   };

//---------------------------------------------------------------------------
VTLink* __fastcall NewObjLink(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof

