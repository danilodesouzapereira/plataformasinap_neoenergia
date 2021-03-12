//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormLocaliza.h"
#include "VTLocaliza.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
//#include "..\Cronometro\VTCronometro.h"
#include "..\Editor\VTEdt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Legenda\TAtributo.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFORMEDT"
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormLocaliza::TFormLocaliza(TForm* Owner, VTApl *apl_owner, TWinControl *parent)
   : TFormEdt(Owner, parent)
   {
   //cria objeto VTApl
   apl = NewObjApl(this, apl_owner);
   apl->Add(DLL_NewObjConsulta(apl));
   apl->Add(DLL_NewObjLegenda(apl));
   apl->Add(NewObjLocaliza(apl));
   lisATRIB = new TList();
   //salva ponteiro p/ objetos
   grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   redes     = (VTRedes*)apl_owner->GetObject(__classid(VTRedes));
   consulta  = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
   localiza  = (VTLocaliza*)apl->GetObject(__classid(VTLocaliza));
   legenda   = (VTLegenda*)apl->GetObject(__classid(VTLegenda));
   //inicia CBoxConsulta
   CBoxConsultaInicia();
   //simula sele��o de consulta
   CBoxConsultaChange(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TFormLocaliza::~TFormLocaliza(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //cancela destaque de Eqptos e moldura
   GraficoDestacaEqpto();
   GraficoMoldura();
   //destr�i objetos
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
   if (apl) {delete apl; apl = NULL;}
   //destr�i listas e seus obejtos
   if (lisATRIB) {LimpaTList(lisATRIB); delete lisATRIB; lisATRIB = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ActionCancelaSelecaoExecute(TObject *Sender)
   {
   //executa Busca
   //ExecutaBusca();
   //reinicia ListBox
   ListBox->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ActionCorExecute(TObject *Sender)
   {//DVK 2014.11.04
   //exibe ColorDialog
   ColorDialog->Color = PanelCor->Color;
   if (ColorDialog->Execute())
      {//seleciona nova cor
      PanelCor->Color = ColorDialog->Color;
      //atualiza
      ExecutaConsultaSelecionada();
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14004105
void __fastcall TFormLocaliza::ActionEdtExecute(TObject *Sender)
{
	try
	{ // se necess�rio, cria um objeto TEdtBusca
		if (edt != NULL)
			delete edt;
		edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, NULL);//ActionSelEqpto);
		// liga indica��o de mouse ativo e for�a bot�o Down
		AtualizaIndicacaoEditorOn(butEdt);
	}
	catch(Exception &e)
	{//nada
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ActionFiltroEqptoErroGisExecute(TObject *Sender)
   {
   //vari�veis locais
   bool  area_visivel;
   int   tipo_eqpto;
   TList *lisEXT;

   //determina tipo de equipamento
   if ((tipo_eqpto = TipoEquipamento()) < 0)
      {
      Aviso("Selecione o tipo de equipamento a ser localizado");
      return;
      }
   //esconde GroupBoxFiltro
   PanelFiltro->Visible = false;
   //mostra PanelCorEqpto
   PanelCorEqpto->Visible = true;
   //verifica abrang�ncia da �rea de busca: rede toda ou apenas �rea vis�vel
   area_visivel = RadioGroupArea->ItemIndex == 0;
   //identifica Eqpto recebido do GIS que teve algum atributo corrigido
   lisEXT = localiza->LisEqptoErroGis(redes, tipo_eqpto, area_visivel);
   //reinicia ListBox
   ListBoxInicia(lisEXT);
   //simula sele��o p/ exibir o gr�fico
   ListBoxClick(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ActionFiltroPorAtributoExecute(TObject *Sender)
   {
   //exibe GroupBoxFiltro
   if (! PanelFiltro->Visible)
      {
      PanelFiltro->Visible = true;
      //esconde PanelCorEqpto
      PanelCorEqpto->Visible = false;
      //reinicia ListBox
      ListBox->Clear();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ActionLocalizaExecute(TObject *Sender)
   {
   //executa busca
   ExecutaBusca();
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14004105
void __fastcall TFormLocaliza::ActionMouseOffExecute(TObject *Sender)
{
	// indica mouse inativo
	ImageMouseOff->Visible = true;
	ImageMouseOn->Visible = false;
}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14004105
void __fastcall TFormLocaliza::ActionMouseOnExecute(TObject *Sender)
{
	//indica mouse ativo
	ImageMouseOn->Visible = true;
	ImageMouseOff->Visible = false;
}

//---------------------------------------------------------------------------
int __fastcall TFormLocaliza::Atributo(void)
   {
   //determina atributo de equipamento
   if (CBoxAtributo->ItemIndex < 0) return(-1);
   return(int(CBoxAtributo->Items->Objects[CBoxAtributo->ItemIndex]));
   }

//---------------------------------------------------------------------------
bool __fastcall TFormLocaliza::AtributoValido(TAtributo *atributo)
   {
   //atributos comuns a todos Eqptos
   if (atributo->bit_index == eqp_ID)     return(true);
   if (atributo->bit_index == eqp_CODIGO) return(true);
   //atributos comuns a todas Ligacoes
   if (atributo->eqpto->TipoLigacao())
      {
      if (atributo->bit_index == lig_REDE)  return(true);
      }
   //atributos espec�ficos de cada Eqpto
   switch(atributo->eqpto->Tipo())
      {
      case eqptoBARRA:
         if (atributo->bit_index == bar_VNOM) return(true);
         break;
      case eqptoCAPACITOR:
         if (atributo->bit_index == cap_Q) return(true);
         break;
      case eqptoCAPSERIE:
         if (atributo->bit_index == caps_SNOM) return(true);
         break;
      case eqptoCARGA:
         if (atributo->bit_index == car_ENERGIA) return(true);
         if (atributo->bit_index == car_FASES  ) return(true);
         if (atributo->bit_index == car_MODELO ) return(true);
         if (atributo->bit_index == car_CLASSE ) return(true);
         break;
      case eqptoCHAVE:
         if (atributo->bit_index == chv_TIPO  ) return(true);
         if (atributo->bit_index == chv_INOM  ) return(true);
         if (atributo->bit_index == chv_ESTADO) return(true);
         break;
      case eqptoGERADOR:
         if (atributo->bit_index == ger_TIPO) return(true);
         if (atributo->bit_index == ger_SNOM) return(true);
         break;
      case eqptoFILTRO:
         return(false);
      case eqptoMEDIDOR:
         return(false);
      case eqptoMUTUA:
         return(false);
      case eqptoREATOR:
         if (atributo->bit_index == reat_Q) return(true);
         break;
      case eqptoREGULADOR:
         if (atributo->bit_index == reg_SNOM) return(true);
         break;
      case eqptoSUPRIMENTO:
         if (atributo->bit_index == sup_TIPO) return(true);
         if (atributo->bit_index == sup_SMAX) return(true);
         break;
      case eqptoTRAFO:
         if (atributo->bit_index == trf_SNOM    ) return(true);
         if (atributo->bit_index == trf_PERDA_FE) return(true);
         break;
      case eqptoTRAFO3E:
         if (atributo->bit_index == trf3_SNOM    ) return(true);
         if (atributo->bit_index == trf3_PERDA_FE) return(true);
         break;
      case eqptoTRAFOZZ:
         return(false);
      case eqptoTRECHO:
         if (atributo->bit_index == tre_COMP   ) return(true);
         if (atributo->bit_index == tre_ARRANJO) return(true);
         if (atributo->bit_index == tre_INOM   ) return(true);
         if (atributo->bit_index == tre_FASES  ) return(true);
         break;
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::CBoxAtributoChange(TObject *Sender)
   {
   //reinicia CBoxOperador
   CBoxOperador->ItemIndex = 0;
   //reinicia RichEditValor
   //RichEditValor->Lines->Clear();
   RichEditValor->Text = "*";
   //executa busca
   ExecutaBusca();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::CBoxAtributoInicia(VTEqpto *eqpto)
   {
   //vari�veis locais
   TAtributo  *atributo;

   //obt�m lista de Atributos do Eqpto
   legenda->LisAtributosEqpto(eqpto, lisATRIB);
   //reinicia CBoxAtributo
   CBoxAtributo->Clear();
   //apreenche CBoxAtributo c/ atributos
   for (int n = 0; n < lisATRIB->Count; n++)
      {
      atributo = (TAtributo*)lisATRIB->Items[n];
      //filtra os atributos habilitados p/ localiza��o
      if (AtributoValido(atributo))
         {
         CBoxAtributo->Items->AddObject(atributo->nome + atributo->unidade, (TObject*)atributo->bit_index);
         }
      }
   //pr�-seleciona atributo C�digo
   if (CBoxAtributo->Items->Count > 0)
      {
      CBoxAtributo->ItemIndex = CBoxAtributo->Items->IndexOfObject((TObject*)eqp_CODIGO);
      }
   //destr�i objetos Atributo em lisATRIB
   LimpaTList(lisATRIB);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::CBoxConsultaChange(TObject *Sender)
   {
   ExecutaConsultaSelecionada();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::CBoxConsultaInicia(void)
   {
   //vari�veis locais
   TAction *Action;

   //reinicia CBoxConsulta
   CBoxConsulta->Clear();
   //loop p/ todas Action em ActionListConsulta
   for (int n = 0; n < ActionListConsulta->ActionCount; n++)
      {
      Action =  (TAction*)(ActionListConsulta->Actions[n]);
      CBoxConsulta->Items->AddObject(Action->Caption, Action);
      }
   //pr�-seleciona primeiro item
   if (CBoxConsulta->Items->Count > 0) CBoxConsulta->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ExecutaConsultaSelecionada(void)
   {
   //vari�veis locais
   TAction *Action;

   //cancela destaque e moldura no gr�fico
   GraficoMoldura();
   GraficoDestacaEqpto();
   //prote��o
   if (CBoxConsulta->ItemIndex < 0) return;
   //executa Action selecionada
   Action = (TAction*)(CBoxConsulta->Items->Objects[CBoxConsulta->ItemIndex]);
   //executa Action
   Action->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::CBoxOperadorChange(TObject *Sender)
   {
   //executa localiza��o
   ExecutaBusca();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::CBoxOperadorInicia(void)
   {
   //reinicia CBoxOperador
   CBoxOperador->Clear();
   //apreenche CBoxOperador
   CBoxOperador->Items->AddObject("Igual a",      (TObject*)opIGUAL);
   CBoxOperador->Items->AddObject("Menor que",    (TObject*)opMENOR);
   CBoxOperador->Items->AddObject("Maior que",    (TObject*)opMAIOR);
   CBoxOperador->Items->AddObject("Diferente de", (TObject*)opDIFERENTE);
   //pr�-seleciona operador opIGUAL
   CBoxAtributo->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::CBoxTipoEqptoChange(TObject *Sender)
   {
   //vari�veis locais
   VTEqpto *eqpto;

   //reinicia RichEditValor
   //RichEditValor->Lines->Clear();
   RichEditValor->Text = "*";
   //prote��o
   if (CBoxTipoEqpto->ItemIndex < 0) return;
   //determina Eqpto selecionado
   eqpto = (VTEqpto*)CBoxTipoEqpto->Items->Objects[CBoxTipoEqpto->ItemIndex];
   //DVK 2014.11.04 executa consulta ou busca
   if(CBoxConsulta->ItemIndex == 0)
      {//reinicia CBoxAtributo
      CBoxAtributoInicia(eqpto);
      //reinicia CBoxOperador
      CBoxOperador->ItemIndex = 0;
      //executa localiza��o
      ExecutaBusca();
      }
   else
      {
      ExecutaConsultaSelecionada();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::CBoxTipoEqptoInicia(void)
   {
   //vari�veis locais
   VTEqpto *eqpto;
   TList   *lisEQP;
   //vari�veis locais
   int tipo_eqpto[] = {eqptoBARRA,     eqptoCAPACITOR,   eqptoCAPSERIE,
                       eqptoCARGA,     eqptoCHAVE,       eqptoFILTRO,
                       eqptoGERADOR,   eqptoMEDIDOR,     eqptoMUTUA,
                       eqptoREATOR,    eqptoREGULADOR,   eqptoSUPRIMENTO,
                       eqptoTRAFO,     eqptoTRAFO3E,     eqptoTRAFOZZ,
                       eqptoTRECHO};
   //reinicia CBoxTipoEqpto
   CBoxTipoEqpto->Clear();
   //preenche CBoxTipoEqpto apenas com os equipamentos existentes
   for (int n = 0; n < sizeof(tipo_eqpto)/sizeof(int); n++)
      {//obt�m lista de Eqptos vis�veis
      lisEQP = consulta->LisEqptoRedeVisible(redes, tipo_eqpto[n]);
      if (lisEQP->Count == 0) continue;
      eqpto = (VTEqpto*)lisEQP->First();
      CBoxTipoEqpto->Items->AddObject(eqpto->TipoAsString(), eqpto);
      }
   if (CBoxTipoEqpto->Items->Count > 0) CBoxTipoEqpto->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ExecutaBusca(void)
   {
   //vari�veis locais
   bool       area_visivel;
   int        count;
   int        tipo_eqpto;
   int        ind_atrib;
   int        operador;
   AnsiString val_atrib;
   VTEqpto    *eqpto;
   TList      *lisEQP;
//   VTCronometro *cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));

   //inicia cron�metro
//   cronometro->Start("TLocaliza", "Eqptos");

   //inicia ListBox
   ListBox->Clear();
   //determina tipo de equipamento
   if ((tipo_eqpto = TipoEquipamento()) < 0)
      {
      Aviso("Selecione o tipo de equipamento a ser localizado");
      return;
      }
   //determina atributo selecionado p/ o filtro
   if ((ind_atrib = Atributo()) < 0)
      {
      Aviso("Selecione o atributo a ser utilizado");
      return;
      }
   //determina operador p/ o filtro
   if ((operador = Operador()) < 0)
      {
      Aviso("Selecione a opera��o de sele��o do atributo");
      return;
      }
   //determina valor do atributo
   val_atrib = RichEditValor->Lines->Text.Trim();
   //verifica abrang�ncia da �rea de busca: rede todo ou apenas �rea vis�vel
   area_visivel = RadioGroupArea->ItemIndex == 0;
   //executa a localiza��o
   lisEQP = localiza->LisEqpto(redes, tipo_eqpto, operador,ind_atrib, val_atrib, area_visivel);
   //reinicia ListBox
   ListBoxInicia(lisEQP);
   //simula sele��o p/ exibir o gr�fico
   ListBoxClick(NULL);
//   cronometro->Stop("TLocaliza");
//   cronometro->Show();
//   cronometro->Reset();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
   }

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14004105
void __fastcall TFormLocaliza::FormMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int HitTest,
		  TMouseActivate &MouseActivate)
{
	ActionEdt->Execute();
}

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::GraficoDestacaEqpto(TList *lisEQP)
   {
   //exibe moldura no Eqpto indicado
//   grafico->DestacaEqpto(lisEQP, clLime, 2);
   //DVK 2014.11.04 utiliza cor do PanelCor
   grafico->DestacaEqpto(lisEQP, PanelCor->Color, 2);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::GraficoMoldura(VTEqpto *eqpto)
   {
   //exibe moldura no Eqpto indicado
   grafico->Moldura(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::GraficoZoom(VTEqpto *eqpto)
   {
   //vari�veis locais
   bool ampliar;

   //prote��o
   if (eqpto == NULL) return;
   //define se deve ampliar ou diminuir imagem
   if (zoom.shift_state.Contains(ssShift)) ampliar = false;
   else                                    ampliar = true;
   //seleciona �rea de zoom
   grafico->SelecionaZoom(eqpto, ampliar);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14004105
void __fastcall TFormLocaliza::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //esconde ButEdt mas executa, para tornar o form ativo
   ActionEdt->Visible   = false;
   ActionEdt->Execute();
   //inicia o CBoxOperador com os poss�veis operdores l�gicos
   CBoxOperadorInicia();
   //inicia o CBoxTipoEqpto com os tipos de equipamentos poss�veis
   CBoxTipoEqptoInicia();
   //simula alter��o de CBoxTipoEqpto p/ atualiza CBoxAtributo
   CBoxTipoEqptoChange(NULL);
   //exibe Eqptos do tipo pr�-selecionado
   //ExecutaBusca();
   //posiciona cursor do mouse em RichEditValor
   RichEditValor->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ListBoxClick(TObject *Sender)
   {
   //vari�veis locais
   VTEqpto *eqpto;

   //verifica se h� um Eqpto selecionado
   if (ListBox->ItemIndex < 0) return;
   eqpto = (VTEqpto*)ListBox->Items->Objects[ListBox->ItemIndex];
   //exibe moldura
   Moldura(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ListBoxDblClick(TObject *Sender)
   {
   //vari�veis locais
   VTEqpto *eqpto;

   //verifica se h� um Eqpto selecionado
   if (ListBox->ItemIndex < 0) return;
   eqpto = (VTEqpto*)ListBox->Items->Objects[ListBox->ItemIndex];
   //redefine �rea de zoom p/ destacar Eqpto
   GraficoZoom(eqpto);
   //exibe moldura
   GraficoMoldura(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ListBoxInicia(TList *lisEQP)
   {
   //vari�veis locais
   VTEqpto *eqpto;

   //inicia ListBox
   ListBox->Clear();
   //atualiza Caption do GBoxEqpto
   GBoxEqpto->Caption = "Localizados: " + IntToStr(lisEQP->Count);
   //cancela ordena��o
   ListBox->Sorted = false;
   //prepara insercao
   ListBox->Items->BeginUpdate();
   //insere Eqptos
   for (int i = 0; i < lisEQP->Count; i++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[i];
      //verifica se o Eqpto j� foi inclu�do
      //if (ListBox->Items->IndexOfObject(eqpto) >= 0) continue;
      if (eqpto->Codigo.IsEmpty())
         {ListBox->Items->AddObject("( Id: " + AnsiString(eqpto->Id) + " )", eqpto);}
      else
         {ListBox->Items->AddObject(eqpto->Codigo, eqpto);}
      //atualiza status do Eqpto p/ indicar que f� foi inclu�do
      }
   //habilita ordena��o
   ListBox->Sorted = true;
   //termina insercao
   ListBox->Items->EndUpdate();
   //seleciona primeiro Eqpto
   if (ListBox->Items->Count > 0) ListBox->ItemIndex = 0;
   //DVK 2014.11.04 destaca lista de equipamentos
   if(CBoxConsulta->ItemIndex == 1) GraficoDestacaEqpto(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::ListBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
   {
   //salva inica��o da tecla Shift
   zoom.shift_state = Shift;
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormLocaliza::LocalizaRede(VTEqpto *eqpto)
   {
   //vari�veis locais
   VTRede *rede;
   TList  *lisREDE;

   //prote��o
   if (eqpto == NULL) return(NULL);
   else if (eqpto->TipoBarra())
      {//procura Barra em todas Rede
      lisREDE = redes->LisRede();
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         if (rede->ExisteBarra((VTBarra*)eqpto)) return(rede);
         }
      }
   else if (eqpto->TipoEqbar())
      {//chamada recursiva
      rede = LocalizaRede(((VTEqbar*)eqpto)->pbarra);
      }
   else if (eqpto->TipoLigacao())
      {
      return(((VTLigacao*)eqpto)->rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::Moldura(VTEqpto *eqpto)
   {
   //exibe moldura no Eqpto indicado
   grafico->Moldura(eqpto);
   }

//---------------------------------------------------------------------------
int __fastcall TFormLocaliza::Operador(void)
   {
   //determina tipo de equipamento selecionado CBoxOperador
   if (CBoxOperador->ItemIndex < 0) return(-1);
   //determina operador selecionado
   return(int(CBoxOperador->Items->Objects[CBoxOperador->ItemIndex]));
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::RadioGroupAreaClick(TObject *Sender)
   {
   //faz nova localiza��o
   ExecutaBusca();
   }

//---------------------------------------------------------------------------
void __fastcall TFormLocaliza::RichEditValorKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
   {
   //vari�veis locais
   int        count;
   AnsiString txt;

   //tecla Enter: executa localizar
   if(Key == VK_RETURN)
      {//elimina return do RichEdit
      //while((count = RichEditValor->Lines->Count) > 1) RichEditValor->Lines->Delete(count-1);
      //RichEditValor->Refresh();
      Key = 0;
      //executa busca
      ExecutaBusca();
      }
   }

//---------------------------------------------------------------------------
int __fastcall TFormLocaliza::TipoEquipamento(void)
   {
   //vari�veis locais
   VTEqpto *eqpto;

   //determina tipo de equipamento selecionado CBoxTipoEqpto
   if (CBoxTipoEqpto->ItemIndex < 0) return(-1);
   //determina Eqpto selecionado p/ obter seu tipo
   eqpto = (VTEqpto*)CBoxTipoEqpto->Items->Objects[CBoxTipoEqpto->ItemIndex];
   if (eqpto == NULL) return(-1);
   return(eqpto->Tipo());
   }

//---------------------------------------------------------------------------
//eof

