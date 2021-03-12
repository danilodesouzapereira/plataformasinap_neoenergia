// --------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include <Graphics.hpp>
#include <ActiveX.hpp>
#include "TCartografia.h"
#include "TFormMapa.h"
#include "VTScg.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTArea.h"
#include "..\Grafico\VTQuad.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Diretorio\VTDir.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Grafico.h"
#include "..\..\DLL_Inc\Rede.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe
// ---------------------------------------------------------------------------

VTCartografia* __fastcall NewObjCartografia(VTApl *apl, TWinControl *parent,
    TAction *ActionMapaDisp)
{
    return (new TCartografia(apl, parent, ActionMapaDisp));
}

// ---------------------------------------------------------------------------
__fastcall TCartografia::TCartografia(VTApl *apl, TWinControl *parent, TAction *ActionMapaDisp)
{
    // variáveis locais
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTLicenca  *licenca  = licencas->ExisteLicenca();

    // salva ponteiro p/ objeto
    this->apl = apl;

    // cria objetos
    goog.area_utm   = DLL_NewObjArea();
    goog.quad_pixel = NewObjQuad();
    FormMapa        = new TFormMapa(NULL, apl, parent, ActionMapaDisp);
    scg             = NewObjScg(apl, licenca->Empresa, licenca->EmpresaStr);
    // inicia dados
    PD.map_type     = mapaROADMAP; // mapaNORMAL;
    z0              = 0.7111111111;
    // define dimensão de FormMapa c/ o mesmo valor 1700x1100 definido p/ a página Web (default)
    FormMapa->ClientWidth  = 1742.0;
    FormMapa->ClientHeight =  865.0;
    // inicia objeto FormMapa
    FormMapa->Start();
}

// ---------------------------------------------------------------------------
__fastcall TCartografia::~TCartografia(void)
{
    // destrói FormMapa
    if (FormMapa)
    {
        FormMapa->Stop();
        FormMapa->Close();
        delete FormMapa;
        FormMapa = NULL;
    }
    // destrói objetos
    if (scg)
    {
        delete scg;
        scg      = NULL;
    }
    if (goog.area_utm)
    {
        delete goog.area_utm;
        goog.area_utm = NULL;
    }
    if (goog.quad_pixel)
    {
        delete goog.quad_pixel;
        goog.quad_pixel = NULL;
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TCartografia::CarregaArea_utm(int map_type, VTArea *area_utm, VTQuad *quad_pixel,
    bool &disponivel, int tipo_coord)
{
    // variáveis locais
    AnsiString mapa_nome;
    double lat_ref, lon_ref;

    // indica que a área não está disponível
    disponivel = false;
    // verifica se área solicitada já está carregada
    if ((map_type == PD.map_type) && goog.area_utm->Igual(area_utm) && goog.quad_pixel->Igual
        (quad_pixel))
    {
        disponivel = true;
        return (true);
    }

    // salva o tipo de mapa
    PD.map_type = map_type;
    // salva áreas recebidas
    goog.area_utm = DLL_NewObjArea();
    goog.area_utm->CopiaAtributosDe(*area_utm);
    goog.quad_pixel = DLL_NewObjQuad();
    goog.quad_pixel->CopiaAtributosDe(*quad_pixel);
    // converte coordenadas utm p/ latitude/longitude (coord.utm estão em cm)
    scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100.,
                             area_ll.lat1,        area_ll.lon1);
    scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100.,
                             area_ll.lat2,        area_ll.lon2);
    // determina centro da área do mapa, em latitude longitude
    lon_ref = (area_ll.lon1 + area_ll.lon2) / 2;
    lat_ref = (area_ll.lat1 + area_ll.lat2) / 2;
    // carrega mapa
    switch (PD.map_type)
    {
    case mapaSATELLITE:
        mapa_nome = "SATELLITE";
        break;
    case mapaHYBRID:
        mapa_nome = "HYBRID";
        break;
    case mapaTERRAIN:
        mapa_nome = "TERRAIN";
        break;
    case mapaCOPEL:
    case mapaROADMAP:
    default:
        mapa_nome = "ROADMAP";
        break;
    }
    // licenca copel
    if (PD.map_type == mapaCOPEL)
    {
//        if (!FormMapa->LoadMapaCopel(area_ll.lat1, area_ll.lon1, area_ll.lat2, area_ll.lon2))
//            return (false);
        if (!FormMapa->LoadMapaCopelUTM(area_utm->x1 / 100., area_utm->y1 / 100., area_utm->x2 / 100., area_utm->y2 / 100.))
            return (false);
        area_ll.lon_med = lon_ref;
        area_ll.lat_med = lat_ref;
        disponivel = true;
    }
    else
    { // outra empresa

//jss20191014
        double h = 0.0;
        double lat_wgs = lat_ref;
        double lon_wgs = lon_ref;

        // Troca datum para Google
        if (scg->ProjCS.datum != datumWGS84)
            scg->DatumConvert(lat_ref, lon_ref, 0.0, lat_wgs, lon_wgs, h);

//jss20191014
//      apesar de não documentado o Google aceita zoom fracionário
//      if (!FormMapa->LoadMapa(mapa_nome, lat_ref, lon_ref, int(goog.zoom)))
        if (!FormMapa->LoadMapa(mapa_nome, lat_wgs, lon_wgs, goog.zoom))
            return (false);
        // salva área do mapa mostrada em FormMapa (em latitude/longitude)
        area_ll.lon_med = lon_ref;
        area_ll.lat_med = lat_ref;
/*
        area_ll.lon1 = lon_ref - ((FormMapa->ClientWidth / 2) / (pow(2.0, goog.zoom) * z0));
        area_ll.lon2 = lon_ref + ((FormMapa->ClientWidth / 2) / (pow(2.0, goog.zoom) * z0));
        area_ll.lat1 = lat_ref - ((FormMapa->ClientHeight / 2) / (pow(2.0, goog.zoom) * z0));
        area_ll.lat2 = lat_ref + ((FormMapa->ClientHeight / 2) / (pow(2.0, goog.zoom) * z0));
*/
        disponivel = true;
    }
    return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TCartografia::CopiaBitmap(VTArea *area_utm, TCanvas *canvas,
                                          int x1, int y1, int x2, int y2,
                                          double fat_escala, int tipo_coord)
{
    // variáveis locais
    int dx, dy, xa, ya, xb, yb, width, height;
    double lat1, lat2, lon1, lon2;
    double lat_med, lon_med;
    TRect destRect;
    TRect sourceRect;
    IViewObject *viewObject;
    Graphics::TBitmap *graph_bmp;

    // verifica se é o mapa da Copel
    if (PD.map_type == mapaCOPEL)
    {
        return CopiaBitmapCopel(area_utm, canvas, x1, y1, x2, y2, fat_escala, tipo_coord);
    }

    // converte coordenadas utm p/ latitude/longitude (coord.utm estão em cm)
//  scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100., lat1, lon1, tipo_coord);
//  scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100., lat2, lon2, tipo_coord);
    scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100., lat1, lon1);
    scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100., lat2, lon2);
    // determina coordenada central da área a ser copiada
    lat_med = (lat1 + lat2) / 2;
    lon_med = (lon1 + lon2) / 2;
    // determina distância entre centro da área solicitada e centro da área carregada
    dx = fabs(area_ll.lon_med - lon_med) * (Power(2.0, goog.zoom) * z0);
    dy = fabs(area_ll.lat_med - lat_med) * (Power(2.0, goog.zoom) * z0);
    width  = (x2 - x1 + 1);   // +1 para arredondar a divisão por 2 mais adiante
    height = (y2 - y1 + 1);   // +1 para arredondar a divisão por 2 mais adiante
    // converte dimensões em função da escala na tela Sinap e da escala na tela do site
    fat_escala = PD.escala / fat_escala;
    width  *= fat_escala;
    height *= fat_escala;
    // define coordenadas da área do mapa a ser copiada
    xa = (FormMapa->WebBrowser->Width  / 2) + dx - (width  / 2);
    ya = (FormMapa->WebBrowser->Height / 2) + dy - (height / 2);
    xb = xa + width;
    yb = ya + height;
    try
    { // obtém objeto IViewObject do WebBrowser
        FormMapa->WebBrowser->Document->QueryInterface(IID_IViewObject, (LPVOID*)&viewObject);
        if (viewObject)
        { // cria objeto Bitmap c/ a dimensão do WebBrowser
            graph_bmp = new Graphics::TBitmap();
            graph_bmp->Width = FormMapa->WebBrowser->Width;
            graph_bmp->Height = FormMapa->WebBrowser->Height;
            // define área a ser copiada
            sourceRect = Rect(0, 0, graph_bmp->Width, graph_bmp->Height);
            // copia área do mapa no objeto Bitmap
            viewObject->Draw(DVASPECT_CONTENT, // dwDrawAspect
                1, // lindex
                NULL, // pvAspect
                NULL, // ptd
                NULL, // hdcTargetDev
                graph_bmp->Canvas->Handle, // hdcDraw
                (LPRECTL)&sourceRect, // lprcBounds
                NULL, // lprcWBounds
                NULL, // pfnContinue
                0); // dwContinue
            // define retângulo da área a ser copiada
            destRect = Rect(x1, y1, x2, y2);
            sourceRect = Rect(xa, ya, xb, yb);
            // copia Bitmap p/ o Canvas destino
            canvas->CopyRect(destRect, graph_bmp->Canvas, sourceRect);
            // destrói IViewObject
            viewObject->Release();
            // destrói Bitmap
            delete graph_bmp;
        }
    }
    catch (Exception &e)
    {
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
bool __fastcall TCartografia::CopiaBitmapCopel(VTArea *area_utm, TCanvas *canvas, int x1, int y1,
    int x2, int y2, double fat_escala, int tipo_coord)
{
    // variáveis locais
    int xa, ya, xb, yb;
    TRect destRect;
    TRect sourceRect;
    IViewObject *viewObject;
    Graphics::TBitmap *graph_bmp;
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
    int d = 16;

    //define coordenadas da imagem a ser copiada
    xa = 0;
    ya = 0;
    xb = FormMapa->WebBrowser->Width;
    yb = FormMapa->WebBrowser->Height;

    //aumenta o formMapa para tirar as barras de rolagem
//    Redimensiona(xb+20, yb+20);
//    Application->ProcessMessages();

    try
    { // obtém objeto IViewObject do WebBrowser
        FormMapa->WebBrowser->Document->QueryInterface(IID_IViewObject, (LPVOID*)&viewObject);
        if (viewObject)
        { // cria objeto Bitmap c/ a dimensão do WebBrowser
            graph_bmp = new Graphics::TBitmap();
            graph_bmp->Width = xb;//FormMapa->WebBrowser->Width;
            graph_bmp->Height = yb;//FormMapa->WebBrowser->Height;
            // define área a ser copiada
            sourceRect = Rect(0, 0, graph_bmp->Width, graph_bmp->Height);
            // copia área do mapa no objeto Bitmap
            viewObject->Draw(DVASPECT_CONTENT, // dwDrawAspect
                1, // lindex
                NULL, // pvAspect
                NULL, // ptd
                NULL, // hdcTargetDev
                graph_bmp->Canvas->Handle, // hdcDraw
                (LPRECTL)&sourceRect, // lprcBounds
                NULL, // lprcWBounds
                NULL, // pfnContinue
                0); // dwContinue
            // define retângulo da área a ser copiada
            destRect = Rect(x1+d, y1+d, x2-d, y2-d);
            sourceRect = Rect(xa+d, ya+d, xb-d, yb-d);
            // copia Bitmap p/ o Canvas destino
            canvas->CopyRect(destRect, graph_bmp->Canvas, sourceRect);
            // destrói IViewObject
            viewObject->Release();
            // destrói Bitmap
            delete graph_bmp;
        }
    }
    catch (Exception &e)
    {
        //reduz o form para tamanho original
//        Redimensiona(xb, yb);
        if(graph_bmp)
            delete graph_bmp;
        return (false);
    }
    //reduz o form para tamanho original
//    Redimensiona(xb, yb);

    return (true);
}
/*

// ---------------------------------------------------------------------------
bool __fastcall TCartografia::CopiaBitmapCopel(VTArea *area_utm, TCanvas *canvas, int x1, int y1,
    int x2, int y2, double fat_escala, int tipo_coord)
{
    // variáveis locais
    int dx, dy, xa, ya, xb, yb, width, height;
    double lat1, lat2, lon1, lon2;
    double lat_med, lon_med;
    TRect destRect;
    TRect sourceRect;
    IViewObject *viewObject;
    Graphics::TBitmap *graph_bmp;
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
    AnsiString arq_bmp = path->DirTmp() + "\\mapa.bmp";

    //verifica se o arquivo existe
    if(! FileExists(arq_bmp)) return false;
    // converte coordenadas utm p/ latitude/longitude (coord.utm estão em cm)
    scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100., lat1, lon1, tipo_coord);
    scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100., lat2, lon2, tipo_coord);
    // determina coordenada central da área a ser copiada
    lat_med = (lat1 + lat2) / 2;
    lon_med = (lon1 + lon2) / 2;
    // determina distância entre centro da área solicitada e centro da área carregada
    dx = fabs(area_ll.lon_med - lon_med) * (Power(2.0, goog.zoom) * z0);
    dy = fabs(area_ll.lat_med - lat_med) * (Power(2.0, goog.zoom) * z0);
    width = (x2 - x1 + 1);
    height = (y2 - y1 + 1);
    // converte dimensões em função da escala na tela Sinap e da escala na tela do site
    fat_escala = PD.escala / fat_escala;
    width *= fat_escala;
    height *= fat_escala;
    // define coordenadas da área do mapa a ser copiada
    xa = (FormMapa->WebBrowser->Width / 2) + dx - (width / 2);
    ya = (FormMapa->WebBrowser->Height / 2) + dy - (height / 2);
    xb = xa + width;
    yb = ya + height;
    try
    { // cria objeto Bitmap c/ a dimensão do WebBrowser
        graph_bmp = new Graphics::TBitmap();
        graph_bmp->Width = FormMapa->WebBrowser->Width;
        graph_bmp->Height = FormMapa->WebBrowser->Height;
        // copia área do mapa no objeto Bitmap a partir do arquivo
        graph_bmp->LoadFromFile(arq_bmp);
        // define área a ser copiada
        sourceRect = Rect(0, 0, graph_bmp->Width, graph_bmp->Height);
        // define retângulo da área a ser copiada
        destRect = Rect(x1, y1, x2, y2);
        sourceRect = Rect(xa, ya, xb, yb);
        // copia Bitmap p/ o Canvas destino
        canvas->CopyRect(destRect, graph_bmp->Canvas, sourceRect);
        // destrói Bitmap
        delete graph_bmp;
    }
    catch (Exception &e)
    {
        if(graph_bmp)
            delete graph_bmp;
        return (false);
    }
    return (true);
}

*/
/*
 //---------------------------------------------------------------------------
 bool __fastcall TCartografia::DefineEscala(VTArea *area_utm, VTQuad *quad_pixel)
 {
 //variáveis locais
 double       x1, y1, x2, y2;
 double       zoom_w, zoom_h, fatx, faty;
 //wchar_t*     wscript;
 AnsiString   UTMZone1, UTMZone2;

 // salva dimensões para a imagem de saida (usadas por ZoomGoogleHeight() e ZoomGoogleWidth())
 dim.saida.width  = quad_pixel->Width();
 dim.saida.height = quad_pixel->Height();
 //converte coord UTM recebidas para Lat/Lon (ATENÇÃO: coord.utm estão em cm)
 scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100., area_ll.lat1, area_ll.lon1);
 scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100., area_ll.lat2, area_ll.lon2);
 //determina melhor Zoom do Google, considerando área da rede e área da tela
 zoom_h = ZoomGoogleHeight();
 zoom_w = ZoomGoogleWidth();
 if      ((zoom_h < 0)&&(zoom_w < 0)) return(false);
 else if ((zoom_h > 0)&&(zoom_w < 0)) goog.zoom = zoom_h;
 else if ((zoom_h < 0)&&(zoom_w > 0)) goog.zoom = zoom_w;
 else if ((zoom_h > 0)&&(zoom_w > 0)) goog.zoom = Min(zoom_h, zoom_w);
 //recalcula área da rede, em latitude/longitude, considerando Zoom selecionado
 area_ll.lon2 = area_ll.lon1 + ( dim.saida.width  / (pow(2.0, goog.zoom) * z0) );
 area_ll.lat2 = area_ll.lat1 + ( dim.saida.height / (pow(2.0, goog.zoom) * z0) );
 //recalcula área da rede em utm e guarda zonas (espaço a mais proposital)
 scg->LatLon_to_NorthEast(area_ll.lat1, area_ll.lon1, y1, x1);
 UTMZone1 = scg->ZonaUTM + " ";
 scg->LatLon_to_NorthEast(area_ll.lat2, area_ll.lon2, y2, x2);
 UTMZone2 = scg->ZonaUTM + " ";
 //verifica se as coordenadas da área estão em zonas diferentes  //DVK 2017.02.01
 if(UTMZone1.AnsiCompareIC(UTMZone2) != 0)
 {
 scg->ConverteUTMmesmaZona(x1, x2);
 }
 //calcula fator de escala (area_utm/area_pixel)
 //teste
 //fatx = dim.google.width  / fabs(x2 - x1);
 //faty = dim.google.height / fabs(y2 - y1);
 fatx = dim.saida.width  / fabs(x2 - x1);
 faty = dim.saida.height / fabs(y2 - y1);
 //salva valor de escala (em pixel/cm)
 //PD.escala = Min(fatx, faty) / 100.;
 PD.escala = fatx / 100.;
 return(true);
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TCartografia::DefineEscala(VTArea *area_utm, VTQuad *quad_pixel)
{ // DVK 2017.06.01
    // variáveis locais
//  double x1, y1, x2, y2;
//  double delta_x, delta_y, lat1, lon1, lat2, lon2;
//  double zoom_w, zoom_h, fatx, faty, fat;
//  wchar_t*     wscript;
//  AnsiString UTMZone1, UTMZone2;

    // salva dimensões para a imagem de saida (usadas por ZoomGoogleHeight() e ZoomGoogleWidth())
    dim.saida.width  = quad_pixel->Width();
    dim.saida.height = quad_pixel->Height();

    // converte coord UTM recebidas para Lat/Lon (ATENÇÃO: coord.utm estão em cm)
    scg->NorthEast_to_LatLon(area_utm->y1 / 100., area_utm->x1 / 100., area_ll.lat1, area_ll.lon1);
    scg->NorthEast_to_LatLon(area_utm->y2 / 100., area_utm->x2 / 100., area_ll.lat2, area_ll.lon2);

//jss20191014
//  anteriormente o zoom do Google (goog.zoom) e a escala da rede (PD.escala)
//  eram calculados separada e independentemente;
//  além disso, o zoom do Google era um valor inteiro calculado iterativamente
//  para ser o mais próximo/ adequado à escala da rede
//  isso leva a uma imprecisão entre ambos os valores (escala e zoom)
//  isso tudo foi modificado da seguinte maneira:
//  - a escala da rede (PD.escala) já vem definida pela apresentação gráfica
//    da rede (sem qualquer mapa de fundo)
//  - o zoom do Google agora é calculado como um número fracionário (não mais
//    um valor inteiro), a partir da escala (PD.escala), eliminando o loop de
//    (sem iterações)
//  - assim, os métodos ZoomGoogleHeight e ZoomGoogleWidth, que já tinham sido
//    transformados para valores fracionários, já não são mais usados
//  - finalmente, a única variável empírica mantida até agora é o valor z0
//    (proporção de pixels/ coordenadas)
/*
    double h, lat1wgs, lat2wgs, lon1wgs, lon2wgs;
    DatumConvert(area_ll.lat1, area_ll.lon1, 0.0, lat1wgs, lon1wgs, h);
    DatumConvert(area_ll.lat2, area_ll.lon2, 0.0, lat2wgs, lon2wgs, h);
    // determina melhor Zoom do Google, considerando área da rede e área da tela
    zoom_h = ZoomGoogleHeight(area_ll.lat1, area_ll.lat2);
    zoom_w = ZoomGoogleWidth (area_ll.lon1, area_ll.lon2);
    if ((zoom_h < 0) && (zoom_w < 0))
        return (false);
    else if ((zoom_h > 0) && (zoom_w < 0))
        goog.zoom = zoom_h;
    else if ((zoom_h < 0) && (zoom_w > 0))
        goog.zoom = zoom_w;
    else if ((zoom_h > 0) && (zoom_w > 0))
        goog.zoom = Min(zoom_h, zoom_w);
    // recalcula área da rede, em latitude/longitude, considerando Zoom selecionado
//  area_ll.lat2 = area_ll.lat1 + (dim.saida.height / (pow(2.0, goog.zoom) * z0));
//  area_ll.lon2 = area_ll.lon1 + (dim.saida.width  / (pow(2.0, goog.zoom) * z0));
    // calcula fator de escala (area_ll/area_pixel)

    lat1 = DegToRad(area_ll.lat1);
    lat2 = DegToRad(area_ll.lat2);
    lon1 = DegToRad(area_ll.lon1);
    lon2 = DegToRad(area_ll.lon2);

    lat1 = area_ll.lat1 * ggprojcs->deg2rad;
    lat2 = area_ll.lat2 * ggprojcs->deg2rad;
    lon1 = area_ll.lon1 * ggprojcs->deg2rad;
    lon2 = area_ll.lon2 * ggprojcs->deg2rad;

    // o cálculo do delta x e y é uma aproximação equi-retangular:
    // http://www.movable-type.co.uk/scripts/latlong.html
    // 6371e3 = raio médio da terra em m - mean radius
    // 6378.137e3 = raio médio da terra   em m - equatorial radius
    delta_x = (lon2 - lon1) * ggprojcs->semi_eixo * cos((lat1 + lat2) / 2.0);
    delta_y = (lat2 - lat1) * ggprojcs->semi_eixo;
*/
    double delta_x, delta_y, fatx, faty;

    delta_x = fabs(area_utm->x2 - area_utm->x1)/ 100.0;
    delta_y = fabs(area_utm->y2 - area_utm->y1)/ 100.0;
    if (IsDoubleZero(delta_x))
        delta_x = 1.;
    if (IsDoubleZero(delta_y))
        delta_y = 1.;

    // Mudança de ajuste
    fatx = dim.saida.width  / delta_x;
    faty = dim.saida.height / delta_y;

    // salva valor de escala (em pixel/cm)
    PD.escala = Min(fatx, faty) / 100.;
    // teste: 95%
    // PD.escala *= 0.95;

//  double zoomx, zoomy, zoomxy, escalax, escalay, escalaxy;
    double zoomx, zoomy;

    zoomx     = PD.escala / z0 *
                fabs(area_utm->x2 - area_utm->x1) /
                fabs(area_ll.lon2 - area_ll.lon1);
    zoomx     = Log2(zoomx);
    zoomy     = PD.escala / z0 *
                fabs(area_utm->y2 - area_utm->y1) /
                fabs(area_ll.lat2 - area_ll.lat1);
    zoomy     = Log2(zoomy);
    goog.zoom = Min(zoomx, zoomy);
/*
    escalax   = pow(2.0, goog.zoom) * z0 *
                fabs(area_ll.lon2 - area_ll.lon1) /
                fabs(area_utm->x2 - area_utm->x1);
    escalay   = pow(2.0, goog.zoom) * z0 *
                fabs(area_ll.lat2 - area_ll.lat1) /
                fabs(area_utm->y2 - area_utm->y1);

    escalaxy  = Min(escalax, escalay);
*/
    return (true);
}

// ---------------------------------------------------------------------------
double __fastcall TCartografia::PM_GetEscala_pixel_cm(void)
{
    return (PD.escala);
}

// ---------------------------------------------------------------------------
int __fastcall TCartografia::PM_GetTipoMapa(void)
{
    return (PD.map_type);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TCartografia::PM_GetZonaUtm(void)
{
    // proteção
    if (scg == NULL)
        return ("");
    else
        return (scg->ZonaUTM);
}

// ---------------------------------------------------------------------------
void __fastcall TCartografia::PM_SetTipoMapa(int map_type)
{
    PD.map_type = map_type;
}

// ---------------------------------------------------------------------------
void __fastcall TCartografia::PM_SetZonaUtm(AnsiString zona)
{
    // proteção
    if (scg != NULL)
    { // redefine Zona
        scg->ZonaUTM = zona;
        // cancela tipo de mapa selecionado
        PD.map_type = mapaNENHUM;
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
void __fastcall TCartografia::Redimensiona(int width, int height)
{
    // redefine dimensão do form mapa
    FormMapa->ClientWidth = width;// - 1; // + 100;
    FormMapa->ClientHeight = height;// - 1; // + 100;
    FormMapa->WebBrowser->Width = width;// - 1; // + 100;
    FormMapa->WebBrowser->Height = height;// - 1; // + 100;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
double __fastcall TCartografia::ZoomGoogleHeight(double lat1, double lat2)
{
/*
    // variáveis locais
#define MAX_ZOOM  24
    bool zoom[MAX_ZOOM];
    int melhor_zoom = -1;
    double height;
    double distancia, distancia_min = 1;
    double lat1 = area_ll.lat1;
    double lat2 = area_ll.lat2;
*/
//jss20191014
    double dzoom;
    dzoom = Log2(dim.saida.height / z0 / fabs(lat2 - lat1));
    return dzoom;
/*
    // inicia vetor
    for (int n = 0; n < MAX_ZOOM; n++)
    {
        zoom[n] = false;
    }
    // 1 - tamanho da figura em pixels deve ser MENOR que o tamanho do monitor (formWeb)
    for (int n = 0; n < MAX_ZOOM; n++)
    {
        zoom[n] = true;
        height = fabs(lat2 - lat1) * pow(2.0, double(n)) * z0;
        if (height > dim.saida.height)
            break;
    }

     //2 - relacao entre figura e imagem de saida deve estar entre 1,8 e 0,9 (arbitrarios)
     for (int n = 0 ; n < MAX_ZOOM ; n++)
     {
     if (zoom[n])
     {
     width = fabs(lon2-lon1) * pow(2.0, double(n)) * z0;
     if (((dim.saida.width / width) >= 1.8) ||
     ((dim.saida.width / width) <  0.9))
     zoom[n] = false;
     }
     }

    // 3 - determina o zoom com relação mais próxima de 1
    for (int n = 0; n < MAX_ZOOM; n++)
    {
        if (zoom[n])
        {
            height = fabs(lat2 - lat1) * pow(2.0, double(n)) * z0;
            distancia = fabs((height / dim.saida.height) - 1.0);
            if (distancia < distancia_min)
            {
                distancia_min = distancia;
                melhor_zoom = n;
            }
        }
    }
    // proteção
    if (melhor_zoom > 19)
        melhor_zoom = -1;

    return (double(melhor_zoom));
*/
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
double __fastcall TCartografia::ZoomGoogleWidth(double lon1, double lon2)
{
/*
    // variáveis locais
#define MAX_ZOOM  24
    bool zoom[MAX_ZOOM];
    int melhor_zoom = -1;
    double width;
    double distancia, distancia_min = 1;
    double lon1 = area_ll.lon1;
    double lon2 = area_ll.lon2;
*/
//jss20191014
    double dzoom;
    dzoom = Log2(dim.saida.width / z0 / fabs(lon2 - lon1));
    return dzoom;
/*
    // inicia vetor
    for (int n = 0; n < MAX_ZOOM; n++)
    {
        zoom[n] = false;
    }
    // 1 - tamanho da figura em pixels deve ser MENOR que o tamanho do monitor (formWeb)
    for (int n = 0; n < MAX_ZOOM; n++)
    {
        zoom[n] = true;
        width = fabs(lon2 - lon1) * pow(2.0, double(n)) * z0;
        if (width > dim.saida.width)
            break;
    }

     //2 - relacao entre figura e imagem de saida deve estar entre 1,8 e 0,9 (arbitrarios)
     if (ajusta_tamanho)
     for (int n = 0 ; n < MAX_ZOOM ; n++)
     if (((dim.saida.width / (fabs(lon2-lon1) * pow(2.0, double(n)) * z0)) >= 1.8) ||
     ((dim.saida.width / (fabs(lon2-lon1) * pow(2.0, double(n)) * z0)) <  0.9) )
     zoom[n] = false;

    // 3 - determina o zoom com relação mais próxima de 1
    for (int n = 0; n < MAX_ZOOM; n++)
    {
        if (zoom[n])
        {
            width = fabs(lon2 - lon1) * pow(2.0, double(n)) * z0;
            distancia = fabs((width / dim.saida.width) - 1.0);
            if (distancia < distancia_min)
            {
                distancia_min = distancia;
                melhor_zoom = n;
            }
        }
    }
    // proteção
    if (melhor_zoom > 19)
        melhor_zoom = -1;

    return (double(melhor_zoom));
*/
}

// ---------------------------------------------------------------------------
// eof
