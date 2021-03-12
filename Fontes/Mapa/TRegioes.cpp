//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRegioes.h"
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
VTRegioes* __fastcall NewObjRegioes(VTApl *apl)
   {
   return(new TRegioes(apl));
   }

//---------------------------------------------------------------------------
__fastcall TRegioes::TRegioes(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria lista
   lisREGIAO = new TList();
   //cria a lista auxilar
   lisAUX = new TList();
   lisREGIAO_ORD = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TRegioes::~TRegioes(void)
   {
   //destrói listas e seus objetos
   if (lisREGIAO) {LimpaTList(lisREGIAO); delete lisREGIAO; lisREGIAO = NULL;}
   //destroi a lista criada
   if(lisAUX){delete lisAUX; lisAUX = NULL;}
   if(lisREGIAO_ORD){delete lisREGIAO_ORD; lisREGIAO_ORD = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TRegioes::AreaRedeContemAreaRegiao(VTRegiao *regiao)
   {
   //variáveis locais
   VTRedes *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTArea  *area_rede   = redes->AreaUtm_cm();
   VTArea  *area_regiao = regiao->Poligono->AreaUtm_cm();

   //verifica se Área das Redes contém um dos pontos da área retangular da Regiao
   if (area_rede->ContemPonto(area_regiao->x1, area_regiao->y1)) return(true);
   if (area_rede->ContemPonto(area_regiao->x2, area_regiao->y2)) return(true);
   //verifica se Área da Regiao contém um dos pontos da área das Redes
   if (area_regiao->ContemPonto(area_rede->x1, area_rede->y1)) return(true);
   if (area_regiao->ContemPonto(area_rede->x2, area_rede->y2)) return(true);

   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TRegioes::Clear(void)
   {
   //destrói todas as Regioes
   LimpaTList(lisREGIAO);
   }

//-----------------------------------------------------------------------------
VTRegiao* __fastcall TRegioes::CriaRegiao(void)
   {
   //variáveis locais
   VTRegiao *regiao;

   //cria nova Regiao e insere na lista
   if ((regiao = NewObjRegiao(apl)) != NULL)
      {//define código
      regiao->Codigo = "Região Nova";
      InsereRegiao(regiao);
      }
   return(regiao);
   }

//-----------------------------------------------------------------------------
void __fastcall TRegioes::DefineRegiaoVisivel(void)
   {
   //variáveis locais
   VTRegiao *regiao;

   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      regiao->Visible = AreaRedeContemAreaRegiao(regiao);
      }
   }

//-----------------------------------------------------------------------------
VTRegiao* __fastcall TRegioes::ExisteRegiao(int regiao_id, bool somenteVisible)
   {
   //variáveis locais
   VTRegiao *regiao;

   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      if (regiao->Id == regiao_id)
         {//se for procurar somente as visiveis
         if(!somenteVisible)
            {
            return(regiao);
            }
         else
            {//se tiver que ser visible verifica se é visible
            if(regiao->Visible)
               {
               return(regiao);
               }
            }
         }
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTRegiao* __fastcall TRegioes::ExisteRegiao(AnsiString codigo, bool somenteVisible)
   {
   //variáveis locais
   VTRegiao *regiao;

   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      if (regiao->Codigo.AnsiCompareIC(codigo) == 0)
         {//se for procurar somente as visiveis
         if(!somenteVisible)
            {
            return(regiao);
            }
         else
            {//se tiver que ser visible verifica se é visible
            if(regiao->Visible)
               {
               return(regiao);
               }
            }
         }
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
//VTRegiao* __fastcall TRegioes::ExisteRegiao(TPoint *point_cm)
//   {
//   //variáveis locais
//   VTRegiao *regiao;
//
//   for (int n = 0; n < lisREGIAO->Count; n++)
//      {
//      regiao = (VTRegiao*)lisREGIAO->Items[n];
//      if (regiao->Poligono->ContemPonto(point_cm->X, point_cm->Y)) return(regiao);
//      }
//   return(NULL);
//   }
//-----------------------------------------------------------------------------
VTRegiao* __fastcall TRegioes::ExisteRegiao(TPoint *point_cm, bool somenteVisible)
   {
   //variáveis locais
   VTRegiao *regiao;

   //percorre a lista ao contrario do sentido que as regioes são desenhadas
   for (int n = (lisREGIAO->Count -1) ; n >= 0; n--)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      if (regiao->Poligono->ContemPonto(point_cm->X, point_cm->Y))
         {//se for procurar somente as visiveis
         if(!somenteVisible)
            {
            return(regiao);
            }
         else
            {//se tiver que ser visible verifica se é visible
            if(regiao->Visible)
               {
               return(regiao);
               }
            }
         }
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
void __fastcall TRegioes::ExisteRegioes(TList *lisREGIAO_vazia, TPoint *point_cm, bool somenteVisible)
   {//variáveis locais
   VTRegiao *regiao;

   //limpa a regiao
   //lisREGIAO_vazia->Clear();
   //percorre a lista ao contrario do sentido que as regioes são desenhadas
   for (int n = (lisREGIAO->Count -1) ; n >= 0; n--)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      if (regiao->Poligono->ContemPonto(point_cm->X, point_cm->Y))
         {//se for procurar somente as visiveis
         if(!somenteVisible)
            {
            lisREGIAO_vazia->Add(regiao);;
            }
         else
            {//se tiver que ser visible verifica se é visible
            if(regiao->Visible)
               {
               lisREGIAO_vazia->Add(regiao);
               }
            }
         }
      }
   }
//-----------------------------------------------------------------------------
bool __fastcall TRegioes::ImportaRegiaoLatLon(AnsiString arq_latlon)
   {
   //variáveis locais
   bool            sucesso;
   VTImportaRegiao *importa;

   try{//cria objeto VTImportaRegiao p/ fazer importação dos pontos
      importa = NewObjImportaRegiao(apl);
      importa->TipoCoordenada = coord_LATLON;
      sucesso = importa->Executa(arq_latlon);
      //inicia Eqptos das Regioes
      IniciaEqptosRegiao();
      //define Regioes visíveis ou não
      DefineRegiaoVisivel();
      //destrói objeto
      delete importa;
      }catch(Exception &e)
         {//nada a fazer
         return(false);
         }
   return(sucesso);
   }

//-----------------------------------------------------------------------------
bool __fastcall TRegioes::ImportaRegiaoUtm(AnsiString arq_utm)
   {
   //variáveis locais
   bool            sucesso;
   VTImportaRegiao *importa;

   try{//cria objeto VTImportaRegiao p/ fazer importação dos pontos
      importa = NewObjImportaRegiao(apl);
      importa->TipoCoordenada = coord_UTM;
      sucesso = importa->Executa(arq_utm);
      //define Regioes visíveis ou não
      DefineRegiaoVisivel();
      //destrói objeto
      delete importa;
      }catch(Exception &e)
         {//nada a fazer
         return(false);
         }
   return(sucesso);
   }

//-----------------------------------------------------------------------------
void __fastcall TRegioes::IniciaEqptosRegiao(void)
   {
   //variáveis locais
   VTRegiao *regiao;
   VTRedes  *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      regiao->ReiniciaLisEqpto(redes);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TRegioes::InsereRegiao(VTRegiao *regiao)
   {
   //proteção
   if (regiao == NULL) return;
   if (lisREGIAO->IndexOf(regiao) < 0)
      {
      lisREGIAO->Add(regiao);
      //redefine cor
      regiao->Color = SelecionaCor();
      }
   }
//-----------------------------------------------------------------------------
void __fastcall TRegioes::InsereRegiaoEFilhas(VTRegiao *regiao)
   {//variaveis locais
   VTRegiao *regiaoAux;

   //protecao
   if(!regiao)return;
   //adiciona pai
   lisREGIAO->Add(regiao);
   //percorre filhas
   for(int nr = 0; nr < regiao->LisRegiaoFilhas->Count; nr++)
      {
      regiaoAux = (VTRegiao*) regiao->LisRegiaoFilhas->Items[nr];
      InsereRegiaoEFilhas(regiaoAux);
      }
   }
//-----------------------------------------------------------------------------
bool __fastcall TRegioes::IsRegiao1InternaRegiao2(VTRegiao *regiao1, VTRegiao *regiao2)
{  //variaveis locais
   bool  interna = true;
   TPoint   *point_cm;
   TList    *lisPOINT;
   VTArea   *area1;

   //protecao
   if((!regiao1) || (!regiao2)){ return false;}
   if((!regiao1->Poligono) || (!regiao2->Poligono)){ return false;}
   if((regiao1->Poligono->LisPoint()->Count < 3) ||
                     (regiao2->Poligono->LisPoint()->Count < 3)){return false;}
   //primeiro verifica se o retangulo da regiao 1 é interno ao retangulo da regiao2
   area1 = regiao1->Poligono->AreaUtm_cm();
   //caso a area retangular da regiao1 estiver dentro da area retangular da regiao2...
   if(regiao2->Poligono->AreaInternaAreaRetangular(area1->x1,area1->y1,area1->x2,area1->y2))
      {//...verifica todos vertices
      lisPOINT = regiao1->Poligono->LisPoint();
      //verifica se algum vertice da regiao 1 está fora da 2
      for(int nv = 0; nv < lisPOINT->Count; nv++)
         {
         point_cm = (TPoint*) lisPOINT->Items[nv];
         if(!(regiao2->Poligono->ContemPonto(point_cm->X, point_cm->Y)))
            {
            return false;
            }
         }
      }
   else
      {//está fora (ou parcialmente fora =( ...)
      interna = false;
      }
   return (interna);
}
//-----------------------------------------------------------------------------
//int __fastcall TRegioes::ListaRegiaoContem(VTRegiao *regiaoA, TList *lisTODAS, TList *lisCONTEM_A)
//{  //variaveis internas
//   VTRegiao *regiaoB;
//
//   //protecao
//   if((!regiaoA) || (!lisTODAS) || (!lisCONTEM_A)) return -1;
//   //limpa a lista das regioes que contem A
//   lisCONTEM_A->Clear();
//   //percorre a lista de todas regioes disponiveis
//   for (int nr = 0; nr  < lisTODAS->Count ; nr++)
//   {
//      regiaoB = (VTRegiao*)lisTODAS->Items[nr];
//      //caso regiao B diferente de A, verifica
//      if(regiaoB != regiaoA)
//      {  //verifica se regiaoA  é interna à B
//         if(IsRegiao1InternaRegiao2(regiaoA,regiaoB))
//         {  //adiciona a regiao B À lista de regioes que contem A
//            lisCONTEM_A->Add(regiaoB);
//         }
//      }
//   }
//   return(lisCONTEM_A->Count);
//}

////-----------------------------------------------------------------------------
//int __fastcall TRegioes::ListaRegiaoContidas(VTRegiao *regiaoA, TList *lisTODAS, TList *lisCONTIDAS)
//{  //variaveis internas
//   VTRegiao *regiaoB;
//   /* TODO : removendo filhas, e for ao contrario */
//   //protecao
//   if((!regiaoA) || (!lisTODAS) || (!lisCONTIDAS)) return -1;
//   //limpa a lista das regioes que contem A
//   lisCONTIDAS->Clear();
//   //percorre a lista de todas regioes disponiveis
//   for (int nr = 0; nr  < lisTODAS->Count ; nr++)
//   {
//      regiaoB = (VTRegiao*)lisTODAS->Items[nr];
//      //caso regiao B diferente de A, verifica
//      if(regiaoB != regiaoA)
//      {  //verifica se regiao B  é interna à A
//         if(IsRegiao1InternaRegiao2(regiaoB,regiaoA))
//         {  //adiciona a regiao B À lista de regioes que são contidas em A
//            lisCONTIDAS->Add(regiaoB);
//         }
//      }
//   }
//   return(lisCONTIDAS->Count);
//}
//-----------------------------------------------------------------------------
int __fastcall TRegioes::ListaRegiaoContidas(VTRegiao *regiaoA, TList *lisTODAS, TList *lisCONTIDAS)
{  //variaveis internas
   VTRegiao *regiaoB;

   //protecao
   if((!regiaoA) || (!lisTODAS) || (!lisCONTIDAS)) return -1;
   //limpa a lista das regioes que contem A
   lisCONTIDAS->Clear();
   //percorre a lista de todas regioes disponiveis
   for (int nr = lisTODAS->Count - 1; nr  > 0 ; nr--)
   {
      regiaoB = (VTRegiao*)lisTODAS->Items[nr];
      //caso regiao B diferente de A, verifica
      if(regiaoB != regiaoA)
      {  //verifica se regiao B  é interna à A
         if(IsRegiao1InternaRegiao2(regiaoB,regiaoA))
         {  //adiciona a regiao B À lista de regioes que são contidas em A
            lisCONTIDAS->Add(regiaoB);
            //remove da lista de todas consultadas
            lisAUX->Remove(regiaoB);
            //remove deixa na lista das ordenadas só as nivel 0
            lisREGIAO_ORD->Remove(regiaoB);
            //arruma pai e level
            regiaoB->RegiaoPai = regiaoA;
            regiaoB->Level = regiaoA->Level + 1;
         }
      }
   }
   return(lisCONTIDAS->Count);
}
//-----------------------------------------------------------------------------
//void __fastcall TRegioes::ListaRegioesLevelZero(TList *lisVAZIA, TList *lisTODAS)
//{  //variaveis locais
//   VTRegiao *regiaoA, *regiaoB;
//   TList    *lisCONTEM = new TList();
//
//   //protecao
//   if((!lisVAZIA) || (!lisTODAS)) return;
//   //varre todas regioes
//   for(int nra = 0; nra < lisTODAS->Count; nra++)
//      {
//      regiaoA = (VTRegiao*) lisTODAS->Items[nra];
//      //se a regiao nao for contida por outras
//      if(ListaRegiaoContem(regiaoA, lisTODAS, lisCONTEM) == 0)
//         {
//         lisVAZIA->Add(regiaoA);
//         regiaoA->Level = 0;
//         }
//      }
//   //deleta a lista criada
//   if(lisCONTEM){delete lisCONTEM; lisCONTEM = NULL;}
//}
//-----------------------------------------------------------------------------
TList* __fastcall TRegioes::LisRegiao(void)
   {
   return(lisREGIAO);
   }
////---------------------------------------------------------------------------
void __fastcall  TRegioes::OrdenaArvore(TList *lisRegiao)
{  //variaveis locais
   VTRegiao *regiao;

   //protecao
   if(!lisRegiao) return;
   //limpa o lisAUX
   lisAUX->Clear();
   lisREGIAO_ORD->Clear();
   //copia a lista de regioes pra temp
   CopiaTList(lisRegiao,lisAUX);
   CopiaTList(lisRegiao,lisREGIAO_ORD);
   //inicializa todas regioes com level = 0
   for(int nr = 0; nr < lisRegiao->Count; nr++)
      {
      regiao = (VTRegiao*) lisRegiao->Items[nr];
      regiao->Level = 0;
      }
   //percorre a lista temporaria
   while (lisAUX->Count > 0)
      {
      regiao = (VTRegiao*) lisAUX->First();
      OrdenaArvoreRegiao(regiao,lisRegiao);
      }
   //ordena a lista de regioes (primeiro pai, dps filhas, netas..etc)
   OrdenaLisRegiao();
}
//---------------------------------------------------------------------------
void __fastcall TRegioes::OrdenaArvoreRegiao(VTRegiao *regiaoA, TList *lisRegiaoParcial)
{  //variaveis locais
   VTRegiao *regiaoB;

   //lista as regioes contidas em regiao
   ListaRegiaoContidas(regiaoA, lisRegiaoParcial, regiaoA->LisRegiaoFilhas);
   //caso tenha filhas
   if(regiaoA->LisRegiaoFilhas->Count != 0)
      {
      //percorre a lista de filhas procurando entre elas
      for(int nr = 0; nr < regiaoA->LisRegiaoFilhas->Count; nr++)
         {//verifica cada regiao filha
         regiaoB = (VTRegiao*) lisRegiaoParcial->Items[nr];
         //verifica se não estão dentro uma da outra
         OrdenaArvoreRegiao(regiaoB,regiaoA->LisRegiaoFilhas);
         }
      }
   //remove da lista de regioes não verificadas
   lisAUX->Remove(regiaoA);
}
//---------------------------------------------------------------------------
void __fastcall TRegioes::OrdenaLisRegiao(void)
{  //variaveis locais
   VTRegiao *regiao;

   //protecao
   if((!lisREGIAO) || (!lisREGIAO_ORD))return;
   //protecao caso a lista das regioes nivel 0 for maior que a lista de todas regioes
   if(lisREGIAO_ORD->Count > lisREGIAO->Count) return;
   //limpa lista de regioes
   lisREGIAO->Clear();
   //percorre  as regioes nivel 0
   for(int nr = 0; nr < lisREGIAO_ORD->Count; nr++)
      {
      regiao = (VTRegiao*) lisREGIAO_ORD->Items[nr];
      InsereRegiaoEFilhas(regiao);
      }
}
//---------------------------------------------------------------------------
bool __fastcall TRegioes::ReiniciaLisEqptoRegiao(void)
   {
   //variáveis locais
   VTRedes *redes;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegioes::ReiniciaLisEqptoRegiao(VTRede *rede)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall  TRegioes::RegioesVisible(TList *lisRegiaoParcial, bool visible)
{  //variáveis locais
   //TList       *lisREGIAOtodas = this->LisRegiao();
   VTRegiao    *regiao;

   //protecao
   if(!lisRegiaoParcial) return;
   //altera as regioes
   for(int nr = 0; nr < lisRegiaoParcial->Count; nr++)
		{
      regiao          = (VTRegiao*)lisRegiaoParcial->Items[nr];
      regiao->Visible = visible;
		}
}
//-----------------------------------------------------------------------------
void __fastcall  TRegioes::RegioesVisibleComplementar(TList *lisRegiaoParcial, bool visible)
{  //variáveis locais
   TList       *lisREGIAOtodas = this->LisRegiao();
   VTRegiao    *regiao;

   //protecao
   if(!lisRegiaoParcial) return;
   //inicializa todas as regioes para o valor contrario de visible
   for(int nr = 0; nr < lisREGIAOtodas->Count; nr++)
		{
      regiao          = (VTRegiao*)lisREGIAOtodas->Items[nr];
      regiao->Visible = (!visible);
		}
   //altera as regioes
   for(int nr = 0; nr < lisRegiaoParcial->Count; nr++)
		{
      regiao          = (VTRegiao*)lisRegiaoParcial->Items[nr];
      regiao->Visible = visible;
		}
}
//-----------------------------------------------------------------------------
void __fastcall TRegioes::RetiraRegiao(VTRegiao *regiao)
   {
   //proteção
   if (regiao == NULL) return;
   //retira Regiao de lisREGIAO
   lisREGIAO->Remove(regiao);
   //destrói objeto
   delete regiao;
   }

//-----------------------------------------------------------------------------
TColor __fastcall TRegioes::SelecionaCor(void)
   {
   //variáveis locais
   int    index;
   TColor vet_cor[] = {clBlue,   clRed,    clGreen,  clYellow,  clSkyBlue, clFuchsia,
                       clOlive,  clPurple, clNavy,   clMaroon,  clTeal, clLime
                        };
                        
   index = lisREGIAO->Count % (sizeof(vet_cor)/sizeof(TColor));
   return(vet_cor[index]);
   }

//---------------------------------------------------------------------------
//eof

