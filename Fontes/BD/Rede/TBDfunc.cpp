//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <SystemTypes.h>
#include "TBDfunc.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Importa\Monta\TStruct.h"
#include "..\Log\VTLog.h"
#include "..\Mapa\VTPoligono.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTMatZ.h"
#include "..\Rede\VTMatY.h"
#include "..\Rede\VTMatC.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrafo.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBDfunc::TBDfunc(VTApl* apl)
   {
   AnsiString strlog;

   this->apl = apl;

   plog = (VTLog*)apl->GetObject(__classid(VTLog));

   if ((curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas))) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      }

   if ((classes = (VTClasses*)apl->GetObject(__classid(VTClasses))) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      }

   }

//---------------------------------------------------------------------------
__fastcall TBDfunc::~TBDfunc()
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DefineStatusEqpto(TList* lisEqp, bool status)
   {
   AnsiString strlog;
   VTEqpto*   eqpto;

   if (!lisEqp)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   for (int n = 0; n < lisEqp->Count; n++)
      {
      eqpto = (VTEqpto*)lisEqp->Items[n];
      DefineStatusEqpto(eqpto, status);
      }

   return(true);
   } // DefineStatusEqpto

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DefineStatusEqpto(VTEqpto* eqpto, bool status)
   {
   AnsiString strlog;

   if (!eqpto)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_EQUIPINV
      return(false);
      }

   eqpto->DefineStatus(sttNOVO,      status);
   eqpto->DefineStatus(sttALTERADO,  status);
   eqpto->DefineStatus(sttDESLOCADO, status);

   return(true);
   } // DefineStatusEqpto

//---------------------------------------------------------------------------
VTChave* __fastcall TBDfunc::ExisteChave(VTRedes* redes, int intRedeId, int intChaveId)
   {
   //variáveis locais
   VTChave* chave;
   VTRede*  rede;
   TList*   lisEQP;

   try
      {
      lisEQP = new TList();

      //verifica se a Rede da Chave está carregada
      if (!redes)                                        return(NULL);
      if ((rede = redes->ExisteRede(intRedeId)) == NULL) return(NULL);
      if (!rede->Carregada)                              return(NULL);
      //determina Chave na rede
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoCHAVE);
      for (int n = 0; n < lisEQP->Count; n++)
         {
         chave = (VTChave*)lisEQP->Items[n];
         if (chave->Id == intChaveId) return(chave);
         }

      delete lisEQP;
      }
   catch (Exception& e)
      {
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TStream* __fastcall TBDfunc::MontaStreamMatComplex(complex<double> matComplex[4][4])
   {
   TStream* ptrStream;
   int      intaux;
   double   dblaux;

   ptrStream = new TMemoryStream();
   if (!MontaStreamMatComplex(matComplex, ptrStream))
      {
      delete ptrStream;
      ptrStream = NULL;
      }

   return(ptrStream);
   } // MontaStreamMatComplex(complex<double> matComplex[4][4])

//---------------------------------------------------------------------------
bool     __fastcall TBDfunc::MontaStreamMatComplex(complex<double> matComplex[4][4], TStream* stream)
   {
   int            intaux;
   double         dblaux;
   unsigned char  buffer[2 * 16 * sizeof(double)]; // Matriz 4x4
   unsigned char* ptrBuffer;

   // Inicia estruturas de dados auxiliares
   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;

   for (int i = 0; i < 4; i++)
      {
      for (int j = 0; j < 4; j++)
         {
         dblaux = matComplex[i][j].real();
         memcpy(ptrBuffer, &(dblaux), sizeof(dblaux)); ptrBuffer += sizeof(dblaux);
         dblaux = matComplex[i][j].imag();
         memcpy(ptrBuffer, &(dblaux), sizeof(dblaux)); ptrBuffer += sizeof(dblaux);
         }
      }

   // Finalmente escreve na stream
   stream->Write(buffer, ptrBuffer - buffer);

   return(true);
   } // MontaStreamMatComplex(complex<double> matComplex[4][4], TStream* stream)

//---------------------------------------------------------------------------
bool     __fastcall TBDfunc::DesmontaStreamMatComplex(TStream* ptrStream, complex<double> matComplex[4][4])
   {
   int            intaux;
   double         dbl_real, dbl_imag;
   unsigned char  buffer[2 * 16 * sizeof(double)]; // 4x4
   unsigned char* ptrBuffer;
   int            intSize;

   if (!ptrStream)
      {
      return(false);
      }

   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
   intSize = ptrStream->Size;
   // Verifica se não houve nenhum problema no dimensionamento
   if (intSize > (int)sizeof(buffer))
      {
      return(false);
      }
   ptrStream->Read(buffer, intSize);

   for (int i = 0; i < 4; i++)
      {
      for (int j = 0; j < 4; j++)
         {
         memcpy(&(dbl_real), ptrBuffer, sizeof(dbl_real)); ptrBuffer += sizeof(dbl_real);
         memcpy(&(dbl_imag), ptrBuffer, sizeof(dbl_imag)); ptrBuffer += sizeof(dbl_imag);
         matComplex[i][j] = complex<double>(dbl_real, dbl_imag);
         }
      }

   return(true);
   } // DesmontaStreamMatComplex(TStream* ptrStream, complex<double> matComplex[4][4])

//---------------------------------------------------------------------------
TStream* __fastcall TBDfunc::MontaStreamPoligono(VTPoligono* poligono)
   {
   TStream* ptrStream;

   ptrStream = new TMemoryStream();
   if (!MontaStreamPoligono(poligono, ptrStream))
      {
      delete ptrStream;
      ptrStream = NULL;
      }

   return(ptrStream);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::MontaStreamPoligono(VTPoligono* poligono, TStream* stream)
   {
   int            intaux;
   double         dblaux;
   unsigned char* ptrBuffer;
   unsigned char* ptrBufferAux;
   TPoint*        point;

   try
      {
      if (!poligono) return(false);
      if (!stream) return(false);
      
      ptrBuffer = new unsigned char[2 * sizeof(int) * poligono->Size];
      ptrBufferAux = ptrBuffer;
      for (int i = 0; i < poligono->Size; i++)
         {
         if ((point = poligono->Point[i]) == NULL) return(NULL);
         memcpy(ptrBufferAux, &(point->x), sizeof(int)); ptrBufferAux += sizeof(int);
         memcpy(ptrBufferAux, &(point->y), sizeof(int)); ptrBufferAux += sizeof(int);
         }
         
      // Finalmente escreve na stream
      stream->Write(ptrBuffer, ptrBufferAux - ptrBuffer);
      }
   __finally
      {
      delete[] ptrBuffer;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DesmontaStreamPoligono(TStream* ptrStream, VTPoligono* poligono)
   {
   int x;
   int y;
   int numPontos;

   if (poligono == NULL) return(false);
   if (ptrStream == NULL) return(false);

   numPontos = ptrStream->Size / (sizeof(int) * 2);
   poligono->Clear();
   for (int i = 0; i < numPontos ; i++)
      {
      ptrStream->Read(&x, sizeof(int));
      ptrStream->Read(&y, sizeof(int));
      poligono->InserePonto(x, y);
      }

   return(true);
   }

//---------------------------------------------------------------------------
TStream* __fastcall TBDfunc::MontaStreamVTMatZ(VTMatZ* ptrMatZ)
   {
   TStream* ptrStream;
   int      intaux;
   double   dblaux;

   ptrStream = new TMemoryStream();
   if (!MontaStreamVTMatZ(ptrMatZ, ptrStream))
      {
      delete ptrStream;
      ptrStream = NULL;
      }

   return(ptrStream);
   } // MontaStreamVTMatZ(VTMatZ* ptrMatZ)

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::MontaStreamVTMatZ(VTMatZ* ptrMatZ, TStream* stream)
   {
   int            intaux;
   float          fltaux;
   unsigned char  buffer[sizeof(int) + (16 * sizeof(strIMP))]; // Matriz 4x4
   unsigned char* ptrBuffer;
   int            intSize;

   intaux = ptrMatZ->Dimensao;
   if (intaux > 4) return(false);  //matriz pode ter tamanho variável (máximo 4)

   // Inicia estruturas de dados auxiliares
   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
   memcpy(ptrBuffer, &(intaux), sizeof(intaux)); ptrBuffer += sizeof(intaux);

   for (int i = 0; i < ptrMatZ->Dimensao; i++)
      {
      for (int j = 0; j < ptrMatZ->Dimensao; j++)
         {
         fltaux = ptrMatZ->R[i][j];
         memcpy(ptrBuffer, &(fltaux), sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
         fltaux = ptrMatZ->X[i][j];
         memcpy(ptrBuffer, &(fltaux), sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
         }
      }

   // Finalmente escreve na stream
   stream->Write(buffer, ptrBuffer - buffer);

   return(true);
   } // MontaStreamVTMatZ(VTMatZ* ptrMatZ, TStream* stream)

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DesmontaStreamVTMatZ(TStream* ptrStream, strIMP matZ[4][4])
   {
   int            intDimensao;
   int            intaux;
   float          fltaux;
   unsigned char  buffer[sizeof(int) + (16 * sizeof(strIMP))]; // 4x4
   unsigned char* ptrBuffer;
   int            intSize;

   if (!ptrStream)
      {
      return(false);
      }

   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
   intSize = ptrStream->Size;
   // Verifica se não houve nenhum problema no dimensionamento
   if (intSize > (int)sizeof(buffer))
      {
      return(false);
      }
   ptrStream->Read(buffer, intSize);

   // Verificar rotina MontaStreamVTMatZ para determinar ordem de escrita
   memcpy(&(intDimensao), ptrBuffer, sizeof(intDimensao)); ptrBuffer += sizeof(intDimensao);
   if (intDimensao > 4)
      {
      return(false);
      }

   for (int i = 0; i < intDimensao; i++)
      {
      for (int j = 0; j < intDimensao; j++)
         {
         memcpy(&(fltaux), ptrBuffer, sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
         matZ[i][j].r = fltaux;
         memcpy(&(fltaux), ptrBuffer, sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
         matZ[i][j].x = fltaux;
         }
      }

   return(true);
   } // DesmontaStreamVTMatZ

//---------------------------------------------------------------------------
TStream* __fastcall TBDfunc::MontaStreamTrafoEqv(VTTrafo *trafo)
   {
   TStream* ptrStream;
   int      intaux;
   double   dblaux;

   ptrStream = new TMemoryStream();
   if (! MontaStreamTrafoEqv(trafo, ptrStream))
      {
      delete ptrStream;
      ptrStream = NULL;
      }

   return(ptrStream);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::MontaStreamTrafoEqv(VTTrafo *trafo, TStream* stream)
   {
   //variáveis locais
   strTRAFO_EQV eqv;

   //proteção
   if (trafo->eqv.matYij_s->Dimensao != MAX_FASE) return(false);
   //copia dados do Trafo para a estrutura local a ser gravada no stream
   eqv.modelo   = trafo->eqv.modelo;
   eqv.y0_ij_pu = trafo->eqv.y0_ij_pu;
   eqv.y0_ji_pu = trafo->eqv.y0_ji_pu;
   eqv.y1_ij_pu = trafo->eqv.y1_ij_pu;
   eqv.y1_ji_pu = trafo->eqv.y1_ji_pu;
   //copia matYij_s
   for (int i = 0; i < MAX_FASE; i++)
      {
      for (int j = 0; j < MAX_FASE; j++)
         {
         eqv.matYij_s[i][j].g = trafo->eqv.matYij_s->G[i][j];
         eqv.matYij_s[i][j].b = trafo->eqv.matYij_s->B[i][j];
         }
      }
   //copia matYji_s
   for (int i = 0; i < MAX_FASE; i++)
      {
      for (int j = 0; j < MAX_FASE; j++)
         {
         eqv.matYji_s[i][j].g = trafo->eqv.matYji_s->G[i][j];
         eqv.matYji_s[i][j].b = trafo->eqv.matYji_s->B[i][j];
         }
      }
   //escreve na stream
   stream->Write((char*)(&eqv), sizeof(strTRAFO_EQV));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DesmontaStreamTrafoEqv(TStream* ptrStream, strTRAFO_EQV *eqv)
   {
   //proteção
   if (! ptrStream) return(false);
   //verifica se a dimensão do stream é igual ao da estrutura
   if (ptrStream->Size != (int)sizeof(strTRAFO_EQV)) return(false);
   //lê dados do stream na estrutura
   ptrStream->Read((char*)(eqv), sizeof(strTRAFO_EQV));

   return(true);
   }

//---------------------------------------------------------------------------
TStream* __fastcall TBDfunc::MontaStreamVTMatC(VTMatC* ptrMatC)
   {
   TStream* ptrStream;
   int      intaux;
   double   dblaux;

   ptrStream = new TMemoryStream();
   if (!MontaStreamVTMatC(ptrMatC, ptrStream))
      {
      delete ptrStream;
      ptrStream = NULL;
      }

   return(ptrStream);
   } // MontaStreamVTMatC(VTMatC* ptrMatC)

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::MontaStreamVTMatC(VTMatC* ptrMatC, TStream* stream)
   {
   int            intaux;
   float          fltaux;
   unsigned char  buffer[sizeof(int) + 16 * sizeof(strIMP)]; // Matriz 4x4
   unsigned char* ptrBuffer;
   int            intSize;

   intaux = ptrMatC->Dimensao;
   if (intaux > 4) return(false);  //matriz pode ter tamanho variável (máximo 4)

   // Inicia estruturas de dados auxiliares
   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
   memcpy(ptrBuffer, &(intaux), sizeof(intaux)); ptrBuffer += sizeof(intaux);

   for (int i = 0; i < ptrMatC->Dimensao; i++)
      {
      for (int j = 0; j < ptrMatC->Dimensao; j++)
         {
         fltaux = ptrMatC->Cap[i][j];
         memcpy(ptrBuffer, &(fltaux), sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
         }
      }

   // Finalmente escreve na stream
   stream->Write(buffer, ptrBuffer - buffer);

   return(true);
   } // MontaStreamVTMatC(VTMatC* ptrMatC, TStream* stream)

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DesmontaStreamVTMatC(TStream* ptrStream, double matC[4][4])
   {
   int            intDimensao;
   int            intaux;
   float          fltaux;
   unsigned char  buffer[sizeof(int) + 16 * sizeof(strIMP)]; // 4x4
   unsigned char* ptrBuffer;
   int            intSize;

   if (!ptrStream)
      {
      return(false);
      }

   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
   intSize = ptrStream->Size;
   // Verifica se não houve nenhum problema no dimensionamento
   if (intSize > (int)sizeof(buffer))
      {
      return(false);
      }
   ptrStream->Read(buffer, intSize);

   // Verificar rotina MontaStreamVTMatZ para determinar ordem de escrita
   memcpy(&(intDimensao), ptrBuffer, sizeof(intDimensao)); ptrBuffer += sizeof(intDimensao);
   if (intDimensao > 4)
      {
      return(false);
      }

   for (int i = 0; i < intDimensao; i++)
      {
      for (int j = 0; j < intDimensao; j++)
         {
         memcpy(&(fltaux), ptrBuffer, sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
         matC[i][j] = fltaux;
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
TStream* __fastcall TBDfunc::MontaStreamCurvaEquipamento(int intTipoEquipId, VTCurva* curva)
   {
   TStream*             ptrStream;
   AnsiString           strlog;

   if (!curva)
      {
      SNPSTRLOGLINE(strlog);
      return(NULL);
      }

   try
      {
      //aloca TMemoryStream
      ptrStream = new TMemoryStream();

      if (!MontaStreamCurvaEquipamento(intTipoEquipId, curva, ptrStream))
         {
         delete ptrStream;
         ptrStream = NULL;
         }
      }
   catch(Exception &e)
      {
      if (ptrStream) delete ptrStream;
      ptrStream = NULL;
      }

   return(ptrStream);
   } // MontaStreamCurvaEquipamento

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::MontaStreamCurvaEquipamento(int intTipoEquipId, VTCurva* curva, TStream* stream)
   {
   bool                 sucesso = true;
   int                  num_pto;
   strPATAMAR           str_patamar;
   int                  intaux;
   AnsiString           strlog;
   struct strCURHEADER  curhdr;
   unsigned char        buffer[sizeof(struct strCURVA)];
   unsigned char*       ptrBuffer;
   int                  intSize;
   float                fltaux;
   unsigned char        uchaux;

   if (!curva)
      {
      SNPSTRLOGLINE(strlog);
      return(NULL);
      }

   try
      {
      // determina número de pontos da Curva
      if ((num_pto = curva->NumeroPontos()) == 0) return(NULL);

      if (!stream)
        return(false);

      // Inicia estruturas de dados auxiliares
      memset(buffer, 0, sizeof(buffer));
      ptrBuffer = buffer;

      // Inicia o cabeçalho da curva
      memset(&curhdr, IDINDEFINIDO, sizeof(curhdr));
      curhdr.uchTipoEquipId = (unsigned char)intTipoEquipId;
      curhdr.uchClasseId = (unsigned char)(curva->Classe ? curva->Classe->Id : IDINDEFINIDO);
      curhdr.uchNumValores = (unsigned char)curva->NumeroValores;

      if ((curva->Classe)&&(curva->Classe->Tag != consIP))
         {
         curhdr.fltValorMin = (float)curva->LimiteInferior;
         curhdr.fltValorMax = (float)curva->LimiteSuperior;
         }
      else
         {
         curhdr.fltValorMin = 0;
         curhdr.fltValorMax = 0;
         }
      curhdr.shtUnidade = (short)curva->Unidade;
      curhdr.shtEscala  = (short)curva->Escala;
      memcpy(ptrBuffer, &(curhdr), sizeof(curhdr)); ptrBuffer += sizeof(curhdr);

      // monta stream
      // salva os pontos
      for (int np = 0; np < num_pto; np++)
         {
         // loop pontos (ini)
         memset(str_patamar.valor, 0, sizeof(str_patamar.valor));
         curva->GetPonto(np, str_patamar.hm_ini, str_patamar.hm_fim, str_patamar.valor, sizeof(str_patamar.valor)/sizeof(double));

         uchaux = (unsigned char)str_patamar.hm_ini.hora;
         memcpy(ptrBuffer, &(uchaux), sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
         uchaux = (unsigned char)str_patamar.hm_ini.minuto;
         memcpy(ptrBuffer, &(uchaux), sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
         uchaux = (unsigned char)str_patamar.hm_fim.hora;
         memcpy(ptrBuffer, &(uchaux), sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
         uchaux = (unsigned char)str_patamar.hm_fim.minuto;
         memcpy(ptrBuffer, &(uchaux), sizeof(uchaux)); ptrBuffer += sizeof(uchaux);

         for (int nv = 0; nv < (int)curhdr.uchNumValores; nv++)
            {
            fltaux = str_patamar.valor[nv];
            memcpy(ptrBuffer, &(fltaux), sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
            }

         // Verifica se não houve nenhum problema no dimensionamento
         if ((ptrBuffer - buffer) > (int)sizeof(struct strCURVA))
            {
            return(false);
            }
         // loop pontos (fim)
         }

      // Finalmente escreve na stream
      stream->Write(buffer, ptrBuffer - buffer);
      }
   catch(Exception &e)
      {
      sucesso = false;
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DesmontaStreamCurvaEquipamento(TStream* ptrStream, struct strCURVA* ptrStrCurva, strCURHEADER& curhdr)
   {
   bool                 sucesso = true;
   int                  intaux;
   strPATAMAR           str_patamar;
   AnsiString           strlog;
   unsigned char        buffer[sizeof(struct strCURVA)];
   unsigned char*       ptrBuffer;
   int                  intSize;
   float                fltaux;
   unsigned char        uchaux;

   if (!ptrStream)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }
   if (!ptrStrCurva)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
   intSize = ptrStream->Size;
   // Verifica se não houve nenhum problema no dimensionamento
   if (intSize > (int)sizeof(buffer))
      {
      return(false);
      }
   ptrStream->Read(buffer, intSize);

   // Leitura do cabeçalho da curva
   // Nesse método o cabeçalho não está sendo utilizado, pois os dados
   // são obtidos a partir do equipamento. O cabeçalho foi introduzido para
   // as curvas típicas.
   memset(&curhdr, IDINDEFINIDO, sizeof(curhdr));
   memcpy(&curhdr, ptrBuffer, sizeof(curhdr)); ptrBuffer += sizeof(curhdr);
   //"limpa" estruturas que serão atualizadas a partir do buffer
   memset(ptrStrCurva, 0, sizeof(struct strCURVA));
   memset(&(str_patamar), 0, sizeof(str_patamar));
   //salva número de pontos e valores da curva
   //ptrStrCurva->num_pat   =  curhdr.shtEscala;
   ptrStrCurva->num_pat = 0;
   ptrStrCurva->num_valor =  curhdr.uchNumValores;
   //lê dados dos patamares
   while (ptrBuffer - buffer < intSize)
      {
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_ini.hora = (int)uchaux;
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_ini.minuto = (int)uchaux;
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_fim.hora = (int)uchaux;
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_fim.minuto = (int)uchaux;

      for (int n = 0; n < (int)curhdr.uchNumValores; n++)
         {
         // insere Ponto na Curva
         memcpy(&(fltaux), ptrBuffer, sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
         str_patamar.valor[n] = fltaux;
         }
      ptrStrCurva->patamar[ptrStrCurva->num_pat] = str_patamar;
      ptrStrCurva->num_pat++;
      }

   return(sucesso);
   } // DesmontaStreamCurvaEquipamento
 /*
//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DesmontaStreamCurvaTipica(TStream* ptrStream, int intConjCurvaId, AnsiString strDescricao, int intTipoCurvaId, int intStatus)
   {
   bool                sucesso = true;
   int                 intaux;
   strPATAMAR          str_patamar;
   AnsiString          strlog;
   VTCurva*            curva;
   struct strCURHEADER curhdr;
   unsigned char       buffer[sizeof(struct strCURVA)];
   unsigned char*      ptrBuffer;
   int                 intSize;
   float               fltaux;
   unsigned char       uchaux;

   if (!ptrStream)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   if (!curvas)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   if (!classes)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
   intSize = ptrStream->Size;
   // Verifica se não houve nenhum problema no dimensionamento
   if (intSize > (int)sizeof(buffer))
      {
      return(false);
      }
   ptrStream->Read(buffer, intSize);

   // Leitura do cabeçalho da curva
   memset(&curhdr, IDINDEFINIDO, sizeof(curhdr));
   memcpy(&curhdr, ptrBuffer, sizeof(curhdr)); ptrBuffer += sizeof(curhdr);
   switch ((int)curhdr.uchTipoEquipId)
      {
      case eqptoCAPACITOR :
         if (curvas->ExisteCurvaTipicaCapacitor(intConjCurvaId))  return(true);
         break;

      case eqptoCARGA :
         if (intTipoCurvaId == curvaLD)
            {// Iluminação pública
            if (curvas->ExisteCurvaTipicaIP(intConjCurvaId)) return(true);
            }
         else
            {// Demanda (PQ)
            if (curvas->ExisteCurvaTipicaCarga(intConjCurvaId))  return(true);
            }
         break;

      case eqptoGERADOR :
         if (curvas->ExisteCurvaTipicaGeradorFotovoltaico(intConjCurvaId)) return(true);
         break;

      case eqptoREATOR :
         if (curvas->ExisteCurvaTipicaReator(intConjCurvaId)) return(true);
         break;

      default :
         SNPSTRLOGLINE(strlog);
         return(false);
      }
   // O monta não tem métodos para inserção de curvas típicas
   curva                   = DLL_NewObjCurvaTipica();
   curva->Id               = intConjCurvaId;
   curva->Codigo           = strDescricao;
   curva->Tipo             = intTipoCurvaId;
   curva->StatusAsUnsigned = intStatus;
   curva->Tipica           = true;
   curva->Unidade          = (int)curhdr.shtUnidade;
   curva->Escala           = (int)curhdr.shtEscala;
   curva->Classe           = classes->ExisteClasse((int)curhdr.uchClasseId);
   curva->Status[sttNOVO]  = false;  //Define como sendo velha
   // Armazena os valores de consumo mínimo e máximo
   ((TCurvaTipica*)curva)->consumo.min = curhdr.fltValorMin;
   ((TCurvaTipica*)curva)->consumo.max = curhdr.fltValorMax;

   memset(&(str_patamar), 0, sizeof(str_patamar));
   while (ptrBuffer - buffer < intSize)
      {
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_ini.hora = (int)uchaux;
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_ini.minuto = (int)uchaux;
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_fim.hora = (int)uchaux;
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_fim.minuto = (int)uchaux;

      for (int n = 0; n < (int)curhdr.uchNumValores; n++)
         {
         memcpy(&(fltaux), ptrBuffer, sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
         str_patamar.valor[n] = fltaux;
         }
      curva->NumeroValores = (int)curhdr.uchNumValores;
      if (!curva->SetPonto(str_patamar.hm_ini, str_patamar.hm_fim, str_patamar.valor, (int)curhdr.uchNumValores))
         {
         SNPSTRLOGLINE(strlog);
         sucesso = false;
         break;
         }
      }
   // Como se trata de uma curva que foi lida, o status não é novo, nem alterado
   curva->Status[sttNOVO]     = false;
   curva->Status[sttALTERADO] = false;

   switch ((int)curhdr.uchTipoEquipId)
      {
      case eqptoCAPACITOR :
         if (!curvas->InsereCurvaTipicaCapacitor(curva))
            {
            SNPSTRLOGLINE(strlog);
            sucesso = false;
            }
         break;

      case eqptoCARGA :
         if (intTipoCurvaId == curvaLD)
            {
            // Iluminação pública
            if (!curvas->InsereCurvaTipicaIP(curva))
               {
               SNPSTRLOGLINE(strlog);
               sucesso = false;
               }
            break;
            }
         else
            {
            // Demanda (PQ)
            if (!curvas->InsereCurvaTipicaCarga(curva))
               {
               SNPSTRLOGLINE(strlog);
               sucesso = false;
               }
            }
         break;

      case eqptoGERADOR :
         if (!curvas->InsereCurvaTipicaGeradorFotovoltaico(curva))
            {
            SNPSTRLOGLINE(strlog);
            sucesso = false;
            }
         break;

      case eqptoREATOR :
         if (!curvas->InsereCurvaTipicaReator(curva))
            {
            SNPSTRLOGLINE(strlog);
            sucesso = false;
            }
         break;

      default :
         SNPSTRLOGLINE(strlog);
         sucesso = false;
      }

   if (!sucesso)
      {
      SNPSTRLOGLINE(strlog);
      if (curva)
         {
         delete(curva);
         }
      }

   return(sucesso);
   } // DesmontaStreamCurvaTipica
*/
//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DesmontaStreamCurvaTipica(TStream* ptrStream, int intConjCurvaId, AnsiString strDescricao, int intTipoCurvaId, int intStatus)
   {
   bool                sucesso = true;
   int                 intaux, classe_id;
   strPATAMAR          str_patamar;
   AnsiString          strlog;
   struct strCURHEADER curhdr;
   unsigned char       buffer[sizeof(struct strCURVA)];
   unsigned char*      ptrBuffer;
   int                 intSize;
   float               fltaux;
   unsigned char       uchaux;
   VTClasse            *classe;
   VTCurva             *curva;

   if (!ptrStream)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   if (!curvas)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   if (!classes)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
   intSize = ptrStream->Size;
   // Verifica se não houve nenhum problema no dimensionamento
   if (intSize > (int)sizeof(buffer))
      {
      return(false);
      }
   ptrStream->Read(buffer, intSize);

   // Leitura do cabeçalho da curva
   memset(&curhdr, IDINDEFINIDO, sizeof(curhdr));
   memcpy(&curhdr, ptrBuffer, sizeof(curhdr)); ptrBuffer += sizeof(curhdr);
   //determina Classe de consumidor associada à Curva (válida somente p/ curvas típicas de carga ou IP)
   classe_id = (int)curhdr.uchClasseId;
   classe = classes->ExisteClasse(classe_id);
   switch ((int)curhdr.uchTipoEquipId)
      {
      case eqptoCAPACITOR :
         if (curvas->ExisteCurvaTipicaCapacitor(intConjCurvaId))  return(true);
         break;

      case eqptoCARGA :
         if (classe == NULL)
            {//algumas curva utilizam classe_id = -1 p/ identificar IP
            if ((classe_id < 0)||(intTipoCurvaId == curvaLD)||(intTipoCurvaId == curvaPFP_MDP))
               {
               classe = classes->ExisteClasseTag(consIP);
               }
            }
         //proteção
         if (classe == NULL) return(false);
         if (classe->Tag == consIP)
            {// Iluminação pública
            if (curvas->ExisteCurvaTipicaIP(intConjCurvaId)) return(true);
            }
         else
            {// Demanda (PQ)
            if (curvas->ExisteCurvaTipicaCarga(intConjCurvaId))  return(true);
            }
         break;
      
      case eqptoGERADOR :
         if (curvas->ExisteCurvaTipicaGeradorFotovoltaicoId(intConjCurvaId)) return(true);
         break;

      case eqptoREATOR :
         if (curvas->ExisteCurvaTipicaReator(intConjCurvaId)) return(true);
         break;

      default :
         SNPSTRLOGLINE(strlog);
         return(false);
      }
   // O monta não tem métodos para inserção de curvas típicas
   curva                   = DLL_NewObjCurva();
   curva->Id               = intConjCurvaId;
//JCG 2014.01.15 - código temporário até alteração da base
   //curva->Codigo           = strDescricao;
   curva->CodigoBD           = strDescricao;
   curva->Tipo             = intTipoCurvaId;
   curva->StatusAsUnsigned = intStatus;
   curva->Tipica           = true;
   curva->Unidade          = (int)curhdr.shtUnidade;
   curva->Escala           = (int)curhdr.shtEscala;
   curva->Classe           = classe;   //classes->ExisteClasse((int)curhdr.uchClasseId);
   curva->Status[sttNOVO]  = false;  //Define como sendo velha
   // Armazena os valores de consumo mínimo e máximo
   curva->LimiteInferior = curhdr.fltValorMin;
   curva->LimiteSuperior = curhdr.fltValorMax;

   memset(&(str_patamar), 0, sizeof(str_patamar));
   while (ptrBuffer - buffer < intSize)
      {
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_ini.hora = (int)uchaux;
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_ini.minuto = (int)uchaux;
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_fim.hora = (int)uchaux;
      memcpy(&(uchaux), ptrBuffer, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);
      str_patamar.hm_fim.minuto = (int)uchaux;

      for (int n = 0; n < (int)curhdr.uchNumValores; n++)
         {
         memcpy(&(fltaux), ptrBuffer, sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
         str_patamar.valor[n] = fltaux;
         }
      curva->NumeroValores = (int)curhdr.uchNumValores;
      if (!curva->SetPonto(str_patamar.hm_ini, str_patamar.hm_fim, str_patamar.valor, (int)curhdr.uchNumValores))
         {
         SNPSTRLOGLINE(strlog);
         sucesso = false;
         break;
         }
      }
   // Como se trata de uma curva que foi lida, o status não é novo, nem alterado
   curva->Status[sttNOVO]     = false;
   curva->Status[sttALTERADO] = false;

   switch ((int)curhdr.uchTipoEquipId)
      {
      case eqptoCAPACITOR :
         if (!curvas->InsereCurvaTipicaCapacitor(curva))
            {
            SNPSTRLOGLINE(strlog);
            sucesso = false;
            }
         break;

      case eqptoCARGA :
         if (classe->Tag == consIP)
            {// Iluminação pública
            if (!curvas->InsereCurvaTipicaIP(curva))
               {
               SNPSTRLOGLINE(strlog);
               sucesso = false;
               }
            break;
            }
         else
            {//curva de demanda
            if (!curvas->InsereCurvaTipicaCarga(curva))
               {
               SNPSTRLOGLINE(strlog);
               sucesso = false;
               }
            }
         break;

      case eqptoGERADOR :
         if (!curvas->InsereCurvaTipicaGeradorFotovoltaico(curva))
            {
            SNPSTRLOGLINE(strlog);
            sucesso = false;
            }
         break;

      case eqptoREATOR :
         if (!curvas->InsereCurvaTipicaReator(curva))
            {
            SNPSTRLOGLINE(strlog);
            sucesso = false;
            }
         break;

      default :
         SNPSTRLOGLINE(strlog);
         sucesso = false;
      }

   if (!sucesso)
      {
      SNPSTRLOGLINE(strlog);
      if (curva)
         {
         delete(curva);
         }
      }

   return(sucesso);
   } // DesmontaStreamCurvaTipica

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DesmontaStreamCurvaCrescimento(VTCrescimento* crescimento, TStream* ptrStream)
   {
   bool           sucesso = true;
   int            intaux;
   AnsiString     strlog;
   unsigned char  buffer[sizeof(struct strCURVACRESCIMENTO)];
   unsigned char* ptrBuffer;
   int            intSize;
   short          shtAnoIni;
   short          shtAnoFim;
   unsigned char  uchPontos;
   float          fltval;

   if (!ptrStream)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   memset(buffer, 0, sizeof(buffer));
   intSize = ptrStream->Size;
   // Verifica se não houve nenhum problema no dimensionamento
   if (intSize > (int)sizeof(buffer))
      {
      return(false);
      }
   ptrStream->Read(buffer, intSize);
   ptrBuffer = buffer;

   memcpy(&shtAnoIni, ptrBuffer, sizeof(shtAnoIni)); ptrBuffer += sizeof(shtAnoIni);
   memcpy(&shtAnoFim, ptrBuffer, sizeof(shtAnoFim)); ptrBuffer += sizeof(shtAnoFim);
   memcpy(&uchPontos, ptrBuffer, sizeof(uchPontos)); ptrBuffer += sizeof(uchPontos);
   for (int i = 0; i < uchPontos; i++)
      {
      memcpy(&fltval, ptrBuffer, sizeof(fltval)); ptrBuffer += sizeof(fltval);
      crescimento->InserePeriodo(shtAnoIni + i, shtAnoIni + i, fltval);
      }

   return(sucesso);
   } // DesmontaStreamCurvaCrescimento(TStream* ptrStream, VTCrescimento* crescimento)

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::MontaStreamCurvaCrescimento(TStream* stream, VTCrescimento* crescimento)
   {
   bool           bolret = true;
   short          shtaux;
   unsigned char  uchaux;
   float          fltaux;
   unsigned char  buffer[sizeof(struct strCURVACRESCIMENTO)];
   unsigned char* ptrBuffer;
   int            intSize;

   if (!stream) return(false);
   if (!crescimento) return(false);
   if (crescimento->NumPeriodo > MAXCRESCPERIODO) return(false);

   // Copia os dados diretamente no buffer
   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
   shtaux = (short)crescimento->AnoInicial;
   memcpy(ptrBuffer, &shtaux, sizeof(shtaux)); ptrBuffer += sizeof(shtaux);
   shtaux = (short)crescimento->AnoFinal;
   memcpy(ptrBuffer, &shtaux, sizeof(shtaux)); ptrBuffer += sizeof(shtaux);
   uchaux = crescimento->NumPeriodo;
   memcpy(ptrBuffer, &uchaux, sizeof(uchaux)); ptrBuffer += sizeof(uchaux);

   // Loop de pontos
   for (int i = crescimento->AnoInicial; i <= crescimento->AnoFinal; i++)
      {
      fltaux = crescimento->Taxa_pu[i];
      memcpy(ptrBuffer, &fltaux, sizeof(fltaux)); ptrBuffer += sizeof(fltaux);
      }

   // Finalmente escreve na stream
   stream->Write(buffer, ptrBuffer - buffer);

   return(bolret);
   } // MontaStreamCurvaCrescimento(VTCrescimento* crescimento)

/*
//---------------------------------------------------------------------------
bool __fastcall TBDfunc::DesmontaStreamDescricao(TStream* ptrStream, AnsiString &descricao)
   {
   bool           sucesso = true;      //pesadelo...
   int            intaux;
   AnsiString     strlog;
   unsigned char  buffer[200];
   unsigned char* ptrBuffer;
   int            intSize;
   short          shtDes;

   if (!ptrStream)
      {
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   memset(buffer, 0, sizeof(buffer));
   intSize = ptrStream->Size;
   // Verifica se não houve nenhum problema no dimensionamento
   if (intSize > (int)sizeof(buffer))
      {
      return(false);
      }
   ptrStream->Read(buffer, intSize);
   ptrBuffer = buffer;

   memcpy(&shtDes, ptrBuffer, sizeof(shtDes));
   ptrBuffer += sizeof(shtDes);
   descricao = shtDes;

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDfunc::MontaStreamDescricao(TStream* stream, AnsiString descricao)
   {
   bool           bolret = true;        //pesadelo...
   short          shtaux;
   unsigned char  uchaux;
   float          fltaux;
   unsigned char  buffer[200];
   unsigned char* ptrBuffer;
   int            intSize;

   if (!stream) return(false);

   // Copia os dados diretamente no buffer
   memset(buffer, 0, sizeof(buffer));
   ptrBuffer = buffer;
//   shtaux = descricao;
//   memcpy(ptrBuffer, &shtaux, sizeof(shtaux)); ptrBuffer += sizeof(shtaux);
   memcpy(ptrBuffer, &descricao, sizeof(descricao)); ptrBuffer += sizeof(descricao);

   // Finalmente escreve na stream
   stream->Write(buffer, ptrBuffer - buffer);

   return(bolret);
   }
*/
//---------------------------------------------------------------------------



