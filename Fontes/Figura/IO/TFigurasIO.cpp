//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Graphics.hpp>
#include "TFigurasIO.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTFiguras.h"
#include "..\Figura\VTLink.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Figura\VTFigura.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFigurasIO* NewObjFigurasIO(VTApl *apl_owner)
   {
   return(new TFigurasIO(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TFigurasIO::TFigurasIO(VTApl *apl_owner)
   {
   //variáveis locais
   VTGeral *geral;

   //salva ponteiro
   this->apl     = apl_owner;
   //obtém demais ponteiros
   figuras   = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   infoset   = geral->Infoset;
   lisOBRA = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFigurasIO::~TFigurasIO(void)
   {
   //nada a fazer
	   if(lisOBRA != NULL)
	   {
		   delete lisOBRA;
		   lisOBRA = NULL;
	   }
   }

//---------------------------------------------------------------------------
VTObra* __fastcall TFigurasIO::ExisteObra(int obra_id)
   {
	   // Variáveis locais
	   VTObra *obra;

	   //Verifica se a obra é da Rede original
	   if(obra_id == -1)
		{return(NULL);}

	   estudo->LisObraEstudo(lisOBRA);
	   for(int no = 0; no < lisOBRA->Count; no++)
	   {
		 obra = (VTObra*)lisOBRA->Items[no];
		 if(obra->Id == obra_id)
		 {
			 return (obra);
		 }
	   }
	   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFigurasIO::FiguraFilename(AnsiString &filename)
   {
   VTPath *path   = (VTPath*)apl->GetObject(__classid(VTPath));

   //proteção

   filename = ExtractFileName(path->DirBase);
   filename = "Figura_" + ChangeFileExt(filename, ".txt");

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFigurasIO::ImportaTXT(VTEstudo *estudo, AnsiString full_filepath, TList *lisREDE_SELE)
   {
   //variáveis locais
   int        valor_int;
   double     valor_flo;
   AnsiString tipo;
   AnsiString filename;
   // Salva ponteiro
   this->estudo = estudo;
   lisREDES = lisREDE_SELE;

   if ((full_filepath.IsEmpty()) || (!FileExists(full_filepath)))
	  {
	  //determina arquivo com dados das figuras
	  if (! FiguraFilename(filename))  return(false);
	  }
   else
	  {
	  filename = full_filepath;
      }

   //lê arquivo
   if (!infoset->LoadFromFile(filename)) return(true);
   for (int index = 0; index < infoset->Count; index++)
      {
      if (! infoset->SelectLine(index)) continue;
      //determina tipo da figura
      if (! infoset->GetInfo("Tipo", tipo)) continue;
      if      (tipo.AnsiCompareIC("Comentario") == 0) ReadComentario();
      else if (tipo.AnsiCompareIC("Texto") == 0)      ReadComentarioTexto();
      else if (tipo.AnsiCompareIC("Link") == 0)       ReadComentarioLink();
      else if (tipo.AnsiCompareIC("Retangulo") == 0)  ReadRetangulo();
      }
   return(true);
   }
//---------------------------------------------------------------------------
int __fastcall TFigurasIO::NextComentarioId(void)
   {
   return(++next_id.comentario);
   }

//---------------------------------------------------------------------------
bool __fastcall TFigurasIO::ReadComentario(void)
   {
   //variáveis locais
   int           estudo_id, obra_id, rede_id, figura_id, formato, visible;
   int           borda_cor, borda_espessura, borda_estilo;
   int           fundo_cor, transparente;
   int           font_size, font_color, font_bold, font_italic, font_underline, font_strikeout;
   int           dim_x, dim_y;
   AnsiString    font_name;
   strUTM        utm;
   strCOORD      esq;
   VTFiguraComentario *comentario;

   //lê atributos
   if (! infoset->GetInfo("EstudoID",       estudo_id))       return(false);
   if (! infoset->GetInfo("ObraID",         obra_id))         return(false);
   if (! infoset->GetInfo("RedeID",         rede_id))         return(false);
   if (! infoset->GetInfo("FiguraId",       figura_id))       return(false);
   if (! infoset->GetInfo("Visible",        visible))         return(false);
   if (! infoset->GetInfo("Formato",        formato))         return(false);
   if (! infoset->GetInfo("FontName",       font_name))       return(false);
   if (! infoset->GetInfo("FontSize",       font_size))       return(false);
   if (! infoset->GetInfo("FontColor",      font_color))      return(false);
   if (! infoset->GetInfo("FontBold",       font_bold))       font_bold      = 0;
   if (! infoset->GetInfo("FontItalic",     font_italic))     font_italic    = 0;
   if (! infoset->GetInfo("FontUnderline",  font_underline))  font_underline = 0;
   if (! infoset->GetInfo("FontStrikeOut",  font_strikeout))  font_strikeout = 0;
   if (! infoset->GetInfo("BordaEstilo",    borda_estilo))    return(false);
   if (! infoset->GetInfo("BordaEspessura", borda_espessura)) return(false);
   if (! infoset->GetInfo("BordaCor",       borda_cor))       return(false);
   if (! infoset->GetInfo("FundoCor",       fundo_cor))       return(false);
   if (! infoset->GetInfo("Transparente",   transparente))    return(false);
   if (! infoset->GetInfo("EsqX",           esq.x))           return(false);
   if (! infoset->GetInfo("EsqY",           esq.y))           return(false);
   if (! infoset->GetInfo("UtmX",           utm.x))           return(false);
   if (! infoset->GetInfo("UtmY",           utm.y))           return(false);
   if (! infoset->GetInfo("DimX",           dim_x))           return(false);
   if (! infoset->GetInfo("DimY",           dim_y))           return(false);
   //Verifica se o comentário está no estudo atual
   if((estudo->Id != estudo_id) && (estudo_id != 0))
   {
	   return(false);
   }
   //cria novo objeto Comentario
   if ((comentario = figuras->InsereComentario()) == NULL) return(false);
   //define formato fmtRETANGULO: importante na definição da dimensão
   comentario->Formato                   = fmtRETANGULO;
   //inicia atributos do Comentario
   comentario->Status[sttEV_NOVO]        = false;
   comentario->Rede                      = redes->ExisteRede(rede_id);
   comentario->Obra                      = ExisteObra(obra_id);
   comentario->Id                        = figura_id;
   comentario->Visible                   = bool(visible);
   if(lisREDES != NULL)
   {
	  if(lisREDES->IndexOf(comentario->Rede) >=0 )
	  {
		  comentario->Visible = true;
	  }
	  else
	  {
		  comentario->Visible = false;
      }
   }
   //comentario->Formato                   = formato;
   comentario->Font->Name                = font_name;
   comentario->Font->Size                = font_size;
   comentario->Font->Color               = TColor(font_color);
   if (font_bold      == 1) comentario->Font->Style << fsBold;
   if (font_italic    == 1) comentario->Font->Style << fsItalic;
   if (font_underline == 1) comentario->Font->Style << fsUnderline;
   if (font_strikeout == 1) comentario->Font->Style << fsStrikeOut;
   comentario->Retangulo->Status[sttEV_NOVO] = false;
   comentario->Retangulo->BordaEstilo    = borda_estilo;
   comentario->Retangulo->BordaEspessura = borda_espessura;
   comentario->Retangulo->BordaCor       = TColor(borda_cor);
   comentario->Retangulo->FundoCor       = TColor(fundo_cor);
   comentario->Retangulo->Transparente   = bool(transparente);
   comentario->Retangulo->SetCentro(utm, esq);
   comentario->Retangulo->SetDimensao(dim_x, dim_y);
   //define o formato real
   comentario->Formato                   = formato;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFigurasIO::ReadComentarioLink(void)
   {
   //variáveis locais
   int           figura_id, eqpto_tipo, eqpto_id;
   int           pos_com, pos_eqp;
   int           estilo, espessura, color;
   VTFiguraComentario *comentario;
   VTEqpto      *eqpto;
   VTLink       *link;

   //lê atributos
   if (! infoset->GetInfo("FiguraId",  figura_id))  return(false);
   if (! infoset->GetInfo("EqptoTipo", eqpto_tipo)) return(false);
   if (! infoset->GetInfo("EqptoId",   eqpto_id))   return(false);
   if (! infoset->GetInfo("PosCom",    pos_com))    return(false);
   if (! infoset->GetInfo("PosEqp",    pos_eqp))    return(false);
//   if (! infoset->GetInfo("Estilo",    estilo))       return(false);
//   if (! infoset->GetInfo("Espessura", espessura))  return(false);
//   if (! infoset->GetInfo("Color",     color))      return(false);
   //determina Eqpto
   if ((eqpto = redes->ExisteEqpto(eqpto_tipo, eqpto_id)) == NULL) return(false);
   //determina Comentario
   if ((comentario = figuras->ExisteComentario(figura_id)) == NULL) return(false);
   //insere Link no Comentario
   if ((link = comentario->InsereLink()) == NULL) return(false);
   link->Eqpto     = eqpto;
   link->PosEqp    = pos_eqp;
   link->PosCom    = pos_com;
   link->BordaTipo    = psSolid;  //estilo
   link->Espessura = 1;       //espessura
   link->Color     = clWhite; //TColor(color);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFigurasIO::ReadComentarioTexto(void)
   {
   //variáveis locais
   int          figura_id;
   AnsiString   texto;
   VTFiguraComentario *comentario;

   //lê atributos
   if (! infoset->GetInfo("FiguraId", figura_id)) return(false);
   if (! infoset->GetInfo("Texto",    texto))     return(false);
   //determina Comentario
   if ((comentario = figuras->ExisteComentario(figura_id)) == NULL) return(false);
   //insere texto no Comentario
   comentario->InsereTexto(texto);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFigurasIO::ReadRetangulo(void)
   {
   //variáveis locais
   int          estudo_id, obra_id, figura_id, visible;
   int          borda_cor, borda_espessura, borda_estilo;
   int          fundo_cor, transparente;
   int          dim_x, dim_y;
   strUTM       utm;
   strCOORD     esq;
   VTRetangulo *retangulo;

   //lê atributos
   if (! infoset->GetInfo("EstudoID",       estudo_id))       return(false);
   if (! infoset->GetInfo("ObraId",         obra_id))         return(false);
   if (! infoset->GetInfo("FiguraId",       figura_id))       return(false);
   if (! infoset->GetInfo("Visible",        visible))         return(false);
   if (! infoset->GetInfo("BordaEstilo",    borda_estilo))    return(false);
   if (! infoset->GetInfo("BordaEspessura", borda_espessura)) return(false);
   if (! infoset->GetInfo("BordaCor",       borda_cor))       return(false);
   if (! infoset->GetInfo("FundoCor",       fundo_cor))       return(false);
   if (! infoset->GetInfo("Transparente",   transparente))    return(false);
   if (! infoset->GetInfo("EsqX",           esq.x))           return(false);
   if (! infoset->GetInfo("EsqY",           esq.y))           return(false);
   if (! infoset->GetInfo("UtmX",           utm.x))           return(false);
   if (! infoset->GetInfo("UtmY",           utm.y))           return(false);
   if (! infoset->GetInfo("DimX",           dim_x))           return(false);
   if (! infoset->GetInfo("DimY",           dim_y))           return(false);
   //Verifica se o comentário está no estudo atual
   if((estudo->Id != estudo_id) && (estudo_id != 0))
   {
	   return(false);
   }
   //cria novo objeto Comentario
   if ((retangulo = figuras->InsereRetangulo()) == NULL) return(false);
   //inicia atributos do Comentario
   retangulo->Id             = figura_id;
   retangulo->Obra           = ExisteObra(obra_id);
   retangulo->Visible        = bool(visible);
   retangulo->BordaEstilo    = borda_estilo;
   retangulo->BordaEspessura = borda_espessura;
   retangulo->BordaCor       = TColor(borda_cor);
   retangulo->FundoCor       = TColor(fundo_cor);
   retangulo->Transparente   = bool(transparente);
   retangulo->SetCentro(utm, esq);
   retangulo->SetDimensao(dim_x, dim_y);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFigurasIO::ExportaTXT(VTEstudo *estudo, AnsiString full_filepath)
   {
   //variáveis locais
   AnsiString    filename;
   this->estudo = estudo;

   if (full_filepath.IsEmpty())
	  {
	  //determina arquivo com dados das figuras
	  if (! FiguraFilename(filename))  return(false);
	  }
   else
	  {
	  filename = full_filepath;
	  }
   //inicia Infoset
   infoset->Clear();
   //grava Comentarios
   WriteComentario(estudo);
   //grava Retngulos
   WriteRetangulo(estudo);
   //salva em arquivo
   infoset->SaveToFile(filename);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFigurasIO::WriteComentario(VTEstudo *estudo)
   {
   //variáveis locais
   int           formato, dim_x, dim_y;
   strUTM        utm;
   strCOORD      esq;
   TList        *lisCOM, *lisLINK;
   TStrings     *texto;
   VTFiguraComentario *comentario;
   VTLink       *link;

   //loop para todos Comentarios
   lisCOM = figuras->LisComentario();
   for (int i = 0; i < lisCOM->Count; i++)
      {
	  comentario = (VTFiguraComentario*)lisCOM->Items[i];
      //redefine ID
      comentario->Id = NextComentarioId();
	  //altera formato para fmtRETANGULO para obter dimensões
      formato             = comentario->Formato;
	  comentario->Formato = fmtRETANGULO;
	  comentario->Status[sttEV_NOVO] = false;
	  comentario->Status[sttEV_ALTERADO] = false;
	  //determina centro e dimensão (no estilo retângulo)
      comentario->Retangulo->GetCentro(utm, esq);
      comentario->Retangulo->GetDimensao(dim_x, dim_y);
      //restaura estilo
	  comentario->Formato = formato;
      //insere linha com dados do Comentario
      infoset->InsertNewLine();
	  infoset->AddInfo("Tipo",           AnsiString("Comentario"));
      infoset->AddInfo("EstudoID",       estudo->Id);
      if (comentario->Obra)              infoset->AddInfo("ObraID", comentario->Obra->Id);
      else                               infoset->AddInfo("ObraID", -1);
      if (comentario->Rede != NULL)      infoset->AddInfo("RedeID", comentario->Rede->Id);
      else                               infoset->AddInfo("RedeID", -1);
      infoset->AddInfo("FiguraId",       comentario->Id);
      infoset->AddInfo("Visible",        int(comentario->Visible));
      infoset->AddInfo("Formato",        comentario->Formato);
      infoset->AddInfo("FontName",       comentario->Font->Name);
      infoset->AddInfo("FontSize",       comentario->Font->Size);
      infoset->AddInfo("FontColor",      comentario->Font->Color);
      if (comentario->Font->Style.Contains(fsBold))      infoset->AddInfo("FontBold",      1);
      if (comentario->Font->Style.Contains(fsItalic))    infoset->AddInfo("FontItalic",    1);
      if (comentario->Font->Style.Contains(fsUnderline)) infoset->AddInfo("FontUnderline", 1);
      if (comentario->Font->Style.Contains(fsStrikeOut)) infoset->AddInfo("FontStrikeOut", 1);
      infoset->AddInfo("BordaEstilo",    comentario->Retangulo->BordaEstilo);
      infoset->AddInfo("BordaEspessura", comentario->Retangulo->BordaEspessura);
      infoset->AddInfo("BordaCor",       int(comentario->Retangulo->BordaCor));
      infoset->AddInfo("FundoCor",       int(comentario->Retangulo->FundoCor));
      infoset->AddInfo("Transparente",   int(comentario->Retangulo->Transparente));
      infoset->AddInfo("EsqX",           esq.x);
      infoset->AddInfo("EsqY",           esq.y);
      infoset->AddInfo("UtmX",           utm.x);
      infoset->AddInfo("UtmY",           utm.y);
      infoset->AddInfo("DimX",           dim_x);
      infoset->AddInfo("DimY",           dim_y);
      //insere linhas com texto do Comentario
      texto = comentario->Texto();
      for (int j = 0; j < texto->Count; j++)
         {
         infoset->InsertNewLine();
		 infoset->AddInfo("Tipo", AnsiString("Texto"));
         infoset->AddInfo("FiguraId", comentario->Id);
         infoset->AddInfo("Texto", texto->Strings[j]);
         }
      //insere linhas com Links do Comentario
      lisLINK = comentario->LisLink();
      for (int j = 0; j < lisLINK->Count; j++)
         {
		 link = (VTLink*)lisLINK->Items[j];
		 link->Status[sttEV_NOVO] = false;
		 link->Status[sttEV_ALTERADO] = false;
         infoset->InsertNewLine();
		 infoset->AddInfo("Tipo",      AnsiString("Link"));
         infoset->AddInfo("FiguraId",  comentario->Id);
         infoset->AddInfo("EqptoTipo", link->Eqpto->Tipo());
         infoset->AddInfo("EqptoId",   link->Eqpto->Id);
         infoset->AddInfo("PosCom",    link->PosCom);
         infoset->AddInfo("PosEqp",    link->PosEqp);
         infoset->AddInfo("Estilo",    link->Estilo);
         infoset->AddInfo("Espessura", link->Espessura);
         infoset->AddInfo("Color",     link->Color);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFigurasIO::WriteRetangulo(VTEstudo *estudo)
   {
   //variáveis locais
   int           dim_x, dim_y;
   strUTM        utm;
   strCOORD      esq;
   TList        *lisRET;
   VTRetangulo  *retangulo;

   //loop para todos Comentarios
   lisRET = figuras->LisRetangulo();
   for (int i = 0; i < lisRET->Count; i++)
      {
      retangulo = (VTRetangulo*)lisRET->Items[i];
	  //redefine ID
      retangulo->Id = NextComentarioId();
      retangulo->GetCentro(utm, esq);
	  retangulo->GetDimensao(dim_x, dim_y);
	  retangulo->Status[sttEV_NOVO] = false;
	  retangulo->Status[sttEV_ALTERADO] = false;
      //insere linha com dados do Retangulo
      infoset->InsertNewLine();
      infoset->AddInfo("Tipo",           AnsiString("Retangulo"));
      infoset->AddInfo("EstudoID",       estudo->Id);
      if (retangulo->Obra)               infoset->AddInfo("ObraID", retangulo->Obra->Id);
      else                               infoset->AddInfo("ObraID", -1);
      infoset->AddInfo("FiguraId",       retangulo->Id);
      infoset->AddInfo("Visible",        int(retangulo->Visible));
      infoset->AddInfo("BordaEstilo",    retangulo->BordaEstilo);
      infoset->AddInfo("BordaEspessura", retangulo->BordaEspessura);
      infoset->AddInfo("BordaCor",       int(retangulo->BordaCor));
      infoset->AddInfo("FundoCor",       int(retangulo->FundoCor));
      infoset->AddInfo("Transparente",   int(retangulo->Transparente));
      infoset->AddInfo("EsqX",           esq.x);
      infoset->AddInfo("EsqY",           esq.y);
      infoset->AddInfo("UtmX",           utm.x);
      infoset->AddInfo("UtmY",           utm.y);
      infoset->AddInfo("DimX",           dim_x);
      infoset->AddInfo("DimY",           dim_y);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof


