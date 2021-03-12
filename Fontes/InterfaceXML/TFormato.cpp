//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormato.h"
#include "..\Apl\VTApl.h"
#include "..\Log\VTLog.h"

//-----------------------------------------------------------------------------
VTFormato* __fastcall NewObjFormato(VTApl *apl)
   {
   try{//cria objeto
      return(new TFormato(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }  
   }

//-----------------------------------------------------------------------------
__fastcall TFormato::TFormato(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   }

//-----------------------------------------------------------------------------
__fastcall TFormato::~TFormato(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::Executa(_di_IXMLTConcessionaria xmlConcessAux)
	{
	//vari�veis locais
	bool formato_ok;

	//salva ponteiro
	this->xmlConcessAux = xmlConcessAux;
	//inicia dados
	num_tab   = 0;
	num_erro  = 0;
	num_aviso = 0;
	//executa avalia��o
	formato_ok = ExecutaAvaliacao();
	//monta resumo do diagn�stico
	LogMsg(AnsiString::StringOfChar('-', 80));
	LogMsg("Resumo do diagn�stico do arquivo XML:");
	if (formato_ok) {LogMsg("> Estrutura XML correta");}
	else            {LogMsg("> Estrutura XML com erro de formato");}
	LogMsg("> Mensagens de Aviso: " + IntToStr(num_aviso));
	LogMsg("> Mensagens de Erro:  " + IntToStr(num_erro));
	LogMsg(AnsiString::StringOfChar('-', 80));
	//formato v�lido
	//if (num_erro  > 0) return(false);
	//if (num_aviso > 0) return(false);
	return(formato_ok);
	}

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ExecutaAvaliacao(void)
	{
	try{//valida conjuntos
		LogBlocoIni("An�lise do objeto TConcessionaria");
		if (! ValidaObjetoConcessionaria(xmlConcessAux))                     return(false);
		if (! ValidaConjuntoCaboMonopolar(xmlConcessAux->Get_CaboRGMR()))    return(false);
      if (! ValidaConjuntoCaboMultiplexado(xmlConcessAux->Get_CaboZ0Z1())) return(false);
		if (! ValidaConjuntoSuporte(xmlConcessAux->Get_Suporte()))           return(false);
      if (! ValidaConjuntoRede(xmlConcessAux->Get_Rede()))                 return(false);
		LogBlocoFim();
		}catch(Exception &e)
			{
         LogErro("na valida��o do formato do arquivo XML");
			return(false);
			}
	return(true);
	}
//-----------------------------------------------------------------------------
void __fastcall TFormato::LogAviso(AnsiString txt)
   {
   plog->Add(AnsiString::StringOfChar(' ', num_tab * 3) + "AVISO: " + txt);
   //incrementa nu�mero de avisos
   num_aviso++;
   }

//-----------------------------------------------------------------------------
void __fastcall TFormato::LogBlocoFim(void)
   {
   //decrementa num_tab
   num_tab--;
   plog->Add(AnsiString::StringOfChar(' ', num_tab * 3) + "}ok");
   }

//-----------------------------------------------------------------------------
void __fastcall TFormato::LogBlocoIni(AnsiString txt)
   {
	plog->Add(AnsiString::StringOfChar(' ', num_tab * 3) + "{" + txt);
   //incrementa num_tab
   num_tab++;
   }

//-----------------------------------------------------------------------------
void __fastcall TFormato::LogErro(AnsiString txt)
   {
   LogMsg(AnsiString::StringOfChar('-', 80));
   plog->Add(AnsiString::StringOfChar(' ', num_tab * 3) + "ERRO: " + txt);
   LogMsg(AnsiString::StringOfChar('-', 80));
   //incrementa nu�mero de avisos
   num_erro++;
   }

//-----------------------------------------------------------------------------
void __fastcall TFormato::LogMsg(AnsiString txt)
   {
   plog->Add(AnsiString::StringOfChar(' ', num_tab * 3) + txt);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoCaboMonopolar(_di_IXMLTCaboRGMRList XMLCaboRGMRList)
   {
   //vari�veis locais
   _di_IXMLTCaboRGMR    XMLCaboRGMR;

   try{
      LogBlocoIni("An�lise do objeto TCaboRGMRList");
      //valida XMLCaboRGMRList
      if (XMLCaboRGMRList == NULL)
         {
         LogAviso("Objeto TCaboRGMRList n�o definido");
         return(true);
         }
      if (XMLCaboRGMRList->Count == 0 )
         {
         LogMsg("Objeto TCaboRGMRList sem nenhum objeto TCaboRGMR");
         }
      else
         {//indica n�mero de cabos definidos
         LogMsg("Objeto TCaboRGMRList com " + IntToStr(XMLCaboRGMRList->Count) + " objetos TCaboRGMR");
         }
      //loop p/ conjunto de Cabos
      for (int i = 0; i < XMLCaboRGMRList->Count; i++)
         {
         LogBlocoIni("An�lise do objeto TCaboRGMR: " + IntToStr(i+1));
         XMLCaboRGMR =  XMLCaboRGMRList->Get_Item(i);
         if (! ValidaObjetoCaboRGMR(XMLCaboRGMR)) return(false);
         LogBlocoFim();
         }
      LogBlocoFim();
      }catch(Exception &e)
         {
         LogErro("na valida��o do objeto TCaboRGMRList");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoCaboMultiplexado(_di_IXMLTCaboZ0Z1List XMLCaboZ0Z1List)
   {
   //vari�veis locais
   _di_IXMLTCaboZ0Z1 XMLCaboZ0Z1;
   _di_IXMLTId       xmlId;          // Id do equipamento
   _di_IXMLTCSeq     xmlCSeq;        // Capacit�ncia de seq��ncia
   _di_IXMLTZseq     xmlZSeq;        // Imped�ncia de seq��ncia
   _di_IXMLTZ        xmlZ;           // Imped�ncia

   try{
      LogBlocoIni("An�lise do objeto TCaboZ0Z1List");
      //valida ponteiro XMLCaboZ0Z1List
      if (XMLCaboZ0Z1List == NULL)
         {
         LogAviso("Objeto TCaboZ0Z1List n�o definido");
         return(true);
         }
      if (XMLCaboZ0Z1List->Count == 0 )
         {
			LogMsg("Objeto TCaboZ0Z1List sem nenhum objeto TCaboZ0Z1");
         }
      else
         {//indica n�mero de cabos definidos
         LogMsg("Objeto TCaboZ0Z1List com " + IntToStr(XMLCaboZ0Z1List->Count) + " objetos TCaboZ0Z1");
         }
      //loop p/ conjunto de Cabos
      for (int i = 0; i < XMLCaboZ0Z1List->Count; i++)
         {
         LogBlocoIni("An�lise do objeto TCaboZ0Z1: " + IntToStr(i+1));
         XMLCaboZ0Z1 =  XMLCaboZ0Z1List->Get_Item(i);
         if (! ValidaObjetoCaboZ0Z1(XMLCaboZ0Z1)) return(false);
         LogBlocoFim();
         }
      LogBlocoFim();
      }catch(Exception &e)
         {
         LogErro("na do objeto TCaboZ0Z1List");
         return(false);
         }

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoCapacitor(_di_IXMLTEqBarCapacitorList lstCapacitor)
   {
   //vari�veis locais
   _di_IXMLTEqBarCapacitor xmlCapacitor;   // Capacitor

   try{//verifica o ponteiro lstCapacitor
      if (lstCapacitor == NULL)
         {
         LogAviso("Objeto TEqBarCapacitorList n�o definido");
         return(true);
         }
      //indica n�mero de objetos
      LogMsg("Objeto TEqBarCapacitorList com " + IntToStr(lstCapacitor->Count) + " objetos TEqBarCapacitor");
      //loop p/ todos objetos
      for (int i = 0; i < lstCapacitor->Count; i++)
         {
         LogBlocoIni("An�lise do objeto TEqBarCapacitor: " + IntToStr(i+1));
         if ((xmlCapacitor = lstCapacitor->Get_Item(i)) == NULL)
            {
            LogErro("Objeto TEqBarCapacitor n�o definido");
            return(false);
            }
			if (! ValidaObjetoEqpto(xmlCapacitor->Get_eqbar(), 1)) return(false);
         //acessa atributos
			try{
            xmlCapacitor->Get_dblTnsNom();
            }catch(Exception &e)
               {
               LogAviso("Objeto TEqBarCapacitor: atributo dblTnsNom n�o definido");
               xmlCapacitor->Set_dblTnsNom(13.8);
               //return(false);
               }
         try{
            xmlCapacitor->Get_dblPotRtv();
            }catch(Exception &e)
               {
               LogAviso("Objeto TEqBarCapacitor: atributo dblPotRtv n�o definido");
               xmlCapacitor->Set_dblPotRtv(300);
               //return(false);
               }
			LogBlocoFim();
         }
      }catch(Exception &e)
         {
         LogErro("na valida��o do objeto TEqBarCapacitorList");
         return(false);
         }

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato:: ValidaConjuntoCapserie(_di_IXMLTEqLigCapacitorSerieList lstCapacitorSerie)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoCarga(_di_IXMLTEqBarCargaList lstCarga)
   {
   //vari�veis locais
   AnsiString            atrib_str;
   _di_IXMLTEqBarCarga   xmlCarga;       // Carga (T�pica ou Especial)
   _di_IXMLTPatamPropList lstPatamar;     // Lista de patamares

   try{//valida o ponteiro
      if (lstCarga == NULL)
         {
         LogAviso("Objeto TEqBarCargaList n�o definido");
         return(true);
         }
      //indica n�mero de objetos
      LogMsg("Objeto TEqBarCargaList com " + IntToStr(lstCarga->Count) + " objetos TEqBarCargar");
      //loop p/ todas Cargas
      for (int i = 0; i < lstCarga->Count; i++)
         {
         LogBlocoIni("An�lise do objeto TEqBarCargar: " + IntToStr(i+1));
			if ((xmlCarga = lstCarga->Get_Item(i)) == NULL)
				{
				LogErro("Objeto TEqBarCargar n�o definido");
				return(false);
				}
			if (! ValidaObjetoEqpto(xmlCarga->Get_eqbar(), 1)) return(false);
			// As curvas s�o obrigat�rias na carga espec�fica, por�m n�o na t�pica
			if ((lstPatamar = xmlCarga->Get_Patamar()) == NULL)
				{
				LogAviso("Objeto TPatamarList n�o definido");
				}
			else
				{
				LogMsg("Objeto TPatamarList com " + IntToStr(lstPatamar->Count) + " objetos TPatamar");
				//analisa objetos Patamar
				for (int j = 0; j < lstPatamar->Count; j++)
					{
					LogBlocoIni("An�lise do objeto TPatamar " + IntToStr(j+1));
					if (! ValidaObjetoPatamar(lstPatamar->Get_Item(j))) return(false);
					LogBlocoFim();
					}
				}
			//acessa atributos
			xmlCarga->Get_intTpoModCrg();
			xmlCarga->Get_intTpoCons();
			xmlCarga->Get_dblEne();
			atrib_str = xmlCarga->Get_strFases();
         if (atrib_str.IsEmpty())
           {
           LogErro("Objeto TEqBarCargar sem fase definida");
           return(false);
         }
			LogBlocoFim();
         }
      }catch(Exception &e)
         {
         LogErro("na valida��o do objeto TEqBarCargaList");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoCargaIP(_di_IXMLTEqBarCargaIPList lstCargaIP)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoChave(_di_IXMLTEqLigChaveList lstChave)
   {
   //vari�veis locais
   _di_IXMLTEqLigChave xmlChave;

   try{//valida o ponteiro
      if (lstChave == NULL)
         {
         LogAviso("Objeto TEqLigChaveList n�o definido");
         return(true);
         }
      //indica n�mero de objetos
      LogMsg("Objeto TEqLigChaveList com " + IntToStr(lstChave->Count) + " objetos TEqLigChave");
      //loop p/ todos objetos
      for (int i = 0; i < lstChave->Count; i++)
         {
         LogBlocoIni("An�lise do objeto TEqLigChave: " + IntToStr(i+1));
         if ((xmlChave = lstChave->Get_Item(i)) == NULL)
            {
            LogErro("Objeto TEqLigChave n�o definido");
            return(false);
				}
			if (! ValidaObjetoEqpto(xmlChave->Get_eqlig(), 2)) return(false);
			LogBlocoFim();
         }
      }catch(Exception &e)
         {
         LogErro("na valida��o do objeto TEqLigChaveList");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoFiltro(_di_IXMLTEqBarFiltroList lstFiltro)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoGerador(_di_IXMLTEqBarGeradorList lstGerador)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoMedicao(_di_IXMLTMedicaoList cnjMedicao)
	{
	//vari�veis locais
	_di_IXMLTMedicao    xmlMedicao;     // Medicao (Medidor e Canal)

	try{//valida o ponteiro
		if (cnjMedicao == NULL)
			{//� normal n�o existir medi��o p/ os equipamentos
			//LogAviso("Objeto TMedicaoList n�o definido");
			return(true);
			}
		//indica n�mero de objetos
		LogMsg("Objeto TMedicaoList com " + IntToStr(cnjMedicao->Count) + " objetos TMedicao");
		//loop p/ todas Medicao
		for (int i = 0; i < cnjMedicao->Count; i++)
			{
			LogBlocoIni("An�lise do obejto TMedicao [" + IntToStr(i+1) +  "]");
			ValidaObjetoMedicao(cnjMedicao->Get_Item(i));
			LogBlocoFim();
			}
		}catch(Exception &e)
			{
			LogErro("na valida��o do objeto TMedicaoList");
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoMedidor(_di_IXMLTEqBarMedidorList lstMedidor)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoMutua(_di_IXMLTMutuaList lstMutua)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoReator(_di_IXMLTEqBarReatorList lstReator)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoRede(_di_IXMLTRedeList XMLRedeList)
   {
   //vari�veis locais
   _di_IXMLTRede    XMLRede;

	try{
		LogBlocoIni("An�lise do objeto TRedeList");
		//obt�m objeto XMLRedeList
		if (XMLRedeList == NULL)
			{
			LogAviso("Arquivo sem objeto TRedeList");
			return(true);
			}
		if (XMLRedeList->Count == 0 )
			{
			LogAviso("Objeto TRedeList sem nenhum objeto TRede");
			}
		else
			{//indica n�mero de cabos definidos
			LogMsg("Objeto TRedeList com " + IntToStr(XMLRedeList->Count) + " objetos TRede");
			}
		//loop p/ conjunto
		for (int i = 0; i < XMLRedeList->Count; i++)
			{
			XMLRede = XMLRedeList->Get_Item(i);
			LogBlocoIni("An�lise da rede [" + IntToStr(i+1) +  "] e de seus equipamentos");
			if (! ValidaObjetoRede(XMLRede)) return(false);
			LogBlocoFim();

         // unck@20100527
         // ATEN��O Guaraldo: limpeza geral de mem�ria. Libera o espa�o ocupado pela rede
         // Isso parte do princ�pio que uma rede sempre cabe inteira na mem�ria
         // Se houver uma situa��o em que n�o couber, essa limpeza precisa passar para
         // dentro da rede, eliminando listas parciais de EqBar e EqLig
         XMLRede->ChildNodes->Clear();
         XMLRede = NULL;

			}
		LogBlocoFim();
		}catch(Exception &e)
			{
			LogErro("na valida��o do objeto TRedeList");
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoRegulador(_di_IXMLTEqLigReguladorList lstRegulador)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoSuporte(_di_IXMLTSuporteList XMLSuporteList)
	{
	//vari�veis locais
	int                       num_pto;
	_di_IXMLTSuporte          xmlSuporte;
   _di_IXMLTSuportePtoFixList xmlPtoFixList;
   _di_IXMLTSuportePtoFix    xmlPtoFix;
   _di_IXMLTCoord            xmlCoord;

	try{//valida o ponteiro
		if (XMLSuporteList == NULL)
			{
			LogAviso("Objeto TSuporteList n�o definido");
			return(true);
			}
		//indica n�mero de objetos
		LogMsg("Objeto TSuporteList com " + IntToStr(XMLSuporteList->Count) + " objetos TSuporte");
		//loop p/ todos Suporte
		for (int i = 0; i < XMLSuporteList->Count; i++)
			{
			LogBlocoIni("An�lise do objeto TSuporte: " + IntToStr(i+1));
			if ((xmlSuporte  = XMLSuporteList->Get_Item(i)) == NULL)
				{
				LogErro("Objeto TSuporte n�o definido");
				return(false);
				}
			//valida Id
			if (! ValidaObjetoID(xmlSuporte->Get_id())) return(false);
			//valida vari�veis
			ValidaVariavelString("strTpo", xmlSuporte->Get_strTpo);
			ValidaVariavelDouble("dblTnsMin", xmlSuporte->Get_dblTnsMin);
			ValidaVariavelDouble("dblTnsMax(", xmlSuporte->Get_dblTnsMax);
			//determina conjunto de pontos de fixa��o
			if ((xmlPtoFixList = xmlSuporte->Get_PtoFix()) == NULL)
				{
				LogErro("Objeto TCoordList n�o definido");
				return(false);
				}
			//determina n�mero de pontos de fixa��o
			ValidaVariavelInteger("NumPtoFixacao", xmlPtoFixList->GetCount);
			num_pto = xmlPtoFixList->GetCount();
			for (int j = 0; j < num_pto; j++)
				{
				if ((xmlPtoFix = xmlPtoFixList->Get_Item(j)) == NULL)
					{
					LogErro("Objeto TSuportePtoFix n�o definido");
					return(false);
					}
            if ((xmlCoord = xmlPtoFix->Get_coordPnt()) == NULL)
               {
               LogErro("Objeto TCoord n�o definido");
               return(false);
               }
				//valida vari�veis do ptonto de fixa��o
				ValidaVariavelDouble("dblX", xmlCoord->Get_dblX);
				ValidaVariavelDouble("dblY", xmlCoord->Get_dblY);
				}
			LogBlocoFim();
			}
		}catch(Exception &e)
			{
			LogErro("na valida��o do objeto TSuporteList");
			return(false);
			}
	return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoSuprimento(_di_IXMLTEqBarSuprimentoList lstSuprimento)
  {
   //vari�veis locais
   AnsiString                atrib_str;
   _di_IXMLTEqBarSuprimento  xmlSuprimento;  // Suprimento
   _di_IXMLTPatamPropList     lstPatamar;     // Lista de patamares

	try{//valida o ponteiro
		if (lstSuprimento == NULL)
			{
			LogAviso("Objeto TEqBarSuprimentoList n�o definido");
			return(true);
			}
		//indica n�mero de objetos
		LogMsg("Objeto TEqBarSuprimentoList com " + IntToStr(lstSuprimento->Count) + " objetos TEqBarSuprimento");
		//loop p/ todos Suprimentos
		for (int i = 0; i < lstSuprimento->Count; i++)
			{
			LogBlocoIni("An�lise do objeto TEqBarSuprimento: " + IntToStr(i+1));
			if ((xmlSuprimento = lstSuprimento->Get_Item(i)) == NULL)
				{
				LogErro("Objeto TEqBarSuprimento n�o definido");
				return(false);
				}
			if (! ValidaObjetoEqpto(xmlSuprimento->Get_eqbar(), 1)) return(false);
			//valida dados do Suprimento
			try{
				xmlSuprimento->Get_intTpoSup();
				}catch(Exception &e)
					{
					LogAviso("Atributo intTpoSup n�o definido");
					xmlSuprimento->Set_intTpoSup(3); //tipoVT
					}
			try{
				xmlSuprimento->Get_dblPotMax();
				}catch(Exception &e)
					{
					LogAviso("Atributo dblPotMax n�o definido");
					xmlSuprimento->Set_dblPotMax(100.);
               }
			try{
            xmlSuprimento->Get_dblTnsNom();
				}catch(Exception &e)
               {
					LogAviso("Atributo dblTnsNom n�o definido");
					xmlSuprimento->Set_dblTnsNom(13.8);
					}
			//valida estruturas do Suprimento
			if (! ValidaObjetoZSeq(xmlSuprimento->Get_zEqv()))         return(false);
			if (! ValidaObjetoPotCpx(xmlSuprimento->Get_cpxPotCC3f())) return(false);
         if (! ValidaObjetoPotCpx(xmlSuprimento->Get_cpxPotCCFt())) return(false);
         // No suprimento, a lista de patamares � obrigat�ria
         if ((lstPatamar = xmlSuprimento->Get_Patamar()) == NULL)
            {
            LogAviso("Objeto TPatamarList n�o definido");
            return(false);
            }
         LogMsg("Objeto TPatamarList com " + IntToStr(lstPatamar->Count) + " objetos TPatamar");
         //analisa objetos Patamar
         for (int j = 0; j < lstPatamar->Count; j++)
            {
            LogBlocoIni("An�lise do objeto TPatamar " + IntToStr(j+1));
            if (! ValidaObjetoPatamar(lstPatamar->Get_Item(j))) return(false);
            LogBlocoFim();
            }
         // valida objeto TMedicao
         //if (! ValidaObjetoMedicao(xmlEqpto)) return(false);
			LogBlocoFim();
			}
		}catch(Exception &e)
			{
			LogErro("na valida��o do objeto TEqBarCargaList");
			return(false);
			}
	return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoTrafo(_di_IXMLTEqLigTrafoList lstTrafo)
   {
   //vari�veis locais
   _di_IXMLTEqLigTrafo           xmlTrafo;             // Trafo
   _di_IXMLTZseq                 xmlZSeq;              // Imped�ncia de seq��ncia
   _di_IXMLTZ                    xmlZ;                 // Imped�ncia
   _di_IXMLTLtc                  xmlLtc;               // LTC
   _di_IXMLTEnrolList             lstEnrol;             // Enrolamentos

   try{//valida o ponteiro
      if (lstTrafo == NULL)
         {
         LogAviso("Objeto TEqLigTrafoList n�o definido");
         return(true);
         }
      //indica n�mero de objetos
      LogMsg("Objeto TEqLigTrafoList com " + IntToStr(lstTrafo->Count) + " objetos TEqLigTrafo");
      //loop p/ todos objetos
      for (int i = 0; i < lstTrafo->Count; i++)
         {
         LogBlocoIni("An�lise do objeto TEqLigTrafo: " + IntToStr(i+1));
         if ((xmlTrafo = lstTrafo->Get_Item(i)) == NULL)
            {
            LogErro("Objeto TEqLigTrafo n�o definido");
            return(false);
            }
         if (! ValidaObjetoEqpto(xmlTrafo->Get_eqlig(), 2)) return(false);
         //valida enrolamento
         if ((lstEnrol = xmlTrafo->Get_Enrol()) == NULL)
            {
            LogErro("Objeto TEnrolList n�o definido");
            return(false);
            }
         // Verifica o n�mero de enrolamentos esperado.
         if (lstEnrol->Count != 2)
            {
            LogErro("Objeto TEnrolList possui "+ IntToStr(lstEnrol->Count) + " objetos TEnrol");
            return(false);
            }
         //valida objetos TEnrol
         LogBlocoIni("An�lise do objeto TEnrol: 1");
         if (! ValidaObjetoEnrol(lstEnrol->Get_Item(0))) return(false);
         LogBlocoFim();
         LogBlocoIni("An�lise do objeto TEnrol: 2");
         if (! ValidaObjetoEnrol(lstEnrol->Get_Item(1))) return(false);
         LogBlocoFim();
         //valida objeto Ltc
         if (! ValidaObjetoLtc(xmlTrafo->Get_ltc())) return(false);
         // valida objetos TMedicao
         //if (! ValidaObjetoMedicao(xmlEqpto)) return(false);
         LogBlocoFim();
         }
      }catch(Exception &e)
         {
         LogErro("na valida��o do objeto TEqLigTrafoList");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoTrafo3E(_di_IXMLTEqLigTrafo3EList lstTrafo3E)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoTrafoZZ(_di_IXMLTEqBarTrafoZZList lstTrafoZZ)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaConjuntoTrecho(_di_IXMLTEqLigTrechoList lstTrecho)
   {
   //vari�veis locais
   AnsiString           atrib_str;
   _di_IXMLTEqLigTrecho xmlTrecho;
   _di_IXMLTTrcTpo      xmlTrcTpo;
   _di_IXMLTTrcTpo01    xmlTrcTpo01;
   _di_IXMLTTrcTpo02    xmlTrcTpo02;
   _di_IXMLTTrcTpo03    xmlTrcTpo03;
   _di_IXMLTCaboId      xmlCabo;
   _di_IXMLTCaboId      xmlCaboA, xmlCaboB, xmlCaboC;

   try{//valida o ponteiro
      if (lstTrecho == NULL)
         {
         LogAviso("Objeto TEqLigTrechoList n�o definido");
         return(true);
         }
      //indica n�mero de objetos
      LogMsg("Objeto TEqLigTrechoList com " + IntToStr(lstTrecho->Count) + " objetos TEqLigTrecho");
      //loop p/ todos objetos
      for (int i = 0; i < lstTrecho->Count; i++)
         {
         LogBlocoIni("An�lise do objeto TEqLigTrecho: " + IntToStr(i+1));
         if ((xmlTrecho = lstTrecho->Get_Item(i)) == NULL)
            {
            LogErro("Objeto TEqLigTrecho n�o definido");
            return(false);
            }
         if (! ValidaObjetoEqpto(xmlTrecho->Get_eqlig(), 2)) return(false);
         //determina tipo do Trecho
         if ((xmlTrcTpo = xmlTrecho->Get_trcTpo()) == NULL)
            {
            LogErro("Objeto TTrcTpo n�o definido");
            return(false);
            }
         if ((xmlTrcTpo01 = xmlTrcTpo->Get_trcTpo01()) == NULL)
            {
            LogErro("Objeto TTrcTpo01 n�o definido");
            return(false);
            }
         if ((xmlTrcTpo02 = xmlTrcTpo->Get_trcTpo02()) == NULL)
            {
            LogErro("Objeto TTrcTpo02 n�o definido");
            return(false);
            }
         if ((xmlTrcTpo03 = xmlTrcTpo->Get_trcTpo03()) == NULL)
            {
            LogErro("Objeto TTrcTpo03 n�o definido");
            return(false);
            }
         if (xmlTrcTpo01->HasChildNodes)
            {// Trecho tipo 1 (ini): arranjoCABOZOZ1
            if ((xmlCabo = xmlTrcTpo01->Get_caboId()) == NULL)
              {
              LogErro("Objeto TCaboId n�o definido - caboZ0Z1");
              return(false);
              }
            //acessa atributos
            xmlCabo->GetText();
            }
         else if (xmlTrcTpo02->HasChildNodes)
            {// Trecho tipo 2 (ini): arranjoCABOxFASE
            if ((xmlCaboA = xmlTrcTpo02->Get_caboIdA()) == NULL)
              {
              LogErro("Objeto TCaboId n�o definido - fase A");
              return(false);
              }
            if ((xmlCaboB = xmlTrcTpo02->Get_caboIdB()) == NULL)
              {
              LogErro("Objeto TCaboId n�o definido - fase B");
              return(false);
              }
            if ((xmlCaboC = xmlTrcTpo02->Get_caboIdC()) == NULL)
              {
              LogErro("Objeto TCaboId n�o definido - fase C");
              return(false);
              }
            //acessa atributos
            atrib_str = xmlTrcTpo02->Get_strEstGeoId();
            if (atrib_str.IsEmpty())
               {
               LogErro("Atributo EstGeoId indefinido");
               return(false);
               }
            atrib_str = xmlCaboA->GetText();
            if (atrib_str.IsEmpty())
               {
               LogAviso("Atributo TCaboId indefinido - fase A");
               }
            atrib_str = xmlCaboB->GetText();
            if (atrib_str.IsEmpty())
               {
               LogAviso("Atributo TCaboId indefinido - fase B");
               }
            atrib_str = xmlCaboC->GetText();
            if (atrib_str.IsEmpty())
               {
               LogAviso("Atributo TCaboId indefinido - fase C");
               }
            }
         else if (xmlTrcTpo03->HasChildNodes)
            {// Trecho tipo 3 (ini): arranjoZ0Z1
            if (! ValidaObjetoZSeq(xmlTrcTpo03->Get_zSeq())) return(false);
            if (! ValidaObjetoCSeq(xmlTrcTpo03->Get_cSeq())) return(false);
            //acessa atributos
            xmlTrcTpo03->Get_dblCorMax();
            }
         else
            {//erro
            LogErro("objeto TEqLigTrecho definido com tipo inv�lido");
            return(false);
            }
         //acessa atributos
         xmlTrecho->Get_dblCmp();
        // valida objetos TMedicao
         //if (! ValidaObjetoMedicao(xmlEqpto)) return(false);
         LogBlocoFim();
         }
      }catch(Exception &e)
         {
         LogErro("na valida��o do objeto TEqLigTrechoList");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoCaboRGMR(_di_IXMLTCaboRGMR XMLCaboRGMR)
   {
   try{
      if (XMLCaboRGMR == NULL)
         {
         LogBlocoIni("An�lise do objeto TCaboRGMR");
         LogErro("Objeto TCaboRGMR n�o definido");
         return(false);
         }
      if (! ValidaObjetoID(XMLCaboRGMR->Get_id())) return(false);
      //acessa atributos
      XMLCaboRGMR->Get_dblCorMax();
      XMLCaboRGMR->Get_dblR();
      XMLCaboRGMR->Get_dblGMR();
      }catch(Exception &e)
         {
         LogBlocoIni("An�lise do objeto TCaboRGMR");
         LogErro("na valida��o do objeto TCaboRGMR");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoCaboZ0Z1(_di_IXMLTCaboZ0Z1 XMLCaboZ0Z1)
   {
   try{
      if (XMLCaboZ0Z1 == NULL)
         {
         LogBlocoIni("An�lise do objeto TCaboZ0Z1");
         LogErro("Objeto TCaboZ0Z1 n�o definido");
         return(false);
         }
      if (! ValidaObjetoID(XMLCaboZ0Z1->Get_id()))        return(false);
      if (! ValidaObjetoCSeq(XMLCaboZ0Z1->Get_dblCSeq())) return(false);
      if (! ValidaObjetoZSeq(XMLCaboZ0Z1->Get_zSeq()))    return(false);
      //acessa atributos
      XMLCaboZ0Z1->Get_dblCorMax();
      }catch(Exception &e)
         {
         LogBlocoIni("An�lise do objeto TCaboZ0Z1");
         LogErro("na valida��o do objeto TCaboZ0Z1");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoConcessionaria(_di_IXMLTConcessionaria xmlConcessAux)
   {
   //vari�veis locais
   AnsiString  atr_str;

   try{
      atr_str = xmlConcessAux->Get_strVer();
      if (atr_str.AnsiCompareIC("2.0") != 0)
         {
         LogAviso("Vers�o incorreta");
         }
      if (! ValidaObjetoID(xmlConcessAux->Get_id())) return(false);
      }catch(Exception &e)
         {
         LogErro("na an�lise do objeto TConcessionaria");
         return(false);
         }

   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato:: ValidaObjetoCoordId(_di_IXMLTCoordId XMLCoordId)
   {
   //vari�veis locais
   _di_IXMLTCoord              xmlCoordUTM;    // Coordenada UTM
   _di_IXMLTCoord              xmlCoordESQ1;   // Coordenada Esquema 1
   _di_IXMLTCoord              xmlCoordESQ2;   // Coordenada Esquema 2
   _di_IXMLTCoordIco           xmlCoordIco;    // Deslocamentos para posicionamento dos �cones

   try{//valida objeto
      if (XMLCoordId == NULL)
         {
         LogBlocoIni("An�lise do objeto TCoordId");
         LogErro("Objeto TCoordId n�o definido");
         return(false);
         }
      //obt�m objetos de TCoordId
      if ((xmlCoordUTM = XMLCoordId->Get_coordUTM()) == NULL)
         {
         LogBlocoIni("An�lise do objeto TCoordId");
         LogErro("Objeto TCoord n�o definido (CoordUTM)");
         return(false);
         }
      if ((xmlCoordESQ1  = XMLCoordId->Get_coordESQ1()) == NULL)
         {
         LogBlocoIni("An�lise do objeto TCoordId");
         LogErro("Objeto TCoord n�o definido (TCoordESQ1)");
         return(false);
         }
      if ((xmlCoordESQ2 = XMLCoordId->Get_coordESQ2()) == NULL)
         {
         LogBlocoIni("An�lise do objeto TCoordId");
         LogErro("Objeto TCoord n�o definido (coordESQ2)");
         return(false);
         }
      if ((xmlCoordIco= XMLCoordId->Get_coordIco()) == NULL)
         {
         LogBlocoIni("An�lise do objeto TCoordId");
         LogErro("Objeto TCoord n�o definido (coordIco)");
         return(false);
         }
      //acessa atributos
      XMLCoordId->Get_strCod();
      XMLCoordId->Get_dblPosBar();
      xmlCoordUTM->Get_dblX();
      xmlCoordUTM->Get_dblY();
      xmlCoordESQ1->Get_dblX();
      xmlCoordESQ1->Get_dblY();
      xmlCoordESQ2->Get_dblX();
      xmlCoordESQ2->Get_dblY();
      try{
         xmlCoordIco->Get_dblDxBar();
         }catch(Exception &e)
            {
            LogAviso("Atributo TCoordIco::dblDxBar n�o definido");
            xmlCoordIco->Set_dblDxBar(50);
            }
      try{
         xmlCoordIco->Get_dblDyBar();
         }catch(Exception &e)
            {
            LogAviso("Atributo TCoordIco::dblDyBar n�o definido");
            xmlCoordIco->Set_dblDyBar(50);
            }
      }catch(Exception &e)
         {
         LogBlocoIni("An�lise do objeto TCoordId");
         LogErro("na valida��o do objeto TCoordId");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoCSeq(_di_IXMLTCSeq xmlCSeq)
   {
   try{//prote��o
      if (xmlCSeq == NULL)
         {
         LogBlocoIni("An�lise do objeto TCSeq");
         LogErro("Objeto TCSeq n�o definido");
         return(false);
         }
      //acessa atributos
      xmlCSeq->Get_dblC0();
      xmlCSeq->Get_dblC1();
      }catch(Exception &e)
         {
         LogBlocoIni("An�lise do objeto TCSeq");
         LogErro("na valida��o do objeto TCSeq");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoEnrol(_di_IXMLTEnrol xmlEnrol)
   {
   //vari�veis locais
   AnsiString atrib_str;

   try{//prote��o
      if (xmlEnrol == NULL)
         {
         LogErro("Objeto TEnrol n�o definido");
         return(false);
         }
      if (! ValidaObjetoZ(xmlEnrol->Get_zTerra())) return(false);
      //acessa atributos
      xmlEnrol->Get_intTpoLig();
      xmlEnrol->Get_dblTnsNom();
      xmlEnrol->Get_dblTnsTap();
      xmlEnrol->Get_dblAngDef();
      atrib_str = xmlEnrol->Get_strFases();
      if (atrib_str.IsEmpty())
         {
         LogErro("Objeto TEnrol com atributo TFases n�o definido");
         return(false);
         }
      }catch(Exception &e)
         {
         LogErro("na valida��o do objeto TEnrol");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoEqpto(_di_IXMLTEqpto xmlEqpto, int num_coordId)
   {
   //vari�veis locais
   _di_IXMLTCoordIdList     lstCoordId;     // Lista de coordenadas identificadas

   try{//valida o ponteiro
      if (xmlEqpto == NULL)
         {
         LogBlocoIni("An�lise do objeto TEqpto");
         LogAviso("Objeto TEqpto n�o definido");
         return(true);
         }
		if (! ValidaObjetoID(xmlEqpto->Get_id())) return(false);
		if ((lstCoordId = xmlEqpto->Get_coordId()) == NULL)
         {
         LogBlocoIni("An�lise do objeto TEqpto");
         LogErro("Objeto TCoordIdList n�o definido");
         return(false);
         }
      //verifica o n�mero de coordenadas esperado.
      if (lstCoordId->Count < num_coordId)
         {
         LogBlocoIni("An�lise do objeto TEqpto");
         LogErro("Objeto TCoordIdList possui " + IntToStr(lstCoordId->Count) + "  objetos TCoordId");
         return(false);
         }
      if (lstCoordId->Count != num_coordId)
         {
         LogAviso("objeto TCoordIdList possui " + IntToStr(lstCoordId->Count) + "  objetos TCoordId");
         }
      for (int i = 0; i < num_coordId; i++)
         {
         if (! ValidaObjetoCoordId(lstCoordId->Get_Item(i))) return(false);
         }
		// valida medi��o
		if (! ValidaConjuntoMedicao(xmlEqpto->Get_Medicao())) return(false);
		}catch(Exception &e)
         {
         LogBlocoIni("An�lise do objeto TEqpto");
         LogErro("na valida��o do objeto TEqpto");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoID(_di_IXMLTId XMLId)
   {
   try{//prote��o
      if (XMLId == NULL)
         {
         LogBlocoIni("An�lise do objeto TId");
         LogErro("Objeto TId n�o definido");
         return(false);
         }
		//verifica atributos
		ValidaVariavelInteger("intId", XMLId->Get_intId);
		ValidaVariavelString("strCod", XMLId->Get_strCod);
		ValidaVariavelString("strDsc", XMLId->Get_strDsc);
		}catch(Exception &e)
         {
         LogBlocoIni("An�lise do objeto TId");
         LogErro("na valida��o do objeto TId");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoLtc( _di_IXMLTLtc xmlLtc)
   {
   try{//prote��o
      if (xmlLtc == NULL)
         {
         LogBlocoIni("An�lise do objeto TLtc");
         LogErro("Objeto TLtc inexistente");
         return(false);
         }
      //verifica atributos
      try{xmlLtc->Get_intTpoUtl();
         }catch(Exception &e)
            {
            LogAviso("Objeto TLtc: atributo intTpoUtl indefinido");
            xmlLtc->Set_intTpoUtl(0);
            }

      }catch(Exception &e)
         {
         LogBlocoIni("An�lise do objeto TLtc");
         LogErro("na valida��o do objeto TLtc");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoMedicao(_di_IXMLTMedicao xmlMedicao)
	{
	try{//valida o ponteiro
		if (xmlMedicao == NULL)
			{
			LogErro("Objeto TMedicao n�o definido");
			return(false);
			}
		//valida Id
		if (! ValidaObjetoID(xmlMedicao->Get_medidorId())) return(false);
		//valida tipo de canal
		if (! ValidaVariavelInteger("intTpoCnl", xmlMedicao->Get_intTpoCnl)) return(false);
		//valida sigma
		if (! ValidaVariavelDouble("dblSigmaCnl", xmlMedicao->Get_dblSigmaCnl)) return(false);
		//valida curva de medi��o
		if (! ValidaObjetoPatamar(xmlMedicao->Get_Patamar())) return(false);
		}catch(Exception &e)
			{
			LogErro("na valida��o do objeto TMedicao");
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoPatamar(_di_IXMLTPatamProp xmlPatamar)
   {
   //vari�veis locais
   _di_IXMLTHorMin xmlhm_ini, xmlhm_fim;

   try{//valida o ponteiro
      if (xmlPatamar == NULL)
         {
         LogErro("Objeto TPatamar n�o definido");
         return(false);
         }
      if ((xmlhm_ini = xmlPatamar->Get_horIni()) == NULL)
         {
         LogErro("Objeto THorMin n�o definido - hora minuto inial");
         return(false);
         }
      if ((xmlhm_fim = xmlPatamar->Get_horFim()) == NULL)
         {
         LogErro("Objeto THorMin n�o definido - hora minuto final");
         return(false);
         }
      //acesa atributos
      xmlhm_ini->Get_intHor();
      xmlhm_ini->Get_intMin();
      xmlhm_fim->Get_intHor();
      xmlhm_fim->Get_intMin();
      xmlPatamar->Get_dblPotAtv();
      xmlPatamar->Get_dblPotRtv();
      xmlPatamar->Get_dblMdlTns();
      xmlPatamar->Get_dblAng();
      }catch(Exception &e)
         {
         LogErro("na valida��o do objeto TPatamar");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoPotCpx(_di_IXMLTPotCpx xmlPotCpx)
   {
   try{//prote��o
      xmlPotCpx->Get_dblPotAtv();
      }catch(Exception &e)
         {
         LogAviso("Pot.Ativa de curto n�o definida");
         xmlPotCpx->Set_dblPotAtv(0.);
         }
   try{//prote��o
      xmlPotCpx->Get_dblPotRtv();
      }catch(Exception &e)
         {
         LogAviso("Pot.Reativa de curto n�o definida");
         xmlPotCpx->Set_dblPotRtv(400.);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoRede(_di_IXMLTRede XMLRede)
   {
   //vari�veis locais
   AnsiString             atrib_str;
   _di_IXMLTCnjEqBar      XMLCjnEqBar;
   _di_IXMLTCnjEqLig      XMLCnjEqLig;

   try{
      LogBlocoIni("An�lise do objeto TRede");
      //prote��o
      if (XMLRede == NULL)
         {
         LogErro("Objeto TRede n�o definido");
         return(false);
         }
      //valida objeto XMLId
      if (! ValidaObjetoID(XMLRede->Get_id())) return(false);
      //obt�m objeto XMLCjnEqBar
      if ((XMLCjnEqBar = XMLRede->Get_cnjEqBar()) == NULL)
         {
         LogErro("Objeto TRede n�o cont�m o objeto TCjnEqBar");
         return(false);
         }
      //obt�m objeto XMLCnjEqLig
      if ((XMLCnjEqLig = XMLRede->Get_cnjEqLig()) == NULL)
         {
         LogErro("Objeto TRede n�o cont�m o objeto TCnjEqLig");
         return(false);
         }
      //acessa atributos
      atrib_str = XMLRede->Get_strTpo();
      if (atrib_str.IsEmpty())
         {
         LogErro("Objeto TRede com atributo 'tipo de rede' n�o definido");
         return(false);
         }
      //valida conjuntos de objetos Eqbar
      if (! ValidaConjuntoMedidor(XMLCjnEqBar->Get_Medidor()))         return(false);
      if (! ValidaConjuntoCapacitor(XMLCjnEqBar->Get_Capacitor()))     return(false);
      if (! ValidaConjuntoCarga(XMLCjnEqBar->Get_Carga()))             return(false);
      if (! ValidaConjuntoCargaIP(XMLCjnEqBar->Get_CargaIP()))         return(false);
      if (! ValidaConjuntoFiltro(XMLCjnEqBar->Get_Filtro()))           return(false);
      if (! ValidaConjuntoGerador(XMLCjnEqBar->Get_Gerador()))         return(false);
      if (! ValidaConjuntoReator(XMLCjnEqBar->Get_Reator()))           return(false);
      if (! ValidaConjuntoSuprimento(XMLCjnEqBar->Get_Suprimento()))   return(false);
      if (! ValidaConjuntoTrafoZZ(XMLCjnEqBar->Get_TrafoZZ()))         return(false);
      //valida conjuntos de objetos Ligacao
      if (! ValidaConjuntoCapserie(XMLCnjEqLig->Get_CapacitorSerie())) return(false);
      if (! ValidaConjuntoChave(XMLCnjEqLig->Get_Chave()))             return(false);
      if (! ValidaConjuntoRegulador(XMLCnjEqLig->Get_Regulador()))     return(false);
      if (! ValidaConjuntoTrafo(XMLCnjEqLig->Get_Trafo()))             return(false);
      if (! ValidaConjuntoTrafo3E(XMLCnjEqLig->Get_Trafo3E()))         return(false);
      if (! ValidaConjuntoTrecho(XMLCnjEqLig->Get_Trecho()))           return(false);
      //validaconjunto de m�tuas
      if (! ValidaConjuntoMutua(XMLRede->Get_Mutua()))                 return(false);
      LogBlocoFim();
      }catch(Exception &)
         {
         LogErro("na valida��o do objeto TRede");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoZ(_di_IXMLTZ xmlZ)
   {
   try{
      //prote��o
      if (xmlZ == NULL)
         {
         LogBlocoIni("An�lise do objeto TZ");
         LogErro("Objeto TZ n�o definido");
         return(false);
         }
      //acessa atributos
       xmlZ->Get_dblR();
       xmlZ->Get_dblX();
      }catch(Exception &)
         {
         LogBlocoIni("An�lise do objeto TZ");
         LogErro("na valida��o do objeto TZ");
         return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaObjetoZSeq(_di_IXMLTZseq xmlZSeq)
   {
   //vari�veis locais
   _di_IXMLTZ xmlZ0, xmlZ1;

   try{
      //prote��o
      if (xmlZSeq == NULL)
         {
         LogBlocoIni("An�lise do objeto TZseq");
         LogErro("Objeto TZseq inexistente");
         return(false);
         }
      if ((xmlZ0 = xmlZSeq->Get_z0()) == NULL)
         {
         LogBlocoIni("An�lise do objeto TZseq");
         LogErro("Objeto TZ inexistente - z0");
         return(false);
         }
      if ((xmlZ1 = xmlZSeq->Get_z1()) == NULL)
         {
         LogBlocoIni("An�lise do objeto TZseq");
         LogErro("Objeto TZ inexistente - z1");
         return(false);
         }
      //acessa atributos
      try{xmlZ0->Get_dblR();
         }catch(Exception &e)
            {
            xmlZ0->Set_dblR(0);
            LogAviso("Objeto TZseq, atributo TZ0::dblR n�o definido");
            }
      try{xmlZ0->Get_dblX();
         }catch(Exception &e)
            {
            xmlZ0->Set_dblX(0);
            LogAviso("Objeto TZseq, atributo TZ0::dblX n�o definido");
            }
      try{xmlZ1->Get_dblR();
         }catch(Exception &e)
            {
            xmlZ1->Set_dblR(0);
            LogAviso("Objeto TZseq, atributo TZ1::dblR n�o definido");
            }
      try{xmlZ1->Get_dblX();
         }catch(Exception &e)
            {
            xmlZ1->Set_dblX(0);
            LogAviso("Objeto TZseq, atributo TZ1::dblX n�o definido");
            }
      }catch(Exception &)
         {
			LogBlocoIni("An�lise do objeto TZseq");
			LogErro("na valida��o do objeto TZseq");
			return(false);
         }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaVariavelDouble(AnsiString nome,  double __fastcall(__closure *funcao)(void))
	{
   //vari�veis locais
   double valor;

	try{//executa a fun��o
		valor = funcao();
		}catch(Exception &e)
			{
			LogErro("na leitura da vari�vel " + nome);
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaVariavelInteger(AnsiString nome, int __fastcall(__closure *funcao)(void))
	{
   //vari�veis locais
   int valor;

	try{//executa a fun��o
		valor = funcao();
		}catch(Exception &e)
			{
			LogErro("na leitura da vari�vel " + nome);
			return(false);
			}
	return(true);
	}
	
//-----------------------------------------------------------------------------
bool __fastcall TFormato::ValidaVariavelString (AnsiString nome, UnicodeString __fastcall(__closure *funcao)(void))
	{
   //vari�veis locais
   AnsiString valor;

	try{//executa a fun��o
		valor = funcao();
		}catch(Exception &e)
			{
			LogErro("na leitura da vari�vel " + nome);
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
//eof