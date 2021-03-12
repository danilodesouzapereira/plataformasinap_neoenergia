//---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TPerdareg.h"
#include "TAplPerdatec.h"
#include "VTResumo.h"
#include "VTResumos.h"
#include "VTPerdatec.h"
#include "AjustaEnergia\VTAjustaEnergia.h"
#include "Medicao\TMedicao.h"
#include "Medicao\VTMedicoes.h"
#include "RedePT\VTMontaRedePT.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Radial\VTArvore.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPerdareg
//---------------------------------------------------------------------------
VTPerdareg* __fastcall NewObjPerdareg(VTApl *apl_owner)
   {
   return(new TPerdareg(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TPerdareg::TPerdareg(VTApl *apl_owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl_owner;
   //inicia dados
   sabado.apl  = NULL;
   domingo.apl = NULL;
   diautil.apl = NULL;
   avisoON = true;
   }

//---------------------------------------------------------------------------
__fastcall TPerdareg::~TPerdareg(void)
   {
   //destrói obejtos
   Clear();
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdareg::CalculaPerdaRegulatoria(TDateTime data)
   {
   //variáveis locais
   bool sucesso;

   try{//calcula perda técnica para dia útil
      sucesso = CalculaPerdaTecnicaDiaUtil(data);
      //calcula perda técnica para sábado
      if (sucesso) sucesso = CalculaPerdaTecnicaSabado(data);
      //calcula perda técnica para domingo
      if (sucesso) sucesso = CalculaPerdaTecnicaDomingo(data);
      //inicia Resumos da perda regulatória a partir dos Resumos das perdas técnicas
      if (sucesso) sucesso = MontaArvoreResumoPerdaRegulatoria();
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdareg::CalculaPerdaTecnicaDiaUtil(TDateTime data)
   {
   //variáveis locais
   bool        sucesso;
   VTPerdatec  *perdatec;

   try{//seleciona curvas típicas de dia útil
      //SelecionaCurvaTipica(DIA_UTIL);
      //cria objeto AplPertec
      diautil.apl = new TAplPerdatec(NULL, apl);
      perdatec     = (VTPerdatec*)diautil.apl->GetObject(__classid(VTPerdatec));
      sucesso      = perdatec->Executa(data, DIA_UTIL);
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdareg::CalculaPerdaTecnicaDomingo(TDateTime data)
   {
   //variáveis locais
   bool        sucesso;
   VTPerdatec  *perdatec;

   try{//seleciona curvas típicas de domingo
      //SelecionaCurvaTipica(DOMINGO);
      //cria objeto AplPertec
      domingo.apl  = new TAplPerdatec(NULL, apl);
      perdatec     = (VTPerdatec*)domingo.apl->GetObject(__classid(VTPerdatec));
      sucesso      = perdatec->Executa(data, DOMINGO);
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdareg::CalculaPerdaTecnicaSabado(TDateTime data)
   {
   //variáveis locais
   bool        sucesso;
   VTPerdatec  *perdatec;

   try{//seleciona curvas típicas de sábado
      //SelecionaCurvaTipica(SABADO);
      //cria objeto AplPertec
      sabado.apl   = new TAplPerdatec(NULL, apl);
      perdatec     = (VTPerdatec*)sabado.apl->GetObject(__classid(VTPerdatec));
      sucesso      = perdatec->Executa(data, SABADO);
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TPerdareg::ClearMontaRede(void)
   {
   //variáveis locais
   VTMontaRedePT *montaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));

   montaRedePT->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TPerdareg::Clear(void)
   {
   if (sabado.apl  != NULL) {delete sabado.apl;  sabado.apl  = NULL;}
   if (domingo.apl != NULL) {delete domingo.apl; domingo.apl = NULL;}
   if (diautil.apl != NULL) {delete diautil.apl; diautil.apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TPerdareg::DeterminaDiasMes(TDateTime data)
   {
   //variáveis locais
   unsigned short mes = MonthOf(data);
   unsigned short ano = YearOf(data);

   //inicia estrutura dias_mes
   num_dias.dom = 0;
   num_dias.sab = 0;
   num_dias.dut = 0;
   num_dias.mes = DaysInMonth(data);
   //determina número de dias úteis, sábados e domingos
   for (unsigned short dia = 1; dia <= num_dias.mes; dia++)
      {
      data = TDateTime(ano, mes, dia);
      switch(DayOfWeek(data))
         {
         case DaySunday:   num_dias.dom++; break;
         case DaySaturday: num_dias.sab++; break;
         default:          num_dias.dut++; break;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdareg::Executa(TDateTime data, bool batch)
   {
   //variáveis locais
   bool             sucesso              = false;
//   bool             incluir_rede_isolada = false;
   int              iteracao       = 1;
   int              max_iteracao   = max_ITAJUSTE;  //3;  //DVK 2016.01.20
   double           erro, erro_max = max_ERROAJUSTE;
   VTAjustaEnergia *ajusta   = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));
   VTArvore        *arvore   = (VTArvore*)apl->GetObject(__classid(VTArvore));
   VTMedicoes      *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   VTRedes         *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTResumos       *resumos  = (VTResumos*)apl->GetObject(__classid(VTResumos));

   //habilita ou nao avisos
   avisoON = !batch;
   //reinicia
   Clear();
   //determina número de dias úteis, sábados e domingos
   DeterminaDiasMes(data);
   //monta Arvore c/ hierarquia das redes
   arvore->Data       = data;
   arvore->TipoArvore = arvoreREDE;
   sucesso            = arvore->Inicia(redes->LisMRede());
   //remonta Arvore, substituindo as Redes pelos respectivos Resumos com dados nulos para perdas e balanço
   if (sucesso) sucesso = resumos->IniciaArvoreComResumo();
   //redefine o tipo da Arvore
   if (sucesso) arvore->TipoArvore = arvorePERDATEC;
   //insere Eqptos na Rede para cálculo de perdas
   if (sucesso) sucesso = PreparaRede(data);
   //verifica se há medição para as redes
   if (ExisteMedicao())
      {//executa ajuste inicial de energia das cargas
      ajusta->ExecutaAjusteSemPerda(num_dias.mes);
      do {//processo iterativo de cálculo de perdas para corrigir a energia das Cargas
         sucesso = CalculaPerdaRegulatoria(data);
		 if (sucesso)
            {//executa ajuste das energias e determina maior erro
			erro = ajusta->ExecutaAjusteComPerda(num_dias.mes);
            if ((erro > erro_max) && (iteracao++ >= max_iteracao)) sucesso = false;
            }
		 } while ((sucesso) && (erro > erro_max));
	  }
   //Calcula as perdas independentemente do erro
   if(!sucesso)
   {
		ajusta->SalvaCargaFinal();
		//gera aviso com os erros das medicoes acima do maximo permitido
		if (MensagemErroMedicoes() == idYes)
		{
			sucesso = CalculaPerdaRegulatoria(data);
		}
		else
		{
        	//faz nada;
		}
   }
   else
   {//calcula perdas regulatórias novamente com o último valor definido para a energias das Cargas
		sucesso = CalculaPerdaRegulatoria(data);
   }

   //if (sucesso) sucesso = CalculaPerdaRegulatoria(data);
//   else
//	{
//		if(avisoON)
//			{Alerta("Perda Regulatória: erro no ajuste de energia " + DoubleToStr("%2.1f %%", erro*100., ','));} //DVK 2016.01.20
//	}
   //salva energia final das Cargas para cálculo de perda comercial
   //if (sucesso) ajusta->SalvaCargaFinal();
   //salva perda técnica na Medicao para calcular a perda comercial
   SalvaPerdaTecnicaNaMedicao();
   //destrói objetos que não são mais necessários
   Clear();

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdareg::ExisteMedicao(void)
   {
   //variáveis locais
   VTMedicoes  *medicoes   = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   TList       *lisMEDICAO = medicoes->LisMedicao();

   return(lisMEDICAO->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdareg::ImportaMedicao(TDateTime data, TStrings *files)
   {
   //variáveis locais
   VTAjustaEnergia *ajusta   = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));
   VTMedicoes      *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

   //determina número de dias úteis, sábados e domingos
   DeterminaDiasMes(data);
   //reinicia medições
   medicoes->Clear();
   //lê dados de medição a partir dos arquivos
   if (! medicoes->ImportaMedicao(files)) return(false);
   //inicia dados das Medicoes: conjunto de redes e cargas
   if (! ajusta->IniciaAllMedicao(num_dias.mes,false)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TPerdareg::MensagemErroMedicoes(void)
{
   AnsiString       msgErro;
   AnsiString		txtErrosMedicao;
   double			erroAux;
   VTMedicoes      	*medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   TList       		*lisMEDICAO = medicoes->LisMedicao();
   TMedicao   		*medicao;
   int	confirmacao;
   double           erro_max = max_ERROAJUSTE;

   txtErrosMedicao = "";
	//verifica as medicoes
	for (int nm = 0; nm < lisMEDICAO->Count; nm++)
	{
		erroAux = 0.;
		medicao = (TMedicao*) lisMEDICAO->Items[nm];
		if (!IsDoubleZero(medicao->energia.medida_mwhmes))
		{
//			erroAux = fabs(medicao->energia.medida_mwhmes
//						- medicao->energia.carga_fim_mwhmes)
//						/ medicao->energia.medida_mwhmes;
			erroAux = max(medicao->fator.bt,medicao->fator.mt);
			erroAux = fabs(erroAux - 1.0);
			if (erroAux > erro_max)
			{
				if (medicao->rede)
				{
					txtErrosMedicao += medicao->rede->Codigo;
					txtErrosMedicao += " erro: " + DoubleToStr("%2.1f %%", erroAux*100., ',');
					txtErrosMedicao += "\n";
				}
			}
		}
	}
	//caso tenha tido alguma medicao com erro superior à tolerancia
	if (!txtErrosMedicao.IsEmpty())
	{
		if(avisoON)
		{
			msgErro = "O erro do ajuste de energia das seguintes medições ficaram acima  de 1%\n";
			msgErro += txtErrosMedicao;
			confirmacao = Confirma(msgErro, "Deseja calcular as perdas mesmo assim?");
		}
	}
	else
	{
		confirmacao = idYes;
	}

	return confirmacao;
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
bool __fastcall TPerdareg::MontaArvoreResumoPerdaRegulatoria(void)
   {
   //variáveis locais
   TList *lisRESUMO;
   struct{
         VTResumo  *resumo;
         VTResumos *resumos;
         }pr, dut, sab, dom;

   //obtém objeto Resumos
   pr.resumos  = (VTResumos*)apl->GetObject(__classid(VTResumos));
   dut.resumos = (VTResumos*)diautil.apl->GetObject(__classid(VTResumos));
   dom.resumos = (VTResumos*)domingo.apl->GetObject(__classid(VTResumos));
   sab.resumos = (VTResumos*)sabado.apl->GetObject(__classid(VTResumos));
   //loop para todos Resumo em pr.resumos
   lisRESUMO = pr.resumos->LisResumo();
   for (int n = 0; n < lisRESUMO->Count; n++)
      {
      pr.resumo = (VTResumo*)lisRESUMO->Items[n];
      //determina Resumo de dia útil da mesma rede
      if ((dut.resumo = dut.resumos->ExisteResumo(pr.resumo->eqpto)) == NULL) continue;
      if ((dom.resumo = dom.resumos->ExisteResumo(pr.resumo->eqpto)) == NULL) continue;
      if ((sab.resumo = sab.resumos->ExisteResumo(pr.resumo->eqpto)) == NULL) continue;
      //copia dados de du.resumo para pr.resumo
      //pr.resumo->CopiaDe(du.resumo);
      //atualiza dados de perda de pr.resumo com a média ponderada do Resumo de dia úitl, sabado e domingo
      pr.resumo->perda.PerdaCabo        = ((dut.resumo->perda.PerdaCabo * num_dias.dut)  +
                                           (dom.resumo->perda.PerdaCabo * num_dias.dom)  +
                                           (sab.resumo->perda.PerdaCabo * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->perda.PerdaTrafoCobre  = ((dut.resumo->perda.PerdaTrafoCobre * num_dias.dut) +
                                           (dom.resumo->perda.PerdaTrafoCobre * num_dias.dom) +
                                           (sab.resumo->perda.PerdaTrafoCobre * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->perda.PerdaTrafoFerro  = ((dut.resumo->perda.PerdaTrafoFerro * num_dias.dut)  +
                                           (dom.resumo->perda.PerdaTrafoFerro * num_dias.dom)  +
                                           (sab.resumo->perda.PerdaTrafoFerro * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->perda.PerdaCapacitor   = ((dut.resumo->perda.PerdaCapacitor * num_dias.dut)  +
                                           (dom.resumo->perda.PerdaCapacitor * num_dias.dom)  +
                                           (sab.resumo->perda.PerdaCapacitor * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->perda.PerdaReator      = ((dut.resumo->perda.PerdaReator * num_dias.dut)  +
                                           (dom.resumo->perda.PerdaReator * num_dias.dom)  +
                                           (sab.resumo->perda.PerdaReator * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->perda.PerdaRegulador   = ((dut.resumo->perda.PerdaRegulador * num_dias.dut)  +
                                           (dom.resumo->perda.PerdaRegulador * num_dias.dom)  +
                                           (sab.resumo->perda.PerdaRegulador * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->perda.PerdaMedidor     = ((dut.resumo->perda.PerdaMedidor * num_dias.dut)  +
                                           (dom.resumo->perda.PerdaMedidor * num_dias.dom)  +
                                           (sab.resumo->perda.PerdaMedidor * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->perda.PerdaRamal       = ((dut.resumo->perda.PerdaRamal * num_dias.dut)  +
                                           (dom.resumo->perda.PerdaRamal * num_dias.dom)  +
                                           (sab.resumo->perda.PerdaRamal * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->perda.PerdaOutros      = ((dut.resumo->perda.PerdaOutros * num_dias.dut)  +
                                           (dom.resumo->perda.PerdaOutros * num_dias.dom)  +
                                           (sab.resumo->perda.PerdaOutros * num_dias.sab)) / num_dias.mes;
      //atualiza dados de balanço de pr.resumo com a média ponderada do Resumo de dia útil, sabado e domingo
      pr.resumo->balanco.EnergiaEntrada     = ((dut.resumo->balanco.EnergiaEntrada * num_dias.dut)  +
                                               (dom.resumo->balanco.EnergiaEntrada * num_dias.dom)  +
                                               (sab.resumo->balanco.EnergiaEntrada * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->balanco.EnergiaSuprimento  = ((dut.resumo->balanco.EnergiaSuprimento * num_dias.dut)  +
                                               (dom.resumo->balanco.EnergiaSuprimento * num_dias.dom)  +
                                               (sab.resumo->balanco.EnergiaSuprimento * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->balanco.EnergiaGerador     = ((dut.resumo->balanco.EnergiaGerador * num_dias.dut)  +
                                               (dom.resumo->balanco.EnergiaGerador * num_dias.dom)  +
                                               (sab.resumo->balanco.EnergiaGerador * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->balanco.EnergiaRecebidaExt = ((dut.resumo->balanco.EnergiaRecebidaExt * num_dias.dut)  +
                                               (dom.resumo->balanco.EnergiaRecebidaExt * num_dias.dom)  +
                                               (sab.resumo->balanco.EnergiaRecebidaExt * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->balanco.EnergiaRecebidaInt = ((dut.resumo->balanco.EnergiaRecebidaInt * num_dias.dut)  +
                                               (dom.resumo->balanco.EnergiaRecebidaInt * num_dias.dom)  +
                                               (sab.resumo->balanco.EnergiaRecebidaInt * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->balanco.EnergiaEntregueExt = ((dut.resumo->balanco.EnergiaEntregueExt * num_dias.dut)  +
                                               (dom.resumo->balanco.EnergiaEntregueExt * num_dias.dom)  +
                                               (sab.resumo->balanco.EnergiaEntregueExt * num_dias.sab)) / num_dias.mes;
      pr.resumo->balanco.EnergiaEntregueInt = ((dut.resumo->balanco.EnergiaEntregueInt * num_dias.dut)  +
                                               (dom.resumo->balanco.EnergiaEntregueInt * num_dias.dom)  +
                                               (sab.resumo->balanco.EnergiaEntregueInt * num_dias.sab)) / num_dias.mes;
      //
      pr.resumo->balanco.EnergiaCarga       = ((dut.resumo->balanco.EnergiaCarga * num_dias.dut)  +
                                               (dom.resumo->balanco.EnergiaCarga * num_dias.dom)  +
											   (sab.resumo->balanco.EnergiaCarga * num_dias.sab)) / num_dias.mes;
	  //
      pr.resumo->balanco.EnergiaPerdaFluxo  = ((dut.resumo->balanco.EnergiaPerdaFluxo * num_dias.dut)  +
                                               (dom.resumo->balanco.EnergiaPerdaFluxo * num_dias.dom)  +
                                               (sab.resumo->balanco.EnergiaPerdaFluxo * num_dias.sab)) / num_dias.mes;
      //
	  pr.resumo->balanco.EnergiaPerdaTotal  = ((dut.resumo->balanco.EnergiaPerdaTotal * num_dias.dut)  +
											   (dom.resumo->balanco.EnergiaPerdaTotal * num_dias.dom)  +
											   (sab.resumo->balanco.EnergiaPerdaTotal * num_dias.sab)) / num_dias.mes;
	  //atualiza dados de energia passante de pr.resumo com a média ponderada do Resumo de dia útil, sabado e domingo
	  pr.resumo->energiaPassante.A1_A3  = 		((dut.resumo->energiaPassante.A1_A3 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A1_A3 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A1_A3 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A1_A2  = 		((dut.resumo->energiaPassante.A1_A2 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A1_A2 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A1_A2 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A1_A3A  = 		((dut.resumo->energiaPassante.A1_A3A * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A1_A3A * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A1_A3A * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A1_A4  = 		((dut.resumo->energiaPassante.A1_A4 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A1_A4 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A1_A4 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A2_A3  = 		((dut.resumo->energiaPassante.A2_A3 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A2_A3 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A2_A3 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A2_A3A  = 		((dut.resumo->energiaPassante.A2_A3A * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A2_A3A * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A2_A3A * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A2_A4  = 		((dut.resumo->energiaPassante.A2_A4 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A2_A4 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A2_A4 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A3_A2  = 		((dut.resumo->energiaPassante.A3_A2 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A3_A2 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A3_A2 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A3_A3A  = 		((dut.resumo->energiaPassante.A3_A3A * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A3_A3A * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A3_A3A * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A3_A4 = 		((dut.resumo->energiaPassante.A3_A4 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A3_A4 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A3_A4 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A3A_A2 = 		((dut.resumo->energiaPassante.A3A_A2 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A3A_A2 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A3A_A2 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A3A_A4 = 		((dut.resumo->energiaPassante.A3A_A4 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A3A_A4 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A3A_A4 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A3A_B = 		((dut.resumo->energiaPassante.A3A_B * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A3A_B * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A3A_B * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A4_A2 = 		((dut.resumo->energiaPassante.A4_A2 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A4_A2 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A4_A2 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A4_A3 = 		((dut.resumo->energiaPassante.A4_A3 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A4_A3 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A4_A3 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A4_A3A = 		((dut.resumo->energiaPassante.A4_A3A * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A4_A3A * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A4_A3A * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A4_A4 = 		((dut.resumo->energiaPassante.A4_A4 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A4_A4 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A4_A4 * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.A4_B = 		((dut.resumo->energiaPassante.A4_B * num_dias.dut)  +
											   (dom.resumo->energiaPassante.A4_B * num_dias.dom)  +
											   (sab.resumo->energiaPassante.A4_B * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.B_A3A = 		((dut.resumo->energiaPassante.B_A3A * num_dias.dut)  +
											   (dom.resumo->energiaPassante.B_A3A * num_dias.dom)  +
											   (sab.resumo->energiaPassante.B_A3A * num_dias.sab)) / num_dias.mes;
	  pr.resumo->energiaPassante.B_A4 = 		((dut.resumo->energiaPassante.B_A4 * num_dias.dut)  +
											   (dom.resumo->energiaPassante.B_A4 * num_dias.dom)  +
											   (sab.resumo->energiaPassante.B_A4 * num_dias.sab)) / num_dias.mes;
	  }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TPerdareg::PreparaRede(void)
   {
   //variáveis locais
   bool           sucesso     = true;
   VTGeral       *geral       = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTMontaRedePT *montaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));

   //verifica se houve alteração em CfgPerdaTec
   if (geral->CfgPerdaTec->Alterada)
      {//indica que os parâmetros já foram usados
      geral->CfgPerdaTec->Alterada = false;
      //insere/retira Eqptos na Rede para cálculo de perdas
      sucesso = montaRedePT->Executa();
      }
   return(sucesso);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TPerdareg::PreparaRede(TDateTime data)
   {
   //variáveis locais
   bool           sucesso     = true;
   VTCurvas      *curvas      = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   VTDemanda     *demanda     = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTGeral       *geral       = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTMontaRedePT *montaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));

   //define mês de referência de Curvas (para corrigir fator de correção de energia)
   curvas->DataReferencia = data;
   //define mês de referência da Demanda (para determinar dias do mês no cálculo da demanda média)
   demanda->DataReferencia = data;
   //insere Eqptos na Rede para cálculo de perdas
   sucesso = montaRedePT->Executa();
   //indica que os parâmetros já foram usados
   geral->CfgPerdaTec->Alterada = false;

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdareg::SalvaPerdaTecnicaNaMedicao(void)
   {
   //variáveis locais
   double     perda_mwh;
   TMedicao   *medicao;
   VTRede     *rede;
   VTResumo   *resumo;
   VTMedicoes *medicoes   = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   VTResumos  *resumos    = (VTResumos*)apl->GetObject(__classid(VTResumos));
   TList      *lisMEDICAO  = medicoes->LisMedicao();

   //procura Medidor em lisMEDIDOR
   for (int n = 0; n < lisMEDICAO->Count; n++)
      {
      medicao = (TMedicao*)lisMEDICAO->Items[n];
      //totaliza perdas da Rede e de suas redes a jusante
      perda_mwh = 0;
      //DVK 2016.01.29 inclui perda própria
      if ((resumo = resumos->ExisteResumo(medicao->rede)) == NULL) continue;
      perda_mwh += resumo->balanco.EnergiaPerdaTotal;
      for (int n = 0; n < medicao->lisREDE_AJUS->Count; n++)
         {
		 rede = (VTRede*)medicao->lisREDE_AJUS->Items[n];
		 //correcao para não contabilizar 2x
		 if (rede == medicao->rede)
			{continue;}
         //determina Resumo da Rede
         if ((resumo = resumos->ExisteResumo(rede)) == NULL) continue;
         //salva perda técnica em Medicao
         perda_mwh += resumo->balanco.EnergiaPerdaTotal;
         }
      for (int n = 0; n < medicao->lisREDE_FIXA->Count; n++)
         {
		 rede = (VTRede*)medicao->lisREDE_FIXA->Items[n];
         //correcao para não contabilizar 2x
		 if (rede == medicao->rede)
			{continue;}
         //determina Resumo da Rede
         if ((resumo = resumos->ExisteResumo(rede)) == NULL) continue;
         //salva perda técnica em Medicao
         perda_mwh += resumo->balanco.EnergiaPerdaTotal;
         }
      //salva perda em Medicao
	  medicao->perda.tecnica_mwhmes = perda_mwh * num_dias.mes;
      }
   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TPerdareg::SelecionaCurvaTipica(int dia)
   {
   //variáveis locais
   VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   VTRedes  *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   curvas->DiaMedicao = dia;
   //reconfigura todas as cargas, associando-as com as curvas típicas selecionadas
   redes->RedefineCurvaTipicaCarga(curvas->DiaMedicao);

   return(true);
   }
 */

//---------------------------------------------------------------------------
//eof
