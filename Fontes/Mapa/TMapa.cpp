//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMapa.h"
#include "VTPoligono.h"
#include "VTRegiao.h"
#include "VTImportaRegiao.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMapa* __fastcall NewObjMapa(VTApl *apl)
   {
   return(new TMapa(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMapa::TMapa(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria lista
   lisREGIAO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMapa::~TMapa(void)
   {
   //destr�i listas e seus objetos
   if (lisREGIAO) {LimpaTList(lisREGIAO); delete lisREGIAO; lisREGIAO = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMapa::AreaRedeContemAreaRegiao(VTRegiao *regiao)
   {
   //vari�veis locais
   VTRedes *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTArea  *area_rede   = redes->AreaUtm_cm();
   VTArea  *area_regiao = regiao->Poligono->AreaUtm_cm();

   //verifica se �rea das Redes cont�m um dos pontos da �rea retangular da Regiao
   if (area_rede->ContemPonto(area_regiao->x1, area_regiao->y1)) return(true);
   if (area_rede->ContemPonto(area_regiao->x2, area_regiao->y2)) return(true);
   //verifica se �rea da Regiao cont�m um dos pontos da �rea das Redes
   if (area_regiao->ContemPonto(area_rede->x1, area_rede->y1)) return(true);
   if (area_regiao->ContemPonto(area_rede->x2, area_rede->y2)) return(true);

   return(false);
   }

//-----------------------------------------------------------------------------
VTRegiao* __fastcall TMapa::CriaRegiao(void)
   {
   //vari�veis locais
   VTRegiao *regiao;

   //cria nova Regiao e insere na lista
   if ((regiao = NewObjRegiao(apl)) != NULL)
      {//define c�digo
      regiao->Codigo = "Regi�o Nova";
      InsereRegiao(regiao);
      }
   return(regiao);
   }

//-----------------------------------------------------------------------------
void __fastcall TMapa::DefineRegiaoVisivel(void)
   {
   //vari�veis locais
   VTRegiao *regiao;

   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      regiao->Visible = AreaRedeContemAreaRegiao(regiao);
      }
   }

//-----------------------------------------------------------------------------
VTRegiao* __fastcall TMapa::ExisteRegiao(int regiao_id)
   {
   //vari�veis locais
   VTRegiao *regiao;

   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      if (regiao->Id == regiao_id) return(regiao);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTRegiao* __fastcall TMapa::ExisteRegiao(AnsiString codigo)
   {
   //vari�veis locais
   VTRegiao *regiao;

   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      if (regiao->Codigo.AnsiCompareIC(codigo) == 0) return(regiao);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
bool __fastcall TMapa::ImportaRegiaoLatLon(AnsiString arq_latlon)
   {
   //vari�veis locais
   bool            sucesso;
   VTImportaRegiao *importa;

   try{//cria objeto VTImportaRegiao p/ fazer importa��o dos pontos
      importa = NewObjImportaRegiao(apl);
      importa->TipoCoordenada = coord_LATLON;
      sucesso = importa->Executa(arq_latlon);
      //inicia Eqptos das Mapa
      IniciaEqptosRegiao();
      //define Mapa vis�veis ou n�o
      DefineRegiaoVisivel();
      //destr�i objeto
      delete importa;
      }catch(Exception &e)
         {//nada a fazer
         return(false);
         }
   return(sucesso);
   }

//-----------------------------------------------------------------------------
bool __fastcall TMapa::ImportaRegiaoUtm(AnsiString arq_utm)
   {
   //vari�veis locais
   bool            sucesso;
   VTImportaRegiao *importa;

   try{//cria objeto VTImportaRegiao p/ fazer importa��o dos pontos
      importa = NewObjImportaRegiao(apl);
      importa->TipoCoordenada = coord_UTM;
      sucesso = importa->Executa(arq_utm);
      //define Mapa vis�veis ou n�o
      DefineRegiaoVisivel();
      //destr�i objeto
      delete importa;
      }catch(Exception &e)
         {//nada a fazer
         return(false);
         }
   return(sucesso);
   }

//-----------------------------------------------------------------------------
void __fastcall TMapa::IniciaEqptosRegiao(void)
   {
   //vari�veis locais
   VTRegiao *regiao;
   VTRedes  *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      regiao->ReiniciaLisEqpto(redes);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TMapa::InsereRegiao(VTRegiao *regiao)
   {
   //prote��o
   if (regiao == NULL) return;
   if (lisREGIAO->IndexOf(regiao) < 0)
      {
      lisREGIAO->Add(regiao);
      //redefine cor
      regiao->Color = SelecionaCor();
      }
   }

//-----------------------------------------------------------------------------
TList* __fastcall TMapa::LisRegiao(void)
   {
   return(lisREGIAO);
   }

//---------------------------------------------------------------------------
bool __fastcall TMapa::ReiniciaLisEqptoRegiao(void)
   {
   //vari�veis locais
   VTRedes *redes;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMapa::ReiniciaLisEqptoRegiao(VTRede *rede)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall TMapa::RetiraRegiao(VTRegiao *regiao)
   {
   //prote��o
   if (regiao == NULL) return;
   //retira Regiao de lisREGIAO
   lisREGIAO->Remove(regiao);
   //destr�i objeto
   delete regiao;
   }

//-----------------------------------------------------------------------------
TColor __fastcall TMapa::SelecionaCor(void)
   {
   //vari�veis locais
   int    index;
   TColor vet_cor[] = {clBlue,   clRed,    clGreen,  clYellow,  clSkyBlue, clFuchsia,
                       clOlive,  clPurple, clNavy,   clMaroon,  clTeal, clLime
                        };

   index = lisREGIAO->Count % (sizeof(vet_cor)/sizeof(TColor));
   return(vet_cor[index]);
   }

//---------------------------------------------------------------------------
//eof


