//---------------------------------------------------------------------------
#ifndef TFluxoCCH
#define TFluxoCCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Fluxo\VTFluxo.h>

//---------------------------------------------------------------------------
class VTData;
class VTFPcc;
class VTMontaData;
class VTNet;

//---------------------------------------------------------------------------
class TFluxoCC : public VTFluxo
   {
   public:
                 __fastcall  TFluxoCC(VTApl *apl_owner);
                 __fastcall ~TFluxoCC(void);
      void       __fastcall  CfgAvisoErro(bool enabled);
      bool       __fastcall  Executa(VTNet      *net,
                                     double     Sbase,
                                     AnsiString arqRelFlow,
                                     AnsiString arqRelRede);
      AnsiString __fastcall Modelo(void);
      bool       __fastcall UtilizaBarraPV(void);
		bool       __fastcall ValidaAderencia(VTNet *net,
                                            VTLog *plog);

   protected:  //métodos acessados via property
      bool       __fastcall  PM_GetEquilibrado(void);

   private: //métodos
      bool __fastcall Executa(VTNet *net,
                              int   ind_pat);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      AnsiString NomeRede;
      struct {
             bool enabled;
             } aviso, relatorio;
      VTFPcc      *fpcc;
      VTData      *data;
      VTMontaData *monta;
   };

#endif
//---------------------------------------------------------------------------
//eof
