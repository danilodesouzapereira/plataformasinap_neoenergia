//---------------------------------------------------------------------------
#ifndef VTEstimaEstadoH
#define VTEstimaEstadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum tipoESTIMA {estimaEQUILIBRADO=1, estimaDESEQUILIBRADO};

//---------------------------------------------------------------------------
enum tipoGRUPO {grupoUNICO=1, grupoSUBESTACAO, grupoPRIMARIO, grupoINDEPENDENTE};

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTEstimaEstado : public TObject
   {
   public:  //property
      __property int  Patamar           = {read=PD.patamar,             write=PD.patamar            };
      __property int  MetodoEE          = {read=PD.metodo_ee,           write=PD.metodo_ee          };
      __property int  NumMaxMontH       = {read=PD.num_max_mont_h,      write=PD.num_max_mont_h};
      __property int  OrdenacaoLinhasQR = {read=PD.ordenacao_linhas_QR, write=PD.ordenacao_linhas_QR};
      __property int  Tipo              = {read=PD.tipo_estimacao,      write=PD.tipo_estimacao     };
      __property int  TipoBarraRef      = {read=PD.tipo_bar_ref,        write=PD.tipo_bar_ref       };
      __property int  TipoGrupo         = {read=PD.tipo_grupo,          write=PD.tipo_grupo         };
      __property bool RelFluxoPotencia  = {read=PD.rel_fluxo_potencia,  write=PD.rel_fluxo_potencia };
      __property bool PerdaFerroEnabled = {read=PD.perda_ferro_enabled, write=PD.perda_ferro_enabled};
      __property bool UtilizaVDNM       = {read=PD.utiliza_vdnm,        write=PD.utiliza_vdnm};

   public:
                   __fastcall  VTEstimaEstado(void) {};
      virtual      __fastcall ~VTEstimaEstado(void) {};
      virtual bool __fastcall  Executa(void) = 0;

   protected:
      struct{
            int  patamar;
            int  metodo_ee;
            int  num_max_mont_h;
            int  ordenacao_linhas_QR;
            int  tipo_estimacao;
            int  tipo_bar_ref;
            int  tipo_grupo;
            bool rel_fluxo_potencia;
            bool perda_ferro_enabled;
            bool utiliza_vdnm;
            }PD;
   };

//---------------------------------------------------------------------------
VTEstimaEstado* __fastcall NewObjEstimaEstado(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
