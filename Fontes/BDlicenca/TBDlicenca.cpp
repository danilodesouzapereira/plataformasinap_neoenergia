//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DB.hpp>
#include "TBDlicenca.h"
#include "VTAccess.h"
#include "VTCmdSql.h"
#include "..\Cript\VTBcd.h"
#include "..\Cript\VTCript.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Micro\VTPendrive.h"
#include "..\..\DLL_Inc\Licenca.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBDlicenca* __fastcall NewObjBDlicenca(void)
   {
   try{//cria objeto
      return(new TBDlicenca());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TBDlicenca::TBDlicenca(void)
   {
   try{//cria objetos
      bcd      = NewObjBcd();
      cript    = NewObjCript();
      access   = NewObjAccess();
      cmd_sql  = NewObjCmdSqlAccess();
      licenca  = DLL_NewObjLicenca();
      }catch(Exception &e)
         {
         }
   //inicia dados
   cript->DefineChave("Copyright Guaraldo 2006");
   }

//---------------------------------------------------------------------------
__fastcall TBDlicenca::~TBDlicenca(void)
   {
   //destrói objetos
   if (bcd)     {delete bcd;     bcd     = NULL;}
   if (cript)   {delete cript;   cript   = NULL;}
   if (access)  {delete access;  access  = NULL;}
   if (cmd_sql) {delete cmd_sql; cmd_sql = NULL;}
   if (licenca) {delete licenca; licenca = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TBDlicenca::AtualizaLicenca(VTLicenca *licenca)
   {
   //monta comando p/ atualizar Licenca na base de dado
   //OBS: somente campos que podem ser alterados
   cmd_sql->AddComando("AtualizaLicenca");
   cmd_sql->AddParametro(licenca->CPU_ID);
   cmd_sql->AddParametro(licenca->DH_alocada);
   cmd_sql->AddParametro(licenca->DH_inicio);
   cmd_sql->AddParametro(licenca->DH_termino);
   cmd_sql->AddParametro(licenca->Status);
	cmd_sql->AddParametro(licenca->Validade);
   cmd_sql->AddParametro(licenca->Renovada);
   //parâmetro p/ cláusula WHERE
   cmd_sql->AddParametro(licenca->ID);
	if (access->ExecutaCmdUpdate(cmd_sql->Text) == 0) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDlicenca::AtualizaPendrive(VTPendrive *pendrive)
   {
   //variáveis locais
   int        len1, len2;
   AnsiString pen_id, pen_id1, pen_id2;

   //verifica conexão c/ base de dados
   if (! access->Conectado()) return(false);
   //criptografa ID do pendrive
   pen_id = pendrive->Id;
   if (! Criptografa(pen_id)) return(false);
   //divide pen_id em duas partes, por problema de tamanho
   len1 = pen_id.Length() / 2;
   len2 = pen_id.Length() - len1;
   pen_id1 = pen_id.SubString(1, len1);
   pen_id2 = pen_id.SubString(len1+1, len2);
   //monta comando SQL p/ gravar tabela Pendrive
   cmd_sql->AddComando("AtualizaTabPendrive");
   cmd_sql->AddParametro(pen_id1);
   cmd_sql->AddParametro(pen_id2);
   //executa comando SQL
   return(access->ExecutaCmdUpdate(cmd_sql->Text));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDlicenca::Conecta(AnsiString database_name)
   {
   return(access->Conecta(database_name));
   }

//-----------------------------------------------------------------------------
bool __fastcall TBDlicenca::ConsultaPendrive(VTPendrive *pendrive)
   {
   //variáveis locais
   TDataSet   *dataset;
   AnsiString pen_id;

   //verifica conexão c/ base de dados
   if (! access->Conectado()) return(false);
   //monta comando SQL p/ consultar tabela Pendrive
   cmd_sql->AddComando("ConsultaTabPendrive");
   //executa comando SQL
   if ((dataset =  access->ExecutaCmdSelect(cmd_sql->Text)) == NULL) return(false);
   if (dataset->Eof) return(false);
   //lê os campos Id1 e Id2 e monta Id criptografado
   pen_id = dataset->FieldByName("Id_1")->AsString + dataset->FieldByName("Id_2")->AsString;
   //descriptografa Id do Pendrive
   if (! Descriptografa(pen_id)) return(false);
   //atualiza Pendrive
   pendrive->Id = pen_id;
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TBDlicenca::Criptografa(AnsiString &txt)
   {
   //variáveis locais
   int     len;
   struct  {char bin[512], hex[1024];} buf;

   //copia texto a ser criptografado em buf.orig sem o EOF
   len = txt.Length();
   if (len > sizeof(buf.bin)) return(false);
   memcpy(buf.bin, txt.c_str(), len);
   //criprografa buffer
   if (! cript->Executa(buf.bin, len)) return(false);
   //converte buffer criptografado de bin p/ hexa, já com EOF
   if (! bcd->BinToHexa(buf.bin, len, buf.hex, sizeof(buf.hex))) return(false);
   //atualiza txt
   txt = buf.hex;
   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall TBDlicenca::Desconecta(void)
   {
   access->Desconecta();
   }

//-----------------------------------------------------------------------------
bool __fastcall TBDlicenca::Descriptografa(AnsiString &txt)
   {
   //variáveis locais
   int     len;
   struct  {char bin[512], hex[1024];} buf;

   //copia texto (que esté em hexa) em buf.dest sem o EOF
   len = txt.Length();
   if (len > sizeof(buf.hex)) return(false);
   memcpy(buf.hex, txt.c_str(), len);
   //converte texto em buf.dest de hexa p/ bin, já com EOF
   if (! bcd->HexaToBin(buf.hex, len, buf.bin, sizeof(buf.bin))) return(false);
   //descriprografa buffer
   if (! cript->Executa(buf.bin, len/2)) return(false);
   //atualiza txt
   txt = buf.bin;
   return(true);
   }

//---------------------------------------------------------------------------
VTLicenca* __fastcall TBDlicenca::ExisteLicenca(AnsiString produto, AnsiString login, AnsiString password)
   {
   //variáveis locais
   TDataSet  *ds;

   //monta comando p/ executar consulta disponível na base de dado
   cmd_sql->AddComando("ExisteLicenca");
   cmd_sql->AddParametro(produto);
   cmd_sql->AddParametro(login);
   cmd_sql->AddParametro(password);
   //obtém dataset c/ a Licenca solicitada
   if ((ds = access->ExecutaCmdSelect(cmd_sql->Text)) == NULL) return(NULL);
   //inicia Licenca c/ os dados do DataSet
   licenca->ID          = ds->FieldByName("licenca_id")->AsInteger;
   licenca->CPU_ID      = ds->FieldByName("cpu_id")->AsString;
   licenca->Produto     = ds->FieldByName("produto_nome")->AsString;
   licenca->Login       = ds->FieldByName("login")->AsString;
   licenca->Password    = ds->FieldByName("password")->AsString;
   licenca->DH_alocada   = ds->FieldByName("dh_alocada")->AsDateTime;
   licenca->DH_inicio   = ds->FieldByName("dh_inicio")->AsDateTime;
//   licenca->DH_controle = ds->FieldByName("dh_controle")->AsDateTime;
   licenca->DH_termino  = ds->FieldByName("dh_termino")->AsDateTime;
   licenca->Status      = ds->FieldByName("status")->AsInteger;
   licenca->Tipo        = ds->FieldByName("tipo")->AsInteger;
   licenca->Validade    = ds->FieldByName("validade")->AsInteger;
   licenca->Renovada    = ds->FieldByName("renovada")->AsInteger;
   //fecha DataSet
   ds->Close();
   return(licenca);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDlicenca::InsereLicenca(VTLicenca *licenca)
   {
   //variáveis locais
   int licenca_id;
   int produto_id;
   int usuario_id;

   //determina parâmetros necessários p/ inserir Licenca
   licenca_id = TabNextId("Licenca", "licenca_id");
   produto_id = TabGetId("Produto",  "produto_id", "nome", licenca->Produto);
   usuario_id = TabGetId("Usuario",  "usuario_id", "nome", "Demo");
   //monta comando p/ executar comando disponível na base de dado
   cmd_sql->AddComando("InsereLicenca");
   //define parâmetros
   cmd_sql->AddParametro(licenca_id);
   cmd_sql->AddParametro(usuario_id);
   cmd_sql->AddParametro(licenca->CPU_ID);
   cmd_sql->AddParametro(produto_id);
   cmd_sql->AddParametro(licenca->Login);
   cmd_sql->AddParametro(licenca->Password);
   cmd_sql->AddParametro(licenca->Tipo);
   cmd_sql->AddParametro(sttLIVRE);
   cmd_sql->AddParametro(licenca->DH_alocada);
   cmd_sql->AddParametro(licenca->DH_inicio);
   cmd_sql->AddParametro(licenca->DH_termino);
   cmd_sql->AddParametro(licenca->Validade);
   cmd_sql->AddParametro(licenca->Renovada);
   //aexecuta comando p/ inserir Licenca na base de dados
   if (access->ExecutaCmdUpdate(cmd_sql->Text) == 0) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDlicenca::InsereLog(int licenca_id, int operacao_id, int resposta_id,
                                      int erro_id, TDateTime dh, AnsiString observacao)
   {
   //monta comando p/ inserir Log na base de dado
   cmd_sql->AddComando("InsereLog");
   cmd_sql->AddParametro(licenca_id);
   cmd_sql->AddParametro(operacao_id);
   cmd_sql->AddParametro(resposta_id);
   cmd_sql->AddParametro(erro_id);
   cmd_sql->AddParametro(dh);
   cmd_sql->AddParametro(observacao);
   //executa atualização da base de dados
   if (access->ExecutaCmdUpdate(cmd_sql->Text) == 0) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TBDlicenca::TabGetId(AnsiString tab_name, AnsiString campo_id,
                                    AnsiString campo_ref, AnsiString valor_ref)
   {
   //variáveis locais
   AnsiString sql;
   TDataSet  *ds;

   //monta comando p/ determinar maior valor existente no campo da tabela
   sql = "SELECT * FROM " + tab_name + " WHERE " + campo_ref  + "=" + AnsiQuotedStr(valor_ref, '\'');
   //obtém dataset
   if ((ds = access->ExecutaCmdSelect(sql)) == NULL) return(1);
   return(ds->FieldByName(campo_id)->AsInteger);
   }

//---------------------------------------------------------------------------
int __fastcall TBDlicenca::TabNextId(AnsiString tab_name, AnsiString campo)
   {
   //variáveis locais
   AnsiString sql;
   TDataSet  *ds;

   //monta comando p/ determinar maior valor existente no campo da tabela
   sql = "SELECT max(" + campo + ") as maximo FROM " + tab_name;
   //obtém dataset
   if ((ds = access->ExecutaCmdSelect(sql)) == NULL) return(1);
   return(ds->FieldByName("maximo")->AsInteger + 1);
   }

//---------------------------------------------------------------------------
//eof

 