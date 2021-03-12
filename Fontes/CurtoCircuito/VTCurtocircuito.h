//---------------------------------------------------------------------------
#ifndef VTCurtocircuitoH
#define VTCurtocircuitoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTNet;
class VTTrecho;
class VTRedes;
class VTRedeCC;
struct STR_ZDef;

//---------------------------------------------------------------------------
class VTCurtocircuito : public TObject
   {
   public:  //property
      __property int      Algoritmo        = {read=PD.algoritmo,         write=PD.algoritmo};
      __property bool     PreFalta         = {read=PD.pre_falta,         write=PD.pre_falta};
      __property bool     AvisoEnabled     = {read=PD.aviso_enabled,     write=PD.aviso_enabled};
      __property bool     CronoEnabled     = {read=PD.crono_enabled,     write=PD.crono_enabled};
      __property int      TipoRelatorio    = {read=PD.tipo_relatorio,    write=PD.tipo_relatorio};
      __property bool     DefeitoIsolado   = {read=PD.defeito_isolado};
      __property VTApl    *Apl             = {read=PM_GetApl};
      __property VTRedeCC *RedeCC          = {read=PM_GetRedeCC};

   public:
                   __fastcall  VTCurtocircuito(void) {};
                   __fastcall ~VTCurtocircuito(void) {};
      virtual bool __fastcall  Executa(VTRedes *redes, STR_ZDef &Zdef, bool rede_alterada, VTBarra *barra, int ind_pat, bool pre_falta) = 0;
      virtual bool __fastcall  Executa(VTRedes *redes, STR_ZDef &Zdef, bool rede_alterada, VTTrecho *trecho, double dist_bar1_km, int ind_pat, bool pre_falta) = 0;
      virtual bool __fastcall  ExecutaCurto3FMax(VTNet *net,
                                                 int    *id_barra_sinap,
                                                 double *idef_3F_ka) = 0;
      virtual bool __fastcall  ExecutaCurtoSimultaneo(VTRedes *redes, STR_ZDef &Zdef, bool rede_alterada, TList *lisDEF, int ind_pat, bool pre_falta) = 0;

   protected:  //métodos acessados via property
      virtual VTApl*    __fastcall PM_GetApl(void) = 0;
      virtual VTRedeCC* __fastcall PM_GetRedeCC(void) = 0;

   protected:  //dados acessados via property
      struct   {
               int  algoritmo;
               bool pre_falta;
               bool aviso_enabled;
               bool crono_enabled;
               bool defeito_isolado;
               int  tipo_relatorio;
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCurtocircuito
//---------------------------------------------------------------------------
VTCurtocircuito* __fastcall NewObjCurtocircuito(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



 