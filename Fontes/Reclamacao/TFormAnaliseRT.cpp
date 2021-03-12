//---------------------------------------------------------------------------
 #include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <math.h>
//---------------------------------------------------------------------------
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Grafico\VTCfgGraf.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Mercado\VTMercado.h>
#include <Fontes\Obra\VTObras.h>
#include <Fontes\Planejamento\VTAlternativa.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#include "TFormAnaliseRT.h"
#include "TFormAnaliseGeral.h"
#include "..\Reclamacao\TFormCfgGraf.h"
#include "..\Reclamacao\TFormCfgTensao.h"
#include "..\Reclamacao\TFormRelTensoes.h"
#include "..\Reclamacao\VTAjusta.h"
#include "..\Reclamacao\VTMedicao.h"
#include "..\Reclamacao\VTMulta.h"
#include "..\Reclamacao\VTPerfilAlt.h"
#include "..\Reclamacao\VTPerfilBarra.h"
#include "..\Reclamacao\VTPerfilRede.h"
#include "..\Reclamacao\VTPerfilRegulador.h"
#include "..\Reclamacao\VTRes505.h"
//#include "\Relatorio\VTRelatorioVR.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormAnaliseRT::TFormAnaliseRT(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl;

   //cria lista
   lisEQP = new TList();
   //inicia
   Inicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormAnaliseRT::~TFormAnaliseRT(void)
   {
   //variaveis locais
   VTCfgGraf  *cfg      = (VTCfgGraf*) apl->GetObject(__classid(VTCfgGraf));
   VTPlanejamento  *planejamento  = (VTPlanejamento*) apl->GetObject(__classid(VTPlanejamento));
   VTGrafico  *grafico  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
   VTApl      *apl_base = grafico->Apl;
   VTEstudo   *estudo = planejamento->ExisteEstudo(-1);

   //volta p/ topologia
   grafico->Show(grafTOPO);
   //volta última alternativa
   estudo->DefineAlternativaAtiva(ult_alternativa, NULL);
   //elimina objeto inserido na classe APL básica
   if(cfg) {apl_base->Remove(cfg); delete cfg;}
   //destroi lista
   if(lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionAnaliseGeralExecute(TObject *Sender)
   {
   //cria form com Analise de todas as alternativas
   //variavel local
   TFormAnaliseGeral *form;

   //esconde formAnaliseRT
   Hide();
   //cria TFormAnaliseGeral
   form  = new TFormAnaliseGeral(this, apl);
   if(form)
      {//mostra form
      form->ShowModal();
      //destroi form
      delete form;
      }
   //reexibe formAnaliseRT
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionChartCorrentesExecute(TObject *Sender)
   {
   //variáveis locais
   TListItem   *item;
   VTRegulador *reg;

   if(ListViewReg->ItemIndex < 0) return;
   //atualiza ChartCorrentes pelo regulador selecionado
   item = ListViewReg->Items->Item[ListViewReg->ItemIndex];
   reg = (VTRegulador*)item->Data;
   multa->Regulador = reg;
   PreencheChartResultadosCorrReg(reg);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionCfgGrafExecute(TObject *Sender)
   {
   //variaveis locais
   TFormCfgGraf *formCfg;
   VTGrafico *grafico = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   //cria form para configuração grafica
   formCfg = new TFormCfgGraf(this, apl);
   if(formCfg)
      {
      Hide();
      formCfg->ShowModal();
      delete formCfg;
      Show();
      }
   grafico->Show(grafREGULADOR);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionCfgTensaoExecute(TObject *Sender)
   {
   //variaveis locais
   TFormCfgTensao *formCfg;
   VTGrafico *grafico = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   //cria form para configuração de faixas de tensão
   formCfg = new TFormCfgTensao(this, apl);
   if(formCfg)
      {
      Hide();
      formCfg->ShowModal();
      if(formCfg->ModalResult == mrOk)
         {//res505 foi alterado: recalcula
         cboxDataChange(NULL);  //DVK 2015.03.26
         IniciaGraficos();
         PageControlAlternativasChange(NULL);
         }
      delete formCfg;
      Show();
      }
   grafico->Show(grafREGULADOR);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionExportaRelatorioExecute(TObject *Sender)
   {
   //variaveis locais
   AnsiString      nome_arq;

   //esconde formAnaliseRT
   Hide();
   Application->ModalPopupMode = pmAuto;
   //define nome do arquivo como Alternativa ativa
   SaveDialogAlternativa->FileName = PageControlAlternativas->ActivePage->Caption;
   if(! SaveDialogAlternativa->Execute()) {Show(); return;}
   //exporta em arq. txt
   nome_arq = SaveDialogAlternativa->FileName;
   if (! ExportaRelatorio(nome_arq) )
//      Aviso("A tabela foi salva em " + SaveDialogAlternativa->FileName);
//   else
      Erro("Erro ao criar arquivo.");
   //reexibe formAnaliseRT
   Show();

   //codigo temporariamente bloqueado
   Aviso("Relatório indisponível");
   }
   */
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionExportaCorrenteExecute(TObject *Sender)
   {
   //variaveis locais
   TFormRelTensoes *form;

   //cria form
   form = new TFormRelTensoes(this, apl, SeriesIa, SeriesIb, SeriesIc, false);
   if(form)
      {
      Hide();
      form->ShowModal();
      delete form;
      Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionExportaTensaoExecute(TObject *Sender)
   {
   //variaveis locais
   TFormRelTensoes *form;
//   VTPerfilBarra   *perfil_barra;
//   VTPerfilAlt     *perfil_alt;
//   VTPlanejamento  *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
//   VTEstudo          *estudo          = planejamento->ExisteEstudo(-1);
//
//   //proteção
//   if(estudo == NULL) return;
//   if (medicao->NumeroDia == 0) return;
//   //obtém perfil de tensão da Barra da carga
//   perfil_alt = (VTPerfilAlt*)estudo->AlternativaAtiva->Object;
//   if(perfil_alt == NULL) return;
//   if ((perfil_barra = perfil_alt->ExistePerfilBarra(carga->pbarra)) == NULL) return;
   //cria form
   form = new TFormRelTensoes(this, apl, SeriesVA, SeriesVB, SeriesVC, true);
   if(form)
      {
      Hide();
      form->ShowModal();
      delete form;
      Show();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionGrafCarregamentoExecute(TObject *Sender)
   {
   //variáveis locais
   VTCfgGraf *cfg     = (VTCfgGraf*) apl->GetObject(__classid(VTCfgGraf));
   VTGrafico *grafico = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   cfg->View = viewCARREG;
   grafico->Show(grafREGULADOR);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionGrafReguladorExecute(TObject *Sender)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   grafico->Show(grafREGULADOR);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ActionGrafTensaoExecute(TObject *Sender)
   {
   //variáveis locais
   VTCfgGraf *cfg     = (VTCfgGraf*) apl->GetObject(__classid(VTCfgGraf));
   VTGrafico *grafico = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   cfg->View = viewTENSAO;
   grafico->Show(grafREGULADOR);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::cboxDataChange(TObject *Sender)
   {
   //variaveis locais
   int              ind_dia;
   VTEstudo       *estudo;
   VTPerfilAlt    *perfil_alt;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

   //proteção
   estudo = planejamento->ExisteEstudo();
   if(! estudo) return;
   perfil_alt = (VTPerfilAlt*)estudo->AlternativaAtiva->Object;
   //proteção
   if (cboxData->ItemIndex < 0) return;
   //verifica se selecionou opção de todos dias
   if (cboxData->ItemIndex == 0)
      {//recalcula índices de qualidade considerando todos dias
      perfil_alt->CalculaDRA_DRP_DRC();
      }
   else if (cboxData->ItemIndex == 1)
      {//recalcula índices de qualidade considerando 1008 pontos de medição
      perfil_alt->CalculaDRA_DRP_DRC(1008);
      }
   else
      {//determina dia selecionado
      ind_dia = cboxData->ItemIndex - 2;
      //recalcula índices de qualidade considerando os dias selecionados
      perfil_alt->CalculaIndicesQualidadeUmDia(ind_dia);
      }
   //apresenta os resultados recalculados
   PreencheGraficos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::ComboBoxDataInicia(void)
   {
   //variaveis locais
   int  num_dia = medicao->NumeroDia;

   //reinicia
   cboxData->Visible = true;
   cboxData->Items->Clear();
   if (num_dia == 0) return;
   //insere opção de mostrar medição de todos os dias
   cboxData->Items->Add("Todos os dias");
   //insere opção de mostrar medição de todos os dias, limitado a 1008 pontos
   cboxData->Items->Add("7 dias (1008 amostras)");
   //insere datas da medição
   for(int ind_dia = 0; ind_dia < num_dia; ind_dia++)
      {
      cboxData->Items->Add(medicao->Data[ind_dia].FormatString("dd/mm/yyyy dddd"));
      }
   //seleciona item inicial
   cboxData->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::CriaAbasComAlternativas(void)
   {
   //variaveis locais
   TList            *lisALT;
   VTAlternativa   *alternativa;
   VTEstudo         *estudo;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

   estudo = planejamento->ExisteEstudo(-1);
   //importa medição e realiza ajustes para todas as alternativas
   lisALT = estudo->LisAlternativa();
   for(int na = 0; na < lisALT->Count; na++)
      {
      alternativa = (VTAlternativa*)lisALT->Items[na];
      if(alternativa != estudo->AlternativaPadrao)
         {//cria abas
         TTabSheet *novaTabSheet = new TTabSheet(PageControlAlternativas);
         novaTabSheet->PageControl = PageControlAlternativas;
         novaTabSheet->Caption = alternativa->Codigo;
         novaTabSheet->Name = AnsiString("ts") + IntToStr(na);
         if(alternativa == estudo->AlternativaAtiva)
            {PageControlAlternativas->ActivePage = novaTabSheet;}
         }
      }
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFormAnaliseRT::ExportaRelatorio(AnsiString nome_arq)
   {
   //variaveis locais
   AnsiString       FileNameBase;
   VTPath          *path           = (VTPath*)apl->GetObject(__classid(VTPath));
   VTStk          *stk          = (VTStk*)apl->GetObject(__classid(VTStk));
   VTProgresso    *prog         = (VTProgresso*)stk->NewObjProgresso(NULL, NULL);
   VTRelatorioVR    *FRelatorioVR = NewObjRelatorioVR(apl);

   prog->Show("Exportando dados para Excel...", 0, 3);
   prog->NextPosition();
   FileNameBase = path->DirDat() + "\\Relatorio\\AnaliseRT.xls";
   //cria copia do arquivo base
   if(!FRelatorioVR->IniciarRelatorio(FileNameBase, nome_arq)) {prog->Hide();return(false);}
   prog->NextPosition();
   //monta arquivo
//   if(!FRelatorioVR->Monta(carga, cod_rede_sec, cod_suprimento, true)) {prog->Hide();return(false);}
   if(!FRelatorioVR->Monta(true)) {prog->Hide();return(false);}
   prog->NextPosition();
   if(!FRelatorioVR->Fecha()) {prog->Hide();return(false);}
   prog->NextPosition();
   prog->Hide();
   //destroi
   if(FRelatorioVR != NULL){delete FRelatorioVR; FRelatorioVR = NULL;}
   if(prog)   {delete prog; prog = NULL;}
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFormAnaliseRT::ExportaRelatorio(AnsiString nome_arq)
   {
   Aviso("Função não disponível nesta versão");
   return(true);
   }
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::Inicia(void)
   {
   //variaveis locais
   VTGrafico  *grafico  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo         *estudo = planejamento->ExisteEstudo(-1);

   ajusta    = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
   medicao    = (VTMedicao*)apl->GetObject(__classid(VTMedicao));
   multa    = (VTMulta*)apl->GetObject(__classid(VTMulta));
   //cria objeto CfgGrafReg e insere no Apl acessado pelo objeto Grafico
   grafico->Apl->Add(NewObjCfgGrafReg(apl));
   //guarda alternativa ativa
   ult_alternativa = estudo->AlternativaAtiva;
   //trata alternativa padrão
   if(estudo->AlternativaPadrao->Object == NULL)
      {
      estudo->AlternativaPadrao->Object = (VTPerfilAlt*)ajusta->LisPerfilAlt->Items[0];
      estudo->AlternativaPadrao->Obras->Analisada = true;
      }
   //inicia
   ComboBoxDataInicia();
   CriaAbasComAlternativas();
   IniciaGraficos();
   PreencheLViewReg();
   PageControlAlternativasChange(NULL);
   PreencheLViewDadosCons();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::IniciaGraficos(void)
   {
   //variáveis locais
   int    index = 0;
   TColor colorA = clMoneyGreen;
   TColor colorP = clWebGold;   //clCream;
   TColor colorC = clWebDarkRed; //clRed;
   TDateTime        data0, data1, data2;
   VTRes505       *res505    = (VTRes505*)apl->GetObject(__classid(VTRes505));

   //reinicia séries de medição
   SeriesV1->Clear();
   SeriesV2->Clear();
   SeriesV3->Clear();
   //reinicia séries de tensão calculada
   SeriesVA->Clear();
   SeriesVB->Clear();
   SeriesVC->Clear();
   //reinicia séries de corrente no regulador
   SeriesIa->Clear();
   SeriesIb->Clear();
   SeriesIc->Clear();
   //reinicia rgFases
//   rgFases->Items->Clear();

   //reinicia limites dos eixos
   if(chartTensao->LeftAxis->Maximum < chartTensao->LeftAxis->Minimum)
      {
      chartTensao->LeftAxis->Maximum = fabs(10 * chartTensao->LeftAxis->Minimum);
      }
   chartTensao->LeftAxis->Minimum = 0;
   chartTensao->LeftAxis->Maximum = 0;
   //
   rgFases->ItemIndex = 0;
   //faixa adequada, precaria e critica
//   SeriesAd->Clear();      SeriesAd->Visible;      SeriesAd->ShowInLegend = false;
//   SeriesPr_inf->Clear(); SeriesPr_inf->Visible;  SeriesPr_inf->ShowInLegend = false;
//   SeriesPr_sup->Clear(); SeriesPr_sup->Visible;  SeriesPr_sup->ShowInLegend = false;
//   SeriesCr_inf->Clear(); SeriesCr_inf->Visible;  SeriesCr_inf->ShowInLegend = false;
//   SeriesCr_sup->Clear(); SeriesCr_sup->Visible;  SeriesCr_sup->ShowInLegend = false;
   SeriesAd->Clear();     SeriesAd->ShowInLegend = false;
   SeriesPr_inf->Clear(); SeriesPr_inf->ShowInLegend = false;
   SeriesPr_sup->Clear(); SeriesPr_sup->ShowInLegend = false;
   SeriesCr_inf->Clear(); SeriesCr_inf->ShowInLegend = false;
   SeriesCr_sup->Clear(); SeriesCr_sup->ShowInLegend = false;
   //define origem da Series
   SeriesAd->YOrigin     = res505->adequado.inf * 1000.;    //adequada.inf em Volt
   SeriesCr_inf->YOrigin = (res505->precario1.inf*0.8) * 1000.0;
   SeriesPr_inf->YOrigin = res505->precario1.inf * 1000.0;
   SeriesPr_sup->YOrigin = res505->precario2.inf * 1000.0;
   SeriesCr_sup->YOrigin = res505->precario2.sup * 1000.0;
   //loop p/ todos dias
   data0 = medicao->Data[0];   //inicia data
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//redefine color
      colorA = (colorA == clMoneyGreen) ? TColor(clMoneyGreen | clMedGray) : clMoneyGreen;
      colorP = (colorP == clWebGold)     ? TColor(clWebGold | clWebYellow) : clWebGold;
      colorC = (colorC == clWebDarkRed) ? TColor(clWebDarkRed | clRed) : clWebDarkRed;
      //loop p/ todos os patamares
      for (int ind_pat = 0; ind_pat < medicao->NumeroMedicaoPorDia; ind_pat++, index++)
         {
         data1 = data0 + index*1.0/(medicao->NumeroMedicaoPorDia);   //patamar inicial
         data2 = data0 + (index+1)*1.0/(medicao->NumeroMedicaoPorDia);   //acrescenta 10 min p/ efeito patamar
         //adequada
         SeriesAd->AddXY(data1, res505->adequado.sup* 1000., "", colorA);
         SeriesAd->AddXY(data2, res505->adequado.sup* 1000., "", colorA);
         //precaria.inf
         SeriesPr_inf->AddXY(data1, res505->precario1.sup* 1000., "", colorP);
         SeriesPr_inf->AddXY(data2, res505->precario1.sup* 1000., "", colorP);
         //precaria.sup
         SeriesPr_sup->AddXY(data1, res505->precario2.sup* 1000., "", colorP);
         SeriesPr_sup->AddXY(data2, res505->precario2.sup* 1000., "", colorP);
         //critica.inf
         SeriesCr_inf->AddXY(data1, res505->critico.inf* 1000., "", colorC);
         SeriesCr_inf->AddXY(data2, res505->critico.inf* 1000., "", colorC);
         //critica.sup
		 SeriesCr_sup->AddXY(data1, (res505->critico.sup*1.2)* 1000., "", colorC);
		 SeriesCr_sup->AddXY(data2, (res505->critico.sup*1.2)* 1000., "", colorC);
         }
      }
   chartTensao->LeftAxis->Maximum = (res505->critico.sup* 1000.) * 1.3;
   chartTensao->LeftAxis->Minimum = (res505->critico.inf* 1000.) * 0.7;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PageControlAlternativasChange(TObject *Sender)
   {
   //variaveis locais
   AnsiString      pageSel;
   VTAlternativa   *alternativa;
   VTGrafico       *grafico = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
   VTPerfilAlt    *perfil_alt;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo         *estudo       = planejamento->ExisteEstudo(-1);
   TList            *lisALT       = estudo->LisAlternativa();

   //proteção
   if(estudo == NULL) return;
   //define alternativa da aba selecionada
   pageSel = PageControlAlternativas->ActivePage->Caption;
   for(int na = 0; na < lisALT->Count; na++)
      {//localiza alternativa a ser efetivada
      alternativa = (VTAlternativa*)lisALT->Items[na];
      if(alternativa->Codigo.AnsiPos(pageSel) != 0)
         {//verifica se é alternativa ativa
         if(alternativa != estudo->AlternativaAtiva)
            {//executa alternativa
            estudo->DefineAlternativaAtiva(alternativa, NULL);
            }
         //verifica se houve mudanças nas obras da alternativa
         if((! alternativa->Obras->Analisada)&&(alternativa != estudo->AlternativaPadrao))
//         if(alternativa->Obras->Modificada)
//         if(alternativa->Object == NULL)
            {//atualiza perfil_alt
            if(! ajusta->CalculaFluxoComObra()) return;
            alternativa->Obras->Analisada = true;
//            alternativa->Obras->Modificada = false;
            }
         break;
         }
      }
   cboxData->ItemIndex = 0;
   //preenche gráficos com resultados da alternativa
   PreencheGraficos();
   //apresenta o grafico
   grafico->Show(grafREGULADOR);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PreencheChartIndices(void)
   {
   //variáveis local
   int            fase;
   VTCarga        *carga = multa->Carga;
   VTPerfilBarra *perfil_barra;
   VTPerfilAlt    *perfil_alt;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo         *estudo          = planejamento->ExisteEstudo(-1);

   //proteção
   if(estudo == NULL) return;
   //reinicia  ChartSeries
   PieSeriesComObra->Clear();
   //proteção
   if ((medicao->NumeroDia == 0)) return;
   //determina a fase
   switch (rgFases->ItemIndex)
      {
      case 0:  fase = faseA; break;
      case 1:  fase = faseB; break;
      case 2:  fase = faseC; break;
      default: fase = faseA; break;
      }
   //verifica se a Carga possui a fase selecionada
   if((carga->Fases & fase) != fase)
      {//Carga não possui a fase selecionada : esconde TCharts
      ChartIndiceComObra->Visible = false;
      return;
      }
   //determina PerfilBarra da alternativa
   perfil_alt = (VTPerfilAlt*)estudo->AlternativaAtiva->Object;
   if(perfil_alt == NULL) return;
   if ((perfil_barra = perfil_alt->ExistePerfilBarra(carga->pbarra)) != NULL)
      {//exibe TChart
      ChartIndiceComObra->Visible = true;
      //insere valores
      PieSeriesComObra->Add(perfil_barra->DRA_perc(fase), "Adequada", clGreen);
      PieSeriesComObra->Add(perfil_barra->DRP_perc(fase), "Precária", clYellow);
      PieSeriesComObra->Add(perfil_barra->DRC_perc(fase), "Crítica",  clRed);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PreencheChartMedicao(void)
   {
   //variaveis  locais
   int             index = 0;
   int             num_amostra = 0;
   double          vfn_kv[MAX_FASE];
   TDateTime       data0, data1, data2;
   VTCarga        *carga = multa->Carga;

   //limpa LineSeries
   SeriesV1->Clear();
   SeriesV2->Clear();
   SeriesV3->Clear();
   //proteção
   if (medicao->NumeroDia == 0) return;
   if (cboxData->ItemIndex < 0) return;
   //não preenche com medição se a carga analisada for diferente da medida
   if (carga != medicao->Carga)
      {
      SeriesV1->Visible = false;
      SeriesV2->Visible = false;
      SeriesV3->Visible = false;
      SeriesV1->ShowInLegend = false;
      SeriesV2->ShowInLegend = false;
      SeriesV3->ShowInLegend = false;
      return;
      }
   //exibe/esconde
   SeriesV1->Visible = medicao->FaseEnabled[indFASE_A];
   SeriesV2->Visible = medicao->FaseEnabled[indFASE_B];
   SeriesV3->Visible = medicao->FaseEnabled[indFASE_C];
   SeriesV1->ShowInLegend = medicao->FaseEnabled[indFASE_A];
   SeriesV2->ShowInLegend = medicao->FaseEnabled[indFASE_B];
   SeriesV3->ShowInLegend = medicao->FaseEnabled[indFASE_C];
   //loop p/ todos dias de medição
   data0 = medicao->Data[0];  //inicia data 00:00
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//loop p/ todos os patamares
      //data0 = medicao->Data[ind_dia];   //inicia data 00:00
      for (int ind_pat = 0; ind_pat < medicao->NumeroMedicaoPorDia; ind_pat++, index++)
         {//reinicia vetor de tensões
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++) {vfn_kv[ind_fase] = 0.;}
         //verifica opção de data selecionada
         if ( (cboxData->ItemIndex == 0)||
             ((cboxData->ItemIndex == 1)&&(num_amostra <= LIMITE_AMOSTRA))||
             ((cboxData->ItemIndex >= 2)&&(ind_dia == (cboxData->ItemIndex-2))))
            {//obtém medição das tensões
            if (medicao->ExisteMedicao(ind_dia, ind_pat, vfn_kv)) num_amostra++;
            else continue;
            }
         data1 = data0 + index*1.0/(medicao->NumeroMedicaoPorDia);   // patamar inicial da medição
         data2 = data0 + (index+1)*1.0/(medicao->NumeroMedicaoPorDia);       //acrescenta 10 min p/ efeito patamar
         //verifica se existe medição p/ fase A
         if (medicao->FaseEnabled[indFASE_A] && (vfn_kv[indFASE_A] > 0))
            {
            SeriesV1->AddXY(data1, vfn_kv[indFASE_A]* 1000.);
            SeriesV1->AddXY(data2, vfn_kv[indFASE_A]* 1000.);
            }
         //verifica se existe medição p/ fase B
         if (medicao->FaseEnabled[indFASE_B] && (vfn_kv[indFASE_B] > 0))
            {
            SeriesV2->AddXY(data1, vfn_kv[indFASE_B]* 1000.);
            SeriesV2->AddXY(data2, vfn_kv[indFASE_B]* 1000.);
            }
         //verifica se existe medição p/ fase C
         if (medicao->FaseEnabled[indFASE_C] && (vfn_kv[indFASE_C] > 0))
            {
            SeriesV3->AddXY(data1, vfn_kv[indFASE_C]* 1000.);
            SeriesV3->AddXY(data2, vfn_kv[indFASE_C]* 1000.);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PreencheChartResultados(void)
   {
   //variaveis locais
   int           index       = 0;
   int           num_amostra = 0;
   double        valor;
   double        vfn_kv[MAX_FASE];
   VTCarga        *carga = multa->Carga;
   VTPerfilBarra *perfil_barra;
   TDateTime      data0, data1, data2;
   VTPerfilAlt    *perfil_alt;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo         *estudo          = planejamento->ExisteEstudo(-1);

   //proteção
   if(estudo == NULL) return;
   //limpa todas as LineSeries
   SeriesVA->Clear();
   SeriesVB->Clear();
   SeriesVC->Clear();
   //proteção
   if (medicao->NumeroDia == 0) return;
   if (cboxData->ItemIndex < 0) return;
   //obtém perfil de tensão da Barra da carga
   perfil_alt = (VTPerfilAlt*)estudo->AlternativaAtiva->Object;
   if(perfil_alt == NULL) return;
   if ((perfil_barra = perfil_alt->ExistePerfilBarra(carga->pbarra)) == NULL) return;
   //exibe/esconde as LineSeries de cada fase
   SeriesVA->Visible = ((carga->Fases & faseA) == faseA)&&(perfil_barra);
   SeriesVB->Visible = ((carga->Fases & faseB) == faseB)&&(perfil_barra);
   SeriesVC->Visible = ((carga->Fases & faseC) == faseC)&&(perfil_barra);
   SeriesVA->ShowInLegend = ((carga->Fases & faseA) == faseA)&&(perfil_barra);
   SeriesVB->ShowInLegend = ((carga->Fases & faseB) == faseB)&&(perfil_barra);
   SeriesVC->ShowInLegend = ((carga->Fases & faseC) == faseC)&&(perfil_barra);
   //loop p/ todos dias de medição
   data0 = medicao->Data[0];   //inicia data
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {
      //loop p/ todos os patamares
      for (int ind_pat = 0; ind_pat < medicao->NumeroMedicaoPorDia; ind_pat++, index++)
         {//reinicia vetor de tensões
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++) {vfn_kv[ind_fase] = 0.;}
         if ( (cboxData->ItemIndex == 0)||
             ((cboxData->ItemIndex == 1)&&(num_amostra <= LIMITE_AMOSTRA))||
             ((cboxData->ItemIndex >= 2)&&(ind_dia == (cboxData->ItemIndex-2))))
            {//determina tensões da Barra
            if (perfil_barra->Vfn_kv(ind_dia, ind_pat, vfn_kv))  num_amostra++;
            else continue;
            }
            data1 = data0 + index*1.0/(medicao->NumeroMedicaoPorDia);   //patamar inicial
            data2 = data0 + (index+1)*1.0/(medicao->NumeroMedicaoPorDia);   //acrescenta 10 min p/ efeito patamar
         //verifica se deve exibir tensões da fase A
         if (SeriesVA->Visible && (vfn_kv[indFASE_A] > 0))
            {
            valor = vfn_kv[indFASE_A]* 1000.;
            SeriesVA->AddXY(data1, valor);
            SeriesVA->AddXY(data2, valor);
            }
         if (SeriesVB->Visible && (vfn_kv[indFASE_B] > 0))
            {
            valor = vfn_kv[indFASE_B]* 1000.;
            SeriesVB->AddXY(data1, valor);
            SeriesVB->AddXY(data2, valor);
            }
         if (SeriesVC->Visible && (vfn_kv[indFASE_C] > 0))
            {
            valor = vfn_kv[indFASE_C]* 1000.;
            SeriesVC->AddXY(data1, valor);
            SeriesVC->AddXY(data2, valor);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PreencheChartResultadosCorrReg(VTRegulador *reg)
   {
   //variaveis locais
   TColor            colorA = clMoneyGreen;
   int                index = 0;
   int               num_amostra = 0;
   double             Iantes[MAX_FASE], Idepois[MAX_FASE], maxAntes=0.0, maxDepois=0.0;
   TColor             color = clMoneyGreen;
   TDateTime          data0, data1, data2;
   VTPerfilAlt       *perfil_alt;
   VTPerfilRegulador *perfil_reg;        
   VTPlanejamento    *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo            *estudo          = planejamento->ExisteEstudo(-1);
//   VTRegulador       *reg;

   //proteção
   if(estudo == NULL) return;
   //limpa series
   SeriesIa->Clear();
   SeriesIb->Clear();
   SeriesIc->Clear();
   SerieAdCorr->Clear();
//   SerieCrCorr->Clear();
   //verifica se há um regulador
//   if((reg = DeterminaRegulador()) == NULL)
   if (reg == NULL)
      {
//      chartTensao->Title->Text->Clear();
//      chartTensao->Title->Text->Add("Tensões [V]");
//      SeriesIa->Visible = false;
//      SeriesIb->Visible = false;
//      SeriesIc->Visible = false;
//      SeriesIa->ShowInLegend = false;
//      SeriesIb->ShowInLegend = false;
//      SeriesIc->ShowInLegend = false;
      PanelReg->Visible = false;
      ActionExportaCorrente->Enabled = false;
      return;
      }
   PanelReg->Visible = true;
   ActionExportaCorrente->Enabled = true;
   //titulo
//   chartTensao->Title->Text->Clear();
//   chartTensao->Title->Text->Add("Tensões [V] e Correntes [A]");
   //mostra series
   SeriesIa->Visible       = ((reg->fases & faseA)==faseA);
   SeriesIa->ShowInLegend  = ((reg->fases & faseA)==faseA);
   SeriesIb->Visible       = ((reg->fases & faseB)==faseB);
   SeriesIb->ShowInLegend  = ((reg->fases & faseB)==faseB);
   SeriesIc->Visible       = ((reg->fases & faseC)==faseC);
   SeriesIc->ShowInLegend  = ((reg->fases & faseC)==faseC);
   //determina PerfilRegulador da rede com obra
   perfil_alt = (VTPerfilAlt*)estudo->AlternativaAtiva->Object;
   if(perfil_alt == NULL) return;
   if ((perfil_reg = perfil_alt->ExistePerfilReg(reg)) == NULL) return;
   //loop p/ todos dias de medição
   data0 = medicao->Data[0];   //inicia data
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//alterna cores da faixa adequada
      colorA = (colorA == clMoneyGreen) ? TColor(clMoneyGreen | clMedGray) : clMoneyGreen;
      //loop p/ todos os patamares
      for (int ind_pat = 0; ind_pat < medicao->NumeroMedicaoPorDia; ind_pat++, index++)
         {//reinicia vetor de correntes
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {Iantes[ind_fase] = 0.; Idepois[ind_fase] = 0.;}
         //verifica opção de data selecionada
         if ( (cboxData->ItemIndex == 0)||
            ((cboxData->ItemIndex == 1)&&(num_amostra <= LIMITE_AMOSTRA))||
            ((cboxData->ItemIndex >= 2)&&(ind_dia == (cboxData->ItemIndex-2))))
            {//determina correntes
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {//define correntes antes e depois do regulador, em cada fase
               perfil_reg->Corrente(ind_dia,ind_pat,ind_fase,
                  &Iantes[ind_fase], &Idepois[ind_fase]);
               }
            if((Idepois[indFASE_A]>0)||(Idepois[indFASE_B]>0)||(Idepois[indFASE_C]>0))
               num_amostra++;
            else continue;
            }
         //define data
         data1 = data0 + index*1.0/(medicao->NumeroMedicaoPorDia);   // patamar inicial
         data2 = data0 + (index+1)*1.0/(medicao->NumeroMedicaoPorDia);   //acrescenta 10 min p/ efeito patamar
         //insere faixas adequada e crítica para correntes
         SerieAdCorr->YOrigin = 0.0;
         SerieAdCorr->AddXY(data1, 112.0,"",colorA);
         SerieAdCorr->AddXY(data2, 112.0,"",colorA);
//         SerieCrCorr->YOrigin = 150.0;
//         SerieCrCorr->AddXY(data1, 200.0);
//         SerieCrCorr->AddXY(data2, 200.0);
         if ((SeriesIa->Visible)&&(Idepois[indFASE_A] > 0))
            {//desenha correntes na fase A
            maxDepois = Max(maxDepois, Idepois[indFASE_A]);      //define valores max
            SeriesIa->AddXY(data1, Idepois[indFASE_A]);
            SeriesIa->AddXY(data2, Idepois[indFASE_A]);
            }
         if ((SeriesIb->Visible)&&(Idepois[indFASE_B] > 0))
            {//desenha correntes na fase B
            maxDepois = Max(maxDepois, Idepois[indFASE_B]);      //define valores max
            SeriesIb->AddXY(data1, Idepois[indFASE_B]);
            SeriesIb->AddXY(data2, Idepois[indFASE_B]);
            }
         if ((SeriesIc->Visible)&&(Idepois[indFASE_C] > 0))
            {//desenha correntes na fase C
            maxDepois = Max(maxDepois, Idepois[indFASE_C]);      //define valores max
            SeriesIc->AddXY(data1, Idepois[indFASE_C]);
            SeriesIc->AddXY(data2, Idepois[indFASE_C]);
            }
         }
      }
   //limites do eixo correntes
   chartCorrente->LeftAxis->Minimum = 0.0;
   chartCorrente->LeftAxis->Maximum = 1.3*maxDepois;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PreencheChartResultadosCorrReg(void)
   {
   //variaveis locais
   int                index = 0;
   double             Iantes[MAX_FASE], Idepois[MAX_FASE], maxAntes=0.0, maxDepois=0.0;
   VTRegulador       *reg;
   VTPerfilRegulador *perfil_reg;
   TColor             color = clMoneyGreen;
   TDateTime          data0, data1, data2;
   VTPerfilAlt    *perfil_alt;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo         *estudo          = planejamento->ExisteEstudo(-1);

   //proteção
   if(estudo == NULL) return;
   //verifica se há um regulador
   if((reg = DeterminaRegulador()) == NULL)
      {
      chartTensao->Title->Text->Clear();
      chartTensao->Title->Text->Add("Tensões [kV]");
      SeriesIa->Visible = false;
      SeriesIb->Visible = false;
      SeriesIc->Visible = false;
      SeriesIa->ShowInLegend = false;
      SeriesIb->ShowInLegend = false;
      SeriesIc->ShowInLegend = false;
      return;
      }
   //titulo
   chartTensao->Title->Text->Clear();
   chartTensao->Title->Text->Add("Tensões [kV] e Correntes [A]");
   //mostra series
   if((reg->fases & faseA)==faseA)
      {
      SeriesIa->Visible = true;
      SeriesIa->ShowInLegend = true;
      }
   if((reg->fases & faseB)==faseB)
      {
      SeriesIb->Visible = true;
      SeriesIb->ShowInLegend = true;
      }
   if((reg->fases & faseC)==faseC)
      {
      SeriesIc->Visible = true;
      SeriesIc->ShowInLegend = true;
      }
   //loop p/ todos dias de medição
   data0 = medicao->Data[0];   //inicia data
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//loop p/ todos os patamares
      for (int ind_pat = 0; ind_pat < medicao->NumeroMedicaoPorDia; ind_pat++, index++)
         {//reinicia vetor de correntes
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {Iantes[ind_fase] = 0.; Idepois[ind_fase] = 0.;}
         //determina PerfilRegulador da rede com obra
         perfil_alt = (VTPerfilAlt*)estudo->AlternativaAtiva->Object;
         if ((perfil_reg = perfil_alt->ExistePerfilReg(reg)) != NULL)
            {//define data
            data1 = data0 + index*1.0/(medicao->NumeroMedicaoPorDia);   // patamar inicial
            data2 = data1 + 1.0/(medicao->NumeroMedicaoPorDia);   //acrescenta 10 min p/ efeito patamar
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {//define correntes antes e depois do regulador, em cada fase
               if ((perfil_reg->Corrente(ind_dia,ind_pat,ind_fase, &Iantes[ind_fase],
                  &Idepois[ind_fase]))&&(ind_fase == indFASE_A))
                  {//desenha correntes na fase A
                  if(Iantes[ind_fase] == 0.0) continue;
                  maxAntes = Max(maxAntes, Iantes[ind_fase]);      //define valores max
//                  maxDepois = Max(maxDepois, Idepois[ind_fase]);
//                  Iantes_a->AddXY(data1, Iantes[ind_fase]);
//                  Iantes_a->AddXY(data2, Iantes[ind_fase]);
//                  Idepois_a->AddXY(data1, Idepois[ind_fase]);
//                  Idepois_a->AddXY(data2, Idepois[ind_fase]);
                  SeriesIa->AddXY(data1, Idepois[ind_fase]);
                  SeriesIa->AddXY(data2, Idepois[ind_fase]);
                  }
               if ((perfil_reg->Corrente(ind_dia,ind_pat,ind_fase, &Iantes[ind_fase],
                  &Idepois[ind_fase]))&&(ind_fase == indFASE_B))
                  {//desenha correntes na fase B
                  if(Iantes[ind_fase] == 0.0) continue;
                  maxAntes = Max(maxAntes, Iantes[ind_fase]);
//                  maxDepois = Max(maxDepois, Idepois[ind_fase]);
//                  Iantes_b->AddXY(data1, Iantes[ind_fase]);
//                  Iantes_b->AddXY(data2, Iantes[ind_fase]);
//                  Idepois_b->AddXY(data1, Idepois[ind_fase]);
//                  Idepois_b->AddXY(data2, Idepois[ind_fase]);
                  SeriesIb->AddXY(data1, Idepois[ind_fase]);
                  SeriesIb->AddXY(data2, Idepois[ind_fase]);
                  }
               if ((perfil_reg->Corrente(ind_dia,ind_pat,ind_fase, &Iantes[ind_fase],
                  &Idepois[ind_fase]))&&(ind_fase == indFASE_C))
                  {//desenha correntes na fase C
                  if(Iantes[ind_fase] == 0.0) continue;
                  maxAntes = Max(maxAntes, Iantes[ind_fase]);
//                  maxDepois = Max(maxDepois, Idepois[ind_fase]);
//                  Iantes_c->AddXY(data1, Iantes[ind_fase]);
//                  Iantes_c->AddXY(data2, Iantes[ind_fase]);
//                  Idepois_c->AddXY(data1, Idepois[ind_fase]);
//                  Idepois_c->AddXY(data2, Idepois[ind_fase]);
                  SeriesIc->AddXY(data1, Idepois[ind_fase]);
                  SeriesIc->AddXY(data2, Idepois[ind_fase]);
                  }
               }
            }
         }
      }
   //limites do eixo correntes
   chartTensao->RightAxis->Minimum = 0.0;
   chartTensao->RightAxis->Maximum = 1.3*Max(maxAntes,maxDepois);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PreencheGraficos(void)
   {
   //variaveis locais
   VTRegulador    *reg;

   PreencheTViewIndices();
   PreencheChartIndices();
   PreencheChartMedicao();
   PreencheChartResultados();
   PreencheLViewReg();
   if(ListViewReg->Items->Count < 1) PreencheChartResultadosCorrReg(NULL);
   else
      {//preenche chart com o primeiro regulador da lista
      reg = (VTRegulador*)ListViewReg->Items->Item[0]->Data;
      PreencheChartResultadosCorrReg(reg);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PreencheLViewDadosCons(void)
   {
   //variaveis locais
   TList         *lisEQB;
   TListItem   *item;
   VTCarga      *carga = multa->Carga;
   VTFases      *fases = (VTFases*)apl->GetObject(__classid(VTFases));
   VTRede      *rede_sec = multa->RedeSec;
   VTSuprimento *supr = multa->Suprimento;

   //limpa LViewDadosCons
   LViewDadosCons->Items->Clear();
   //preenche
   item = LViewDadosCons->Items->Add();
   item->Caption = "Rede Primária";
   item->SubItems->Add(supr->Codigo);

   item = LViewDadosCons->Items->Add();
   item->Caption = "Rede Secundária";
   item->SubItems->Add(rede_sec->Codigo);

   item = LViewDadosCons->Items->Add();
   item->Caption = "Consumidor Medido";
   item->SubItems->Add(medicao->Carga->Codigo);

   item = LViewDadosCons->Items->Add();
   item->Caption = "Consumidor Analisado";
   item->SubItems->Add(carga->Codigo);

   item = LViewDadosCons->Items->Add();
   item->Caption = "Fases";
   item->SubItems->Add(fases->AsStrABCN[carga->Fases]);

    item = LViewDadosCons->Items->Add();
   item->Caption = "Consumo Mensal";
   item->SubItems->Add(DoubleToStr("%4.1f",carga->Energia_kwhmes, ',')+" kWh/mês");

   item = LViewDadosCons->Items->Add();
   item->Caption = "Consumidores no mesmo ponto";
   lisEQB = carga->pbarra->LisEqbar();
   item->SubItems->Add(lisEQB->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PreencheLViewReg(void)
   {
   //variaveis locais
   TListItem   *item;
   VTRegulador   *reg;
   VTRede      *rede_sec = multa->RedeSec;

   //limpa ListViewReg
   ListViewReg->Items->Clear();
   //obtém lista de reguladores da Rede secundária
   lisEQP->Clear();
   rede_sec->LisLigacao(lisEQP, eqptoREGULADOR);
   for(int nr = 0; nr < lisEQP->Count; nr++)
      {
      item = ListViewReg->Items->Add();
      reg = (VTRegulador*)lisEQP->Items[nr];
      if(reg->Codigo.IsEmpty()) item->Caption = "id = " + IntToStr(reg->Id);
      else                      item->Caption = reg->Codigo;
      item->Data = reg;
//      multa->Regulador = reg;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::PreencheTViewIndices(void)
   {
   //variaveis locais
   AnsiString      dra, drc, drp;
   int             fase_tag;
   TTreeNode     *node;
   VTCarga        *carga = multa->Carga;
   VTFases        *fases = (VTFases*)apl->GetObject(__classid(VTFases));
   VTPerfilBarra *perfil_barra;
   VTPerfilAlt    *perfil_alt;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo         *estudo          = planejamento->ExisteEstudo(-1);

   //proteção
   if(estudo == NULL) return;
   //limpa TViewIndices
   TViewIndices->Items->Clear();
   //determina PerfilBarra da rede com obra
   perfil_alt = (VTPerfilAlt*)estudo->AlternativaAtiva->Object;
   if(perfil_alt == NULL) return;
   if ((perfil_barra = perfil_alt->ExistePerfilBarra(carga->pbarra)) != NULL)
      {//insere valores
      for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
         {//node_pai = cada fase
         fase_tag = fases->IndexToTag[ind_fase];
         if((carga->Fases & fase_tag) == fase_tag)
            {//node_child = DRA, DRP, DRC
            node =  TViewIndices->Items->Add(NULL, "Fase "+fases->AsStrUpper[fase_tag]);
            dra = "Adequado: "+ IntToStr(perfil_barra->DRA(fase_tag)) + "/";
            dra+= IntToStr(perfil_barra->NumeroAmostragem());
            TViewIndices->Items->AddChild(node, dra);
            drp = "Precário: "+ IntToStr(perfil_barra->DRP(fase_tag)) + "/";
            drp+= IntToStr(perfil_barra->NumeroAmostragem());
            TViewIndices->Items->AddChild(node, drp);
            drc = "Crítico: "+ IntToStr(perfil_barra->DRC(fase_tag)) + "/";
            drc+= IntToStr(perfil_barra->NumeroAmostragem());
            TViewIndices->Items->AddChild(node, drc);
            node->Expand(true);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseRT::rgFasesClick(TObject *Sender)
   {
   PreencheChartIndices();
   }

//---------------------------------------------------------------------------
//eof                                      



