//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include "TFormEdtCarIP.h"
//#include "TFormEdtPlan.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Patamar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormEdtCarIP::TFormEdtCarIP(TComponent* Owner, VTApl *apl, VTCarga *carga)
                           : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl   = apl;
   this->carga = carga;
   //cria objeto Patamares e define sua escala
   patamares  = DLL_NewObjPatamares(apl);
   legenda    = DLL_NewObjLegenda(apl);
   lisVETVAL  = new TList();
   lisEQP     = new TList();
   /*
   //verifica se deve habilitar edi��o de dados de planejamento
   if ((carga->planeja.ano > 0)&&
       ((carga->planeja.ano >= rede->planeja.ano)||(rede->planeja.ano == MAX_ANO)))
      {//cria Form p/ edi��o de dados do Planejamento
      FormEdtPlan = new TFormEdtPlan(this, tsPlaneja, apl, carga);
      //habilita TableSheet de planejamento
      tsPlaneja->TabVisible = true;
      }
   else
      {//desabilita TableSheet de planejamento
      tsPlaneja->TabVisible = false;
      }
   */
   //desabilita TableSheet de planejamento
   tsPlaneja->TabVisible     = false;
   TabSheetCurva->TabVisible = false;
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtCarIP::~TFormEdtCarIP(void)
   {//vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destr�i objetos criados
   //if (FormEdtPlan) {delete FormEdtPlan; FormEdtPlan = NULL;}
   //destr�i objetos criados
   if (legenda)   {delete legenda;   legenda   = NULL;}
   if (patamares) {delete patamares; patamares = NULL;}
   //destr�i lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP; lisEQP = NULL;}
   //destr�i listas e seus obejtos
   if (lisVETVAL) {LimpaTList(lisVETVAL); delete lisVETVAL; lisVETVAL = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::ActionConfirmaExecute(TObject *Sender)
	{
	//l� e valida os dados
   if (! LeDadosCarga()) return;
   /*
   //verifica se � editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(carga)) return;
      }
   */
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se � editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(carga);
      }
   */
   //fecha o form indicando exclus�o do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::CBoxMBChange(TObject *Sender)
   {
   //reinicia CBoxFase
   IniciaCBoxFase();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::DefValoresDefaultCarga(void)
   {
   //vari�veis locais
   VTCarga   *ult_carga;
   VTClasses *classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
//   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //verifica se a Carga possui Classe
   if (carga->classe == NULL)
      {//define Classe
      carga->classe = classes->ExisteClasseTag(consIP);
      }
   //verifica se a Carga n�o est� definido
   if (carga->IP_Pot_kw == 0.)
      {//define dados da Carga de IP
      carga->IP_Pot_kw = 0.4;
      carga->IP_Fatpot = 0.9;
      carga->Fases     = faseAN;
      carga->Icte_pu   = 1.;
      carga->Scte_pu   = 0.;
      carga->Zcte_pu   = 0.;
      //seleciona Carga de refer�ncia p/ copiar dados
      if ((ult_carga = ExisteCargaIP(lisEQP)) == NULL) return;
      //copia dados
      carga->IP_Pot_kw = ult_carga->IP_Pot_kw;
      carga->IP_Fatpot = ult_carga->IP_Fatpot;
      carga->Fases     = ult_carga->Fases;
      carga->Icte_pu   = ult_carga->Icte_pu;
      carga->Scte_pu   = ult_carga->Scte_pu;
      carga->Zcte_pu   = ult_carga->Zcte_pu;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::ExibeCurva(void)
   {
   //vari�veis locais
   int         ind_cor;
   double      val, val_max, val_min, increment;
   AnsiString  titulo;
   TColor      cor = clBlue;
   VTVetval    *vetval;

   //prote��o: verifica se h� uma Curva selecionada
   if (CBoxCurva->ItemIndex < 0) return;
   //redefine Curva do Carga
   carga->curva  = (VTCurva*)CBoxCurva->Items->Objects[CBoxCurva->ItemIndex];
   //obt�m lista de valores de Pot.Ativa definida pela Curva da Carga
   legenda->LisVetValFluxo(carga, lisVETVAL, car_P);
   if (lisVETVAL->Count == 0) return;
   //insere Serie no TChart p/ o Vetval
   vetval = (VTVetval*)lisVETVAL->First();
   //reinicia s�rie
   Series1->Clear();
   if (chart->LeftAxis->Maximum >= 0)
      {
      chart->LeftAxis->Minimum  = 0.;
      chart->LeftAxis->Maximum  = 0.;
      }
   else
      {
      chart->LeftAxis->Maximum  = 0.;
      chart->LeftAxis->Minimum  = 0.;
      }
   //preenche a S�rie c/ os valores em todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {//determina valor no patamar
      val = vetval->Valor[np];
      //insere ponto na Serie
      //Series1->AddXY(np+1, val, IntToStr(np+1), cor);
      Series1->AddXY(np,   val, IntToStr(np+1), cor);
      Series1->AddXY(np+1, val, IntToStr(np+1), cor);
      //determina valores m�ximo e m�nimo
      if (np == 0) val_max = val_min = val;
      val_max = max(val_max, val);
      val_min = min(val_min, val);
      }
   //inclui uma margem adicional nos valores m�ximo e m�nimo
   (val_max > 0) ? (val_max += 0.1 * val_max) : (val_max -= 0.1 * val_max);
   (val_min > 0) ? (val_min -= 0.1 * val_min) : (val_min += 0.1 * val_min);
   //define incremento
   increment = (val_max - val_min) / 10.;
   if      (increment > 10000) increment = 10000.;
   else if (increment > 1000)  increment = 1000.;
   else if (increment > 100)   increment = 100.;
   else if (increment > 10)    increment = 10.;
   else if (increment > 1.)    increment = 1.;
   else if (increment > 0.1)   increment = 0.1;
   else if (increment > 0.01)  increment = 0.01;
   else                        increment = 0.001;
   //redefine valores m�nimo e m�ximo do eixo y
   chart->LeftAxis->Visible   = true;
   chart->LeftAxis->Automatic = false;
   if (val_max >= 0)
      {
      chart->LeftAxis->Maximum   = val_max;
      chart->LeftAxis->Minimum   = val_min;
      }
   else
      {
      chart->LeftAxis->Minimum   = val_min;
      chart->LeftAxis->Maximum   = val_max;
      }
   chart->LeftAxis->Increment = increment;
   //reinicia t�tulo
   chart->Title->Text->Clear();
   chart->Title->Text->Add(vetval->DescUnid());
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::ExibeDadosCarga(void)
   {
   //exibe c�digo
   EditCodigo->Text = carga->Codigo;
   //exibe pot.ativa e fat.pot�ncia
	EditNumLamp->Text	= IntToStr(carga->IP_NumLamp);
	EditPot->Text     = DoubleToStr("%4.3f", carga->IP_Pot_kw * 1000.);   //kW => W
	EditFatPot->Text  = DoubleToStr("%4.3f", carga->IP_Fatpot);
   //inicia CBoxMB(monof�sica, bif�sica)
   CBoxMB->ItemIndex = (carga->MBTfasica == cargaMONOFASICA) ? 0 : 1;
   //inicia CBoxFase c/ as poss�veis fases da Carga
   IniciaCBoxFase();
   //inicia CBoxClasse p/ identificar a Classe da Carga
   //CBoxClasse->ItemIndex = CBoxClasse->Items->IndexOfObject(pcarga->classe);
   //inicia CBoxModCarga p/ identificar o modelo de carga da Carga
   if      (carga->Icte_pu > 0.) CBoxModCarga->ItemIndex = cargaICTE - cargaICTE;
   else if (carga->Scte_pu > 0.) CBoxModCarga->ItemIndex = cargaSCTE - cargaICTE;
   else                          CBoxModCarga->ItemIndex = cargaZCTE - cargaICTE;
   //verifica se o Carga possui uma Curva t�pica associada
   if (carga->curva != NULL)
      {
      CBoxCurva->ItemIndex = CBoxCurva->Items->IndexOfObject(carga->curva);
      if ((CBoxCurva->ItemIndex < 0)&&(CBoxCurva->Items->Count > 0)) CBoxCurva->ItemIndex = 0;
      }
   }

//---------------------------------------------------------------------------
VTCarga*  __fastcall TFormEdtCarIP::ExisteCargaIP(TList *lisEQP)
   {
   //vari�veis locais
   VTCarga *carga;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obt�m lista de Cargas da Rede
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoCARGA);
   if (lisEQP->Count == 0) return(NULL);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      carga = (VTCarga*)lisEQP->Items[n];
      if (carga->classe->Tag == consIP) return(carga);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::FormClose(TObject *Sender,TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::FormShow(TObject *Sender)
   {
   //se necess�rio, define valores default p/ Carga
   DefValoresDefaultCarga();
   //inicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //inicia CBoxCurva c/ as curvas t�picas de Carga
   IniciaCBoxCurva();
   //exibe dados do Carga
   ExibeDadosCarga();
   //seleciona TableSheet
	PageControl->ActivePage = TabSheetCarga;
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::IniciaCBoxCurva(void)
   {
   //vari�veis locais
   VTCurva  *curva;
   VTCurvas *curvas   = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   TList    *lisCURVA = curvas->ListaCurvaTipicaIP();

   //reinicia CBoxCurva
   CBoxCurva->Clear();
   for (int n = 0; n < lisCURVA->Count; n++)
      {
      curva = (VTCurva*)lisCURVA->Items[n];
      CBoxCurva->Items->AddObject(curva->Codigo, curva);
      }
   //pr�-seleciona primeira Curva
   if (CBoxCurva->Items->Count > 0) CBoxCurva->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::IniciaCBoxFase(void)
   {
   //vari�veis locais
   TFase   *fase;
   TList   *lisFASE;
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));
   
   //inicia lisFASE c/ o conjunto de fases definidas pelo CBoxMBT
   switch (CBoxMB->ItemIndex)
      {
      case 0:  lisFASE = fases->LisFasesCarga(cargaMONOFASICA); break;
      default: lisFASE = fases->LisFasesCarga(cargaBIFASICA);   break;
      }
   //reinicia CBoxFase
   CBoxFase->Clear();
   for (int n = 0; n < lisFASE->Count; n++)
      {
      fase = (TFase*)lisFASE->Items[n];
      CBoxFase->Items->AddObject(fase->codigo, fase);
      }
   //pr�-seleciona fases da Carga
   if (CBoxFase->Items->Count < 0) return;
   CBoxFase->ItemIndex = CBoxFase->Items->IndexOf(fases->AsStrUpper[carga->Fases]);
   if (CBoxFase->ItemIndex < 0) CBoxFase->ItemIndex = 0; 
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::IniciaParteFixaTela(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarIP::LeDadosCarga(void)
   {
   //vari�veis locais
   double q_cap;

   //verifica se h� uma Curva t�pica selecionada
   if (CBoxCurva->ItemIndex < 0)
      {
      Aviso("Selecione a curva t�pica da carga de IP");
      return(false);
      }
   try{//l� atributos do capacitor
		carga->Codigo     = EditCodigo->Text;
		carga->IP_NumLamp = StrToInteger(EditNumLamp->Text);
		carga->IP_Pot_kw  = StrToDouble(EditPot->Text) / 1000.;   //W => kW
		carga->IP_Fatpot  = StrToDouble(EditFatPot->Text);
		if (! LeFases())         return(false);
		if (! LeValorModCarga()) return(false);
		//if (! SelecionaClasse()) return(false);
		carga->curva  = (VTCurva*)CBoxCurva->Items->Objects[CBoxCurva->ItemIndex];
		//valida valores lidos
		if (carga->IP_NumLamp <= 0)
			{
			Aviso("N�mero inv�lido de l�mpadas");
			return(false);
			}
		if (IsDoubleZero(carga->IP_Pot_kw))
			{
			Aviso("Defina a pot�ncia da l�mpada");
			return(false);
			}
		if (IsDoubleZero(carga->IP_Fatpot))
			{
			Aviso("Defina o fator de pot�ncia da l�mpada");
			return(false);
			}
		if (carga->IP_Fatpot > 1.)
			{
			Aviso("Fator de pot�ncia inv�lido");
			return(false);
			}
		}catch(Exception &e)
         {
			Erro("Valores inv�lidos nos dados da carga");
			return(false);
			}
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarIP::LeFases(void)
   {
   //vari�veis locais
   TFase *fase;

   //verifica se est� selecionada uma fase
   if (CBoxFase->ItemIndex < 0)
      {
      Erro("Defina as fase da carga");
      return(false);
      }
   if ((fase = (TFase*)CBoxFase->Items->Objects[CBoxFase->ItemIndex]) == NULL) return(false);
   //atualiza fase da Carga
   carga->Fases = fase->tag;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarIP::LeValorModCarga(void)
   {
   //vari�veis locais
   double icte = 0., scte = 0., zcte = 0.;

   switch(CBoxModCarga->ItemIndex)
      {
      case 0:  icte = 1.; break;
      case 1:  scte = 1.; break;
      default: zcte = 1.; break;
      }
   //atualiza Carga
   carga->Icte_pu = icte;
   carga->Scte_pu = scte;
   carga->Zcte_pu = zcte;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::OpcaoRetira(bool enabled)
   {
   butRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::PageControlChange(TObject *Sender)
   {
   //verifica se est� selecionado visualiza��o da Curva
	if (PageControl->ActivePage != TabSheetCurva) return;
   //atualiza TChart
   ExibeCurva();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarIP::PageControlChanging(TObject *Sender, bool &AllowChange)
	{
	//habilita a mudan�a
	AllowChange = true;
	//verifica se o TabSheet atual �  TabSheetCarga
	if (PageControl->ActivePage == TabSheetCarga)
		{//l� e valida dados de pot�ncia
		AllowChange = LeDadosCarga();
		}
	}

//---------------------------------------------------------------------------
//eof

