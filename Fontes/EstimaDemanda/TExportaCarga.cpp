//---------------------------------------------------------------------------
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTFases.h"
#include "TExportaCarga.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TExportaCarga::TExportaCarga(VTApl *apl_owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   path      = (VTPath*)apl->GetObject(__classid(VTPath));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //cria lista
   lisCAR = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TExportaCarga::~TExportaCarga(void)
   {
   //destrói lista sem destrui seus objetos
   if (lisCAR) {delete lisCAR; lisCAR = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaCarga::Executa(AnsiString filename, int ind_pat)
   {
   //variáveis locais
   VTCarga   *carga;

   try{//abre arquivo para gravação
      filename = path->DirTmp() + "\\" + filename;
      if ((fp = fopen(filename.c_str(), "w")) == NULL) return(false);
      //insere string com nomes dos campos
      fputs("//Carga_ID; Barra_ID; Fases; (Pa; Qa); (Pb; Qb); (Pc; Qc); Classe\n", fp);
      //inicia lista com todas Cargas
      lisCAR->Clear();
      redes->LisEqpto(lisCAR, eqptoCARGA);
      //loop para todas Cargas
      for (int n = 0; n < lisCAR->Count; n++)
         {
         carga = (VTCarga*)lisCAR->Items[n];
         GravaCarga(carga, ind_pat);
         }
      //fecha arquivo
      fclose(fp);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaCarga::GravaCarga(VTCarga *carga, int ind_pat)
   {
   //variáveis locais
   AnsiString      txt, fases_str;
   int             fases_car, carga_id, barra_id;
   complex<double> s_mva[MAX_FASE];

   try{//determina fases da Carga
      carga_id  = carga->Id;
      barra_id  = carga->pbarra->Id;
      fases_car = carga->Fases;
      fases_str = fases->AsStrABCN[fases_car];
      //inicia string com ID da Carga, ID da Barra, fases
      txt.sprintf("%d; %d; %s;", carga_id, barra_id, fases_str.c_str());
      //determina demanda por fase da Carga no Patamar
      if (demanda->Get_S3f_mva(carga, ind_pat, s_mva))
         {
         for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
            {
            txt = txt + DoubleToStr(" %4.3f;", s_mva[ind_fase].real() * 1000.);
            txt = txt + DoubleToStr(" %4.3f;", s_mva[ind_fase].imag() * 1000.);
            }
         //inclui classe de consumidor
         switch(carga->classe->Tag)
            {
            case consRESIDENCIAL: txt += " RES;"; break;
            case consCOMERCIAL:   txt += " COM;"; break;
            case consINDUSTRIAL:  txt += " IND;"; break;
            case consRURAL:       txt += " RUR;"; break;
            case consOUTROS:      txt += " OUT;"; break;
            case consIP:          txt += " IP;";  break;
            case consA4:          txt += " A4;";  break;
            case consEQUIVALENTE: txt += " EQV;"; break;
            case consA2:          txt += " A2;";  break;
            default:              txt += " IND;"; break;
            }
         }
      //inclui new line
      txt += "\n";
      //grava dados no arquivo
      fputs(txt.c_str(), fp);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

