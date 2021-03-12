//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <DateUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Arranjo\VTSuporte.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Classes\VTClasses.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Importa\Monta\VTMonta.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTCluster.h>
#include <Fontes\Rede\VTDominio.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Rede\VTTipoChave.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTrafo.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Classes.h>
#include <DLL_Inc\Rede.h>
#include "TPreMonta.h"
#include "TSe.h"

//---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta(VTApl *apl)
   {
   try{//cria objeto TPreMonta
      return(new TPreMonta(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta::TPreMonta(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   arranjos  = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
	curvas    = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   monta     = (VTMonta*)apl->GetObject(__classid(VTMonta));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
	tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
	//cria listas
	lisSE = new TList();
	//inicia dados
	sucesso        = true;
	data           = DateOf(Now());
	DefineFormatoCampoAcoplamento(2);
	DefineSubestacaoComPrimarios(true);
	}

//---------------------------------------------------------------------------
__fastcall TPreMonta::~TPreMonta(void)
	{
	//destrói lista e seus objetos
	if (lisSE) {LimpaTList(lisSE); delete lisSE; lisSE = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TPreMonta::AtualizaCargaEquivalente(VTCarga *carga_eqv, VTCarga *carga)
   {
   //variáveis locais
   complex<double> s_mva;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTDemanda   *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //copia informações do esquemático
   carga_eqv->esq = carga->esq;
   //incorpora demanda da Carga na demanda da Carga equivalente
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {//determina demanda da Carga equivalente da Et no patamar
		s_mva  = demanda->S_mva[carga_eqv][np];
      //soma demanda da Carga no patamar
      s_mva += demanda->S_mva[carga][np];
      //atualiza demanda da Carga equivalente da Et no patamar
      demanda->S_mva[carga_eqv][np] = s_mva;
      }
   }

//---------------------------------------------------------------------------
double __fastcall TPreMonta::CalculaPotenciaLampada(double energia_mwhmes, VTCurva *curva)
	{
	//variáveis locais
	double    energia_kwh;
	double    num_horas;
	double    p_kw;
	VTDemanda *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

	//calcula energia diária em kWh
	energia_kwh = (energia_mwhmes * 1000.) / demanda->DiasMes;
	//determina número de horas em que a lâmpada permanece acesa
	num_horas = curva->Integral(0);
	if (IsDoubleZero(num_horas)) p_kw = 0.;
	else                         p_kw = energia_kwh / num_horas;
	//elimina quarta casa decimal em diante
	return((int(p_kw * 1000)) * 0.001);
	}

//---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::CriaCargaEquivalente(int barra_id, AnsiString codigo, int fases_tag)
   {
   //variáveis locais
   strCARGA     str_carga;
   VTCarga     *carga;
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//cria Carga equivalente com fases iguais às da rede secundária e demanda nula
      str_carga.car_id       = FalsoId();
      str_carga.bar_id       = barra_id;
      str_carga.codigo       = codigo;
      str_carga.extern_id    = "";
      str_carga.classe_cons  = consEQUIVALENTE;
      str_carga.num_cons.comercial   = 0;
      str_carga.num_cons.residencial = 0;
      str_carga.num_cons.industrial  = 0;
      str_carga.num_cons.rural       = 0;
      str_carga.num_cons.outros      = 0;
      str_carga.num_cons.a4          = 0;
      str_carga.modelo.icte_pu = 1;
      str_carga.modelo.scte_pu = 0;
      str_carga.modelo.zcte_pu = 0;
      str_carga.fases          = fases_tag & faseABCN;
      str_carga.energia_mwhmes = 0;
      //cancela dados de IP
      str_carga.ip.num_lamp =  0.;
      str_carga.ip.pot_kw   =  0.;
      str_carga.ip.fat_pot  =  1.;
      //define curva própria
      str_carga.curva.id        = -1;
      str_carga.curva.tipica    = false;
		str_carga.curva.num_pat   = patamares->NumPatamar();
      str_carga.curva.num_valor = nvCURVA_PQ;
      //define curva de demanda própria nula
      for (int np = 0; np < patamares->NumPatamar(); np++)
			{//define hora inicial e final do patamar
         patamar = patamares->Patamar[np];
         str_carga.curva.patamar[np].hm_ini.hora   = patamar->HoraIni;
         str_carga.curva.patamar[np].hm_ini.minuto = patamar->MinutoIni;
         str_carga.curva.patamar[np].hm_fim.hora   = patamar->HoraFim;
			str_carga.curva.patamar[np].hm_fim.minuto = patamar->MinutoFim;
         str_carga.curva.patamar[np].valor[indP]   = 0;
         str_carga.curva.patamar[np].valor[indQ]   = 0;
         }
      //define posição da carga na Barra
		str_carga.esq.posbar  = 50.;
		str_carga.esq.dxbar   = DXBAR;
		str_carga.esq.dybar   = DYBAR;
      //insere Carga em VTMonta
      carga = ((VTCarga*)(monta->InsereCarga(str_carga)));
      }catch(Exception &e)
         {
         carga = NULL;
         }
   return(carga);
	}

//-----------------------------------------------------------------------------
void __fastcall TPreMonta::DefineDataReferencia(TDateTime data)
	{
	this->data = data;
	}

//-----------------------------------------------------------------------------
void __fastcall TPreMonta::DefineFormatoCampoAcoplamento(int opcao)
	{
	//salva formato do campo "código de acoplamento"
	formato_acoplamento = opcao;
	}

//-----------------------------------------------------------------------------
void __fastcall TPreMonta::DefineSubestacaoComPrimarios(bool enabled)
	{
	subestacao_integrada = enabled;
   }

//---------------------------------------------------------------------------
VTDominio* __fastcall TPreMonta::ExisteDominio(void)
	{
   //variáveis locais
   TList *lisDOMINIO = redes->LisDominio();
   if (lisDOMINIO->Count > 0)
      {
      return((VTDominio*)(lisDOMINIO->First()));
      }
   return(NULL);
	}

//-----------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::ExisteInsereBarra(AnsiString codigo)
	{
	//variáveis locais
	VTBarra *barra;

	try{//verifica se a Barra já existe
		if ((barra = monta->ExisteBarra(codigo)) == NULL)
			{//insere uma nova Barra
			barra = InsereBarra(codigo);
			}
		}catch (Exception &e)
			{
			//if (plog) plog->Add("Erro na criação do ponto de acomplamento " + AnsiQuotedStr(codigo, '\''));
			barra = NULL;;
			}
	return(barra);
	}

//-----------------------------------------------------------------------------
VTRede* __fastcall TPreMonta::ExisteInsereRedeBT(AnsiString codigo, VTBarra *barra_ini)
	{
	//variáveis locais
	VTRede *rede;

	try{//verifica se a Barra já existe
		if ((rede = ExisteRede(codigo)) == NULL)
			{//insere uma nova Rede
			rede = InsereRede(codigo, redeSEC, NULL);
			}
		}catch (Exception &e)
			{
			//if (plog) plog->Add("Erro na criação do ponto de acomplamento " + AnsiQuotedStr(codigo, '\''));
			rede = NULL;;
			}
	return(rede);
	}

//-----------------------------------------------------------------------------
TSe* __fastcall TPreMonta::ExisteInsereSe(sdi_TRAFO_AT_MT &sdi)
	{
	//variáveis locais
	TSe     *se;
	VTBarra *barra_at, *barra_mt;
	VTRede  *redeSE;

	try{//verifica se existe a rede em lisETD
		if ((se = ExisteSe(sdi.subestacao)) == NULL)
			{//cria um novo objeto Se e insere em lisSE
			se = NewObjSe(sdi.subestacao);
			lisSE->Add(se);
			//cria Barra AT
			se->InsereBarraAT(barra_at = InsereBarra("Bar" + DoubleToStr("%2.1f", sdi.vnom_pri_kv)));
			barra_at->vnom = sdi.vnom_pri_kv;
			//cria Barra MT
			se->InsereBarraMT(barra_mt = InsereBarra("Bar" + DoubleToStr("%2.1f", sdi.vnom_sec_kv)));
			barra_mt->vnom = sdi.vnom_sec_kv;
			//cria nova Rede
			se->InsereRedeSE(redeSE = InsereRede(sdi.subestacao, redeETD, barra_at));
			}
		else
			{//verifica se existe a Barra MT
			if ((barra_mt = se->ExisteBarraMT(sdi.vnom_sec_kv)) == NULL)
				{//cria Barra MT
				se->InsereBarraMT(barra_mt = InsereBarra("Bar" + DoubleToStr("%2.1f", sdi.vnom_sec_kv)));
				barra_mt->vnom = sdi.vnom_sec_kv;
				}
			}
		}catch (Exception &e)
			{
			if (plog) plog->Add("Erro na criação da subestação " + AnsiQuotedStr(sdi.codigo, '\''));
			se = NULL;;
			}
	return(se);
	}

//-----------------------------------------------------------------------------
VTRede* __fastcall TPreMonta::ExisteRede(AnsiString codigo)
	{
	//verifica se é a última rede referenciada
	if ((ult_rede != NULL) && (ult_rede->Codigo.AnsiCompareIC(codigo) == 0))return (ult_rede);
	ult_rede = redes->ExisteRede(codigo);

	return(ult_rede);
	}

//-----------------------------------------------------------------------------
TSe* __fastcall TPreMonta::ExisteSe(AnsiString codigo)
	{
	//variáveis locais
	TSe *se;

	for (int n = 0; n < lisSE->Count; n++)
		{
		se = (TSe*)lisSE->Items[n];
		if (se->codigo.AnsiCompareIC(codigo) == 0) return(se);
		}
	return(NULL);
	}

//-----------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::InsereBarra(AnsiString codigo)
	{
	//variáveis locais
	int      ind_ini, ind_fim;
	strBARRA str_barra;
	VTBarra *barra;

	try{//preenche estrutura strBARRA
		str_barra.bar_id  = FalsoId();
		str_barra.codigo  = codigo;
		str_barra.vnom    = 0.;
		str_barra.utm.x   = 0;
		str_barra.utm.y   = 0;
		//verifica se o código da Barra contém suas coordenadas
		if (codigo.Length() >= 17)
			{
			switch (formato_acoplamento)
				{
				case 0: //sem informação de coordenada
					break;
				case 1: //coordX com 8 dígitos e coordY 9 dígitos
					str_barra.utm.x = StrToInt(codigo.SubString(1, 8));
					str_barra.utm.y = StrToInt(codigo.SubString(9, 9));
					break;
				case 2: //coordX_coordY_sequencial
					ind_ini = 1;
					if ((ind_fim = codigo.AnsiPos("_")) > 0)
						{
						str_barra.utm.x = StrToInt(codigo.SubString(ind_ini, ind_fim-ind_ini));
						codigo[ind_fim] = '.';
						//determina coord.y
						ind_ini = ind_fim + 1;
						if ((ind_fim = codigo.AnsiPos("_")) > 0)
							{
							str_barra.utm.y = StrToInt(codigo.SubString(ind_ini, ind_fim-ind_ini));
							}
						}
					break;
				}
			}
		str_barra.esq.x1  = str_barra.esq.x2 = str_barra.utm.x;
		str_barra.esq.y1  = str_barra.esq.y2 = str_barra.utm.y;
		//insere Barra
		barra = (VTBarra*)monta->InsereBarraEqpto(str_barra);
		}catch (Exception &e)
			{
			barra = NULL;
			}
	//verifica se a Barra foi criada
	if (barra == NULL)
		{
		if (plog) plog->Add("Erro na criação do ponto de acomplamento " + AnsiQuotedStr(codigo, '\''));
		}
	return(barra);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCargaBT(sdi_CARGA_BT &sdi)
   {
	//variáveis locais
	int         ind_mes;
   int         fases_tag;
	strCARGA    str_carga;
	VTBarra    *barra;
	VTCarga    *carga;
	VTCurva    *curva;

	//cofigura Log
	if (plog) plog->DefineValor("Eqpto", "CARGA_BT");
	try{//verifica se as fases estão definidas
		if (sdi.fases.IsEmpty())
			{//assume carga trifásica
			if (plog) plog->Add("Carga BT sem fases definidas");
			fases_tag = faseABCN;
			}
		else
			{//determina fases, filtrando terra
			fases_tag = fases->ExisteFase(sdi.fases) & faseABCN;
			}
		//determina Barra da Carga
		if ((barra  = monta->ExisteBarra(sdi.codigo_pto)) == NULL)
			{
			if (plog) plog->Add("Carga BT: não existe ponto de acomplamento " + AnsiQuotedStr(sdi.codigo_pto, '\''));
			//cria uma nova Barra, sem coordenadas
			if ((barra = InsereBarra(sdi.codigo_pto)) == NULL) return(true);
			}
		//determina curva típica da Carga
		if ((curva = curvas->ExisteCurvaTipicaCarga(sdi.tipologia_curva)) == NULL)
			{//procura Curva de IP
			if ((curva = curvas->ExisteCurvaTipicaIP(sdi.tipologia_curva)) == NULL)
				{
				if (plog) plog->Add("Carga BT: não existe tipologia da curva de carga " + AnsiQuotedStr(sdi.tipologia_curva, '\''));
				return(true);
				}
			}
		//preenche estrutura strCARGA
		str_carga.car_id       = FalsoId();
		str_carga.bar_id       = barra->Id;
		str_carga.codigo       = sdi.codigo;
      str_carga.extern_id    = "";
		//define classe de consumidor/energia
		str_carga.classe_cons    = curva->Classe->Tag;
		str_carga.modelo.icte_pu = 1.;
		str_carga.modelo.scte_pu = 0.;
		str_carga.modelo.zcte_pu = 0.;
		str_carga.fases          = fases_tag;
		//define consumo em função do mês da data dos dados
		ind_mes = MonthOf(data) - 1;
		str_carga.energia_mwhmes = sdi.energia_mwhmes[ind_mes];
      //cancela vetor de demanda p/ curva própria
		str_carga.curva.id      = curva->Id;
      str_carga.curva.tipica  = true;
		str_carga.curva.num_pat = 0;
		//verifica se é uma Carga de IP
		if (curva->Classe->Tag == consIP)
			{//calcula potência da lâmpada
			str_carga.ip.num_lamp = 1;
			str_carga.ip.pot_kw   = CalculaPotenciaLampada(str_carga.energia_mwhmes, curva);
			//assume valor fixo
			str_carga.ip.fat_pot  = 0.92;
			}
		//esquemático
		str_carga.esq.posbar = 50.;
		str_carga.esq.dxbar  =  0;
		str_carga.esq.dybar  = DYBAR;
		//insere Carga em VTMonta
      carga = (VTCarga*)monta->InsereCarga(str_carga);
      }catch (Exception &e)
         {
         carga = NULL;
         }
   return(carga);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCargaMT(sdi_CARGA_MT &sdi)
   {
	//variáveis locais
	int         ind_mes;
   int         fases_tag;
	strCARGA    str_carga;
	VTBarra    *barra;
	VTCarga    *carga;
	VTCurva    *curva;
	VTRede     *redeMT;

	//cofigura Log
	if (plog) plog->DefineValor("Eqpto", "CARGA_MT");
	try{//verifica se as fases estão definidas
		if (sdi.fases.IsEmpty())
			{//assume carga trifásica
			if (plog) plog->Add("Carga MT sem fases definidas");
			fases_tag = faseABCN;
			}
		else
			{//determina fases, filtrando terra
			fases_tag = fases->ExisteFase(sdi.fases) & faseABCN;
			}
		//verifica se existe a Rede MT
		if ((redeMT = ExisteRede(sdi.circuito)) == NULL)
			{
			if (plog) plog->Add("Não está associada a um circuito");
			return(true);
			}
		//determina Barra da Carga
		if ((barra  = monta->ExisteBarra(sdi.codigo_pto)) == NULL)
			{
			if (plog) plog->Add("Carga MT: não existe ponto de acomplamento " + AnsiQuotedStr(sdi.codigo_pto, '\''));
			//cria uma nova Barra, sem coordenadas
			if ((barra = InsereBarra(sdi.codigo_pto)) == NULL) return(true);
			}
		//determina curva típica da Carga
		if ((curva = curvas->ExisteCurvaTipicaCarga(sdi.tipologia_curva)) == NULL)
			{
			if (plog) plog->Add("Carga MT: não existe tipologia da curva de carga " + AnsiQuotedStr(sdi.tipologia_curva, '\''));
			return(true);
			}
		//preenche estrutura strCARGA
		str_carga.car_id       = FalsoId();
		str_carga.bar_id       = barra->Id;
		str_carga.codigo       = sdi.codigo;
      str_carga.extern_id    = "";
		//define classe de consumidor/energia
		str_carga.classe_cons    = curva->Classe->Tag;
		str_carga.modelo.icte_pu = 1.;
		str_carga.modelo.scte_pu = 0.;
		str_carga.modelo.zcte_pu = 0.;
		str_carga.fases          = fases_tag;
		//define consumo em função do mês da data dos dados
		ind_mes = MonthOf(data) - 1;
		str_carga.energia_mwhmes = sdi.energia_mwhmes[ind_mes];
      //cancela vetor de demanda p/ curva própria
		str_carga.curva.id      = curva->Id;
      str_carga.curva.tipica  = true;
      str_carga.curva.num_pat = 0;
      //cancela dados de IP
      str_carga.ip.num_lamp = 0.;
      str_carga.ip.pot_kw   = 0.;
		str_carga.ip.fat_pot  = 1.;
		//esquemático
		str_carga.esq.posbar = 50.;
		str_carga.esq.dxbar  =  0;
		str_carga.esq.dybar  = DYBAR;
		//insere Carga em VTMonta
      carga = (VTCarga*)monta->InsereCarga(str_carga);
      }catch (Exception &e)
         {
         carga = NULL;
         }
   return(carga);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereChaveBT(sdi_CHAVE_BT &sdi)
   {
	//variáveis locais
	int         fases_tag;
	strCHAVE    str_chave;
	VTBarra     *barra1, *barra2;
	VTRede      *redeBT;
	VTLigacao   *ligacao;

   //configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Chave BT");
	try{//verifica se as fases estão definidas
		if (sdi.fases.IsEmpty())
			{//assume carga trifásica
			if (plog) plog->Add("Fases não definidas");
			return(true);
			}
		//converte fases
		fases_tag = fases->ExisteFase(sdi.fases);
		//verifica se existe a Rede
		if ((redeBT = ExisteRede(sdi.codigo_rede)) == NULL)
			{
			if (plog) plog->Add("Não está associado a uma rede");
			return(true);
			}
		//determina Barras
		if ((barra1 = ExisteInsereBarra(sdi.codigo_pto1)) == NULL) return(true);
		if ((barra2 = ExisteInsereBarra(sdi.codigo_pto2)) == NULL) return(true);
		//verifica se as Barras são iguais
		if (barra1 == barra2)
			{
			if (plog) plog->Add("Pontos de acomplamento iguais");
			return(true);
			}
		//verifica se existe um trecho associado
		if ((ligacao = redeBT->ExisteLigacao(eqptoTRECHO, sdi.codigo_segmento)) != NULL)
			{//verifica se o Trecho conecta as mesmas Barras
			if ((ligacao->pbarra1 == barra1)&&(ligacao->pbarra2 == barra2))
				{//elimina Trecho da rede
				redeBT->RemoveLigacao(ligacao);
				DLL_DeleteEqpto(ligacao);
				}
			if ((ligacao->pbarra1 == barra2)&&(ligacao->pbarra2 == barra1))
				{//elimina Trecho da rede
				redeBT->RemoveLigacao(ligacao);
				DLL_DeleteEqpto(ligacao);
				}
			else
				{
				if (plog) plog->Add("Chave BT associada a segmento com acomplamento diferente");
				}
			}
		//preenche estrutura strCHAVE
		str_chave.rede_id           = redeBT->Id;
		str_chave.chv_id            = FalsoId();
		str_chave.codigo            = sdi.codigo_segmento;
		str_chave.bar1_id           = barra1->Id;
		str_chave.bar2_id           = barra2->Id;
		str_chave.tipochave.id      = ID_NULO;
		str_chave.tipochave.tipo    = chaveINDEFINIDA;
		str_chave.tipochave.codigo  = "";
		str_chave.estado            = (sdi.estado == 1) ? chvABERTA: chvFECHADA;
		str_chave.corn              = 200;
		//str_chave.protecao_ativa    = false;
		//str_chave.telecomandada     = false;
		//str_chave.tecno_comunicacao = comNAO_DISPONIVEL;
		//define parâmetros para esquemático
		str_chave.esq.posbar1    = 50.;
		str_chave.esq.posbar2    = 50.;
		//insere Chave em VTMonta
		if ((monta->InsereChave(str_chave))== NULL) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na inserção de chave BT");
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereChaveMT(sdi_CHAVE_MT &sdi)
   {
	//variáveis locais
	int         fases_tag;
	strCHAVE    str_chave;
	VTBarra     *barra1, *barra2;
	VTRede      *redeMT;
	VTLigacao   *ligacao;

   //configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Chave MT");
	try{//verifica se as fases estão definidas
		if (sdi.fases.IsEmpty())
			{//assume carga trifásica
			if (plog) plog->Add("Fases não definidas");
			return(true);
			}
		//converte fases
		fases_tag = fases->ExisteFase(sdi.fases);
		//verifica se existe a Rede
		if ((redeMT = ExisteRede(sdi.circuito)) == NULL)
			{
			if (plog) plog->Add("Não está associado a uma rede");
			return(true);
			}
		//determina Barras
		if ((barra1 = ExisteInsereBarra(sdi.codigo_pto1)) == NULL) return(true);
		if ((barra2 = ExisteInsereBarra(sdi.codigo_pto2)) == NULL) return(true);
		//verifica se as Barras são iguais
		if (barra1 == barra2)
			{
			if (plog) plog->Add("Pontos de acomplamento iguais");
			return(true);
			}
		//verifica se existe um trecho associado
		if (! sdi.codigo_segmento.IsEmpty())
			{
			if ((ligacao = redeMT->ExisteLigacao(eqptoTRECHO, sdi.codigo_segmento)) != NULL)
				{//verifica se o Trecho conecta as mesmas Barras
				if ((ligacao->pbarra1 == barra1)&&(ligacao->pbarra2 == barra2))
					{//elimina Trecho da rede
					redeMT->RemoveLigacao(ligacao);
					DLL_DeleteEqpto(ligacao);
					}
				if ((ligacao->pbarra1 == barra2)&&(ligacao->pbarra2 == barra1))
					{//elimina Trecho da rede
					redeMT->RemoveLigacao(ligacao);
					DLL_DeleteEqpto(ligacao);
					}
				else
					{
					if (plog) plog->Add("Chave MT associada a segmento com acomplamento diferente");
					}
				}
			}
		//preenche estrutura strCHAVE
		str_chave.rede_id           = redeMT->Id;
		str_chave.chv_id            = FalsoId();
		str_chave.codigo            = sdi.codigo_segmento;
		str_chave.bar1_id           = barra1->Id;
		str_chave.bar2_id           = barra2->Id;
		str_chave.tipochave.id      = ID_NULO;
		str_chave.tipochave.tipo    = chaveINDEFINIDA;
		str_chave.tipochave.codigo  = "";
		str_chave.estado            = (sdi.estado == 1) ? chvABERTA: chvFECHADA;
		str_chave.corn              = 200;
		//str_chave.protecao_ativa    = false;
		//str_chave.telecomandada     = false;
		//str_chave.tecno_comunicacao = comNAO_DISPONIVEL;
		//define parâmetros para esquemático
		str_chave.esq.posbar1    = 50.;
		str_chave.esq.posbar2    = 50.;
		//insere Chave em VTMonta
		if ((monta->InsereChave(str_chave))== NULL) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na inserção de chave MT");
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCircuito(sdi_CIRCUITO &sdi)
	{
	//variáveis locais
	strREDE      str_rede;
	TSe          *se;
	VTBarra      *barra;
	VTRede       *redeMT;
	VTSuprimento *suprimento;

	//configura Log
	if (plog) plog->DefineValor("Eqpto", "Circuito");
	try{//insere Rede em VTMonta
		if (plog) plog->DefineValor("Rede", sdi.codigo);
		//determina Barra inicial
		if ((barra = ExisteInsereBarra(sdi.codigo_pto)) == NULL) return(false);
		//define tensão da Barra inicial
		barra->vnom = sdi.vnom_kv;
		//insere Suprimento
		if (InsereSuprimento(sdi.codigo, barra, sdi.vope_pu) == NULL) return(false);
		//insere a Rede
		if ((redeMT = InsereRede(sdi.codigo, redePRI, barra)) == NULL) return(false);
		//insere RedeMT na Se
		if ((se = ExisteSe(sdi.subestacao)) != NULL)
			{
			se->InsereRedeMT(redeMT);
			}
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na inserção de Circuito");
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCondutor(sdi_CONDUTOR &sdi)
	{
	//variáveis locais
	strARRANJO2 str_arranjo;

	//cofigura Log
	if (plog) plog->DefineValor("Eqpto", "Condutor");
	try{//preenche estrutura strARRANJO2
		str_arranjo.tipico     = true;
		str_arranjo.data       = DateOf(Now());
		str_arranjo.arranjo_id = FalsoId();
		str_arranjo.codigo     = sdi.codigo;
		str_arranjo.extern_id  = sdi.codigo;
		str_arranjo.tipo_cfg   = arranjoZ0Z1;
		str_arranjo.fases      = faseINV;
		str_arranjo.res_terra  = 100.;
		str_arranjo.vmin_kv    = 0;
		str_arranjo.vmax_kv    = 0;
		str_arranjo.color      = clBlue;
		str_arranjo.status     = sttNOVO;
		//verifica o tipo de arranjo
		str_arranjo.cfg_arranjoZ0Z1.iadm_amp   = sdi.i_max_amp;
		str_arranjo.cfg_arranjoZ0Z1.z1.r       = sdi.r_ohm_km;
		str_arranjo.cfg_arranjoZ0Z1.z1.x       = sdi.x_ohm_km;
		str_arranjo.cfg_arranjoZ0Z1.z0         = str_arranjo.cfg_arranjoZ0Z1.z1;
		str_arranjo.cfg_arranjoZ0Z1.c1         = 0.;
		str_arranjo.cfg_arranjoZ0Z1.c0         = 0.;
		str_arranjo.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
		if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z0))
			{
			if (plog) plog->Add("Impedância Z0 nula");
			//return(true);
			}
		if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z1))
			{
			if (plog) plog->Add("Impedância Z1 nula");
			//return(true);
			}
		//insere Arranjo em VTMonta
		if (monta->InsereArranjo(str_arranjo) == NULL) return(true);
		}catch (Exception &e)
			{
			if (plog) plog->Add("Erro na criação do condutor " + AnsiQuotedStr(sdi.codigo, '\''));
			return(false);
			}
	return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCoordenada(sdi_COORDENADA &sdi)
	{
	//variáveis locais
	strBARRA str_barra;
	VTBarra  *barra;

	//verifica se as coordenadas já foram definidas a partir do código da Barra
	if (formato_acoplamento > 0) return(true);
	//cofigura Log
	if (plog) plog->DefineValor("Eqpto", "COORDENADAS");
	try{//verifica se existe a Barra
		if ((barra  = monta->ExisteBarra(sdi.codbase)) == NULL)
			{
			if (plog) plog->Add("Não existe ponto de acomplamento " + AnsiQuotedStr(sdi.codigo, '\''));
			//cria uma nova Barra, sem coordenadas
			if ((barra = InsereBarra(sdi.codigo)) == NULL) return(true);
			}
		//atualiza coordenadas utm da Barra
		barra->Codigo = sdi.codigo;
		barra->utm.x  = int(sdi.utm_m.x * 100.); //m ==> cm
		barra->utm.y  = int(sdi.utm_m.y * 100.); //m ==> cm
		//atualiza coordenadas esquemático da Barra
		barra->esq.x1 = barra->esq.x2 = barra->utm.x;
		barra->esq.y1 = barra->esq.y2 = barra->utm.y;
		}catch (Exception &e)
         {
			if (plog) plog->Add("Erro na definição das coordenadas da barra " + AnsiQuotedStr(sdi.codigo, '\''));
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDominio(AnsiString codigo)
	{
	//variáveis locais
	AnsiString  txt;
	strDOMINIO str_dominio;

	//verifica se já foi criado um Dominio
	if (dominio != NULL) return(true);
	//verifica se já existe um Dominio
	if ((dominio = ExisteDominio()) != NULL) return(true);
	//insere um Dominio em VTMonta
	txt.sprintf("%s_%04d.%02d.%02d", codigo.c_str(), YearOf(data), MonthOf(data), DayOf(data));
	str_dominio.obra        = false;
	str_dominio.dominio_id  = FalsoId();
	str_dominio.codigo      = txt;
	str_dominio.data_criacao = data;
	//insere Dominio e salva ponteiro p/ objeto
	dominio = (VTDominio*)monta->InsereDominio(str_dominio);

	return(dominio != NULL);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereGeradorBT(sdi_GERADOR_BT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereGeradorMT(sdi_GERADOR_MT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereRamalBT(sdi_RAMAL_BT &sdi)
	{
	//variáveis locais
	int         fases_tag;
	strTRECHO   str_tre;
	VTArranjo   *arranjo;
	VTBarra     *barra1, *barra2;
	VTRede      *redeBT;

   //configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Ramal BT");
	try{//verifica se as fases estão definidas
		if (sdi.fases.IsEmpty())
			{//gera mensagem de log
			if (plog) plog->Add("Fases não definidas");
			return(true);
			}
		//converte fases
		fases_tag = fases->ExisteFase(sdi.fases);
		//verifica se existe a Rede
		if ((redeBT = ExisteRede(sdi.rede)) == NULL)
			{
			if (plog) plog->Add("Não está associado a uma rede");
			return(true);
			}
		//verifica se existe o Arranjo indicado
		if ((arranjo = arranjos->ExisteArranjo(sdi.codigo_condutor)) == NULL)
			{
			if (plog) plog->Add("Não existe o arranjo indicado");
			return(true);
			}
		//verifica se o Arranjo ainda não está configurado como ramal
		if (! arranjo->ramal)
			{//define Arranjo como Ramal
			arranjo->ramal = true;
			}
		//verifica se as fases do Arranjo ainda não estão definidas
		if (arranjo->Fases == faseINV)
			{//define fases do Arranjo iguais às fases do Trecho
			arranjo->Fases = fases_tag;
			}
		//verifica se as fases do Trecho são iguais às fases do Arranjo
		else if (arranjo->Fases != fases_tag)
			{
			if (plog) plog->Add("Ramal BT com fases inconsistentes com fases do condutor");
			return(true);
			}
		//determina Barras
		if ((barra1 = ExisteInsereBarra(sdi.codigo_pto1)) == NULL) return(true);
		if ((barra2 = ExisteInsereBarra(sdi.codigo_pto2)) == NULL) return(true);
		//verifica se as Barras são iguais
		if (barra1 == barra2)
			{
			if (plog) plog->Add("Pontos de acomplamento iguais");
			return(true);
			}
		//preenche estrutura do trecho
		str_tre.obra            = false;
		str_tre.tre_id         = FalsoId();
		str_tre.rede_id        = redeBT->Id;
		str_tre.bar1_id        = barra1->Id;
		str_tre.bar2_id        = barra2->Id;
		str_tre.codigo         = sdi.codigo;
      str_tre.extern_id      = "";
		str_tre.fases          = fases_tag;
		str_tre.comp_km        = sdi.comprimento_km;
		//define parâmetros para esquemático
		str_tre.esq.posbar1    = 50.;
		str_tre.esq.posbar2    = 50.;
		str_tre.comentario.comentario = "";
      str_tre.comentario.dx = 0;
      str_tre.comentario.dy = 0;
      //proteção
      if ((str_tre.comp_km < 0.001)||(str_tre.comp_km > 0.200))
         {//define comprimento de 10 metros
			if (plog) plog->Add("Segmento c/ comprimento = " + DoubleToStr("%4.3f km", sdi.comprimento_km));
         }
      //insere Trecho em VTMonta
      if (monta->InsereTrecho(str_tre, arranjo) == NULL) return(true);
      }catch(Exception &e)
         {
			if (plog) plog->Add("Erro na inserção de segmento BT");
			return(false);
         }
	return(true);
   }

//-----------------------------------------------------------------------------
VTRede* __fastcall TPreMonta::InsereRede(AnsiString codigo, int segmento, VTBarra *barra_ini)
	{
	//variáveis locais
	strREDE  str_rede;
	VTRede   *rede;

	//configura Log
	if (plog) plog->DefineValor("Eqpto", "Rede");
	if (plog) plog->DefineValor("Rede", codigo);
	try{//se ainda não existir, cria o Dominio
		if (dominio == NULL) InsereDominio(codigo_dominio);
		//insere Rede em VTMonta
		str_rede.rede_id            = FalsoId();
		str_rede.mrede_id           = ID_NULO;
		str_rede.cluster_id         = ID_NULO;
		str_rede.barini_id          = (barra_ini != NULL) ? barra_ini->Id : ID_NULO;
		str_rede.dominio_id         = (dominio != NULL) ? dominio->Id : ID_NULO;
		str_rede.codigo             = codigo;
		str_rede.tiporede.id        = ID_NULO;
		str_rede.tiporede.segmento  = segmento;
		str_rede.tiporede.codigo    = "";
		str_rede.equivalente        = false;
		switch(segmento)
			{
			case redeETD:
				str_rede.color    = clWhite;
				str_rede.estatica = true;
				str_rede.radial   = false;
				break;
			case redePRI:
				str_rede.color    = clBlue;
				str_rede.estatica = false;
				str_rede.radial   = true;
				break;
			case redeSEC:
				str_rede.color    = clRed;
				str_rede.estatica = true;
				str_rede.radial   = true;
				break;
			default:
				str_rede.color    = clYellow;
				str_rede.estatica = true;
				str_rede.radial   = false;
				break;
			}
		if ((rede = (VTRede*)monta->InsereRede(str_rede)) == NULL) return(false);
		//define Barra inicial
		if (barra_ini != NULL) rede->DefineBarraInicial(barra_ini);
		}catch(Exception &e)
			{//cancela última Subestacao criada
			rede = NULL;
			if (plog) plog->Add("Erro na criação da rede " + AnsiQuotedStr(codigo, '\''));
			}
	return(rede);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereReguladorMT(sdi_REGULADOR &sdi)
	{
	//variáveis locais
	int           fases_pri, fases_sec;
	double        r_pu, x_pu;
	strREGULADOR  str_regulador;
	VTBarra       *barra1, *barra2;
	VTRede        *redeMT;
	VTRegulador   *regulador;

	//configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Regulador");
	try{//verifica se as fases do primário e do secundário estão definidas
		if (sdi.fases_pri.IsEmpty())
			{
			if (plog) plog->Add("Fases do primário indefinidas");
			return(true);
			}
		if (sdi.fases_sec.IsEmpty())
			{
			if (plog) plog->Add("Fases do secundário indefinidas");
			return(true);
			}
		//verifica se fases diferentes
		if (fases_pri != fases_sec)
			{
			if (plog) plog->Add("Fases do primário diferentes das fases do secundário");
			}
		//verifica se existe a Rede MT
		if ((redeMT = ExisteRede(sdi.circuito)) == NULL)
			{
			if (plog) plog->Add("Não está associado a um circuito");
			return(true);
			}
		//verifica se existe o Regualdor
		if ((regulador = (VTRegulador*)redeMT->ExisteLigacao(eqptoREGULADOR, sdi.codigo)) != NULL)
			{//verifica se regulador com ligacao delta_aberto
			if (regulador->ligacao == lgDELTA_ABERTO)
				{//redefine fases: fase comum entre as duas unidades
				regulador->fases = regulador->fases & fases_pri;
				}
			else
				{//nada a fazer
            }
			return(true);
			}
		//determina Barras
		if ((barra1 = ExisteInsereBarra(sdi.codigo_pto1)) == NULL) return(true);
		if ((barra2 = ExisteInsereBarra(sdi.codigo_pto2)) == NULL) return(true);
		//verifica se as Barras são iguais
		if (barra1 == barra2)
			{
			if (plog) plog->Add("Pontos de acomplamento iguais");
			return(true);
			}
		//determina fases do primário e do secundário
		fases_pri = fases->ExisteFase(sdi.fases_pri);
		fases_sec = fases->ExisteFase(sdi.fases_sec);
		//calcula impedância em pu
		r_pu = sdi.r_perc   * 0.01;
		x_pu = sdi.xhl_perc * 0.01;
		//insere Regualdor
		str_regulador.id            = FalsoId();
		str_regulador.rede_id       = redeMT->Id;
		str_regulador.bar1_id       = barra1->Id;
		str_regulador.bar2_id       = barra2->Id;
		str_regulador.codigo        = sdi.codigo;
		str_regulador.extern_id     = "";
		str_regulador.z1.r          = r_pu;
		str_regulador.z1.x          = x_pu;
		str_regulador.z0            = str_regulador.z1;
		//define atributos não existentes no arquivo
		str_regulador.modelo        = reguPADRAO;
		str_regulador.by_pass       = false;
		str_regulador.var_tensao    = 10;
		str_regulador.num_passo     = 16;
		str_regulador.snom_mva      = (3. * sdi.snom_kva) / 1000.; //kVA =>MVA
		str_regulador.sentido.reversivel  = true;
		str_regulador.sentido.bar_ref_id  = barra1->Id;
		str_regulador.ajuste_auto         = false;
		str_regulador.cfg_auto.bar_ref_id = barra2->Id;
		str_regulador.cfg_auto.v_pu       = 1.0;
		str_regulador.cfg_fixo.passo      = (sdi.vreg_pu - 1.0) / (str_regulador.var_tensao / str_regulador.num_passo);
		//dados não utilizados
		//sdi.perda_vazio_perc;
		//sdi.perda_total_perc;
		//define tipo de ligacao e fases
		switch(sdi.tipo)
			{
			case 1:	//monofásico
				str_regulador.ligacao = lgMONOFASICO;
				str_regulador.fases   = fases_pri;
				break;
			case 2:	//delta aberto
				str_regulador.ligacao = lgDELTA_ABERTO;
				//assume as fases da unidade. A segunda unidade servirá p/ redefinir a fase comum
				str_regulador.fases   = fases_pri;
				break;
			case 3:	//trifásico
				str_regulador.ligacao = lgTRIANG;
				str_regulador.fases   = faseABC;
				break;
			case 4:	//delta fechado
				str_regulador.ligacao = lgDELTA_FECHADO;
				str_regulador.fases   = faseABC;
				break;
			}
		//define parâmetros para esquemático
		str_regulador.esq.posbar1 = 50;
		str_regulador.esq.posbar2 = 50;
		if (monta->InsereRegulador(str_regulador) == NULL) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na inserção de Regulador");
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSegmentoBT(sdi_SEGMENTO_BT &sdi)
   {
	//variáveis locais
	int         fases_tag;
	strTRECHO   str_tre;
	VTArranjo   *arranjo;
	VTBarra     *barra1, *barra2;
	VTRede      *redeBT;

   //configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Segmento BT");
	try{//verifica se as fases estão definidas
		if (sdi.fases.IsEmpty())
			{//assume carga trifásica
			if (plog) plog->Add("Fases não definidas");
			return(true);
			}
		//converte fases
		fases_tag = fases->ExisteFase(sdi.fases);
		//verifica se existe a Rede
		if ((redeBT = ExisteRede(sdi.rede)) == NULL)
			{
			if (plog) plog->Add("Não está associado a uma rede");
			return(true);
			}
		//verifica se existe o Arranjo indicado
		if ((arranjo = arranjos->ExisteArranjo(sdi.codigo_condutor)) == NULL)
			{
			if (plog) plog->Add("Não existe o arranjo indicado");
			return(true);
			}
		//verifica se as fases do Arranjo ainda não estão definidas
		if (arranjo->Fases == faseINV)
			{//define fases do Arranjo iguais às fases do Trecho
			arranjo->Fases = fases_tag;
			}
		//verifica se as fases do Trecho são iguais às fases do Arranjo
		else if (arranjo->Fases != fases_tag)
			{
			if (plog) plog->Add("Segmento BT com fases inconsistentes com fases do condutor");
			return(true);
			}
		//determina Barras
		if ((barra1 = ExisteInsereBarra(sdi.codigo_pto1)) == NULL) return(true);
		if ((barra2 = ExisteInsereBarra(sdi.codigo_pto2)) == NULL) return(true);
		//verifica se as Barras são iguais
		if (barra1 == barra2)
			{
			if (plog) plog->Add("Pontos de acomplamento iguais");
			return(true);
			}
		//preenche estrutura do trecho
		str_tre.obra            = false;
		str_tre.tre_id         = FalsoId();
		str_tre.rede_id        = redeBT->Id;
		str_tre.bar1_id        = barra1->Id;
		str_tre.bar2_id        = barra2->Id;
		str_tre.codigo         = sdi.codigo;
      str_tre.extern_id      = "";
		str_tre.fases          = fases_tag;
		str_tre.comp_km        = sdi.comprimento_km;
		//define parâmetros para esquemático
		str_tre.esq.posbar1    = 50.;
		str_tre.esq.posbar2    = 50.;
		str_tre.comentario.comentario = "";
      str_tre.comentario.dx = 0;
      str_tre.comentario.dy = 0;
      //proteção
      if ((str_tre.comp_km < 0.001)||(str_tre.comp_km > 0.200))
         {//define comprimento de 10 metros
			if (plog) plog->Add("Segmento c/ comprimento = " + DoubleToStr("%4.3f km", sdi.comprimento_km));
         }
      //insere Trecho em VTMonta
      if (monta->InsereTrecho(str_tre, arranjo) == NULL) return(true);
      }catch(Exception &e)
         {
			if (plog) plog->Add("Erro na inserção de segmento BT");
			return(false);
         }
	return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSegmentoMT(sdi_SEGMENTO_MT &sdi)
   {
	//variáveis locais
	int         fases_tag;
	strTRECHO   str_tre;
	VTArranjo   *arranjo;
	VTBarra     *barra1, *barra2;
	VTRede      *redeMT;

   //configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Segmento MT");
	try{//verifica se as fases estão definidas
		if (sdi.fases.IsEmpty())
			{//assume fases ABC
			if (plog) plog->Add("Fases não definidas");
			fases_tag = faseABC;
			}
		else
			{//converte fases
			fases_tag = fases->ExisteFase(sdi.fases);
			}
		//verifica se existe a Rede
		if ((redeMT = ExisteRede(sdi.circuito)) == NULL)
			{
			if (plog) plog->Add("Não está associado a um circuito");
			return(true);
			}
		//verifica se existe o Arranjo indicado
		if ((arranjo = arranjos->ExisteArranjo(sdi.codigo_condutor)) == NULL)
			{
			if (plog) plog->Add("Não existe o arranjo indicado");
			return(true);
			}
		//verifica se as fases do Arranjo ainda não estão definidas
		if (arranjo->Fases == faseINV)
			{//define fases do Arranjo iguais às fases do Trecho
			arranjo->Fases = fases_tag;
			}
		//verifica se as fases do Trecho são iguais às fases do Arranjo
		else if (arranjo->Fases != fases_tag)
			{
			if (plog) plog->Add("Segmento MT com fases inconsistentes com fases do condutor");
			return(true);
			}
		//determina Barras
		if ((barra1 = ExisteInsereBarra(sdi.codigo_pto1)) == NULL) return(true);
		if ((barra2 = ExisteInsereBarra(sdi.codigo_pto2)) == NULL) return(true);
		//verifica se as Barras são iguais
		if (barra1 == barra2)
			{
			if (plog) plog->Add("Pontos de acomplamento iguais");
			return(true);
			}
		//preenche estrutura do trecho
		str_tre.obra            = false;
		str_tre.tre_id         = FalsoId();
		str_tre.rede_id        = redeMT->Id;
		str_tre.bar1_id        = barra1->Id;
		str_tre.bar2_id        = barra2->Id;
		str_tre.codigo         = sdi.codigo;
      str_tre.extern_id      = "";
		str_tre.fases          = fases_tag;
		str_tre.comp_km        = sdi.comprimento_km;
		//define parâmetros para esquemático
		str_tre.esq.posbar1    = 50.;
		str_tre.esq.posbar2    = 50.;
		str_tre.comentario.comentario = "";
      str_tre.comentario.dx = 0;
      str_tre.comentario.dy = 0;
      //proteção
      if ((str_tre.comp_km < 0.001)||(str_tre.comp_km > 0.200))
         {//define comprimento de 10 metros
			if (plog) plog->Add("Segmento c/ comprimento = " + DoubleToStr("%4.3f km", sdi.comprimento_km));
         }
      //insere Trecho em VTMonta
      if (monta->InsereTrecho(str_tre, arranjo) == NULL) return(true);
      }catch(Exception &e)
         {
			if (plog) plog->Add("Erro na inserção de segmento MT");
			return(false);
         }
	return(true);
	}

//-----------------------------------------------------------------------------
VTSuprimento* __fastcall TPreMonta::InsereSuprimento(AnsiString codigo, VTBarra *barra, double v_pu)
	{
	//variáveis locais
	strSUPRIMENTO str_sup;
	VTSuprimento  *suprimento;

	//configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Suprimento");
	try{//inicia estrutura
		str_sup.sup_id     = FalsoId();
		str_sup.bar_id     = barra->Id;
		str_sup.codigo     = codigo;
		str_sup.tiposup    = supVF;
      str_sup.smax       = 100;
		str_sup.vnom_kv    = barra->vnom;
		str_sup.zeq0.r     = 0.;
		str_sup.zeq0.x     = 0.;
		str_sup.zeq1       = str_sup.zeq0;
		str_sup.pcc_3f.p   = 0;
		str_sup.pcc_3f.q   = 400;
      str_sup.pcc_ft.p   = 0;
		str_sup.pcc_ft.q   = 600;
		//define curva com apenas 1 ponto
		str_sup.curva.id        = -1;
		str_sup.curva.tipica    = false;
		str_sup.curva.num_pat   = 1;
		str_sup.curva.num_valor = nvCURVA_PQVT;
		str_sup.curva.patamar[0].hm_ini.hora   = 0;
		str_sup.curva.patamar[0].hm_ini.minuto = 0;
		str_sup.curva.patamar[0].hm_fim.hora   = 24;
		str_sup.curva.patamar[0].hm_fim.minuto = 0;
		str_sup.curva.patamar[0].valor[indP]   = 0.;
		str_sup.curva.patamar[0].valor[indQ]   = 0.;
		str_sup.curva.patamar[0].valor[indV]   = v_pu;
		str_sup.curva.patamar[0].valor[indT]   = 0.;
		//define parâmetros para esquemático
		str_sup.esq.posbar = 50.;
		str_sup.esq.dxbar  = DXBAR;
   	str_sup.esq.dybar  = DYBAR;
		//insere Suprimento
		suprimento = (VTSuprimento*)monta->InsereSuprimento(str_sup);
      }catch(Exception &e)
         {
			if (plog) plog->Add("Erro na inserção de suprimento");
			suprimento = NULL;
         }
	return(suprimento);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafoAT_MT(sdi_TRAFO_AT_MT &sdi)
	{
	//variáveis locais
	double     r_pu, x_pu;
	strTRAFO   str_trafo;
	VTBarra    *barra1, *barra2;
	TSe        *se;

	//configura Resumo
	if (plog) plog->DefineValor("Eqpto", "TrafoAT_MT");
	try{//verifica se existe a Se
		if ((se = ExisteInsereSe(sdi)) == NULL)
			{
			if (plog) plog->Add("Não está associado a uma subestação");
			return(true);
			}
		//determina Barras
		if ((barra1 = se->barraAT) == NULL)                        return(true);
		if ((barra2 = se->ExisteBarraMT(sdi.vnom_sec_kv)) == NULL) return(true);
		//verifica se as Barras são iguais
		if (barra1 == barra2)
			{
			if (plog) plog->Add("Pontos de acomplamento iguais");
			return(true);
			}
		//calcula impedância em pu
		r_pu = (sdi.perda_total_perc - sdi.perda_vazio_perc) * 0.01;
		x_pu = 0.05;
		//insere Trafo
		str_trafo.trf_id        = FalsoId();
		str_trafo.rede_id       = se->redeSE->Id;
		str_trafo.bar1_id       = barra1->Id;
		str_trafo.bar2_id       = barra2->Id;
		str_trafo.codigo        = sdi.codigo;
		str_trafo.extern_id     = "";
		str_trafo.snom_mva      = sdi.snom_mva;
		str_trafo.z1.r          = r_pu;
		str_trafo.z1.x          = x_pu;
		str_trafo.z0            = str_trafo.z1;
		//perda no ferro em percentagem
		str_trafo.perda_ferro = sdi.perda_vazio_perc;
		//primário
		str_trafo.pri.vnom      = barra1->vnom;
		str_trafo.pri.tap       = barra1->vnom;
		str_trafo.pri.defasagem = 0;
		str_trafo.pri.zterra.r  = 0.;
		str_trafo.pri.zterra.x  = 0.;
		str_trafo.pri.fases     = faseABC;
		str_trafo.pri.ligacao   = lgTRIANG;
		str_trafo.pri.defasagem = 0;
		//secundário
		str_trafo.sec.vnom      = barra2->vnom;
		str_trafo.sec.tap       = barra2->vnom;
		str_trafo.sec.defasagem = 0;
		str_trafo.sec.zterra.r  = 0;
		str_trafo.sec.zterra.x  = 0;
		str_trafo.sec.fases     = faseABCN;
		//assume ligação Dy1
		str_trafo.sec.ligacao   = lgEST_AT;
		str_trafo.sec.defasagem = -30;
		//dados de LTC: assume sem ajuste automático
		str_trafo.ltc.pri_sec = ltcNAO_USADO;
		str_trafo.ltc.bar_id  = ID_NULO;
		str_trafo.ltc.v_pu    = 1.0;
		//define parâmetros para esquemático
		str_trafo.esq.posbar1 = 50;
		str_trafo.esq.posbar2 = 50;
		if (monta->InsereTrafo(str_trafo) == NULL) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na inserção de TrafoMT_BT");
			return(false);
         }
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafoMT_BT(sdi_TRAFO_MT_BT &sdi)
	{
	//variáveis locais
	int        fases_pri, fases_sec;
	double     r_pu, x_pu;
	strTRAFO   str_trafo;
	VTBarra    *barra1, *barra2;
	VTRede     *redeBT, *redeMT;

	//configura Resumo
	if (plog) plog->DefineValor("Eqpto", "TrafoMT_BT");
	try{//verifica se as fases do primário e do secundário estão definidas
		if (sdi.fases_pri.IsEmpty())
			{
			if (plog) plog->Add("Fases do primário indefinidas");
			return(true);
			}
		if (sdi.fases_sec.IsEmpty())
			{
			if (plog) plog->Add("Fases do secundário indefinidas");
			return(true);
			}
		//verifica se existe a Rede MT
		if ((redeMT = ExisteRede(sdi.circuito)) == NULL)
			{
			if (plog) plog->Add("Não está associado a um circuito");
			return(true);
			}
		//determina Barras
		if ((barra1 = ExisteInsereBarra(sdi.codigo_pto1)) == NULL) return(true);
		if ((barra2 = ExisteInsereBarra(sdi.codigo_pto2)) == NULL) return(true);
		//verifica se as Barras são iguais
		if (barra1 == barra2)
			{
			if (plog) plog->Add("Pontos de acomplamento iguais");
			return(true);
			}
		//assume que Barra1 é do primário e que Barra2 é do secundário
		if (IsDoubleZero(barra1->vnom)) barra1->vnom = redeMT->BarraInicial()->vnom;
		if (IsDoubleZero(barra2->vnom)) barra2->vnom = sdi.vff_sec_kv;
		//senecessário, cria a Rede BT
		if ((redeBT = ExisteInsereRedeBT(sdi.codigo_trafo, barra1)) == NULL) return(true);
		//determina fases do primário e do secundário
		fases_pri = fases->ExisteFase(sdi.fases_pri);
		fases_sec = fases->ExisteFase(sdi.fases_sec);
		//verifica se as fases do terciário foram definidas
		if (! sdi.fases_ter.IsEmpty())
			{//trafo monofásico a 5 fios: compõe fases do secundário
			fases_sec |= fases->ExisteFase(sdi.fases_ter);
			}
		//calcula impedância em pu
		r_pu = sdi.r_perc   * 0.01;
		x_pu = sdi.xhl_perc * 0.01;
		//verifica se as fases do terciário foram definidas
		if (! sdi.fases_ter.IsEmpty())
			{//trafo monofásico a 5 fios: a reatância foi dividida entre os enrolamentos
			x_pu = (sdi.xhl_perc * 0.01) / 0.75;;
			}
		//insere Trafo
		str_trafo.trf_id        = FalsoId();
		str_trafo.rede_id       = redeBT->Id;
		str_trafo.bar1_id       = barra1->Id;
		str_trafo.bar2_id       = barra2->Id;
		str_trafo.codigo        = sdi.codigo_trafo + IntToStr(sdi.sequencial);
		str_trafo.extern_id     = "";
		str_trafo.snom_mva      = sdi.snom_kva * 0.001; //kVA =>MVA
		str_trafo.z1.r          = r_pu;
		str_trafo.z1.x          = x_pu;
		str_trafo.z0            = str_trafo.z1;
		//perda no ferro em percentagem
		str_trafo.perda_ferro = sdi.perda_vazio_perc;
		//primário
		str_trafo.pri.vnom      = redeMT->BarraInicial()->vnom;
		str_trafo.pri.tap       = str_trafo.pri.vnom;
		str_trafo.pri.defasagem = 0;
		str_trafo.pri.zterra.r  = 0.;
		str_trafo.pri.zterra.x  = 0.;
		str_trafo.pri.fases     = fases_pri;
		//define tipo de ligacao
		switch(fases->NumeroDeFases(fases_pri))
			{
			case 1: str_trafo.pri.ligacao = lgMONOFASICO; break;
			case 2: str_trafo.pri.ligacao = lgMONOFASICO; break;
			case 3: str_trafo.pri.ligacao = lgTRIANG;     break;
			}
		//secundário
		str_trafo.sec.vnom      = sdi.vff_sec_kv;
		str_trafo.sec.tap       = sdi.vtap_pu * sdi.vff_sec_kv;
		str_trafo.sec.defasagem = 0;
		str_trafo.sec.zterra.r  = 0;
		str_trafo.sec.zterra.x  = 0;
		str_trafo.sec.fases     = fases_sec;
		//define tipo de ligacao
		switch(fases->NumeroDeFases(fases_sec))
			{
			case 1: str_trafo.sec.ligacao = lgMONOFASICO; break;
			case 2: str_trafo.sec.ligacao = lgMONOFASICO; break;
			case 3:
				str_trafo.sec.ligacao   = lgEST_AT;
				//assume ligação Dy1
				str_trafo.sec.defasagem = -30;
				break;
			}
		//dados de LTC: assume sem ajuste automático
		str_trafo.ltc.pri_sec = ltcNAO_USADO;
		str_trafo.ltc.bar_id  = ID_NULO;
		str_trafo.ltc.v_pu    = 1.0;
		//define parâmetros para esquemático
		str_trafo.esq.posbar1 = 50;
		str_trafo.esq.posbar2 = 50;
		if (monta->InsereTrafo(str_trafo) == NULL) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add("Erro na inserção de TrafoMT_BT");
			return(false);
         }
	return(true);
	}
//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::IntegraSubestacaoComPrimarios(void)
	{
	//variáveis locais
	TSe *se;
	VTRede *redePRI;

	for (int n = 0; n < lisSE->Count; n++)
		{
		se = (TSe*)lisSE->Items[n];
		se->IntegraComRedesPRI();
		}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::Sucesso(void)
	{
	return(sucesso);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataIniciaDados(AnsiString cod_dominio)
	{
	//reinicia dados locais
	LimpaTList(lisSE);
	ult_rede = NULL;
	dominio  = NULL;
	//salva o código do Domínio que só será criado se houver inserção de rede
	codigo_dominio = cod_dominio;

	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataFimDados(void)
	{
	//interliga a subestação com seus primários
	if (subestacao_integrada) IntegraSubestacaoComPrimarios();
	//destrói objetos TSe
	LimpaTList(lisSE);
	//
	return(monta->TrataFimDeDados());
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::ValidaImpedancia(strIMP &z)
	{
	if (IsDoubleZero(z.r) && IsDoubleZero(z.x)) //return(false);
		{//define valores default para R e X
		z.r = 0.01;
		z.x = 0.05;
		return(false);
      }
	return(true);
	}

/*
//-----------------------------------------------------------------------------
AnsiString __fastcall TPreMonta::DefineCodigoUnicoArranjo(AnsiString cod_orig)
   {
   //variáveis locais
   int count= 1;
   AnsiString cod_alt;

   //verifica se não existe arranjo com o código indicado
   if (arranjos->ExisteArranjo(cod_orig) == NULL) return(cod_orig);
   //código já existe: redefine o código
   do {
      cod_alt = cod_orig + "/1" + IntToStr(count++);
      }while (arranjos->ExisteArranjo(cod_alt) != NULL);
   return(cod_alt);
   }

//---------------------------------------------------------------------------
double __fastcall TPreMonta::DefineDefasagemTrafo(int ligacao_pri, int ligacao_sec)
   {
   if (ligacao_pri == ligacao_sec) return(0);
   if (ligacao_pri == lgTRIANG)    return(30);
   else                            return(-30);
   }

//---------------------------------------------------------------------------
VTDominio* __fastcall TPreMonta::ExisteDominio(void)
   {
   //variáveis locais
   TList *lisDOMINIO = redes->LisDominio();
   if (lisDOMINIO->Count > 0)
      {
      return((VTDominio*)(lisDOMINIO->First()));
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereArranjo(sdi_ARRANJO &sdi)
	{
	//variáveis locais
	strARRANJO2 str_arranjo;
	VTSuporte   *suporte;

	//cofigura Log
	if (plog) plog->DefineValor("Eqpto", "Arranjo");
	try{//preenche estrutura strARRANJO2
		str_arranjo.tipico     = true;
		str_arranjo.data       = DateOf(Now());
		str_arranjo.arranjo_id = sdi.id;
		str_arranjo.codigo     = DefineCodigoUnicoArranjo(sdi.cod);
		str_arranjo.extern_id  = sdi.extern_id;
		str_arranjo.tipo_cfg   = sdi.tipo;
		str_arranjo.fases      = fases->ExisteFase(sdi.fases);
		str_arranjo.res_terra  = 100.;
		str_arranjo.vmin_kv    = 0;
		str_arranjo.vmax_kv    = 0;
		str_arranjo.color      = clBlue;
		str_arranjo.status     = sttNOVO;
		//verifica o tipo de arranjo
		str_arranjo.cfg_arranjoZ0Z1.iadm_amp   = sdi.iadm_amp;
		str_arranjo.cfg_arranjoZ0Z1.z1         = sdi.z1_ohm_km;
		str_arranjo.cfg_arranjoZ0Z1.z0         = str_arranjo.cfg_arranjoZ0Z1.z1;
		str_arranjo.cfg_arranjoZ0Z1.c1         = 0.;
		str_arranjo.cfg_arranjoZ0Z1.c0         = 0.;
		str_arranjo.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
		if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z0))
			{
			if (plog) plog->Add("Impedância Z0 nula");
			//return(true);
			}
		if (! ValidaImpedancia(str_arranjo.cfg_arranjoZ0Z1.z1))
			{
			if (plog) plog->Add("Impedância Z1 nula");
			//return(true);
			}
		//insere Arranjo em VTMonta
		if (monta->InsereArranjo(str_arranjo) == NULL) return(true);  //return(false);
		}catch (Exception &e)
			{
			if (plog) plog->Add(ERRO_DE_SISTEMA);
			return(false);
			}
	return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereBarra(sdi_BARRA &sdi)
	{
	//variáveis locais
	strBARRA str_barra;
	VTEqpto  *eqpto;

	//cofigura Log
	if (plog) plog->DefineValor("Eqpto", "BARRA");
   try{//proteção: verifica se existe a Rede sendo importada
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //preenche estrutura strBARRA
      str_barra.bar_id  = sdi.id;
      str_barra.codigo  = sdi.cod;
      str_barra.vnom    = sdi.vnom_kv;
      str_barra.utm.x   = sdi.utm_m.x * 100; //m ==> cm
      str_barra.utm.y   = sdi.utm_m.y * 100; //m ==> cm
      str_barra.esq     = sdi.esq;
      //insere Barra
      if (monta->InsereBarraEqpto(str_barra) == NULL) return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TPreMonta::InsereBarraStart(void)
   {
   monta->InsereBarraStart();
   }

//---------------------------------------------------------------------------
void __fastcall TPreMonta::InsereBarraStop(void)
   {
   monta->InsereBarraStop();
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCapacitor(sdi_CAPACITOR &sdi)
	{
	//variáveis locais
	strCAPACITOR str_capacitor;

	//configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Capacitor");
	try{//proteção: verifica se existe a Rede
		if (rede == NULL)
			{
			if (plog) plog->Add("Não está associado a uma rede");
			return(true);
         }
      //verifica pot.reativa
      if (IsDoubleZero(sdi.snom_kva))
         {
         if (plog) plog->Add("Potência reativa nula");
         return(true);
         }
      //preenche estrutura strCAPACITOR
      str_capacitor.cap_id       = sdi.id;
      str_capacitor.extern_id    = "";
      str_capacitor.codigo       = sdi.cod;
      str_capacitor.bar_id       = sdi.id_barra;
      str_capacitor.vnom         = sdi.vnom_kv;
      str_capacitor.q_mvar       = sdi.snom_kva * 0.001;   //kVAr => MVAr
      str_capacitor.ligacao      = TraduzTipoLigacao(sdi.ligacao);
		//define curva
		str_capacitor.curva.id        = ID_NULO;
		str_capacitor.curva.tipica    = true;
		//não preenche pontos da curva
		str_capacitor.curva.num_pat   = 0;
		str_capacitor.curva.num_valor = 0;
		//define parâmetros para esquemático
		str_capacitor.esq          = sdi.esq;
		//insere Capacitor em VTMonta
		if (! monta->InsereCapacitor(str_capacitor)) return(false);
		}catch (Exception &e)
			{
			if (plog) plog->Add(ERRO_DE_SISTEMA);
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCapacitorSerie(sdi_CAPSERIE &sdi)
   {
   //variáveis locais
   strCAPSERIE  str_capserie;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Capserie");
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //preenche estrutura strCHAVE
      str_capserie.obra        = false;
      str_capserie.id          = sdi.id;
      str_capserie.rede_id     = rede->Id;
      str_capserie.bar1_id     = sdi.id_bar1;
      str_capserie.bar2_id     = sdi.id_bar2;
      str_capserie.codigo      = sdi.cod;
      str_capserie.extern_id   = sdi.extern_id;
      str_capserie.vnom        = sdi.vnom_kv;
      str_capserie.snom        = sdi.snom_kva;
      str_capserie.z           = sdi.z_ohm;
      str_capserie.esq         = sdi.esq;
      //insere Capserie em VTMonta
      if ((monta->InsereCapserie(str_capserie))== NULL) return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::InsereCargaIP(sdi_CARGA_IP &sdi)
   {
   //variáveis locais
   int         fases_tag;
   strCARGA    str_carga;
   VTCarga    *carga;

   try{//verifica se as fases estão definidas
      if (sdi.fases.IsEmpty())
         {//assume fases da ET sem neutro/terra
         if (plog) plog->Add("Carga IP sem fases definidas");
         fases_tag = faseAB;
         }
      else
         {//determina fases, filtrando neutro
         fases_tag = fases->ExisteFase(sdi.fases) & faseABC;
         }
      //preenche estrutura strCARGA
      str_carga.car_id       = sdi.id;
      str_carga.bar_id       = sdi.id_barra;
      str_carga.codigo       = sdi.cod;
      str_carga.extern_id    = "";
      //define classe de consumidor e modelo de carga
      str_carga.classe_cons    = consIP;
      str_carga.modelo.icte_pu = 0;
      str_carga.modelo.scte_pu = 1;
      str_carga.modelo.zcte_pu = 0;
      str_carga.fases          = fases_tag;
      //define dados de IP
      str_carga.ip.num_lamp = sdi.num_lampada;
      str_carga.ip.pot_kw   = sdi.p_w * 0.001; //W => kW
      str_carga.ip.fat_pot  = sdi.fp_pu;
      //dfine uso de curva típica
      str_carga.energia_mwhmes = 0;
      str_carga.curva.id       = -1;
      str_carga.curva.tipica   = true;
      str_carga.curva.num_pat  = 0;
      //define dados esquemático
      str_carga.esq           = sdi.esq;
      //insere Carga em VTMonta
      carga = (VTCarga*)monta->InsereCarga(str_carga);
      }catch (Exception &e)
         {
         carga = NULL;
         }
   return(carga);
   }

//-----------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::InsereCargaTipica(sdi_CARGA &sdi)
   {
   //variáveis locais
   int         fases_tag;
   strCARGA    str_carga;
   VTCarga    *carga;

   try{//verifica se as fases estão definidas
      if (sdi.fases.IsEmpty())
         {//assume fases da ET sem terra
         if (plog) plog->Add("Carga sem fases definidas");
         fases_tag = faseABCN;
         }
      else
         {//determina fases, filtrando terra
         fases_tag = fases->ExisteFase(sdi.fases) & faseABCN;
         }
      //preenche estrutura strCARGA
      str_carga.car_id       = sdi.id;
      str_carga.bar_id       = sdi.id_barra;
      str_carga.codigo       = sdi.cod;
      str_carga.extern_id    = "";
      //define classe de consumidor/energia
      str_carga.classe_cons    = TraduzClasseConsumidor(sdi.classe, sdi.grupo_tarifa);
      str_carga.modelo.icte_pu = sdi.i_cte_pu;
      str_carga.modelo.scte_pu = sdi.s_cte_pu;
      str_carga.modelo.zcte_pu = sdi.z_cte_pu;
      str_carga.fases          = fases_tag;
      str_carga.energia_mwhmes = sdi.consumo_kwhm * 0.001; //kWh/mes => MWh/mes
      //cancela vetor de demanda p/ curva própria
      str_carga.curva.id      = -1;
      str_carga.curva.tipica  = true;
      str_carga.curva.num_pat = 0;
      //cancela dados de IP
      str_carga.ip.num_lamp = 0.;
      str_carga.ip.pot_kw   = 0.;
      str_carga.ip.fat_pot  = 1.;
      str_carga.esq         = sdi.esq;
      //insere Carga em VTMonta
      carga = (VTCarga*)monta->InsereCarga(str_carga);
      }catch (Exception &e)
         {
         carga = NULL;
         }
   return(carga);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereChave(sdi_CHAVE &sdi)
   {
   //variáveis locais
   strCHAVE  str_chave;

   //cofigura Log
   if (plog) plog->DefineValor("Eqpto", "Chave");
   //verifica se está incluindo a topologia da subestação
   //if (! ETD_Enabled) return(true);
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //descarta chave "GRAMPO DE LINHA VIVA"
      //if (sdi.tipo.AnsiCompareIC("GRAMPO DE LINHA VIVA") == 0) return(true);
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
		//preenche estrutura strCHAVE
		str_chave.rede_id           = rede->Id;
		str_chave.chv_id            = sdi.id;
		str_chave.codigo            = sdi.cod;
		str_chave.bar1_id           = sdi.id_bar1;
		str_chave.bar2_id           = sdi.id_bar2;
		str_chave.tipochave.id      = sdi.tipochave.id;
		str_chave.tipochave.tipo    = sdi.tipochave.tipo;
		str_chave.tipochave.codigo  = sdi.tipochave.cod;
		str_chave.estado            = (sdi.estado == 0) ? chvABERTA: chvFECHADA;
		str_chave.corn              = sdi.iadm_amp;
		str_chave.protecao_ativa    = sdi.protecao_ativa;
		str_chave.telecomandada     = sdi.telecomandada;
		str_chave.tecno_comunicacao = sdi.tecno_comunicacao;
		//define parâmetros para esquemático
		str_chave.esq         = sdi.esq;
		//insere Chave em VTMonta
		if ((monta->InsereChave(str_chave))== NULL) return(false);
		}catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
VTCluster* __fastcall TPreMonta::InsereCluster(AnsiString codigo)
   {
   //variáveis locais
   strCLUSTER str;
   VTCluster *cluster;

   //verifica se foi definido o código
   if (codigo.IsEmpty()) return(NULL);
   //verifica se existe o Cluster
   if ((cluster = redes->ExisteCluster(codigo)) == NULL)
      {//inser um novo Cluster
      str.obra       = false;
      str.cluster_id = FalsoId();
      str.codigo     = codigo;
      cluster = (VTCluster*)monta->InsereCluster(str);
      }
   return(cluster);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDominio(AnsiString codigo)
	{
	//variáveis locais
	AnsiString  txt;
	strDOMINIO str_dominio;
	TDateTime   data = DateOf(Now());

	//verifica se já foi criado um Dominio
	if (dominio != NULL) return(true);
	//verifica se já existe um Dominio
	if ((dominio = ExisteDominio()) != NULL) return(true);
	//insere um Dominio em VTMonta
	txt.sprintf("%s_%04d.%02d.%02d", codigo.c_str(), YearOf(data), MonthOf(data), DayOf(data));
	str_dominio.obra        = false;
	str_dominio.dominio_id  = ID_NULO;
	str_dominio.codigo      = txt;
	str_dominio.data_criacao = data;
	//insere Dominio e salva ponteiro p/ objeto
	dominio = (VTDominio*)monta->InsereDominio(str_dominio);

	return(dominio != NULL);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereFiltro(sdi_FILTRO &sdi)
	{
   //variáveis locais
   strFILTRO  str_filtro;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Filtro");
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica pot.reativa
      if (IsDoubleZero(sdi.snom_kva))
         {
         if (plog) plog->Add("Potência nominal nula");
         return(true);
         }
      //preenche estrutura strFILTRO
      str_filtro.id           = sdi.id;
      str_filtro.extern_id    = "";
      str_filtro.codigo       = sdi.cod;
      str_filtro.bar_id       = sdi.id_bar;
      str_filtro.vnom         = sdi.vnom_kv;
      str_filtro.snom         = sdi.snom_kva * 0.001;   //kVAr => MVAr
      //define parâmetros para esquemático
      str_filtro.esq          = sdi.esq;
      //insere Filtro em VTMonta
      if (! monta->InsereFiltro(str_filtro)) return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereGerador(sdi_GERADOR &sdi)
   {
   //variáveis locais
   strGERADOR str_gerador;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Suprimento");
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se a tensão nominal do Suprimento é nula
      if (IsDoubleZero(sdi.vnom_kv))
         {//assume tensão nominal da Barra
         if (plog) plog->Add("Tensão nominal nula");
         }
      //insere Rede em VTMonta
      str_gerador.ger_id       = sdi.id;
      str_gerador.bar_id       = sdi.id_bar;
      str_gerador.codigo       = sdi.cod;
      str_gerador.tiposup      = TraduzTipoSuprimento(sdi.tipo_sup);
      str_gerador.snom_mva     = sdi.snom_kva * 0.001;  //kVA => MVA
      str_gerador.smax_mva     = str_gerador.snom_mva;
      str_gerador.vnom         = sdi.vnom_kv;
      str_gerador.z0           = sdi.z0_pu;
      str_gerador.z1           = sdi.z1_pu;
      str_gerador.ligacao      = sdi.ligacao;           //DVK 2013.07.11
      str_gerador.fplim        = sdi.fplim_pu;          //DVK 2013.07.11
      str_gerador.tipo_geracao = sdi.tipo_geracao;   //DVK 2013.07.11
      //define curva
		str_gerador.curva.id        = ID_NULO;
		str_gerador.curva.tipica    = true;
		//não preenche pontos da curva
		str_gerador.curva.num_pat   = 0;
		str_gerador.curva.num_valor = 0;
		//define parâmetros para esquemático
		str_gerador.esq = sdi.esq;
		//insere Suprimento
		if (monta->InsereGerador(str_gerador) == NULL) return(false);
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereMutua(sdi_MUTUA &sdi)
   {
   //variáveis locais
   strMUTUA str_mutua;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Mutua");
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se os Trechos são distintos
      if (sdi.id_tre1 == sdi.id_tre2)
         {
         if (plog) plog->Add("Trechos iguais");
         return(true);
         }
      //preenche estrutura strMUTUA
      str_mutua.id           = sdi.id;
      str_mutua.extern_id    = "";
      str_mutua.codigo       = sdi.cod;
      str_mutua.tre1_id      = sdi.id_tre1;
      str_mutua.tre2_id      = sdi.id_tre2;
      //barras de referências dos trecos
      str_mutua.tre1_bar_id  = sdi.id_bar_tre2;
      str_mutua.tre2_bar_id  = sdi.id_bar_tre2;
      str_mutua.z0           = sdi.z0;
      str_mutua.z1           = sdi.z1;
      //define parâmetros para esquemático
      str_mutua.postre1      = sdi.postre1;
      str_mutua.postre2      = sdi.postre2;
      //insere Mutua em VTMonta
      if (! monta->InsereMutua(str_mutua)) return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereReator(sdi_REATOR &sdi)
   {
   //variáveis locais
   strREATOR     str_reator;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Reator");
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica pot.reativa
      if (IsDoubleZero(sdi.q_kvar))
         {
         if (plog) plog->Add("Potência reativa nula");
         return(true);
         }
      //preenche estrutura strREATOR
      str_reator.id           = sdi.id;
      str_reator.extern_id    = "";
      str_reator.codigo       = sdi.cod;
      str_reator.bar_id       = sdi.id_bar;
      str_reator.vnom         = sdi.vnom_kv;
      str_reator.q_mvar       = sdi.q_kvar * 0.001;   //kVAr => MVAr
      str_reator.ligacao      = sdi.ligacao;
		//define curva
		str_reator.curva.id     = ID_NULO;
		str_reator.curva.tipica = true;
		//não preenche pontos da curva
		str_reator.curva.num_pat   = 0;
		str_reator.curva.num_valor = 0;
		//define parâmetros para esquemático
      str_reator.esq = sdi.esq;
      //insere Reator em VTMonta
      if (! monta->InsereReator(str_reator)) return(false);
      }catch (Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereRede(sdi_REDE &sdi)
	{
	//variáveis locais
	strREDE    str_rede;
	VTCluster *cluster;

	//se ainda não existir, cria o Dominio
	if (dominio == NULL) InsereDominio(codigo_dominio);
	//reinicia ponteiro p/ Rede sendo montada
	rede = NULL;
	//configura Log
	if (plog) plog->DefineValor("Eqpto", "Rede");
	try{//insere Rede em VTMonta
		if (plog) plog->DefineValor("Rede", sdi.cod);
		str_rede.rede_id            = sdi.id;
		str_rede.mrede_id           = ID_NULO;
		str_rede.cluster_id         = ID_NULO;
		str_rede.barini_id          = ID_NULO;
		str_rede.dominio_id         = (dominio != NULL) ? dominio->Id : ID_NULO;
		str_rede.codigo             = sdi.cod;
		str_rede.tiporede.id        = sdi.tiporede.id;
		str_rede.tiporede.segmento  = sdi.tiporede.seg;
		str_rede.tiporede.codigo    = sdi.tiporede.cod;
		str_rede.color              = TColor(sdi.color_r + (sdi.color_g << 8) + (sdi.color_b << 16));
		str_rede.estatica           = true;
		str_rede.radial             = false;
		str_rede.equivalente        = false;
		//verifica se Rede está associada a um Cluster
		cluster = InsereCluster(sdi.cluster);
		str_rede.cluster_id = (cluster != NULL) ? cluster->Id : ID_NULO;
		if ((rede = (VTRede*)monta->InsereRede(str_rede)) == NULL) return(false);
		}catch(Exception &e)
			{//cancela última Subestacao criada
			rede = NULL;
			if (plog) plog->Add(ERRO_DE_SISTEMA);
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereReguladorAutomatico(sdi_REGULADOR &sdi)
	{
	//variáveis locais
   strREGULADOR str_reg;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Regulador Automatico");
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //preenche estrutura strREGULADOR
      str_reg.obra        = false;
      str_reg.id          = sdi.id;
      str_reg.rede_id     = rede->Id;
      //define barra de entrada como barra1
      if (sdi.id_bar_ent == sdi.id_bar1)
         {
         str_reg.bar1_id     = sdi.id_bar1;
         str_reg.bar2_id     = sdi.id_bar2;
         }
      else
         {
         str_reg.bar1_id     = sdi.id_bar2;
         str_reg.bar2_id     = sdi.id_bar1;
         }
      str_reg.codigo      = sdi.cod;
      str_reg.by_pass     = false;
      str_reg.ligacao     = TraduzTipoLigacao(sdi.ligacao);
      str_reg.fases       = fases->ExisteFase(sdi.fases);
      //proteção
      if (sdi.fases.IsEmpty())
         {//define fases em função do tipo da ligação
         switch(str_reg.ligacao)
            {
            case lgEST_AT:           str_reg.fases = faseABCNT; break;
            case lgTRIANG:           str_reg.fases = faseABC;   break;
            case lgEST_ISO:          str_reg.fases = faseABC;   break;
            case lgMONOFASICO:       str_reg.fases = faseAN;    break;
            case lgDELTA_ABERTO:     str_reg.fases = faseB;     break;
            case lgDELTA_FECHADO:    str_reg.fases = faseABC;   break;
            default:                 str_reg.fases = faseABC;   break;
            }
         }
      str_reg.snom        = sdi.snom_kva * 0.001; //kVA =>MVA
      str_reg.var_tensao  = sdi.dv_perc;
      str_reg.num_passo   = sdi.num_passo / 2;
      //proteção: se não houver valores de número de passos e faixa de tensão, assume desligado
      if ((str_reg.var_tensao == 0)||(str_reg.num_passo == 0))
         {
         str_reg.by_pass = true;
         if (str_reg.var_tensao == 0) str_reg.var_tensao = 10;
         if (str_reg.num_passo  == 0)  str_reg.num_passo = 16;
         if (plog) plog->Add("Regulador redefinido como inativo");
         }
      str_reg.z0          = sdi.z0_pu;
      str_reg.z1          = sdi.z1_pu;
      str_reg.sentido.reversivel = true;
      str_reg.ajuste_auto        = true;
      //configura parâmetros para ajuste automático
      str_reg.cfg_auto.bar_ref_id = sdi.ajuste_auto.id_bar_ref;
      str_reg.cfg_auto.v_pu       = sdi.ajuste_auto.v_pu;
      //parâmetros p/ ajuste fixo
      str_reg.cfg_fixo.passo      = 0;
      //define parâmetros para esquemático
      str_reg.esq = sdi.esq;
      //insere Regulador em VTMonta
      if (monta->InsereRegulador(str_reg)== NULL) return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereReguladorTapFixo(sdi_REGULADOR &sdi)
   {
   //variáveis locais
   strREGULADOR str_reg;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Regulador Tap Fixo");
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //preenche estrutura strREGULADOR
      str_reg.obra        = false;
      str_reg.id          = sdi.id;
      str_reg.rede_id     = rede->Id;
      str_reg.bar1_id     = sdi.id_bar1;
      str_reg.bar2_id     = sdi.id_bar2;
      str_reg.codigo      = sdi.cod;
      str_reg.by_pass     = false;
      str_reg.ligacao     = TraduzTipoLigacao(sdi.ligacao);
      str_reg.fases       = fases->ExisteFase(sdi.fases);
      //proteção
      if (sdi.fases.IsEmpty())
         {//define fases em função do tipo da ligação
         switch(str_reg.ligacao)
            {
            case lgEST_AT:           str_reg.fases = faseABCNT; break;
            case lgTRIANG:           str_reg.fases = faseABC;   break;
            case lgEST_ISO:          str_reg.fases = faseABC;   break;
            case lgMONOFASICO:       str_reg.fases = faseAN;    break;
            case lgDELTA_ABERTO:     str_reg.fases = faseB;     break;
            case lgDELTA_FECHADO:    str_reg.fases = faseABC;   break;
            default:                 str_reg.fases = faseABC;   break;
            }
         }
      str_reg.snom        = sdi.snom_kva * 0.001; //kVA =>MVA
      str_reg.var_tensao  = sdi.dv_perc;
      str_reg.num_passo   = sdi.num_passo / 2;
      //proteção: se não houver valores de número de passos e faixa de tensão, assume desligado
      if ((str_reg.var_tensao == 0)||(str_reg.num_passo == 0))
         {
         str_reg.by_pass = true;
         if (str_reg.var_tensao == 0) str_reg.var_tensao = 10;
         if (str_reg.num_passo  == 0)  str_reg.num_passo = 16;
         if (plog) plog->Add("Regulador redefinido como inativo");
         }
      str_reg.z0                 = sdi.z0_pu;
      str_reg.z1                 = sdi.z1_pu;
      str_reg.sentido.reversivel = true;
      str_reg.ajuste_auto        = false;
      //parâmetros p/ ajuste fixo
      str_reg.cfg_fixo.passo      = sdi.ajuste_fixo.passo;
      //configura parâmetros para ajuste automático
		str_reg.cfg_auto.bar_ref_id = ID_NULO;
      str_reg.cfg_auto.v_pu       = 1.;
      //define parâmetros para esquemático
      str_reg.esq = sdi.esq;
      //insere Regulador em VTMonta
      if (monta->InsereRegulador(str_reg)== NULL) return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSuprimento(sdi_SUPRIMENTO &sdi)
	{
	//variáveis locais
	strSUPRIMENTO str_sup;

	//configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Suprimento");
	try{//proteção: verifica se existe a Rede
		if (rede == NULL)
			{
			if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se a tensão nominal do Suprimento é nula
      if (IsDoubleZero(sdi.vnom_kv))
         {//assume tensão nominal da Barra
         if (plog) plog->Add("Tensão nominal nula");
         }
      //insere Rede em VTMonta
      str_sup.sup_id     = sdi.id;
      str_sup.bar_id     = sdi.id_bar;
      str_sup.codigo     = sdi.cod;
      str_sup.tiposup    = TraduzTipoSuprimento(sdi.tipo_sup);
      str_sup.smax       = 100;
      str_sup.vnom_kv    = sdi.vnom_kv;
      str_sup.zeq0       = sdi.z0_pu;
      str_sup.zeq1       = sdi.z1_pu;
      str_sup.pcc_3f.p   = 0;
      str_sup.pcc_3f.q   = 0;
      str_sup.pcc_ft.p   = 0;
      str_sup.pcc_ft.q   = 0;
		//define curva com apenas 1 ponto
		str_sup.curva.id        = -1;
		str_sup.curva.tipica    = false;
		str_sup.curva.num_pat   = 1;
		str_sup.curva.num_valor = nvCURVA_PQVT;
		str_sup.curva.patamar[0].hm_ini.hora   = 0;
		str_sup.curva.patamar[0].hm_ini.minuto = 0;
		str_sup.curva.patamar[0].hm_fim.hora   = 24;
		str_sup.curva.patamar[0].hm_fim.minuto = 0;
		str_sup.curva.patamar[0].valor[indP]   = 0.;
		str_sup.curva.patamar[0].valor[indQ]   = 0.;
		str_sup.curva.patamar[0].valor[indV]   = 1.;
		str_sup.curva.patamar[0].valor[indT]   = 0.;
		//define parâmetros para esquemático
      str_sup.esq = sdi.esq;
      //insere Suprimento
		if (monta->InsereSuprimento(str_sup) == NULL) return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafo2E(sdi_TRAFO2E &sdi)
	{
	//variáveis locais
	strTRAFO   str_trafo;

	//configura Resumo
	if (plog) plog->DefineValor("Eqpto", "Trafo");
	try{//proteção: verifica se existe a Rede da subestação
		if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.pri.id_bar == sdi.sec.id_bar)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
		//insere Trafo
		str_trafo.trf_id        = sdi.id;
		str_trafo.rede_id       = rede->Id;
		str_trafo.bar1_id       = sdi.pri.id_bar;
		str_trafo.bar2_id       = sdi.sec.id_bar;
		str_trafo.codigo        = sdi.cod;
		str_trafo.extern_id     = "";
		str_trafo.snom_mva      = sdi.snom_kva * 0.001; //kVA =>MVA
		str_trafo.z0            = sdi.z0_pu;
		str_trafo.z1            = sdi.z1_pu;
		if (IsDoubleZero(sdi.sbase_kva))
			{str_trafo.perda_ferro = 0.1;}
		else
			{str_trafo.perda_ferro = ((sdi.perda_fe_w /1000.) / sdi.sbase_kva) * 100.;}
		//primário
		str_trafo.pri.ligacao   = TraduzTipoLigacao(sdi.pri.ligacao);
		str_trafo.pri.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
		str_trafo.pri.vnom      = sdi.pri.vnom_kv;
		str_trafo.pri.tap       = sdi.pri.vtap_kv;
		str_trafo.pri.defasagem = 0;
      str_trafo.pri.zterra    = sdi.pri.zat_ohm;
      //secundário
		str_trafo.sec.ligacao   = TraduzTipoLigacao(sdi.sec.ligacao);
      str_trafo.sec.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
      str_trafo.sec.vnom      = sdi.sec.vnom_kv;
      str_trafo.sec.tap       = sdi.sec.vtap_kv;
		str_trafo.sec.defasagem = DefineDefasagemTrafo(str_trafo.pri.ligacao, str_trafo.sec.ligacao);
      str_trafo.sec.zterra    = sdi.sec.zat_ohm;
      //dados de LTC
      str_trafo.ltc.pri_sec   = TraduzTipoLTC(sdi.ajuste.tipo);
      if (str_trafo.ltc.pri_sec == ltcNAO_USADO)
			{//sem ajuste
         str_trafo.ltc.bar_id = ID_NULO;
         str_trafo.ltc.v_pu   = 1.0;
         }
      else
			{//assume que a barra controlada é a do secundário
			str_trafo.ltc.bar_id = str_trafo.bar2_id;
			str_trafo.ltc.v_pu   = 1.0;
			}
		//define parâmetros para esquemático
		str_trafo.esq           = sdi.esq;
		if (monta->InsereTrafo(str_trafo) == NULL) return(false);
		}catch(Exception &e)
			{
			if (plog) plog->Add(ERRO_DE_SISTEMA);
			return(false);
         }
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafo3E(sdi_TRAFO3E &sdi)
	{
	//variáveis locais
	strTRAFO3E str_trafo;

	//configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Trafo3E");
   //verifica se está incluindo a topologia da subestação
   //if (! ETD_Enabled) return(true);
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se foi definido Sbase
      if (IsDoubleZero(sdi.sbase_kva))  sdi.sbase_kva = sdi.pri.snom_kva;
      if (IsDoubleZero(sdi.sbase_kva))  sdi.sbase_kva = 100000;
      //insere Trafo
      str_trafo.trf_id        = sdi.id;
      str_trafo.rede_id       = rede->Id;
      str_trafo.bar1_id       = sdi.pri.id_bar;
      str_trafo.bar2_id       = sdi.sec.id_bar;
      str_trafo.bar3_id       = sdi.ter.id_bar;
      str_trafo.codigo        = sdi.cod;
      str_trafo.extern_id     = "";
      str_trafo.perda_ferro   = ((sdi.perda_fe_w * 0.001) / sdi.sbase_kva) * 100.;
      //primário
      str_trafo.pri.ligacao   = TraduzTipoLigacao(sdi.pri.ligacao);
      str_trafo.pri.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
      str_trafo.pri.snom      = sdi.pri.snom_kva * 0.001; //kva =>mva
      str_trafo.pri.vnom      = sdi.pri.vnom_kv;
      str_trafo.pri.tap       = sdi.pri.vtap_kv;
      str_trafo.pri.defasagem = 0;
      str_trafo.pri.zterra    = sdi.pri.zat_ohm;
      //secundário
      str_trafo.sec.ligacao   = TraduzTipoLigacao(sdi.sec.ligacao);
      str_trafo.sec.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
      str_trafo.sec.snom      = sdi.sec.snom_kva * 0.001; //kva =>mva
      str_trafo.sec.vnom      = sdi.sec.vnom_kv;
      str_trafo.sec.tap       = sdi.sec.vtap_kv;
      str_trafo.sec.defasagem = DefineDefasagemTrafo(str_trafo.pri.ligacao, str_trafo.sec.ligacao);
      str_trafo.sec.zterra    = sdi.sec.zat_ohm;
      //terciário
      str_trafo.ter.ligacao   = TraduzTipoLigacao(sdi.ter.ligacao);
      str_trafo.ter.fases     = (str_trafo.pri.ligacao == lgEST_AT) ? faseABCN : faseABC;
      str_trafo.ter.snom      = sdi.ter.snom_kva * 0.001; //kva =>mva
      str_trafo.ter.vnom      = sdi.ter.vnom_kv;
      str_trafo.ter.tap       = sdi.ter.vtap_kv;
      str_trafo.ter.defasagem = DefineDefasagemTrafo(str_trafo.pri.ligacao, str_trafo.ter.ligacao);
      str_trafo.ter.zterra    = sdi.ter.zat_ohm;
      //impedâncias de sequência ps
      str_trafo.zps0 = sdi.zps0_pu;
      str_trafo.zps1 = sdi.zps1_pu;
      //impedâncias de sequência pt
      str_trafo.zpt0 = sdi.zpt0_pu;
      str_trafo.zpt1 = sdi.zpt1_pu;
      //impedâncias de sequência st
      str_trafo.zst0 = sdi.zst0_pu;
      str_trafo.zst1 = sdi.zst1_pu;
      //define parâmetros para esquemático
      str_trafo.esq           = sdi.esq;
      if (! monta->InsereTrafo3E(str_trafo)) return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafoMono(sdi_TRAFO_MONO &sdi)
   {
   //variáveis locais
   strTRAFO   str_trafo;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Trafo Monofasico");
   try{//proteção: verifica se existe a Rede da subestação
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.pri.id_bar == sdi.sec.id_bar)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //insere Trafo
      str_trafo.trf_id        = sdi.id;
      str_trafo.rede_id       = rede->Id;
      str_trafo.bar1_id       = sdi.pri.id_bar;
      str_trafo.bar2_id       = sdi.sec.id_bar;
      str_trafo.codigo        = sdi.cod;
      str_trafo.extern_id     = "";
      str_trafo.snom_mva      = sdi.snom_kva * 0.001; //kVA =>MVA
      str_trafo.z0            = sdi.z_pu;
      str_trafo.z1            = sdi.z_pu;
      if (IsDoubleZero(sdi.sbase_kva))
         {str_trafo.perda_ferro = 0.1;}
      else
         {str_trafo.perda_ferro = ((sdi.perda_fe_w * 0.001) / sdi.sbase_kva) * 100.;}
      //primário
      str_trafo.pri.ligacao   = lgMONOFASICO;
      str_trafo.pri.fases     = fases->ExisteFase(sdi.pri.fases);
      str_trafo.pri.vnom      = sdi.pri.vnom_kv;
      str_trafo.pri.tap       = sdi.pri.vtap_kv;
      str_trafo.pri.defasagem = 0;
      str_trafo.pri.zterra    = sdi.pri.zat_ohm;
      //secundário
      str_trafo.sec.ligacao   = lgMONOFASICO;
      str_trafo.sec.fases     = fases->ExisteFase(sdi.sec.fases);
      str_trafo.sec.vnom      = sdi.sec.vnom_kv;
      str_trafo.sec.tap       = sdi.sec.vtap_kv;
      str_trafo.sec.defasagem = 0;
      str_trafo.sec.zterra    = sdi.sec.zat_ohm;
      //dados de LTC: sem ajuste
      str_trafo.ltc.pri_sec = ltcNAO_USADO;
      str_trafo.ltc.bar_id  = ID_NULO;
      str_trafo.ltc.v_pu    = 1.0;
      //define parâmetros para esquemático
      str_trafo.esq           = sdi.esq;
      if (monta->InsereTrafo(str_trafo) == NULL) return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrafoZZ(sdi_TRAFOZZ &sdi)
   {
   //variáveis locais
   strTRAFOZZ str_trafoZZ;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "TrafoZZ");
   try{//proteção: verifica se existe a Rede da subestação
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associada a uma rede");
         return(true);
         }
      //insere Trafo
      str_trafoZZ.id        = sdi.id;
      str_trafoZZ.bar_id    = sdi.id_bar;
      str_trafoZZ.codigo    = sdi.cod;
      str_trafoZZ.extern_id = sdi.extern_id;
      str_trafoZZ.vnom      = sdi.vnom_kv;
      str_trafoZZ.snom      = sdi.snom_kva * 0.001; //kVA =>MVA
      str_trafoZZ.z0        = sdi.z0_pu;
      //define parâmetros para esquemático
      str_trafoZZ.esq       = sdi.esq;
      if (monta->InsereTrafoZZ(str_trafoZZ) == NULL) return(false);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereTrecho(sdi_TRECHO &sdi)
   {
   //variáveis locais
   AnsiString  cod_cabo;
   strTRECHO   str_tre;
   VTArranjo   *arranjo;

   //configura Resumo
   if (plog) plog->DefineValor("Eqpto", "Trecho");
   try{//proteção: verifica se existe a Rede
      if (rede == NULL)
         {
         if (plog) plog->Add("Não está associado a uma rede");
         return(true);
         }
      //verifica se existe o Arranjo indicado
      if ((arranjo = arranjos->ExisteArranjo(sdi.arranjo_tipico.id_arranjo)) == NULL)
         {
         if (plog) plog->Add("Não existe o arranjo indicado");
         return(true);
         }
      //verifica se as Barras são distintas
      if (sdi.id_bar1 == sdi.id_bar2)
         {
         if (plog) plog->Add("Barra inicial igual à barra final");
         return(true);
         }
      //preenche estrutura do trecho
      str_tre.obra            = false;
      str_tre.tre_id         = sdi.id;
      str_tre.rede_id        = rede->Id;
      str_tre.bar1_id        = sdi.id_bar1;
      str_tre.bar2_id        = sdi.id_bar2;
      str_tre.codigo         = sdi.cod;
      str_tre.extern_id      = "";
      str_tre.fases          = arranjo->Fases;
      str_tre.comp_km        = sdi.comp_m * 0.001;  //m => km
      //define parâmetros para esquemático
      str_tre.esq            = sdi.esq;
      str_tre.comentario.comentario = "";
      str_tre.comentario.dx = 0;
      str_tre.comentario.dy = 0;
      //proteção
      if ((str_tre.comp_km < 0.001)||(str_tre.comp_km > 0.200))
         {//define comprimento de 10 metros
         if (plog) plog->Add("Trecho c/ comprimento = " + DoubleToStr("%2.1f m", sdi.comp_m));
         }
      //insere Trecho em VTMonta
      if (monta->InsereTrecho(str_tre, arranjo) == NULL) return(true);
      }catch(Exception &e)
         {
         if (plog) plog->Add(ERRO_DE_SISTEMA);
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::Sucesso(void)
	{
	return(sucesso);
	}

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzClasseConsumidor(AnsiString codigo, AnsiString grupo)
	{
	//variáveis locais
	int       classe_tag;
	VTClasse *classe;

	//verifica se existe a Classe indicada
	if ((classe = classes->ExisteClasse(codigo)) != NULL)
		{
		classe_tag = classe->Tag;
		}
   //tenta identificar outros códigos dentro de cada grupo tarifário
   else if (grupo.AnsiCompareIC("A2") == 0)
      {
      classe_tag = consA2;
      }
   else if (grupo.AnsiCompareIC("A4") == 0)
      {
      classe_tag = consA4;
      }
   else
      {
      if      (codigo.AnsiCompareIC("RESIDENCIAL") == 0) classe_tag = consRESIDENCIAL;
      else if (codigo.AnsiCompareIC("COMERCIAL")   == 0) classe_tag = consCOMERCIAL;
      else if (codigo.AnsiCompareIC("INDUSTRIAL")  == 0) classe_tag = consINDUSTRIAL;
      else if (codigo.AnsiCompareIC("RURAL")       == 0) classe_tag = consRURAL;
	   else                                               classe_tag = consOUTROS;
      }
   return(classe_tag);
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoChave(AnsiString codigo)
   {
   //variáveis locais
   VTTipoChave *tipochave;

   //verifica se existe o tipo de chave
   if ((tipochave = tipos->ExisteTipoChave(codigo)) != NULL)
      {//verifica se tipochave indefinida
      if (tipochave->Tipo != chaveINDEFINIDA) return(tipochave->Tipo);
      }
   if (codigo.AnsiPos("FUSI") > 0) return(chaveBASE_FUSIVEL);
   if (codigo.AnsiPos("DISJ") > 0) return(chaveDISJUNTOR);
   if (codigo.AnsiPos("RELI") > 0) return(chaveRELIGADORA);
   if (codigo.AnsiPos("SECC") > 0) return(chaveSECCIONADORA);
   return(chaveINDEFINIDA);
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoLigacao(AnsiString ligacao)
   {
   //verifica tipo de ligação definida por string
   if      (ligacao.AnsiCompareIC("Yaterrado")    == 0) return(lgEST_AT);
   else if (ligacao.AnsiCompareIC("Delta")        == 0) return(lgTRIANG);
   else if (ligacao.AnsiCompareIC("Yisolado")     == 0) return(lgEST_ISO);
   else if (ligacao.AnsiCompareIC("Monofasico")   == 0) return(lgMONOFASICO);
   else if (ligacao.AnsiCompareIC("DeltaAberto")  == 0) return(lgDELTA_ABERTO);
   else if (ligacao.AnsiCompareIC("DeltaFechado") == 0) return(lgDELTA_FECHADO);
   else if (ligacao.AnsiCompareIC("Indefinido")   == 0) return(lgEST_ISO);
   else
      {//tipo de ligação definida por constante
      try{
         switch(StrToInteger(ligacao))
            {
            case lgEST_AT:           return(lgEST_AT);
            case lgTRIANG:           return(lgTRIANG);
            case lgEST_ISO:          return(lgEST_ISO);
            case lgMONOFASICO:       return(lgMONOFASICO);
            case lgDELTA_ABERTO:     return(lgDELTA_ABERTO);
            case lgDELTA_FECHADO:    return(lgDELTA_FECHADO);
            default:                 return(lgEST_ISO);
            }
         }catch(Exception &e)
            {//retorna default
            return(lgEST_ISO);
            }

      }
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoLTC(AnsiString ajuste)
   {
   //ajuste definido como string
   if (ajuste.AnsiCompareIC("primario")      == 0) return(ltcPRIMARIO);
   if (ajuste.AnsiCompareIC("secundario")    == 0) return(ltcSECUNDARIO);
   if (ajuste.AnsiCompareIC("NAO UTILIZADO") == 0) return(ltcNAO_USADO);
   //ajuste definido com valor numérico
   try{
      switch(StrToInteger(ajuste))
         {
         case 0:  return(ltcNAO_USADO);
         case 1:  return(ltcPRIMARIO);
         case 2:  return(ltcSECUNDARIO);
         default: return(ltcNAO_USADO);
         }
      }catch(Exception &e)
         {//retorna default
         return(ltcNAO_USADO);
         }
   }

//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoRede(AnsiString tipo)
   {
   //variáveis locais
   int        segmento = redeINDEF;
   VTTipoRede *tiporede;

   if ((tiporede = tipos->ExisteTipoRede(tipo)) != NULL)
      {
      segmento = tiporede->Segmento;
      }
   return(segmento);
   }


//-----------------------------------------------------------------------------
int __fastcall TPreMonta::TraduzTipoSuprimento(int tipo_sup)
   {
   if (tipo_sup == 1) return(supPV);
   if (tipo_sup == 2) return(supPQ);
   if (tipo_sup == 3) return(supVF);
   //retorna default
   return(supVF);
   }
   
//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataIniciaDados(AnsiString cod_dominio)
	{
	//reinicia dados locais
	rede    = NULL;
	dominio = NULL;
	//insere um novo Domínio
	//InsereDominio(cod_dominio);
	//salva o código do Domínio que só será criado se houver inserção de rede
	codigo_dominio = cod_dominio;

	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataFimDados(void)
	{
	//variáveis locais
	VTRede      *rede;
	TList       *lisREDE = redes->LisRede();

	//cancela Barra inicial das Redes
	for (int n = 0; n < lisREDE->Count; n++)
      {
		rede = (VTRede*)lisREDE->Items[n];
      rede->DefineBarraInicial(NULL);
		}
   return(monta->TrataFimDeDados());
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta::ValidaImpedancia(strIMP &z)
	{
	if (IsDoubleZero(z.r) && IsDoubleZero(z.x)) //return(false);
		{//define valores default para R e X
		z.r = 0.01;
		z.x = 0.05;
		return(false);
      }
	return(true);
   }
*/

//-----------------------------------------------------------------------------
// eof



