//---------------------------------------------------------------------------
#ifndef EstruturaH
#define EstruturaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\Constante\Fases.h"
//---------------------------------------------------------------------------
class VTBarra;

//---------------------------------------------------------------------------
#define Sbase100MVA   100

//---------------------------------------------------------------------------
struct strCOORD{//coordenadas (x,y)
               int x, y;
               };
//---------------------------------------------------------------------------
struct strPOT  {//potência
               float p, q;
               };
//---------------------------------------------------------------------------
struct strDEM  {//demanda
               //int    fases;
               //int    modelo; //modelo de carga
               float p;   //MW
               float q;   //MVAr
               };
//---------------------------------------------------------------------------
struct strSUP  {//suprimento
               float p;   //MW
               float q;   //MVAr
               float v;   //pu
               float t;   //radiano
               };
//---------------------------------------------------------------------------
struct strADM  {//admitância
               float g, b;
               };
//---------------------------------------------------------------------------
struct strIMP  {//impedância
               float r, x;
               };
//---------------------------------------------------------------------------
struct strTRAFO_EQV  {//atributos de trafo equivalente
                     char    modelo;
                     strADM  y0_ij_pu;
                     strADM  y0_ji_pu;
                     strADM  y1_ij_pu;
                     strADM  y1_ji_pu;
                     strADM  matYij_s[MAX_FASE][MAX_FASE];
                     strADM  matYji_s[MAX_FASE][MAX_FASE];
                     };
/*
//---------------------------------------------------------------------------
struct strCOORD{//coordenadas (utm ou esq) p/ representação gráfico de qualquer Eqpto
               int x1, y1, x2, y2, x3, y3, xm, ym;
               struct {
                      int     len;
                      TPoint *point;
                      }polyline;
               };
*/
//---------------------------------------------------------------------------
struct strUTM  {//dados de Ligacao p/ para diag.utm
               int x, y;
               };
//---------------------------------------------------------------------------
struct strESQ_BARRA {//dados de Barra p/ para diag.esquemático
                    int x1, y1, x2, y2;
                    };
//---------------------------------------------------------------------------
struct strESQ_EQBAR {//dados de Ligacao p/ para diag.esquemático
                    float posbar;        //posição nas barras (%)
                    int   dxbar, dybar;  //distância em relação ao ponto de conexão na barra
                    };
//---------------------------------------------------------------------------
struct strESQ_LIGA  {//estrutura p/ representação esquemática da Ligacao
                    float posbar1, posbar2, posbar3; //posição nas barras (%) para diag.esquemático
                    int   comprimento;               //uso temporário p/ salvar comprimento
                                                     //durante montagem de diag.esquemático
                    };
//---------------------------------------------------------------------------
struct strESQ_REDE    {//estrutura p/ representação esquemática de rede reduzida
                      VTBarra *pbarra;    //Barra fictícia p/ desenhar o esquemático
                      TList   *lisBAR;    //lista de Barras fictícias p/ conectar os Trechos
                      TList   *lisLIG;    //lista de Ligacoes fictícias (Trechos)
                      int     height;
                      int     width;
                      };
//---------------------------------------------------------------------------
struct strESQ_MEDIDOR {//estrutura p/ representação esquemática do Medidor
                      int dxbar, dybar;  //distância (m) em relação ao ponto de conexão no Eqpto
                      int width, height; //dimensão (pixel) da caixa de texto
                      };

//---------------------------------------------------------------------------
#endif
//eof


