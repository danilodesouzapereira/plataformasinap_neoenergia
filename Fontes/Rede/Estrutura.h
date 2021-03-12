//---------------------------------------------------------------------------
#ifndef EstruturaH
#define EstruturaH

//arquivos inclu�dos-----------------------------------------------------------
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
struct strPOT  {//pot�ncia
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
struct strADM  {//admit�ncia
               float g, b;
               };
//---------------------------------------------------------------------------
struct strIMP  {//imped�ncia
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
struct strCOORD{//coordenadas (utm ou esq) p/ representa��o gr�fico de qualquer Eqpto
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
struct strESQ_BARRA {//dados de Barra p/ para diag.esquem�tico
                    int x1, y1, x2, y2;
                    };
//---------------------------------------------------------------------------
struct strESQ_EQBAR {//dados de Ligacao p/ para diag.esquem�tico
                    float posbar;        //posi��o nas barras (%)
                    int   dxbar, dybar;  //dist�ncia em rela��o ao ponto de conex�o na barra
                    };
//---------------------------------------------------------------------------
struct strESQ_LIGA  {//estrutura p/ representa��o esquem�tica da Ligacao
                    float posbar1, posbar2, posbar3; //posi��o nas barras (%) para diag.esquem�tico
                    int   comprimento;               //uso tempor�rio p/ salvar comprimento
                                                     //durante montagem de diag.esquem�tico
                    };
//---------------------------------------------------------------------------
struct strESQ_REDE    {//estrutura p/ representa��o esquem�tica de rede reduzida
                      VTBarra *pbarra;    //Barra fict�cia p/ desenhar o esquem�tico
                      TList   *lisBAR;    //lista de Barras fict�cias p/ conectar os Trechos
                      TList   *lisLIG;    //lista de Ligacoes fict�cias (Trechos)
                      int     height;
                      int     width;
                      };
//---------------------------------------------------------------------------
struct strESQ_MEDIDOR {//estrutura p/ representa��o esquem�tica do Medidor
                      int dxbar, dybar;  //dist�ncia (m) em rela��o ao ponto de conex�o no Eqpto
                      int width, height; //dimens�o (pixel) da caixa de texto
                      };

//---------------------------------------------------------------------------
#endif
//eof


