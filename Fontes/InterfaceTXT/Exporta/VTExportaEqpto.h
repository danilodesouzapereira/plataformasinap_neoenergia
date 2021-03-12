//---------------------------------------------------------------------------
#ifndef VTExportaEqptoH
#define VTExportaEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TLine.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTBarra;
class VTBateria;
class VTCabo;
class VTCanal;
class VTCapacitor;
class VTCapserie;
class VTCarga;
class VTChave;
class VTClasse;
class VTCurva;
class VTFases;
class VTFiltro;
class VTGerador;
class VTMedidor;
class VTMutua;
class VTReator;
class VTRede;
class VTRegulador;
class VTSuporte;
class VTSuprimento;
class VTTipoChave;
class VTTipoRede;
class VTTrafo;
class VTTrafo3E;
class VTTrecho;
class VTTrafoZZ;

//---------------------------------------------------------------------------
class VTExportaEqpto : public TObject
   {
   public:
      __property char Separador = {read=PD.separador,   write=PD.separador};
      __property int  Versao    = {read=PD.versao,      write=PD.versao};

   public:
                         __fastcall  VTExportaEqpto(void) {};
                         __fastcall ~VTExportaEqpto(void) {};
      virtual AnsiString __fastcall  MontaArranjoCaboPorFase(VTArranjo *arranjo) = 0;
      virtual AnsiString __fastcall  MontaArranjoCaboZ0Z1(VTArranjo *arranjo) = 0;
      virtual AnsiString __fastcall  MontaArranjoZ0Z1(VTArranjo *arranjo) = 0;
      virtual AnsiString __fastcall  MontaArranjoPUSb100(VTArranjo *arranjo) = 0;
	  virtual AnsiString __fastcall  MontaBarra(VTBarra *barra) = 0;
	  virtual AnsiString __fastcall  MontaBateria(VTBateria *bateria) = 0;
      virtual AnsiString __fastcall  MontaCaboUnipolar(VTCabo *cabo) = 0;
      virtual AnsiString __fastcall  MontaCaboZ0Z1(VTCabo *cabo) = 0;
      virtual AnsiString __fastcall  MontaCapacitor(VTCapacitor *capacitor) = 0;
      virtual AnsiString __fastcall  MontaCapserie(VTCapserie *capserie) = 0;
      virtual AnsiString __fastcall  MontaCargaComMedicao(VTCarga *carga) = 0;
      virtual AnsiString __fastcall  MontaCargaIP(VTCarga *carga) = 0;
      virtual AnsiString __fastcall  MontaCargaSemMedicao(VTCarga *carga) = 0;
      virtual AnsiString __fastcall  MontaChave(VTChave *chave) = 0;
      virtual AnsiString __fastcall  MontaCurvaTipica(VTCurva *curva) = 0;
	  virtual AnsiString __fastcall  MontaDiagnostico(double limites[27]) = 0;
	  virtual AnsiString __fastcall  MontaFiltro(VTFiltro *filtro) = 0;
      virtual AnsiString __fastcall  MontaGerador(VTGerador *gerador) = 0;
      virtual AnsiString __fastcall  MontaMedidor(VTMedidor *medidor) = 0;
	  virtual AnsiString __fastcall  MontaMedidorCanal(VTCanal *canal) = 0;
      virtual AnsiString __fastcall  MontaMutua(VTMutua *mutua) = 0;
      virtual AnsiString __fastcall  MontaReator(VTReator *reator) = 0;
      virtual AnsiString __fastcall  MontaRede(VTRede *rede) = 0;
      virtual AnsiString __fastcall  MontaReguladorAutomatico(VTRegulador *regulador) = 0;
      virtual AnsiString __fastcall  MontaReguladorTapFixo(VTRegulador *regulador) = 0;
      virtual AnsiString __fastcall  MontaSuporte(VTSuporte *suporte) = 0;
      virtual AnsiString __fastcall  MontaTipoChave(VTTipoChave *tipochave) = 0;
      virtual AnsiString __fastcall  MontaTipoConsumidor(VTClasse *classe) = 0;
      virtual AnsiString __fastcall  MontaTipoRede(VTTipoRede *tiporede) = 0;
      virtual AnsiString __fastcall  MontaSuprimento(VTSuprimento *suprimento) = 0;
      virtual AnsiString __fastcall  MontaTrafoMonofasico(VTTrafo *trafo) = 0;
      virtual AnsiString __fastcall  MontaTrafoTrifasico2E(VTTrafo *trafo) = 0;
      virtual AnsiString __fastcall  MontaTrafoTrifasico3E(VTTrafo3E *trafo) = 0;
      virtual AnsiString __fastcall  MontaTrafoZZ(VTTrafoZZ *trafo) = 0;
      virtual AnsiString __fastcall  MontaTrechoArranjoTipico(VTTrecho *trecho) = 0;
      virtual AnsiString __fastcall  MontaTrechoZ0Z1(VTTrecho *trecho) = 0;
      virtual AnsiString __fastcall  MontaVersao(void) = 0;

   protected:
      struct{
            char separador;
            int  versao;
            }PD;
   };

//---------------------------------------------------------------------------
VTExportaEqpto* NewObjExportaEqpto(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

