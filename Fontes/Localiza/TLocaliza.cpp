//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLocaliza.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
//#include "..\Cronometro\VTCronometro.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função para criar objeto VTLocaliza
VTLocaliza* __fastcall NewObjLocaliza(VTApl *apl)
   {
   return(new TLocaliza(apl));
   }

//---------------------------------------------------------------------------
__fastcall TLocaliza::TLocaliza(VTApl *apl)
   {
   //salva objetos
   this->apl = apl;
   //cria objetos
   consulta  = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
   legenda   = (VTLegenda*)apl->GetObject(__classid(VTLegenda));
   lisEQP   = new TList();
   lisSTRAUX = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TLocaliza::~TLocaliza(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
   if (lisSTRAUX)  {delete lisSTRAUX;  lisSTRAUX  = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TLocaliza::ComparaAnsi(AnsiString nomeeqpto, AnsiString regra)
   {
   //variáveis locais
   bool resultado;
   char *nome;
   char *filtro;

   //proteção
   if ((regra.IsEmpty()) || (regra.AnsiCompareIC("*") == 0)) return(true);
   //cria char array
   nome    = new char[nomeeqpto.Length()+1];
   filtro  = new char[regra.Length()+1];
   //copia ansistring para char*
   strcpy(nome, nomeeqpto.c_str());
   strcpy(filtro, regra.c_str());
   //executa comparação e guarda resultado
   resultado = ComparaChar(nome, filtro);
   //libera memória
   delete[] nome;
   delete[] filtro;
   //retorna resultado
   return resultado;
   }

//---------------------------------------------------------------------------
bool __fastcall TLocaliza::ComparaChar(char *nome, char *filtro)
   {
   //variáveis locais
   char *bloco;

   //guarda primeiro e ultimo caractere do filtro
   char filtro_pri = filtro[0];
   char filtro_ult = filtro[(int)strlen(filtro)-1];

   //guarda o comprimento do nome
   int nome_comp = strlen(nome);
   //pos é usado para marcar a posicao no nome[pos]
   int pos = 0;
   //se nome ou filtro tiverem comprimento zero, retorna zero
   if ((strlen(filtro) == 0) || (nome_comp == 0)) return false;
   //a função strtok separa um pedaço do filtro entre dois *
   bloco = strtok (filtro, "*");
   //compara os começos do nome e do filtro, se diferentes return false
   if (filtro_pri != '*')
      {
      if (strnicmp(nome, bloco, (int)strlen(bloco)) != 0)
         {
         return false;
         }
      }
   //nesse while são pegos pedaços do filtro entre *, que são procurados
   //no nome
   while (bloco != NULL)
      {
      for (pos = pos ; pos < nome_comp ; pos++)
         {
         if (strnicmp(&nome[pos], bloco, (int)strlen(bloco)) == 0) //achou o bloco
            {
            pos += strlen(bloco);
            break;
            }
         else if (pos >= nome_comp - 1) {return false;}
         }
      bloco = strtok (NULL, "*");
      if (bloco == NULL)
         {
         if ( (filtro_ult != '*') && (pos != nome_comp) )  {return false;}
         }
      else if (pos >= nome_comp)
         {//ainda ha blocos a procurar mas ja chegou ao fim do nome
         return false;
         }
      }
   return true;
   }

//---------------------------------------------------------------------------
TList* __fastcall TLocaliza::LisEqpto(VTRedes *redes, int tipo_eqpto, int operador,
                                      int ind_atrib, AnsiString val_alvo, bool area_visivel)
   {
   //variáveis locais
   double       val_num, val_alvo_num;
   AnsiString   val_str;
   VTEqpto      *eqpto;
   TList        *lisEXT;
   //VTCronometro *cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));

   //inicia cronômetro
   //cronometro->Start("TLocaliza", "Eqptos");
   //reinicia lisEQP
   lisEQP->Clear();
   //obtém lista de todos Eqptos do tipo indicado
   if (area_visivel)
      {//busca somente na área visível da rede
      lisEXT = consulta->LisEqptoRedeVisible(redes, tipo_eqpto);
      }
   else
      {//busca em toda a área da rede
      lisEXT = consulta->LisEqptoRede(redes, tipo_eqpto);
      }
   //verifica se não foi definido um valor p/ atributo
   if ((val_alvo.IsEmpty())||(val_alvo.AnsiCompare("*") == 0))
      {//retorna todos Eqptos visíveis do tipo indicado
      CopiaTList(lisEXT, lisEQP);
      return(lisEQP);
      }
   //verifica se valor alvo é string ou número
	if (StrToDouble(val_alvo, val_alvo_num))
      {
      for (int n = 0; n < lisEXT->Count; n++)
         {
         eqpto = (VTEqpto*)lisEXT->Items[n];
         try{//determina valor do atributo
            val_str = legenda->ValorAtributo(eqpto, ind_atrib);
			val_num = StrToDouble(val_str);
            //compara valores numérivos
			if (ValorValido(operador, val_num, val_alvo_num)) lisEQP->Add(eqpto);
			}
			catch(Exception &)
			   {//nada a fazer
			   }
		 }
	  }
   else
      {
      for (int n = 0; n < lisEXT->Count; n++)
         {
         eqpto = (VTEqpto*)lisEXT->Items[n];
         try{//determina valor do atributo
            val_str = legenda->ValorAtributo(eqpto, ind_atrib);
            //compara strings
			if (ValorValido(operador, val_str, val_alvo))
				{lisEQP->Add(eqpto);}
			}
			catch(Exception &)
			   {//nada a fazer
               }
         }
	  }
   //cronometro->Stop("TLocaliza");
   //cronometro->Show();
   //cronometro->Reset();
   return(lisEQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TLocaliza::LisEqptoErroGis(VTRedes *redes, int tipo_eqpto, bool area_visivel)
   {
   //variáveis locais
   VTEqpto *eqpto;
	TList   *lisEXT;

   //reinicia lisEQP
   lisEQP->Clear();
   //obtém lista de todos Eqptos do tipo indicado
   if (area_visivel)
      {//busca somente na área visível da rede
      lisEXT = consulta->LisEqptoRedeVisible(redes, tipo_eqpto);
      }
   else
      {//busca em toda a área da rede
      lisEXT = consulta->LisEqptoRede(redes, tipo_eqpto);
      }
   //retira da lista todos Eqptos que não apresentam status de erro no GIS
   for (int n = 0; n < lisEXT->Count; n++)
      {
      eqpto = (VTEqpto*)lisEXT->Items[n];
      if (eqpto->Status[sttERROGIS]) lisEQP->Add(eqpto);
      }
   return(lisEQP);
   }
//---------------------------------------------------------------------------
bool __fastcall TLocaliza::ValorValido(int operador, AnsiString valor, AnsiString val_alvo)
   {
   //variáveis locais
   int len, lenValor, lenAlvo, teste;
   bool sucesso = true;
   int pos_anterior, pos;
   bool lastCharSeparator = false;
   char *lastChar;

   //verifica se val_alvo contém o caractere '*'
   len = val_alvo.Pos("*");
   //limpa auxiliar
   lisSTRAUX->Clear();
   if (len > 0)
	  {
	  lastChar = val_alvo.AnsiLastChar();
	  if (*lastChar == '*')
		{
		lastCharSeparator = true;
		}
	  //val_alvo = val_alvo.SubString(1, len);
	  pos = pos_anterior = 0;
	  //separa a string em sub strings entre os *
	  ExtraiStrings(val_alvo,"*",lisSTRAUX);
	  teste = lisSTRAUX->Count;
	  //verifica se o valor atende ao operador
	  for (int i = 0; (i < lisSTRAUX->Count)&&(sucesso); i++)
		 {
		 val_alvo = lisSTRAUX->Strings[i];
		 //se for vazio, faz nada
		 if (val_alvo.IsEmpty())
			{continue;}
		 //verifica
		 lenValor = valor.Length();
		 lenAlvo = val_alvo.Length();
		 if (lenAlvo > lenValor)
			{pos = 0;}
		 else
			{pos = valor.Pos(val_alvo);}
		 switch(operador)
			{
			case opIGUAL:
			   //existem 2 exceções
			   if ((i == 0) && (pos != 1) && (len != 1))
				  {//busca inicio
				  sucesso = false;
				  }
			   if ((i != 0) && (i == lisSTRAUX->Count - 1) && (pos !=  (lenValor - lenAlvo + 1)) && (!lastCharSeparator))
				  {//busca fim
				  sucesso = false;
				  }
			   //verificacao normal
			   if ((pos == 0) || (pos < pos_anterior))
				  {
				  sucesso = false;
				  }
			   break;
			case opDIFERENTE:
			   //verifica se esta presente
			   if (pos != 0)
				  {sucesso = false;}
			   break;
			case opMENOR:
			case opMAIOR:
			   pos = valor.AnsiCompareIC(val_alvo);
			   if ((pos < 0) != (pos_anterior < 0))
				  {sucesso = false;}
			   break;
			 }
		 pos_anterior = pos;
		 if (sucesso == false)
		    {break;}
		 }
      return sucesso;
//	  if (valor.Length() > len)
//		{valor = valor.SubString(1, len);}
	  }
   else
	  {//busca valor simples
	  switch(operador)
		  {
		  case opIGUAL:
			 if (valor.Length() != val_alvo.Length())
				{return(false);}
			 return(valor.AnsiCompareIC(val_alvo) == 0);
		  case opDIFERENTE:  //limita a comparação ao menor número de caracteres
			 if (valor.Length() != val_alvo.Length())
				{return(true);}
			 return(valor.AnsiCompareIC(val_alvo) != 0);
		  case opMENOR:
			 return(valor.AnsiCompareIC(val_alvo) < 0);
		  case opMAIOR:
			 return(valor.AnsiCompareIC(val_alvo) > 0);
		  }
	   return(false);
	  }
   }

//---------------------------------------------------------------------------
bool __fastcall TLocaliza::ValorValido(int operador, double valor, double val_alvo)
   {
   //verifica se o valor atende ao operador
   switch(operador)
      {
      case opIGUAL:
         return(IsDoubleZero(valor - val_alvo));
      case opDIFERENTE:  //limita a comparação ao menor número de caracteres
         return(! IsDoubleZero(valor - val_alvo));
      case opMENOR:
         return(valor < val_alvo);
      case opMAIOR:
         return(valor > val_alvo);
      }
   return(false);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TLocaliza::ValorValido(int operador, AnsiString valor, AnsiString val_alvo)
   {
   //variáveis locais
   int index;

   //verifica se val_alvo contém o caractere '*'
   index = val_alvo.Length();
   if (val_alvo[index] == '*')
      {//a comparação deve ser feita c/ o string do valor_alvo sem o caractere '*'
      val_alvo = val_alvo.SubString(1, index - 1);
      return(ValorValidoComAsterisco(operador, valor, val_alvo));
      }
   else
      {
      return(ValorValidoSemAsterisco(operador, valor, val_alvo));
      }
   }


//---------------------------------------------------------------------------
bool __fastcall TLocaliza::ValorValidoComAsterisco(int operador, AnsiString valor, AnsiString val_alvo)
   {
   //verifica se o valor atende ao operador
   switch(operador)
      {
      case opIGUAL:  //parte inicial de valor deve ser igual ao val_alvo
         if (valor.Length() < val_alvo.Length()) return(false);
         //obtém substring de valor c/ mesmo tamanho do val_alvo
         valor = valor.SubString(1, val_alvo.Length());
         return(valor.AnsiCompareIC(val_alvo) == 0);

PAREI AQUI

      case opDIFERENTE:  //limita a comparação ao menor número de caracteres
         if (valor.Length() < val_alvo.Length())
            {
            val_alvo = val_alvo.SubString(1, valor.Length());
            }
         else if (val_alvo.Length() < valor.Length())
            {
            valor = valor.SubString(1, val_alvo.Length());
            }
         return(valor.AnsiCompareIC(val_alvo) != 0);
      case opMENOR:
         return(valor.AnsiCompareIC(val_alvo) < 0);
      case opMAIOR:
         return(valor.AnsiCompareIC(val_alvo) > 0);
      }
   return(false);
   }
*/
//---------------------------------------------------------------------------
TList* __fastcall TLocaliza::LocalizaPorCodigo(VTRedes *redes, int tipo_eqpto, AnsiString filtro)
   {
   //variáveis locais
   VTEqpto *eqpto;
   TList   *lisEXT;

   //reinicia lisEQP
   lisEQP->Clear();
   //obtém lista de todos equipamentos visíveis do tipo indicado
   lisEXT = consulta->LisEqptoRedeVisible(redes, tipo_eqpto);
   for (int n = 0; n < lisEXT->Count; n++)
      {
      eqpto = (VTEqpto*)lisEXT->Items[n];
      //compara com o que foi digitado pelo usuário
      if (ComparaAnsi(eqpto->Codigo, filtro))
         {//nome passa pelo filtro: insere na lisEQP
         if (lisEQP->IndexOf(eqpto) < 0) lisEQP->Add(eqpto);
         }
      }
   return(lisEQP);
   }

//---------------------------------------------------------------------------
//eof
