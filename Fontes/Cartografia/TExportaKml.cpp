//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TExportaKml.h"
#include "VTScg.h"
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrafo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTExportaKml
//---------------------------------------------------------------------------
VTExportaKml* __fastcall NewObjExportaKml(VTApl *apl)
{
    return((VTExportaKml*)new TExportaKml(apl));
}

//---------------------------------------------------------------------------
__fastcall TExportaKml::TExportaKml(VTApl *apl)
{
    //variáveis locais
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTLicenca  *licenca  = licencas->ExisteLicenca();

    // salva ponteiro p/ objeto
    this->apl = apl;
    // cria objetos locais
    scg   = NewObjScg(apl, licenca->Empresa, licenca->EmpresaStr);
    Lines = new TStringList();
    // inicia dados p/ cálculo de coordenadas lat_lon
    z0 = 0.7111111111;
}

//---------------------------------------------------------------------------
__fastcall TExportaKml::~TExportaKml(void)
{
    //destrói objetos
    if (scg)   {delete scg;   scg   = NULL;}
    if (Lines) {delete Lines; Lines = NULL;}
}

//---------------------------------------------------------------------------
bool __fastcall TExportaKml::Executa(TList *lisREDE, AnsiString arq_kml)
{
    // variáveis locais
    VTRede *rede;

    // reinicia StringList
    Lines->Clear();
    // insere header
    Header();
    // abre "document"
    DocumentOpen("Rede");
    // ==================================
    // abre "folder" p/ desenhar Ligacoes
    FolderOpen("Caminhamento");
    // loop p/ todas redes
    for (int n = 0; n < lisREDE->Count; n++)
    {
        rede = (VTRede*)lisREDE->Items[n];
        if (!rede->Carregada)
            continue;
        // insere linhas p/ todas Ligacoes da Rede
        if (rede->Visible())
            InsereLinhas(rede->LisLigacao(), rede->Codigo);
    }
    // fecha "folder"
    FolderClose();
    // ==================================
    // abre "folder" p/ identificar chaves
    FolderOpen("Chaves");
    // loop p/ todas redes
    // AnsiString descricao;
    for (int n = 0; n < lisREDE->Count; n++)
    {
        rede = (VTRede*)lisREDE->Items[n];
        if (!rede->Carregada)
            continue;
        // insere marcadores p/ todas Ligacoes da Rede do tipo Chave
        if (rede->Visible())
            InsereMarcadorChave(rede->LisLigacao());
    }
    // fecha "folder"
    FolderClose();
    // ==================================
    // abre "folder" p/ identificar trafos
    FolderOpen("Transformadores");
    // loop p/ todas redes
    AnsiString descricao;
    for (int n = 0; n < lisREDE->Count; n++)
    {
        rede = (VTRede*)lisREDE->Items[n];
        if (!rede->Carregada)
            continue;
        // insere marcadores p/ todas Ligacoes da Rede do tipo Trafo
        if (rede->Visible())
            InsereMarcadorTrafo(rede->LisLigacao());
    }
    // fecha "folder"
    FolderClose();
    // ==================================
    // abre "folder" p/ identificar redes
    FolderOpen("Redes");
    // loop p/ todas redes
    for (int n = 0; n < lisREDE->Count; n++)
    {
        rede = (VTRede*)lisREDE->Items[n];
        if (!rede->Carregada)
            continue;
        if (rede->BarraInicial() != NULL)
        {
            VTBarra* pBarra;
            pBarra = (VTBarra*)rede->BarraInicial();
            int x, y;
            pBarra->CoordenadasUtm_cm(x, y);
            InserePonto(x, y, rede->Codigo, rede->Color, "pal4/icon28", " ");
        }
    }
    // fecha "folder"
    FolderClose();
    // ==================================
    // fecha "document"
    DocumentClose();
    // grava o arquivo
    Lines->SaveToFile(arq_kml);
    return (true);
}

//---------------------------------------------------------------------------
bool __fastcall TExportaKml::Executa(VTRede *rede, AnsiString arq_kml)
{
    // proteção
    if (!rede->Carregada)
        return (false);
    // reinicia StringList
    Lines->Clear();
    // insere header
    Header();
    // abre "document"
    DocumentOpen("Rede");
    // ==================================
    // abre "folder" p/ desenhar Ligacoes
    FolderOpen("Caminhamento");
    // insere linhas p/ todas Ligacoes da Rede
    InsereLinhas(rede->LisLigacao(), rede->Codigo);
    // fecha "folder"
    FolderClose();
    // ==================================
    // abre "folder" p/ identificar chaves
    FolderOpen("Chaves");
    // insere marcadores p/ todas Ligacoes da Rede do tipo Chave
    InsereMarcadorChave(rede->LisLigacao());
    // fecha "folder"
    FolderClose();
    // ==================================
    // abre "folder" p/ identificar trafos
    FolderOpen("Transformadores");
    // insere marcadores p/ todas Ligacoes da Rede do tipo Trafo
    InsereMarcadorTrafo(rede->LisLigacao());
    // fecha "folder"
    FolderClose();
    // ==================================
    // abre "folder" p/ identificar redes
    FolderOpen("Redes");
    if (rede->BarraInicial() != NULL)
    {
        VTBarra* pBarra;
        pBarra = (VTBarra*)rede->BarraInicial();
        int x, y;
        pBarra->CoordenadasUtm_cm(x, y);
        InserePonto(x, y, rede->Codigo, rede->Color, "pal4/icon28", " ");
    }
    // fecha "folder"
    FolderClose();
    // ==================================
    // fecha "document"
    DocumentClose();
    // grava o arquivo
    Lines->SaveToFile(arq_kml);
    return (true);
}

//---------------------------------------------------------------------------
void __fastcall TExportaKml::DocumentClose(void)
{
    Lines->Add("</Document>");
    Lines->Add("</kml>");
}

//---------------------------------------------------------------------------
void __fastcall TExportaKml::DocumentOpen(AnsiString nome)
{
    Lines->Add( "<Document>");
    Lines->Add( "\t<name>"  + nome +  "</name>");
}

//---------------------------------------------------------------------------
void __fastcall TExportaKml::FolderClose(void)
{
    Lines->Add( "\t</Folder>");
}

//---------------------------------------------------------------------------
void __fastcall TExportaKml::FolderOpen(AnsiString nome)
{
    Lines->Add( "\t<Folder>");
    Lines->Add( "\t\t<name>" + nome + "</name>");
}

//---------------------------------------------------------------------------
void __fastcall TExportaKml::Header(void)
{
    Lines->Add( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    Lines->Add( "<kml xmlns=\"http://earth.google.com/kml/2.0\">");
}

//---------------------------------------------------------------------------
void __fastcall TExportaKml::InsereLinhas(TList *lisLIGACAO, AnsiString codigo_rede)
{
    int x1, x2, y1, y2;
    double lat1, lon1, lat2, lon2;
    double lat_wgs, lon_wgs, h;
    VTLigacao *ligacao;

    for (int nl = 0; nl < lisLIGACAO->Count; nl++)
    {
        ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
        //obtém coordenada da Barra
        ligacao->CoordenadasUtm_cm(x1, y1, x2, y2);
// jss20191014
        // os ajustes empíricos usados nesse método foram retirados
        // ajuste das coordenadas
//      x1-=2200;
//      y1-=1700;
//      x2-=2200;
//      y2-=1700;
        //converte coordenadas utm p/ latitude/longitude (coord.utm estão em cm)
//      scg->NorthEast_to_LatLon(y1 / 100., x1 / 100., lat1, lon1, scg->TipoCoord);
//      scg->NorthEast_to_LatLon(y2 / 100., x2 / 100., lat2, lon2, scg->TipoCoord);
        scg->NorthEast_to_LatLon(y1 / 100., x1 / 100., lat1, lon1);
        scg->NorthEast_to_LatLon(y2 / 100., x2 / 100., lat2, lon2);
        if (scg->ProjCS.datum != datumWGS84)
        {
            scg->DatumConvert(lat1, lon1, 0.0, lat_wgs, lon_wgs, h);
            lat1 = lat_wgs;
            lon1 = lon_wgs;
            scg->DatumConvert(lat2, lon2, 0.0, lat_wgs, lon_wgs, h);
            lat2 = lat_wgs;
            lon2 = lon_wgs;
        }
        //insere linhas
        Lines->Add("\t\t<Style id=\"" + codigo_rede + "\">");
        Lines->Add("\t\t\t<LineStyle>");
        Lines->Add("\t\t\t\t<width>3.0</width>");
        Lines->Add("\t\t\t\t<color>" + ConverteCor(ligacao->rede->Color) + "</color>");
        Lines->Add("\t\t\t</LineStyle>");
        Lines->Add("\t\t</Style>");

        Lines->Add("\t\t<Placemark>");
        Lines->Add("\t\t\t<name>" + codigo_rede + "</name>");
        //Lines->Add("\t\t\t<description>" + codigo_rede + "</description>");
        Lines->Add("\t\t\t<styleUrl>#" + codigo_rede + "</styleUrl>");
        Lines->Add("\t\t\t<gx:balloonVisibility>1</gx:balloonVisibility>");
        Lines->Add("\t\t\t<LineString>");
        Lines->Add("\t\t\t<tessellate>1</tessellate>");
        Lines->Add("\t\t\t\t<coordinates>");
        Lines->Add("\t\t\t\t\t" + DoubleToStr("%16.15f", lon1, '.') + "," +
                                  DoubleToStr("%16.15f", lat1, '.') + ",0 " +
                                  DoubleToStr("%16.15f", lon2, '.') + "," +
                                  DoubleToStr("%16.15f", lat2, '.') + ",0");
        Lines->Add("\t\t\t\t</coordinates>");
        Lines->Add("\t\t\t</LineString>");
        Lines->Add("\t\t</Placemark>");
    }
}

//---------------------------------------------------------------------------
void __fastcall TExportaKml::InsereMarcadorChave(TList *lisLIGACAO)
{
    AnsiString descricao;
    int x, y;
    VTLigacao *ligacao;

    for (int nl = 0; nl < lisLIGACAO->Count; nl++)
    {
        ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
        if(ligacao->Tipo() == eqptoCHAVE)
        {
            VTChave* pChave;
            pChave = (VTChave*)ligacao;
            ligacao->pbarra2->CoordenadasUtm_cm(x,y);
            if (pChave->Aberta)
            {
                descricao = "Chave aberta";
                InserePonto(x, y, ligacao->Codigo, clGreen , "pal3/icon63", descricao);
            }
            if (pChave->Fechada)
            {
                descricao = "Chave fechada";
                InserePonto(x, y, ligacao->Codigo, clRed, "pal3/icon63", descricao);
            }
            if (pChave->Bloqueada)
            {
                descricao = "Chave bloqueada";
                InserePonto(x, y, ligacao->Codigo, clYellow , "pal3/icon63", descricao);
            }
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TExportaKml::InsereMarcadorTrafo(TList *lisLIGACAO)
{
    AnsiString descricao;
    int x, y;
    VTLigacao *ligacao;

    for (int nl = 0; nl < lisLIGACAO->Count; nl++)
    {
        ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
        if(ligacao->Tipo() == eqptoTRAFO)
        {
            VTTrafo* pTrafo;
            pTrafo = (VTTrafo*)ligacao;
            descricao = "Potência nominal: " +
                        FloatToStr(pTrafo->snom) + " MVA";
            ligacao->pbarra1->CoordenadasUtm_cm(x,y);
            InserePonto(x, y, ligacao->Codigo, ligacao->rede->Color, "pal4/icon60", descricao);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TExportaKml::InserePonto(int x, int y, AnsiString nome_ligacao, TColor cor, AnsiString marcador, AnsiString descricao)
{
    double lat, lon;
    double lat_wgs, lon_wgs, h;

// jss20191014
    // os ajustes empíricos usados nesse método foram retirados
    //ajuste das coordenadas
//  x-=2200;
//  y-=1700;

    //converte coordenadas utm p/ latlon
//  scg->NorthEast_to_LatLon(y / 100., x / 100., lat, lon, scg->TipoCoord);
    scg->NorthEast_to_LatLon(y / 100., x / 100., lat, lon);
    if (scg->ProjCS.datum != datumWGS84)
    {
        scg->DatumConvert(lat, lon, 0.0, lat_wgs, lon_wgs, h);
        lat = lat_wgs;
        lon = lon_wgs;
    }
    //insere ponto
    Lines->Add("\t\t<Style id=\"" + nome_ligacao + "-downArrowIcon\">");
    Lines->Add("\t\t\t<IconStyle>");
    Lines->Add("\t\t\t\t<scale>1.5</scale>");
    Lines->Add("\t\t\t\t<color>" + ConverteCor(cor) + "</color>");
    Lines->Add("\t\t\t\t<Icon>");
    Lines->Add("\t\t\t\t\t<href>http://maps.google.com/mapfiles/kml/" + marcador + ".png</href>");
    Lines->Add("\t\t\t\t</Icon>");
    Lines->Add("\t\t\t</IconStyle>");
    Lines->Add("\t\t</Style>");

    Lines->Add("\t\t<Placemark>");
    Lines->Add("\t\t\t<name>" + nome_ligacao + "</name>");
    Lines->Add("\t\t\t<description>" + descricao + "</description>");
    Lines->Add("\t\t\t<styleUrl>#" + nome_ligacao + "-downArrowIcon</styleUrl>");
    Lines->Add("\t\t\t<gx:balloonVisibility>1</gx:balloonVisibility>");
    Lines->Add("\t\t\t<Point>");
    Lines->Add("\t\t\t\t<coordinates>" + DoubleToStr("%16.15f",lon,'.') + ", "
                                       + DoubleToStr("%16.15f",lat,'.') + ", 0.0 </coordinates>");
    Lines->Add("\t\t\t</Point>");
    Lines->Add("\t\t</Placemark>");
}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaKml::ConverteCor(TColor Cor)
{
    AnsiString TextoCor = "FF" + IntToHex(Cor, 6);
    return(TextoCor);
}

//---------------------------------------------------------------------------
//eof
