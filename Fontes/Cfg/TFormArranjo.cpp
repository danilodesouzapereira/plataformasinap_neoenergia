//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormArranjo.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTPtoFixacao.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTMatC.h"
#include "..\Rede\VTMatZ.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormArranjo::TFormArranjo(TComponent* Owner, VTApl *apl, VTArranjo *arranjo)
                        :TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl          = apl;
   this->arranjo_orig = arranjo;
   fases              = (VTFases*)apl->GetObject(__classid(VTFases));
   arranjos           = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   //define uso do c�digo ABCN p/ as fases dos Trechos
   fases->DefCodigoABC();
   //cria clone do Arranjo
   this->arranjo = arranjo_orig->Clone();   
	//se Arranjo existente, bloqueia altera��o do n�mero de fases
//	GroupBoxfase->Enabled = arranjo->Status[sttNOVO];  //DVK 2015.09.30 comentado
	}

//---------------------------------------------------------------------------
__fastcall TFormArranjo::~TFormArranjo(void)
   {
   //destr�i objetos criados
   if (arranjo) {delete arranjo; arranjo = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ActionAlteraCaboFaseExecute(TObject *Sender)
   {
   //l� Cabo selecionado para as fase
   LeCaboDasFases();
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ActionAlteraCaboZ0Z1Execute(TObject *Sender)
   {
   //l� Cabo pr�-reunido selecionado
   LeCaboZ0Z1();
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ActionAlteraFasesExecute(TObject *Sender)
   {
   //l� campos edit�veis p/ salvar eventuais altera��es
   LeDadosEditaveis();
   //l� fase selecionada p/ o Arranjo
   LeFases();
   //atualiza tela
   ExibeDados();
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ActionAlteraPtoFixacaoFaseExecute(TObject *Sender)
   {
   //l� PtoFixacao selecionado
   LePtoFixacaoDasFases();
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ActionAlteraSuporteExecute(TObject *Sender)
   {
   //l� campos edit�veis p/ salvar eventuais altera��es
   LeDadosEditaveis();
   //l� Suporte selecionado
   //LeSuporte();
   //atualiza tela
   ExibeDados();
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ActionAlteraTipoCfgExecute(TObject *Sender)
	{
	//vari�veis locais
	TFase *fase;

	//l� campos edit�veis ANTES de ler o novo tipo de configura��o
	LeDadosEditaveis();
	//l� tipo de configura��o selecionado DEPOIS de ler dados edit�veis
	LeTipoConfiguracao();
	//reinicia CBoxFase c/ as Fases compat�veis c/ tipo de arranjo
	IniciaCBoxFase();
   //l� fase selecionada p/ o Arranjo
   LeFases();
   //calcula matriz de imped�ncias do Arranjo
   CalculaMatZ();
	//atualiza dados na tela
   ExibeDados();
	}

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ActionConfirmaExecute(TObject *Sender)
   {
	//l� todos os dados
	LeDados();
   //valida dados
   if (! ValidaDados()) return;
   //calcula Matriz de imped�ncias
   if (! CalculaMatZ())
      {
      Erro("N�o foi poss�vel calcular a matriz de imped�ncias do arranjo");
      return;
      }
   //atualiza status indicando que o Arranjo foi alterado
   arranjo->Status[sttALTERADO] = true;
   //atualiza Arranjo original
   arranjo_orig->CopiaAtributosDe(*arranjo);
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::CalculaMatZ(void)
   {
   //vari�veis locais
   int      vet_fase[] = {faseA, faseB, faseC, faseN};
   VTMontaZ *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));
   
   //prote��o: verifica se todos os dados est�o dispon�veis p/ calcular a matriz de imped�ncias
   switch(arranjo->TipoCfg)
      {
      case arranjoCABOxFASE:
         //verifica se o Suporte est� definido
         if (arranjo->suporte == NULL) return(false);
         //verifica se os cabos das fases est�o definidos
         for (int ind_fase = indFASE_A, fase = faseA; ind_fase <= indNEUTRO; ind_fase++, fase++)
            {
            if (arranjo->Fases & vet_fase[ind_fase])
               {
               if (arranjo->fase[ind_fase].pto_fixacao == NULL) return(false);
               if (arranjo->fase[ind_fase].cabo        == NULL) return(false);
               }
            }
         break;
      case arranjoCABO_Z0Z1:
         //verifica se o caboZ0Z1 est� definido
         if (arranjo->caboZ0Z1 == NULL) return(false);
         break;
      case arranjoZ0Z1:
      case arranjoPU_SB100:
         //verifica se h� imped�ncia de sequencia zerada
         if (IsDoubleZero(arranjo->z0.r) && IsDoubleZero(arranjo->z0.x)) return(false);
         if (IsDoubleZero(arranjo->z1.r) && IsDoubleZero(arranjo->z1.x)) return(false);
			break;
      case arranjoMAT_Z:
      case arranjoMUTUA:
         //matriz � editada, n�o precisa ser calculada
         return(true);
      default:
         return(false);
		}
   //executa c�lculo
	return(montaZ->Executa(arranjo));
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::CBoxCaboZ0Z1Change(TObject *Sender)
   {
   LeCaboZ0Z1();
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ExibeDados(void)
   {
   //atualiza CBoxFase c/ a fase do Arranjo
   CBoxFase->ItemIndex = CBoxFase->Items->IndexOf(fases->AsStrABCN[arranjo->Fases]);
   //esconde todos GBox
   GBoxSupCabo->Hide();
   GBoxLimite->Hide();
   GBoxCaboZ0Z1->Hide();
   GBoxSuporte->Hide();
   //esconde PageControl e todos seus TabSheet
   PageControl->Visible = false;
   for (int n = 0; n < PageControl->PageCount; n++)
      {
      PageControl->Pages[n]->Visible    = false;
      PageControl->Pages[n]->TabVisible = false;
      }
   //exibe dados de acordo c/ tipo de configura��o selecionado em RGroupTipo
   switch(arranjo->TipoCfg)
      {
      case arranjoCABOxFASE:
         ExibeDadosSuporteCabo();
         break;
      case arranjoCABO_Z0Z1:
         ExibeDadosCaboZ0Z1();
         break;
      case arranjoZ0Z1:
         ExibeDadosLimite();
         ExibeDadosZ0Z1();
         break;
      case arranjoMAT_Z:
         ExibeDadosLimite();
         ExibeDadosMatZ();
         ExibeDadosMatC();
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ExibeDadosCaboZ0Z1(void)
   {
   //esconde GBoxSupCabo, GBoxMatZ, GBoxLimite (nesta ordem)
   //GBoxSupCabo->Hide();
   //GBoxMatZ->Hide();
   //GBoxLimite->Hide();
   //exibe GBoxSuporte e GBoxCaboZ0Z1
   GBoxSuporte->Show();
   GBoxCaboZ0Z1->Show();
   //pr�-seleciona Suporte do Arranjo
   CBoxSuporte->ItemIndex = CBoxSuporte->Items->IndexOfObject(arranjo->suporte);
   //pr�-seleciona Cabo pr�-reunido do Arranjo
   CBoxCaboZ0Z1->ItemIndex = CBoxCaboZ0Z1->Items->IndexOfObject(arranjo->caboZ0Z1);
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ExibeDadosLimite(void)
   {
   //corrente admiss�vel
   EditIadm->Text = DoubleToStr("%2.1f", arranjo->Ifase_amp[0]);
   //limites de tens�o
   EditVmin->Text = DoubleToStr("%4.3f", arranjo->Vmin_KV);
   EditVmax->Text = DoubleToStr("%4.3f", arranjo->Vmax_KV);
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ExibeDadosMatC(void)
   {
   //vari�veis locais
	int          dim = arranjo->MatC->Dimensao;
   double       cap;
   TStringGrid *SGrid = SGridMatC;

   //exibe PageControl e TabSheetMatC
   PageControl->Visible     = true;
   TabSheetMatC->Visible    = true;
   TabSheetMatC->TabVisible = true;
   //configura SGrid
   SGrid->DefaultColWidth = 75;
   SGrid->RowCount        = dim+1;
   SGrid->ColCount        = dim+1;
   //preenche t�tulos das linhas e colunas
   SGrid->Cells[0][0] = "";
   for (int i = 1; i <= dim; i++)
      {
      SGrid->Cells[i][0] = IntToStr(i);
      SGrid->Cells[0][i] = IntToStr(i);
      }
   //preenche dados da MatC
   for (int i = 0; i < dim; i++)
      {
      for (int j = 0; j < dim; j++)
         {
         cap = arranjo->MatC->Cap[i][j];
         SGrid->Cells[j+1][i+1] = DoubleToStr("%5.4f", cap);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ExibeDadosMatZ(void)
   {
   //vari�veis locais
	int          dim = arranjo->MatZ->Dimensao;
   double       r, x;
   TStringGrid *SGrid = SGridMatZ;

   //exibe GBoxSuporte, GBoxLimite, PageControl  e TabSheetMatZ
   GBoxSuporte->Visible     = true;
   GBoxLimite->Visible      = true;
   PageControl->Visible     = true;
   TabSheetMatZ->Visible    = true;
   TabSheetMatZ->TabVisible = true;
   //pr�-seleciona Suporte do Arranjo
   CBoxSuporte->ItemIndex = CBoxSuporte->Items->IndexOfObject(arranjo->suporte);
   //configura SGrid
   SGrid->DefaultColWidth = 90;
   SGrid->RowCount        = dim+1;
   SGrid->ColCount        = dim+1;
   //preenche t�tulos das linhas e colunas
   SGrid->Cells[0][0] = "";
   for (int i = 1; i <= dim; i++)
      {
      SGrid->Cells[i][0] = IntToStr(i);
      SGrid->Cells[0][i] = IntToStr(i);
      }
   //preenche dados da MatZ
   for (int i = 0; i < dim; i++)
      {
      for (int j = 0; j < dim; j++)
         {
         r = arranjo->MatZ->R[i][j];
         x = arranjo->MatZ->X[i][j];
         SGrid->Cells[j+1][i+1] = ComplexToStr("%5.4f", r, x);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ExibeDadosSuporteCabo(void)
   {
   //vari�veis locais
   TCheckBox *CheckBox[MAX_FASE] = {CheckBoxFaseA,       CheckBoxFaseB,       CheckBoxFaseC,       CheckBoxNeutro};
   TComboBox *CBoxCabo[MAX_FASE] = {CBoxCaboFaseA,       CBoxCaboFaseB,       CBoxCaboFaseC,       CBoxCaboNeutro};
   TComboBox *CBoxPto[MAX_FASE]  = {CBoxPtoFixacaoFaseA, CBoxPtoFixacaoFaseB, CBoxPtoFixacaoFaseC, CBoxPtoFixacaoNeutro};
   TPanel    *Panel[MAX_FASE]    = {PanelFaseA,          PanelFaseB,          PanelFaseC,          PanelNeutro};

   //esconde GBoxSuporte, GBoxMatZ, GBoxCaboZ0Z1, GBoxLimite (nesta ordem)
   //GBoxSuporte->Hide();
	//GBoxMatZ->Hide();
   //GBoxCaboZ0Z1->Hide();
   //GBoxLimite->Hide();
   //exibe GBoxSupCabo
   GBoxSupCabo->Show();
   //exibe valor de Res.Terra
   EditResTerra->Text = DoubleToStr("%2.1f", arranjo->ResTerra);
   //seleciona Suporte do Arranjo
   CBoxSupAereo->ItemIndex = CBoxSupAereo->Items->IndexOfObject(arranjo->suporte);
   //inicia CBoxPtoFixacao
   IniciaCBoxPtoFixacao();
   //esconde Panels das fases n�o utilizadas (ordem inversa p/ n�o haver reoordena��o da tela)
   for (int nf = MAX_FASE-1; nf >= 0; nf--)
      {//verifica se as fases do Arranjo inclui a fase nf
		Panel[nf]->Visible = fases->Fase1ContemFase2(arranjo->Fases, fases->IndexToTag[nf]);
      }
   //seleciona PtoFixacao e cabo de cada fase
    for (int nf = 0; nf < MAX_FASE; nf++)
      {//verifica se as fases do Arranjo inclui a fase nf
      if (fases->Fase1ContemFase2(arranjo->Fases, fases->IndexToTag[nf]))
         {
         //Panel[nf]->Visible      = true;
         CheckBox[nf]->Checked   = true;
         CBoxCabo[nf]->ItemIndex = CBoxCabo[nf]->Items->IndexOfObject(arranjo->fase[nf].cabo);
         CBoxPto[nf]->ItemIndex  = CBoxPto[nf]->Items->IndexOfObject(arranjo->fase[nf].pto_fixacao);
         }
      else
         {
         //Panel[nf]->Visible      = false;
         CheckBox[nf]->Checked   = false;
         CBoxCabo[nf]->ItemIndex = -1;
         CBoxPto[nf]->ItemIndex  = -1;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::ExibeDadosZ0Z1(void)
   {
   //vari�veis locais
   TStringGrid *SGrid = SGridZ0Z1;

   //exibe GBoxSuporte, GBoxLimite, PageControl e TabSheetZ0Z1
   GBoxSuporte->Visible     = true;
   GBoxLimite->Visible      = true;
   PageControl->Visible     = true;
   TabSheetZ0Z1->TabVisible = true;
   TabSheetZ0Z1->Visible    = true;
   //pr�-seleciona Suporte do Arranjo
   CBoxSuporte->ItemIndex = CBoxSuporte->Items->IndexOfObject(arranjo->suporte);
   //preenche GBoxMatZ
   //define dimens�o de SGridZ
   SGrid->RowCount = 3;
   SGrid->ColCount = 4;
   SGrid->DefaultColWidth = 60;
   //preenche t�tulos das colunas
   SGrid->Cells[0][0] = "";
   SGrid->Cells[1][0] = "r(ohm/km)";
   SGrid->Cells[2][0] = "x(ohm/km)";
   SGrid->Cells[3][0] = "c(nF/km)";
   //preenche linha de Seq.0
   SGrid->Cells[0][1] = "Seq.0";
   SGrid->Cells[1][1] = DoubleToStr("%5.4f", arranjo->z0.r);
   SGrid->Cells[2][1] = DoubleToStr("%5.4f", arranjo->z0.x);
   SGrid->Cells[3][1] = DoubleToStr("%5.4f", arranjo->c0);
   //preenche linha de Seq.1
   SGrid->Cells[0][2] = "Seq.1";
   SGrid->Cells[1][2] = DoubleToStr("%5.4f", arranjo->z1.r);
   SGrid->Cells[2][2] = DoubleToStr("%5.4f", arranjo->z1.x);
   SGrid->Cells[3][2] = DoubleToStr("%5.4f", arranjo->c1);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TFormArranjo::ExisteArranjo(AnsiString codigo)
   {
   //vari�veis locais
   VTArranjo *um_arranjo;
   TList     *lisARR = arranjos->LisArranjo();

   //procura Arranjo c/ o c�digo indicado
   for (int n = 0; n < lisARR->Count; n++)
      {
      um_arranjo = (VTArranjo*)lisARR->Items[n];
      //desconsidera o Arranjo sendo editado
      if (um_arranjo == arranjo_orig) continue;
      if (um_arranjo->Codigo.AnsiCompareIC(codigo) == 0) return(um_arranjo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::FormShow(TObject *Sender)
   {
	//seleciona tipo de configura��o do Arranjo
	RGroupTipo->OnClick = NULL;
	RGroupTipo->ItemIndex = arranjo->TipoCfg;
	RGroupTipo->OnClick = ActionAlteraTipoCfgExecute;
	//inicia CBoxFase
	IniciaCBoxFase();
	//inicia CBoxCaboZ0Z1
	IniciaCBoxCaboZ0Z1();
	//inicia CBoxSupAereo
	IniciaCBoxSupAereo();
	IniciaCBoxSuporte();
	//inicia CBoxCaboFaseA, CBoxCaboFaseB, CBoxCaboFaseC, CBoxCaboNeutro
	IniciaCBoxCaboFase();
	//exibe c�digo do Arranjo
	EditCodigo->Text = arranjo->Codigo;
	//exibe indica��o de ramal
	CheckBoxRamal->Checked = arranjo->ramal;
	//exibe dados de acordo c/ tipo do Arranjo
	ExibeDados();
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::IniciaCBoxCaboFase(void)
   {
   //vari�veis locais
   VTCabo    *cabo;
   TComboBox *CBoxCabo[] = {CBoxCaboFaseA, CBoxCaboFaseB, CBoxCaboFaseC, CBoxCaboNeutro};
   TList     *lisCABO    = arranjos->LisCabo();

   //preenche todos os ComboBox de Cabo de fase
   for (int n = 0; n < sizeof(CBoxCabo)/sizeof(TComboBox*); n++)
      {//reinicia CBoxCabo
      CBoxCabo[n]->Clear();
      //insere todos os Cabos exceto os pr�-reunidos
      for (int nc = 0; nc < lisCABO->Count; nc++)
         {
         cabo = (VTCabo*)lisCABO->Items[nc];
         //filtra cabo exclu�do
         if (cabo->Status[sttEXCLUIDO]) continue;
         //n�o inclui Cabo pr�-reunido
         if (cabo->TipoCfg == caboPRE_REUNIDO) continue;
         CBoxCabo[n]->Items->AddObject(cabo->Codigo, cabo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::IniciaCBoxCaboZ0Z1(void)
   {
   //vari�veis locais
   VTCabo *cabo;
   TList  *lisCABO = arranjos->LisCabo();

   //reinicia CBoxCaboZ0Z1
   CBoxCaboZ0Z1->Clear();
   //insere somente os Cabos pr�-reunidos
   for (int nc = 0; nc < lisCABO->Count; nc++)
      {
      cabo = (VTCabo*)lisCABO->Items[nc];
      //filtra cabo exclu�do
      if (cabo->Status[sttEXCLUIDO]) continue;
      //inclui somente Cabo pr�-reunido
      if ((cabo->TipoCfg == caboPRE_REUNIDO)||(cabo->TipoCfg == caboZ0Z1))
         {
         CBoxCaboZ0Z1->Items->AddObject(cabo->Codigo, cabo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::IniciaCBoxFase(void)
   {
   //vari�veis locais
   TFase      *fase;
	TList      *lisFASE;

	//obt�m fases v�lidas p/ o tipo de configura��o
	switch(RGroupTipo->ItemIndex)
   	{
		case 0:	//arranjoCABOxFASE
			lisFASE = fases->LisFasesTrecho();
			break;
		case 1:	//arranjoCABO_Z0Z1
			lisFASE = fases->LisFasesTrechoTrifasico();
			break;
		case 2:	//arranjoZ0Z1
			//lisFASE = fases->LisFasesTrechoTrifasico();
			lisFASE = fases->LisFasesTrecho();
			break;
		case 3:	//arranjoMAT_Z
			lisFASE = fases->LisFasesTrecho();
			break;
		default:
			lisFASE = fases->LisFasesTrecho();
			break;
		}
   //reinicia CBoxFase
   CBoxFase->Clear();
   for (int n = 0; n < lisFASE->Count; n++)
      {
      fase = (TFase*)lisFASE->Items[n];
      CBoxFase->Items->AddObject(fase->codigo, fase);
      }
	//pr�-seleciona fases do Arranjo
	CBoxFase->ItemIndex = CBoxFase->Items->IndexOfObject(fases->TagToFase[arranjo->Fases]);
	if (CBoxFase->ItemIndex < 0)
		{//pr�-seleciona fases ABC
		CBoxFase->ItemIndex = CBoxFase->Items->IndexOfObject(fases->TagToFase[faseABC]);
		}
	if (CBoxFase->ItemIndex < 0)
		{//pr�-seleciona primeira fase do CBox ABC
		CBoxFase->ItemIndex = 0;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::IniciaCBoxPtoFixacao(void)
   {
   //vari�veis locais
   VTPtoFixacao *pto_fixacao;
   TList        *lisPTO;
   TComboBox    *CBoxPto[] = {CBoxPtoFixacaoFaseA, CBoxPtoFixacaoFaseB, CBoxPtoFixacaoFaseC, CBoxPtoFixacaoNeutro};

   //reinicia todos CBoxPto
   for (int n = 0; n < sizeof(CBoxPto)/sizeof(TComboBox*); n++)
      {//reinicia CBoxCabo
      CBoxPto[n]->Clear();
      }
   //prote��o
   if (arranjo->suporte == NULL) return;
   //obt�m lista de PtoFixacao do Suporte
   lisPTO = arranjo->suporte->LisPtoFixacao();
   //preenche todos os ComboBox de PtoFixacao
   for (int n = 0; n < sizeof(CBoxPto)/sizeof(TComboBox*); n++)
      {//insere todos os PtoFixacao
      for (int np = 0; np < lisPTO->Count; np++)
         {
         pto_fixacao = (VTPtoFixacao*)lisPTO->Items[np];
         CBoxPto[n]->Items->AddObject(pto_fixacao->codigo, pto_fixacao);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::IniciaCBoxSupAereo(void)
   {
   //vari�veis locais
   VTSuporte *suporte;
   TList     *lisSUP = arranjos->LisSuporte();

   //reinicia CBoxSupAereo
   CBoxSupAereo->Clear();
   //insere todos os Suporte
   for (int n = 0; n < lisSUP->Count; n++)
      {
      suporte = (VTSuporte*)lisSUP->Items[n];
      if (suporte->TipoDeSuporte == suporteAEREO)
         {
         CBoxSupAereo->Items->AddObject(suporte->Codigo, suporte);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::IniciaCBoxSuporte(void)
   {
   //vari�veis locais
   VTSuporte *suporte;
   TList     *lisSUP = arranjos->LisSuporte();

   //reinicia CBoxSuporte
   CBoxSuporte->Clear();
   //insere todos os Suporte
   for (int n = 0; n < lisSUP->Count; n++)
      {
      suporte = (VTSuporte*)lisSUP->Items[n];
      CBoxSuporte->Items->AddObject(suporte->Codigo, suporte);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeCaboDasFases(void)
   {
   //vari�veis locais
   int       multiplicidade = 1;
   VTCabo    *cabo;
   TComboBox *CBoxCabo[MAX_FASE] = {CBoxCaboFaseA, CBoxCaboFaseB, CBoxCaboFaseC, CBoxCaboNeutro};

   //determina Cabo de cada fase
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//verifica se as fases do Arranjo inclui a fase nf
      if (fases->Fase1ContemFase2(arranjo->Fases, fases->IndexToTag[nf]))
         {//Arranjo utiliza a fase nf: determina o Cabo definido
         if (CBoxCabo[nf]->ItemIndex < 0)
            {
            cabo           = NULL;
            }
         else
            {
            cabo           = (VTCabo*)CBoxCabo[nf]->Items->Objects[CBoxCabo[nf]->ItemIndex];
            }
         arranjo->DefCaboFase(nf, cabo, multiplicidade);
         }
      else
         {//arranjo n�o utiliza a fase nf
         arranjo->DefCaboFase(nf, NULL, 0);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeCaboZ0Z1(void)
   {
   //vari�veis locais
   VTCabo *cabo;

   //prote��o
   if (CBoxCaboZ0Z1->ItemIndex < 0)
      {cabo = NULL;}
   else
      {cabo = (VTCabo*)CBoxCaboZ0Z1->Items->Objects[CBoxCaboZ0Z1->ItemIndex];}
   //redefine Cabo pr�-reunido do Arranjo
   arranjo->DefCaboZ0Z1(cabo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeCodigo(void)
   {
   //l� c�digo do Arranjo
   arranjo->Codigo = EditCodigo->Text;
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeDados(void)
   {
   //l� c�digo do Arranjo
   LeCodigo();
   //l� fases do Arranjo
	LeFases();
	//l� informa��o de ramal
	LeRamal();
	//l� tipo de configura��o do Arranjo
   LeTipoConfiguracao();
   switch(arranjo->TipoCfg)
      {
      case arranjoCABOxFASE:
         LeSuporteAereo();
         LeCaboDasFases();
         LePtoFixacaoDasFases();
         LeResTerra();
         break;
      case arranjoCABO_Z0Z1:
         LeSuporte();
         LeCaboZ0Z1();
         break;
      case arranjoZ0Z1:
         LeSuporte();
         LeLimites();
         LeZ0Z1();
         break;
      case arranjoMAT_Z:
         LeSuporte();
         LeLimites();
         LeMatC();
         LeMatZ();
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeDadosEditaveis(void)
   {
   //l� c�digo do Arranjo
   LeCodigo();
   //l� fases do Arranjo
   //LeFases();
   //IMPORTANTE: l� dados de acordo com a configura��o atual do Arranjo e
   //            N�O de acordo com RadioGroupTipo
   switch(arranjo->TipoCfg)
      {
      case arranjoCABOxFASE:
         LeSuporteAereo();
         LeCaboDasFases();
         //LePtoFixacaoDasFases();
         LeLimites();
         LeResTerra();
         break;
      case arranjoCABO_Z0Z1:
         LeSuporte();
         break;
      case arranjoZ0Z1:
         LeSuporte();
         LeLimites();
         LeZ0Z1();
         break;
      case arranjoMAT_Z:
         LeSuporte();
         LeMatC();
         LeMatZ();
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeFases(void)
   {
   //vari�veis locais
   TFase *fase;

   if (CBoxFase->ItemIndex < 0)
      {arranjo->Fases = faseINV;}
   else
      {//determina fase selecionada
      fase = (TFase*)CBoxFase->Items->Objects[CBoxFase->ItemIndex];
      //redefine fases do Arranjo
      arranjo->Fases = fase->tag;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeLimites(void)
   {
   try{//l� valores limites
      arranjo->Iadm_amp = StrToDouble(EditIadm->Text);
      arranjo->Vmin_KV  = StrToDouble(EditVmin->Text);
      arranjo->Vmax_KV  = StrToDouble(EditVmax->Text);
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeMatC(void)
	{
	//vari�veis locais
	AnsiString   txt;
   double       cap;
	int          dim = arranjo->MatC->Dimensao;
   TStringGrid *SGrid = SGridMatC;

	//zera todos elementos de MatC
	arranjo->MatC->Zera();
	try{//l� matriz c/ valores complexos
		for (int i = 0; i < dim; i++)
         {
			for (int j = 0; j < dim; j++)
            {
            cap = StrToDouble(SGrid->Cells[j+1][i+1]);
				arranjo->MatC->Cap[i][j] = cap;
            }
         }
      }catch(Exception &e)
         {//nada a fazer
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeMatZ(void)
   {
	//vari�veis locais
	AnsiString   txt;
   double       r, x;
	int         dim = arranjo->MatZ->Dimensao;
   TStringGrid *SGrid = SGridMatZ;

	//zera todos elementos de MatZ
	arranjo->MatZ->Zera();
	try{//l� matriz c/ valores complexos
		for (int i = 0; i < dim; i++)
         {
         for (int j = 0; j < dim; j++)
            {
				if (! LeStrComplexo(SGrid->Cells[j+1][i+1], r, x))
               {r = x = 0.;}
            arranjo->MatZ->R[i][j] = r;
            arranjo->MatZ->X[i][j] = x;
            }
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LePtoFixacaoDasFases(void)
   {
   //vari�veis locais
   VTPtoFixacao *pto_fixacao;
   TComboBox    *CBoxPto[MAX_FASE] = {CBoxPtoFixacaoFaseA, CBoxPtoFixacaoFaseB, CBoxPtoFixacaoFaseC, CBoxPtoFixacaoNeutro};

   //determina PtoFixacao de cada fase
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//verifica se as fases do Arranjo inclui a fase nf
      if (fases->Fase1ContemFase2(arranjo->Fases, fases->IndexToTag[nf]))
			{//Arranjo utiliza a fase nf: determina o PtoFixacao definido
         if (CBoxPto[nf]->ItemIndex < 0)
            {pto_fixacao = NULL;}
         else
            {pto_fixacao = (VTPtoFixacao*)CBoxPto[nf]->Items->Objects[CBoxPto[nf]->ItemIndex];}
         arranjo->DefPtoFixacaoFase(nf, pto_fixacao);
         }
      else
         {//arranjo n�o utiliza a fase nf
         arranjo->DefPtoFixacaoFase(nf, NULL);
         }
      }
   }
//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeRamal(void)
	{
	arranjo->ramal = CheckBoxRamal->Checked;
	}

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeResTerra(void)
	{
	//vari�veis locais
	double res_terra;

	try{
		res_terra = StrToDouble(EditResTerra->Text);
		}catch(Exception &e)
			{//assume valor inv�lido
			res_terra = 0.;
			}
	//atualiza arranjo
	arranjo->ResTerra = res_terra;
	}

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::LeStrComplexo(AnsiString txt, double &r, double &x)
   {
   //vari�veis locais
   int        ind_x, len;
   char       *pc;
   AnsiString aux, txt_r, txt_x;

   //reinicia r e x
   r = x = 0.;
   //elimina caracteres ' ' de txt
   aux = "";
   len = txt.Length();
   for (int n = 1; n <= len; n++)
      {
      if (txt[n] != ' ') aux += txt[n];
      }
   txt = aux;
   //verifica se string vazio
   if (txt.IsEmpty()) return(true);
   //procura os string "+j", "-j", "+i", "-i"
   ind_x = aux.AnsiPos("+j");
   if (ind_x == 0) ind_x = aux.AnsiPos("-j");
   if (ind_x == 0) ind_x = aux.AnsiPos("+i");
   if (ind_x == 0) ind_x = aux.AnsiPos("-i");
   if (ind_x == 0)
      {//n�o existe parte imagin�ria
      txt_r = txt;
      }
   else
      {//encontrou parte imagin�ria: separa strings da parte real e imagin�ria
      len = txt.Length();
      for (int n = 1; n <= len; n++)
         {
         if ((txt[n] == 'j')||(txt[n] == 'i')) continue;
         //salva caractere em txt_r ou txt_x
         (n < ind_x) ? txt_r += txt[n] : txt_x += txt[n];
         }
      }
   try{//converte valores
      r = StrToDouble(txt_r);
      x = StrToDouble(txt_x);
      }catch(Exception &e)
         {return(false);}
   //executou leitura corretamente
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeSuporte(void)
   {
   //vari�eis locais
   VTSuporte *suporte;

   //prote��o
   if (CBoxSuporte->ItemIndex < 0)
      {suporte = NULL;}
   else
      {suporte = (VTSuporte*)CBoxSuporte->Items->Objects[CBoxSuporte->ItemIndex];}
   //atualiza Suporte do Arranjo
   arranjo->suporte = suporte;
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeSuporteAereo(void)
   {
   //vari�eis locais
   VTSuporte *suporte;

   //prote��o
   if (CBoxSupAereo->ItemIndex < 0)
      {suporte = NULL;}
   else
      {suporte = (VTSuporte*)CBoxSupAereo->Items->Objects[CBoxSupAereo->ItemIndex];}
   //atualiza Suporte do Arranjo
   arranjo->suporte = suporte;
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeTipoConfiguracao(void)
   {
   //atualiza tipo de configura��o do Arranjo
   if (RGroupTipo->ItemIndex < 0)
      {arranjo->TipoCfg = arranjoZ0Z1;}
   else
      {arranjo->TipoCfg = RGroupTipo->ItemIndex;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormArranjo::LeZ0Z1(void)
   {
   //vari�veis locais
   double      c0, c1;
   strIMP      z0, z1;
   TStringGrid *SGrid = SGridZ0Z1;

   try{//l� Z0
      z0.r = StrToDouble(SGrid->Cells[1][1]);
      z0.x = StrToDouble(SGrid->Cells[2][1]);
      c0   = StrToDouble(SGrid->Cells[3][1]);
      }catch(Exception &e)
         {//assume valor inv�lido
         z0.r = z0.x = c0 = 0.;
         }
   try{//l� Z1
      z1.r = StrToDouble(SGrid->Cells[1][2]);
      z1.x = StrToDouble(SGrid->Cells[2][2]);
      c1   = StrToDouble(SGrid->Cells[3][2]);
      }catch(Exception &e)
         {//assume valor inv�lido
         z1.r = z1.x = c1 = 0.;
         }
   //atualiza arranjo
   arranjo->z0 = z0;
   arranjo->c0 = c0;
   arranjo->z1 = z1;
   arranjo->c1 = c1;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaCaboDasFases(void)
   {
   //determina Cabo de cada fase
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//verifica se as fases do Arranjo inclui a fase nf
      if (fases->Fase1ContemFase2(arranjo->Fases, fases->IndexToTag[nf]))
         {//Arranjo utiliza a fase nf: verifica se o Cabo est� definido
         if (arranjo->fase[nf].cabo == NULL)
            {
            Erro("Defina o cabo para a fase " + fases->AsStrABCN[fases->IndexToTag[nf]]);
            return(false);
            }
         }
      else
         {//arranjo n�o utiliza a fase nf
         arranjo->fase[nf].cabo = NULL;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaCaboPreReunido(void)
   {
   //verifica se o caboZ0Z1 est� definido
   if (arranjo->caboZ0Z1 == NULL)
      {
      Erro("Cabo e inv�lido");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaCodigo(void)
   {
   //verifica c�digo vazio
   if (arranjo->Codigo.IsEmpty())
      {
      Erro("Defina o c�digo do arranjo");
      return(false);
      }
   //verifica se existe outro Arranjo c/ mesmo c�digo
   if (ExisteArranjo(arranjo->Codigo) != NULL)
      {
      Erro("Existe outro arranjo com o c�digo definido");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaDados(void)
   {
   //valida c�digo do Arranjo
   if (! ValidaCodigo()) return(false);
   //valida fases do Arranjo
   if (! ValidaFases()) return(false);
   //valida tipo de configura��o do Arranjo
   if (! ValidaTipoConfiguracao()) return(false);
   //valida dados dependentes do tipo de configura��o
   switch(arranjo->TipoCfg)
      {
      case arranjoCABOxFASE:
         if (! ValidaSuporte())            return(false);
         if (! ValidaCaboDasFases())       return(false);
         if (! ValidaPtoFixacaoDasFases()) return(false);
         if (! ValidaResTerra())           return(false);
         break;
      case arranjoCABO_Z0Z1:
         if (! ValidaCaboPreReunido())     return(false);
         break;
      case arranjoZ0Z1:
         if (! ValidaLimites()) return(false);
         if (! ValidaZ0Z1())    return(false);
         break;
      case arranjoMAT_Z:
         if (! ValidaLimites()) return(false);
         if (! ValidaMatZ())    return(false);
         break;
      }
   //dados lidos corretamente
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaFases(void)
   {
   //verifica se a fase do Arranjo est� definida
   if (arranjo->Fases == faseINV)
      {
      Erro("Defina as fases do arranjo");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaLimites(void)
   {
   //corrente admiss�vel
   if (arranjo->Ifase_amp[0] <= 0.)
      {
      Erro("Corrente admiss�vel c/ valor inv�lido");
      return(false);
      }
   //tens�es m�nima e m�xima
   if (arranjo->Vmin_KV < 0.) arranjo->Vmin_KV = 0;
   if (arranjo->Vmax_KV < 0.) arranjo->Vmax_KV = 0;
   if ((arranjo->Vmin_KV > 0.)&&(arranjo->Vmax_KV > 0.)&&(arranjo->Vmin_KV > arranjo->Vmax_KV))
      {
      Erro("Tens�o m�nima do arranjo � maior que a tens�o m�xima");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaMatZ(void)
   {
   //vari�veis locais
	AnsiString   txt;
   double       r, x;
	int          dim   = arranjo->MatZ->Dimensao;
   TStringGrid *SGrid = SGridMatZ;

   try{//l� matriz c/ valores complexos
		for (int i = 0; i < dim; i++)
         {
         for (int j = 0; j < dim; j++)
            {
            if (! LeStrComplexo(SGrid->Cells[j+1][i+1], r, x))
               {
               txt.sprintf("Elemento [%d][%d] da MatZ c/ valor inv�lido", i, j);
               Erro(txt);
               return(false);
               }
            }
         }
      }catch(Exception &e)
         {
         Erro("MatZ c/ valor inv�lido");
         return(false);
         }
   return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaPtoFixacaoDasFases(void)
   {
   //valida PtoFixacao de cada fase
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//verifica se as fases do Arranjo inclui a fase nf
      if (fases->Fase1ContemFase2(arranjo->Fases, fases->IndexToTag[nf]))
         {//Arranjo utiliza a fase nf
         if (arranjo->fase[nf].pto_fixacao == NULL)
            {
				Erro("Defina o ponto de fixa��o para a fase " + fases->AsStrABCN[fases->IndexToTag[nf]]);
            return(false);
            }
         }
      else
         {//arranjo n�o utiliza a fase nf
         arranjo->DefPtoFixacaoFase(nf, NULL);
         }
      }
   //verifica se um mesmo PtoFixacao foi selecionada p/ mais de uma fase
   for (int i = 0; i < MAX_FASE-1; i++)
      {
      if (arranjo->fase[i].pto_fixacao == NULL) continue;
      for (int j = i+1; j < MAX_FASE; j++)
         {
         if (arranjo->fase[i].pto_fixacao == arranjo->fase[j].pto_fixacao)
            {
            Erro("Um mesmo ponto de fixa��o foi selecionado para mais de uma fase");
            return(false);
            }
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaResTerra(void)
   {
   if (arranjo->ResTerra <= 0)
      {
      Erro("Resistividade de terra c/ valor inv�lido");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaSuporte(void)
   {
   if (arranjo->suporte == NULL)
      {
      Erro("Defina o suporte usitlizado pelo arranjo");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaTipoConfiguracao(void)
   {
   //valida tipo de configura��o do Arranjo
   if ((arranjo->TipoCfg < arranjoCABOxFASE)||(arranjo->TipoCfg >arranjoMAT_Z))
      {
      Erro("Defina o tipo de configura��o do arranjo");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormArranjo::ValidaZ0Z1(void)
   {
   //valida Z0
   if ((arranjo->z0.r <= 0.)&&(arranjo->z0.x <= 0.))
      {
      Erro("Imped�ncia de seq.0 c/ valor nulo ou negativo");
      return(false);
      }
   //valida Z1
   if ((arranjo->z1.r <= 0.)&&(arranjo->z1.x <= 0.))
      {
      Erro("Imped�ncia de seq.1 c/ valor nulo ou negativo");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

