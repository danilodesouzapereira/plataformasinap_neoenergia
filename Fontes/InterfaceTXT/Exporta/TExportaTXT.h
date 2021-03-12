//---------------------------------------------------------------------------
#ifndef TExportaTXTH
#define TExportaTXTH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaTXT.h"

//---------------------------------------------------------------------------
class VTFases;
class VTRede;
class VTExportaEqpto;

//---------------------------------------------------------------------------
class TExportaTXT : public VTExportaTXT
   {
   public:
           __fastcall  TExportaTXT(VTApl *apl);
           __fastcall ~TExportaTXT(void);
      bool __fastcall  ExportaArranjos(AnsiString arq_txt);
      bool __fastcall  ExportaCurvasTipicas(AnsiString arq_txt);
      bool __fastcall  ExportaEqptos(TList *lisEXT, AnsiString arq_txt);
      bool __fastcall  ExportaRedes(AnsiString arq_txt);
      bool __fastcall  ExportaUmaRede(VTRede *rede, AnsiString arq_txt);

   private: //métodos
      AnsiString __fastcall Cript(AnsiString &txt);
      bool       __fastcall ExportaArranjos(void);
	  bool       __fastcall ExportaCurvasTipicas(void);
      bool       __fastcall ExportaEqptos(TList *lisEXT);
      bool       __fastcall ExportaFormato(void);
      bool       __fastcall ExportaRedes(void);
      bool       __fastcall ExportaUmaRede(VTRede *rede);
      bool       __fastcall ExportaTipos(void);
      void       __fastcall IniciaObjAsIdArranjos(void);
      void       __fastcall IniciaObjAsIdCurvasTipicas(void);
      void       __fastcall IniciaObjAsIdCurvas(TList *lisCURVA);
      void       __fastcall IniciaObjAsIdEqptos(void);
      void       __fastcall IniciaObjAsIdEqptos(TList *lisEQP);
      void       __fastcall IniciaObjAsIdEqptos(VTRede *rede);
      void       __fastcall IniciaObjAsIdTipos(void);
      void       __fastcall InsereHeader(char *header);
      TList*     __fastcall LisEqpto(TList *lisORIG, int tipo_eqpto);
      TList*     __fastcall LisEqpto(VTRede *rede, int tipo_eqpto);
      int        __fastcall LisLigacao(TList *lisORIG, TList *lisDEST, VTRede *rede);
      bool       __fastcall MontaBlocoArranjoCaboPorfase(void);
      bool       __fastcall MontaBlocoArranjoCaboZ0Z1(void);
      bool       __fastcall MontaBlocoArranjoPUSB100(void);
      bool       __fastcall MontaBlocoArranjoZ0Z1(void);
      bool       __fastcall MontaBlocoCaboUnipolar(void);
      bool       __fastcall MontaBlocoCaboZ0Z1(void);
      bool       __fastcall MontaBlocoCriptografia(void);
      bool       __fastcall MontaBlocoCurvaTipica(TList *lisCURVA);
      bool       __fastcall MontaBlocoCurvaCapacitor(void);
      bool       __fastcall MontaBlocoCurvaDemanda(void);
      bool       __fastcall MontaBlocoCurvaGerador(void);
      bool       __fastcall MontaBlocoCurvaIluninacaoPublica(void);
      bool       __fastcall MontaBlocoCurvaReator(void);
      bool       __fastcall MontaBlocoDiagnostico(void);
	  bool       __fastcall MontaBlocoEqptoBarra(TList *lisEQP);
	  bool 		 __fastcall MontaBlocoEqptoBateria(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoCapacitor(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoCapserie(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoCargaIP(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoCargaComMedicao(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoCargaSemMedicao(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoChave(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoFiltro(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoGerador(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoMedidor(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoMutua(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoReator(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoRede(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoReguladorAutomatico(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoReguladorTapFixo(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoSuprimento(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrafo(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrafoMonofasico(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrafo3E(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrafoZZ(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrechoArranjoProprio(TList *lisEQP);
      bool       __fastcall MontaBlocoEqptoTrechoArranjoTipico(TList *lisEQP);
      bool       __fastcall MontaBlocoFimDados(void);
      bool       __fastcall MontaBlocoSuporte(void);
      bool       __fastcall MontaBlocoTipoChave(void);
      bool       __fastcall MontaBlocoTipoConsumidor(void);
      bool       __fastcall MontaBlocoTipoRede(void);
      bool       __fastcall MontaBlocoVersao(void);
      void       __fastcall OrdenaRedes(TList *lisREDE, TList *lisORDENADA);

   private: //objetos externos
      VTApl   *apl;
      VTFases *fases;

   private: //dados locais
      VTExportaEqpto *exporta_eqpto;
      TList          *lisREDE;
      TList          *lisLIG;
      TList          *lisEQP;
      TStrings       *lines;
   };

#endif
//-----------------------------------------------------------------------------
// eof


