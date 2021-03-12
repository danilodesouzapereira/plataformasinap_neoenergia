
//---------------------------------------------------------------------------
#ifndef VTCurtoMAH
#define VTCurtoMAH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class  VTApl;
class  VTBarra;
class  VTData;
class  VTLigacao;
class  VTNet;
class  VTRedeCC;
class  VTTrecho;
struct strIMP;

//---------------------------------------------------------------------------
class VTCurtoMA : public TObject
   {
   public:  //property
      __property bool AvisoEnabled  = {read=PD.aviso_enabled,  write=PD.aviso_enabled};
      __property int  TipoRelatorio = {read=PD.tipo_relatorio, write=PD.tipo_relatorio};

   public:
                   __fastcall  VTCurtoMA(void) {};
                   __fastcall ~VTCurtoMA(void) {};
      virtual bool __fastcall  Executa(int      ind_pat,
                                       VTData   *data,
                                       VTBarra  *barra,
                                       VTNet    *net,
                                       VTRedeCC *redeCC) = 0;
      virtual bool __fastcall  Executa(int      ind_pat,
                                       VTData   *data,
                                       TList    *lisDEF,
                                       VTNet    *net,
                                       VTRedeCC *redeCC) = 0;
      virtual bool __fastcall  Executa(int     tipo_def,
                                       strIMP  zdef_fase_ohm,
                                       strIMP  zdef_terra_ohm,
                                       VTBarra *barra) = 0;
      virtual bool __fastcall  Executa(int      tipo_def,
                                       strIMP   zdef_fase_ohm,
                                       strIMP   zdef_terra_ohm,
                                       VTTrecho *trecho,
                                       VTBarra  *barra_ref,
                                       double   dist_m) = 0;
      virtual bool __fastcall  Iamp(VTLigacao       *ligacao,
                                    complex<double> iamp[MAX_FASE+MAX_FASE]) = 0;
      virtual bool __fastcall  Inicia(VTData *data,
                                      VTNet  *net,
                                      int    ind_pat) = 0;
      virtual bool __fastcall  Vfn_kv(VTBarra         *barra,
                                      complex<double> vkv[MAX_FASE]) = 0;

   protected:  //dados acessados via property
      struct   {
               bool aviso_enabled;
               int  tipo_relatorio;
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCurtoMA
//---------------------------------------------------------------------------
VTCurtoMA * __fastcall NewObjCurtoMA(VTApl *apl);
//---------------------------------------------------------------------------
#endif
//eof



 