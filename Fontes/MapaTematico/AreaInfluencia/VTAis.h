//-----------------------------------------------------------------------------
#ifndef VTAis_H
#define VTAis_H

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTPoligono;
class VTApl;
class VTSubestacao;

//-----------------------------------------------------------------------------
struct strOPCAO_AIS  {
                     bool   sfirme;
                     bool   carga_propria;
                     int    ind_pat;
                     double cargto_max_pu;
                     };

//-----------------------------------------------------------------------------
class VTAis : public TObject
   {
   public:  //property
      __property VTSubestacao* Subestacao = {read=PD.subestacao,   write=PD.subestacao};
      __property VTPoligono*   Poligono   = {read=PD.poligono                         };
      __property TColor        Color      = {read=PD.color     , write=PD.color       };
      __property bool          Visible    = {read=PD.visible  , write=PD.visible     };

   public:
                      __fastcall  VTAis(void) {};
                      __fastcall ~VTAis(void){};
      virtual double  __fastcall  DemandaAtendida_mva(void) = 0;
      virtual double  __fastcall  DemandaLimite_mva(void) = 0;
      virtual TList * __fastcall  LisCelula(void) = 0;
      virtual TList * __fastcall  LisPtoCarga(void) = 0;
      virtual bool    __fastcall  MontaAreaInfluencia(strOPCAO_AIS &opcao) = 0;
      virtual void    __fastcall  PintaCelulas(TColor color, bool merge=true) = 0;

   protected:  //dados acessados via property
      struct   {
               VTSubestacao *subestacao;
               VTPoligono   *poligono;
               bool          visible;
               TColor        color;
               double        s_mva;
               double        s_max_mva;
               }PD;
    };

//-----------------------------------------------------------------------------
//função global
VTAis* __fastcall NewObjAis(VTApl *apl, VTSubestacao *subestacao);

#endif
//---------------------------------------------------------------------------
//eof
