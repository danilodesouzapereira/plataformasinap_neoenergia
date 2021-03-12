//---------------------------------------------------------------------------
#ifndef VTEstimaDemandaH
#define VTEstimaDemandaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
enum tipoESTIMACAO {estimacaoEQUILIBRADA=1, estimacaoDESEQUILIBRADA};

//---------------------------------------------------------------------------
enum tipoREDUCAO {reducaoNENHUMA=1, reducaoZIP, reducaoEQV};

//---------------------------------------------------------------------------
class VTEstimaDemanda : public TObject
   {
   public:  //property
      __property bool   LogEnabled         = {read=PD.log_enabled,          write=PD.log_enabled};
      __property bool   SalvaCurvaEstimada = {read=PD.salva_curva_estimada, write=PD.salva_curva_estimada};
      __property bool   UtilizaVDNM        = {read=PD.utiliza_vdnm,         write=PD.utiliza_vdnm};
      __property int    HoraReferencia     = {read=PD.hora_referencia,      write=PM_SetHoraReferencia};
      __property int    HorasAfetadas      = {read=PD.horas_afetadas,       write=PD.horas_afetadas};
      __property int    MetodoEE           = {read=PD.metodo_ee,            write=PD.metodo_ee};
      __property int    NumMaxMontH        = {read=PD.num_max_mont_h,       write=PD.num_max_mont_h};
      __property int    OrdenacaoLinhasQR  = {read=PD.ordenacao_linhas_QR,  write=PD.ordenacao_linhas_QR};
      __property int    TipoEstimacao      = {read=PD.tipo_estimacao,       write=PD.tipo_estimacao};
      __property int    TipoReducao        = {read=PD.tipo_reducao,         write=PD.tipo_reducao};
      __property int    TipoRelatorio      = {read=PD.tipo_relatorio,       write=PD.tipo_relatorio};
      __property double FatMultMedChave    = {read=PD.fat_mult_med_chave,   write=PD.fat_mult_med_chave};

   public:
                        __fastcall  VTEstimaDemanda(void) {};
                        __fastcall ~VTEstimaDemanda(void) {};
      virtual bool      __fastcall  ExecutaPorRedePrimaria(VTRedes *redes) = 0;
      virtual bool      __fastcall  ExecutaRedeUnica(VTRedes *redes) = 0;
      virtual TStrings* __fastcall  StrLisRedeEstimada(void) = 0;
      virtual TStrings* __fastcall  StrLisRedeNaoEstimada(void) = 0;

   protected:  //métodos acessados via property
      virtual void __fastcall PM_SetHoraReferencia(int hora) = 0;

   protected:  //dados acessados via property
      struct{
            bool   log_enabled;
            bool   salva_curva_estimada;
            bool   utiliza_vdnm;
            int    hora_referencia;
            int    horas_afetadas;
            int    metodo_ee;
            int    num_max_mont_h;
            int    ordenacao_linhas_QR;
            int    tipo_estimacao;
            int    tipo_reducao;
            int    tipo_relatorio;
            double fat_mult_med_chave;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEstimaDemanda
//---------------------------------------------------------------------------
VTEstimaDemanda* __fastcall NewObjEstimaDemanda(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



 