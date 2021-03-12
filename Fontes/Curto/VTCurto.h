//---------------------------------------------------------------------------
#ifndef VTCurtoH
#define VTCurtoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
enum enumCURTO {curtoCS, curtoMA};

//---------------------------------------------------------------------------
class  VTApl;
class  VTBarra;
class  VTCurtoCS;
class  VTCurtoMA;
class  VTData;
class  VTLigacao;
class  VTNet;
class  VTRedeCC;
class  VTTrecho;
struct strIMP;

//---------------------------------------------------------------------------
class VTCurto : public TObject
   {
   public:
                   __fastcall  VTCurto(void) {};
                   __fastcall ~VTCurto(void) {};
      virtual bool __fastcall  Executa(bool     aviso_enabled,
                                       int      ind_pat,
                                       VTData   *data,
                                       VTBarra  *barra,
                                       VTRedeCC *redeCC,
                                       VTTrecho *trecho=NULL,
                                       VTTrecho *trecho_fic1=NULL,
                                       VTTrecho *trecho_fic2=NULL) = 0;
      virtual bool __fastcall  Executa(int     tipo_def,
                                       strIMP  zdef_fase_ohm,
                                       strIMP  zdef_terra_ohm,
                                       VTBarra *barra) = 0;
      virtual bool __fastcall  Executa(int      tipo_def,
                                       strIMP   zdef_fase_ohm,
                                       strIMP   zdef_terra_ohm,
                                       VTTrecho *trecho,
                                       VTBarra  *bar_ref,
                                       double   dist_m) = 0;
      virtual bool __fastcall  Iamp(VTLigacao *ligacao,
                                    double    iamp[MAX_FASE+MAX_FASE]) = 0;
      virtual bool __fastcall  Inicia(VTNet *net,
                                      int   ind_pat) = 0;
      virtual bool __fastcall  Vfn_kv(VTBarra *barra,
                                      double  vkv[MAX_FASE]) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCurto
//---------------------------------------------------------------------------
VTCurto * __fastcall NewObjCurto(VTApl *apl);

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCurtoCS
//---------------------------------------------------------------------------
VTCurtoCS * __fastcall NewObjCurtoCS(VTApl *apl);

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCurtoMA
//---------------------------------------------------------------------------
VTCurtoMA * __fastcall NewObjCurtoMA(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 