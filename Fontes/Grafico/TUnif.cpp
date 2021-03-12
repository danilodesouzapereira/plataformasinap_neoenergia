//---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop
#include "TUnif.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
#include "TPadrao.h"
#include "TFormGraf.h"
#include "VTGrid.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Constante\Const.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Edita\VTEdita.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Figura\VTFiguras.h"
#include "..\Geral\VTGeral.h"
#include "..\Mapa\VTPoligono.h"
#include "..\Mapa\VTRegiao.h"
#include "..\Mapa\VTRegioes.h"
#include "..\Obra\VTObra.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCargaNL.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMotor.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTYref.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\Redegraf\VTCelula.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/*
//-----------------------------------------------------------------------------
// função global
//-----------------------------------------------------------------------------
VTUnif* NewObjUnif(VTApl *apl, AnsiString nome)
   {
   return(new TUnif(apl, nome));
   }
*/
//---------------------------------------------------------------------------
// TUnif::TUnif() - construtor
//---------------------------------------------------------------------------
__fastcall TUnif::TUnif(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   //salva tipo e nome do diagrama unifilar
   this->tipo_graf = tipo_graf;
   this->nome      = nome;
   //salva ponteiros p/ objetos
   this->apl = apl;
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   grid      = geral->grid;
   redegraf  = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   regioes   = (VTRegioes*)apl->GetObject(__classid(VTRegioes));
   //salva nome do Diag.Unif
   //cria objetos
   area     = DLL_NewObjArea();
   //draw     = NewObjDrawTopo(apl);
   consulta = DLL_NewObjConsulta(apl);
   //desabilita mapa temático
   MapaTematicoEnabled = false;
   //inicia posição p/ animação
   //padrao->flow.ano        = 0;
   //padrao->flow.num_pat    = 0;
   redraw_animacao.apagar  = false;
   redraw_animacao.postre  = 2;
   }

//---------------------------------------------------------------------------
// TUnif::~TUnif() - destrutor
//---------------------------------------------------------------------------
__fastcall TUnif::~TUnif(void)
   {
   //destrói objetos
   if (area)     {delete area;     area     = NULL;}
   if (draw)     {delete draw;     draw     = NULL;}
   if (consulta) {delete consulta; consulta = NULL;}
   }
   
//---------------------------------------------------------------------------
void __fastcall TUnif::Animacao(void)
   {
   //verifica se deva apagar círculos atualmente exibidos
   if (redraw_animacao.apagar)
      {//apaga círculos atualmente exibidos
      RedrawAnimacao();
      //redefine posição para desenhar novos cículos
      if ((redraw_animacao.postre  += 2) >= 100) redraw_animacao.postre = 2;
      }
   //redesenha círculos
   RedrawAnimacao();
   //atualiza gráfico
   form->RedrawTelaReal();
   }
/*
//-----------------------------------------------------------------------------
TColor __fastcall TUnif::Color(VTEqpto *eqpto, TColor cor)
   {
   //variáveis locais
   VTCarga *carga;

   //verifica se Eqpto está associado a uma Obra associado ao objeto Obras
   if ((eqpto->Obra == NULL)||(eqpto->Obra->Obras == NULL))
      {
      if (eqpto->Tipo() == eqptoCARGA)
         {
         carga =(VTCarga*)eqpto;
         switch(carga->classe->Tag)
            {
            case consRESIDENCIAL: return(clWhite);
            case consCOMERCIAL:   return(clYellow);
            case consINDUSTRIAL:  return(clMoneyGreen);
            case consRURAL:       return(clPurple);
            case consOUTROS:      return(clTeal);
            case consA4:          return(clSkyBlue);
            case consEQUIVALENTE: return(clOlive);
            }
         }
      //retorna a cor pré-definida p/ o Eqpto
      return(cor);
      }
	//se Eqpto é um Eqbar, assume cor de acordo com sua Barra
   if (eqpto->TipoEqbar())
      {eqpto = ((VTEqbar*)eqpto)->pbarra;}
   if (eqpto->Status[sttISOLADO]) return(padrao->isolado.cor);
   else                           return(clAqua);
   }
*/
//-----------------------------------------------------------------------------
TColor __fastcall TUnif::Color(VTEqpto *eqpto, TColor cor)
	{
	/*
	//verifica se Eqpto está associado a uma Obra associado ao objeto Obras
	if ((eqpto->Obra == NULL)||(eqpto->Obra->Obras == NULL))
		{
		if (eqpto->TipoEqbar()) cor = padrao->barra.cor;
		}
	else
		{//verifica se os Eqptos da Obras devem ser mostrados com cor diferenciada
		if (eqpto->Obra->ColorEnabled)
			{//define cor do Eqpto
			cor = eqpto->Obra->Color;
			}
		}
	return(cor);
	*/

   VTCarga* carga;

   //verifica se Eqpto está associado a uma Obra associado ao objeto Obras
	if ((eqpto->Obra == NULL)||(eqpto->Obra->Obras == NULL))
		{
		if (eqpto->TipoEqbar())
			{
			if(eqpto->Tipo() == eqptoCARGA)
				{
				carga = (VTCarga*)eqpto;
				if(carga->GrupoA)
					{
					cor = clWebDodgerBlue;
					}
				else if(carga->classe->Tag == consEQUIVALENTE)
					{
					cor = clOlive;
					}
				return cor;
				}
			cor = padrao->barra.cor;
			}
		}
	else
		{//verifica se os Eqptos da Obras devem ser mostrados com cor diferenciada
		if (eqpto->Obra->ColorEnabled)
			{//define cor do Eqpto
			cor = eqpto->Obra->Color;
			}
		}
	return(cor);
	}

//-----------------------------------------------------------------------------
void __fastcall TUnif::DefObjAnimacao(VTAnimacao *animacao)
   {
   //salva ponteiro para objeto
   this->animacao = animacao;
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::DefObjFormGraf(TFormGraf *form)
   {
   //salva ponteiro para objeto
   this->form = form;
   //define objetos p/ Draw
   draw->DefObjFormGraf(form);
   //draw->DefObjGrid(grid);
   //define objetos p/ UnifMedicao
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::DefObjPadrao(TPadrao *padrao)
   {
   //salva ponteiro para objeto
   this->padrao = padrao;
   //define objeto p/ Draw
   draw->DefObjPadrao(padrao);
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::DestacaEqpto(TList *lisEQP, TColor color, int dim)
   {
   //variáveis locais
   int     orig_dim;
   VTEqpto *eqpto;

   //proteção
   if ((lisEQP == NULL)||(lisEQP->Count == 0)) return;
   //redefine padrão: dimensão das linhas
   orig_dim            = padrao->ligacao.dim;
   padrao->ligacao.dim = dim;
   //reapresenta Eqptos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      eqpto->Status[sttREDRAW] = true;
      switch(eqpto->Tipo())
         {//mutua
         case eqptoMUTUA:      RedrawMutua((VTMutua*)eqpto, color);           break;
         //ligacao
         case eqptoCAPSERIE:   RedrawCapserie((VTCapserie*)eqpto, color);     break;
         case eqptoCHAVE:      RedrawChave((VTChave*)eqpto, color);           break;
         case eqptoTRECHO:     RedrawTrecho((VTTrecho*)eqpto, color);         break;
         case eqptoTRAFO:      RedrawTrafo((VTTrafo*)eqpto, color);           break;
         case eqptoTRAFO3E:    RedrawTrafo3E((VTTrafo3E*)eqpto, color);       break;
         //eqbar
         case eqptoCAPACITOR:  RedrawCapacitor((VTCapacitor*)eqpto, color);   break;
         case eqptoCARGA:      RedrawCarga((VTCarga*)eqpto, color);           break;
         case eqptoFILTRO:     RedrawFiltro((VTFiltro*)eqpto, color);         break;
         case eqptoGERADOR:    RedrawGerador((VTGerador*)eqpto, color);       break;
         case eqptoMEDIDOR:    RedrawMedidor((VTMedidor*)eqpto, color);         break;
         case eqptoREATOR:     RedrawReator((VTReator*)eqpto, color);         break;
         case eqptoSUPRIMENTO: RedrawSuprimento((VTSuprimento*)eqpto, color); break;
         case eqptoTRAFOZZ:    RedrawTrafoZZ((VTTrafoZZ*)eqpto, color);       break;
         case eqptoYREF:       RedrawYref((VTYref*)eqpto, color);             break;
         //barra
         case eqptoBARRA:      RedrawBarra((VTBarra*)eqpto, color);           break;
         default:              break;
         }
      }
   //atualiza gráfico
   form->RedrawTelaReal();
   //restaura padrão: dimensão das linhas
   padrao->ligacao.dim = orig_dim;
   }

/*
//---------------------------------------------------------------------------
void __fastcall TUnif::HabilitaLegenda(void)
   {
   //variáveis locais
   double  dist_media, dx, dy, dxy;
   int     num_rede, num_lig;
   VTArea  *area_rede;
   VTBarra *pbarra;
   VTRede  *rede;
   TList   *lisLIG;
   TList   *lisREDE = redes->LisRede();

   //obtém área de zoom (área total das redes visível na tela)
   form->AreaVisivel(area);
   //determina número de Barras dentro da área de zoom
   num_rede   = 0;
   dist_media = 0.;
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      //verifica se a Rede não está sendo apresentada
      if (! rede->Visible()) continue;
      //determina a área da Rede
      area_rede = rede->AreaEsquematico();
      //obtém lista de Barras e de Ligacoes da Rede
      lisLIG = rede->LisLigacao();
      if ((num_lig = lisLIG->Count) == 0) num_lig = 1;
      //calcula o coprimento médio das Ligacoes
      if (dx > dy) dxy = area_rede->Width()  / num_lig;
      else         dxy = area_rede->Height() / num_lig;
      //acumula distância média
      dist_media +=dxy;
      //incrementa número de redes
      num_rede++;
      }
   //calcula distância média em pixel
   if (num_rede == 0) num_rede = 1;
   dist_media = (dist_media / num_rede) * form->Escala();
   draw->HabilitaLegenda(dist_media > 30);
   }
*/
/*
//---------------------------------------------------------------------------
void __fastcall TUnif::HabilitaLegenda(void)
   {
   //variáveis locais
   TList *lisEQP;

   //determina número de Barras visíveis
   lisEQP = LisEqptoRede();
   draw->HabilitaBarra(lisEQP->Count < 400);
   draw->HabilitaLegenda(lisEQP->Count < 200);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TUnif::HabilitaLegenda(void)
   {
   //variáveis locais
   int count;

   //determina número de Barras visíveis
   count = consulta->QtdeEqptoRedeVisible(redes, eqptoBARRA);
   draw->HabilitaBarra(count < 500);          //400
   draw->HabilitaLegenda(count < 150);
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::LimpaTela(void)
   {
   //variáveis locais
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //limpa a tela
   form->LimpaTela(redegraf->CorFundo, false);
   }

//---------------------------------------------------------------------------
TList* __fastcall TUnif::LisEqptoRede(int eqp_tipo)
   {
   //obtém lista de Eqptos do tipo indicado
   return(consulta->LisEqptoRedeVisible(redes, eqp_tipo));
   }

//---------------------------------------------------------------------------
TList* __fastcall TUnif::LisEqptoRede(VTRede *rede, int eqp_tipo)
   {
   //obtém lista de Eqptos do tipo indicado da Rede indicada
   return(consulta->LisEqptoRedeVisible(rede, eqp_tipo));
   }

//---------------------------------------------------------------------------
TList* __fastcall TUnif::LisEqptoReducao(VTReducao *reducao, int eqp_tipo)
   {
   //determina lista de Eqptos
   //return(consulta->LisEqpto(reducao, eqp_tipo));
   return(consulta->LisEqptoReducaoVisible(reducao, eqp_tipo));
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::Moldura(TList *lisEQP)
   {
   //variáveis locais
   int          x1, y1, x2, y2;
   VTEqpto      *eqpto;
   VTCoordenada *coordenada;

   //loop p/ todos Eqptos em lisEQP
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //obtém coordenadas do Eqpto
      coordenada = redegraf->CoordEqpto[eqpto];
      //if (coordenada->Line == NULL) continue;
      if (coordenada->Line)
         {//verifica se é um Trafo3E
         if (eqpto->Tipo() == eqptoTRAFO3E)
            {//desenha moldura na área em torno do Trafo
            //form->DrawMolduraCirculo(coordenada->Line->xm, coordenada->Line->ym, pmXor);
            x1 = min(min(coordenada->Line->x1, coordenada->Line->x2), coordenada->Line->x3);
            y1 = min(min(coordenada->Line->y1, coordenada->Line->y2), coordenada->Line->y3);
            x2 = max(max(coordenada->Line->x1, coordenada->Line->x2), coordenada->Line->x3);
            y2 = max(max(coordenada->Line->y1, coordenada->Line->y2), coordenada->Line->y3);
            form->DrawMolduraArea(x1, y1, x2, y2, pmXor, clWhite);
            //atualiza gráfico
            //form->RedrawTelaReal();
            }
         else
            {//desenha moldura
            form->DrawMolduraLinha(coordenada->Line->x1, coordenada->Line->y1,
                                   coordenada->Line->x2, coordenada->Line->y2, pmXor, clWhite);
            //atualiza gráfico
            //form->RedrawTelaReal();
            }
         }
      else if (coordenada->Area)
            {//desenha moldura
            form->DrawMolduraArea(coordenada->Area->x1, coordenada->Area->y1,
                                  coordenada->Area->x2, coordenada->Area->y2, pmXor, clWhite);
            //atualiza gráfico
            //form->RedrawTelaReal();
            }
      else if (coordenada->Polyline)
         {
         }
      }
   //atualiza gráfico
   if (lisEQP->Count) form->RedrawTelaReal();
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::MolduraSolida(TList *lisEQP, TColor color)
   {
   //variáveis locais
   VTEqpto      *eqpto;
   VTCoordenada *coordenada;

   //loop p/ todos Eqptos em lisEQP
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //obtém coordenadas do Eqpto
      coordenada = redegraf->CoordEqpto[eqpto];
      if (coordenada->Line == NULL) continue;
      if (coordenada->Line)
         {//verifica se é um Trafo3E
         if (eqpto->Tipo() == eqptoTRAFO3E)
            {//desenha moldura
            form->DrawMolduraCirculo(coordenada->Line->xm, coordenada->Line->ym, pmCopy, color);
            //atualiza gráfico
            form->RedrawTelaReal();
            }
         else
            {//desenha moldura
            form->DrawMolduraLinha(coordenada->Line->x1, coordenada->Line->y1,
                                   coordenada->Line->x2, coordenada->Line->y2, pmCopy, color);
            //atualiza gráfico
            form->RedrawTelaReal();
            }
         }
      else if (coordenada->Area)
            {//desenha moldura
            form->DrawMolduraArea(coordenada->Area->x1, coordenada->Area->y1,
                                  coordenada->Area->x2, coordenada->Area->y2, pmCopy, color);
            //atualiza gráfico
            form->RedrawTelaReal();
            }
      else if (coordenada->Polyline)
         {
         }
      }
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TUnif::PM_GetNome(void)
   {
   return(nome);
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::PM_SetAnimacaoEnabled(bool enabled)
   {
   //verifica se está desabilitando a animação
   if (! enabled)
      {//verifica se deva apagar círculos atualmente exibidos
      if (redraw_animacao.apagar)
         {//apaga círculos atualmente exibidos
         RedrawAnimacao();
         //atualiza gráfico
         form->RedrawTelaReal();
         }
      //reinicia controle da animação
      redraw_animacao.apagar  = false;
      redraw_animacao.postre  = 2;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::PM_SetMapaTematicoEnabled(bool enabled)
   {
   mapa_tematico.enabled = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::RedrawAll(void)
   {
   //variáveis locais
   VTRede *rede;
   bool    chave_enabled = false;

   //atualiza ponteiros
   edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
   redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //define cor para Eqptos isolados no gráfico de topologia
   rede = redes->RedeIsolada();
   rede->Color = padrao->isolado.cor;
   //define cor para Eqptos isolados no gráfico de fluxo
   geral->COR.Faixa[diagISOLADO] = padrao->isolado.cor;
   //limpa a tela
   LimpaTela();
   //verifica se deve habilitar exibição de bitmap e legenda
   HabilitaLegenda();
   //FKM 2016.01.06 - exibe Regioes
   RedrawRegiao();
   //exibe grid
   RedrawGrid();
   //apresenta mapa temático
   RedrawMapaTematico();
   //desenha Ligacoes, exceto as chaves que serão desenhadas depois das barras
   //RedrawAllLigacaoIsolada();
   RedrawAllLigacao(chave_enabled);
   //desenha Eqbar
   //RedrawAllEqbarIsolado();
   RedrawAllEqbar();
   //desenha Mutuas
   RedrawAllMutua();
   //desenha eqptos das redes reduzidas
   RedrawAllRedeReduzida();
   //desenha Barras
   RedrawAllBarra();
   //desenha as Chaves
   RedrawAllChave();
   //RedrawAllBarraIsolada();
   //exibe Regioes
   //RedrawRegiao();
   //exibe figuras
   RedrawAllFigura();
   }

/*
//---------------------------------------------------------------------------
void __fastcall TUnif::Redraw(int op, VTEdt *editor)
   {
   //salva ponteiro para objeto
   this->editor = editor;
   //não utilizado nesta classe
   Redraw();
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllBarra(void)
   {
   //variáveis locais
   TColor   cor;
   VTBarra *pbarra;
   TList   *lisBAR;

   //determina lista de Barras das Redes
   if ((lisBAR = LisEqptoRede(eqptoBARRA)) != NULL)
      {//loop p/ todas as Barras
      for (int nbar = 0; nbar < lisBAR->Count; nbar++)
         {
         pbarra = (VTBarra*)lisBAR->Items[nbar];
         //verifica se é uma Barra isolada
         if (pbarra->Status[sttISOLADO])
            {//assume cor de eqpto isolado
            cor = padrao->isolado.cor;
            }
         else
            {//verifica se a Barra possui Eqbar
            //cor = (pbarra->NumeroDeEqbar() == 0) ? padrao->barra.cor: padrao->barra.cor_eqbar;
            cor = padrao->barra.cor;
				}
         RedrawBarra(pbarra, Color(pbarra, cor));
         //identifica Barra swing
         //if (pbarra->Swing()) RedrawBarraSwing(pbarra);
         }
      }
   }

/*
//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllBarraIsolada(void)
   {

ALTERAR:Incluir_barras_da_RedeIsolada_nas_Celulas

   //variáveis locais
   VTBarra *pbarra;
   VTRede  *rede;
   TList   *lisBAR;

   //determina lista de Barras isoladas (não ligadas por nenhuma Ligacao)
   lisBAR = redes->LisBarraIsolada();
   for (int nbar = 0; nbar < lisBAR->Count; nbar++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nbar];
      RedrawBarra(pbarra, Color(pbarra, padrao->isolado.cor));
      }
   //determina lista de Barras da Rede isolada
   rede = redes->RedeIsolada();
   lisBAR = rede->LisBarra();
   //loop p/ todas as Barras
   for (int nbar = 0; nbar < lisBAR->Count; nbar++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nbar];
      RedrawBarra(pbarra, Color(pbarra, padrao->isolado.cor));
      }
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllBateria(void)
   {
   //variáveis locais
   VTBateria  *pbateria;
   TList      *lisBAT;

   //determina lista de Baterias
   if ((lisBAT = LisEqptoRede(eqptoBATERIA)) != NULL)
	  {//loop p/ todos Baterias
	  for (int nb = 0; nb < lisBAT->Count; nb++)
         {
		 pbateria = (VTBateria*)lisBAT->Items[nb];
         //verifica se deve exibir o Capacitor
		 if (! pbateria->Show[eqp_VISIBLE]) continue;
		 RedrawBateria(pbateria, Color(pbateria, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllCapacitor(void)
   {
   //variáveis locais
   VTCapacitor *pcap;
   TList      *lisCAP;

   //determina lista de Capacitores
   if ((lisCAP = LisEqptoRede(eqptoCAPACITOR)) != NULL)
      {//loop p/ todos Capacitores
      for (int nc = 0; nc < lisCAP->Count; nc++)
         {
         pcap = (VTCapacitor*)lisCAP->Items[nc];
         //verifica se deve exibir o Capacitor
         if (! pcap->Show[eqp_VISIBLE]) continue;
         RedrawCapacitor(pcap, Color(pcap, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllCapserie(void)
   {
   //variáveis locais
   VTCapserie *capserie;
   TList      *lisEQP;

   //determina lista de Capserie
   if ((lisEQP = LisEqptoRede(eqptoCAPSERIE)) != NULL)
      {//loop p/ todos Capserie
      for (int n = 0; n < lisEQP->Count; n++)
         {
         capserie = (VTCapserie*)lisEQP->Items[n];
         //verifica se deve exibir eqpto
         if (! capserie->Show[eqp_VISIBLE]) continue;
         RedrawCapserie(capserie, Color(capserie, capserie->rede->Color));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllCarga(void)
   {
   //variáveis locais
   VTCarga   *pcarga;
   TList     *lisCAR;

   //determina lista de Cargas
   if ((lisCAR = LisEqptoRede(eqptoCARGA)) != NULL)
      {//loop p/ todas Cargas
      for (int nc = 0; nc < lisCAR->Count; nc++)
         {
         pcarga = (VTCarga*)lisCAR->Items[nc];
         //verifica se deve exibir as Cargas
         if (! pcarga->Show[eqp_VISIBLE]) continue;
         RedrawCarga(pcarga, Color(pcarga, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllCargaNL(void)
   {
   //variáveis locais
   VTCargaNL   *cargaNL;
   TList       *lisCARNL;

   //determina lista de Cargas
   if ((lisCARNL = LisEqptoRede(eqptoCARGANL)) != NULL)
      {//loop p/ todas Cargas
      for (int nc = 0; nc < lisCARNL->Count; nc++)
         {
         cargaNL = (VTCargaNL*)lisCARNL->Items[nc];
         //verifica se deve exibir as Cargas
         if (! cargaNL->Show[eqp_VISIBLE]) continue;
         RedrawCargaNL(cargaNL, Color(cargaNL, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllChave(void)
   {
   //variáveis locais
   VTChave *pchave;
   TList   *lisCHV;
   TColor  cor;

   //determina lista de Chaves
   if ((lisCHV = LisEqptoRede(eqptoCHAVE)) != NULL)
      {//loop p/ todas as Chaves
      for (int nchv = 0; nchv < lisCHV->Count; nchv++)
         {
         pchave = (VTChave*)lisCHV->Items[nchv];
         //cor    = (pchave->rede) ? pchave->rede->Color : padrao->isolado.cor;
         //verifica se deve exibir a Chave com a cor de TipoChave ou da Rede
         if      (padrao->chave.op_cor == 0)  cor = pchave->TipoChave->Color;
         else if (pchave->rede != NULL)       cor = pchave->rede->Color;
         else                                 cor = padrao->isolado.cor;
         //verifica se cor selecionada é igual à cor de fundo
         if (cor == redegraf->CorFundo)
            {
            if (cor == clBlack) cor = clWhite;
            else                cor = clBlack;
            }
         RedrawChave(pchave, Color(pchave, cor));
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllEqbar(void)
   {
   RedrawAllCapacitor();
   RedrawAllCarga();
   RedrawAllCargaNL();
   RedrawAllGerador();
   RedrawAllFiltro();
   RedrawAllMedidor();
   RedrawAllMotor();
   RedrawAllReator();
   RedrawAllSuprimento();
   RedrawAllTrafoZZ();
   RedrawAllZref();
   RedrawAllBateria();
   }
/*
//---------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllEqbarIsolado(void)
   {
   //variáveis locais
   VTEqpto *eqpto;
   TList   *lisEQP;

   //determina lista de Eqbar da Rede isolada
   lisEQP = consulta->LisEqpto(redes->RedeIsolada(), eqptoEQBAR);
   //loop p/ todos Eqbar
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      switch(eqpto->Tipo())
         {
         case eqptoCAPACITOR:  RedrawCapacitor((VTCapacitor*)eqpto,   padrao->isolado.cor); break;
         case eqptoCARGA:      RedrawCarga((VTCarga*)eqpto,           padrao->isolado.cor); break;
         case eqptoGERADOR:    RedrawGerador((VTGerador*)eqpto,       padrao->isolado.cor); break;
         case eqptoFILTRO:     RedrawFiltro((VTFiltro*)eqpto,         padrao->isolado.cor); break;
         case eqptoREATOR:     RedrawReator((VTReator*)eqpto,         padrao->isolado.cor); break;
         case eqptoSUPRIMENTO: RedrawSuprimento((VTSuprimento*)eqpto, padrao->isolado.cor); break;
         case eqptoTRAFOZZ:    RedrawTrafoZZ((VTTrafoZZ*)eqpto,       padrao->isolado.cor); break;
         default: break;
         }
      }
   }
*/
//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16228677
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TUnif::RedrawAllFigura(void)
   {
   //variáveis locais
   VTFiguraComentario *comentario;
   VTRetangulo  *retangulo;
   VTFiguras    *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   TList        *lisFIG;

   //desenha Comentarios
   if(figuras == NULL)
   {return;}
   lisFIG  = figuras->LisComentario();
   for (int n = 0; n < lisFIG->Count; n++)
      {
      comentario = (VTFiguraComentario*)lisFIG->Items[n];
      //desenha Comentario
	  draw->FigComentario(comentario, comentario->Retangulo->FundoCor);
      }
   //desenha Retangulos
   lisFIG  = figuras->LisRetangulo();
   for (int n = 0; n < lisFIG->Count; n++)
      {
      retangulo = (VTRetangulo*)lisFIG->Items[n];
      //desenha Comentario
      draw->FigRetangulo(retangulo, retangulo->FundoCor);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllFiltro(void)
   {
   //variáveis locais
   VTFiltro *filtro;
   TList    *lisEQP;

   //determina lista de Filtro
   if ((lisEQP = LisEqptoRede(eqptoFILTRO)) != NULL)
      {//loop p/ todos Filtro
      for (int n = 0; n < lisEQP->Count; n++)
         {
         filtro = (VTFiltro*)lisEQP->Items[n];
         //verifica se deve exibir eqpto
         if (! filtro->Show[eqp_VISIBLE]) continue;
         RedrawFiltro(filtro, Color(filtro, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllGerador(void)
   {
   //variáveis locais
   VTGerador *pger;
   TList     *lisGER;

   //determina lista de Geradores
   if ((lisGER = LisEqptoRede(eqptoGERADOR)) != NULL)
      {//loop p/ todos Geradores
      for (int n = 0; n < lisGER->Count; n++)
         {
         pger = (VTGerador*)lisGER->Items[n];
         //verifica se deve exibir os Geradores
         if (! pger->Show[eqp_VISIBLE]) continue;
         RedrawGerador(pger, Color(pger, clWhite));
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllLigacao(bool chave_enabled)
   {
   if (chave_enabled) RedrawAllChave();
   RedrawAllCapserie();
   RedrawAllRegulador();
   RedrawAllTrafo();
   RedrawAllTrafo3E();
   RedrawAllTrecho();
   }
/*
//---------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllLigacaoIsolada(void)
   {
   //variáveis locais
   VTEqpto *eqpto;
   TList  *lisEQP;

   //determina lista de Ligacao da Rede isolada
   lisEQP = consulta->LisEqpto(redes->RedeIsolada(), eqptoLIGACAO);
   //loop p/ todas Ligacoes
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      switch(eqpto->Tipo())
         {
         case eqptoCHAVE:   RedrawChave((VTChave*)eqpto,     padrao->isolado.cor); break;
         case eqptoTRAFO:   RedrawTrafo((VTTrafo*)eqpto,     padrao->isolado.cor); break;
         case eqptoTRAFO3E: RedrawTrafo3E((VTTrafo3E*)eqpto, padrao->isolado.cor); break;
         case eqptoTRECHO:  RedrawTrecho((VTTrecho*)eqpto,   padrao->isolado.cor); break;
         default: break;
         }
      }
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllMedidor(void)
   {
   //variáveis locais
   VTMedidor *medidor;
   TList     *lisEQP;

   //determina lista de Medidor
   if ((lisEQP = LisEqptoRede(eqptoMEDIDOR)) != NULL)
      {//loop p/ todos Medidor
      for (int n = 0; n < lisEQP->Count; n++)
         {
         medidor = (VTMedidor*)lisEQP->Items[n];
         //verifica se deve exibir eqpto
         if (! medidor->Show[eqp_VISIBLE]) continue;
         RedrawMedidor(medidor, Color(medidor, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllMotor(void)
   {
   //variáveis locais
   VTMotor   *motor;
   TList     *lisEQP;

   //determina lista de motor
   if ((lisEQP = LisEqptoRede(eqptoMOTOR)) != NULL)
      {//loop p/ todos motor
      for (int n = 0; n < lisEQP->Count; n++)
         {
         motor = (VTMotor*)lisEQP->Items[n];
         //verifica se deve exibir eqpto
         if (! motor->Show[eqp_VISIBLE]) continue;
         RedrawMotor(motor, Color(motor, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllMutua(void)
   {
   //variáveis locais
   VTMutua *mutua;
   TList   *lisEQP;

   //determina lista de Mutua
   if ((lisEQP = LisEqptoRede(eqptoMUTUA)) != NULL)
      {//loop p/ todos Mutua
      for (int n = 0; n < lisEQP->Count; n++)
         {
         mutua = (VTMutua*)lisEQP->Items[n];
         //verifica se deve exibir eqpto
         if (! mutua->Show[eqp_VISIBLE]) continue;
         RedrawMutua(mutua, Color(mutua, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllReator(void)
   {
   //variáveis locais
   VTReator *reator;
   TList   *lisEQP;

   //determina lista de Reator
   if ((lisEQP = LisEqptoRede(eqptoREATOR)) != NULL)
      {//loop p/ todos Reator
      for (int n = 0; n < lisEQP->Count; n++)
         {
         reator = (VTReator*)lisEQP->Items[n];
         //verifica se deve exibir eqpto
         if (! reator->Show[eqp_VISIBLE]) continue;
         RedrawReator(reator, Color(reator, clWhite));
         }
      }
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllRedeReduzida(void)
   {
   //variáveis locais
   VTMRede   *mrede;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisMR   = redes->LisMRede();
   TList     *lisREDE = redes->LisRede();

   //loop p/ todas MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //OBS: uma MRede não carregada possui uma única Reducao
      reducao = (VTReducao*)((mrede->LisReducao())->First());
      //verifica se deve exibir a Reducao
      if (! reducao->Visible()) continue;
      //exibe Reducao
      RedrawReducao(reducao, mrede->Color);
      }
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllRedeReduzida(void)
   {
   //variáveis locais
   TList     *lisREDUCAO;
   VTMRede   *mrede;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisMR   = redes->LisMRede();

   //loop p/ todas MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //OBS: uma MRede não carregada possui uma única Reducao
      lisREDUCAO = mrede->LisReducao();
      if (lisREDUCAO->Count == 0) continue;
      reducao = (VTReducao*)(lisREDUCAO->First());
      //verifica se deve exibir a Reducao
      if (! reducao->Visible()) continue;
      //exibe Reducao
      RedrawReducao(reducao, mrede->Color);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllRegulador(void)
   {
   //variáveis locais
   VTRegulador *regulador;
   TList       *lisEQP;

   //determina lista de Regulador
   if ((lisEQP = LisEqptoRede(eqptoREGULADOR)) != NULL)
      {//loop p/ todos Regulador
      for (int n = 0; n < lisEQP->Count; n++)
         {
         regulador = (VTRegulador*)lisEQP->Items[n];
         //verifica se deve exibir eqpto
         if (! regulador->Show[eqp_VISIBLE]) continue;
         RedrawRegulador(regulador, Color(regulador, regulador->rede->Color));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllSuprimento(void)
   {
   //variáveis locais
   VTSuprimento *psup;
   TList        *lisSUP;

   //determina lista de Suprimentos
   if ((lisSUP = LisEqptoRede(eqptoSUPRIMENTO)) != NULL)
      {//loop p/ todos Suprimentos
      for (int n = 0; n < lisSUP->Count; n++)
         {
         psup = (VTSuprimento*)lisSUP->Items[n];
         //verifica se deve exibir os Suprimentos
         if (! psup->Show[eqp_VISIBLE]) continue;
         RedrawSuprimento(psup, Color(psup, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllTrafo(void)
   {
   //variáveis locais
   VTTrafo *ptrafo;
   TList   *lisTRF;

   //determina lista de Trafos
   if ((lisTRF = LisEqptoRede(eqptoTRAFO)) != NULL)
      {//loop p/ todos Trafos
      for (int ntrf = 0; ntrf < lisTRF->Count; ntrf++)
         {
         ptrafo = (VTTrafo*)lisTRF->Items[ntrf];
         RedrawTrafo(ptrafo, Color(ptrafo, ptrafo->rede->Color));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllTrafo3E(void)
   {
   //variáveis locais
   VTTrafo3E *ptrf3e;
   TList     *lisT3E;

   //determina lista de Trafo3E
   if ((lisT3E = LisEqptoRede(eqptoTRAFO3E)) != NULL)
      {//loop p/ todas os Trafo3E
      for (int ntrf = 0; ntrf < lisT3E->Count; ntrf++)
         {
         ptrf3e = (VTTrafo3E*)lisT3E->Items[ntrf];
         RedrawTrafo3E(ptrf3e, Color(ptrf3e, ptrf3e->rede->Color));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllTrafoZZ(void)
   {
   //variáveis locais
   VTTrafoZZ *trafoZZ;
   TList     *lisEQP;

   //determina lista de TrafoZZ
   if ((lisEQP = LisEqptoRede(eqptoTRAFOZZ)) != NULL)
      {//loop p/ todos TrafoZZ
      for (int n = 0; n < lisEQP->Count; n++)
         {
         trafoZZ = (VTTrafoZZ*)lisEQP->Items[n];
         //verifica se deve exibir eqpto
         if (! trafoZZ->Show[eqp_VISIBLE]) continue;
         RedrawTrafoZZ(trafoZZ, Color(trafoZZ, clWhite));
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllTrecho(void)
   {
   //variáveis locais
   VTTrecho *ptrecho;
   TList    *lisTRE;

   //determina lista de Trechos
   if ((lisTRE = LisEqptoRede(eqptoTRECHO)) != NULL)
      {//loop p/ todos Trechos
      for (int ntre = 0; ntre < lisTRE->Count; ntre++)
         {
         ptrecho = (VTTrecho*)lisTRE->Items[ntre];
         //RedrawTrecho(ptrecho, Color(ptrecho, padrao->tre.cor));
         RedrawTrecho(ptrecho, Color(ptrecho, ptrecho->rede->Color));
         }
      }
   }

/*
//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllTrecho(void)
   {
   //variáveis locais
   VTTrecho *ptrecho;
   VTRede   *rede;
   TList    *lisTRE;
   TList    *lisREDE = redes->LisRede();

   //loop p/ toda VTRede
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //verifica se VTRede está visível
      if (! rede->Visible()) continue;
      //determina lista de Trechos
      if ((lisTRE = LisEqptoRede(rede, eqptoTRECHO)) != NULL)
         {//loop p/ todos Trechos
         for (int ntre = 0; ntre < lisTRE->Count; ntre++)
            {
            ptrecho = (VTTrecho*)lisTRE->Items[ntre];
            //RedrawTrecho(ptrecho, Color(ptrecho, padrao->tre.cor));
            RedrawTrecho(ptrecho, Color(ptrecho, ptrecho->rede->Color));
            }
         }
      //atualiza gráfico
      //form->RedrawTelaReal();
      }
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawAllZref(void)
   {
   //variáveis locais
   VTYref *yref;
   TList  *lisEQP;

   //determina lista de Zref
   if ((lisEQP = LisEqptoRede(eqptoYREF)) != NULL)
      {//loop p/ todos Zref
      for (int n = 0; n < lisEQP->Count; n++)
         {
         yref = (VTYref*)lisEQP->Items[n];
         //verifica se deve exibir eqpto
         if (! yref->Show[eqp_VISIBLE]) continue;
         RedrawYref(yref, Color(yref, clWhite));
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TUnif::RedrawAnimacao(void)
   {
   //método a ser implemantado nas classes derivadas
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawBarra(VTBarra *pbar, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(pbar)) return;
   draw->Barra(pbar, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawBarraSwing(VTBarra *pbar)
   {
   /*
   //IMPORTANTE: barra swing não faz sentido em FlowGauss
   //variáveis locais
   int x1, y1, x2, y2;

   //determina coordenadas da Barra
   pbar->CoordenadasEsquematico(x1, y1, x2, y2);
   grid->SnapToGrid(x1, y1, x2, y2);
   //verifica se é uma Barra definida como swing
   if (pbar->Swing())
      {//barra definida como swing
      RedrawCodigoDown(x2, y2, "swing", padrao->swing.cor);
      }
   else
      {//barra definida como swing
      RedrawCodigoDown(x2, y2, "swing (*)", padrao->swing.cor);
      }
   */
   }

//-----------------------------------------------------------------------------
// TUnif::RedrawBordas()   -  private

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawBateria(VTBateria *pbateria, TColor cor)
   {
   //controle de planejamento
   //if (! ValidaEqptoPlaneja(pbateria)) return; //não estamoss implementando as baterias no planejamento por enquanto
   draw->Bateria(pbateria, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawBordas(void)
   {
   //desenha borda
   //form->DrawBorda(padrao->cor_borda);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawCapacitor(VTCapacitor *pcap, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(pcap)) return;
   draw->Capacitor(pcap, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawCapserie(VTCapserie *capserie, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(capserie)) return;
   //desenha Chave
   draw->Capserie(capserie, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawCarga(VTCarga *pcar, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(pcar)) return;
   draw->Carga(pcar, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawCargaNL(VTCargaNL *cargaNL, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(cargaNL)) return;
   draw->CargaNL(cargaNL, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawChave(VTChave *pchave, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(pchave)) return;
   //desenha Chave
   draw->Chave(pchave, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawFiltro(VTFiltro *filtro, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(filtro)) return;
   draw->Filtro(filtro, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawGerador(VTGerador *pger, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(pger)) return;
   draw->Gerador(pger, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawGrid(void)
   {
   //variáveis locais
   int    x1, y1, x2, y2;

   //verifica se deve apresentar o grid
   if (! grid->Visible()) return;
   if (grid->dim <= 0)    return;
   //determina área da rede visível da tela
   form->AreaVisivel(area);
   x1 = min(area->x1, area->x2);
   x2 = max(area->x1, area->x2);
   y1 = min(area->y1, area->y2);
   y2 = max(area->y1, area->y2);
   //ajusta as coordenadas iniciais p/ um valor menor, múltiplo da grid->dimensão da quadrícula
   x1 = (x1/grid->dim) * grid->dim;
   y1 = (y1/grid->dim) * grid->dim;
   //desenha linhas verticais da grade
   for (int x = x1; x <= x2; x += grid->dim)
      {
      form->DrawLinha(x, y1, x, y2, 1, grid->cor);
      }
   //desenha linhas horizontais da grade
   for (int y = y1; y <= y2; y += grid->dim)
      {
      form->DrawLinha(x1, y, x2, y, 1, grid->cor);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawMapaTematico(void)
   {
   //variáveis locais
   TColor    cor_fundo = redegraf->CorFundo;
   TList     *lisCEL;
   VTArea    *area;
   VTCelula  *celula;
   VTCelulas *celulas;

   //verifica se o mapa temático está habilitado
   if (! mapa_tematico.enabled) return;
   //obt ém lista de Celulas
   celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   lisCEL  = celulas->LisCelula();
   //loop p/ toda Celula
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      area   = redegraf->Area(celula);
      //verifica se está exibindo mapa
      if (form->MapaVisible())
         {//não desenha a Celula se ela tiver cor igual a cor de fundo
         if (celula->Color == cor_fundo) continue;
         //desenha retângulo da Celula com sua cor, de forma transparente p/ continuar mostrando o mapa
         draw->AreaTransparente(area, celula->Color);
         }
      else
         {//desenha retângulo da Celula com sua cor
         draw->Area(area, celula->Color);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawMedidor(VTMedidor *medidor, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(medidor)) return;
   draw->Medidor(medidor, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawMotor(VTMotor *motor, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(motor)) return;
   draw->Motor(motor, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawMutua(VTMutua *mutua, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(mutua)) return;
   draw->Mutua(mutua, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawReator(VTReator *reator, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(reator)) return;
   draw->Reator(reator, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawRedeEsquematico(VTRede *rede, TColor cor)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTLigacao *pliga;
   TList     *lisEQP;

   //obtém lista de todas Ligacoes do esquemático da Rede
   lisEQP = rede->LisLigacaoEsquematico();
   //loop p/ todos Ligacoes do esquemático
   for (int n = 0; n < lisEQP->Count; n++)
      {
      pliga = (VTLigacao*)lisEQP->Items[n];
      //desenha linha da Ligacao
      RedrawTrecho((VTTrecho*)pliga, Color(pliga, cor));
      }
   //exibe nome da Rede reduzida
   draw->RedeNome(rede->Codigo, rede->BarraFicticia(), pliga, rede->Color);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawReducao(VTReducao *reducao, TColor color)
   {
   //verifica se deve exibir esquemático ou Eqptos
   if (reducao->Show[rdu_EQPTOS])
      {//exibe Eqptos da Reducao
      RedrawReducaoEqptos(reducao, color);
      }
   else
      {//exibe esquemático  da Reducao
      RedrawReducaoEsquematico(reducao, color);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawReducaoEqptos(VTReducao *reducao, TColor cor)
   {
   //variáveis locais
   VTEqpto *eqpto;
   TList   *lisEQP;

   //obtém lista de todos objetos VEqpto dos objetos VTReducao visíveis
   lisEQP = LisEqptoReducao(reducao);
   //loop p/ todos objetos VEqpto
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      switch(eqpto->Tipo())
         {
         case eqptoBARRA:
            RedrawBarra((VTBarra*)eqpto, Color(eqpto, padrao->barra.cor));
            break;
         case eqptoCARGA:
            RedrawCarga((VTCarga*)eqpto, Color(eqpto, clWhite));
            break;
         case eqptoSUPRIMENTO:
            RedrawSuprimento((VTSuprimento*)eqpto, Color(eqpto, clWhite));
            break;
         case eqptoTRAFO:
            RedrawTrafo((VTTrafo*)eqpto, Color(eqpto, clWhite));
            break;
         case eqptoTRECHO:
            RedrawTrecho((VTTrecho*)eqpto, Color(eqpto, cor));
            break;
         case eqptoYREF:
            RedrawYref((VTYref*)eqpto, Color(eqpto, cor));
            break;
         }
      }
/*
   //determina lista de Barras das Reducoes
   if ((lisEQP = LisEqptoReducao(reducao, eqptoBARRA)) != NULL)
      {//loop p/ todas as Barras
      for (int nbar = 0; nbar < lisEQP->Count; nbar++)
         {
         pbarra = (VTBarra*)lisEQP->Items[nbar];
         RedrawBarra(pbarra, Color(pbarra, padrao->barra.cor));
         //identifica Barra swing
         //if (pbarra->Swing()) RedrawBarraSwing(pbarra);
         }
      }
*/
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawReducaoEsquematico(VTReducao *reducao, TColor cor)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTEqpto   *eqpto;
   VTTrecho  *trecho;
   TList     *lisEQP;

   //obtém lista de todas Ligacoes do esquemático das Reducoes visíveis
   //lisEQP = reducao->LisLigacaoEsquematico();
   lisEQP = LisEqptoReducao(reducao, eqptoTRECHO);
   if (lisEQP->Count == 0) return;
   //loop p/ todos objetos VEqpto
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() != eqptoTRECHO) continue;
      trecho = (VTTrecho*)eqpto;
      //desenha linha da Ligacao
      RedrawTrecho(trecho, Color(trecho, cor));
      }
   //exibe nome da Rede reduzida
   draw->RedeNome(reducao->Codigo, reducao->BarraFicticia(), (VTTrecho*)eqpto, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawRegiao(void)
   {
   //variáveis locais
   TList     *lisREGIAO;
   VTRegiao  *regiao;

   //proteção: o objeto VTRegioes pode não existir
   if (regioes == NULL) return;
   //loop p/ todos objetos VTRegiao
   lisREGIAO = regioes->LisRegiao();
   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      if (regiao->Visible) draw->Poligono(regiao->Poligono->LisPoint(), clBlack, 1, true, regiao->Color );
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawRegulador(VTRegulador *regulador, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(regulador)) return;
   draw->Regulador(regulador, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawSuprimento(VTSuprimento *psup, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(psup)) return;
   draw->Suprimento(psup, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawTrafo(VTTrafo *ptrf, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(ptrf)) return;
   draw->Trafo(ptrf, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawTrafo3E(VTTrafo3E *ptrf3e, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(ptrf3e)) return;
   draw->Trafo3E(ptrf3e, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawTrafoZZ(VTTrafoZZ *trafoZZ, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(trafoZZ)) return;
   draw->TrafoZZ(trafoZZ, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawTrecho(VTTrecho *ptre, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(ptre)) return;
   draw->Trecho(ptre, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnif::RedrawYref(VTYref *yref, TColor cor)
   {
   //controle de planejamento
   if (! ValidaEqptoPlaneja(yref)) return;
   draw->Yref(yref, cor);
   }

//-----------------------------------------------------------------------------
int __fastcall TUnif::Tipo(void)
   {
   return(tipo_graf);
   }

/*
//-----------------------------------------------------------------------------
bool __fastcall TUnif::ValidaEqptoPlaneja(VTEqpto *eqpto)
   {
   //verifica se o Eqpto está associado a uma Obra
   //if (eqpto->Obra == NULL) return(true);
   //verifica se Eqpto está associado a uma Obra associado ao objeto Obras
   if ((eqpto->Obra == NULL)||(eqpto->Obra->Obras == NULL)) return(true);
   //verifica se Obra está visível
   return(eqpto->Obra->Visible);
   }
*/

//-----------------------------------------------------------------------------
bool __fastcall TUnif::ValidaEqptoPlaneja(VTEqpto *eqpto)
   {
   //verifica se o Eqpto deve ser desenhado
   if (! eqpto->Show[eqp_VISIBLE]) return(false);
   //if (! eqpto->Status[sttREDRAW]) return(false);
   //Eqpto deve der desenhado: atualiza seu Statur p/ evitar que ele seja desenhado novamente
   //eqpto->Status[sttREDRAW] = false;
   return(true);
   }
   
//---------------------------------------------------------------------------
//eof


