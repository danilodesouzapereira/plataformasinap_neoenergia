//---------------------------------------------------------------------------
#ifndef VTFluxoH
#define VTFluxoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTNet;
class VTLog;

//---------------------------------------------------------------------------
class VTFluxo : public TObject
   {
   public:  //property
      __property bool   Equilibrado       = {read=PM_GetEquilibrado                                    };
      __property bool   RedeComCarga      = {read=PD.rede_com_carga,      write=PD.rede_com_carga      };
      __property int    ModeloCarga       = {read=PD.modelo_carga,        write=PD.modelo_carga        };
      __property bool   PerdaFerroEnabled = {read=PD.perda_ferro_enabled, write=PD.perda_ferro_enabled };
      __property int    TipoRelatorio     = {read=PD.tipo_relatorio,      write=PD.tipo_relatorio      };
      __property bool   RelatorioBarDesc  = {read=PD.relatorio_bar_desc,  write=PD.relatorio_bar_desc  };
      __property int    NumPatamaresOK    = {read=PD.num_patamares_ok                                  };

   public:
                         __fastcall  VTFluxo(void) {};
                         __fastcall ~VTFluxo(void) {};
      virtual void       __fastcall  CfgAvisoErro(bool enabled) = 0;
      virtual bool       __fastcall  Executa(VTNet *net, double Sbase,
                                             AnsiString arqRelFlow, AnsiString arqRelRede) = 0;
      virtual AnsiString __fastcall  Modelo(void) = 0;
      virtual bool       __fastcall  UtilizaBarraPV(void) = 0;
      virtual bool       __fastcall  ValidaAderencia(VTNet *net, VTLog *plog) = 0;

   protected:  //métodos acessados via property
      virtual bool       __fastcall  PM_GetEquilibrado(void) = 0;

   protected:  //dados acessados via property
      struct   {
               bool rede_com_carga;
               int  modelo_carga;
               bool perda_ferro_enabled;
               int  tipo_relatorio;
               bool relatorio_bar_desc;
               int  num_patamares_ok;
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxo
//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxoBarra(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoCC(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoG1(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoG3(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoG3_AS_NR3(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoH3(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoMRT(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoNR1(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoNR3(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoRadial1(VTApl *apl);
VTFluxo* __fastcall NewObjFluxoRadial3(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 