//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgFlow.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Mercado\VTMercado.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTRedes.h>
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCfgFlow::TFormCfgFlow(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   curvas    = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   flow      = (VTFlow*)apl->GetObject(__classid(VTFlow));
   mercado   = (VTMercado*)apl->GetObject(__classid(VTMercado));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //posiciona o Form
   FormIniciaPosicao();
   //obtém configuacao atual
   cfg.TipoFluxo          = flow->TipoFluxo;
   cfg.ModeloCarga        = flow->ModeloCarga;
   cfg.TipoRelatorio      = flow->TipoRelatorio;
   cfg.RelatorioBarDesc   = flow->RelatorioBarDesc;
   cfg.AvisoEnabled       = flow->AvisoEnabled;
   cfg.CronometroEnabled  = flow->CronometroEnabled;
   cfg.MultiThreadEnabled = flow->MultiThreadEnabled;
   cfg.PorRedeEnabled     = flow->PorRedeEnabled;
   cfg.PerdaFerroEnabled  = flow->PerdaFerroEnabled;
   cfg.ProgressoEnabled   = flow->ProgressoEnabled;
   cfg.TramoEnabled       = flow->TramoEnabled;
   cfg.AnoMercado         = mercado->AnoAtual;
   cfg.DiaCurvaTipica     = curvas->DiaMedicao;
   //atualiza tela
   ExibeConfiguracao();
   }


//---------------------------------------------------------------------------
__fastcall TFormCfgFlow::~TFormCfgFlow(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ActionConfirmaExecute(TObject *Sender)
   {
   //lê condiguração
   if (! SalvaConfiguracao()) return;
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::CBoxAnoChange(TObject *Sender)
   {
   //habilita/desabilita utilização do Mercado
   CheckBoxMercado->Checked = (CBoxAno->ItemIndex >= 0);
   //simula evendo em CheckBoxMercado
   CheckBoxMercadoClick(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::CheckBoxMercadoClick(TObject *Sender)
   {
   //exibe esconde PanelAno
   PanelAno->Visible = CheckBoxMercado->Checked;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeConfiguracao(void)
   {
   //modelo de cálculo e modelagem da rede
   ExibeOpcaoTipoFluxo();
   //processamento
   ExibeOpcaoProcessamento();
   //progresso
   ExibeOpcaoProgresso();
   //cronômetro
   ExibeOpcaoCronometro();
   //mercado
   ExibeOpcaoMercado();
   //curva típica
   ExibeOpcaoCurvaTipica();
   //modelo de carga
   ExibeOpcaoModeloCarga();
   //perda de ferro nos trafos
   ExibeOpcaoPerdaFerroTrafo();
   //relatório
   ExibeOpcaoRelatorio();
   }
//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeOpcaoCronometro(void)
   {
   CheckBoxCronometro->Checked = cfg.CronometroEnabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeOpcaoCurvaTipica(void)
   {
   switch(cfg.DiaCurvaTipica)
      {
      case DOMINGO:         RadioGroupCurvaTipica->ItemIndex = 0; break;
      case SABADO:          RadioGroupCurvaTipica->ItemIndex = 1; break;
      case DIA_UTIL:        RadioGroupCurvaTipica->ItemIndex = 2; break;
      case SEMANA_COMPLETA: RadioGroupCurvaTipica->ItemIndex = 3; break;
      default:              RadioGroupCurvaTipica->ItemIndex = 3; break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeOpcaoMercado(void)
   {
   //variáveis locais
   int ano_inicial = mercado->AnoInicial;
   int ano_final   = mercado->AnoFinal;
   int ano_atual   = mercado->AnoAtual;

   //desabilita evento OnChange
   CBoxAno->OnChange = NULL;
   //reinicia CBoxAno
   CBoxAno->Clear();
   //insere anos entre ano_inicial e ano_final
   for (int ano = ano_inicial+1; ano <= ano_final; ano++)
      {
      CBoxAno->Items->Add(IntToStr(ano));
      }
   //pré-seleciona ano atual
   if ((ano_atual > ano_inicial) && (ano_atual <= ano_final))
      CBoxAno->ItemIndex = (ano_atual - ano_inicial - 1);
   else
      CBoxAno->ItemIndex = -1;
   //reabilita evento OnChange
   CBoxAno->OnChange = NULL;
   //simula alteração do ano selecionado
   CBoxAnoChange(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeOpcaoModeloCarga(void)
   {
   switch(cfg.ModeloCarga)
      {
      case cargaAUTO:  RadioGroupModeloCarga->ItemIndex = 0; break;
      case cargaICTE:  RadioGroupModeloCarga->ItemIndex = 1; break;
      case cargaSCTE:  RadioGroupModeloCarga->ItemIndex = 2; break;
      case cargaZCTE:  RadioGroupModeloCarga->ItemIndex = 3; break;
      case cargaANEEL: RadioGroupModeloCarga->ItemIndex = 4; break;
      default:         RadioGroupModeloCarga->ItemIndex = 0; break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeOpcaoPerdaFerroTrafo(void)
   {
   CheckBoxPerdaFerro->Checked = cfg.PerdaFerroEnabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeOpcaoProcessamento(void)
   {
   CheckBoxCalculoPorRede->Checked = cfg.PorRedeEnabled;
   CheckBoxThread->Checked         = cfg.MultiThreadEnabled;
   CheckBoxTramo->Checked          = cfg.TramoEnabled;
   CheckBoxAviso->Checked          = cfg.AvisoEnabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeOpcaoProgresso(void)
   {
   CheckBoxProgresso->Checked = cfg.ProgressoEnabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeOpcaoRelatorio(void)
   {
   //tipo de relatório de fluxo
   switch(cfg.TipoRelatorio)
      {
      case relNENHUM:   RadioGroupRelatorio->ItemIndex = 0; break;
      case relRESUMIDO: RadioGroupRelatorio->ItemIndex = 1; break;
      case relCOMPLETO: RadioGroupRelatorio->ItemIndex = 2; break;
      default:          RadioGroupRelatorio->ItemIndex = 0; break;
      }
   //relatório de barra desconexa
   CheckBoxRelatorioBarDesc->Checked = cfg.RelatorioBarDesc;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::ExibeOpcaoTipoFluxo(void)
   {
   switch(cfg.TipoFluxo)
      {
      case fluxoGAUSS1:
         RadioGroupModeloRede->ItemIndex    = 1;
         RadioGroupModeloCalculo->ItemIndex = 2;
         break;
      case fluxoGAUSS3:
         RadioGroupModeloRede->ItemIndex    = 0;
         RadioGroupModeloCalculo->ItemIndex = 2;
         break;
      case fluxoNR1:
         RadioGroupModeloRede->ItemIndex    = 1;
         RadioGroupModeloCalculo->ItemIndex = 1;
         break;
      case fluxoNR3:
         RadioGroupModeloRede->ItemIndex    = 0;
         RadioGroupModeloCalculo->ItemIndex = 1;
         break;
      case fluxoEQUILIBRADO:
         RadioGroupModeloRede->ItemIndex    = 1;
         RadioGroupModeloCalculo->ItemIndex = 0;
         break;
      case fluxoDESEQUILIBRADO:
         RadioGroupModeloRede->ItemIndex    = 0;
         RadioGroupModeloCalculo->ItemIndex = 0;
         break;
      case fluxoAUTO:
         RadioGroupModeloRede->ItemIndex    = 0;
         RadioGroupModeloCalculo->ItemIndex = 0;
         break;
      default:
         RadioGroupModeloRede->ItemIndex    = 0;
         RadioGroupModeloCalculo->ItemIndex = 0;
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgFlow::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaConfiguracao(void)
   {
   //salva todas opções
   if (! SalvaOpcaoCronometro())      return(false);
   if (! SalvaOpcaoCurvaTipica())     return(false);
   if (! SalvaOpcaoMercado())         return(false);
   if (! SalvaOpcaoModeloCarga())     return(false);
   if (! SalvaOpcaoPerdaFerroTrafo()) return(false);
   if (! SalvaOpcaoProcessamento())   return(false);
   if (! SalvaOpcaoProgresso())       return(false);
   if (! SalvaOpcaoRelatorio())       return(false);
   if (! SalvaOpcaoTipoFluxo())       return(false);
   //verifica se foi alterada o dia das curvas típicas
   if (curvas->DiaMedicao != cfg.DiaCurvaTipica)
      {//atualiza Curvas
      curvas->DiaMedicao = cfg.DiaCurvaTipica;
      //reconfigura todas as cargas, associando-as com as curvas típicas selecionadas
      redes->RedefineCurvaTipicaCarga(curvas->DiaMedicao);
      }
   //atualiza Mercado
   mercado->AnoAtual = cfg.AnoMercado;
   //atualiza Flow
   flow->TipoFluxo          = cfg.TipoFluxo;
   flow->ModeloCarga        = cfg.ModeloCarga;
   flow->TipoRelatorio      = cfg.TipoRelatorio;
   flow->RelatorioBarDesc   = cfg.RelatorioBarDesc;
   flow->AvisoEnabled       = cfg.AvisoEnabled;
   flow->CronometroEnabled  = cfg.CronometroEnabled;
   flow->MultiThreadEnabled = cfg.MultiThreadEnabled;
   flow->PorRedeEnabled     = cfg.PorRedeEnabled;
   flow->PerdaFerroEnabled  = cfg.PerdaFerroEnabled;
   flow->ProgressoEnabled   = cfg.ProgressoEnabled;
   flow->TramoEnabled       = cfg.TramoEnabled;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaOpcaoCronometro(void)
   {
   cfg.CronometroEnabled = CheckBoxCronometro->Checked;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaOpcaoCurvaTipica(void)
   {
   switch(RadioGroupCurvaTipica->ItemIndex)
      {
      case 0: cfg.DiaCurvaTipica = DOMINGO;         break;
      case 1: cfg.DiaCurvaTipica = SABADO;          break;
      case 2: cfg.DiaCurvaTipica = DIA_UTIL;        break;
      case 3: cfg.DiaCurvaTipica = SEMANA_COMPLETA; break;
      default:
         Aviso("Selecione o conjunto de curvas típicas de demanda");
         return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaOpcaoMercado(void)
   {
   //verifica se foi selecionado aplicação de taxas de mercado
   if (! CheckBoxMercado->Checked)
      {//redefine ano atualigual a ano inicial
      cfg.AnoMercado = mercado->AnoInicial;
      return(true);
      }
   //verifica se foi selecionado um ano
   if (CBoxAno->ItemIndex < 0)
      {
      Aviso("Selecione o ano do mercado");
      return(false);
      }
   //salva ano atual do Mercado
   if (! StrToInteger(CBoxAno->Text, cfg.AnoMercado))
      {
      Aviso("Ano do mercado inválido");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaOpcaoModeloCarga(void)
   {
   switch(RadioGroupModeloCarga->ItemIndex)
      {
      case 0: cfg.ModeloCarga = cargaAUTO;  break;
      case 1: cfg.ModeloCarga = cargaICTE;  break;
      case 2: cfg.ModeloCarga = cargaSCTE;  break;
      case 3: cfg.ModeloCarga = cargaZCTE;  break;
      case 4: cfg.ModeloCarga = cargaANEEL; break;
      default:
         Aviso("Selecione modelo de carga");
         return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaOpcaoPerdaFerroTrafo(void)
   {
   cfg.PerdaFerroEnabled = CheckBoxPerdaFerro->Checked;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaOpcaoProcessamento(void)
   {
   cfg.PorRedeEnabled     = CheckBoxCalculoPorRede->Checked;
   cfg.MultiThreadEnabled = CheckBoxThread->Checked;
   cfg.TramoEnabled       = CheckBoxTramo->Checked;
   cfg.AvisoEnabled       = CheckBoxAviso->Checked;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaOpcaoProgresso(void)
   {
   cfg.ProgressoEnabled = CheckBoxProgresso->Checked;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaOpcaoRelatorio(void)
   {
   //relatório de fluxo
   switch(RadioGroupRelatorio->ItemIndex)
      {
      case 0: cfg.TipoRelatorio = relNENHUM; break;
      case 1: cfg.TipoRelatorio = relRESUMIDO; break;
      case 2: cfg.TipoRelatorio = relCOMPLETO; break;
      default:
         {
         Aviso("Selecione o tipo de relatório");
         return(false);
         }
      }
   //relatório de barra desconexa
   cfg.RelatorioBarDesc = CheckBoxRelatorioBarDesc->Checked;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgFlow::SalvaOpcaoTipoFluxo(void)
   {
   //proteção
   if (RadioGroupModeloCalculo->ItemIndex < 0)
      {
      Aviso("Selecione o modelo de cálculo");
      return(false);
      }
   if (RadioGroupModeloRede->ItemIndex < 0)
      {
      Aviso("Selecione a modelagem da rede");
      return(false);
      }
   //verifica modelo de rede
   switch(RadioGroupModeloRede->ItemIndex)
      {
      case 0: //rede real (desequilibrada)
         switch(RadioGroupModeloCalculo->ItemIndex)
            {
            case 0: cfg.TipoFluxo = fluxoAUTO;   break;
            case 1: cfg.TipoFluxo = fluxoNR3;    break;
            case 2: cfg.TipoFluxo = fluxoGAUSS3; break;
            }
         break;
      case 1:  //rede equilibrada
         switch(RadioGroupModeloCalculo->ItemIndex)
            {
            case 0: cfg.TipoFluxo = fluxoEQUILIBRADO; break;
            case 1: cfg.TipoFluxo = fluxoNR1;         break;
            case 2: cfg.TipoFluxo = fluxoGAUSS1;      break;
            }
         break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof


