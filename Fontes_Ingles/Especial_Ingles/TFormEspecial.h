//---------------------------------------------------------------------------
#ifndef TFormEspecialH
#define TFormEspecialH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class  VTProgresso;

//---------------------------------------------------------------------------
class TFormEspecial : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionListModulo;
   TImageList *ImageList;
   TAction *ActionAlocaCapacitor;
   TAction *ActionAlocaRegulador;
   TAction *ActionBalanceamento;
   TAction *ActionMercado;
   TAction *ActionAjusteDemanda;
   TActionList *ActionListRelatorio;
   TAction *ActionRelEqpto;
   TAction *ActionDiagEqpto;
   TAction *ActionDiagFlow;
   TAction *ActionZoom;
   TAction *ActionPerdaTecnica;
   TAction *ActionCustoNodal;
   TAction *ActionEstabilidade;
   TAction *ActionDistanciaEletrica;
   TAction *ActionPrintPreview;
   TAction *ActionGoogleMaps;
   TAction *ActionPartidaMotor;
   TAction *ActionFluxoHarmonico;
   TAction *ActionRelCarga;
   TActionList *ActionListMaster;
   TAction *ActionPriv_Debug;
   TAction *ActionPriv_Equivalente;
   TAction *ActionPriv_SinapServer;
   TAction *ActionPriv_SinapClient;
   TAction *ActionRelFluxo;
   TAction *ActionExportaNIX;
   TAction *ActionPriv_Protecao;
	TAction *ActionAnaliseMalhas;
   TAction *ActionPriv_EstimaEstado;
   TAction *ActionPriv_EstimaDemanda;
   TAction *ActionPerdaRegulatoria;
   TAction *ActionPriv_SpinServer;
   TAction *ActionPriv_SpinClient;
   TAction *ActionPriv_Confiabilidade;
   TAction *ActionPriv_TrocaEqvBT;
   TAction *ActionPriv_InterfaceODSS;
   void __fastcall ActionAlocaCapacitorExecute(TObject *Sender);
   void __fastcall ActionAlocaReguladorExecute(TObject *Sender);
   void __fastcall ActionBalanceamentoExecute(TObject *Sender);
   void __fastcall ActionMercadoExecute(TObject *Sender);
   void __fastcall ActionAjusteDemandaExecute(TObject *Sender);
   void __fastcall ActionRelEqptoExecute(TObject *Sender);
   void __fastcall ActionDiagEqptoExecute(TObject *Sender);
   void __fastcall ActionDiagFlowExecute(TObject *Sender);
   void __fastcall ActionZoomExecute(TObject *Sender);
   void __fastcall ActionEstabilidadeExecute(TObject *Sender);
   void __fastcall ActionCustoNodalExecute(TObject *Sender);
   void __fastcall ActionPerdaTecnicaExecute(TObject *Sender);
   void __fastcall ActionDistanciaEletricaExecute(TObject *Sender);
   void __fastcall ActionPrintPreviewExecute(TObject *Sender);
   void __fastcall ActionGoogleMapsExecute(TObject *Sender);
   void __fastcall ActionPartidaMotorExecute(TObject *Sender);
   void __fastcall ActionFluxoHarmonicoExecute(TObject *Sender);
   void __fastcall ActionRelCargaExecute(TObject *Sender);
   void __fastcall ActionPriv_DebugExecute(TObject *Sender);
   void __fastcall ActionPriv_EquivalenteExecute(TObject *Sender);
   void __fastcall ActionPriv_SinapServerExecute(TObject *Sender);
   void __fastcall ActionPriv_SinapClientExecute(TObject *Sender);
   void __fastcall ActionRelFluxoExecute(TObject *Sender);
   void __fastcall ActionExportaNIXExecute(TObject *Sender);
   void __fastcall ActionPriv_ProtecaoExecute(TObject *Sender);
	void __fastcall ActionAnaliseMalhasExecute(TObject *Sender);
   void __fastcall ActionPriv_EstimaEstadoExecute(TObject *Sender);
   void __fastcall ActionPriv_EstimaDemandaExecute(TObject *Sender);
   void __fastcall ActionPerdaRegulatoriaExecute(TObject *Sender);
   void __fastcall ActionPriv_SpinServerExecute(TObject *Sender);
   void __fastcall ActionPriv_SpinClientExecute(TObject *Sender);
   void __fastcall ActionPriv_ConfiabilidadeExecute(TObject *Sender);
   void __fastcall ActionPriv_TrocaEqvBTExecute(TObject *Sender);
   void __fastcall ActionPriv_InterfaceODSSExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormEspecial(TComponent *Owner, VTApl *apl_owner);
        __fastcall ~TFormEspecial(void);
   void __fastcall  Atualiza(void);

private:   //métodos
   bool         __fastcall ExisteModuloAtivoConflitante(void);
   //static TForm* __fastcall ExisteRelatorio(TForm *Owner, AnsiString form_name); //Copia Relatório
   TComponent*  __fastcall ExisteModuloAtivo(AnsiString nomeComponent); //FKM
   TWinControl* __fastcall SelecionaParentParaJanela(void);
   void         __fastcall VerificaModulos(void);

private:   //objetos externos
   VTApl *apl;

//TESTES
private:
   VTProgresso *progresso;


};

//---------------------------------------------------------------------------
#endif
//eof
