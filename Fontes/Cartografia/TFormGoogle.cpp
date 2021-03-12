//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormGoogle.h"
#include "TZoomGoogle.h"
#include "VTExportaKml.h"
#include "VTScg.h"
#include "VTCartografia.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Grafico\TPadrao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormGoogle::TFormGoogle(TComponent* Owner, VTApl *apl, TWinControl *parent)
   : TForm(Owner)
   {
   //variáveis locais
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   //salva objetos
   this->apl = apl;
   if (parent) Parent = parent;
   //cria objetos locais
   scg    = NewObjScg(apl, licenca->Empresa, licenca->EmpresaStr);
   Lines  = new TStringList();
   arvore = DLL_NewObjArvore(apl);
   //inicia dados p/ cálculo de coordenadas lat_lon
   z0        = 0.7111111111;
	//define tipo de mapa
	map_type    = mapaSATELLITE;
	//inicia TView
   TViewInicia();
   //carrega mapa da área da rede
//	IniciaMapa();     //temporariamente não mostra mapa miniatura, até corrigir o problema do Google Maps
	}

//---------------------------------------------------------------------------
__fastcall TFormGoogle::~TFormGoogle(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói objetos
   if (scg)    {delete scg;    scg    = NULL;}
   if (arvore) {delete arvore; arvore = NULL;}
   if (Lines)  {delete Lines;  Lines  = NULL;}
   }

//---------------------------------------------------------------------------
//semelhante a IniciaMapa() só que não considera zoom ZoomGoogle.MelhorValor
//---------------------------------------------------------------------------
	void __fastcall TFormGoogle::AtualizaMapa(void)
	{
	//variáveis locais
	AnsiString  mapa_nome;
	double      lat_med, lon_med;
	AREA_LL     area_ll;
	VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTArea      *area_utm  = redes->AreaUtm_cm();

	//converte coordenadas utm da área da Rede p/ latitude/longitude (coord.utm estão em cm)
//  scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100., area_ll.lat1, area_ll.lon1, scg->TipoCoord);
//  scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100., area_ll.lat2, area_ll.lon2, scg->TipoCoord);
    scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100., area_ll.lat1, area_ll.lon1);
    scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100., area_ll.lat2, area_ll.lon2);
	//calcula ponto médio da área
	lat_med = (area_ll.lat1 + area_ll.lat2) / 2;
	lon_med = (area_ll.lon1 + area_ll.lon2) / 2;
	//carrega a área do mapa selecionado
	switch (map_type)
		{
		case mapaSATELLITE: mapa_nome = "SATELLITE"; break;
		case mapaHYBRID:    mapa_nome = "HYBRID";    break;
		case mapaTERRAIN:   mapa_nome = "TERRAIN";   break;
		case mapaROADMAP:
		default:            mapa_nome = "ROADMAP";   break;
		}
	LoadMapa(mapa_nome, lat_med, lon_med, zoom, true);
	}

//---------------------------------------------------------------------------
void __fastcall TFormGoogle::BotaoKMLClick(TObject *Sender)
   {
   VTExportaKml *ExportaKml;
   VTRedes      *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTPath       *path   = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString   arq_kml = path->DirExporta() + "\\mapa_rede.kml";
	TList 		 *lisREDE = redes->LisRede();
	TList 		 *lisBARRA = redes->LisBarra();
	VTBarra		 *barra;
	int x,y;

	//verificações antes de executar ExportaKML
	for (int n = 0; n < lisBARRA->Count; n++)
		{
		//ver se as redes possuem coordenadas geográficas
		barra = (VTBarra*)lisBARRA->Items[n];
		if (! barra->CoordenadasUtm_cm(x,y))
			{
			Aviso("A rede carregada não possui coordenadas geográficas cadastradas.");
			return;
			}
		}
	//ver se existem redes abertas
	if (lisREDE->Count==0)
		{
		Aviso("Não há redes carregadas.");
		}
	//executa
	else
		{
		ExportaKml = NewObjExportaKml(apl);
		ExportaKml->Executa(redes->LisRede(), arq_kml);
		delete ExportaKml;
		//mensagem
		Aviso("O arquivo " + AnsiQuotedStr(arq_kml, '\'') + " foi criado corretamente");
		}
	}
/*
//---------------------------------------------------------------------------
void __fastcall TFormGoogle::DesenhaRede(void)
   {
   //variáveis locais
   int        x, y;
   double     lat, lon;
   TList     *lisBAR;
   VTBarra   *barra;
   VTRede    *rede;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisREDE = redes->LisRede();

   //loop p/ todas redes
   for (int n = 0; (n < 1)&&(n < lisREDE->Count); n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //insere coordenadas da rede
      Lines->Add("var flightPlanCoordinates = [");
      //loop p/ todas Ligacoes da Rede
      lisBAR = rede->LisBarra();
      for (int nb = 0; nb < lisBAR->Count; nb++)
         {
			barra = (VTBarra*)lisBAR->Items[nb];
			//obtém coordenadada Barra
         barra->CoordenadasUtm_cm(x, y);
         //converte coordenadas utm p/ latlon
			UTMtoLL(EllipsoId, y / 100., x / 100., UTMZone.c_str(), lat, lon);
         //insere coordenadas no mapa
         if (nb < (lisBAR->Count-1) ) Lines->Add(HTML_Ponto(lat, lon) + ",");
         else                         Lines->Add(HTML_Ponto(lat, lon));
         }
      Lines->Add("]");
      //define polyline
      Lines->Add("var flightPath = new google.maps.Polyline({");
      Lines->Add("   path: flightPlanCoordinates,");
      Lines->Add("   strokeColor: \"#FF0000\",     strokeOpacity: 1.0,     strokeWeight: 2");
      Lines->Add("});");
      }
   Lines->Add("flightPath.setMap(map);");
  }
*/
//---------------------------------------------------------------------------
void __fastcall TFormGoogle::DesenhaRede(void)
   {
	//variáveis locais
   AnsiString pontos_name, linha_name;
   int        x1, y1, x2, y2;
   double     lat1, lon1, lat2, lon2;
   TList     *lisLIG;
   VTLigacao *ligacao;
   VTRede    *rede;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisREDE = redes->LisRede();

   //loop p/ todas redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
	   //loop p/ todas Ligacoes da Rede visíveis
	   if(! rede->Visible()) continue;
	   lisLIG = rede->LisLigacao();
      //for (int nl = 0; (num_linha < 300)&&(nl < lisLIG->Count); nl++, num_linha++)
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //obtém coordenadada Barra
         ligacao->CoordenadasUtm_cm(x1, y1, x2, y2);
         //insere coordenadas da rede
         pontos_name.sprintf("pontos%d%d", nr+1, nl+1);
         Lines->Add("var " + pontos_name + " = [");
         //converte coordenadas utm p/ latitude/longitude (coord.utm estão em cm)
//       scg->NorthEast_to_LatLon(y1 / 100., x1 / 100., lat1, lon1, scg->TipoCoord);
//       scg->NorthEast_to_LatLon(y2 / 100., x2 / 100., lat2, lon2, scg->TipoCoord);
         scg->NorthEast_to_LatLon(y1 / 100., x1 / 100., lat1, lon1);
         scg->NorthEast_to_LatLon(y2 / 100., x2 / 100., lat2, lon2);
         //insere coordenadas no mapa
         Lines->Add(HTML_Ponto(lat1, lon1) + ",");
         Lines->Add(HTML_Ponto(lat2, lon2));
         Lines->Add("]");
         //define polyline
         linha_name.sprintf("linha%d%d", nr+1, nl+1);
         Lines->Add("var " + linha_name + " = new google.maps.Polyline({");
         Lines->Add("   path: " + pontos_name + ",");
         Lines->Add("   strokeColor: \"#FF0000\",     strokeOpacity: 1.0,     strokeWeight: 2");
         Lines->Add("});");
         Lines->Add(linha_name + ".setMap(map);");
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormGoogle::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormGoogle::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form canto superior esquerdo
   Top  = 0;
   Left = 0;;
   }

//---------------------------------------------------------------------------
void __fastcall TFormGoogle::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormGoogle::HTML_Ponto(double lat, double lon)
   {
   //variáveis locais
   AnsiString txt;

   txt.sprintf("new google.maps.LatLng(%7.6f, %7.6f)", lat, lon);
   return(txt);
   }

//---------------------------------------------------------------------------
void __fastcall TFormGoogle::IniciaMapa(void)
	{
	//variáveis locais
	AnsiString  mapa_nome;
	//int         zoom;
	double      lat_med, lon_med;
	AREA_LL     area_ll;
	VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTArea      *area_utm  = redes->AreaUtm_cm();
	TZoomGoogle ZoomGoogle = TZoomGoogle();

	//converte coordenadas utm da área da Rede p/ latitude/longitude (coord.utm estão em cm)
//  scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100., area_ll.lat1, area_ll.lon1, scg->TipoCoord);
//  scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100., area_ll.lat2, area_ll.lon2, scg->TipoCoord);
    scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100., area_ll.lat1, area_ll.lon1);
    scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100., area_ll.lat2, area_ll.lon2);
	//calcula ponto médio da área
	lat_med = (area_ll.lat1 + area_ll.lat2) / 2;
	lon_med = (area_ll.lon1 + area_ll.lon2) / 2;
	//determina zoom do Google
	zoom = ZoomGoogle.MelhorValor(z0, area_ll, WebBrowser->Height, WebBrowser->Width);
	//carrega a área do mapa selecionado
	switch (map_type)
		{
		case mapaSATELLITE: mapa_nome = "SATELLITE"; break;
		case mapaHYBRID:    mapa_nome = "HYBRID";    break;
		case mapaTERRAIN:   mapa_nome = "TERRAIN";   break;
		case mapaROADMAP:
		default:            mapa_nome = "ROADMAP";   break;
		}
	LoadMapa(mapa_nome, lat_med, lon_med, zoom, true);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormGoogle::LoadMapa(AnsiString map_type, double lat, double lon, int zoom, bool draw_rede)
   {
   //variáveis locais
   bool       sucesso = true;
   AnsiString var_latlng;
   AnsiString option_mapTypeId, option_zoom;
   VTPath     *path    = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_html = path->DirTmp() + "\\google.html";

   //define configuração de acordo com os parâmetros
   var_latlng.sprintf("var latlng = new google.maps.LatLng(%16.15f, %16.15f);", lat, lon);
   option_zoom.sprintf("zoom: %d,", zoom);
   option_mapTypeId = "mapTypeId: google.maps.MapTypeId." + map_type;
   //reinicia StringList
   Lines->Clear();

   Lines->Add( "<html>"                                                                                         );
   Lines->Add( "   <head>"                                                                                      );
   Lines->Add( "      <meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\" />"               );
   Lines->Add( "      <style type=\"text/css\">"                                                                );
   Lines->Add( "         html { height: 100% }"                                                                 );
   Lines->Add( "         body { height: 100%; margin: 0; padding: 0}"                                           );
   Lines->Add( "         #map_canvas { height: 100% }"                                                          );
   Lines->Add( "      </style>"                                                                                 );
   Lines->Add( "      <title>Sinap - Teste Google Maps JavaScript API</title>"                                  );
   Lines->Add( "      <script type=\"text/javascript\""                                                         );
//   Lines->Add( "         src=\"http://maps.googleapis.com/maps/api/js?sensor=true\">"                           );
   Lines->Add( "         src=\"http://maps.googleapis.com/maps/api/js?key=AIzaSyAgrnjPh61UtCLv4CJp04Yh1K1ZNEApRnQ&sensor=true&callback=initialize\">");
   Lines->Add( "      </script>"                                                                                );
   Lines->Add( "      <script type=\"text/javascript\">"                                                        );
	Lines->Add( "         function initialize() {"                                                               );
// Lines->Add( "            var latlng = new google.maps.LatLng(-34,151);"                                      );
   Lines->Add(              var_latlng                                                                          );
   Lines->Add( "            var myOptions = {"                                                                  );
// Lines->Add( "               zoom: 13,"                                                                       );
   Lines->Add(                 option_zoom                                                                      );
   Lines->Add( "               center: latlng,"                                                                 );
   Lines->Add( "               disableDefaultUI: true,"                                                         );
// Lines->Add( "               mapTypeId: google.maps.MapTypeId.ROADMAP"                                        );
   Lines->Add(                 option_mapTypeId                                                                 );
   Lines->Add( "            };"                                                                                 );
   Lines->Add( "            var map = new google.maps.Map(document.getElementById(\"map_canvas\"), myOptions);" );
   //verifica se deve desenhar rede
   if (draw_rede) DesenhaRede();
   //
   Lines->Add( "         }"                                                                                     );
   Lines->Add( "      </script>"                                                                                );
   Lines->Add( "   </head>"                                                                                     );
   Lines->Add( "   <body onload=\"initialize()\">"                                                              );
   Lines->Add( "      <div id=\"map_canvas\" style=\"width:100%; height:100%; scroll: no\"></div>"              );
   Lines->Add( "   </body>"                                                                                     );
   Lines->Add( "</html>"                                                                                        );
   //salva StringList no arquivo
   Lines->SaveToFile(arq_html);
   try{//executa o JavaScript
      WebBrowser->Navigate(arq_html.c_str());
     }catch(Exception &e)
         {
       sucesso = false;
         }
   //limpa StringList
   Lines->Clear();
   return(sucesso);
   }
//---------------------------------------------------------------------------
void __fastcall TFormGoogle::TViewDblClick(TObject *Sender)
   {
   //variáveis locais
   AnsiString    class_name, arq_kml;
   VTNode       *pNode;
   VTRede       *rede;
   VTExportaKml *ExportaKml;
   VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTBarra		 *barra;
	//VTRedes		 *redes;
	TList *lisBARRA;
	int x, y;
   
	//verificações antes de executar ExportaKML

   //verifica se existe uma Rede associada ao TreeNode
   if ((TView->Selected == NULL)||(TView->Selected->Data == NULL))
      {
      Aviso("Selecione uma rede");
      return;
      }
   //verifica se objeto associado ao TreeNode é uma Rede
	class_name = ((TObject*)(TView->Selected->Data))->ClassName();
   if (class_name.AnsiCompareIC("TRede") == 0)
		{//determina Rede associada ao TreeNode
		rede = (VTRede*)(TView->Selected->Data);
		lisBARRA = rede->LisBarra();
		for (int n = 0; n < lisBARRA->Count; n++)
			{
			//ver se as redes possuem coordenadas geográficas
			barra = (VTBarra*)lisBARRA->Items[n];
			if (! barra->CoordenadasUtm_cm(x,y))
				{
				Aviso("A rede carregada não possui coordenadas geográficas cadastradas.");
				return;
				}
			}

		//cria objeto ExportaKml p/ gerar o arquivo KML
      arq_kml    = path->DirExporta() + "\\" + rede->Codigo + ".kml";
      ExportaKml = NewObjExportaKml(apl);
      ExportaKml->Executa(rede, arq_kml);
      delete ExportaKml;
      //mensagem
      Aviso("O arquivo " + AnsiQuotedStr(arq_kml, '\'') + " foi criado corretamente");
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormGoogle::TViewInicia(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia TView
   TView->Items->Clear();
   //inicia Arvore e o TreeView
   arvore->Inicia(redes);
   arvore->TreeViewInicia(TView);
   //expande o TView
   TView->FullExpand();
   }

//---------------------------------------------------------------------------
void __fastcall TFormGoogle::maiszoomClick(TObject *Sender)
	{
	zoom = zoom+2;
	AtualizaMapa();
	}

//---------------------------------------------------------------------------
void __fastcall TFormGoogle::menoszoomClick(TObject *Sender)
	{
	zoom = zoom-2;
	AtualizaMapa();
	}

//---------------------------------------------------------------------------
void __fastcall TFormGoogle::WebBrowserDocumentComplete(TObject *ASender, const IDispatch *pDisp,
			 const OleVariant &URL)
	{
	//
	}

//---------------------------------------------------------------------------
//eof

