//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TImportaRegiao.h"
#include "VTRegiao.h"
#include "VTRegioes.h"
#include "VTPoligono.h"
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Cartografia\VTScg.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Cartografia.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTImportaRegiao* __fastcall NewObjImportaRegiao(VTApl *apl)
   {
   try{//cria objeto TImportaRegiao
      return(new TImportaRegiao(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TImportaRegiao::TImportaRegiao(VTApl *apl)
   {
   //variáveis locais
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   //salva objetos
   this->apl = apl;
   //assume que o coordenadas utm
   TipoCoordenada = coord_UTM;
   //cria Strings
   lines  = new TStringList();
   campos = new TStringList();
   scg    = DLL_NewObjScg(apl, licenca->Empresa, licenca->EmpresaStr);
   }

//---------------------------------------------------------------------------
__fastcall TImportaRegiao::~TImportaRegiao(void)
   {
   //destrói objetos
   if (lines)  {delete lines;  lines  = NULL;}
   if (campos) {delete campos; campos = NULL;}
   if (scg)    {delete scg;    scg    = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaRegiao::Executa(AnsiString arq_txt)
   {
   //variáveis locais
   AnsiString codigo;
   double     lat, lon, utm_x, utm_y;
   //double     z;
   VTRegioes  *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));

   //proteção
   if (! FileExists(arq_txt)) return(NULL);
   //lê arquivo
   lines->LoadFromFile(arq_txt);
   //extrai campos
   if (ExtraiCampos(lines->Strings[0]) == 0) return(NULL);
   //determina código da Regiao
   codigo = campos->Strings[0];
   //verifica se a Regiao já existe
   if ((regiao = regioes->ExisteRegiao(codigo)) != NULL)
      {//destrói Regiao existente
      regioes->RetiraRegiao(regiao);
      }
   //cria uma nova regiao
   if ((regiao = regioes->CriaRegiao()) == NULL) return(NULL);
   //determina nome da Região
   regiao->Codigo = codigo;
   //verifica o tipo de unidade das coordenadas
   if (TipoCoordenada == coord_UTM)
      {//loop de leitura das coordenadas utm_x, utm_y, z
      for (int n = 1; n < campos->Count-2; n+=3)
         {
         utm_x = StrToDouble(campos->Strings[n]);
         utm_y = StrToDouble(campos->Strings[n+1]);
         //z     = StrToDouble(campos->Strings[n+2]);
         //insere nova Barra
         if (! InserePontoUtm(utm_x, utm_y)) return(NULL);
         }
      }
   else
      {//loop de leitura das coordenadas lat/lon: x, y, z
      for (int n = 1; n < campos->Count-2; n+=3)
         {
         lon = StrToDouble(campos->Strings[n]);
         lat = StrToDouble(campos->Strings[n+1]);
         //z   = StrToDouble(campos->Strings[n+2]);
         //insere nova Barra
         if (! InserePontoLatLon(lat, lon)) return(NULL);
         }
      }
   //proteção
   if (regiao->Poligono->Size < 3)
      {//retira Regiao criada
      regioes->RetiraRegiao(regiao);
      regiao = NULL;
      }
   return(regiao != NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TImportaRegiao::ExtraiCampos(AnsiString txt)
   {
   //variáveis locais
   #define EOS '\0'
   char    *p_ini, *p_fim;
   char    separador = ';';
   int     len       = txt.Length();

   //reinicia StrinList campos
   campos->Clear();
   //filtra linha com menos de 2 caracteres
   if (len < 2) return(0);
   //filtra linha de comentário
   if ((txt[1] == '/') && (txt[2] == '/')) return(0);
   //se necessário, inclui separador no final de txt
   if (txt[len] != separador) txt += separador;
   //extrai os campos delimitados por separador em txt
   p_ini = txt.c_str();
   while ((p_fim = strchr(p_ini, separador)) != NULL)
      {//insere string na lista
      *p_fim = EOS; //insere delimitador de string
      campos->Add(AnsiString(p_ini).Trim());
      //copia em pstr o substring p_ini a p_fim-1
      *p_fim = separador;     //restaura separador no string original
      p_ini = p_fim + 1;
      }
   return(campos->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TImportaRegiao::IniciaZonaCoordenadasUtm(void)
   {
   //variáveis locais
   AnsiString 	UTMZoneX, UTMZoneY;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   ReferenceEllipsoid = 23;
	//UTMZoneX = "24"; UTMZoneY = "K";   //	EDP_ESCELSA:
   switch(licenca->Empresa)
		{
		case SINAPSIS:            {UTMZoneX = "23"; UTMZoneY = "K"; break;}
		case ELETROPAULO:         {UTMZoneX = "23"; UTMZoneY = "K"; break;}
		case RGESUL:              {UTMZoneX = "22"; UTMZoneY = "J"; break;}
		case ELETROBRAS_ALAGOAS : {UTMZoneX = "23"; UTMZoneY = "K"; break;}
		case CEMIG:               {UTMZoneX = "23"; UTMZoneY = "K"; break;}
		case COPEL:               {UTMZoneX = "22"; UTMZoneY = "K"; break;}
		case EDP_BANDEIRANTES:    {UTMZoneX = "23"; UTMZoneY = "K"; break;}
		case EDP_ESCELSA:         {UTMZoneX = "24"; UTMZoneY = "K"; break;}
		case FECOERGS:            {UTMZoneX = "22"; UTMZoneY = "J"; break;}
		case ELETROBRAS_AMAZONAS: {UTMZoneX = "20"; UTMZoneY = "M"; break;}
		default: //lê arquivo de configuração
         //if (! LeArquivoZonaEmpresa()) {UTMZoneX = "23"; UTMZoneY = "K";}
         UTMZoneX = "23"; UTMZoneY = "K";
         break;
      }
   UTMZone = UTMZoneX + UTMZoneY;
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaRegiao::InserePontoLatLon(double lat, double lon)
   {
   //variáveis locais
   double utm_x, utm_y;

   //converte lat/lon para coordendas utm
   scg->LatLon_to_NorthEast(lat, lon, utm_y, utm_x);
   //insere Ponto em coordenadas utm
   return(InserePontoUtm(utm_x, utm_y));
   }

//---------------------------------------------------------------------------
bool __fastcall TImportaRegiao::InserePontoUtm(double utm_x, double utm_y)
   {
   //insere ponto no Poligono da Regiao
   return(regiao->Poligono->InserePonto(utm_x *100., utm_y * 100.));
   }

//---------------------------------------------------------------------------
//eof

