//-----------------------------------------------------------------------------
#ifndef VTCcs_H
#define VTCcs_H

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTSubestacao;
class VTPoligono;

//-----------------------------------------------------------------------------
class VTCcs : public TObject
   {
   public:  //property
      __property VTSubestacao* Subestacao = {read=PD.subestacao,   write=PD.subestacao};
      __property VTPoligono*   Poligono   = {read=PD.poligono                         };
      __property TColor        Color      = {read=PD.color     , write=PD.color       };
      __property bool          Visible     = {read=PD.visible  , write=PD.visible     };

   public:
                      __fastcall  VTCcs(void) {};
                      __fastcall ~VTCcs(void){};
      virtual bool    __fastcall  CoordenadasCentroCarga(int &x_cm, int &y_cm) = 0;
      virtual TList * __fastcall  LisCelula(void) = 0;
      virtual TList * __fastcall  LisPtoCarga(void) = 0;
      virtual bool    __fastcall  MontaCentroCarga(int ind_pat) = 0;
      virtual void    __fastcall  PintaCelulas(TColor color, bool merge=true) = 0;

   protected:  //dados acessados via property
      struct   {
               VTSubestacao *subestacao;
               VTPoligono   *poligono;
               bool          visible;
               TColor        color;
               }PD;
    };

//-----------------------------------------------------------------------------
//função global
VTCcs* __fastcall NewObjCcs(VTApl *apl, VTSubestacao *subestacao);

#endif
//---------------------------------------------------------------------------
//eof
