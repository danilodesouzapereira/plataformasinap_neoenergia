//---------------------------------------------------------------------------
#ifndef VTEstimadorH
#define VTEstimadorH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTData;

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigos_barra_ref[]', em TImpressao.cpp
enum enum_BARRA_REF {brFIXOS_1THETA_0V=0, brFIXOS_1THETA_1V, brFIXOS_nTHETA_0V,
                     brFIXOS_nTHETA_nV};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigos_metodo_EE[]', em TImpressao.cpp
enum enum_METODO_EE {meeCONVENCIONAL=0, meeQR_FAST_GIVENS, meeQR_GIVENS,
                     meeQR_HOUSEHOLDER};

//---------------------------------------------------------------------------
class VTEstimador : public TObject
   {
   public:  //property
      __property bool   LimitaSigmas      = {read  = PD.limita_sigmas,
                                             write = PD.limita_sigmas};
      __property bool   UtilizaVDNM       = {read  = PD.utiliza_vdnm,
                                             write = PD.utiliza_vdnm};
      __property int    MetodoEE          = {read  = PD.metodo_ee,
                                             write = PD.metodo_ee};
      __property int    NumMaxMontH       = {read  = PD.num_max_mont_h,
                                             write = PD.num_max_mont_h};
      __property int    OrdenacaoLinhasQR = {read  = PD.ordenacao_linhas_QR,
                                             write = PD.ordenacao_linhas_QR};
      __property int    TipoBarraRef      = {read  = PD.tipo_barra_ref,
                                             write = PD.tipo_barra_ref};
      __property int    TipoRelatorio     = {read  = PD.tipo_relatorio,
                                             write = PD.tipo_relatorio};
      __property double FatMultMedChave   = {read  = PD.fat_mult_med_chave,
                                             write = PD.fat_mult_med_chave};
      __property double SigmaMinimoPu     = {read  = PD.sigma_min_pu,
                                             write = PD.sigma_min_pu};
      __property double SigmaMaximoPu     = {read  = PD.sigma_max_pu,
                                             write = PD.sigma_max_pu};

   public:
                   __fastcall  VTEstimador(void) {};
      virtual      __fastcall ~VTEstimador(void) {};
      virtual bool __fastcall  Executa(VTData *data) = 0;

   protected:  // Dados acessados via property
      struct   {
               bool   limita_sigmas;
               bool   utiliza_vdnm;
               int    metodo_ee;
               int    num_max_mont_h;
               int    ordenacao_linhas_QR;
               int    tipo_barra_ref;
               int    tipo_relatorio;
               double fat_mult_med_chave;
               double sigma_min_pu;
               double sigma_max_pu;
               }PD;
   };

//---------------------------------------------------------------------------
VTEstimador * __fastcall NewObjEstimador1(VTApl *apl);
VTEstimador * __fastcall NewObjEstimador3(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
