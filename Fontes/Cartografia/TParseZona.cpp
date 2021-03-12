// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TParseZona.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "VTScg.h"
#include "..\Licenca\VTEmpresa.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TParseZona::TParseZona(VTApl *apl)
{
    // salva ponteiro p/ objetos
    this->apl = apl;
    try
    { // cria objeto TStringList
        campos = new TStringList();
        lines = new TStringList();
        new_lines = new TStringList(); // DVK 2014.11.07
        // inicia dados
        utm_zone.x = "";
        utm_zone.y = "";
        // DVK 2014.11.07
        tipo_coord = coordNONE;
        VerificaArquivoZonas();
    }
    catch (Exception &e)
    {
    }
}

// -----------------------------------------------------------------------------
__fastcall TParseZona::~TParseZona(void)
{
    // destrói objetos criados
   if (new_lines){delete lines;  lines  = NULL;} //DVK 2014.11.07
   if (lines)    {delete lines;  lines  = NULL;}
   if (campos)   {delete campos; campos = NULL;}
    }

// -----------------------------------------------------------------------------
double __fastcall TParseZona::CentralMeridian(void)
{ // DVK 2014.11.07
    return (polyc.centralMeridian);
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TParseZona::ReadProjCSParms(TProjCS &ProjCS)
{
    // jss20191014
    // adicionar no UP o arquivo ProjCS.txt para cada empresa
    VTPath    *path    = (VTPath*)apl->GetObject(__classid(VTPath));
    AnsiString arq_txt = path->DirGeral() + "\\ProjCS.txt";
    // verifica se existe o arquivo de definição das zonas utm das empresas
    // se o arquivo não existir usaremos valores padrão
    if (!FileExists(arq_txt))
    {
        ProjCS.datum               = datumSAD69;
        ProjCS.semi_eixo           = 6378160.0;
        ProjCS.invflat             = 298.25;
        ProjCS.achat               = 1.0 / ProjCS.invflat;
        ProjCS.equad               = 2.0 * ProjCS.achat -
                                     ProjCS.achat * ProjCS.achat;
        ProjCS.elinquad            = ProjCS.equad / (1.0 - ProjCS.equad);
        ProjCS.e1                  = (1.0 - sqrt(1.0 - ProjCS.equad)) /
                                     (1.0 + sqrt(1.0 - ProjCS.equad));
        ProjCS.deg2rad             = 0.0174532925199433;
        ProjCS.rad2deg             = 1.0 / ProjCS.deg2rad;
        ProjCS.false_easting       =   500000.0;
        ProjCS.false_northing      = 10000000.0;
        ProjCS.central_meridian    = -51.0; // COPEL // -45.0; // SP
        ProjCS.zone_name           = "22J"; // COPEL // "23K"; // SP
        ProjCS.zone_number         = 22;    // COPEL // 23;    // SP
        ProjCS.zone_letter         = "J";   // COPEL // "K"    // SP
        ProjCS.k0                  = 0.9996;
        ProjCS.origin_lat          = 0.0;
        ProjCS.tipo_coord          = coordUTM;
        ProjCS.dx                  = 67.35;
        ProjCS.dy                  = -3.88;
        ProjCS.dz                  = 38.22;
        return(false);
    }
    // carrega arquivo em lines
    lines->LoadFromFile(arq_txt);
    // lê todas as linhas
    // req_fields terá um bit posicionado para cada campo required.
    // ao final testa-se se todos os campos necessários (required) foram
    // fornecidos.
    int req_fields = 0;
    for (int nl = 0; nl < lines->Count; nl++)
    {   // extrai campos da linha de texto
        if (ExtraiCampos(lines->Strings[nl], campos) != 2)
            continue;
        if (!campos->Strings[0].CompareIC("datum_code"))
        {
            ProjCS.datum_code          = campos->Strings[1];
        }
        else if (!campos->Strings[0].CompareIC("datum_name"))
        {
            ProjCS.datum_name          = campos->Strings[1];
            if (!ProjCS.datum_name.AnsiCompareIC("D_South_American_1969"))
                ProjCS.datum           = datumSAD69;
            else if (!ProjCS.datum_name.AnsiCompareIC("D_WGS_1984"))
                ProjCS.datum           = datumWGS84;
            req_fields                ^= 0x0001;
        }
        else if (!campos->Strings[0].CompareIC("spheroid_name"))
        {
            ProjCS.spheroid_name       = campos->Strings[1];
        }
        else if (!campos->Strings[0].CompareIC("semi_eixo"))
        {
            ProjCS.semi_eixo           = StrToDouble(campos->Strings[1]);
            req_fields                ^= 0x0002;
        }
        else if (!campos->Strings[0].CompareIC("invflat"))
        {
            ProjCS.invflat             = StrToDouble(campos->Strings[1]);
            ProjCS.achat               = 1.0 / ProjCS.invflat;
            ProjCS.equad               = 2.0 * ProjCS.achat -
                                         ProjCS.achat * ProjCS.achat;
            ProjCS.elinquad            = ProjCS.equad / (1.0 - ProjCS.equad);
            ProjCS.e1                  = (1.0 - sqrt(1.0 - ProjCS.equad)) /
                                         (1.0 + sqrt(1.0 - ProjCS.equad));
            req_fields                ^= 0x0004;
        }
        else if (!campos->Strings[0].CompareIC("greenwich"))
        {
            ProjCS.greenwich           = StrToDouble(campos->Strings[1]);
        }
        else if (!campos->Strings[0].CompareIC("deg2rad"))
        {
            ProjCS.deg2rad             = StrToDouble(campos->Strings[1]);
            ProjCS.rad2deg             = 1.0 / ProjCS.deg2rad;
            req_fields                ^= 0x0008;
        }
        else if (!campos->Strings[0].CompareIC("projection_name"))
        {
            ProjCS.projection_name     = campos->Strings[1];
        }
        else if (!campos->Strings[0].CompareIC("false_easting"))
        {
            ProjCS.false_easting       = StrToDouble(campos->Strings[1]);
            req_fields                ^= 0x0010;
        }
        else if (!campos->Strings[0].CompareIC("false_northing"))
        {
            ProjCS.false_northing      = StrToDouble(campos->Strings[1]);
            req_fields                ^= 0x0020;
        }
        else if (!campos->Strings[0].CompareIC("central_meridian"))
        {
            ProjCS.central_meridian    = StrToDouble(campos->Strings[1]);
            req_fields                ^= 0x0040;
        }
        else if (!campos->Strings[0].CompareIC("zone_name"))
        {
            ProjCS.zone_name           = campos->Strings[1];
            ProjCS.zone_number         =
                campos->Strings[1].SubString(0, 2).ToInt();
            ProjCS.zone_letter         = campos->Strings[1].SubString(2, 1);
            req_fields                ^= 0x0080;
        }
        else if (!campos->Strings[0].CompareIC("k0"))
        {
            ProjCS.k0                  = StrToDouble(campos->Strings[1]);
            req_fields                ^= 0x0100;
        }
        else if (!campos->Strings[0].CompareIC("origin_lat"))
        {
            ProjCS.origin_lat          = StrToDouble(campos->Strings[1]);
            req_fields                ^= 0x0200;
        }
        else if (!campos->Strings[0].CompareIC("unit_name"))
        {
            ProjCS.unit_name           = campos->Strings[1];
        }
        else if (!campos->Strings[0].CompareIC("unit_value"))
        {
            ProjCS.unit_value          = StrToDouble(campos->Strings[1]);
        }
        else if (!campos->Strings[0].CompareIC("tipo_coord"))
        {
            if (!campos->Strings[1].CompareIC("coordUTM"))
                ProjCS.tipo_coord      = coordUTM;
            else if (!campos->Strings[1].CompareIC("coordLATLON"))
                ProjCS.tipo_coord      = coordLATLON;
            else if (!campos->Strings[1].CompareIC("coordPOLYC"))
                ProjCS.tipo_coord      = coordPOLYC;
            req_fields                ^= 0x0400;
        }
        else if (!campos->Strings[0].CompareIC("dx"))
        {
            ProjCS.dx                  = StrToDouble(campos->Strings[1]);
            req_fields                ^= 0x0800;
        }
        else if (!campos->Strings[0].CompareIC("dy"))
        {
            ProjCS.dy                  = StrToDouble(campos->Strings[1]);
            req_fields                ^= 0x1000;
        }
        else if (!campos->Strings[0].CompareIC("dz"))
        {
            ProjCS.dz                  = StrToDouble(campos->Strings[1]);
            req_fields                ^= 0x2000;
        }
    }
    // req_fields tem um bit posicionado para cada campo required.
    // testa-se se todos os campos necessários (required) foram
    // fornecidos.
    if (req_fields == 0x3FFF)
        return (true);
    else
        return (false);
}

// -----------------------------------------------------------------------------
bool __fastcall TParseZona::ExisteEmpresa(AnsiString empresa)
{
    // variáveis locais
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_txt = path->DirGeral() + "\\ZonasUTM.txt";

    // formato do arquivo
    // Nome_Empresa; Numero_zonaUTM; Letra_zonaUTM; Tipo_coordenada; ...
    // ...falseEasting; falseNorthing; centralMeridian; originLatitude
   try{//verifica se existe o arquivo de definição das zonas utm das empresas
      if (! FileExists(arq_txt)) return(false);
        // carrega arquivo em lines
        lines->LoadFromFile(arq_txt);
        // lê todas as linhas até encontrar a empresa indicada
        for (int nl = 0; nl < lines->Count; nl++)
        { // extrai campos da linha de texto
            // if (ExtraiCampos(lines->Strings[nl], campos) < 3) continue;  //DVK 2014.11.07
         if (ExtraiCampos(lines->Strings[nl], campos) < 8) continue;
            {//verifica se encontrou a empresa
            if (campos->Strings[0].CompareIC(empresa) == 0)
            { // salva coordenadas da zona
                utm_zone.x = campos->Strings[1];
                utm_zone.y = campos->Strings[2];
                // DVK 2014.11.07 coluna com tipo de coordenada e constantes polyc
                tipo_coord = StrToInt(campos->Strings[3]);
                polyc.falseEasting = StrToDouble(campos->Strings[4]);
                polyc.falseNorthing = StrToDouble(campos->Strings[5]);
                polyc.centralMeridian = StrToDouble(campos->Strings[6]);
                polyc.originLatitude = StrToDouble(campos->Strings[7]);
                return (true);
            }
        }
    }
      }catch(Exception &e)
    {
        return (false);
    }
    return (false);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
int __fastcall TParseZona::ExtraiCampos(AnsiString txt, TStrings *campos)
{
    // variáveis locais
#define EOS '\0'
    char *p_ini, *p_fim;
    char separador = ';';
    int  len = txt.Length();

    // reinicia StrinList campos
    campos->Clear();
    // filtra linha de comentário
    if (txt.Pos("//") > 0)
    {
        len = txt.Pos("//") - 1;
        txt = txt.SubString(1, len).Trim();
    }
    len = txt.Length();
    // filtra linha com menos de 2 caracteres
    if (len < 2)
        return (0);
    // se necessário, inclui separador no final de txt
    if (txt[len] != separador)
        txt += separador;
    // extrai os campos delimitados por separador em txt
    p_ini = txt.c_str();
    while ((p_fim = strchr(p_ini, separador)) != NULL)
    {   // insere string na lista
        *p_fim = EOS; // insere delimitador de string
        campos->Add(AnsiString(p_ini).Trim());
        // copia em pstr o substring p_ini a p_fim-1
        *p_fim = separador; // restaura separador no string original
        p_ini = p_fim + 1;
    }
    return (campos->Count);
}

// -----------------------------------------------------------------------------
double __fastcall TParseZona::FalseEasting(void)
{ // DVK 2014.11.07
    return (polyc.falseEasting);
}

// -----------------------------------------------------------------------------
double __fastcall TParseZona::FalseNorthing(void)
{ // DVK 2014.11.07
    return (polyc.falseNorthing);
}

// -----------------------------------------------------------------------------
void __fastcall  TParseZona::InsereColunas(AnsiString linha, int tipo, double fE, double fN, double cM, double oL)
{ // DVK 2014.11.07
    // adiciona campo
   linha = linha + IntToStr(tipo) + ";"
                 + DoubleToStr("%2.1f", fE, '.') + ";"
                 + DoubleToStr("%2.1f", fN, '.') + ";"
                 + DoubleToStr("%2.1f", cM, '.') + ";"
                 + DoubleToStr("%2.1f", oL, '.') + ";";
    new_lines->Add(linha);
}
/*
 //-----------------------------------------------------------------------------
 void __fastcall  TParseZona::InsereColunaTipoCoord(AnsiString linha, int tipo)
 {//DVK 2014.11.07
 //adiciona campo
 linha = linha + IntToStr(tipo) + ";";
 new_lines->Add(linha);
 }
 */
// -----------------------------------------------------------------------------
double __fastcall TParseZona::OriginLatitude(void)
{ // DVK 2014.11.07
    return (polyc.originLatitude);
}

// -----------------------------------------------------------------------------
int __fastcall TParseZona::TipoCoord(void)
{ // DVK 2014.11.07
    return (tipo_coord);
}

// -----------------------------------------------------------------------------
AnsiString __fastcall TParseZona::UTMZoneX(void)
{
    return (utm_zone.x);
}

// -----------------------------------------------------------------------------
AnsiString __fastcall TParseZona::UTMZoneY(void)
{
    return (utm_zone.y);
}

// -----------------------------------------------------------------------------
void __fastcall TParseZona::VerificaArquivoZonas(void)
{ // DVK 2014.11.07
    // variáveis locais
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_txt = path->DirGeral() + "\\ZonasUTM.txt";

    // verifica se existe o arquivo de definição das zonas utm das empresas
   if (! FileExists(arq_txt)) return;
    // carrega arquivo em lines
    lines->LoadFromFile(arq_txt);
    // DVK 2014.11.07 limpa new_lines
    new_lines->Clear();
    // lê todas as linhas
    for (int nl = 0; nl < lines->Count; nl++)
    { // extrai campos da linha de texto
        if (ExtraiCampos(lines->Strings[nl], campos) < 8)
        { // versão antiga: insere coluna tipo de coordenada e constantes polyc
            if (campos->Strings[0].CompareIC("ELETROBRAS - Alagoas") == 0)
            {
            InsereColunas(lines->Strings[nl], coordPOLYC,
                        1000000.0, 10000000.0, -36.751, -9.5);
            }
            else if (campos->Strings[0].CompareIC("CEAL") == 0)
            {
            InsereColunas(lines->Strings[nl], coordPOLYC,
                        1000000.0, 10000000.0, -36.751, -9.5);
            }
            else if (campos->Strings[0].CompareIC("ELETROBRAS - Piauí") == 0)
            {
            InsereColunas(lines->Strings[nl], coordPOLYC,
                        1000000.0, 10000000.0, -43.0, -7.0);
            }
            else
            {
            InsereColunas(lines->Strings[nl], coordUTM,
                        0.0, 0.0, 0.0, 0.0);
            }
        }
    }
    // salva arquivo
   if(new_lines->Count > 0) new_lines->SaveToFile(arq_txt);
}

// -----------------------------------------------------------------------------
// eof
