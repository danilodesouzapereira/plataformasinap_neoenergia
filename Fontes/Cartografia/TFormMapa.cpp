// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMapa.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormMapa::TFormMapa(TComponent* Owner, VTApl *apl, TWinControl *parent,
	TAction *ActionMapaDisp) : TForm(Owner)
{
	// NÃO insere este Form em seu Parent: inserindo o mapa não é carregado (por que será ?)
	// if (parent) Parent = parent;
	// salva objetos
	this->apl = apl;
	this->ActionMapaDisp = ActionMapaDisp;
	// desabilita timer
	Timer->Enabled = false;
	// WebBrowser->FullScreen = true;
	// cria objetos locais
	Lines = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TFormMapa::~TFormMapa(void)
{
	// destrói objetos
	if (Lines)
	{
		delete Lines;
		Lines = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMapa::WebBrowserDocumentComplete(TObject *ASender, const IDispatch *pDisp,
	OleVariant &URL)
{
	// atualiza WebBrowser
	// WebBrowser->Refresh();
	// permite tratamento de eventos do Windows
	// Application->ProcessMessages();
	// habilita Timer p/ executar ActionMapaDisp
	// Timer->Tag      = 1;
	Timer->Interval = 2000;
	Timer->Enabled = true;
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormMapa::LoadMapa(AnsiString map_type, double lat, double lon, int zoom)
 {
 //variáveis locais
 bool       sucesso = true;
 AnsiString var_latlng;
 AnsiString option_mapTypeId, option_zoom;
 VTPath     *path    = (VTPath*)apl->GetObject(__classid(VTPath));
 AnsiString arq_html = path->DirTmp() + "\\google.html";

 //define configuração de acordo com os parâmetros
 var_latlng.sprintf("var latlng = new google.maps.LatLng(%4.3f, %4.3f);", lat, lon);
 option_zoom.sprintf("zoom: %d,", zoom);
 option_mapTypeId = "mapTypeId: google.maps.MapTypeId." + map_type;
 //reinicia StringList
 Lines->Clear();
 Lines->Add("<html>"                                                                                                            );
 Lines->Add("<head>"                                                                                                            );
 Lines->Add("<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\" />"                                        );
 Lines->Add("<script type=\"text/javascript\" src=\"http://maps.google.com/maps/api/js?sensor=false\"></script>"                );
 Lines->Add("<script type=\"text/javascript\">"                                                                                 );
 Lines->Add("   function initialize()"                                                                                          );
 Lines->Add("      {"                                                                                                           );
 // Lines->Add("      var latlng = new google.maps.LatLng(-34.397, 150.644);"                                                      );
 Lines->Add(       var_latlng                                                                                                   );
 Lines->Add("      var myOptions ="                                                                                             );
 Lines->Add("         {"                                                                                                        );
 // Lines->Add("         zoom: 8,"                                                                                                 );
 Lines->Add(          option_zoom                                                                                               );
 Lines->Add("         center: latlng,"                                                                                          );
 // Lines->Add("         mapTypeId: google.maps.MapTypeId.ROADMAP"                                                                 );
 Lines->Add(          option_mapTypeId                                                                                          );
 Lines->Add("         };"                                                                                                       );
 Lines->Add("      var map = new google.maps.Map(document.getElementById(\"map_canvas\"), myOptions);"                          );
 Lines->Add("      }"                                                                                                           );
 Lines->Add("</script>"                                                                                                         );
 Lines->Add("</head>"                                                                                                           );
 Lines->Add("<body onload=\"initialize()\">"                                                                                    );
 Lines->Add("<div id=\"map_canvas\" style=\"width:100%; height:100%\"></div>"                                                   );
 Lines->Add("</body>"                                                                                                           );
 Lines->Add("</html>"                                                                                                           );
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
 */

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
//jss20191014
//  apesar de não documentado o Google aceita zoom fracionário
//bool __fastcall TFormMapa::LoadMapa(AnsiString map_type, double lat, double lon, int zoom)
bool __fastcall TFormMapa::LoadMapa(AnsiString map_type, double lat, double lon, double zoom)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString var_latlng;
	AnsiString option_mapTypeId, option_zoom;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq_html = path->DirTmp() + "\\google.html";

	// define configuração de acordo com os parâmetros
	var_latlng.sprintf("var latlng = new google.maps.LatLng(%16.15f, %16.15f);", lat, lon);
//jss20191014
//  apesar de não documentado o Google aceita zoom fracionário
//  option_zoom.sprintf("zoom: %d,", zoom);
    option_zoom.sprintf("zoom: %.13f,", zoom);
	option_mapTypeId = "mapTypeId: google.maps.MapTypeId." + map_type;
	// reinicia StringList
	Lines->Clear();

	Lines->Add("<!DOCTYPE html>");
	Lines->Add("<html>");
	Lines->Add("   <head>");
	Lines->Add("      <meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\" />");
	Lines->Add("      <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />");
	Lines->Add("      <style type=\"text/css\">");
	Lines->Add("         html { height: 100% }");
	Lines->Add("         body { height: 100%; margin: 0; padding: 0}");
	Lines->Add("         #map_canvas { height: 100% }");
	Lines->Add("      </style>");
	Lines->Add("      <title>Sinap - Teste Google Maps JavaScript API</title>");
	// Lines->Add( "      <script type=\"text/javascript\""                                                         );
	Lines->Add("      <script async defer type=\"text/javascript\"");
	// Lines->Add( "         src=\"http://maps.googleapis.com/maps/api/js?sensor=true\">"                           );
	// Lines->Add( "         src=\"http://maps.googleapis.com/maps/api/js?v=3.21?sensor=true&callback=initialize\">");
	Lines->Add
		("         src=\"http://maps.googleapis.com/maps/api/js?key=AIzaSyAgrnjPh61UtCLv4CJp04Yh1K1ZNEApRnQ&sensor=true&callback=initialize\">"
		);
	Lines->Add("      </script>");
	Lines->Add("      <script type=\"text/javascript\">");
	Lines->Add("         function initialize() {");
	// Lines->Add( "            var latlng = new google.maps.LatLng(-34,151);"                                      );
	Lines->Add("            " + var_latlng);
	Lines->Add("            var myOptions = {");
	// Lines->Add( "               zoom: 13,"                                                                       );
	Lines->Add("               " + option_zoom);
	Lines->Add("               center: latlng,");
	Lines->Add("               disableDefaultUI: true,");
	// Lines->Add( "               mapTypeId: google.maps.MapTypeId.ROADMAP"                                        );
	Lines->Add("               " + option_mapTypeId);
	Lines->Add("            };");
	Lines->Add
		("            var map = new google.maps.Map(document.getElementById(\"map_canvas\"), myOptions);"
		);
	Lines->Add("         }");
	Lines->Add("      </script>");
	Lines->Add("   </head>");
	Lines->Add("   <body onload=\"initialize()\">");
	Lines->Add("      <div id=\"map_canvas\" style=\"width:100%; height:100%; scroll: no\"></div>");
	Lines->Add("   </body>");
	Lines->Add("</html>");
	// salva StringList no arquivo
	Lines->SaveToFile(arq_html);
	try
	{ // executa o JavaScript
		WebBrowser->Navigate(arq_html.c_str());
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	// limpa StringList
	Lines->Clear();
	return (sucesso);
}
/*
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938     //WSG84
bool __fastcall TFormMapa::LoadMapaCopel(double lat1, double lon1, double lat2, double lon2)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString var_latlng;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq_html = path->DirTmp() + "\\google.html";

	// define configuração de acordo com os parametros
	var_latlng.sprintf("%7.6f,%7.6f,%7.6f,%7.6f", lon1, lat1, lon2, lat2);
	// reinicia StringList
	Lines->Clear();
	Lines->Add("<!DOCTYPE html>");
	Lines->Add("<html>");
	Lines->Add("<head></head>");
	Lines->Add("<style type=\"text/css\">");
	Lines->Add("   html { height: 100%; scroll: no }");
	Lines->Add("   body { height: 100%; margin: 0; padding: 0; scroll: no}");
	Lines->Add("   #map_canvas { height: 100%; scroll: no }");
	Lines->Add("</style>");
	Lines->Add("<title>Sinap - WMS Copel</title>");
	Lines->Add("<body>");
	Lines->Add("<img class=\"\" src=\"");
	Lines->Add
		("https://www.copel.com/arcgis/services/WMS_CARTO_WGS84/MapServer/"
		"WMSServer?SERVICE=WMS&LAYERS=Municipios,Quadra,LOGRADOURO&FORMAT=image/bmp&REQUEST=GetMap&STYLES"
		"=&CRS=CRS%3A84&BBOX=" +
		var_latlng + "&WIDTH=" + IntToStr(WebBrowser->Width) + "&HEIGHT=" +
		IntToStr(WebBrowser->Height) + "&version=1.3.0\">");
	Lines->Add("</img>");
	Lines->Add("</body>");
	Lines->Add("</html>");
	Lines->SaveToFile(arq_html);

	try
	{ // executa o JavaScript
		WebBrowser->Navigate(arq_html.c_str());
		// WebBrowser->Navigate("https://www.copel.com/arcgis/services/WMS_CARTO_WGS84/MapServer/"
		// "WMSServer?SERVICE=WMS&LAYERS=Quadra,LOGRADOURO&FORMAT=image/png&REQUEST=GetMap&STYLES"
		// "=&CRS=CRS%3A84&BBOX=-51.768238,-24.756725,-51.760395,-24.752604&WIDTH=1200&HEIGHT=600&version=1.3.0");
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	// limpa StringList
	Lines->Clear();
	return (sucesso);
}
*/

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938      //SAD69
// https://sinapsisenergia.teamwork.com/#/tasks/16870413
bool __fastcall TFormMapa::LoadMapaCopelUTM(double x1, double y1, double x2, double y2)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString var_utm;
    AnsiString var_layers;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq_html = path->DirTmp() + "\\google.html";

	// define configuração de acordo com os parametros
	var_utm.sprintf("%7.2f,%7.2f,%7.2f,%7.2f", x1, y1, x2, y2);
    var_layers  = "Município,";
    var_layers += "Quadra,";
    var_layers += "LOGRADOURO,";
    var_layers += "CaminhosCarreadoresTrilhas,";
    var_layers += "Rodovias,";
    var_layers += "RiosPrincipais,";
    var_layers += "ÁreaUsoComunitário,";
    var_layers += "TerraIndígenaPonto,";
    var_layers += "TerraIndígenaÁrea,";
    var_layers += "OutrasUnidadesConservaçãoProtegidas,";
    var_layers += "UnidadeConservaçãoNãoSnuc,";
    var_layers += "UnidadeConservaçãoUsoSustentável,";
    var_layers += "UnidadeConservaçãoProtecaoIntegral";
	// reinicia StringList
	Lines->Clear();
	Lines->Add("<!DOCTYPE html>");
	Lines->Add("<html>");
	Lines->Add("<head></head>");
	Lines->Add("<style type=\"text/css\">");
	Lines->Add("   html { height: 100%; scroll: no }");
	Lines->Add("   body { height: 100%; margin: 0; padding: 0; scroll: no}");
	Lines->Add("   #map_canvas { height: 100%; scroll: no }");
	Lines->Add("</style>");
	Lines->Add("<title>Sinap - WMS Copel</title>");
	Lines->Add("<body>");
	Lines->Add("<img class=\"\" src=\"");
	Lines->Add
		("https://www.copel.com/arcgis/services/WMS_CARTO/MapServer/"
		"WMSServer?SERVICE=WMS&LAYERS=" + var_layers + "&FORMAT=image/bmp&REQUEST=GetMap&STYLES"
		"=&CRS=EPSG%3A29192&BBOX=" +
		var_utm + "&WIDTH=" + IntToStr(WebBrowser->Width) + "&HEIGHT=" +
		IntToStr(WebBrowser->Height) + "&version=1.3.0\">");
	Lines->Add("</img>");
	Lines->Add("</body>");
	Lines->Add("</html>");
	Lines->SaveToFile(arq_html);

	try
	{ // executa o JavaScript
		WebBrowser->Navigate(arq_html.c_str());
		// WebBrowser->Navigate("https://www.copel.com/arcgis/services/WMS_CARTO_WGS84/MapServer/"
		// "WMSServer?SERVICE=WMS&LAYERS=Quadra,LOGRADOURO&FORMAT=image/png&REQUEST=GetMap&STYLES"
		// "=&CRS=CRS%3A84&BBOX=-51.768238,-24.756725,-51.760395,-24.752604&WIDTH=1200&HEIGHT=600&version=1.3.0");
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	// limpa StringList
	Lines->Clear();
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMapa::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMapa::FormShow(TObject *Sender)
{
	// posiciona Form fora da área visível
	Left = Screen->Width + 100;
	Top = Screen->Height + 100;

	// TESTES
	// Left = Screen->Width  * 0.8;
	// Top  = Screen->Height * 0.8;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMapa::Start(void)
{
	// torna este Form visível
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMapa::Stop(void)
{
	// esconde este Form
	Hide();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMapa::TimerTimer(TObject *Sender)
{
	// desabilita o Timer
	Timer->Enabled = false;
	// executa Action externa
	ActionMapaDisp->Execute();
}

// ---------------------------------------------------------------------------
// eof
