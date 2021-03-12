
//---------------------------------------------------------------------------
#ifndef VTCurtoCSH
#define VTCurtoCSH

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
class VTCurtoCS : public TObject
   {
   public:  //property
      __property bool AvisoEnabled  = {read=PD.aviso_enabled,  write=PD.aviso_enabled};
      __property int  TipoRelatorio = {read=PD.tipo_relatorio, write=PD.tipo_relatorio};

   public:
                   __fastcall  VTCurtoCS(void) {};
                   __fastcall ~VTCurtoCS(void) {};
      virtual bool __fastcall  Curto_3F_Max(VTData *data,
                                            int    *id_barra_sinap,
                                            double *idef_3F_ka) = 0;
      virtual bool __fastcall  Executa1(int      ind_pat,
                                        VTData   *data,
                                        VTBarra  *barra,
                                        VTNet    *net,
                                        VTRedeCC *redeCC,
                                        VTTrecho *trecho=NULL,
                                        VTTrecho *trecho_fic1=NULL,
                                        VTTrecho *trecho_fic2=NULL) = 0;
      virtual bool __fastcall  Executa2(int     tipo_def,
                                        strIMP  zdef_fase_ohm,
                                        strIMP  zdef_terra_ohm,
                                        VTBarra *barra) = 0;
      virtual bool __fastcall  Executa3(int      tipo_def,
                                        strIMP   zdef_fase_ohm,
                                        strIMP   zdef_terra_ohm,
                                        VTTrecho *trecho,
                                        VTBarra  *barra_ref,
                                        double   dist_m) = 0;
      virtual bool __fastcall  Iamp(VTLigacao       *ligacao,
                                    complex<double> iamp[MAX_FASE+MAX_FASE]) = 0;
      virtual bool __fastcall  IampBarDef(double iamp[MAX_FASE],
                                          double assim_iamp[MAX_FASE]) = 0;
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
//função global p/ criar objeto da classe VTCurtoCS
//---------------------------------------------------------------------------
VTCurtoCS * __fastcall NewObjCurtoCS(VTApl *apl);
//---------------------------------------------------------------------------
#endif
//eof



 