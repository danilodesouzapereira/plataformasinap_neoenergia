//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgPlaneja.h"
#include "..\DLL_Stub\Basico.h"
#include "..\BDado\VTBDado.h"
#include "..\Geral\VTGeral.h"
#include "..\Sisplan\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TformCfgPlaneja::TformCfgPlaneja(TComponent* Owner, AnsiString dirBase, AnsiString dirRede)
   : TForm(Owner)
   {

   //cria objeto BDado
   bdado = DLL_NewObjBDado();
   bdado->DefDiretorio(dirBase, dirRede);
   //cria objeto Geral
   geral = bdado->LeGeral();
   //recebe valor do geral
   PLAN=geral->PLAN;

   }

//---------------------------------------------------------------------------
__fastcall TformCfgPlaneja::~TformCfgPlaneja(void)
   {

   //destrói objetos criados
   if (geral) {delete geral; geral = NULL;}
   if (bdado) {delete bdado; bdado = NULL;}

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPlaneja::ActionCancelaExecute(TObject *Sender)
   {

   ModalResult = mrCancel;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPlaneja::ActionConfirmaExecute(TObject *Sender)
   {

   //lê e valida dados editados
   if (! LeDados()) return;
   //atualiza base de dados
   bdado->GravaGeral(geral);
   //fecha o form
   ModalResult = mrOk;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPlaneja::ActionDefaultExecute(TObject *Sender)
   {

   //variáveis locais
   unsigned short ano, mes, dia;

   //ano referência: ano atual
   Now().DecodeDate(&ano, &mes, &dia);
   PLAN.ANO_REF = ano;
   //número de anos de planejamento
   PLAN.NUM_ANO = MAX_ANO;
   PLAN.FAT_SCARGA    = geral->PLAN.FAT_SCARGA;
   PLAN.FAT_PERDA     = geral->PLAN.FAT_PERDA;
   PLAN.CUSTO_DEMAN   = geral->PLAN.CUSTO_DEMAN;
   PLAN.CUSTO_ENERGIA = geral->PLAN.CUSTO_ENERGIA;
   PLAN.CUSTO_END     = geral->PLAN.CUSTO_END;
   PLAN.RED_POTMIN    = geral->PLAN.RED_POTMIN;
   PLAN.OTI_CVAR      = geral->PLAN.OTI_CVAR;
   PLAN.RAD_CVAR      = geral->PLAN.RAD_CVAR;
   PLAN.RAD_CFIXO     = geral->PLAN.RAD_CFIXO;
   PLAN.RAD_NCHV      = geral->PLAN.RAD_NCHV;
   PLAN.TAXA_ANO      = geral->PLAN.TAXA_ANO;
   PLAN.VIDA_UTIL     = geral->PLAN.VIDA_UTIL;
   //atualiza tela
   ExibeDados();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPlaneja::edtCDemanKeyPress(TObject *Sender,
      char &Key)
   {

   switch (Key)
        {
        case 'E':
        case 'e':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case ',':
        case VK_DECIMAL:
        case VK_RETURN:
        case VK_BACK:
        case VK_TAB:
                break;

        default:
                Key=0;
        }
   }
//---------------------------------------------------------------------------
void __fastcall TformCfgPlaneja::ExibeDados(void)
   {

   //variáveis locais
   unsigned short ano, mes, dia;

   //ano referência: do ano anterior até 20 anos seguintes
   Now().DecodeDate(&ano, &mes, &dia);
   ano--;
   cmbAnoRef->Clear();

   for (int n = 0; n < 20; n++, ano++)
      {
      cmbAnoRef->Items->Add(IntToStr(ano));
      if (ano == PLAN.ANO_REF) cmbAnoRef->ItemIndex = n;
      }

   if (cmbAnoRef->ItemIndex < 0) cmbAnoRef->ItemIndex = 0;

   //número de anos de planejamento
   cmbNumAno->Clear();

   for (int n = 1; n <= MAX_ANO; n++)
      {
      cmbNumAno->Items->Add(IntToStr(n));
      if (n == PLAN.NUM_ANO) cmbNumAno->ItemIndex = n-1;
      }

   edtFSCarga->Text  = DoubleToStr("%3.0f", 100*(PLAN.FAT_SCARGA - 1));
   edtFPerda->Text   = DoubleToStr("%3.0f", 100*PLAN.FAT_PERDA);
   edtCDeman->Text   = DoubleToStr("%8.2f", PLAN.CUSTO_DEMAN);
   edtCEnd->Text   = DoubleToStr("%8.2f", PLAN.CUSTO_END);
   edtCEnergia->Text = DoubleToStr("%8.2f", PLAN.CUSTO_ENERGIA);
   edtTaxaAno->Text  = DoubleToStr("%3.0f", 100*PLAN.TAXA_ANO);
   edtVidaUtil->Text = DoubleToStr("%3.0f", PLAN.VIDA_UTIL);

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPlaneja::FormClose(TObject *Sender,      TCloseAction &Action)
   {

   //destrói o form
   Action = caFree;

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgPlaneja::FormShow(TObject *Sender)
   {

   //apresenta dados na tela
   ExibeDados();

   }
//---------------------------------------------------------------------------
bool __fastcall TformCfgPlaneja::LeDados(void)
   {
   try
   {
   PLAN.ANO_REF = cmbAnoRef->Text.ToInt();
   PLAN.NUM_ANO = cmbNumAno->Text.ToInt();
   //valida os dados
   if ((PLAN.NUM_ANO < 1)||(PLAN.NUM_ANO > MAX_ANO))
      {
      Erro("Planejamento: Número de anos inválido");
      return(false);
      }
   PLAN.FAT_SCARGA    = 1 + StrToDouble(edtFSCarga->Text)/100;
   PLAN.FAT_PERDA     = StrToDouble(edtFPerda->Text)/100;
   PLAN.CUSTO_DEMAN   = StrToDouble(edtCDeman->Text);
   PLAN.CUSTO_ENERGIA = StrToDouble(edtCEnergia->Text);
   PLAN.CUSTO_END = StrToDouble(edtCEnd->Text);
   //PLAN.RED_POTMIN    = StrToDouble(edtPMin->Text);
   //PLAN.OTI_CVAR      = StrToDouble(edtOCVar->Text);
   //PLAN.RAD_CVAR      = StrToDouble(edtRCVar->Text);
   //PLAN.RAD_CFIXO     = StrToDouble(edtRCFixo->Text);
   //PLAN.RAD_NCHV      = edtRNChv->Text.ToInt();
   PLAN.TAXA_ANO      = StrToDouble(edtTaxaAno->Text)/100;
   PLAN.VIDA_UTIL     = edtVidaUtil->Text.ToInt();
   //valida os dados
   if ((PLAN.FAT_SCARGA < 1)||(PLAN.FAT_SCARGA > 1.6))
      {
      Erro("Planejamento: Fator de sobrecarga inválido (0 a 60 %)");
      return(false);
      }
   if ((PLAN.FAT_PERDA < 0)||(PLAN.FAT_PERDA > 1))
      {
      Erro("Planejamento: Fator de perda inválido (0 a 100 %)");
      return(false);
      }
   if ((PLAN.RAD_NCHV < 0)||(PLAN.RAD_NCHV > 10))
      {
      Erro("Planejamento: Fator de perda inválido (0 a 10 %)");
      return(false);
      }
   if ((PLAN.TAXA_ANO < 0)||(PLAN.TAXA_ANO > 100))
      {
      Erro("Planejamento: Taxa anual de juros inválida (0 a 100 %)");
      return(false);
      }
   if ((PLAN.VIDA_UTIL < 10)||(PLAN.VIDA_UTIL > 50))
      {
      Erro("Planejamento: Tempo de vida inválido (10 a 50 anos)");
      return(false);
      }
   }//try
   //tratamento de erro
   catch (EConvertError &exception)
      {
      Erro("Planejamento: valor não numérico");
      return(false);
      }
   //leu os dados corretamente: atualiza objeto geral
   geral->PLAN = PLAN;

   return(true);
   }
//---------------------------------------------------------------------------

