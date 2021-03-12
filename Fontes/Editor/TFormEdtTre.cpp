//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormEdtTre.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\MontaZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtTre::TFormEdtTre(TComponent* Owner, VTApl *apl, VTTrecho *ptrecho, bool op_retira)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl     = apl;
   this->ptrecho = ptrecho;
   geral         = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //configura opção de retirar
   ActionRetira->Visible = op_retira;
   //cria objeto Arranjo p/ edição de um Arranjo próprio
   arranjo       = DLL_NewObjArranjo();
   //copia dados do Arranjo do Trecho, se houver
   if (ptrecho->arranjo) {arranjo->CopiaAtributosDe(*(ptrecho->arranjo));}
   //IMPORTANTE: redefine Id e Owner
   arranjo->Id      = -1;
   arranjo->Owner   = NULL;
   arranjo->TipoCfg = arranjoZ0Z1;
   //cria objeto MontaZ
   montaZ  = DLL_NewObjMontaZ(apl);
   //cria lista
   lisEQP = new TList();
   lisLIG = new TList();
   //se necessário, define valores default p/ Trecho
   DefValoresDefaultTrecho();
   //inicia CBox c/ os Arranjos
   CBoxArranjoInicia();
   //inicia CBoxFases
   CBoxFasesInicia();
   //inicia CBox c/ as Redes
   IniciaCBoxRede();
   //exibe dados do Trecho e das Chaves
   ExibeDadosTrecho();
   //desabilita divisão de trecho
   CheckBoxDivisao->Checked = false;
   GBoxDivisao->Visible     = false;
   //habilita divisão do Trecho somente se não for um Trecho novo
   if (ptrecho->StatusNovo())
      {//desabilita divisão
      CheckBoxDivisao->Visible = false;
      }
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTre::~TFormEdtTre(void)
   {//variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //apaga moldura
   Moldura();
   //destrói objetos
   if (montaZ)  {delete montaZ;  montaZ  = NULL;}
   if (arranjo) {delete arranjo; arranjo = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP; lisEQP = NULL;}
   if (lisLIG)  {delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::ActionCancela1Execute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::ActionConfirma1Execute(TObject *Sender)
   {
   /*
   //lê e valida dados de planejamento
   if (formEdtPlan)
      {
      if (! formEdtPlan->LeDados()) return;
      }
   */
   //lê e valida dados do Trecho
   if (! LeDadosTrecho())     return;
   if (! ValidaDadosTrecho()) return;
   //lê dados do Arranjo típico
   if (! LeDadosArranjoTipico())      return;
   //lê dados do Arranjo próprio
   if (! LeDadosArranjoProprio())     return;
   if (! ValidaDadosArranjoProprio()) return;
   //associa Trecho com Arranjo típico ou próprio
   if (RGroupArranjo->ItemIndex == 0)
      {//Arranjo típico
      ptrecho->DefineObjArranjo((VTArranjo*)CBoxArranjo->Items->Objects[CBoxArranjo->ItemIndex]);
      }
   else
      {//Arranjo próprio: recalcula MatZ do Arranjo
      montaZ->Executa(arranjo);
      //define Arranjo próprio (IMPORTANTE: cancela Owner do Arranjo)
      arranjo->Owner = NULL;
      ptrecho->DefineObjArranjo(arranjo);
      //cancela ponteiro p/ Arranjo p/ que o destrutor não destrua o objeto 
      arranjo = NULL;
      }
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::ActionRetira1Execute(TObject *Sender)
   {
   //fecha o form indicando exclusão do Trecho
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16351053
void __fastcall TFormEdtTre::CBoxArranjoInicia(void)
   {
   //variáveis locais
   long       newWidth;
   TCanvas    *canvas   = CBoxArranjo->Canvas;
   VTArranjo  *arranjo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisARR   = arranjos->LisArranjo();

   //reinicia CBox c/ os Arranjos
   CBoxArranjo->Clear();
   if (lisARR->Count == 0) return;
   //inclui no CBoxArranjo somente os Arranjos que atendem às restrições de fase das barras
   for (int n = 0; n < lisARR->Count; n++)
	  {
	  arranjo = (VTArranjo*)lisARR->Items[n];
	  //verifica se o Arranjo é obsoleto
	  //if (arranjo->StatusExcluido()) {continue;}
	  if (arranjo->StatusExcluido())
		 {//verifica se é o Arranjo do Trecho
		 if (arranjo == ptrecho->arranjo)
			{//insere Arranjo no CBoxArranjo
			CBoxArranjo->Items->AddObject(arranjo->Codigo, arranjo);
			// verifica se o tamanho do código cabe no combo box e adiciona
			// 110 pixels para a diferença do combobox para o form
			newWidth  = canvas->TextWidth(arranjo->Codigo) + 110;
			if (this->Width < newWidth)
				{
				this->Width = newWidth;
				FormAjustaPosicao();
				}
			}
		 }
	  else
		 {//insere Arranjo no CBoxArranjo
		 CBoxArranjo->Items->AddObject(arranjo->Codigo, arranjo);
		 // verifica se o tamanho do código cabe no combo box e adiciona
		 // 110 pixels para a diferença do combobox para o form
		 newWidth  = canvas->TextWidth(arranjo->Codigo) + 110;
		 if (this->Width < newWidth)
			{
			this->Width = newWidth;
			FormAjustaPosicao();
			}
		 }
	  }
   //define ordem alfabética
   CBoxArranjo->Sorted = true;
   //pré-seleciona Arranjo do Trecho
   CBoxArranjo->ItemIndex = CBoxArranjo->Items->IndexOfObject(ptrecho->arranjo);
   if ((CBoxArranjo->ItemIndex < 0)&&(CBoxArranjo->Items->Count > 0))
      {
      CBoxArranjo->ItemIndex = 0;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::CBoxFasesInicia(void)
   {
   //variáveis locais
   TFase   *fase;
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));
   TList   *lisFASE = fases->LisFasesTrechoTrifasico();

   //inicia CBoxFases c/ fases p/ arranjo próprio (Trecho trifásico)
   CBoxFases->Clear();
   for (int n = 0; n < lisFASE->Count; n++)
      {
      fase = (TFase*)lisFASE->Items[n];
      CBoxFases->Items->AddObject(fase->codigo, fase);
      }
   //pré seleciona fases do Arranjo próprio
   if ((ptrecho->arranjo != NULL)&&(! ptrecho->ArranjoTipico))
      {
      fase = fases->TagToFase[ptrecho->arranjo->Fases];
      if (fase != NULL) CBoxFases->ItemIndex = CBoxFases->Items->IndexOfObject(fase);
      }
   //proteção: se as fases não estão definidas, assume o primeiro item
   if ((CBoxFases->Items->Count > 0)&&(CBoxFases->ItemIndex < 0)) CBoxFases->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::CheckBoxDivisaoClick(TObject *Sender)
   {
   //variáveis locais
   VTBarra *barra;

   //verifica se CBoxBarRef ainda não foi iniciado
   if (CBoxBarRef->Items->Count == 0)
      {//inicia CBoxBarRef com as Barras di Trecho
      CBoxBarRef->Items->AddObject(ptrecho->pbarra1->Codigo, ptrecho->pbarra1);
      CBoxBarRef->Items->AddObject(ptrecho->pbarra2->Codigo, ptrecho->pbarra2);
      CBoxBarRef->ItemIndex = 0;
      }
   if (CheckBoxDivisao->Checked)
      {//exibe GBoxDivisao
      GBoxDivisao->Visible = true;
      //FKM - 2015.10.07 Ajusta o tamanho do form \\
      this->AutoSize = true;
      this->AutoSize = false;
      //FKM - 2015.10.07 Ajusta o tamanho do form //
      //exibe moldura na Barra selecionada
      barra = (VTBarra*)CBoxBarRef->Items->Objects[CBoxBarRef->ItemIndex];
      Moldura(barra);
      }
   else
      {//esconde GBoxDivisao
      GBoxDivisao->Visible = false;
      //FKM - 2015.10.07 Ajusta o tamanho do form \\
      this->AutoSize = true;
      this->AutoSize = false;
      //FKM - 2015.10.07 Ajusta o tamanho do form //
      //apaga moldura
      Moldura();
      }
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18417316
void __fastcall TFormEdtTre::DefValoresDefaultTrecho(void)
{
    //variáveis locais
    VTTrecho *ult_tre;
    VTRedes  *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

    //verifica se o Trecho já está definido
    if (ptrecho->arranjo != NULL)
    {
        //Trecho já está definido: verifica se o Arranjo é próprio
        if (!ptrecho->ArranjoTipico)
            //Arranjo próprio: copia seus dados
            arranjo->CopiaAtributosDe(*(ptrecho->arranjo));
        return;
    }
    //define comprimento do Trecho pelas coordenadas utm das Barras
    if (TrechoComCoordenadaUtm())
        //calcula comprimento
        ptrecho->Comprimento_km = ptrecho->ComprimentoUtm_cm() * 1e-5;
    else
        //define trecho c/ comprimento 0.1 metro
        ptrecho->Comprimento_km = 1e-4;
    //verifica se há outros Trechos definidos
    //primeiro nas barras originais do trecho e
    //se não achar no último trecho da rede
    lisEQP->Clear();
    if(ptrecho->pbarra1)
        ptrecho->pbarra1->LisLigacao(lisEQP, eqptoTRECHO);
    if((lisEQP->Count <= 0) && ptrecho->pbarra2)
        ptrecho->pbarra2->LisLigacao(lisEQP, eqptoTRECHO);
    if((lisEQP->Count <= 0) && ptrecho->pbarra3)
        ptrecho->pbarra3->LisLigacao(lisEQP, eqptoTRECHO);
    if(lisEQP->Count <= 0)
        redes->LisLigacao(lisEQP, eqptoTRECHO);
    if (lisEQP->Count > 0)
    {
        //determina último Trecho
        ult_tre = (VTTrecho*)lisEQP->Last();
        //verifica se Trecho tem comprimento menor que 1 m
        if (ptrecho->Comprimento_m < 1.)
            //copia comprimento do último Trecho
            ptrecho->Comprimento_km = ult_tre->Comprimento_km;
        //verifica se o último Trecho possui Arranjo típico
        if (ult_tre->ArranjoTipico)
            //assume mesmo Arranjo típico
            ptrecho->arranjo = ult_tre->arranjo;
        else
        {
            //copia Arranjo próprio do último Arranjo e redefine Id e Owner
            arranjo->CopiaAtributosDe(*(ult_tre->arranjo));
            arranjo->Id      = -1;
            arranjo->Owner   = ptrecho;
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::ExibeDadosArranjoProprio(void)
   {
   //variáveis locais
   strIMP z0_pu, z1_pu;
   double yc0_pu, yc1_pu;
   double Sbase = 100.;

   //exibe corrente admissível
   EditIadm->Text = DoubleToStr("%2.1f", arranjo->Iadm_amp);
   //reinicia SGridZseq
   for (int nr = 0; nr < SGridZseq->RowCount; nr++) SGridZseq->Rows[nr]->Clear();
   //preenche SGrid de acordo c/ o tipo de configuração
   if (arranjo->TipoCfg == arranjoPU_SB100)
      {//arranjoCEPEL: z0, z1, yc0 e yc1 em pu Sb100
      arranjo->Z0_pu(Sbase, ptrecho->pbarra1->vnom, ptrecho->Comprimento_km , z0_pu, yc0_pu);
      arranjo->Z1_pu(Sbase, ptrecho->pbarra1->vnom, ptrecho->Comprimento_km, z1_pu, yc1_pu);
      //arranjoZ0_Z1:: z0,z1 em ohm/km e c0,c1 em nF/km
      SGridZseq->Cells[0][0] = "";
      SGridZseq->Cells[1][0] = "r(pu)";
      SGridZseq->Cells[2][0] = "x(pu)";
      SGridZseq->Cells[3][0] = "b(pu)";
      //preenche linha de Seq.0
      SGridZseq->Cells[0][1] = "Seq.0";
      SGridZseq->Cells[1][1] = DoubleToStr("%8.7f", z0_pu.r);
      SGridZseq->Cells[2][1] = DoubleToStr("%8.7f", z0_pu.x);
      SGridZseq->Cells[3][1] = DoubleToStr("%8.7f", yc0_pu);
      //preenche linha de Seq.1
      SGridZseq->Cells[0][2] = "Seq.1";
      SGridZseq->Cells[1][2] = DoubleToStr("%8.7f", z1_pu.r);
      SGridZseq->Cells[2][2] = DoubleToStr("%8.7f", z1_pu.x);
      SGridZseq->Cells[3][2] = DoubleToStr("%8.7f", yc1_pu);
      }
   else
      {//arranjoZ0Z1:: z0,z1 em ohm/km e c0,c1 em nF/km
      SGridZseq->Cells[0][0] = "";
      SGridZseq->Cells[1][0] = "r(ohm/km)";
      SGridZseq->Cells[2][0] = "x(ohm/km)";
      SGridZseq->Cells[3][0] = "c(nF/km)";
      //preenche linha de Seq.0
      SGridZseq->Cells[0][1] = "Seq.0";
      SGridZseq->Cells[1][1] = DoubleToStr("%8.7f", arranjo->z0.r);
      SGridZseq->Cells[2][1] = DoubleToStr("%8.7f", arranjo->z0.x);
      SGridZseq->Cells[3][1] = DoubleToStr("%8.7f", arranjo->c0);
      //preenche linha de Seq.1
      SGridZseq->Cells[0][2] = "Seq.1";
      SGridZseq->Cells[1][2] = DoubleToStr("%8.7f", arranjo->z1.r);
      SGridZseq->Cells[2][2] = DoubleToStr("%8.7f", arranjo->z1.x);
      SGridZseq->Cells[3][2] = DoubleToStr("%8.7f", arranjo->c1);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::ExibeDadosTrecho(void)
   {
   //exibe código
   EditCodigo->Text = ptrecho->Codigo;
   //exibe coprimento
   if (ptrecho->Comprimento_m < 1.) {edtComp->Text = "";}
   else                             {edtComp->Text   = DoubleToStr("%3.2f", ptrecho->Comprimento_m);}
   //define se Arranjo típico ou próprio
   if   (ptrecho->arranjo != NULL)  RGroupArranjo->ItemIndex = (ptrecho->ArranjoTipico) ? 0 : 1;
   else                             RGroupArranjo->ItemIndex = (arranjo->Owner == NULL) ? 0 : 1;
   //define tipo de configuração (bloqueando tratamento de alteração)
   RGroupTipoCfg->OnClick = NULL;
   RGroupTipoCfg->ItemIndex = (arranjo->TipoCfg == arranjoZ0Z1) ? 0 : 1;
   RGroupTipoCfg->OnClick = RGroupTipoCfgClick;
   //exibe dados do Arranjo próprio (antes de atualizar RGroupTipoCfg)
   ExibeDadosArranjoProprio();
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TFormEdtTre::ExisteDivisao(double &dist_m)
   {
   if (! CheckBoxDivisao->Checked) return(NULL);
   //retorna Barra e distância
   dist_m = divisao.dist_m;
   return(divisao.bar_ref);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::FormClose(TObject *Sender, TCloseAction &Action)
   {//esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
	  {//lê parâmetros do Form
	  geral->Infoset->LoadFromFile(this);
	  }
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16351053
void __fastcall TFormEdtTre::FormAjustaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
	  {//lê parâmetros do Form
	  geral->Infoset->SaveToFile(this);
	  }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::FormShow(TObject *Sender)
   {
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::IniciaCBoxRede(void)
   {
   //variáveis locais
   int      index;
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //reinicia CBox c/ as Redes
   cboxRede->Clear();
   if (lisREDE->Count == 0) return;
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      cboxRede->Items->AddObject(rede->Codigo, rede);
      }
   //define ordem alfabética
   cboxRede->Sorted = true;
   //pré-seleciona Rede que conecta as Barras da Chave
   index = 0;
   if ((rede = SelecionaRede(lisREDE)) != NULL) index = cboxRede->Items->IndexOfObject(rede);
   cboxRede->ItemIndex = index;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTre::LeComprimento(void)
   {
   //variáveis locais
   double comp_m;

   //se a divisão de Trecho estiver habilitada os dados já estão corretos
   if (CheckBoxDivisao->Checked) return(true);
   try{//lê comprimento
      comp_m = StrToDouble(edtComp->Text);
      //proteção: verifica comprimento nulo ou negativo
      if (comp_m <= 0.)
         {
         Aviso("Comprimento do trecho c/ valor inválido");
         return(false);
         }
      //atualiza comprimento
      ptrecho->Comprimento_km = comp_m * 0.001;
      } catch(Exception &e)
         {
         Aviso("Comprimento do trecho c/ caractere inválido");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTre::LeCorrenteAdmissivel(void)
   {
   //lê Iadm
   try{//lê valores limites
      arranjo->Iadm_amp = StrToDouble(EditIadm->Text);
      }catch(Exception &e)
         {
         Aviso("Corrente admissível do trecho c/ valor inválido");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTre::LeDadosArranjoProprio(void)
   {
   //variáveis locais
   strIMP z0, z1;
   double c0, c1;
   double Sbase = 100.;
   TFase  *fase;

   try{//verifica se o Arranjo está configurado c/ Arranjo típico
      if (RGroupArranjo->ItemIndex == 0) return(true);
      //verifica se a fase está definida
      if (CBoxFases->ItemIndex < 0)
         {
         Aviso("Selecione as fases do arranjo");
         return(false);
         }
      //cancela código
      arranjo->Codigo = "";
      //lê corrente admissível
      if (! LeCorrenteAdmissivel()) return(false);
      //lê tipo de configuração
      arranjo->TipoCfg = (RGroupTipoCfg->ItemIndex == 0) ? arranjoZ0Z1 : arranjoPU_SB100;
      //define fases
      fase = (TFase*)(CBoxFases->Items->Objects[CBoxFases->ItemIndex]);
      arranjo->Fases = fase->tag;
      //lê valores de Seq.0
      z0.r = StrToDouble(SGridZseq->Cells[1][1]);
      z0.x = StrToDouble(SGridZseq->Cells[2][1]);
      c0   = StrToDouble(SGridZseq->Cells[3][1]);
      //lê valores de Seq.1
      z1.r = StrToDouble(SGridZseq->Cells[1][2]);
      z1.x = StrToDouble(SGridZseq->Cells[2][2]);
      c1   = StrToDouble(SGridZseq->Cells[3][2]);
      //atualiza Arranjo de acordo c/ seu tipo de configuração
      if (arranjo->TipoCfg == arranjoPU_SB100)
         {//arranjoPU_SB100: z0, z1, c0 e c1 em pu Sb100
         arranjo->DefZ0Z1_pu(Sbase, ptrecho->pbarra1->vnom, ptrecho->Comprimento_km, z0, z1);
         arranjo->DefC0C1_pu(Sbase, ptrecho->pbarra1->vnom, ptrecho->Comprimento_km, c0, c1);
         }
      else
         {//arranjoZ0Z1:: z0,z1 em ohm/km e c0,c1 em nF/km
         arranjo->DefZ0Z1_ohm_km(z0, z1);
         arranjo->DefC0C1_nF_km(c0, c1);
         }
      //valida fases do Arranjo c/ as fases das Barras
      }catch(Exception &e)
         {
         Aviso("Dados do arranjo com valores inválidos.");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTre::LeDadosArranjoTipico(void)
   {
   //proteção: verifica se Arranjo típico
   if (RGroupArranjo->ItemIndex == 0)
      {//Arranjo típico
      if (CBoxArranjo->ItemIndex < 0)
         {//não há arranjo típico selecionado
         Aviso("Selecione o arranjo típico do trecho.");
         return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTre::LeDadosDivisao(void)
   {
   //variáveis locais
   double comp_m;

   //verifica se a divisão está selecionada
   if (! CheckBoxDivisao->Checked) return(true);
   //verifica se a Barra de referência está selecionada
   if (CBoxBarRef->ItemIndex < 0)
      {//
      Aviso("Selecione barra de referência para divisão do trecho");
      return(false);
      }
   //salva Barras de referência
   divisao.bar_ref = (VTBarra*)(CBoxBarRef->Items->Objects[CBoxBarRef->ItemIndex]);
   try{//lê distância da Barra de referência
      comp_m = StrToDouble(EditDistBarRef->Text);
      //proteção: verifica comprimento nulo ou negativo
      if (comp_m <= 0.)
         {
         Aviso("Distância p/ divisão do trecho c/ valor inválido");
         return(false);
         }
      //atualiza comprimento
      divisao.dist_m = comp_m;
      } catch(Exception &e)
         {
         Aviso("Distância p/ divisão do trecho c/ caractere inválido");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTre::LeDadosTrecho(void)
   {
   //proteção: verifica se a Rede está definida
   if (cboxRede->ItemIndex < 0)
      {
      Aviso("Defina a rede do trecho.");
      return(false);
      }
   //determina Rede do Trecho
   ptrecho->rede = (VTRede*)cboxRede->Items->Objects[cboxRede->ItemIndex];
   //lê código do trecho
   ptrecho->Codigo = EditCodigo->Text;
   //lê comprimento do Trecho
   if (! LeComprimento()) return(false);
   //lê dados para divisão do Trecho
   if (! LeDadosDivisao()) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::Moldura(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   grafico->Moldura(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::RGroupArranjoClick(TObject *Sender)
   {
   if (RGroupArranjo->ItemIndex == 0)
      {
      GBoxArranjoProprio->Visible = false;
      GBoxArranjoTipico->Visible  = true;
      //FKM - 2015.10.07 Ajusta o tamanho do form \\
      this->AutoSize = true;
      this->AutoSize = false;
      //FKM - 2015.10.07 Ajusta o tamanho do form //
      }
   else
      {
      GBoxArranjoTipico->Visible  = false;
      GBoxArranjoProprio->Visible = true;
      ExibeDadosArranjoProprio();
      //FKM - 2015.10.07 Ajusta o tamanho do form \\
      this->AutoSize = true;
      this->AutoSize = false;
      //FKM - 2015.10.07 Ajusta o tamanho do form //
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTre::RGroupTipoCfgClick(TObject *Sender)
   {
   //variáveis locais
   strIMP z0, z1;
   double c0, c1;
   double Sbase = 100.;

   //lê comprimento do Trecho
   LeComprimento();
   try{//lê valores de Seq.0
      z0.r = StrToDouble(SGridZseq->Cells[1][1]);
      z0.x = StrToDouble(SGridZseq->Cells[2][1]);
      c0   = StrToDouble(SGridZseq->Cells[3][1]);
      //lê valores de Seq.1
      z1.r = StrToDouble(SGridZseq->Cells[1][2]);
      z1.x = StrToDouble(SGridZseq->Cells[2][2]);
      c1   = StrToDouble(SGridZseq->Cells[3][2]);
      //atualiza Arranjo de acordo c/ seu tipo de configuração ANTES da alteração do RGroupTipoCfg
      if (RGroupTipoCfg->ItemIndex == 0)
         {//arranjoZ0Z1: anterior era arranjoCEPEL: z0, z1, c0 e c1 em pu Sb100
         arranjo->DefZ0Z1_pu(Sbase, ptrecho->pbarra1->vnom, ptrecho->Comprimento_km, z0, z1);
         arranjo->DefC0C1_pu(Sbase, ptrecho->pbarra1->vnom, ptrecho->Comprimento_km, c0, c1);
         }
      else
         {//arranjoCEPEL: anterior era arranjoZ0Z1:: z0,z1 em ohm/km e c0,c1 em nF/km
         arranjo->DefZ0Z1_ohm_km(z0, z1);
         arranjo->DefC0C1_nF_km(c0, c1);
         }
      //atualiza tipo de configuração
      arranjo->TipoCfg = (RGroupTipoCfg->ItemIndex == 0) ? arranjoZ0Z1 : arranjoPU_SB100;
      //reapresenta valores do Arranjo
      ExibeDadosArranjoProprio();
      } catch(Exception &e)
         {//nada a fazer
         Aviso("Impedâncias de seqüência c/ valor inválido");
         }
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtTre::SelecionaRede(TList *lisREDE)
   {
   //variáveis locais
   VTRede  *rede;

   //verifica se o Trafo já possui uma Rede definida
   if (ptrecho->rede) return(ptrecho->rede);
   //procura uma Rede que conecta as Barras da Chave
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if ((rede->ExisteBarra(ptrecho->pbarra1))||
          (rede->ExisteBarra(ptrecho->pbarra2))) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTre::TrechoComCoordenadaUtm(void)
   {
   //variáveis locais
   VTBarra *barra;

   //verifica se Barras do Trecho tem coord.utm definida
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ptrecho->Barra(ind_bar)) == NULL) continue;
      if ((barra->utm.x == 0)||(barra->utm.y == 0)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTre::ValidaDadosArranjoProprio(void)
   {
   //verifica se o Trecho está configurado c/ Arranjo típico
   if (RGroupArranjo->ItemIndex == 0) return(true);
   //valida corrente admissível
   if (arranjo->Iadm_amp <= 0.)
      {
      Erro("Corrente admissível nula ou negativa");
      return(false);
      }
   //valida Z0
   if ((arranjo->z0.r <= 0.)&&(arranjo->z0.x <= 0.))
      {
      Erro("Impedância de seq.0 c/ valor nulo ou negativo");
      return(false);
      }
   //valida Z1
   if ((arranjo->z1.r <= 0.)&&(arranjo->z1.x <= 0.))
      {
      Erro("Impedância de seq.1 c/ valor nulo ou negativo");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTre::ValidaDadosTrecho(void)
   {
   //variáveis locais
   double comp_km;

   //valida código
   //if (ptrecho->Codigo.IsEmpty())
   //   {
   //   Erro("Defina o código do trecho");
   //   return(false);
   //   }
   //valida comprimento
   if (ptrecho->Comprimento_km < 0.001)   //1 metro
      {
      Erro("Comprimento do trecho inválido");
      return(false);
      }
   //valida distância para divisão do trecho
   if (CheckBoxDivisao->Checked)
      {
      if (divisao.dist_m >= ptrecho->Comprimento_m)
         {
         Erro("A distância p/ divisão do trecho é maior que o comprimento do trecho");
         return(false);
         }
      }
   //verifica comprimento editado c/ comprimento calculado pelas coordenadas
   if (TrechoComCoordenadaUtm())
      {//calcula comprimento
      comp_km = ptrecho->ComprimentoUtm_cm() * 1e-5;
      if (! IsDoubleZero(ptrecho->Comprimento_km - comp_km, comp_km * 0.05))
         {
         switch(Confirma("O comprimento do trecho é diferente da distância entre barras: " + DoubleToStr("(%5.4f km)", comp_km),
                         "O valor editado é o comprimento correto ?", MB_YESNOCANCEL))
            {
            case IDYES:   //mantém o valor editado
               break;
            case IDNO:   //troca valor editado pela distância
               ptrecho->Comprimento_km = comp_km;
               break;
            case IDCANCEL:   //continua edição
               return(false);
            }
         }
      }
   return(true);
   }


//---------------------------------------------------------------------------
//eof



