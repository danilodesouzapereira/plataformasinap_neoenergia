//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCarro.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCarro::TFormCarro(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objetos
   lisCAR = new TList();
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFormCarro::~TFormCarro(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisCAR) {delete lisCAR; lisCAR = NULL;}
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCarro::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCarro::ActionConfirmaExecute(TObject *Sender)
   {
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //l� configura��o selecionada pelo usu�rio
   if (!(ValidaDados() && LeDados())) return;

   //elimina Cargas de carro j� inclu�das
   RetiraCargaCarro();
   //cria  Carga adicional p/  consumidor residencial
   if (InsereCargaCarro())
      {//atualiza gr�fico
      grafico->AreaRedeAltera();
      //fecha o form
      ModalResult = mrOk;
	  }
   Aviso("Os pontos de carga de carros el�tricos foram inseridos com sucesso");
   }

//---------------------------------------------------------------------------
void __fastcall TFormCarro::ActionRetiraExecute(TObject *Sender)
   {
   //retira todas as Cargas de carro el�trico
   RetiraCargaCarro();
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TFormCarro::CriaCargaCarro(VTCarga *carga_res)
   {
   //vari�veis locais
   VTCurva     *curva;
   VTCarga     *carga;
   VTClasses   *classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //cria Curva de demanda
   if ((curva = CriaCurvaCarga()) == NULL) return(NULL);
   //cria Carga
   if ((carga = DLL_NewObjCarga(patamares->NumPatamar())) == NULL) return(NULL);
   carga->Id             = FalsoId();
   carga->Codigo         = "Carro El�trico";
   carga->classe         = classes->ExisteClasseTag(consOUTROS); //carga_res->classe;
   carga->Fases          = carga_res->Fases;
   carga->Energia_mwhmes = 0;
   carga->IP_NumLamp     = 0;
   carga->IP_Pot_kw      = 0;
   carga->IP_Fatpot      = 0;
   //define modelo de Carga Icte
   carga->Icte_pu    = 1;
   carga->esq.posbar =   carga_res->esq.posbar;
   carga->esq.dxbar  = - carga_res->esq.dxbar / 2;
   carga->esq.dybar  = - carga_res->esq.dybar / 2;
   //define Curva da Carga
   carga->DefineCurva(curva);
   //associa Carga c/ a Barra (SEM INSERIR)
   carga->pbarra = carga_res->pbarra;
   return(carga);
   }


//---------------------------------------------------------------------------
VTCurva* __fastcall TFormCarro::CriaCurvaCarga(void)
   {
   //vari�veis locais
   int      num_pat;
   int 		hora;
   strHM     hm_iniCarga, hm_ini, hm_fim, hm_fimCarga;
   union    {
			double vet[nvCURVA_PQ];
			struct {double p, q;} var;
            } valor;
   VTCurva *curva;

   //cria uma nova Curva
   curva                = DLL_NewObjCurva();
   curva->Id            = FalsoId();
   curva->Codigo        = "";
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Escala        = pat01HORA;
   curva->NumeroValores = nvCURVA_PQ;
   curva->Tipica        = false;
   curva->Eqpto         = NULL;
   curva->Classe        = NULL;
   //define per�odo com carga
   hm_iniCarga.hora   = hora_ini;
   hm_iniCarga.minuto =  0;
   hora = hora_ini + num_hora;
   if (hora >= 24)
   {
		hora = hora - 24;
   }
   hm_fimCarga.hora   =  hora;
   hm_fimCarga.minuto = 0;
   valor.var.p = pot_kw / 1000.;
   valor.var.q = 0.;
   curva->SetPonto(hm_iniCarga, hm_fimCarga, valor.vet, sizeof(valor.vet)/sizeof(double));
   //define per�odo sem carga
   hm_ini =  hm_fimCarga;
   hm_fim =  hm_iniCarga;
   valor.var.p = 0.;
   valor.var.q = 0.;
   curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
   curva->Reconfigura(pat01HORA);
   return(curva);
   }
//---------------------------------------------------------------------------
void __fastcall TFormCarro::FormClose(TObject *Sender, TCloseAction &Action)   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCarro::InsereCargaCarro(void)
   {
   //vari�veis locais
   int     num_tot;
   VTCarga *carga, *carga_res;
   VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia lista c/ todas Cargas
   lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoCARGA);
   //identifica cargas residenciais
   for (int nc = lisEQP->Count-1; nc >= 0; nc--)
      {
      carga = (VTCarga*)lisEQP->Items[nc];
      //verifica se Carga � residencial
      if (carga->classe->Tag != consRESIDENCIAL) lisEQP->Delete(nc);
      }
   //determina n�mero de Cargas de carro devem ser inclu�das
   num_tot = lisEQP->Count * qtde_carro;
   //reinicia lista de Cargas do tipo carro
   lisCAR->Clear();
   //cria uma Carga tipo carro p/ cada Carga residencial
   for (int nc = 0; nc < num_tot; nc++)
      {
      carga_res = (VTCarga*)lisEQP->Items[nc];
      //cria uma Carga adicional SEM inserir na Barra
      if ((carga = CriaCargaCarro(carga_res)) != NULL) lisCAR->Add(carga);
      }
   //insere Cargas na rede
   if (lisCAR->Count > 0) edita->InsereLisEqpto(lisCAR);
   return(lisCAR->Count > 0);
   }

// https://sinapsisenergia.teamwork.com/#/tasks/16090086
//---------------------------------------------------------------------------
bool __fastcall TFormCarro::LeDados(void)
   {
   // vari�veis locais
   double potencia;

   //l� porcentagems de consumidores residenciais com carro el�trico
   if (CBoxQtde->ItemIndex < 0)
      {
	  Aviso("Selecione porcentagem de resid�ncias c/ carro el�trico.");
      return(false);
      }
   if (CBoxHoraIni->ItemIndex < 0)
      {
	  Aviso("Selecione hora de in�cio da carga.");
      return(false);
      }
   if (CBoxNumHora->ItemIndex < 0)
	  {
	  Aviso("Selecione n�mero de horas de carga.");
	  return(false);
	  }
   //
   potencia = StrToDouble(edtPotencia->Text);
   if ((potencia <= 0.0)|| (potencia >= 1000.0))
   {
	   Aviso("Digite um valor de pot�ncia do ponto de carga entre 0 e 1000 (kW).");
	   return(false);
   }
//   if (CBoxPotencia->ItemIndex < 0)
//	  {
//	  Aviso("Selecione pot�ncia do ponto de carga (kW)");
//	  return(false);
//	  }
   try{
	  qtde_carro = StrToDouble(CBoxQtde->Text) / 100.;
      hora_ini   = CBoxHoraIni->ItemIndex;
	  num_hora   = StrToInt(CBoxNumHora->Text);
      pot_kw     = potencia;
	  //pot_kw     = StrToDouble(CBoxPotencia->Text);
      }catch(Exception &e)
         {
         Erro("Erro nos dados de configura��o");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCarro::RetiraCargaCarro(void)
   {
   //vari�veis locais
   VTCarga *carga;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //inicia lista c/ todas Cargas
   lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoCARGA);
   //mant�m na lista apenas as Cargas relativas a carro el�trico
   for (int nc = lisEQP->Count-1; nc >= 0; nc--)
      {
      carga = (VTCarga*)lisEQP->Items[nc];
      //verifica se Carga � carro
      if (carga->Codigo.AnsiCompareIC("Carro El�trico") != 0) lisEQP->Delete(nc);
      }
   //pede confirma��o do usu�rio
   if (lisEQP->Count == 0) return;
   if (Confirma("Existem " + IntToStr(lisEQP->Count) + " pontos de carga de bateria",
                "Confirma exclus�o destas cargas?") != IDYES) return;
   //retira Cargas
   edita->RetiraLisEqpto(lisEQP);
   }

// https://sinapsisenergia.teamwork.com/#/tasks/16090086
//---------------------------------------------------------------------------
bool __fastcall TFormCarro::ValidaDados(void)
{
	// variaveis locais
	double valor;

	try
	{
		if (edtPotencia->Text.IsEmpty())
			{
				Aviso("Digite um valor para pot�ncia do ponto de carga.");
				return (false);
			}
		if (!StrToDouble(edtPotencia->Text, valor))
			{
				Aviso("Pot�ncia do ponto de carga inv�lida.");
				return (false);
			}
	}
	catch (Exception &e)
	{
		Aviso("Texto inv�lido: " + e.Message);
		return (false);
	}
	return (true);
}

//---------------------------------------------------------------------------
//eof



