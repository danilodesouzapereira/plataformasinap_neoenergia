//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Filectrl.hpp>
#include "TExportaSec.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Curvas\VTCurva.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\InterfaceTXT\Exporta\VTExportaTXT.h"
#include "..\..\Log\VTLog.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Potencia\VTFornecimento.h"
#include "..\..\Progresso\VTProgresso.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTSecundario.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTEqbar.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\..\DLL_Inc\Curvas.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\InterfaceTXT.h"
#include "..\..\..\DLL_Inc\Potencia.h"
#include "..\..\..\DLL_Inc\Progresso.h"
#include "..\..\..\DLL_Inc\Radial.h"
#include "..\..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTExportaSec* __fastcall NewObjExportaSec(VTApl *apl)
   {
   return(new TExportaSec(apl));
   }

//---------------------------------------------------------------------------
__fastcall TExportaSec::TExportaSec(VTApl *apl)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //salva ponteiros p/ objetos
   this->apl = apl;
   //cria objetos
   lisEQP  = new TList();
   //radial = DLL_NewObjRadial(apl);
   //verifica se existe o objeto VTRadial
   radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
   if (radial == NULL)
      {//cria objeto e insere em Apl
      apl->Add(radial = DLL_NewObjRadial(apl));
      //inicia objeto Radial
      radial->Inicia(redes);
      }
   }

//---------------------------------------------------------------------------
__fastcall TExportaSec::~TExportaSec(void)
   {
   //destrói objetos
   //if (radial)    {delete radial; radial = NULL;}
   //destrói listas sem destruir seus objetos
   if (lisEQP)  {delete lisEQP;  lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaSec::CriaArquivoTXT(VTSecundario *secundario, AnsiString dir)
   {
   //variavel local
   bool          sucesso;
   AnsiString    arquivo;
   VTExportaTXT *exportaTXT;

   //proteção
   if(secundario == NULL) return(false);
   if(dir.IsEmpty())      return(false);

   try{//define nome do arquivo TXT
      arquivo = dir + "\\" + secundario->Rede->Codigo + ".txt";
      //cria objeto ExportaTXT
      if ((exportaTXT = DLL_NewObjExportaTXT(apl)) == NULL) return(false);
      //configura opções do arquivo TXT
      exportaTXT->ExportaCurvaTipica  = true;
      exportaTXT->ExportaArranjo      = true;
      exportaTXT->ExportaCabo         = true;
      exportaTXT->ExportaSuporte      = true;
      exportaTXT->ExportaRede         = true;
      exportaTXT->ExportaReducao      = false;
      exportaTXT->ExportaTipoChave    = true;
      exportaTXT->ExportaTipoRede     = true;
      //exporta a Rede do Secundario
      sucesso = exportaTXT->ExportaUmaRede(secundario->Rede, arquivo);
      //destrói objeto ExportaTXT
      delete exportaTXT;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaSec::CriaDiretorio(AnsiString dir)
   {
   //variavel local
   VTLog  *log  = (VTLog*)  apl->GetObject(__classid(VTLog));
   
   //proteção
   if(dir.IsEmpty()) return(false);

   //verifica se já existe a rede primária: senão cria
   if (! DirectoryExists(dir))
      {//cria diretório com nome do Primário
      if(! CreateDir(dir))
         {
         //sem permissão p/ criar diretório
         if(log) log->Add("Erro ao criar diretorio: " + dir);
         return false;
         }
      }
   return true;
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaSec::Executa(bool showProgress)
   {
   //variáveis locais
   AnsiString   dir_base, dir_primario;
	VTProgresso  *prog  = (VTProgresso*)DLL_NewObjProgresso(NULL, NULL);
	VTPath       *path  = (VTPath*) apl->GetObject(__classid(VTPath));
   TList        *lisSEC, *lisPRI;
   VTPrimario   *primario;
   VTSecundario *secundario;

   //cria repositorio dos arquivos txt
   dir_base = path->DirExporta() + "\\RedesBT_TXT";
   if(! CriaDiretorio(dir_base)) return false;
   //obtem lista de redes primárias
   lisPRI = radial->LisPrimario();
   //obtém lista de redes secundárias
   lisSEC = radial->LisSecundario();
   //exibe progresso
   if(showProgress) prog->Start(progTEXTO);
   if(showProgress) prog->Add("Exportar redes em arquivos TXT");
   //loop p/ cada primario e seus respectivos secundarios
   for(int npri = 0; npri < lisPRI->Count; npri++)
      {
      primario = (VTPrimario*) lisPRI->Items[npri];
      //define nome do diretorio
      dir_primario = dir_base + "\\" + primario->Rede->Codigo;
      //cria diretorio p/ cada primario
      if(! CriaDiretorio(dir_primario)) continue;
      //obtem os respcectivos secundarios
      lisSEC   = primario->LisSecundario();
      //loop p/ todos Secundario
      for (int n = 0; n < lisSEC->Count; n++)
         {
         secundario = (VTSecundario*)lisSEC->Items[n];
         //incrementa
         if(showProgress) prog->Add(secundario->Rede->Codigo);
         //cria Suprimento na barra inicial do Secundario
         InsereSuprimento(secundario->BarraInicial, primario->Rede->Codigo);
         //cria arquivo TXT
         CriaArquivoTXT(secundario, dir_primario);
         //remove suprimento
         RemoveSuprimento(secundario->BarraInicial);
         }
      }
   if(prog)
      {
      prog->Stop();
      delete prog;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaSec::InsereSuprimento(VTBarra *barra, AnsiString codigo)
   {
   //variáveis locais
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTSuprimento *sup;
	VTCurva      *curva;
   VTFornecimento *fornec;

   //proteção
   if(! barra) return false;

   //cria objeto suprimento
   sup = DLL_NewObjSuprimento(patamares->Escala);
   if(! sup) return false;
   //define atributos do Suprimento
   sup->Id      = FalsoId();
   sup->Codigo  = codigo;
   sup->tiposup = supVF;
   //cria curva de suprimento 
   curva = DLL_NewObjCurva();
   if(! curva) {delete sup; return false;}
   curva                = DLL_NewObjCurva();
   curva->Id            = -1;
   //curva->Codigo        = "";
   curva->Tipo          = curvaPQVT;
   curva->Unidade       = unidMVA;
   curva->Escala        = patamares->Escala; //patEDITAVEL;
   curva->NumeroValores = 4;
   curva->Tipica        = false;
   //curva->Eqpto       = psup;
   curva->Classe        = NULL;

   curva->Reconfigura(patamares->Escala);
   sup->DefineCurva(curva);

   //cria objeto
	fornec  = DLL_NewObjFornecimento(apl);
   if(! fornec)  {delete sup; delete curva; return false;}

   //define tensão com 1 pu
   for(int n = 0; n < patamares->Escala; n++)
      fornec->V_pu[sup][n] = 1.0;

   //insere suprimento na barra
   barra->InsereEqbar(sup);

   //destroi objeto criado
   if(fornec)  {delete fornec; fornec = NULL;}
   
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TExportaSec::RemoveSuprimento(VTBarra *barra)
   {
   //variáveis locais
   VTEqbar      *eqbar;
   TList        *lisEqbar;

   //proteção
   if(! barra) return false;

   //cria objeto suprimento
   lisEqbar =  barra->LisEqbar();

   for(int neq = 0; neq < lisEqbar->Count; neq++)
      {
      eqbar = (VTEqbar*) lisEqbar->Items[neq];
      if(eqbar->Tipo() != eqptoSUPRIMENTO) continue;
      barra->RemoveEqbar(eqbar);
      DLL_DeleteEqpto(eqbar);
      //delete eqbar; eqbar = NULL;
      break;
      }

   return(true);
   }


//-----------------------------------------------------------------------------
// eof
