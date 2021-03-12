//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Util\TExtraiCampos.h>
#include "TParseCustom.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TParseCustom::TParseCustom(VTApl *apl)
   {
   //define ponteiro
   this->apl = apl;
   //cria obejtos
   lines      = new TStringList();
   campos     = new TStringList();
   subStrings = new TStringList();
   }

//-----------------------------------------------------------------------------
__fastcall TParseCustom::~TParseCustom(void)
   {
   //destrói objetos
   if (lines)      {delete lines;  lines  = NULL;}
   if (campos)     {delete campos; campos = NULL;}
   if (subStrings) {delete subStrings; subStrings = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TParseCustom::ConverteTensaoParaPrimario(double rel_tp)
   {//DVK 2015.04.09
   //variáveis locais
   complex<double> va_pri, vb_pri, vc_pri;
   complex<double> va_sec, vb_sec, vc_sec;
   complex<double> v0, v1, v2;
   double          va, vb, vc;

   //não converte se a relação for 1:1 ou angulo de defasagem for -1
   if((ang_defasagem == -1) || IsDoubleZero(rel_tp - 1.0))
      {//insere tensões em fase
      registro.v1_cmplx = polar(registro.v1,0.);
      registro.v2_cmplx = polar(registro.v2,0.);
      registro.v3_cmplx = polar(registro.v3,0.);
      return;
      }
   //guarda valores de tensão no secundario
   va = registro.v1;
   vb = registro.v2;
   vc = registro.v3;
   //define os angulos
   va_sec = polar(va, DegToRad(double(ang_defasagem)));  //VAB_pri
   vb_sec = polar(vb, DegToRad(ang_defasagem + 120.));   //VBC_pri
   vc_sec = polar(vc, DegToRad(ang_defasagem - 120.));   //VCA_pri
   //calcula componentes simétricas
   v0 = CZero();
   v1 = (1./3.)*((va_sec + (Alfa()*vb_sec)) + (Alfa2()*vc_sec));
   v2 = (1./3.)*((va_sec + (Alfa2()*vb_sec)) + (Alfa()*vc_sec));
   //determina as tensões no primário
   va_pri = v1 + v2;
   vb_pri = (v1*Alfa2()) + (v2*Alfa());
   vc_pri = (v1*Alfa()) + (v2*Alfa2());
   //salva novos valores multiplicados pela relação de transformação
   registro.v1_cmplx = va_pri * rel_tp;
   registro.v2_cmplx = vb_pri * rel_tp;
   registro.v3_cmplx = vc_pri * rel_tp;
   registro.v1 = Abs(registro.v1_cmplx);
   registro.v2 = Abs(registro.v2_cmplx);
   registro.v3 = Abs(registro.v3_cmplx);
   }

//---------------------------------------------------------------------------
bool __fastcall TParseCustom::Executa(AnsiString filename, VTMedicao *medicao,
                                      strMEDIDOR *medidor)
   {
   //variáveis locais
   bool sucesso = true;

   //verifica qual maior indice de coluna
   maxCol = medidor->colData;
   if(medidor->colHora > maxCol){maxCol = medidor->colHora;}
   if(medidor->colV1 > maxCol)  {maxCol = medidor->colV1;}
   if(medidor->colV2 > maxCol)  {maxCol = medidor->colV2;}
   if(medidor->colV3 > maxCol)  {maxCol = medidor->colV3;}
   //salva ponteiro
   this->medicao = medicao;
   //inicia dados
   registro.num_seq = 0;
   ang_defasagem = medidor->ang_defasagem;
   try{//carrega dados do arquivo txt
      lines->LoadFromFile(filename);
      for (int nl = 0; (nl < lines->Count) && sucesso; nl++)
         {//extrai campos da linha
         if (! ExtraiCampos(lines->Strings[nl], campos)) continue;
         //descarta linha com menos de maxCol campos
         if (campos->Count < maxCol) continue;
         //valida registro
         switch (InsereRegistroMedicao(medidor))
            {
            case diagOK:
            case diagCONTINUA:       continue;
            case diagINTERROMPE: return(false);
            }
         }
      }catch(Exception &E)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//-----------------------------------------------------------------------------
bool __fastcall TParseCustom::ExtraiCampos(AnsiString txt, TStringList *lista)
   {
   int fim = 1;

   //limpa campos
   campos->Clear();
   //extrai campos
   ExtraiStrings(txt, " ;\t", campos);
   for (int n = 0; n < lista->Count; n++)
      {
      lista->Strings[n] = lista->Strings[n].Trim();
      }
   return(lista->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TParseCustom::LeData(AnsiString data)
{
   subStrings->Clear();
   if(data.IsEmpty()){return false;}
   else
   {
      ExtraiStrings(data, "/-", subStrings);
      try
      {  //se o dia nao estiver no intervalo 1<dia<31 retorna falso
         if((StrToInt(subStrings->Strings[0])<1)||(StrToInt(subStrings->Strings[0])>31)){return false;}
         else{registro.dia = StrToInt(subStrings->Strings[0]);}
         //se o mes nao estiver no intervalo 1<mes<12 retorna falso
         if((StrToInt(subStrings->Strings[1])<1)||(StrToInt(subStrings->Strings[1])>12)){return false;}
         else{registro.mes = StrToInt(subStrings->Strings[1]);}
         if (subStrings->Count ==3) {registro.ano = StrToInt(subStrings->Strings[2]);}
         return true;
      }
      catch (Exception &e)
      {
         return false;
      }
   }
}

//---------------------------------------------------------------------------
bool __fastcall TParseCustom::LeHora(AnsiString hora)
{
   subStrings->Clear();
   if(hora.IsEmpty()){return false;}
   else
   {
      ExtraiStrings(hora, ":", subStrings);
      try
      {
         registro.hora    = StrToInt(subStrings->Strings[0]);
         registro.minuto  = StrToInt(subStrings->Strings[1]);
         return true;
      }
      catch (Exception &e)
      {
         return false;
      }
   }
}

//---------------------------------------------------------------------------
bool __fastcall TParseCustom::LeTensao(double *campo, AnsiString tensao)
{
      if(tensao.IsEmpty()){*campo = 0.0; return true;}
      try
      {
         if (! StrToDouble(tensao, *campo))  return false;
         return true;
      }
      catch (Exception &e)
      {
         return false;
      }
}
//---------------------------------------------------------------------------
bool __fastcall   TParseCustom::PreencheStringGrid(AnsiString filename, TStringGrid *grid)
{
   //variáveis locais
   bool        sucesso = true;
   int         numCol, numRow;
   TStringList *lista;

   lista = new TStringList();
   //inicia dados
   try{//carrega dados do arquivo txt
      lines->LoadFromFile(filename);
      //medir grid necessário
      numRow = lines->Count;
      ExtraiStrings(lines->Strings[numRow/2]," ;\t", lista);
      numCol = lista->Count;
      lista->Clear();
      grid->ColCount = numCol;
      if(numRow > 10)
      {//exibir somente as 10 primeiras linhas
         grid->RowCount = 10+ grid->FixedRows;
         for(int nc = 0; nc < numCol; nc++)
         {
            grid->Cells[nc][0] = nc+1;
         }
         for (int nl = grid->FixedRows; nl <= grid->RowCount; nl++)
         {//extrai campos da linha
            //ExtraiCampos(lines->Strings[nl], lista)
            ExtraiStrings(lines->Strings[nl], " ;\t", lista);

            for (int i = 0; i< lista->Count; i++)
            {
               grid->Cells[i][nl] = lista->Strings[i];
            }
         }
      }
      }catch(Exception &E)
      {
         delete lista;
         sucesso = false;
      }
   delete lista;
   return(sucesso);
}
/*
//-----------------------------------------------------------------------------
int __fastcall TParseCustom::InsereRegistroMedicao(strMEDIDOR *medidor)
   {
   //variáveis locais
   int diagnostico = diagOK;

   //inicializa estrutura
   registro.v1 = 0;
   registro.v2 = 0;
   registro.v3 = 0;
   try{//proteção:
      if (campos->Count < maxCol) return false;
      //inicia atributos do Registro
      if(! LeData(campos->Strings[(medidor->colData -1)])){ return(diagCONTINUA);}
      if(! LeHora(campos->Strings[(medidor->colHora -1)])){ return(diagCONTINUA);}
      if(! LeTensao(&registro.v1, campos->Strings[(medidor->colV1 -1)])){ return(diagCONTINUA);}
      //testar caso tenha menos fases que 3
      if(medidor->numFases > 1)
         {
         if(! LeTensao(&registro.v2, campos->Strings[(medidor->colV2 -1)])){ return(diagCONTINUA);}
         }
      if(medidor->numFases > 2)
         {
         if(! LeTensao(&registro.v3, campos->Strings[(medidor->colV3 -1)])){ return(diagCONTINUA);}
         }
      //DVK 2015.04.09 converte valores de tensão para o primario do trafo
      ConverteTensaoParaPrimario(medidor->relTP);
      //habilita fases com medição
      if (! IsDoubleZero(registro.v1)) medicao->FaseEnabled[indFASE_A] = true;
      if (! IsDoubleZero(registro.v2)) medicao->FaseEnabled[indFASE_B] = true;
      if (! IsDoubleZero(registro.v3)) medicao->FaseEnabled[indFASE_C] = true;
      //insere Registro em Medicao
      registro.num_seq++;
      diagnostico = medicao->InsereMedicao(registro);
      }catch(Exception &E)
         {
         return(diagnostico);
         }
   return(diagnostico);
   }
*/
//-----------------------------------------------------------------------------
int __fastcall TParseCustom::InsereRegistroMedicao(strMEDIDOR *medidor)
   {
   //variáveis locais
   int diagnostico   = diagOK;

   //inicializa estrutura
   registro.v1 = 0;
   registro.v2 = 0;
   registro.v3 = 0;
   try{//proteção:
      if (campos->Count < maxCol) return false;
      //inicia atributos do Registro (data e hora)
      if(! LeData(campos->Strings[(medidor->colData -1)])) {return(diagCONTINUA);}
      if(! LeHora(campos->Strings[(medidor->colHora -1)])) {return(diagCONTINUA);}
      //valores de tensão
      if(! LeTensao(&registro.v1, campos->Strings[(medidor->colV1 -1)])) {return(diagCONTINUA);}
      if(! LeTensao(&registro.v2, campos->Strings[(medidor->colV2 -1)])) {return(diagCONTINUA);}
      if(! LeTensao(&registro.v3, campos->Strings[(medidor->colV3 -1)])) {return(diagCONTINUA);}
      //DVK 2015.04.09 converte valores de tensão para o primario do trafo
      ConverteTensaoParaPrimario(medidor->relTP);
      //habilita fases com medição
      if (! IsDoubleZero(registro.v1)) medicao->FaseEnabled[indFASE_A] = true;
      if (! IsDoubleZero(registro.v2)) medicao->FaseEnabled[indFASE_B] = true;
      if (! IsDoubleZero(registro.v3)) medicao->FaseEnabled[indFASE_C] = true;
      //insere Registro em Medicao
      registro.num_seq++;
      diagnostico = medicao->InsereMedicao(registro);
      }catch(Exception &E)
         {
         return(diagnostico);
         }
   return(diagnostico);
   }

//---------------------------------------------------------------------------
//eof


